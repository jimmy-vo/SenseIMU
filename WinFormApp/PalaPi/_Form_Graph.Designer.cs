namespace SENSE
{
    partial class _Form_Graph
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle6 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.Graph = new ZedGraph.ZedGraphControl();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.group_Interval = new System.Windows.Forms.GroupBox();
            this.num_Graph_Interval = new System.Windows.Forms.NumericUpDown();
            this.group_DATA = new System.Windows.Forms.GroupBox();
            this.List_Graph_DATA = new System.Windows.Forms.ComboBox();
            this.group_Scale = new System.Windows.Forms.GroupBox();
            this.check_Graph_Realtime = new System.Windows.Forms.RadioButton();
            this.List_Graph_ScaleMode = new System.Windows.Forms.ComboBox();
            this.check_Graph_YZ = new System.Windows.Forms.RadioButton();
            this.check_Graph_XZ = new System.Windows.Forms.RadioButton();
            this.check_Graph_XY = new System.Windows.Forms.RadioButton();
            this.DATAGrid = new System.Windows.Forms.DataGridView();
            this.Data_Type = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Value = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Unit = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.group_Interval.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.num_Graph_Interval)).BeginInit();
            this.group_DATA.SuspendLayout();
            this.group_Scale.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DATAGrid)).BeginInit();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 350F));
            this.tableLayoutPanel1.Controls.Add(this.Graph, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 1, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 520F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(782, 520);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // Graph
            // 
            this.Graph.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.Graph.AutoValidate = System.Windows.Forms.AutoValidate.Disable;
            this.Graph.BackColor = System.Drawing.SystemColors.Control;
            this.Graph.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.Graph.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Graph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Graph.EditButtons = System.Windows.Forms.MouseButtons.None;
            this.Graph.EditModifierKeys = System.Windows.Forms.Keys.None;
            this.Graph.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Graph.ForeColor = System.Drawing.SystemColors.MenuHighlight;
            this.Graph.IsShowContextMenu = false;
            this.Graph.IsShowCopyMessage = false;
            this.Graph.LinkButtons = System.Windows.Forms.MouseButtons.None;
            this.Graph.LinkModifierKeys = System.Windows.Forms.Keys.None;
            this.Graph.Location = new System.Drawing.Point(0, 0);
            this.Graph.Margin = new System.Windows.Forms.Padding(0);
            this.Graph.Name = "Graph";
            this.Graph.PanButtons = System.Windows.Forms.MouseButtons.Right;
            this.Graph.PanButtons2 = System.Windows.Forms.MouseButtons.None;
            this.Graph.PanModifierKeys = System.Windows.Forms.Keys.None;
            this.Graph.PointDateFormat = "";
            this.Graph.PointValueFormat = "";
            this.Graph.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.Graph.ScrollGrace = 0D;
            this.Graph.ScrollMaxX = 0D;
            this.Graph.ScrollMaxY = 0D;
            this.Graph.ScrollMaxY2 = 0D;
            this.Graph.ScrollMinX = 0D;
            this.Graph.ScrollMinY = 0D;
            this.Graph.ScrollMinY2 = 0D;
            this.Graph.SelectButtons = System.Windows.Forms.MouseButtons.None;
            this.Graph.SelectModifierKeys = System.Windows.Forms.Keys.None;
            this.Graph.Size = new System.Drawing.Size(432, 520);
            this.Graph.TabIndex = 2;
            this.Graph.ZoomStepFraction = 0.05D;
            this.Graph.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Graph_MouseClick);
            this.Graph.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.Graph_DoubleClick);
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.AutoSize = true;
            this.tableLayoutPanel2.ColumnCount = 1;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel3, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.DATAGrid, 0, 1);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(435, 3);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 2;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.Size = new System.Drawing.Size(344, 514);
            this.tableLayoutPanel2.TabIndex = 0;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.AutoSize = true;
            this.tableLayoutPanel3.ColumnCount = 2;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.Controls.Add(this.tableLayoutPanel4, 1, 0);
            this.tableLayoutPanel3.Controls.Add(this.group_Scale, 0, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Top;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel3.Size = new System.Drawing.Size(338, 134);
            this.tableLayoutPanel3.TabIndex = 4;
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.AutoSize = true;
            this.tableLayoutPanel4.ColumnCount = 1;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Controls.Add(this.group_Interval, 0, 2);
            this.tableLayoutPanel4.Controls.Add(this.group_DATA, 0, 0);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Top;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(172, 3);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 3;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel4.Size = new System.Drawing.Size(163, 99);
            this.tableLayoutPanel4.TabIndex = 4;
            // 
            // group_Interval
            // 
            this.group_Interval.AutoSize = true;
            this.group_Interval.Controls.Add(this.num_Graph_Interval);
            this.group_Interval.Dock = System.Windows.Forms.DockStyle.Top;
            this.group_Interval.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.group_Interval.Location = new System.Drawing.Point(3, 53);
            this.group_Interval.Name = "group_Interval";
            this.group_Interval.Size = new System.Drawing.Size(157, 43);
            this.group_Interval.TabIndex = 3;
            this.group_Interval.TabStop = false;
            this.group_Interval.Text = "Interval (ms)";
            // 
            // num_Graph_Interval
            // 
            this.num_Graph_Interval.AutoSize = true;
            this.num_Graph_Interval.BackColor = System.Drawing.SystemColors.Control;
            this.num_Graph_Interval.Dock = System.Windows.Forms.DockStyle.Top;
            this.num_Graph_Interval.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.num_Graph_Interval.Increment = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.num_Graph_Interval.Location = new System.Drawing.Point(3, 17);
            this.num_Graph_Interval.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.num_Graph_Interval.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.num_Graph_Interval.Name = "num_Graph_Interval";
            this.num_Graph_Interval.Size = new System.Drawing.Size(151, 23);
            this.num_Graph_Interval.TabIndex = 26;
            this.num_Graph_Interval.Value = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            // 
            // group_DATA
            // 
            this.group_DATA.AutoSize = true;
            this.group_DATA.Controls.Add(this.List_Graph_DATA);
            this.group_DATA.Dock = System.Windows.Forms.DockStyle.Top;
            this.group_DATA.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.group_DATA.Location = new System.Drawing.Point(3, 3);
            this.group_DATA.Name = "group_DATA";
            this.group_DATA.Size = new System.Drawing.Size(157, 44);
            this.group_DATA.TabIndex = 1;
            this.group_DATA.TabStop = false;
            this.group_DATA.Text = "DATA";
            // 
            // List_Graph_DATA
            // 
            this.List_Graph_DATA.BackColor = System.Drawing.SystemColors.Control;
            this.List_Graph_DATA.Dock = System.Windows.Forms.DockStyle.Top;
            this.List_Graph_DATA.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.List_Graph_DATA.FormattingEnabled = true;
            this.List_Graph_DATA.Location = new System.Drawing.Point(3, 17);
            this.List_Graph_DATA.Name = "List_Graph_DATA";
            this.List_Graph_DATA.Size = new System.Drawing.Size(151, 24);
            this.List_Graph_DATA.TabIndex = 0;
            this.List_Graph_DATA.Text = "Choose...";
            this.List_Graph_DATA.SelectedIndexChanged += new System.EventHandler(this.Graph_Filter_Update);
            // 
            // group_Scale
            // 
            this.group_Scale.AutoSize = true;
            this.group_Scale.Controls.Add(this.check_Graph_Realtime);
            this.group_Scale.Controls.Add(this.List_Graph_ScaleMode);
            this.group_Scale.Controls.Add(this.check_Graph_YZ);
            this.group_Scale.Controls.Add(this.check_Graph_XZ);
            this.group_Scale.Controls.Add(this.check_Graph_XY);
            this.group_Scale.Dock = System.Windows.Forms.DockStyle.Top;
            this.group_Scale.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.group_Scale.Location = new System.Drawing.Point(3, 3);
            this.group_Scale.Name = "group_Scale";
            this.group_Scale.Size = new System.Drawing.Size(163, 128);
            this.group_Scale.TabIndex = 2;
            this.group_Scale.TabStop = false;
            this.group_Scale.Text = "Scale Mode";
            // 
            // check_Graph_Realtime
            // 
            this.check_Graph_Realtime.AutoSize = true;
            this.check_Graph_Realtime.Checked = true;
            this.check_Graph_Realtime.Dock = System.Windows.Forms.DockStyle.Top;
            this.check_Graph_Realtime.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.check_Graph_Realtime.ForeColor = System.Drawing.SystemColors.WindowText;
            this.check_Graph_Realtime.Location = new System.Drawing.Point(3, 104);
            this.check_Graph_Realtime.Name = "check_Graph_Realtime";
            this.check_Graph_Realtime.Size = new System.Drawing.Size(157, 21);
            this.check_Graph_Realtime.TabIndex = 36;
            this.check_Graph_Realtime.TabStop = true;
            this.check_Graph_Realtime.Text = "Real time";
            this.check_Graph_Realtime.UseVisualStyleBackColor = true;
            this.check_Graph_Realtime.CheckedChanged += new System.EventHandler(this.Graph_Filter_Update);
            // 
            // List_Graph_ScaleMode
            // 
            this.List_Graph_ScaleMode.BackColor = System.Drawing.SystemColors.Control;
            this.List_Graph_ScaleMode.Dock = System.Windows.Forms.DockStyle.Top;
            this.List_Graph_ScaleMode.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.List_Graph_ScaleMode.FormattingEnabled = true;
            this.List_Graph_ScaleMode.Items.AddRange(new object[] {
            "Compact",
            "Scroll",
            "Center",
            "Free"});
            this.List_Graph_ScaleMode.Location = new System.Drawing.Point(3, 80);
            this.List_Graph_ScaleMode.Name = "List_Graph_ScaleMode";
            this.List_Graph_ScaleMode.Size = new System.Drawing.Size(157, 24);
            this.List_Graph_ScaleMode.TabIndex = 2;
            this.List_Graph_ScaleMode.Text = "Center";
            this.List_Graph_ScaleMode.SelectedIndexChanged += new System.EventHandler(this.Graph_ScaleMode_Update);
            // 
            // check_Graph_YZ
            // 
            this.check_Graph_YZ.AutoSize = true;
            this.check_Graph_YZ.Dock = System.Windows.Forms.DockStyle.Top;
            this.check_Graph_YZ.Enabled = false;
            this.check_Graph_YZ.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.check_Graph_YZ.ForeColor = System.Drawing.SystemColors.WindowText;
            this.check_Graph_YZ.Location = new System.Drawing.Point(3, 59);
            this.check_Graph_YZ.Name = "check_Graph_YZ";
            this.check_Graph_YZ.Size = new System.Drawing.Size(157, 21);
            this.check_Graph_YZ.TabIndex = 35;
            this.check_Graph_YZ.Text = "Y-Z";
            this.check_Graph_YZ.UseVisualStyleBackColor = true;
            this.check_Graph_YZ.CheckedChanged += new System.EventHandler(this.Graph_Filter_Update);
            // 
            // check_Graph_XZ
            // 
            this.check_Graph_XZ.AutoSize = true;
            this.check_Graph_XZ.Dock = System.Windows.Forms.DockStyle.Top;
            this.check_Graph_XZ.Enabled = false;
            this.check_Graph_XZ.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.check_Graph_XZ.ForeColor = System.Drawing.SystemColors.WindowText;
            this.check_Graph_XZ.Location = new System.Drawing.Point(3, 38);
            this.check_Graph_XZ.Name = "check_Graph_XZ";
            this.check_Graph_XZ.Size = new System.Drawing.Size(157, 21);
            this.check_Graph_XZ.TabIndex = 34;
            this.check_Graph_XZ.Text = "X-Z";
            this.check_Graph_XZ.UseVisualStyleBackColor = true;
            this.check_Graph_XZ.CheckedChanged += new System.EventHandler(this.Graph_Filter_Update);
            // 
            // check_Graph_XY
            // 
            this.check_Graph_XY.AutoSize = true;
            this.check_Graph_XY.Dock = System.Windows.Forms.DockStyle.Top;
            this.check_Graph_XY.Enabled = false;
            this.check_Graph_XY.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.check_Graph_XY.ForeColor = System.Drawing.SystemColors.WindowText;
            this.check_Graph_XY.Location = new System.Drawing.Point(3, 17);
            this.check_Graph_XY.Name = "check_Graph_XY";
            this.check_Graph_XY.Size = new System.Drawing.Size(157, 21);
            this.check_Graph_XY.TabIndex = 33;
            this.check_Graph_XY.Text = "X-Y";
            this.check_Graph_XY.UseVisualStyleBackColor = true;
            this.check_Graph_XY.CheckedChanged += new System.EventHandler(this.Graph_Filter_Update);
            // 
            // DATAGrid
            // 
            this.DATAGrid.AllowUserToAddRows = false;
            this.DATAGrid.AllowUserToDeleteRows = false;
            this.DATAGrid.AllowUserToResizeColumns = false;
            this.DATAGrid.AllowUserToResizeRows = false;
            this.DATAGrid.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.DATAGrid.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.DATAGrid.BackgroundColor = System.Drawing.SystemColors.Control;
            this.DATAGrid.ColumnHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.None;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle1.NullValue = null;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.DATAGrid.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this.DATAGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.DATAGrid.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Data_Type,
            this.Value,
            this.Unit});
            this.DATAGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DATAGrid.EditMode = System.Windows.Forms.DataGridViewEditMode.EditOnEnter;
            this.DATAGrid.GridColor = System.Drawing.SystemColors.Control;
            this.DATAGrid.Location = new System.Drawing.Point(3, 143);
            this.DATAGrid.MultiSelect = false;
            this.DATAGrid.Name = "DATAGrid";
            this.DATAGrid.ReadOnly = true;
            this.DATAGrid.RowHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.None;
            dataGridViewCellStyle5.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle5.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            dataGridViewCellStyle5.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle5.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle5.SelectionBackColor = System.Drawing.Color.Transparent;
            dataGridViewCellStyle5.SelectionForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle5.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.DATAGrid.RowHeadersDefaultCellStyle = dataGridViewCellStyle5;
            this.DATAGrid.RowHeadersVisible = false;
            this.DATAGrid.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            dataGridViewCellStyle6.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle6.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle6.SelectionForeColor = System.Drawing.SystemColors.WindowText;
            this.DATAGrid.RowsDefaultCellStyle = dataGridViewCellStyle6;
            this.DATAGrid.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.DATAGrid.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.DATAGrid.ShowCellErrors = false;
            this.DATAGrid.ShowCellToolTips = false;
            this.DATAGrid.ShowEditingIcon = false;
            this.DATAGrid.ShowRowErrors = false;
            this.DATAGrid.Size = new System.Drawing.Size(338, 368);
            this.DATAGrid.TabIndex = 3;
            // 
            // Data_Type
            // 
            this.Data_Type.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            dataGridViewCellStyle2.BackColor = System.Drawing.Color.Silver;
            dataGridViewCellStyle2.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.Color.Silver;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.Color.Silver;
            this.Data_Type.DefaultCellStyle = dataGridViewCellStyle2;
            this.Data_Type.HeaderText = "Data";
            this.Data_Type.Name = "Data_Type";
            this.Data_Type.ReadOnly = true;
            this.Data_Type.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.Data_Type.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // Value
            // 
            this.Value.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleRight;
            dataGridViewCellStyle3.Format = "000.00";
            this.Value.DefaultCellStyle = dataGridViewCellStyle3;
            this.Value.HeaderText = "Value";
            this.Value.Name = "Value";
            this.Value.ReadOnly = true;
            this.Value.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // Unit
            // 
            this.Unit.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            dataGridViewCellStyle4.BackColor = System.Drawing.Color.Silver;
            dataGridViewCellStyle4.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle4.SelectionBackColor = System.Drawing.Color.Silver;
            dataGridViewCellStyle4.SelectionForeColor = System.Drawing.Color.Silver;
            this.Unit.DefaultCellStyle = dataGridViewCellStyle4;
            this.Unit.HeaderText = "Unit";
            this.Unit.Name = "Unit";
            this.Unit.ReadOnly = true;
            this.Unit.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.Unit.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // _Form_Graph
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
            this.ClientSize = new System.Drawing.Size(782, 520);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "_Form_Graph";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Data";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel4.PerformLayout();
            this.group_Interval.ResumeLayout(false);
            this.group_Interval.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.num_Graph_Interval)).EndInit();
            this.group_DATA.ResumeLayout(false);
            this.group_Scale.ResumeLayout(false);
            this.group_Scale.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DATAGrid)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.DataGridView DATAGrid;
        private ZedGraph.ZedGraphControl Graph;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.GroupBox group_Interval;
        private System.Windows.Forms.NumericUpDown num_Graph_Interval;
        private System.Windows.Forms.GroupBox group_DATA;
        private System.Windows.Forms.ComboBox List_Graph_DATA;
        private System.Windows.Forms.GroupBox group_Scale;
        private System.Windows.Forms.RadioButton check_Graph_Realtime;
        private System.Windows.Forms.ComboBox List_Graph_ScaleMode;
        private System.Windows.Forms.RadioButton check_Graph_YZ;
        private System.Windows.Forms.RadioButton check_Graph_XZ;
        private System.Windows.Forms.RadioButton check_Graph_XY;
        private System.Windows.Forms.DataGridViewTextBoxColumn Data_Type;
        private System.Windows.Forms.DataGridViewTextBoxColumn Value;
        private System.Windows.Forms.DataGridViewTextBoxColumn Unit;
    }
}