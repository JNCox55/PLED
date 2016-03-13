using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.IO;

namespace IngestImage
{
    class Program
    {
        public void Ingest(int xImageDim, int yImageDim, int xLocation, int yLocation, String inPath, String outPath)
        {
            //define image and file input
            Image inputImage = Image.FromFile(inPath);
            //c is original image in bitmap
            Bitmap c = new Bitmap(inputImage);
            //d is new, modified image in bitmap
            Bitmap d = new Bitmap(inputImage, new Size(xImageDim, yImageDim));

            /*G-Code interpretations:

              G00 X--.-- Y--.-- Z--.-- Rapid Positioning    (units in mm, redefine?)
              G01 X--.-- Y--.-- Z--.-- Linear Interpolation (units in mm, redefine?)
              G04 P--.--               Dwell                (units in seconds, redefine?)
              M04                      Spindle on           (Laser on?)
              M05                      Spindle off          (Laser off?)
              M02                      End of program
              S--                      Set speed of spindle (Laser intensity?)

            */

            //define which x and y pixel we are at
            int row = 0;
            int column = -1;

            //define and create file for G codes
            string path = outPath + ".gcode";
            if (!File.Exists(path))
            {
                using (StreamWriter sw = File.CreateText(path))
                {
                    //define image size
                    sw.WriteLine("size " + d.Width + "," + d.Height);

                    //write go to starting point of engraving and dwell G-Codes to file
                    sw.WriteLine("G00 X" + xLocation + " Y" + yLocation);
                    sw.WriteLine("G04 P0.5");

                    //loop pulls in pixels, converts to grayscale.  i = image width (rows), x = image height (columns)
                    for (int i = 0; i < d.Height; i++)
                    {
                        //increase which row (y value) we are at
                        if (row != 0 || column != -1)
                        {
                            row++;
                        }

                        //makes sure at right pixels
                        if ((row % 2 == 0 && row != 0 && column != 0) || column == d.Width - 1)
                        {
                            column++;
                        }
                        if((row %2 != 0 && row != 0 && column != d.Width) || column == 0)
                        {
                            column--;
                        }

                        for (int x = 0; x < d.Width; x++)
                        {
                            //increase/decreases which column (x value) we are at
                            if (row % 2 != 0)
                            {
                                column--;
                            }
                            else
                            {
                                column++;
                            }

                            //select pixel
                            Color oc = d.GetPixel(x, i);

                            //define to grayscale
                            int grayScale = (int)((oc.R * 0.3) + (oc.G * 0.59) + (oc.B * 0.11));
                    
                            //truncate to 8 shades
                            grayScale = grayScale / 32;
                    
                            //move back to being in increments of 32 - creates better variance in shades
                            grayScale = grayScale * 32;

                            //set new pixel to grayscale
                            Color nc = Color.FromArgb(oc.A, grayScale, grayScale, grayScale);

                            //write pixel to specified position
                            d.SetPixel(x, i, nc);

                            //G-Code to move to next pixel
                            sw.WriteLine("G01 X" + (column + xLocation) + " Y" + (row + yLocation));

                            //G-Code to set intensity of laser
                            sw.WriteLine("S" + grayScale / 32);

                            //G-Code to turn on laser
                            sw.WriteLine("M04");

                            //G-Code for dwell
                            sw.WriteLine("G04 P0.015");

                            //G-Code to turn off laser
                            sw.WriteLine("M05");
                        }
                    }

                    //End G-Code File
                    sw.WriteLine("M02");
                    sw.Close();
                }
            }
            //save new image
            d.Save(outPath + ".jpg");

            //dispose of bitmap variables
            c.Dispose();
            d.Dispose();

            //end program
            return;
        }
    }
}
