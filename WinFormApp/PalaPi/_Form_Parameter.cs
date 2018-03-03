using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SENSE
{
    public partial class _Form_Parameter : Form
    {
        public _Form_Parameter()
        {
            InitializeComponent();
            Parameter_Init();
        }


        internal string Parameter_NewUpdate()
        {
            string New = OUT;
            OUT = "";
            return New;
        }

        internal uint Parameter_CMD()
        {
            ParamCMD send = CMD;
            CMD = ParamCMD.NONE;
            return (uint)send;
        }

        private void Parameter_Init()
        {
            //Debug("PARAM: Init");

            // ParameterView.Columns["Value"].DefaultCellStyle.Format = "0.#";
            for (int i = 0; i < Parameter.Length; i++)
                ParameterView.Rows.Add(Parameter[i].name, "0");
            this.ParameterView.Rows[0].Visible = false;
            //Debug("PARAM: Init done");
        }

        private enum PARA_MODE { MODE0 = 3, MODE1 = 4, MODE2 = 5, MODE3 = 6, MODE4 = 7 }; //assign bufer length

        public enum ParamCMD
        {
            NONE            = 0,
            ParamRequest    = 1,
            ParamSAVE       = 2,
        };
        ParamCMD CMD;

        struct PARAMETER_DATA
        {
            public string name;
            public PARA_MODE mode;
        }

        private PARAMETER_DATA[] Parameter = new PARAMETER_DATA[29]
            {
                new PARAMETER_DATA {name = "None", mode = PARA_MODE.MODE2},
                
                new PARAMETER_DATA {name = "Gyro Bias X", mode = PARA_MODE.MODE2},
                new PARAMETER_DATA {name = "Gyro Bias Y", mode = PARA_MODE.MODE2},
                new PARAMETER_DATA {name = "Gyro Bias Z", mode = PARA_MODE.MODE2},

                new PARAMETER_DATA {name = "Magn Offset X", mode = PARA_MODE.MODE2},
                new PARAMETER_DATA {name = "Magn Offset Y", mode = PARA_MODE.MODE2},
                new PARAMETER_DATA {name = "Magn Offset Z", mode = PARA_MODE.MODE2},

                new PARAMETER_DATA {name = "Magn Gain X", mode = PARA_MODE.MODE2},
                new PARAMETER_DATA {name = "Magn Gain Y", mode = PARA_MODE.MODE2},
                new PARAMETER_DATA {name = "Magn Gain Z", mode = PARA_MODE.MODE2},

                new PARAMETER_DATA {name = "Magn Axis X", mode = PARA_MODE.MODE2},
                new PARAMETER_DATA {name = "Magn Axis Y", mode = PARA_MODE.MODE2},
                new PARAMETER_DATA {name = "Magn Axis Z", mode = PARA_MODE.MODE2},
                
                new PARAMETER_DATA {name = "Complementary X", mode = PARA_MODE.MODE1},
                new PARAMETER_DATA {name = "Complementary Y", mode = PARA_MODE.MODE1},
                new PARAMETER_DATA {name = "Complementary Z", mode = PARA_MODE.MODE1},

                new PARAMETER_DATA {name = "AHRS Kp", mode = PARA_MODE.MODE2},
                new PARAMETER_DATA {name = "AHRS Ki", mode = PARA_MODE.MODE2},
                 
                new PARAMETER_DATA {name = "KF DCM R1 ", mode = PARA_MODE.MODE3},        
                new PARAMETER_DATA {name = "KF DCM Q1 ", mode = PARA_MODE.MODE3},     
                new PARAMETER_DATA {name = "KF DCM R2 ", mode = PARA_MODE.MODE3},      
                new PARAMETER_DATA {name = "KF DCM Q2 ", mode = PARA_MODE.MODE3},   

                new PARAMETER_DATA {name = "Thrs acce", mode = PARA_MODE.MODE2},
                new PARAMETER_DATA {name = "Thrs gyro", mode = PARA_MODE.MODE2},
 
                new PARAMETER_DATA {name = "aKF DCM R1 ", mode = PARA_MODE.MODE3},        
                new PARAMETER_DATA {name = "aKF DCM Q1 ", mode = PARA_MODE.MODE3},     
                new PARAMETER_DATA {name = "aKF DCM R2 ", mode = PARA_MODE.MODE3},      
                new PARAMETER_DATA {name = "aKF DCM Q2 ", mode = PARA_MODE.MODE3},                  
                
                new PARAMETER_DATA {name = "Frequency (Hz)", mode = PARA_MODE.MODE2},
                
            };

        private void Parameter_Save_Click(object sender, EventArgs e)
        {
            for (uint member = 1; member < Parameter.Length; member++)
            {
                try
                {
                    float value = (float)Convert.ToDouble(ParameterView.Rows[(int)member].Cells["Parameter_Value"].Value);
                    if (PARAM_Write(member, value) == false)
                        member = (uint)Parameter.Length;
                }
                catch (FormatException)
                {
                    Interaction.MsgBox("Invalid input Format at " + Parameter[member].name, MsgBoxStyle.OkOnly, "Error Input");
                    member = (uint)Parameter.Length;
                }
                catch (InvalidCastException)
                {
                    Interaction.MsgBox("Invalid input Cast at " + Parameter[member].name, MsgBoxStyle.OkOnly, "Error Input");
                    member = (uint)Parameter.Length;
                }
            }

            MsgBoxResult Res = Interaction.MsgBox("Do you want to Update new parameter?", MsgBoxStyle.YesNo, "Warning");
            if (Res == MsgBoxResult.Yes)                CMD = ParamCMD.ParamSAVE;
        }
        
        private void Parameter_Refresh_Click(object sender, EventArgs e)
        {
            CMD = ParamCMD.ParamRequest;
        }

        private bool PARAM_Write(uint type, float value)
        {
            byte[] PARAM_Output = new byte[(int)PARA_MODE.MODE4];

            float PARA_Limit = 0;
            int PARA_exp = 0;
            int PARA_significand = 0;
            float PARA_temp = 0;
            int cnt = 0;
            //Convert float to struct
            if (Parameter[type].mode == PARA_MODE.MODE1) PARA_Limit = 127;
            else if (Parameter[type].mode == PARA_MODE.MODE2) PARA_Limit = 32767;
            else if (Parameter[type].mode == PARA_MODE.MODE3) PARA_Limit = 8388607;
            else if (Parameter[type].mode == PARA_MODE.MODE4) PARA_Limit = 214748364;

            if (Math.Abs(value) > PARA_Limit)
            {
                Interaction.MsgBox("Overfloat input at " + Parameter[type].name, MsgBoxStyle.OkOnly, "Error Input");
                return false;
            }
            else
            {
                if (value == 0)
                {
                    PARA_exp = 0;
                    PARA_significand = 0;
                }
                else if ((value - (int)value) != 0)
                {
                    PARA_temp = value;
                    while (Math.Abs(PARA_significand) <= PARA_Limit)
                    {
                        if (PARA_exp < 127)
                        {
                            PARA_exp--;
                            PARA_temp *= 10;
                            PARA_significand = (int)(PARA_temp / 1);
                        }
                        else // nearly 0;
                        {
                            PARA_exp = 0;
                            PARA_significand = 0;
                        }
                    }
                    while (Math.Abs(PARA_significand) > PARA_Limit)
                    {
                        if (PARA_exp < 0)
                        {
                            PARA_exp++;
                            PARA_significand /= 10;
                        }
                    }
                }
                else
                {
                    PARA_exp = 0;
                    PARA_significand = (int)value;
                }
                PARA_exp = Math.Abs(PARA_exp);

                PARAM_Output[cnt++] = (byte)Parameter[type].mode;
                PARAM_Output[cnt++] = (byte)type;
                //Convert struct to buffer 	
                switch (Parameter[type].mode)
                {
                    case PARA_MODE.MODE0:
                        break;
                    case PARA_MODE.MODE1:
                        PARAM_Output[cnt++] = (byte)(PARA_significand & 0x000000FF);
                        break;
                    case PARA_MODE.MODE2:
                        byte[] PARAM_Output2 = new byte[3];
                        PARAM_Output[cnt++] = (byte)((PARA_significand & 0x0000FF00) >> 8);
                        PARAM_Output[cnt++] = (byte)(PARA_significand & 0x000000FF);
                        break;
                    case PARA_MODE.MODE3:
                        byte[] PARAM_Output3 = new byte[4];
                        PARAM_Output[cnt++] = (byte)((PARA_significand & 0x00FF0000) >> 16);
                        PARAM_Output[cnt++] = (byte)((PARA_significand & 0x0000FF00) >> 8);
                        PARAM_Output[cnt++] = (byte)(PARA_significand & 0x000000FF);
                        break;
                    case PARA_MODE.MODE4:
                        byte[] PARAM_Output4 = new byte[5];
                        PARAM_Output[cnt++] = (byte)((PARA_significand & 0xFF000000) >> 31 >> 1);
                        PARAM_Output[cnt++] = (byte)((PARA_significand & 0x00FF0000) >> 16);
                        PARAM_Output[cnt++] = (byte)((PARA_significand & 0x0000FF00) >> 8);
                        PARAM_Output[cnt++] = (byte)(PARA_significand & 0x000000FF);
                        break;
                }
                PARAM_Output[cnt++] = (byte)(PARA_exp & 0x00FF);
            }

            Parameter_NewProcess("VND#" + System.Text.Encoding.Default.GetString(PARAM_Output));
            return true;
        }

        public double PARAM_Read(byte[] Frame)
        {
            uint PARA_exp = 0;
            float PARA_result = 0;
            uint temp = 0x00;
            int significand = 0;
            int cnt = 4;

            PARA_MODE mode = (PARA_MODE)Frame[cnt++];

            int type = Frame[cnt++];
            if (type > Parameter.Length)
            {
                //Debug("PARAM: out of range, need to check Parameter");
                return 0;
            }

            if (mode != Parameter[type].mode)
            {
                //Debug("PARAM: (" + type.ToString() + ") Mode conflict:" + ((PARA_MODE)mode).ToString() + " vs " + Parameter[type].mode.ToString());
                return 0;
            }

            switch (mode)
            {
                case PARA_MODE.MODE0:
                    break;
                case PARA_MODE.MODE1:
                    temp |= (uint)Frame[cnt++] & 0x000000FF;
                    if ((temp & 0x00000080) == 0x00000080)
                        temp |= 0xFFFFFF00;
                    significand = (int)temp;
                    break;
                case PARA_MODE.MODE2:
                    temp |= ((uint)Frame[cnt++] << 8) & 0x0000FF00;
                    temp |= (uint)Frame[cnt++] & 0x000000FF;
                    if ((temp & 0x00008000) == 0x00008000)
                        temp |= 0xFFFF0000;
                    significand = (int)temp;
                    break;
                case PARA_MODE.MODE3:
                    temp |= ((uint)Frame[cnt++] << 16) & 0x00FF0000;
                    temp |= ((uint)Frame[cnt++] << 8) & 0x0000FF00;
                    temp |= (uint)Frame[cnt++] & 0x000000FF;
                    if ((temp & 0x00800000) == 0x00800000)
                        temp |= 0xFF000000;
                    significand = (int)temp;
                    break;
                case PARA_MODE.MODE4:
                    temp |= ((uint)Frame[cnt++] << 32) & 0xFF000000;
                    temp |= ((uint)Frame[cnt++] << 16) & 0x00FF0000;
                    temp |= ((uint)Frame[cnt++] << 8) & 0x0000FF00;
                    temp |= (uint)Frame[cnt++] & 0x000000FF;
                    significand = (int)temp;
                    break;
            }
            PARA_exp = (uint)Frame[cnt++];

            //Convert struct to float
            PARA_result = (float)significand;
            while (PARA_exp-- != 0)
                PARA_result /= 10;

            this.ParameterView.Rows[type].Cells["Parameter_Value"].Value = PARA_result;

            return PARA_result;
        }
        private string OUT = "";
        private void Parameter_NewProcess(string input)
        {
            OUT+=input;
        }

    }
}
