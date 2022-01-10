using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using Alchemi.Core;
using Alchemi.Core.Owner;
using Swensen;

namespace Problem_02
{
    class Program : GApplication
    {
        static public GApplication App       = new GApplication();
        static public List<List<BigInt>> Matrix = new List<List<BigInt>>();
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

        static public List<List<BigInt>> SubMatrix(int start_idx, int end_idx)
        {
            List<List<BigInt>> sendMatrix = new List<List<BigInt>>();
            for (int i = start_idx; i <= end_idx; i++)
            {
                List<BigInt> temp = new List<BigInt>();
                for (int j = 0; j < Matrix[0].Count; j++)
                {
                    temp.Add(Matrix[i][j]);
                }
                sendMatrix.Add(temp);
            }
            return sendMatrix;
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
                List<BigInt> temp = new List<BigInt>();
                for (int j = 0; j < n; j++)
                {
                    temp.Add((BigInt)rnd.Next(1, 1000));        // random numbers
                }
                Matrix.Add(temp);
            }
            PrintMatrix();

            var clusters = m / k;
            for (int i = 0; i < clusters; i++)
            {
                if (i == clusters - 1)
                {
                    var sendMatrix = SubMatrix(i * k, m - 1);
                    App.Threads.Add(new LCMCalculator(sendMatrix, i * k, m - 1));
                }
                else
                {
                    var sendMatrix = SubMatrix(i * k, i * k + k - 1);
                    App.Threads.Add(new LCMCalculator(sendMatrix, i * k, i * k + k - 1));
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
        public List<List<BigInt>> Matrix;
        public List<BigInt> results = new List<BigInt>();

        public LCMCalculator(List<List<BigInt>> Matrix, int start, int end)
        {
            this.Matrix = Matrix;
            this.start  = start;
            this.end    = end;
        }

        static public BigInt LCM(BigInt a, BigInt b)
        {
            BigInt res = a * b;
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
            for (int i = 0; i < Matrix.Count; i++)
            {
                BigInt lcm = 1;
                for (int j = 0; j < Matrix[i].Count; j++)
                {
                    lcm = LCM(lcm, Matrix[i][j]);
                }
                results.Add(lcm);
            }
        }
    }
}