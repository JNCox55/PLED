namespace PLED_GUI
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.SelectWoodDimensions = new System.Windows.Forms.Button();
            this.y_woodDimension = new System.Windows.Forms.TextBox();
            this.x_woodDimension = new System.Windows.Forms.TextBox();
            this.woodDimensionUnits = new System.Windows.Forms.TextBox();
            this.xWoodDimPixels = new System.Windows.Forms.TextBox();
            this.yWoodDimPixels = new System.Windows.Forms.TextBox();
            this.woodDimPixUnit = new System.Windows.Forms.TextBox();
            this.endPLED = new System.Windows.Forms.Button();
            this.submitPLED = new System.Windows.Forms.Button();
            this.xSlider = new System.Windows.Forms.TrackBar();
            this.xLocText = new System.Windows.Forms.TextBox();
            this.yLocText = new System.Windows.Forms.TextBox();
            this.ySlider = new System.Windows.Forms.TrackBar();
            this.xLocBox = new System.Windows.Forms.TextBox();
            this.EngTime = new System.Windows.Forms.TextBox();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.imgLoad = new System.Windows.Forms.Button();
            this.imgPath = new System.Windows.Forms.TextBox();
            this.ximgsizebox = new System.Windows.Forms.TextBox();
            this.yimgsizebox = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.ximgslider = new System.Windows.Forms.TrackBar();
            this.yimgslider = new System.Windows.Forms.TrackBar();
            this.PlaqueSize = new System.Windows.Forms.TextBox();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.cent = new System.Windows.Forms.Button();
            this.horzCent = new System.Windows.Forms.Button();
            this.vertCent = new System.Windows.Forms.Button();
            this.imgBox = new System.Windows.Forms.PictureBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.yLocBox = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.xSlider)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ySlider)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ximgslider)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.yimgslider)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.imgBox)).BeginInit();
            this.SuspendLayout();
            // 
            // SelectWoodDimensions
            // 
            this.SelectWoodDimensions.Location = new System.Drawing.Point(13, 15);
            this.SelectWoodDimensions.Name = "SelectWoodDimensions";
            this.SelectWoodDimensions.Size = new System.Drawing.Size(140, 20);
            this.SelectWoodDimensions.TabIndex = 0;
            this.SelectWoodDimensions.Text = "Set Wood Dimensions";
            this.SelectWoodDimensions.UseVisualStyleBackColor = true;
            this.SelectWoodDimensions.Click += new System.EventHandler(this.SelectWoodDimensions_Click);
            // 
            // y_woodDimension
            // 
            this.y_woodDimension.BackColor = System.Drawing.SystemColors.Info;
            this.y_woodDimension.Enabled = false;
            this.y_woodDimension.Location = new System.Drawing.Point(243, 15);
            this.y_woodDimension.Name = "y_woodDimension";
            this.y_woodDimension.Size = new System.Drawing.Size(70, 20);
            this.y_woodDimension.TabIndex = 1;
            this.y_woodDimension.Text = "y dimension";
            this.y_woodDimension.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // x_woodDimension
            // 
            this.x_woodDimension.BackColor = System.Drawing.SystemColors.Info;
            this.x_woodDimension.Enabled = false;
            this.x_woodDimension.Location = new System.Drawing.Point(163, 15);
            this.x_woodDimension.Name = "x_woodDimension";
            this.x_woodDimension.Size = new System.Drawing.Size(70, 20);
            this.x_woodDimension.TabIndex = 2;
            this.x_woodDimension.Text = "x dimension";
            this.x_woodDimension.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // woodDimensionUnits
            // 
            this.woodDimensionUnits.BackColor = System.Drawing.SystemColors.Control;
            this.woodDimensionUnits.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.woodDimensionUnits.Enabled = false;
            this.woodDimensionUnits.Location = new System.Drawing.Point(323, 18);
            this.woodDimensionUnits.Name = "woodDimensionUnits";
            this.woodDimensionUnits.Size = new System.Drawing.Size(70, 13);
            this.woodDimensionUnits.TabIndex = 1;
            this.woodDimensionUnits.Text = "inches";
            // 
            // xWoodDimPixels
            // 
            this.xWoodDimPixels.BackColor = System.Drawing.SystemColors.Info;
            this.xWoodDimPixels.Enabled = false;
            this.xWoodDimPixels.Location = new System.Drawing.Point(163, 41);
            this.xWoodDimPixels.Name = "xWoodDimPixels";
            this.xWoodDimPixels.Size = new System.Drawing.Size(70, 20);
            this.xWoodDimPixels.TabIndex = 2;
            this.xWoodDimPixels.Text = "x dimension";
            this.xWoodDimPixels.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // yWoodDimPixels
            // 
            this.yWoodDimPixels.BackColor = System.Drawing.SystemColors.Info;
            this.yWoodDimPixels.Enabled = false;
            this.yWoodDimPixels.Location = new System.Drawing.Point(243, 41);
            this.yWoodDimPixels.Name = "yWoodDimPixels";
            this.yWoodDimPixels.Size = new System.Drawing.Size(70, 20);
            this.yWoodDimPixels.TabIndex = 1;
            this.yWoodDimPixels.Text = "y dimension";
            this.yWoodDimPixels.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // woodDimPixUnit
            // 
            this.woodDimPixUnit.BackColor = System.Drawing.SystemColors.Control;
            this.woodDimPixUnit.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.woodDimPixUnit.Enabled = false;
            this.woodDimPixUnit.Location = new System.Drawing.Point(323, 44);
            this.woodDimPixUnit.Name = "woodDimPixUnit";
            this.woodDimPixUnit.Size = new System.Drawing.Size(70, 13);
            this.woodDimPixUnit.TabIndex = 1;
            this.woodDimPixUnit.Text = "pixels";
            // 
            // endPLED
            // 
            this.endPLED.Location = new System.Drawing.Point(734, 350);
            this.endPLED.Name = "endPLED";
            this.endPLED.Size = new System.Drawing.Size(60, 20);
            this.endPLED.TabIndex = 3;
            this.endPLED.Text = "Close";
            this.endPLED.UseVisualStyleBackColor = true;
            this.endPLED.Click += new System.EventHandler(this.endPLED_Click_1);
            // 
            // submitPLED
            // 
            this.submitPLED.Location = new System.Drawing.Point(668, 350);
            this.submitPLED.Name = "submitPLED";
            this.submitPLED.Size = new System.Drawing.Size(60, 20);
            this.submitPLED.TabIndex = 2;
            this.submitPLED.Text = "Submit";
            this.submitPLED.UseVisualStyleBackColor = true;
            this.submitPLED.Click += new System.EventHandler(this.submitPLED_Click);
            // 
            // xSlider
            // 
            this.xSlider.Location = new System.Drawing.Point(13, 252);
            this.xSlider.Maximum = 0;
            this.xSlider.Name = "xSlider";
            this.xSlider.Size = new System.Drawing.Size(300, 45);
            this.xSlider.TabIndex = 3;
            this.xSlider.Scroll += new System.EventHandler(this.xSlider_Scroll);
            // 
            // xLocText
            // 
            this.xLocText.BackColor = System.Drawing.SystemColors.Control;
            this.xLocText.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.xLocText.Enabled = false;
            this.xLocText.Location = new System.Drawing.Point(13, 233);
            this.xLocText.Name = "xLocText";
            this.xLocText.Size = new System.Drawing.Size(380, 13);
            this.xLocText.TabIndex = 1;
            this.xLocText.Text = "Adjust x location in pixels";
            // 
            // yLocText
            // 
            this.yLocText.BackColor = System.Drawing.SystemColors.Control;
            this.yLocText.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.yLocText.Enabled = false;
            this.yLocText.Location = new System.Drawing.Point(440, 354);
            this.yLocText.Name = "yLocText";
            this.yLocText.Size = new System.Drawing.Size(120, 13);
            this.yLocText.TabIndex = 1;
            this.yLocText.Text = "Estimated engrave time:";
            // 
            // ySlider
            // 
            this.ySlider.Location = new System.Drawing.Point(13, 314);
            this.ySlider.Maximum = 0;
            this.ySlider.Name = "ySlider";
            this.ySlider.Size = new System.Drawing.Size(300, 45);
            this.ySlider.TabIndex = 3;
            this.ySlider.Scroll += new System.EventHandler(this.ySlider_Scroll);
            // 
            // xLocBox
            // 
            this.xLocBox.BackColor = System.Drawing.SystemColors.Info;
            this.xLocBox.Enabled = false;
            this.xLocBox.Location = new System.Drawing.Point(323, 252);
            this.xLocBox.Name = "xLocBox";
            this.xLocBox.Size = new System.Drawing.Size(70, 20);
            this.xLocBox.TabIndex = 2;
            this.xLocBox.Text = "x location";
            this.xLocBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // EngTime
            // 
            this.EngTime.BackColor = System.Drawing.SystemColors.Info;
            this.EngTime.Enabled = false;
            this.EngTime.Location = new System.Drawing.Point(561, 351);
            this.EngTime.MaxLength = 4;
            this.EngTime.Name = "EngTime";
            this.EngTime.Size = new System.Drawing.Size(81, 20);
            this.EngTime.TabIndex = 2;
            this.EngTime.Text = "time";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "Load Image";
            this.openFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // imgLoad
            // 
            this.imgLoad.Location = new System.Drawing.Point(13, 67);
            this.imgLoad.Name = "imgLoad";
            this.imgLoad.Size = new System.Drawing.Size(75, 23);
            this.imgLoad.TabIndex = 1;
            this.imgLoad.Text = "Load Image";
            this.imgLoad.UseVisualStyleBackColor = true;
            this.imgLoad.Click += new System.EventHandler(this.imgLoad_Click);
            // 
            // imgPath
            // 
            this.imgPath.BackColor = System.Drawing.SystemColors.Control;
            this.imgPath.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.imgPath.Enabled = false;
            this.imgPath.Location = new System.Drawing.Point(98, 72);
            this.imgPath.Name = "imgPath";
            this.imgPath.Size = new System.Drawing.Size(295, 13);
            this.imgPath.TabIndex = 1;
            this.imgPath.Text = "Image Path";
            // 
            // ximgsizebox
            // 
            this.ximgsizebox.BackColor = System.Drawing.SystemColors.Info;
            this.ximgsizebox.Enabled = false;
            this.ximgsizebox.Location = new System.Drawing.Point(323, 124);
            this.ximgsizebox.Name = "ximgsizebox";
            this.ximgsizebox.Size = new System.Drawing.Size(70, 20);
            this.ximgsizebox.TabIndex = 2;
            this.ximgsizebox.Text = "x pixels";
            this.ximgsizebox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // yimgsizebox
            // 
            this.yimgsizebox.BackColor = System.Drawing.SystemColors.Info;
            this.yimgsizebox.Enabled = false;
            this.yimgsizebox.Location = new System.Drawing.Point(323, 186);
            this.yimgsizebox.Name = "yimgsizebox";
            this.yimgsizebox.Size = new System.Drawing.Size(70, 20);
            this.yimgsizebox.TabIndex = 2;
            this.yimgsizebox.Text = "y pixels";
            this.yimgsizebox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // textBox3
            // 
            this.textBox3.BackColor = System.Drawing.SystemColors.Control;
            this.textBox3.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox3.Enabled = false;
            this.textBox3.Location = new System.Drawing.Point(13, 105);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(380, 13);
            this.textBox3.TabIndex = 1;
            this.textBox3.Text = "Adjust x image pixel count";
            // 
            // textBox4
            // 
            this.textBox4.BackColor = System.Drawing.SystemColors.Control;
            this.textBox4.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox4.Enabled = false;
            this.textBox4.Location = new System.Drawing.Point(13, 167);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(380, 13);
            this.textBox4.TabIndex = 1;
            this.textBox4.Text = "Adjust y image pixel count";
            // 
            // ximgslider
            // 
            this.ximgslider.Location = new System.Drawing.Point(13, 124);
            this.ximgslider.Maximum = 0;
            this.ximgslider.Name = "ximgslider";
            this.ximgslider.Size = new System.Drawing.Size(300, 45);
            this.ximgslider.TabIndex = 5;
            this.ximgslider.Scroll += new System.EventHandler(this.ximgslider_Scroll);
            // 
            // yimgslider
            // 
            this.yimgslider.Location = new System.Drawing.Point(13, 186);
            this.yimgslider.Maximum = 0;
            this.yimgslider.Name = "yimgslider";
            this.yimgslider.Size = new System.Drawing.Size(300, 45);
            this.yimgslider.TabIndex = 6;
            this.yimgslider.Scroll += new System.EventHandler(this.yimgslider_Scroll);
            // 
            // PlaqueSize
            // 
            this.PlaqueSize.BackColor = System.Drawing.Color.BurlyWood;
            this.PlaqueSize.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PlaqueSize.Enabled = false;
            this.PlaqueSize.Location = new System.Drawing.Point(440, 13);
            this.PlaqueSize.MaximumSize = new System.Drawing.Size(330, 330);
            this.PlaqueSize.Multiline = true;
            this.PlaqueSize.Name = "PlaqueSize";
            this.PlaqueSize.Size = new System.Drawing.Size(330, 330);
            this.PlaqueSize.TabIndex = 7;
            // 
            // cent
            // 
            this.cent.Location = new System.Drawing.Point(293, 354);
            this.cent.Name = "cent";
            this.cent.Size = new System.Drawing.Size(100, 23);
            this.cent.TabIndex = 9;
            this.cent.Text = "Center";
            this.cent.UseVisualStyleBackColor = true;
            this.cent.Click += new System.EventHandler(this.cent_Click);
            // 
            // horzCent
            // 
            this.horzCent.Location = new System.Drawing.Point(13, 354);
            this.horzCent.Name = "horzCent";
            this.horzCent.Size = new System.Drawing.Size(100, 23);
            this.horzCent.TabIndex = 10;
            this.horzCent.Text = "Horizontal Center";
            this.horzCent.UseVisualStyleBackColor = true;
            this.horzCent.Click += new System.EventHandler(this.horzCent_Click);
            // 
            // vertCent
            // 
            this.vertCent.Location = new System.Drawing.Point(153, 354);
            this.vertCent.Name = "vertCent";
            this.vertCent.Size = new System.Drawing.Size(100, 23);
            this.vertCent.TabIndex = 11;
            this.vertCent.Text = "Vertical Center";
            this.vertCent.UseVisualStyleBackColor = true;
            this.vertCent.Click += new System.EventHandler(this.vertCent_Click);
            // 
            // imgBox
            // 
            this.imgBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.imgBox.Location = new System.Drawing.Point(440, 13);
            this.imgBox.Name = "imgBox";
            this.imgBox.Size = new System.Drawing.Size(100, 50);
            this.imgBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.imgBox.TabIndex = 12;
            this.imgBox.TabStop = false;
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.SystemColors.Info;
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Enabled = false;
            this.textBox1.Location = new System.Drawing.Point(597, 354);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(43, 13);
            this.textBox1.TabIndex = 1;
            this.textBox1.Text = "  hours";
            // 
            // textBox2
            // 
            this.textBox2.BackColor = System.Drawing.SystemColors.Control;
            this.textBox2.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox2.Enabled = false;
            this.textBox2.Location = new System.Drawing.Point(13, 295);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(380, 13);
            this.textBox2.TabIndex = 1;
            this.textBox2.Text = "Adjust y location in pixels";
            // 
            // yLocBox
            // 
            this.yLocBox.BackColor = System.Drawing.SystemColors.Info;
            this.yLocBox.Enabled = false;
            this.yLocBox.Location = new System.Drawing.Point(323, 314);
            this.yLocBox.Name = "yLocBox";
            this.yLocBox.Size = new System.Drawing.Size(70, 20);
            this.yLocBox.TabIndex = 2;
            this.yLocBox.Text = "y location";
            this.yLocBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(806, 382);
            this.Controls.Add(this.imgBox);
            this.Controls.Add(this.vertCent);
            this.Controls.Add(this.horzCent);
            this.Controls.Add(this.cent);
            this.Controls.Add(this.PlaqueSize);
            this.Controls.Add(this.yimgslider);
            this.Controls.Add(this.ximgslider);
            this.Controls.Add(this.imgLoad);
            this.Controls.Add(this.ySlider);
            this.Controls.Add(this.xSlider);
            this.Controls.Add(this.submitPLED);
            this.Controls.Add(this.endPLED);
            this.Controls.Add(this.imgPath);
            this.Controls.Add(this.woodDimPixUnit);
            this.Controls.Add(this.textBox4);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.yLocText);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.xLocText);
            this.Controls.Add(this.woodDimensionUnits);
            this.Controls.Add(this.yWoodDimPixels);
            this.Controls.Add(this.yimgsizebox);
            this.Controls.Add(this.y_woodDimension);
            this.Controls.Add(this.ximgsizebox);
            this.Controls.Add(this.EngTime);
            this.Controls.Add(this.yLocBox);
            this.Controls.Add(this.xLocBox);
            this.Controls.Add(this.xWoodDimPixels);
            this.Controls.Add(this.x_woodDimension);
            this.Controls.Add(this.SelectWoodDimensions);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "PLED";
            ((System.ComponentModel.ISupportInitialize)(this.xSlider)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ySlider)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ximgslider)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.yimgslider)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.imgBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button SelectWoodDimensions;
        private System.Windows.Forms.TextBox y_woodDimension;
        private System.Windows.Forms.TextBox x_woodDimension;
        private System.Windows.Forms.TextBox woodDimensionUnits;
        private System.Windows.Forms.TextBox xWoodDimPixels;
        private System.Windows.Forms.TextBox yWoodDimPixels;
        private System.Windows.Forms.TextBox woodDimPixUnit;
        private System.Windows.Forms.Button endPLED;
        private System.Windows.Forms.Button submitPLED;
        private System.Windows.Forms.TrackBar xSlider;
        private System.Windows.Forms.TextBox xLocText;
        private System.Windows.Forms.TextBox yLocText;
        private System.Windows.Forms.TrackBar ySlider;
        private System.Windows.Forms.TextBox xLocBox;
        private System.Windows.Forms.TextBox EngTime;
        private System.Windows.Forms.Button imgLoad;
        private System.Windows.Forms.TextBox imgPath;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.TextBox ximgsizebox;
        private System.Windows.Forms.TextBox yimgsizebox;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TrackBar ximgslider;
        private System.Windows.Forms.TrackBar yimgslider;
        private System.Windows.Forms.TextBox PlaqueSize;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Button cent;
        private System.Windows.Forms.Button horzCent;
        private System.Windows.Forms.Button vertCent;
        private System.Windows.Forms.PictureBox imgBox;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox yLocBox;
    }
}

