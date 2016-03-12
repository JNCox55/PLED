namespace PLED_GUI
{
    partial class woodDimensions
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
            this.x_woodDimension = new System.Windows.Forms.TextBox();
            this.y_woodDimension = new System.Windows.Forms.TextBox();
            this.setDimension = new System.Windows.Forms.Button();
            this.woodDimensionDialog = new System.Windows.Forms.TextBox();
            this.closeDim = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // x_woodDimension
            // 
            this.x_woodDimension.Location = new System.Drawing.Point(48, 33);
            this.x_woodDimension.Name = "x_woodDimension";
            this.x_woodDimension.Size = new System.Drawing.Size(110, 20);
            this.x_woodDimension.TabIndex = 0;
            this.x_woodDimension.Text = "x dimension (inches)";
            this.x_woodDimension.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // y_woodDimension
            // 
            this.y_woodDimension.Location = new System.Drawing.Point(180, 33);
            this.y_woodDimension.Name = "y_woodDimension";
            this.y_woodDimension.Size = new System.Drawing.Size(110, 20);
            this.y_woodDimension.TabIndex = 1;
            this.y_woodDimension.Text = "y dimension (inches)";
            this.y_woodDimension.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // setDimension
            // 
            this.setDimension.Location = new System.Drawing.Point(50, 58);
            this.setDimension.Name = "setDimension";
            this.setDimension.Size = new System.Drawing.Size(106, 23);
            this.setDimension.TabIndex = 2;
            this.setDimension.Text = "Set Dimensions";
            this.setDimension.UseVisualStyleBackColor = true;
            this.setDimension.Click += new System.EventHandler(this.setDimension_Click);
            // 
            // woodDimensionDialog
            // 
            this.woodDimensionDialog.BackColor = System.Drawing.SystemColors.Control;
            this.woodDimensionDialog.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.woodDimensionDialog.Enabled = false;
            this.woodDimensionDialog.Location = new System.Drawing.Point(-7, 12);
            this.woodDimensionDialog.Name = "woodDimensionDialog";
            this.woodDimensionDialog.Size = new System.Drawing.Size(360, 13);
            this.woodDimensionDialog.TabIndex = 2;
            this.woodDimensionDialog.Text = "Please set the size of your plaque in inches:";
            this.woodDimensionDialog.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // closeDim
            // 
            this.closeDim.Location = new System.Drawing.Point(183, 58);
            this.closeDim.Name = "closeDim";
            this.closeDim.Size = new System.Drawing.Size(106, 23);
            this.closeDim.TabIndex = 3;
            this.closeDim.Text = "Close";
            this.closeDim.UseVisualStyleBackColor = true;
            this.closeDim.Click += new System.EventHandler(this.closeDim_Click_1);
            // 
            // woodDimensions
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(344, 93);
            this.Controls.Add(this.closeDim);
            this.Controls.Add(this.woodDimensionDialog);
            this.Controls.Add(this.setDimension);
            this.Controls.Add(this.y_woodDimension);
            this.Controls.Add(this.x_woodDimension);
            this.Name = "woodDimensions";
            this.Text = "Wood Dimensions";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox x_woodDimension;
        private System.Windows.Forms.TextBox y_woodDimension;
        private System.Windows.Forms.TextBox woodDimensionDialog;
        private System.Windows.Forms.Button closeDim;
        private System.Windows.Forms.Button setDimension;
    }
}