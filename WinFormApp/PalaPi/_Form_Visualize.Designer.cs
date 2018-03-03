namespace SENSE
{
    partial class _Form_Visualize
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
            this.Visualize = new Tao.Platform.Windows.SimpleOpenGlControl();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // Visualize
            // 
            this.Visualize.AccumBits = ((byte)(0));
            this.Visualize.AutoCheckErrors = false;
            this.Visualize.AutoFinish = false;
            this.Visualize.AutoMakeCurrent = true;
            this.Visualize.AutoSwapBuffers = true;
            this.Visualize.BackColor = System.Drawing.SystemColors.Control;
            this.Visualize.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.Visualize.ColorBits = ((byte)(32));
            this.Visualize.DepthBits = ((byte)(16));
            this.Visualize.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Visualize.Font = new System.Drawing.Font("Courier New", 13F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.World);
            this.Visualize.ForeColor = System.Drawing.SystemColors.Window;
            this.Visualize.Location = new System.Drawing.Point(0, 0);
            this.Visualize.Name = "Visualize";
            this.Visualize.Size = new System.Drawing.Size(283, 242);
            this.Visualize.StencilBits = ((byte)(0));
            this.Visualize.TabIndex = 0;
            this.Visualize.Load += new System.EventHandler(this.Visualize_Load);
            this.Visualize.SizeChanged += new System.EventHandler(this.Visualize_SizeChanged);
            this.Visualize.Paint += new System.Windows.Forms.PaintEventHandler(this.Render);
            this.Visualize.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Visualize_KeyDown);
            this.Visualize.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Visualize_KeyUp);
            this.Visualize.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Visualize_MouseDown);
            this.Visualize.MouseLeave += new System.EventHandler(this.Visualize_MouseLeave);
            this.Visualize.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Visualize_MouseMove);
            this.Visualize.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Visualize_MouseUp);
            // 
            // _Form_Visualize
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.ClientSize = new System.Drawing.Size(283, 242);
            this.Controls.Add(this.Visualize);
            this.MinimizeBox = false;
            this.Name = "_Form_Visualize";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Graphic";
            this.ResumeLayout(false);

        }

        #endregion

        private Tao.Platform.Windows.SimpleOpenGlControl Visualize;
        private System.Windows.Forms.ToolTip toolTip1;
    }
}