using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Timers;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Data.SqlClient;
using Microsoft.VisualBasic;
using System.Net.Sockets;
using InTheHand;
using InTheHand.Net.Bluetooth;
using InTheHand.Net.Ports;
using InTheHand.Net.Sockets;
using InTheHand.Net.Bluetooth.AttributeIds;
using InTheHand.Net;
using InTheHand.Windows.Forms;
using System.IO.Ports;
using ZedGraph;
using System.Windows.Media.Media3D;
using System.Runtime.InteropServices;

namespace SENSE
{   
    public partial class _Form_Sense : Form
    {
        [DllImport("User32.Dll")]
        private static extern long SetCursorPos(int x, int y);

        [DllImport("user32.dll")]
        private static extern bool GetCursorPos(out Point lpPoint);
        
        [System.Runtime.InteropServices.DllImport("user32.dll")]
        private static extern void mouse_event(MouseState dwFlags, int dx, int dy, int cButtons, int dwExtraInfo);

        Thread AutoConnect;

        _Form_Visualize Visualize_WINDOW;
        _Form_Parameter Parameter_WINDOW;
        _Form_Graph Graph_WINDOW;

        private enum SERIAL_STATE
        {
            Bth_Upsupport,
            COM_None,
            Wait_ConnectClick,
            Connect_Auto,
            Connecting,
            Connected,
            Closing
        }

        struct DATA_FRAME 
        {
            public byte[] buffer;
            public bool synchronize;
        }

        private struct STREAM_DATA
        {
            public double value;
            public string name;
            public string unit;
            public Color color;
            public SymbolType Point;
            public Int32 buffer;
            public Int32 FramePos;
            public Int32 NoByte;
            public float DivBy;
            public string Format;
        }

        private struct MOUSE
        {
            public MouseState State;
            public Point cursor;
            public UInt64 tick;
        }

        enum MouseState
        {
            NONE = 0,
            ABSOLUTE = 0x8000,
            LEFTDOWN = 0x02,
            LEFTUP = 0x04,
            RIGHTDOWN = 0x08,
            RIGHTUP = 0x10,
            BOTHDOWN = 0x20,
            BOTHUP = 0x40,
            MOVE = 0x0001,
            ABS_LEFTDOWN = 0x02 | 0x8000,
            ABS_LEFTUP = 0x04 | 0x8000,
            ABS_RIGHTDOWN = 0x08 | 0x8000,
            ABS_RIGHTUP = 0x10 | 0x8000,
            ABS_BOTHDOWN = 0x20 | 0x8000,
            ABS_BOTHUP = 0x40 | 0x8000,
            ABS_MOVE = 0x0001 | 0x8000,
        };        

        public enum CMD
        {
            NONE = 0,
            Param_Request = 1,
            Param_Save = 2,
            Sense_Calib = 3,
            Algo_Tilt = 4,
            Algo_Gyro = 5,
            Algo_Comp = 6,
            Algo_AHRS = 7,
            Algo_DCMkf = 8,
            Algo_DCMkfa = 9,
        }

        private SERIAL_STATE Serial_State = SERIAL_STATE.COM_None;
        private string[] Port_Name_List = null;
        private CMD Algo = CMD.Algo_DCMkf;

        private double BAT, TEMPE;

        private int Mouse_Count;
        private UInt64 Mouse_tick;
        private int Bat_Count;

        private MOUSE Button_New, Button_Last;
        private Vector3D Mouse_Perspective;
        private Rectangle SCREEN;
        private bool MouseEnable;
        private Vector3D Mouse_LastAngle;
        Point cursor = new Point();

        private Quaternion QUATERNION;
        private Vector3D GYRO, MAGN, ACCE, EULER;
        private static double[] DCM = new double[9];
        private static bool[] Button = new bool[2];
        private double SampleTime;
        private UInt16 FrameCNT, Last_FrameCNT;
        private uint ConnectionLost;

        private byte[] protocol = System.Text.Encoding.Default.GetBytes("VND");
        private static uint Protocol_StreamSize = 100;
        private static uint Protocol_ConsoleSize = 100;
        private static uint Protocol_ParameterSize = 11;
        private UInt16 FRAME_Pos = 0;

