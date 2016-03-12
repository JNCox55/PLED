using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PLED_GUI
{
    public partial class woodDimensions : Form
    {
        public string ReturnValue1 { get; set; }
        public string ReturnValue2 { get; set; }

        public woodDimensions()
        {
            InitializeComponent();
        }

        private void setDimension_Click(object sender, EventArgs e)
        {
            if (x_woodDimension.Text == "x dimension (inches)" || y_woodDimension.Text == "x dimension (inches)"
            || Convert.ToDouble(x_woodDimension.Text) < 0 || Convert.ToDouble(x_woodDimension.Text) > 14.4 
            || Convert.ToDouble(y_woodDimension.Text) < 0 || Convert.ToDouble(y_woodDimension.Text) > 14.4)
            {
                this.ReturnValue1 = "Invalid Input";
                this.ReturnValue2 = "Invalid Input";
                this.DialogResult = DialogResult.Abort;
                this.Close();
            }

            else
            {
                this.ReturnValue1 = x_woodDimension.Text;
                this.ReturnValue2 = y_woodDimension.Text;
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
        }

        private void closeDim_Click_1(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Abort;
            this.Close();
        }
    }
}
