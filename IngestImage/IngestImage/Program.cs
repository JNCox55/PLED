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
        static void Main()
        {
            //define image and file input
            Image inputImage = Image.FromFile("C:/Users/Casey/Documents/projectTest/pokemon.jpg");
            //c is original image in bitmap
            Bitmap c = new Bitmap(inputImage);
            //d is new, modified image in bitmap
            Bitmap d = new Bitmap(c.Width, c.Height);

            /*G-Code interpretations:

              G00 X--.-- Y--.-- Z--.-- Rapid Positioning    (units in mm, redefine?)
              G01 X--.-- Y--.-- Z--.-- Linear Interpolation (units in mm, redefine?)
              G04 P--.--               Dwell                (units in seconds, redefine?)
              M04                      Spindle on           (Laser on?)
              M05                      Spindle off          (Laser off?)
              M02                      End of program
              S--                      Set speed of spindle (Laser intensity?)

            */

            //unit testing numbers
            int zero = 0;
            int one = 0;
            int two = 0;
            int three = 0;
            int four = 0;
            int five = 0;
            int six = 0;
            int seven = 0;

            //define which x and y pixel we are at
            int row = 0;
            int column = -1;

            //define g-code filename
            string gcodeFileName;

            //get c-code filename
            Console.WriteLine("Please enter G-Code file name:");
            gcodeFileName = Console.ReadLine();

            //define and create file for G codes
            string path = "C:/Users/Casey/Documents/projectTest/" + gcodeFileName + ".gcode";
            if (!File.Exists(path))
            {
                using (StreamWriter sw = File.CreateText(path))
                {
                    //define image size
                    sw.WriteLine("size " + c.Width + "," + c.Height);

                    //write go to starting point of engraving and dwell G-Codes to file
                    sw.WriteLine("G00 X0 Y0");
                    sw.WriteLine("G04 P0.5");

                    //loop pulls in pixels, converts to grayscale.  i = image width (rows), x = image height (columns)
                    for (int i = 0; i < c.Height; i++)
                    {
                        //increase which row (y value) we are at
                        if (row != 0 || column != -1)
                        {
                            row++;
                        }

                        //makes sure at right pixels
                        if ((row % 2 == 0 && row != 0 && column != 0) || column == c.Width - 1)
                        {
                            column++;
                        }
                        if((row %2 != 0 && row != 0 && column != c.Width) || column == 0)
                        {
                            column--;
                        }

                        for (int x = 0; x < c.Width; x++)
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
                            Color oc = c.GetPixel(x, i);

                            //define to grayscale
                            int grayScale = (int)((oc.R * 0.3) + (oc.G * 0.59) + (oc.B * 0.11));
                    
                            //truncate to 8 shades
                            grayScale = grayScale / 32;

                            //find number of pixels of each shade
                            if (grayScale == 0) {zero++;}
                            if (grayScale == 1) {one++;}
                            if (grayScale == 2) {two++;}
                            if (grayScale == 3) {three++;}
                            if (grayScale == 4) {four++;}
                            if (grayScale == 5) {five++;}
                            if (grayScale == 6) {six++;}
                            if (grayScale == 7) {seven++;}
                    
                            //move back to being in increments of 32 - creates better variance in shades
                            grayScale = grayScale * 32;

                            //set new pixel to grayscale
                            Color nc = Color.FromArgb(oc.A, grayScale, grayScale, grayScale);

                            //write pixel to specified position
                            d.SetPixel(x, i, nc);

                            //G-Code to move to next pixel
                            sw.WriteLine("G01 X" + column + " Y" + row);

                            //G-Code to set intensity of laser
                            sw.WriteLine("S" + grayScale / 32);

                            //G-Code to turn on laser
                            sw.WriteLine("M04");

                            //G-Code for dwell
                            sw.WriteLine("G04 P0.0015");

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
            d.Save("C:/Users/Casey/Documents/projectTest/TestSuckas.jpg");
            
            //tell how many pixels of each shade
            Console.Write("Zeroes: "); Console.Write(zero); Console.WriteLine();
            Console.Write("Ones: "); Console.Write(one); Console.WriteLine();
            Console.Write("Twos: "); Console.Write(two); Console.WriteLine();
            Console.Write("Threes: "); Console.Write(three); Console.WriteLine();
            Console.Write("Fours: "); Console.Write(four); Console.WriteLine();
            Console.Write("Fives: "); Console.Write(five); Console.WriteLine();
            Console.Write("Sixes: "); Console.Write(six); Console.WriteLine();
            Console.Write("Sevens: "); Console.Write(seven); Console.WriteLine();
            Console.ReadLine();

            //dispose of bitmap variables
            c.Dispose();
            d.Dispose();

            //end program
            return;
        }
    }
}
