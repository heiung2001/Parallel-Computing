using System;
using System.Collections.Generic;
using System.Text;
using Alchemi.Core.Owner;
using Alchemi.Core;

namespace SquareNumbers
{
    class Program : GApplication
    {
        static public GApplication App = new GApplication();
        static public DateTime startTime;

        [STAThread]
        static public void Main(string[] args)
        {
            Console.WriteLine("Listing Square Numbers");
            Console.WriteLine("----------------------");

            int n, k;
            Console.Write("Enter an integer: ");
            n = Int32.Parse(Console.ReadLine());
            Console.Write("Enter number of works that each thread need to handle: ");
            k = Int32.Parse(Console.ReadLine());

            var clusters = (int)Math.Ceiling((double)n / k);
            for (int i = 0; i < clusters; i++)
            {
                if (i == clusters - 1)
                {
                    App.Threads.Add(new CheckSquareNumber(i * k + 1, n));
                }
                else
                {
                    App.Threads.Add(new CheckSquareNumber(i * k + 1, i * k + k));
                }
            }

            GConnection gconn   = GConnection.FromConsole("localhost", "9000", "user", "user");
            App.ApplicationName = "Square Numbers - Alchemi";
            App.Connection      = gconn;

            App.Manifest.Add(new ModuleDependency(typeof(CheckSquareNumber).Module));
            App.ThreadFinish      += App_ThreadFinish;
            App.ApplicationFinish += new GApplicationFinish(App_ApplicationFinish);

            startTime = DateTime.Now;
            Console.WriteLine("Thread started!");
            App.Start();
            Console.ReadLine();
        }

        static private void App_ThreadFinish(GThread thread)
        {
            CheckSquareNumber work = thread as CheckSquareNumber;
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
    class CheckSquareNumber : GThread
    {
        public int start, end;
        public List<int> results = new List<int> ();

        public CheckSquareNumber(int start, int end)
        {
            this.start = start;
            this.end   = end;
        }

        public override void Start()
        {
            var lower = (int)Math.Ceiling(Math.Sqrt(start));
            var upper = (int)Math.Floor(Math.Sqrt(end));

            for (int i = lower; i <= upper; i++)
            {
                results.Add(i*i);
            }
        }
    }
}
