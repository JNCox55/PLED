using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace IngestImage
{
    class Program
    {
        static void Main()
        {
            Image inputImage = Image.FromFile("C:/Users/Casey/Documents/projectTest/pokemon.jpg");
            Bitmap c = new Bitmap(inputImage);
            Bitmap d = new Bitmap(c.Width, c.Height);

            int zero = 0;
            int one = 0;
            int two = 0;
            int three = 0;
            int four = 0;
            int five = 0;
            int six = 0;
            int seven = 0;

            for (int i = 0; i < c.Width; i++)
            {
                for (int x = 0; x < c.Height; x++)
                {
                    Color oc = c.GetPixel(i, x);
                    int grayScale = (int)((oc.R * 0.3) + (oc.G * 0.59) + (oc.B * 0.11));
                    grayScale = grayScale / 32;

                    if (grayScale == 0) {zero++;}
                    if (grayScale == 1) {one++;}
                    if (grayScale == 2) {two++;}
                    if (grayScale == 3) {three++;}
                    if (grayScale == 4) {four++;}
                    if (grayScale == 5) {five++;}
                    if (grayScale == 6) {six++;}
                    if (grayScale == 7) {seven++;}
                    
                    grayScale = grayScale * 32;

                    Color nc = Color.FromArgb(oc.A, grayScale, grayScale, grayScale);
                    d.SetPixel(i, x, nc);
                }
            }

            d.Save("C:/Users/Casey/Documents/projectTest/TestSuckas.jpg");
            
            Console.Write("Zeroes: "); Console.Write(zero); Console.WriteLine();
            Console.Write("Ones: "); Console.Write(one); Console.WriteLine();
            Console.Write("Twos: "); Console.Write(two); Console.WriteLine();
            Console.Write("Threes: "); Console.Write(three); Console.WriteLine();
            Console.Write("Fours: "); Console.Write(four); Console.WriteLine();
            Console.Write("Fives: "); Console.Write(five); Console.WriteLine();
            Console.Write("Sixes: "); Console.Write(six); Console.WriteLine();
            Console.Write("Sevens: "); Console.Write(seven); Console.WriteLine();
            Console.ReadLine();

            c.Dispose();
            d.Dispose();

            return;
        }
    }
}
