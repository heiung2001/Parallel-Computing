using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using Alchemi.Core;
using Alchemi.Core.Owner;

namespace Problem_02
{
    class Program : GApplication
    {
        static public GApplication App       = new GApplication();
        static public List<List<int>> Matrix = new List<List<int>>();
        static public DateTime startTime;

        static public void PrintMatrix()
        {
            using (TextWriter tw = new StreamWriter(@"..\..\matrix.txt"))
            {
                for (int i = 0; i < Matrix.Count; i++)
                {
                    for (int j = 0; j < Matrix[0].Count; j++)
                    {
                        tw.Write(Matrix[i][j] + " ");
                    }
                    tw.WriteLine();
                }
            }
        }

        [STAThread]
        static public void Main(string[] args)
        {
            Console.WriteLine("[Least Common Multiple - Grid Application]");
            Console.WriteLine("--------------------------------------------");

            Random rnd = new Random();
            Console.Write("Enter the number of Rows: ");
            var m = Int32.Parse(Console.ReadLine());
            Console.Write("Enter the number of Cols: ");
            var n = Int32.Parse(Console.ReadLine());
            Console.Write("Define the number of Rows in each thread: ");
            var k = Int32.Parse(Console.ReadLine());

            for (int i = 0; i < m; i++)
            {
                List<int> temp = new List<int>();
                for (int j = 0; j < n; j++)
                {
                    temp.Add(rnd.Next(1, 10));
                }
                Matrix.Add(temp);
            }
            PrintMatrix();

            var clusters = m / k;
            for (int i = 0; i < clusters; i++)
            {
                if (i == clusters - 1)
                {
                    App.Threads.Add(new LCMCalculator(Matrix, i * k, m - 1));
                }
                else
                {
                    App.Threads.Add(new LCMCalculator(Matrix, i * k, i * k + k - 1));
                }
            }

            Console.WriteLine("\n<Login the Alchemi Grid>");
            GConnection gconn   = GConnection.FromConsole("localhost", "9000", "user", "user");
            App.ApplicationName = "Least Common Multiple - Alchemi";
            App.Connection      = gconn;
            
            App.Manifest.Add(new ModuleDependency(typeof(LCMCalculator).Module));
            App.ThreadFinish      += App_ThreadFinish;
            App.ApplicationFinish += new GApplicationFinish(App_ApplicationFinish);

            startTime = DateTime.Now;
            Console.WriteLine("<Thread Started!>");
            App.Start();
            Console.ReadLine();
        }

        static private void App_ThreadFinish(GThread thread)
        {
            LCMCalculator work = thread as LCMCalculator;
            Console.Write("Thread {0} ({1}:{2}): ", thread.Id, work.start, work.end);
            for (int i = 0; i < work.results.Count; i++)
            {
                Console.Write(work.results[i] + " ");
            }
            Console.WriteLine("\n");
        }

        static private void App_ApplicationFinish()
        {
            Console.WriteLine("Calculation finished after {0} second", DateTime.Now - startTime);
        }
    }

    [Serializable]
    class LCMCalculator : GThread
    {
        public int start, end;
        public List<List<int>> Matrix;
        public List<int> results = new List<int>();

        public LCMCalculator(List<List<int>> Matrix, int start, int end)
        {
            this.Matrix = Matrix;
            this.start  = start;
            this.end    = end;
        }

        static public int LCM(int a, int b)
        {
            int res = a * b;
            while (a != 0 && b != 0)
            {
                if (a > b)
                    a %= b;
                else
                    b %= a;
            }
            return a > 0 ? res / a : res / b;
        }

        public override void Start()
        {
            for (int i = start; i <= end; i++)
            {
                int lcm = 1;
                for (int j = 0; j < Matrix[0].Count; j++)
                {
                    lcm = LCM(lcm, Matrix[i][j]);
                }
                results.Add(lcm);
            }
        }
    }
}