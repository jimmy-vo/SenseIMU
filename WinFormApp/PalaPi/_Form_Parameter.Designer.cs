namespace SENSE
{
    partial class _Form_Parameter
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle16 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle19 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle20 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle17 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(_Form_Parameter));
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle18 = new System.Windows.Forms.DataGridViewCellStyle();
            this.ParameterView = new System.Windows.Forms.DataGridView();
            this.Type = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Parameter_Value = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.button_ParameterSave = new System.Windows.Forms.Button();
            this.button_Parameter_Refresh = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.ParameterView)).BeginInit();
            this.tableLayoutPanel5.SuspendLayout();
            this.SuspendLayout();
            // 
            // ParameterView
            // 
            this.ParameterView.AllowUserToAddRows = false;
            this.ParameterView.AllowUserToDeleteRows = false;
            this.ParameterView.AllowUserToResizeColumns = false;
            this.ParameterView.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.ParameterView.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.ParameterView.BackgroundColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle16.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle16.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle16.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.5F);
            dataGridViewCellStyle16.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle16.NullValue = null;
            dataGridViewCellStyle16.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle16.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle16.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.ParameterView.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle16;
            this.ParameterView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.ParameterView.ColumnHeadersVisible = false;
            this.ParameterView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Type,
            this.Parameter_Value});
            resources.ApplyResources(this.ParameterView, "ParameterView");
            this.ParameterView.GridColor = System.Drawing.SystemColors.ControlLight;
            this.ParameterView.MultiSelect = false;
            this.ParameterView.Name = "ParameterView";
            dataGridViewCellStyle19.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle19.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            dataGridViewCellStyle19.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.5F);
            dataGridViewCellStyle19.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle19.SelectionBackColor = System.Drawing.Color.Transparent;
            dataGridViewCellStyle19.SelectionForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle19.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.ParameterView.RowHeadersDefaultCellStyle = dataGridViewCellStyle19;
            this.ParameterView.RowHeadersVisible = false;
            dataGridViewCellStyle20.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle20.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle20.SelectionForeColor = System.Drawing.SystemColors.WindowText;
            this.ParameterView.RowsDefaultCellStyle = dataGridViewCellStyle20;
            this.ParameterView.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.ParameterView.ShowCellErrors = false;
            this.ParameterView.ShowCellToolTips = false;
            this.ParameterView.ShowEditingIcon = false;
            this.ParameterView.ShowRowErrors = false;
            // 
            // Type
            // 
            this.Type.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.DisplayedCells;
            dataGridViewCellStyle17.BackColor = System.Drawing.Color.Silver;
            dataGridViewCellStyle17.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle17.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle17.SelectionBackColor = System.Drawing.Color.Silver;
            dataGridViewCellStyle17.SelectionForeColor = System.Drawing.Color.Silver;
            this.Type.DefaultCellStyle = dataGridViewCellStyle17;
            this.Type.FillWeight = 50F;
            resources.ApplyResources(this.Type, "Type");
            this.Type.Name = "Type";
            this.Type.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.Type.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // Parameter_Value
            // 
            this.Parameter_Value.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            dataGridViewCellStyle18.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle18.Format = "0.########";
            dataGridViewCellStyle18.NullValue = "0";
            this.Parameter_Value.DefaultCellStyle = dataGridViewCellStyle18;
            this.Parameter_Value.FillWeight = 50F;
            resources.ApplyResources(this.Parameter_Value, "Parameter_Value");
            this.Parameter_Value.MaxInputLength = 10;
            this.Parameter_Value.Name = "Parameter_Value";
            this.Parameter_Value.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // tableLayoutPanel5
            // 
            resources.ApplyResources(this.tableLayoutPanel5, "tableLayoutPanel5");
            this.tableLayoutPanel5.Controls.Add(this.button_ParameterSave, 0, 0);
            this.tableLayoutPanel5.Controls.Add(this.button_Parameter_Refresh, 0, 0);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            // 
            // button_ParameterSave
            // 
            resources.ApplyResources(this.button_ParameterSave, "button_ParameterSave");
            this.button_ParameterSave.ForeColor = System.Drawing.SystemColors.WindowText;
            this.button_ParameterSave.Name = "button_ParameterSave";
            this.button_ParameterSave.UseVisualStyleBackColor = true;
            this.button_ParameterSave.Click += new System.EventHandler(this.Parameter_Save_Click);
            // 
            // button_Parameter_Refresh
            // 
            resources.ApplyResources(this.button_Parameter_Refresh, "button_Parameter_Refresh");
            this.button_Parameter_Refresh.ForeColor = System.Drawing.SystemColors.WindowText;
            this.button_Parameter_Refresh.Name = "button_Parameter_Refresh";
            this.button_Parameter_Refresh.UseVisualStyleBackColor = true;
            this.button_Parameter_Refresh.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Parameter_Refresh_Click);
            // 
            // _Form_Parameter
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoValidate = System.Windows.Forms.AutoValidate.EnablePreventFocusChange;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.Controls.Add(this.ParameterView);
            this.Controls.Add(this.tableLayoutPanel5);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "_Form_Parameter";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            ((System.ComponentModel.ISupportInitialize)(this.ParameterView)).EndInit();
            this.tableLayoutPanel5.ResumeLayout(false);
            this.tableLayoutPanel5.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView ParameterView;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private System.Windows.Forms.Button button_ParameterSave;
        private System.Windows.Forms.Button button_Parameter_Refresh;
        private System.Windows.Forms.DataGridViewTextBoxColumn Type;
        private System.Windows.Forms.DataGridViewTextBoxColumn Parameter_Value;
    }
}