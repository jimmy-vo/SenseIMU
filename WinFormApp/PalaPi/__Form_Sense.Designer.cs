namespace SENSE
{
    partial class _Form_Sense
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(_Form_Sense));
            this.Serial_COM = new System.IO.Ports.SerialPort(this.components);
            this.group_Conig = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.groupBox9 = new System.Windows.Forms.GroupBox();
            this.num_lowBat = new System.Windows.Forms.NumericUpDown();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.List_PORT = new System.Windows.Forms.ComboBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.List_Parity = new System.Windows.Forms.ComboBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.List_BAUD = new System.Windows.Forms.ComboBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.List_Stopbits = new System.Windows.Forms.ComboBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.List_DATAbits = new System.Windows.Forms.ComboBox();
            this.button_Serial_Scan = new System.Windows.Forms.Button();
            this.group_Console = new System.Windows.Forms.GroupBox();
            this.Textbox_Console = new System.Windows.Forms.TextBox();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.group_MOUSE = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel7 = new System.Windows.Forms.TableLayoutPanel();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.num_mouse_pers = new System.Windows.Forms.NumericUpDown();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.List_Algorithm = new System.Windows.Forms.ComboBox();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.Num_mouse_min = new System.Windows.Forms.NumericUpDown();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.Button_Data = new System.Windows.Forms.Button();
            this.Button_Parameter = new System.Windows.Forms.Button();
            this.Button_Visualize = new System.Windows.Forms.Button();
            this.Button_Calib = new System.Windows.Forms.Button();
            this.Noti = new System.Windows.Forms.NotifyIcon(this.components);
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.TIMER = new System.Windows.Forms.Timer(this.components);
            this.group_Conig.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.groupBox9.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.num_lowBat)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.group_Console.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.group_MOUSE.SuspendLayout();
            this.tableLayoutPanel7.SuspendLayout();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.num_mouse_pers)).BeginInit();
            this.groupBox8.SuspendLayout();
            this.groupBox7.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Num_mouse_min)).BeginInit();
            this.tableLayoutPanel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // Serial_COM
            // 
            this.Serial_COM.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.Serial_DataReceived);
            // 
            // group_Conig
            // 
            resources.ApplyResources(this.group_Conig, "group_Conig");
            this.group_Conig.BackColor = System.Drawing.SystemColors.Control;
            this.group_Conig.Controls.Add(this.tableLayoutPanel1);
            this.group_Conig.ForeColor = System.Drawing.SystemColors.WindowText;
            this.group_Conig.Name = "group_Conig";
            this.group_Conig.TabStop = false;
            this.toolTip.SetToolTip(this.group_Conig, resources.GetString("group_Conig.ToolTip"));
            // 
            // tableLayoutPanel1
            // 
            resources.ApplyResources(this.tableLayoutPanel1, "tableLayoutPanel1");
            this.tableLayoutPanel1.Controls.Add(this.groupBox9, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.groupBox2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.groupBox3, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.groupBox1, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.groupBox4, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.groupBox5, 1, 1);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.toolTip.SetToolTip(this.tableLayoutPanel1, resources.GetString("tableLayoutPanel1.ToolTip"));
            // 
            // groupBox9
            // 
            resources.ApplyResources(this.groupBox9, "groupBox9");
            this.groupBox9.Controls.Add(this.num_lowBat);
            this.groupBox9.ForeColor = System.Drawing.SystemColors.WindowText;
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.TabStop = false;
            this.toolTip.SetToolTip(this.groupBox9, resources.GetString("groupBox9.ToolTip"));
            // 
            // num_lowBat
            // 
            resources.ApplyResources(this.num_lowBat, "num_lowBat");
            this.num_lowBat.BackColor = System.Drawing.SystemColors.Control;
            this.num_lowBat.DecimalPlaces = 1;
            this.num_lowBat.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.num_lowBat.Maximum = new decimal(new int[] {
            65,
            0,
            0,
            65536});
            this.num_lowBat.Minimum = new decimal(new int[] {
            45,
            0,
            0,
            65536});
            this.num_lowBat.Name = "num_lowBat";
            this.toolTip.SetToolTip(this.num_lowBat, resources.GetString("num_lowBat.ToolTip"));
            this.num_lowBat.Value = new decimal(new int[] {
            46,
            0,
            0,
            65536});
            // 
            // groupBox2
            // 
            resources.ApplyResources(this.groupBox2, "groupBox2");
            this.groupBox2.Controls.Add(this.List_PORT);
            this.groupBox2.ForeColor = System.Drawing.SystemColors.WindowText;
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.TabStop = false;
            this.toolTip.SetToolTip(this.groupBox2, resources.GetString("groupBox2.ToolTip"));
            // 
            // List_PORT
            // 
            resources.ApplyResources(this.List_PORT, "List_PORT");
            this.List_PORT.BackColor = System.Drawing.SystemColors.Control;
            this.List_PORT.ForeColor = System.Drawing.SystemColors.WindowText;
            this.List_PORT.FormattingEnabled = true;
            this.List_PORT.Name = "List_PORT";
            this.toolTip.SetToolTip(this.List_PORT, resources.GetString("List_PORT.ToolTip"));
            // 
            // groupBox3
            // 
            resources.ApplyResources(this.groupBox3, "groupBox3");
            this.groupBox3.Controls.Add(this.List_Parity);
            this.groupBox3.ForeColor = System.Drawing.SystemColors.WindowText;
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.TabStop = false;
            this.toolTip.SetToolTip(this.groupBox3, resources.GetString("groupBox3.ToolTip"));
            // 
            // List_Parity
            // 
            resources.ApplyResources(this.List_Parity, "List_Parity");
            this.List_Parity.BackColor = System.Drawing.SystemColors.Control;
            this.List_Parity.ForeColor = System.Drawing.SystemColors.WindowText;
            this.List_Parity.FormattingEnabled = true;
            this.List_Parity.Items.AddRange(new object[] {
            resources.GetString("List_Parity.Items"),
            resources.GetString("List_Parity.Items1"),
            resources.GetString("List_Parity.Items2"),
            resources.GetString("List_Parity.Items3"),
            resources.GetString("List_Parity.Items4")});
            this.List_Parity.Name = "List_Parity";
            this.toolTip.SetToolTip(this.List_Parity, resources.GetString("List_Parity.ToolTip"));
            // 
            // groupBox1
            // 
            resources.ApplyResources(this.groupBox1, "groupBox1");
            this.groupBox1.Controls.Add(this.List_BAUD);
            this.groupBox1.ForeColor = System.Drawing.SystemColors.WindowText;
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.TabStop = false;
            this.toolTip.SetToolTip(this.groupBox1, resources.GetString("groupBox1.ToolTip"));
            // 
            // List_BAUD
            // 
            resources.ApplyResources(this.List_BAUD, "List_BAUD");
            this.List_BAUD.BackColor = System.Drawing.SystemColors.Control;
            this.List_BAUD.ForeColor = System.Drawing.SystemColors.WindowText;
            this.List_BAUD.FormattingEnabled = true;
            this.List_BAUD.Items.AddRange(new object[] {
            resources.GetString("List_BAUD.Items"),
            resources.GetString("List_BAUD.Items1"),
            resources.GetString("List_BAUD.Items2"),
            resources.GetString("List_BAUD.Items3"),
            resources.GetString("List_BAUD.Items4"),
            resources.GetString("List_BAUD.Items5"),
            resources.GetString("List_BAUD.Items6")});
            this.List_BAUD.Name = "List_BAUD";
            this.toolTip.SetToolTip(this.List_BAUD, resources.GetString("List_BAUD.ToolTip"));
            // 
            // groupBox4
            // 
            resources.ApplyResources(this.groupBox4, "groupBox4");
            this.groupBox4.Controls.Add(this.List_Stopbits);
            this.groupBox4.ForeColor = System.Drawing.SystemColors.WindowText;
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.TabStop = false;
            this.toolTip.SetToolTip(this.groupBox4, resources.GetString("groupBox4.ToolTip"));
            // 
            // List_Stopbits
            // 
            resources.ApplyResources(this.List_Stopbits, "List_Stopbits");
            this.List_Stopbits.BackColor = System.Drawing.SystemColors.Control;
            this.List_Stopbits.ForeColor = System.Drawing.SystemColors.WindowText;
            this.List_Stopbits.FormattingEnabled = true;
            this.List_Stopbits.Items.AddRange(new object[] {
            resources.GetString("List_Stopbits.Items"),
            resources.GetString("List_Stopbits.Items1")});
            this.List_Stopbits.Name = "List_Stopbits";
            this.toolTip.SetToolTip(this.List_Stopbits, resources.GetString("List_Stopbits.ToolTip"));
            // 
            // groupBox5
            // 
            resources.ApplyResources(this.groupBox5, "groupBox5");
            this.groupBox5.Controls.Add(this.List_DATAbits);
            this.groupBox5.ForeColor = System.Drawing.SystemColors.WindowText;
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.TabStop = false;
            this.toolTip.SetToolTip(this.groupBox5, resources.GetString("groupBox5.ToolTip"));
            // 
            // List_DATAbits
            // 
            resources.ApplyResources(this.List_DATAbits, "List_DATAbits");
            this.List_DATAbits.BackColor = System.Drawing.SystemColors.Control;
            this.List_DATAbits.ForeColor = System.Drawing.SystemColors.WindowText;
            this.List_DATAbits.FormattingEnabled = true;
            this.List_DATAbits.Items.AddRange(new object[] {
            resources.GetString("List_DATAbits.Items"),
            resources.GetString("List_DATAbits.Items1")});
            this.List_DATAbits.Name = "List_DATAbits";
            this.toolTip.SetToolTip(this.List_DATAbits, resources.GetString("List_DATAbits.ToolTip"));
            // 
            // button_Serial_Scan
            // 
            resources.ApplyResources(this.button_Serial_Scan, "button_Serial_Scan");
            this.button_Serial_Scan.BackColor = System.Drawing.Color.Transparent;
            this.button_Serial_Scan.ForeColor = System.Drawing.SystemColors.WindowText;
            this.button_Serial_Scan.Name = "button_Serial_Scan";
            this.toolTip.SetToolTip(this.button_Serial_Scan, resources.GetString("button_Serial_Scan.ToolTip"));
            this.button_Serial_Scan.UseVisualStyleBackColor = false;
            this.button_Serial_Scan.Click += new System.EventHandler(this.Serial_Click);
            // 
            // group_Console
            // 
            resources.ApplyResources(this.group_Console, "group_Console");
            this.group_Console.BackColor = System.Drawing.SystemColors.Control;
            this.group_Console.Controls.Add(this.Textbox_Console);
            this.group_Console.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.group_Console.Name = "group_Console";
            this.group_Console.TabStop = false;
            this.toolTip.SetToolTip(this.group_Console, resources.GetString("group_Console.ToolTip"));
            // 
            // Textbox_Console
            // 
            resources.ApplyResources(this.Textbox_Console, "Textbox_Console");
            this.Textbox_Console.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Textbox_Console.Name = "Textbox_Console";
            this.Textbox_Console.ReadOnly = true;
            this.toolTip.SetToolTip(this.Textbox_Console, resources.GetString("Textbox_Console.ToolTip"));
            this.Textbox_Console.DoubleClick += new System.EventHandler(this.Console_doubleclick);
            // 
            // tableLayoutPanel4
            // 
            resources.ApplyResources(this.tableLayoutPanel4, "tableLayoutPanel4");
            this.tableLayoutPanel4.Controls.Add(this.group_MOUSE, 2, 0);
            this.tableLayoutPanel4.Controls.Add(this.group_Conig, 1, 0);
            this.tableLayoutPanel4.Controls.Add(this.tableLayoutPanel2, 0, 0);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.toolTip.SetToolTip(this.tableLayoutPanel4, resources.GetString("tableLayoutPanel4.ToolTip"));
            // 
            // group_MOUSE
            // 
            resources.ApplyResources(this.group_MOUSE, "group_MOUSE");
            this.group_MOUSE.Controls.Add(this.tableLayoutPanel7);
            this.group_MOUSE.Name = "group_MOUSE";
            this.group_MOUSE.TabStop = false;
            this.toolTip.SetToolTip(this.group_MOUSE, resources.GetString("group_MOUSE.ToolTip"));
            // 
            // tableLayoutPanel7
            // 
            resources.ApplyResources(this.tableLayoutPanel7, "tableLayoutPanel7");
            this.tableLayoutPanel7.Controls.Add(this.groupBox6, 0, 0);
            this.tableLayoutPanel7.Controls.Add(this.groupBox8, 0, 2);
            this.tableLayoutPanel7.Controls.Add(this.groupBox7, 0, 1);
            this.tableLayoutPanel7.Name = "tableLayoutPanel7";
            this.toolTip.SetToolTip(this.tableLayoutPanel7, resources.GetString("tableLayoutPanel7.ToolTip"));
            // 
            // groupBox6
            // 
            resources.ApplyResources(this.groupBox6, "groupBox6");
            this.groupBox6.Controls.Add(this.num_mouse_pers);
            this.groupBox6.ForeColor = System.Drawing.SystemColors.WindowText;
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.TabStop = false;
            this.toolTip.SetToolTip(this.groupBox6, resources.GetString("groupBox6.ToolTip"));
            // 
            // num_mouse_pers
            // 
            resources.ApplyResources(this.num_mouse_pers, "num_mouse_pers");
            this.num_mouse_pers.BackColor = System.Drawing.SystemColors.Control;
            this.num_mouse_pers.Maximum = new decimal(new int[] {
            90,
            0,
            0,
            0});
            this.num_mouse_pers.Minimum = new decimal(new int[] {
            30,
            0,
            0,
            0});
            this.num_mouse_pers.Name = "num_mouse_pers";
            this.toolTip.SetToolTip(this.num_mouse_pers, resources.GetString("num_mouse_pers.ToolTip"));
            this.num_mouse_pers.Value = new decimal(new int[] {
            40,
            0,
            0,
            0});
            this.num_mouse_pers.ValueChanged += new System.EventHandler(this.Mouse_Update_Event);
            // 
            // groupBox8
            // 
            resources.ApplyResources(this.groupBox8, "groupBox8");
            this.groupBox8.Controls.Add(this.List_Algorithm);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.TabStop = false;
            this.toolTip.SetToolTip(this.groupBox8, resources.GetString("groupBox8.ToolTip"));
            // 
            // List_Algorithm
            // 
            resources.ApplyResources(this.List_Algorithm, "List_Algorithm");
            this.List_Algorithm.BackColor = System.Drawing.SystemColors.Control;
            this.List_Algorithm.FormattingEnabled = true;
            this.List_Algorithm.Items.AddRange(new object[] {
            resources.GetString("List_Algorithm.Items"),
            resources.GetString("List_Algorithm.Items1"),
            resources.GetString("List_Algorithm.Items2"),
            resources.GetString("List_Algorithm.Items3"),
            resources.GetString("List_Algorithm.Items4"),
            resources.GetString("List_Algorithm.Items5")});
            this.List_Algorithm.Name = "List_Algorithm";
            this.toolTip.SetToolTip(this.List_Algorithm, resources.GetString("List_Algorithm.ToolTip"));
            this.List_Algorithm.SelectedIndexChanged += new System.EventHandler(this.Algorithm_Change);
            // 
            // groupBox7
            // 
            resources.ApplyResources(this.groupBox7, "groupBox7");
            this.groupBox7.Controls.Add(this.Num_mouse_min);
            this.groupBox7.ForeColor = System.Drawing.SystemColors.WindowText;
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.TabStop = false;
            this.toolTip.SetToolTip(this.groupBox7, resources.GetString("groupBox7.ToolTip"));
            // 
            // Num_mouse_min
            // 
            resources.ApplyResources(this.Num_mouse_min, "Num_mouse_min");
            this.Num_mouse_min.BackColor = System.Drawing.SystemColors.Control;
            this.Num_mouse_min.DecimalPlaces = 2;
            this.Num_mouse_min.Increment = new decimal(new int[] {
            5,
            0,
            0,
            131072});
            this.Num_mouse_min.Maximum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.Num_mouse_min.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            131072});
            this.Num_mouse_min.Name = "Num_mouse_min";
            this.toolTip.SetToolTip(this.Num_mouse_min, resources.GetString("Num_mouse_min.ToolTip"));
            this.Num_mouse_min.Value = new decimal(new int[] {
            2,
            0,
            0,
            65536});
            this.Num_mouse_min.ValueChanged += new System.EventHandler(this.Mouse_Update_Event);
            // 
            // tableLayoutPanel2
            // 
            resources.ApplyResources(this.tableLayoutPanel2, "tableLayoutPanel2");
            this.tableLayoutPanel2.Controls.Add(this.Button_Data, 0, 3);
            this.tableLayoutPanel2.Controls.Add(this.button_Serial_Scan, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.Button_Parameter, 0, 1);
            this.tableLayoutPanel2.Controls.Add(this.Button_Visualize, 0, 2);
            this.tableLayoutPanel2.Controls.Add(this.Button_Calib, 0, 4);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.toolTip.SetToolTip(this.tableLayoutPanel2, resources.GetString("tableLayoutPanel2.ToolTip"));
            // 
            // Button_Data
            // 
            resources.ApplyResources(this.Button_Data, "Button_Data");
            this.Button_Data.BackColor = System.Drawing.Color.Transparent;
            this.Button_Data.ForeColor = System.Drawing.SystemColors.WindowText;
            this.Button_Data.Name = "Button_Data";
            this.toolTip.SetToolTip(this.Button_Data, resources.GetString("Button_Data.ToolTip"));
            this.Button_Data.UseVisualStyleBackColor = false;
            this.Button_Data.Click += new System.EventHandler(this.Data_Click);
            // 
            // Button_Parameter
            // 
            resources.ApplyResources(this.Button_Parameter, "Button_Parameter");
            this.Button_Parameter.ForeColor = System.Drawing.SystemColors.WindowText;
            this.Button_Parameter.Name = "Button_Parameter";
            this.toolTip.SetToolTip(this.Button_Parameter, resources.GetString("Button_Parameter.ToolTip"));
            this.Button_Parameter.UseVisualStyleBackColor = true;
            this.Button_Parameter.Click += new System.EventHandler(this.Parameter_Click);
            // 
            // Button_Visualize
            // 
            resources.ApplyResources(this.Button_Visualize, "Button_Visualize");
            this.Button_Visualize.BackColor = System.Drawing.Color.Transparent;
            this.Button_Visualize.ForeColor = System.Drawing.SystemColors.WindowText;
            this.Button_Visualize.Name = "Button_Visualize";
            this.toolTip.SetToolTip(this.Button_Visualize, resources.GetString("Button_Visualize.ToolTip"));
            this.Button_Visualize.UseVisualStyleBackColor = false;
            this.Button_Visualize.Click += new System.EventHandler(this.Visualize_Click);
            // 
            // Button_Calib
            // 
            resources.ApplyResources(this.Button_Calib, "Button_Calib");
            this.Button_Calib.BackColor = System.Drawing.Color.Transparent;
            this.Button_Calib.ForeColor = System.Drawing.SystemColors.WindowText;
            this.Button_Calib.Name = "Button_Calib";
            this.toolTip.SetToolTip(this.Button_Calib, resources.GetString("Button_Calib.ToolTip"));
            this.Button_Calib.UseVisualStyleBackColor = false;
            this.Button_Calib.Click += new System.EventHandler(this.Calib_click);
            // 
            // Noti
            // 
            resources.ApplyResources(this.Noti, "Noti");
            this.Noti.BalloonTipClicked += new System.EventHandler(this.Noti_Click);
            this.Noti.Click += new System.EventHandler(this.Noti_Click);
            this.Noti.DoubleClick += new System.EventHandler(this.Noti_Click);
            // 
            // TIMER
            // 
            this.TIMER.Enabled = true;
            this.TIMER.Interval = 20;
            this.TIMER.Tick += new System.EventHandler(this.TIMER_process);
            // 
            // _Form_Sense
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.Controls.Add(this.group_Console);
            this.Controls.Add(this.tableLayoutPanel4);
            this.ForeColor = System.Drawing.SystemColors.Window;
            this.MaximizeBox = false;
            this.Name = "_Form_Sense";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
            this.toolTip.SetToolTip(this, resources.GetString("$this.ToolTip"));
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SENSE_FormClosing);
            this.Load += new System.EventHandler(this.SENSE_Load);
            this.LocationChanged += new System.EventHandler(this.SENSE_SizeEvent);
            this.SizeChanged += new System.EventHandler(this.SENSE_SizeEvent);
            this.group_Conig.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.groupBox9.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.num_lowBat)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            this.group_Console.ResumeLayout(false);
            this.group_Console.PerformLayout();
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel4.PerformLayout();
            this.group_MOUSE.ResumeLayout(false);
            this.group_MOUSE.PerformLayout();
            this.tableLayoutPanel7.ResumeLayout(false);
            this.tableLayoutPanel7.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.num_mouse_pers)).EndInit();
            this.groupBox8.ResumeLayout(false);
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Num_mouse_min)).EndInit();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }
        
        #endregion

        private System.IO.Ports.SerialPort Serial_COM;
        private System.Windows.Forms.GroupBox group_Console;
        private System.Windows.Forms.TextBox Textbox_Console;
        private System.Windows.Forms.GroupBox group_Conig;
        private System.Windows.Forms.ComboBox List_Parity;
        private System.Windows.Forms.ComboBox List_BAUD;
        private System.Windows.Forms.ComboBox List_PORT;
        private System.Windows.Forms.ComboBox List_Stopbits;
        private System.Windows.Forms.ComboBox List_DATAbits;
        private System.Windows.Forms.Button button_Serial_Scan;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.NotifyIcon Noti;
        private System.Windows.Forms.GroupBox group_MOUSE;
        private System.Windows.Forms.NumericUpDown num_mouse_pers;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.NumericUpDown Num_mouse_min;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Button Button_Visualize;
        private System.Windows.Forms.Button Button_Parameter;
        private System.Windows.Forms.Button Button_Data;
        private System.Windows.Forms.ToolTip toolTip;
        private System.Windows.Forms.GroupBox groupBox9;
        private System.Windows.Forms.NumericUpDown num_lowBat;
        private System.Windows.Forms.Timer TIMER;
        private System.Windows.Forms.Button Button_Calib;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel7;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.ComboBox List_Algorithm;
    }
}

