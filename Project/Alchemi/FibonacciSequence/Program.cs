using System;
using System.Collections.Generic;
using Alchemi.Core;
using Alchemi.Core.Utility;
using Alchemi.Core.Owner;

namespace FibonacciSequence
{
    class Program : GApplication
    {
        static public GApplication App = new GApplication();
        static public DateTime startTime;

        [STAThread]
        static public void Main(string[] args)
        {
            Console.WriteLine("[Fibonacci Sequence - Grid Application]");
            Console.WriteLine("---------------------------------------");

            var n = Int32.Parse(Utils.ValueFromConsole("Enter an integer","100"));
            var k = Int32.Parse(Utils.ValueFromConsole("Enter the number of nums per thread", "5"));

            var num_threads = n / k;
            for (int i = 0; i < num_threads; i++)
            {
                if (i == num_threads - 1)
                {
                    App.Threads.Add(new FibListing(i * k, n));
                }
                else
                {
                    App.Threads.Add(new FibListing(i * k, i * k + k - 1));
                }
            }

            Console.WriteLine("\n<Login the Alchemi Grid>");
            GConnection gconn   = GConnection.FromConsole("localhost", "9000", "user", "user");
            App.ApplicationName = "Listing Fibonacci Sequence - Alchemi";
            App.Connection      = gconn;

            App.Manifest.Add(new ModuleDependency(typeof(FibListing).Module));
            App.ThreadFinish += App_ThreadFinish;
            App.ApplicationFinish += new GApplicationFinish(App_ApplicationFinish);

            startTime = DateTime.Now;
            Console.WriteLine("<Thread Started!>");
            App.Start();
            Console.ReadLine();
        }

        static private void App_ThreadFinish(GThread thread)
        {
            FibListing work = thread as FibListing;
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
    class FibListing : GThread
    {
        public int start, end;
        public List<int> results = new List<int>();

        public FibListing(int start, int end)
        {
            this.start = start;
            this.end   = end;
        }

        public bool IsPerfectSquare(int x)
        {
            var squ = (int)Math.Sqrt(x);
            return (squ * squ == x);
        }

        public bool IsFibonacci(int x)
        {
            return IsPerfectSquare(5 * x * x + 4) || IsPerfectSquare(5 * x * x - 4);
        }

        public override void Start()
        {
            for (int i = start; i <= end; i++)
            {
                if (IsFibonacci(i) == true)
                {
                    results.Add(i);
                }
            }
        }
    }
}
