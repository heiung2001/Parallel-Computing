using System;
using System.Collections.Generic;
using System.Text;
using Alchemi.Core;
using Alchemi.Core.Owner;

namespace Problem_02
{
    class MultiMatrix : GApplication
    {
        public static GApplication App = new GApplication();
        public static List<List<int>> Matrix = new List<List<int>>();
        private static DateTime startTime;

        public static void DisplayMatrix()
        {
            for (int i = 0; i < Matrix.Count; i++)
            {
                Console.Write("\n");
                for (int j = 0; j < Matrix[0].Count; j++)
                {
                    Console.Write(Matrix[i][j]);
                }
            }
        }

        public static void ProcessInitial(out int m, out int n, out int k)
        {
            Random rnd = new Random();
            Console.Write("Input Number of Rows: ");
            m = int.Parse(Console.ReadLine());
            Console.Write($"Input Number of Cols: ");
            n = int.Parse(Console.ReadLine());
            Console.Write($"Define Number of Rows in Each Thread: ");
            k = int.Parse(Console.ReadLine());

            for (int i = 0; i < m; i++)
            {
                List<int> temp = new List<int>();
                for (int j = 0; j < n; j++)
                {
                    temp.Add(rnd.Next(1, 10));
                }
                Matrix.Add(temp);
            }
            DisplayMatrix();
        }

        [STAThread]
        public static void Main(string[] args)
        {
            int m, n, k;
            Console.WriteLine("[Solution for Problem 2, Alchemi]\n--------------------------------\n");
            Console.WriteLine("Press <enter> to start ...");
            Console.ReadLine();

            ProcessInitial(out m, out n, out k);

            GConnection gc = GConnection.FromConsole("localhost", "9000", "user", "user");
            
            startTime = DateTime.Now;
            App = new GApplication(gc);
            App.ApplicationName = "LCM Calculator - Alchemi";
            
        }
    }
}