        private static DATA_FRAME Data_Frame = new DATA_FRAME { buffer = new byte[Protocol_StreamSize], synchronize = false };
        private static DATA_FRAME Console_Frame = new DATA_FRAME { buffer = new byte[Protocol_ConsoleSize], synchronize = false };
        private static DATA_FRAME Prameter_Frame = new DATA_FRAME { buffer = new byte[Protocol_ParameterSize], synchronize = false };

        
        private static STREAM_DATA[] Protocol_StreamData = new STREAM_DATA[36] 
        {
            new STREAM_DATA { name = "Roll", unit = "degree",           FramePos = 4, NoByte = 2, DivBy = 100.0f,    Format = "0.0", color = Color.LightYellow, Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Pitch", unit = "degree",          FramePos = 6, NoByte = 2, DivBy = 100.0f,    Format = "0.0", color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Yaw", unit = "degree",            FramePos = 8, NoByte = 2, DivBy = 100.0f,    Format = "0.0", color = Color.LightGreen,  Point = SymbolType.None, buffer = 60000 },                    
            
            new STREAM_DATA { name = "Quaternion 1", unit = "",         FramePos = 10, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000" , color = Color.LightYellow, Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Quaternion 2", unit = "",         FramePos = 13, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000" , color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Quaternion 3", unit = "",         FramePos = 16, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000" , color = Color.LightGreen,  Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Quaternion 4", unit = "",         FramePos = 19, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000" , color = Color.LightPink,   Point = SymbolType.None, buffer = 60000 },                
            
            new STREAM_DATA { name = "dcm 0", unit = "",                FramePos = 22, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000" , color = Color.LightYellow, Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "dcm 1", unit = "",                FramePos = 25, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000" , color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "dcm 2", unit = "",                FramePos = 28, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000" , color = Color.LightGreen,  Point = SymbolType.None, buffer = 60000 },       
            new STREAM_DATA { name = "dcm 3", unit = "",                FramePos = 31, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000" , color = Color.LightYellow, Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "dcm 4", unit = "",                FramePos = 34, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000" , color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "dcm 5", unit = "",                FramePos = 37, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000" , color = Color.LightGreen,  Point = SymbolType.None, buffer = 60000 },      
            new STREAM_DATA { name = "dcm 6", unit = "",                FramePos = 40, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000" , color = Color.LightYellow, Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "dcm 7", unit = "",                FramePos = 43, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000" , color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000},
            new STREAM_DATA { name = "dcm 8", unit = "",                FramePos = 46, NoByte = 3, DivBy = 1000000.0f, Format = "0.0000000", color = Color.LightGreen,  Point = SymbolType.None, buffer = 60000 },               
            
            new STREAM_DATA { name = "Sample Time", unit = "Micro Sec", FramePos = 49, NoByte = 3, DivBy = 1.0f, Format = "0", color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000  },  
            
            new STREAM_DATA { name = "Frame Count", unit = "",          FramePos = 52, NoByte = 2, DivBy = 1.0f,       Format = "0", color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000 },
                        
            new STREAM_DATA { name = "Batery", unit = "Volt",           FramePos = 54, NoByte = 2, DivBy = 100.0f, Format = "0.00", color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000  },  
            
            new STREAM_DATA { name = "Temperature", unit = "°C",        FramePos = 56, NoByte = 2, DivBy = 100.0f, Format = "0.00", color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000  },  
            
            new STREAM_DATA { name = "Mouse", unit = "",                FramePos = 58, NoByte = 1, DivBy = 1.0f, Format = "0", color = Color.LightYellow, Point = SymbolType.None, buffer = 60000 },
        
            new STREAM_DATA { name = "Gyrometer X", unit = "degree/s",  FramePos = 59, NoByte = 2, DivBy = 10.0f,       Format = "0.00", color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Gyrometer Y", unit = "degree/s",  FramePos = 61, NoByte = 2, DivBy = 10.0f,       Format = "0.00", color = Color.LightYellow, Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Gyrometer Z", unit = "degree/s",  FramePos = 63, NoByte = 2, DivBy = 10.0f,      Format = "0.00", color = Color.LightGreen,  Point = SymbolType.None, buffer = 60000 },   

            new STREAM_DATA { name = "Magnetometer X", unit = "scale",  FramePos = 65, NoByte = 2, DivBy = 10.0f,      Format = "0.00", color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Magnetometer Y", unit = "scale",  FramePos = 67, NoByte = 2, DivBy = 10.0f,      Format = "0.00", color = Color.LightYellow, Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Magnetometer Z", unit = "scale",  FramePos = 69, NoByte = 2, DivBy = 10.0f,      Format = "0.00", color = Color.LightGreen,  Point = SymbolType.None, buffer = 60000 },                   
            
            new STREAM_DATA { name = "Accelerometer X", unit = "",      FramePos = 71, NoByte = 2, DivBy = 10.0f,    Format = "0.0", color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Accelerometer Y", unit = "",      FramePos = 73, NoByte = 2, DivBy = 10.0f,    Format = "0.0", color = Color.LightYellow, Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Accelerometer Z", unit = "",      FramePos = 75, NoByte = 2, DivBy = 10.0f,    Format = "0.0", color = Color.LightGreen,  Point = SymbolType.None, buffer = 60000 },   
            
            new STREAM_DATA { name = "Velocity X ", unit = "cm/s",      FramePos = 77, NoByte = 3, DivBy = 10000.0f,    Format = "0.0000", color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Velocity Y ", unit = "cm/s",      FramePos = 80, NoByte = 3, DivBy = 10000.0f,    Format = "0.0000", color = Color.LightYellow, Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Velocity Z ", unit = "cm/s",      FramePos = 83, NoByte = 3, DivBy = 10000.0f,    Format = "0.0000", color = Color.LightGreen,  Point = SymbolType.None, buffer = 60000 },                   
            
            new STREAM_DATA { name = "Position X", unit = "cm",         FramePos = 86, NoByte = 3, DivBy = 10000.0f,    Format = "0.0000", color = Color.LightBlue,   Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Position Y", unit = "cm",         FramePos = 89, NoByte = 3, DivBy = 10000.0f,    Format = "0.0000", color = Color.LightYellow, Point = SymbolType.None, buffer = 60000 },
            new STREAM_DATA { name = "Position Z", unit = "cm",         FramePos = 92, NoByte = 3, DivBy = 10000.0f,    Format = "0.0000", color = Color.LightGreen,  Point = SymbolType.None, buffer = 60000 },  
        };


#region Callback

        private delegate void Form_Callback(SERIAL_STATE State);
        private delegate void Noti_CallBack(string message);
        private delegate void Tittle_Callback(string str);
        private delegate void Console_CallBack(string message);

        private void Form_Update(SERIAL_STATE State) { this.Invoke(new Form_Callback(this.SenseForm_Process), new object[] { State }); }
        private void Notification_Update(string text) { this.Invoke(new Noti_CallBack(this.Noti_Write), new object[] { text }); }
        private void Tittle_Update(string str) { this.Invoke(new Tittle_Callback(this.SENSE_Tittle_process), new object[] { str }); }
        private void Console_Update(string text)  {this.Invoke(new Console_CallBack(this.Console_Write), new object[] { text });}

        private void Console_Write(string message)
        {
            Textbox_Console.AppendText(message);
        }

        private void Noti_Write(string message)
        {
            Noti.Visible = false;
            Noti.Visible = true;
            Noti.BalloonTipText = message;
            Noti.ShowBalloonTip(5000);
        }

        private void SENSE_Tittle_process(string str)
        {
            this.Text = "The SENSE - " + str;
        }

#endregion


#region SENSE

        public _Form_Sense()
            {
                InitializeComponent();
            }

        private void SENSE_Load(object sender, EventArgs e)
        {
            TIMER.Start();

            List_BAUD.Text      = List_BAUD.Items[5].ToString();
            List_DATAbits.Text  = List_DATAbits.Items[1].ToString();
            List_Stopbits.Text  = List_Stopbits.Items[0].ToString();
            List_Parity.Text    = List_Parity.Items[0].ToString();

            AutoConnect = new System.Threading.Thread(new System.Threading.ThreadStart(AutoConnect_Process));
            AutoConnect.Start();
        }

        private void SENSE_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (Serial_COM.IsOpen) Serial_COM.Close();        
            TIMER.Enabled = false;        
        }
        
        private bool SENSE_SizeProcess()
        {
            int X = this.Location.X;
            int Y = this.Location.Y + this.Height;
            int Width = this.Width;

            this.MaximumSize = new Size(this.MinimumSize.Width, Screen.PrimaryScreen.WorkingArea.Height*2/3);

            if (Parameter_WINDOW != null)
            {
                int resultY = this.Location.Y;
                int resultX = this.Location.X + this.Width;

                //Parameter_WINDOW.MaximumSize = new Size(this.MinimumSize.Height, Screen.PrimaryScreen.WorkingArea.Height / 2);
                //Parameter_WINDOW.MinimumSize = new Size(this.MinimumSize.Height, 0);
                Parameter_WINDOW.Location = new Point(resultX, resultY);
                Parameter_WINDOW.Height = this.Height;
                Width = Parameter_WINDOW.Location.X + Parameter_WINDOW.Width - this.Location.X;
            }
            if (Visualize_WINDOW != null)
            {
                Visualize_WINDOW.Height = this.Height;
                Visualize_WINDOW.Width = this.Height;
                int resultY = this.Location.Y;
                int resultX = this.Location.X - Visualize_WINDOW.Width;
                Visualize_WINDOW.Location = new Point(resultX, resultY);

                X = Visualize_WINDOW.Location.X;
                Width += Visualize_WINDOW.Location.X + Visualize_WINDOW.Width - X;
            }
            if (Graph_WINDOW != null)
            {
                int resultY = this.Location.Y + this.Height;
                int resultX = X;

                Graph_WINDOW.Location = new Point(resultX, resultY);
                Graph_WINDOW.Height = Screen.PrimaryScreen.WorkingArea.Height - Graph_WINDOW.Location.Y;
                Graph_WINDOW.Width = Width;
            }
            return true;
        }
        
        private void Noti_Click(object sender, EventArgs e)
        {
            ShowInTaskbar = true;
            Noti.Visible = false;
            WindowState = FormWindowState.Normal;
        }

        private void Connection_Process()
        {
            Form_Update(Serial_process(Serial_State));

            if (Serial_State == SERIAL_STATE.Connect_Auto)
                Form_Update(Serial_process(Serial_State));

            if (Serial_State == SERIAL_STATE.Connecting)
                Form_Update(Serial_process(Serial_State));
        }

        private void AutoConnect_Process()
        {
            Tittle_Update("Waiting for SENSE...");
            while (Serial_State != SERIAL_STATE.Connected) Connection_Process();
            ConnectionLost = 0;
        }

        private void SENSE_SizeEvent(object sender, EventArgs e)
        {

            if (this.WindowState == FormWindowState.Minimized)
            {
                Notification_Update("Hide to system tray");
                this.ShowInTaskbar = false;
            }
            else if (this.WindowState == FormWindowState.Maximized)
            {
                SENSE_SizeProcess();
            }
            else if (this.WindowState == FormWindowState.Normal)
            {
                SENSE_SizeProcess();
                SCREEN = Screen.PrimaryScreen.Bounds;
                if (SCREEN.Height > SCREEN.Width)
                {
                    Mouse_Perspective.X = (double)num_mouse_pers.Value;
                    Mouse_Perspective.Y = (double)num_mouse_pers.Value * SCREEN.Height / SCREEN.Width;
                }
                else
                {
                    Mouse_Perspective.Y = (double)num_mouse_pers.Value;
                    Mouse_Perspective.X = (double)num_mouse_pers.Value * SCREEN.Width / SCREEN.Height;
                }
            }
        }
       
        private void Console_doubleclick(object sender, EventArgs e)
        {
            Textbox_Console.Clear();
        }

        private void SenseForm_Process(SERIAL_STATE State)
        {
            switch (State)
            {
                case SERIAL_STATE.COM_None:

                    button_Serial_Scan.Text = "Scan";
                    List_PORT.Enabled = false;
                    List_BAUD.Enabled = false;
                    List_DATAbits.Enabled = false;
                    List_Stopbits.Enabled = false;
                    List_Parity.Enabled = false;
                    button_Serial_Scan.Enabled = true;
                    Button_Data.Enabled = false;
                    Button_Parameter.Enabled = false;
                    Button_Visualize.Enabled = false;
                    Button_Calib.Enabled = false;
                    Serial_State = State;
                    break;

                case SERIAL_STATE.Bth_Upsupport:

                    button_Serial_Scan.Text = "Check Support";
                    List_PORT.Enabled = false;
                    List_BAUD.Enabled = false;
                    List_DATAbits.Enabled = false;
                    List_Stopbits.Enabled = false;
                    List_Parity.Enabled = false;
                    button_Serial_Scan.Enabled = true;
                    Button_Data.Enabled = false;
                    Button_Parameter.Enabled = false;
                    Button_Visualize.Enabled = false;
                    Button_Calib.Enabled = false;
                    Serial_State = State;
                    break;

                case SERIAL_STATE.Wait_ConnectClick:

                    button_Serial_Scan.Text = "Connect";
                    List_PORT.Enabled = true;
                    List_BAUD.Enabled = true;
                    List_DATAbits.Enabled = true;
                    List_Stopbits.Enabled = true;
                    List_Parity.Enabled = true;
                    button_Serial_Scan.Enabled = true;
                    List_PORT.Text = Port_Name_List[0];
                    Button_Data.Enabled = false;
                    Button_Parameter.Enabled = false;
                    Button_Visualize.Enabled = false;
                    Button_Calib.Enabled = false;

                    int index = -1;
                    string ComPortName = null;
                    List_PORT.Items.Clear();
                    do
                    {
                        index += 1;
                        List_PORT.Items.Add(Port_Name_List[index]);
                    }
                    while (!((Port_Name_List[index] == ComPortName) || (index == Port_Name_List.GetUpperBound(0))));

                    Array.Sort(Port_Name_List);
                                        
                    Serial_COM.PortName = Convert.ToString(List_PORT.Text);
                    Serial_COM.BaudRate = Convert.ToInt32(List_BAUD.Text);
                    Serial_COM.DataBits = Convert.ToInt16(List_DATAbits.Text);
                    Serial_COM.StopBits = (StopBits)Enum.Parse(typeof(StopBits), List_Stopbits.Text);
                    Serial_COM.Parity = (Parity)Enum.Parse(typeof(Parity), List_Parity.Text);
                    Serial_COM.Encoding = Encoding.Default;                    

                    Serial_State = State;
                    break;
                case SERIAL_STATE.Connect_Auto:

                    button_Serial_Scan.Text = "Connect";
                    List_PORT.Enabled = true;
                    List_BAUD.Enabled = true;
                    List_DATAbits.Enabled = true;
                    List_Stopbits.Enabled = true;
                    List_Parity.Enabled = true;
                    button_Serial_Scan.Enabled = true;
                    List_PORT.Text = Port_Name_List[0];
                    Button_Data.Enabled = false;
                    Button_Parameter.Enabled = false;
                    Button_Visualize.Enabled = false;
                    Button_Calib.Enabled = false;

                    List_PORT.Items.Clear();
                    List_PORT.Items.Add(Port_Name_List[0]);

                    Serial_COM.PortName = Convert.ToString(List_PORT.Text);
                    Serial_COM.BaudRate = Convert.ToInt32(List_BAUD.Text);
                    Serial_COM.DataBits = Convert.ToInt16(List_DATAbits.Text);
                    Serial_COM.StopBits = (StopBits)Enum.Parse(typeof(StopBits), List_Stopbits.Text);
                    Serial_COM.Parity = (Parity)Enum.Parse(typeof(Parity), List_Parity.Text);
                    Serial_COM.Encoding = Encoding.Default;

                    Serial_State = State;
                    break;

                case SERIAL_STATE.Connecting:

                    button_Serial_Scan.Text = "Connecting...";
                    List_PORT.Enabled = false;
                    List_BAUD.Enabled = false;
                    List_DATAbits.Enabled = false;
                    List_Stopbits.Enabled = false;
                    List_Parity.Enabled = false;
                    button_Serial_Scan.Enabled = false;
                    Button_Data.Enabled = false;
                    Button_Parameter.Enabled = false;
                    Button_Visualize.Enabled = false;
                    Button_Calib.Enabled = false;
                    Serial_State = State;
                    break;

                case SERIAL_STATE.Connected:
                    button_Serial_Scan.Text = "Disconnect";
                    List_PORT.Enabled = false;
                    List_BAUD.Enabled = false;
                    List_DATAbits.Enabled = false;
                    List_Stopbits.Enabled = false;
                    List_Parity.Enabled = false;
                    button_Serial_Scan.Enabled = true;
                    Button_Data.Enabled = true;
                    Button_Parameter.Enabled = true;
                    Button_Visualize.Enabled = true;
                    Button_Calib.Enabled = true;
                    Serial_State = State;
                    break;

                case SERIAL_STATE.Closing:
                    button_Serial_Scan.Text = "Disonnecting...";
                    List_PORT.Enabled = false;
                    List_BAUD.Enabled = false;
                    List_DATAbits.Enabled = false;
                    List_Stopbits.Enabled = false;
                    List_Parity.Enabled = false;
                    button_Serial_Scan.Enabled = false;
                    Button_Data.Enabled = false;
                    Button_Parameter.Enabled = false;
                    Button_Visualize.Enabled = false;
                    Button_Calib.Enabled = false;
                    Serial_State = State;
                    break;
            }
        }

        private void Algorithm_Change(object sender, EventArgs e)
            {
                switch (List_Algorithm.Text)
                {
                    case "Tilt Compensation":
                        Algo = CMD.Algo_Tilt;
                        break;
                    case "Gyro Integration":
                        Algo = CMD.Algo_Gyro;
                        break;
                    case "Complementary":
                        Algo = CMD.Algo_Comp;
                        break;
                    case "AHRS":
                        Algo = CMD.Algo_AHRS;
                        break;
                    case "DCM-based KF":
                        Algo = CMD.Algo_DCMkf;
                        break;
                    case "DCM-based KF*":
                        Algo = CMD.Algo_DCMkfa;
                        break;
                    default:
                        break;
                }
                Serial_SendCMD(Algo);
            }

        private void Calib_click(object sender, EventArgs e)
        {
            MsgBoxResult Res = Interaction.MsgBox("Do you want to go into Calibration mode?", MsgBoxStyle.YesNo, "Calibration");
            if (Res == MsgBoxResult.Yes)
            {
                this.Location = new Point(this.Location.X, 0);
                this.Size = new System.Drawing.Size(this.MaximumSize.Width, this.MaximumSize.Height);
                Serial_SendCMD(CMD.Sense_Calib);
            }
        }

#endregion
        
                
#region Graph

        private void Graph_WINDOWClose(object sender, FormClosedEventArgs e)
        {
            Graph_WINDOW = null;
        }

        private void Data_Click(object sender, EventArgs e)
        {
            SENSE_SizeProcess();
            if (Graph_WINDOW == null)
            {

                this.Location = new Point(this.Location.X, 0);
                string[] name = new string[Protocol_StreamData.Length];
                string[] unit = new string[Protocol_StreamData.Length];
                Color[] color = new Color[Protocol_StreamData.Length];
                SymbolType[] Point = new SymbolType[Protocol_StreamData.Length];
                Int32[] buffer = new Int32[Protocol_StreamData.Length];
                string[] Format = new string[Protocol_StreamData.Length];

                for (int cnt = 0; cnt < Protocol_StreamData.Length; cnt++)
                {
                    name[cnt] = Protocol_StreamData[cnt].name;
                    unit[cnt] = Protocol_StreamData[cnt].unit;
                    color[cnt] = Protocol_StreamData[cnt].color;
                    Point[cnt] = Protocol_StreamData[cnt].Point;
                    buffer[cnt] = Protocol_StreamData[cnt].buffer;
                    Format[cnt] = Protocol_StreamData[cnt].Format;
                }

                Graph_WINDOW = new _Form_Graph(name, unit, color, Point, buffer, Format);
                Graph_WINDOW.FormClosed += Graph_WINDOWClose;
            }
            SENSE_SizeProcess();
            if (!Graph_WINDOW.Visible) Graph_WINDOW.Show(this);
            if (!Graph_WINDOW.Focused) Graph_WINDOW.Focus();
        }

#endregion

        
#region Visualize

        private void VISUALIZE_WindowClose(object sender, FormClosedEventArgs e)
        {
            Visualize_WINDOW = null;
        }

        private void Visualize_Click(object sender, EventArgs e)
        {
            if (Visualize_WINDOW == null)
            {
                Visualize_WINDOW = new _Form_Visualize();
                Visualize_WINDOW.FormClosed += VISUALIZE_WindowClose;
            }
            SENSE_SizeProcess();
            if (!Visualize_WINDOW.Visible) Visualize_WINDOW.Show(this);
            if (!Visualize_WINDOW.Focused) Visualize_WINDOW.Focus();
        }
        
#endregion


#region Parameter

        private void Parameter_Mornitor()
        {
            while (Parameter_WINDOW != null)
            {
                string NEW = Parameter_WINDOW.Parameter_NewUpdate();
                if (NEW != "") Serial_Send(NEW);
                try { Serial_SendCMD((CMD)Parameter_WINDOW.Parameter_CMD()); }
                catch { }
            }
        }

        private void Parameter_Click(object sender, EventArgs e)
        {
            SENSE_SizeProcess();
            if (Parameter_WINDOW == null)
            {
                Parameter_WINDOW = new _Form_Parameter();
                Serial_SendCMD(CMD.Param_Request);
                Parameter_WINDOW.FormClosed += Parameter_WINDOWClose;
            }
            SENSE_SizeProcess();
            if (Parameter_WINDOW.Visible == false)
            {
                if (!Parameter_WINDOW.Focused) Parameter_WINDOW.Focus();
                if (!Parameter_WINDOW.Visible)
                {
                    Parameter_WINDOW.Show(this);
                    Thread myThread = new System.Threading.Thread(new System.Threading.ThreadStart(Parameter_Mornitor));
                    myThread.Start();
                }
            }
        }

        private void Parameter_WINDOWClose(object sender, FormClosedEventArgs e)
        {
            Parameter_WINDOW = null;
        }

#endregion


#region Serial
        
        private SERIAL_STATE Serial_process(SERIAL_STATE state)
        {
            MouseEnable = false;
            switch (state)
            {
                case SERIAL_STATE.Bth_Upsupport:

                    return state;
                                        
                case SERIAL_STATE.COM_None:

                    Form_Update(SERIAL_STATE.COM_None);      
                    if (!BluetoothRadio.IsSupported)
                    {
                        Interaction.MsgBox("Bluetooth hardware not found, make sure that it is turned on!", MsgBoxStyle.OkOnly, "Bluetooth hardware");
                        return  SERIAL_STATE.Bth_Upsupport;
                    }
                    else
                    {
                        try { Port_Name_List = SerialPort.GetPortNames();}
                            catch { return SERIAL_STATE.COM_None; }

                        if (Port_Name_List.Length == 1) return SERIAL_STATE.Connect_Auto;
                        else if (Port_Name_List.Length > 1)
                        {
                            Notification_Update(Port_Name_List.Length.ToString() + " ports found, choose to connect!");
                            return SERIAL_STATE.Wait_ConnectClick;
                        }
                        else
                        {                            
                            Interaction.MsgBox("No Serial Port found, this is because SENSE is not paired as Bluetooth device.", MsgBoxStyle.OkOnly, "Bluetooth");

                            return SERIAL_STATE.COM_None;
                        }
                    }

                case SERIAL_STATE.Wait_ConnectClick:

                    return SERIAL_STATE.Connecting;

                case SERIAL_STATE.Connect_Auto:

                    return SERIAL_STATE.Connecting;

                case SERIAL_STATE.Connecting:  
                    Serial_State = Serial_Open();
                    if (Serial_State == SERIAL_STATE.Connected)
                    {
                        Notification_Update("Serial Link: Connected");

                        Serial_SendCMD(Algo);
                    }
                    return Serial_State;
                    
                case SERIAL_STATE.Connected:

                    Form_Update(SERIAL_STATE.Closing);
                    Form_Update(Serial_Close());

                    if (Serial_State == SERIAL_STATE.COM_None)
                    {
                        //Notification_Update("Serial Link: Disonnected");
                        return Serial_State;
                    }
                    break;

                default:

                    return Serial_State;
            }
            return Serial_State;
        }

        private void Serial_Click(object sender, EventArgs e)
        {
            if (!AutoConnect.IsAlive)
            {
                AutoConnect = new System.Threading.Thread(new System.Threading.ThreadStart(Connection_Process));
                AutoConnect.Start();
            }

        }

        private SERIAL_STATE Serial_Open()
        {
            try
            {
                Serial_COM.Open();
                if (Serial_COM.IsOpen)  return SERIAL_STATE.Connected;
            }
            catch {}

            return SERIAL_STATE.COM_None;
        }

        private SERIAL_STATE Serial_Close()
        {
            try
            {
                Serial_COM.Close();
                if (!Serial_COM.IsOpen)                    return SERIAL_STATE.COM_None;
            }
            catch {}

            return SERIAL_STATE.COM_None;
        }

        private void Serial_SendCMD(CMD CMD)
        {
            if (CMD != CMD.NONE)
            {
                byte[] Seiral_Output = new byte[5];
                int cnt = 0;

                Seiral_Output[cnt++] = (Byte)'V';
                Seiral_Output[cnt++] = (Byte)'N';
                Seiral_Output[cnt++] = (Byte)'D';
                Seiral_Output[cnt++] = (Byte)'_';
                Seiral_Output[cnt++] = (Byte)CMD;

                Serial_Send(System.Text.Encoding.Default.GetString(Seiral_Output));
            }
        }

        public void Serial_Send(string message)
        {
            if (Serial_COM.IsOpen)
            {
                try
                {
                    Serial_COM.Write(message);
                }
                catch
                {
                    Notification_Update("Serial Link: Fail to Send");
                }
            }
        }
        
        private double Serial_FrameAlign(int addr, int size)
        {
            if (size == 1)
            {
                return Convert.ToDouble((sbyte)Data_Frame.buffer[addr]);
            }
            else if (size == 2)
            {
                UInt16 data = 0;
                data |= (UInt16)((Data_Frame.buffer[addr] << 8));
                data |= (UInt16)(Data_Frame.buffer[addr + 1]);
                return Convert.ToDouble((Int16)data);
            }
            else
            {
                UInt32 data = 0;
                data |= (UInt32)((Data_Frame.buffer[addr + 0] << 16) & 0x00FF0000);
                data |= (UInt32)((Data_Frame.buffer[addr + 1] << 8) & 0x0000FF00);
                data |= (UInt32)((Data_Frame.buffer[addr + 2]) & 0x000000FF);
                if ((data & 0x00800000) == 0x00800000)
                    data |= 0xFF000000;
                return Convert.ToDouble((Int32)data);
            }
        }

        private void Serial_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            string Serial_Input = Serial_COM.ReadExisting();

            byte[] raw = new byte[Serial_Input.Length];
            raw = System.Text.Encoding.Default.GetBytes(Serial_Input);

            for (int i = 0; i < raw.Length; i++)
            {
                if (FRAME_Pos < protocol.Length)   //checking header
                {
                    if (protocol[FRAME_Pos] == raw[i])
                    {
                        FRAME_Pos++;
                    }
                    else
                    {
                        FRAME_Pos = 0;
                    }
                }
                else if (FRAME_Pos == protocol.Length) //Cheking Indicator 
                {
                    switch (raw[i])
                    {
                        case (byte)'_':
                            Data_Frame.synchronize = true;
                            Console_Frame.synchronize = false;
                            Prameter_Frame.synchronize = false;
                            break;
                        case (byte)'*':
                            Data_Frame.synchronize = false;
                            Console_Frame.synchronize = true;
                            Prameter_Frame.synchronize = false;
                            break;
                        case (byte)'$':
                            Data_Frame.synchronize = false;
                            Console_Frame.synchronize = false;
                            Prameter_Frame.synchronize = true;
                            break;
                        default:
                            FRAME_Pos = 0;
                            Data_Frame.synchronize = false;
                            Console_Frame.synchronize = false;
                            Prameter_Frame.synchronize = false;
                            break;
                    }
                    FRAME_Pos++;
                }
                else
                {
                    if (Data_Frame.synchronize == true)
                    {
                        Data_Frame.buffer[FRAME_Pos++] = raw[i];
                        if (FRAME_Pos >= Protocol_StreamSize)
                        {
                            FRAME_Pos = 0;
                            for (int cnt = 0; cnt < Protocol_StreamData.Length; cnt++)
                                Protocol_StreamData[cnt].value = Serial_FrameAlign(Protocol_StreamData[cnt].FramePos, Protocol_StreamData[cnt].NoByte) / Protocol_StreamData[cnt].DivBy;
                            Data_Frame.synchronize = false;
                        }
                    }
                    else if (Console_Frame.synchronize == true)
                    {
                        Console_Frame.buffer[FRAME_Pos++] = raw[i];
                        if (FRAME_Pos == Protocol_ConsoleSize)
                        {
                            FRAME_Pos = 0;
                            Console_Frame.synchronize = false;
                            string console = System.Text.Encoding.ASCII.GetString(Console_Frame.buffer, 4, Console_Frame.buffer.Length - 4);
                            Console_Update(Environment.NewLine + console.Replace("\n", Environment.NewLine));
                        }
                    }
                    else if (Prameter_Frame.synchronize == true)
                    {
                        Prameter_Frame.buffer[FRAME_Pos++] = raw[i];
                        if (FRAME_Pos >= Protocol_ParameterSize)
                        {
                            if(Parameter_WINDOW!= null)
                                try { Parameter_WINDOW.PARAM_Read(Prameter_Frame.buffer); }   catch { }
                            FRAME_Pos = 0;
                            Prameter_Frame.synchronize = false;
                        }
                    }
                    else
                    {
                        //Notification_Update("DATA: Sync error");
                    }
                }
            }

        }

#endregion


#region Mouse

        private bool Mouse_filter(Vector3D New, Vector3D last)
        {
            double Y2 = (New.Y - last.Y) * (New.Y - last.Y);
            double Z2 = (New.Z - last.Z) * (New.Z - last.Z);

            return (Math.Sqrt(Y2 + Z2) > (double)Num_mouse_min.Value);
        }

        private Vector3D Mouse_AngleDiff(Vector3D New, Vector3D last)
        {
            double Zplus = New.Z + 360 - last.Z;
            double Zminu = New.Z - 360 - last.Z;
            double Z = New.Z - last.Z;

            double Yplus = New.Y + 360 - last.Y;
            double Yminu = New.Y - 360 - last.Y;
            double Y = New.Y - last.Y;

            if (Math.Abs(Z) > Math.Abs(Zminu)) Z = Zminu;
            if (Math.Abs(Z) > Math.Abs(Zplus)) Z = Zplus;
            if (Math.Abs(Y) > Math.Abs(Yminu)) Y = Zminu;
            if (Math.Abs(Y) > Math.Abs(Yplus)) Y = Zplus;

            return new Vector3D(0,Y,Z);
        }

        private void Mouse_Update_Event(object sender, EventArgs e)
        {
            SCREEN = Screen.PrimaryScreen.Bounds;
            if (SCREEN.Height > SCREEN.Width)
            {
                Mouse_Perspective.X = (double)num_mouse_pers.Value;
                Mouse_Perspective.Y = (double)num_mouse_pers.Value * SCREEN.Height / SCREEN.Width;
            }
            else
            {
                Mouse_Perspective.Y = (double)num_mouse_pers.Value;
                Mouse_Perspective.X = (double)num_mouse_pers.Value * SCREEN.Width / SCREEN.Height;
            }
        }

        private void Mouse_Process()
        {
            if (Button[0] == true && Button[1] == true)         Button_New.State = MouseState.BOTHDOWN;
            else if (Button[0] == false && Button[1] == false)  Button_New.State = MouseState.BOTHUP;
            else if (Button[0] == false && Button[1] == true)   Button_New.State = MouseState.RIGHTDOWN;
            else if (Button[0] == true && Button[1] == false)   Button_New.State = MouseState.LEFTDOWN;

            //enable and center

            if (Button_New.State == MouseState.BOTHDOWN)
            {
                if (Mouse_Count >= 0)
                {
                    Mouse_Count += TIMER.Interval;
                    if (Mouse_Count > 1000)
                    {
                        Mouse_Count = -1;
                        MouseEnable = !MouseEnable;
                        if (MouseEnable) Notification_Update("Mouse Enabled!");
                        else Notification_Update("Mouse Disabled!");
                    }
                }
            }
            else Mouse_Count = 0;

            //cursor get
            if (MouseEnable)
            {
                Mouse_tick += (ulong)TIMER.Interval;
                if (Mouse_Count > TIMER.Interval)  //center
                {
                    cursor = new Point(SCREEN.Width / 2, SCREEN.Height / 2);
                    Mouse_LastAngle = EULER;
                    SetCursorPos(Convert.ToInt16(cursor.X), Convert.ToInt16(cursor.Y));
                }
                else
                {
                    if (Mouse_filter(EULER, Mouse_LastAngle))
                    {
                        Vector3D Angle_diff = Mouse_AngleDiff(EULER, Mouse_LastAngle);
                        Point Mouse_Move = new Point((int)(-Angle_diff.Z / (Mouse_Perspective.X / 2) * SCREEN.Width / 2), (int)(-Angle_diff.Y / (Mouse_Perspective.Y / 2) * SCREEN.Height / 2));
                        cursor = new Point(cursor.X + Mouse_Move.X, cursor.Y + Mouse_Move.Y);
                        Mouse_LastAngle = EULER;
                        SetCursorPos(Convert.ToInt16(cursor.X), Convert.ToInt16(cursor.Y));
                    }
                }

                if (Button_New.State != Button_Last.State)
                {
                    Button_New.cursor = cursor;
                    Button_New.tick = Mouse_tick;

                    switch (Button_New.State)
                    {
                        case MouseState.BOTHDOWN:
                            SetCursorPos(Convert.ToInt16(Button_Last.cursor.X), Convert.ToInt16(Button_Last.cursor.Y));
                            if (Button_Last.State == MouseState.LEFTDOWN) mouse_event(MouseState.LEFTUP, 0, 0, 0, 0);
                            if (Button_Last.State == MouseState.RIGHTDOWN) mouse_event(MouseState.RIGHTUP, 0, 0, 0, 0);
                            SetCursorPos(Convert.ToInt16(Button_New.cursor.X), Convert.ToInt16(Button_New.cursor.Y));
                            break;
                        case MouseState.BOTHUP:
                            SetCursorPos(Convert.ToInt16(Button_Last.cursor.X), Convert.ToInt16(Button_Last.cursor.Y));
                            if (Button_Last.State == MouseState.LEFTDOWN) mouse_event(MouseState.LEFTUP, 0, 0, 0, 0);
                            if (Button_Last.State == MouseState.RIGHTDOWN) mouse_event(MouseState.RIGHTUP, 0, 0, 0, 0);
                            SetCursorPos(Convert.ToInt16(Button_New.cursor.X), Convert.ToInt16(Button_New.cursor.Y));
                            break;
                        case MouseState.LEFTDOWN:
                            SetCursorPos(Convert.ToInt16(Button_Last.cursor.X), Convert.ToInt16(Button_Last.cursor.Y));
                            if (Button_Last.State == MouseState.BOTHDOWN) mouse_event(MouseState.BOTHUP, 0, 0, 0, 0);
                            if (Button_Last.State == MouseState.RIGHTDOWN) mouse_event(MouseState.RIGHTUP, 0, 0, 0, 0);
                            SetCursorPos(Convert.ToInt16(Button_New.cursor.X), Convert.ToInt16(Button_New.cursor.Y));
                            mouse_event(Button_New.State, 0, 0, 0, 0);
                            break;
                        case MouseState.RIGHTDOWN:
                            SetCursorPos(Convert.ToInt16(Button_Last.cursor.X), Convert.ToInt16(Button_Last.cursor.Y));
                            if (Button_Last.State == MouseState.LEFTDOWN) mouse_event(MouseState.LEFTUP, 0, 0, 0, 0);
                            if (Button_Last.State == MouseState.BOTHDOWN) mouse_event(MouseState.BOTHUP, 0, 0, 0, 0);
                            SetCursorPos(Convert.ToInt16(Button_New.cursor.X), Convert.ToInt16(Button_New.cursor.Y));
                            mouse_event(Button_New.State, 0, 0, 0, 0);
                            break;
                    }
                }

                Button_Last = Button_New;
            }
        }

#endregion

        private void TIMER_process(object sender, EventArgs e)
        {
            if (Serial_State == SERIAL_STATE.Connected)
            {
                TIMER.Enabled = false;
                int i = 0;

                EULER.X = Protocol_StreamData[i++].value;
                EULER.Y = Protocol_StreamData[i++].value;
                EULER.Z = Protocol_StreamData[i++].value;

                QUATERNION.W = Protocol_StreamData[i++].value;
                QUATERNION.X = Protocol_StreamData[i++].value;
                QUATERNION.Y = Protocol_StreamData[i++].value;
                QUATERNION.Z = Protocol_StreamData[i++].value;

                for (int cnt = 0; cnt < 9; cnt++)
                    DCM[cnt] = Protocol_StreamData[i++].value;

                SampleTime = Protocol_StreamData[i++].value;

                FrameCNT = (UInt16)Protocol_StreamData[i++].value;

                BAT = Protocol_StreamData[i++].value;

                TEMPE = Protocol_StreamData[i++].value;
                
                Button[0] = (((UInt32)Protocol_StreamData[i].value & 1) == 1);
                Button[1] = (((UInt32)Protocol_StreamData[i++].value & 2) == 2);

                GYRO.X = Protocol_StreamData[i++].value;
                GYRO.Y = Protocol_StreamData[i++].value;
                GYRO.Z = Protocol_StreamData[i++].value;

                MAGN.X = Protocol_StreamData[i++].value;
                MAGN.Y = Protocol_StreamData[i++].value;
                MAGN.Z = Protocol_StreamData[i++].value;

                ACCE.X = Protocol_StreamData[i++].value;
                ACCE.Y = Protocol_StreamData[i++].value;
                ACCE.Z = Protocol_StreamData[i++].value;


                if (Graph_WINDOW != null)
                    if (Graph_WINDOW.Visible)
                        for (int cnt = 0; cnt < Protocol_StreamData.Length; cnt++) Graph_WINDOW.GRAPH_Update_StreamData(cnt, Protocol_StreamData[cnt].value);

                if (Visualize_WINDOW != null)
                    if (Visualize_WINDOW.Visible)
                        Visualize_WINDOW.UpdateData(DCM);

                Mouse_Process();

                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                if (BAT < (double)num_lowBat.Value) Bat_Count += TIMER.Interval;
                else Bat_Count = 0;             
                
                if (FrameCNT == Last_FrameCNT)
                {
                    ConnectionLost++;
                    if (ConnectionLost > 3000 / TIMER.Interval)                        Tittle_Update("Connection lost");
                    else
                    {
                        if ((Bat_Count > 2000 / TIMER.Interval) && MouseEnable)
                        {
                            Bat_Count = 0;
                            num_lowBat.Value -= num_lowBat.Increment;
                            Notification_Update(string.Format("Low battery: {0:f1} V", BAT));
                        }
                        else
                            Tittle_Update(string.Format("{0:f1} V - {1:f1} °C", BAT, TEMPE));
                    }
                }
                else ConnectionLost = 0;

                Last_FrameCNT = FrameCNT;

                TIMER.Enabled = true;
            }
        }


    }    
}