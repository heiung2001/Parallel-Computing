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
                for (int j = 0; j < Matrix[0].Count; j++)
                {
                    Console.Write(Matrix[i][j]);
                }
                Console.Write("\n");
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
            // DisplayMatrix();
        }

        [STAThread]
        public static void Main(string[] args)
        {
            int m, n, k;
            Console.WriteLine("[Solution for Problem 2, Alchemi]\n--------------------------------\n");
            Console.WriteLine("Press <enter> to start ...");
            Console.ReadLine();

            ProcessInitial(out m, out n, out k);
            DisplayMatrix();

            //for (int i = 0; i < m / k + 1; i++)
            //{
            //    if ((i + 1) * k <= m && i * k < m)
            //    {
            //        App.Threads.Add(new LCMcalculator(Matrix, i * k, i * k + k - 1));
            //    }
            //    else if (i * k <= m)
            //    {
            //        App.Threads.Add(new LCMcalculator(Matrix, i * k, m - 1));
            //        Console.WriteLine(i * k);
            //    }
            //}

            var cluster = m / k + 1;
            for (int i = 0; i < cluster; i++)
            {
                if (i == cluster - 1)
                {
                    App.Threads.Add(new LCMcalculator(Matrix, i * k, m - 1));
                }
                else
                {
                    App.Threads.Add(new LCMcalculator(Matrix, i * k, i * k + k - 1));
                }
            }


            GConnection gc = GConnection.FromConsole("localhost", "9000", "user", "user");

            startTime = DateTime.Now;
            //App = new GApplication(gc);
            App.Connection = gc;
            App.ApplicationName = "LCM Calculator - Alchemi";
            App.Manifest.Add(new ModuleDependency(typeof(LCMcalculator).Module));
            App.ThreadFinish += App_ThreadFinish;
            App.ApplicationFinish += new GApplicationFinish(App_ApplicationFinish);
            startTime = DateTime.Now;
            Console.WriteLine("Thread started");
            App.Start();
            Console.ReadLine();

            //App.Connection = new GConnection("localhost", 9000, "user", "user");
            //App.Manifest.Add(new ModuleDependency(typeof(LCMcalculator).Module));
            //App.ThreadFinish += App_ThreadFinish;
            //App.ApplicationName = "BCNN";
            //App.ApplicationFinish += new GApplicationFinish(App_ApplicationFinish);
            //startTime = DateTime.Now;
            //Console.WriteLine("Thread Started");
            //App.Start();
            //Console.ReadLine();
        }

        private static void App_ThreadFinish(GThread thread)
        {
            LCMcalculator s = thread as LCMcalculator;
            Console.Write("Luong {0} ({1}:{2}):", thread.Id, s.start, s.end);
            for (int i = 0; i < s.ans.Count; i++)
            {
                Console.Write(s.ans[i] + " ");
            }
            Console.Write("\n");
        }

        private static void App_ApplicationFinish()
        {
            Console.WriteLine("Calculation finished after {0} seconds", DateTime.Now - startTime);
        }
    }

    [Serializable]
    class LCMcalculator : GThread
    {
        public int start, end;
        public List<List<int>> Matrix;
        public List<int> ans = new List<int>();

        public LCMcalculator(List<List<int>> Matrix, int start, int end)
        {
            this.Matrix = Matrix;
            this.start = start;
            this.end = end;
        }

        public static int LCM(int a, int b)
        {
            int result = a * b;
            while (a != 0 && b != 0)
            {
                if (a > b)
                    a %= b;
                else
                    b %= a;
            }
            return a > 0 ? result / a : result / b;
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
                ans.Add(lcm);
            }
        }
    }
}