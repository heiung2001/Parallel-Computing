using System;
using System.Collections.Generic;
using Alchemi.Core;
using Alchemi.Core.Utility;
using Alchemi.Core.Owner;
using Swensen;

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

            var n = BigInt.Parse(Utils.ValueFromConsole("Enter an integer","100"));
            var k = BigInt.Parse(Utils.ValueFromConsole("Enter the number of nums per thread", "5"));

            var num_threads = n / k;
            for (BigInt i = 0; i < num_threads; i++)
            {
                if (i == num_threads - 1)
                {
                    App.Threads.Add(new FibListing((BigInt)(i * k), (BigInt)n));
                }
                else
                {
                    App.Threads.Add(new FibListing((BigInt)(i * k), (BigInt)(i * k + k - 1)));
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
            for (var i = 0; i < work.results.Count; i++)
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
        public BigInt start, end;
        public List<BigInt> results = new List<BigInt>();

        public FibListing(BigInt start, BigInt end)
        {
            this.start = start;
            this.end   = end;
        }

        public bool IsPerfectSquare(BigInt x)
        {
            BigInt squ = BigInt.Sqrt(x);
            return (squ * squ == x);
        }

        public bool IsFibonacci(BigInt x)
        {
            return IsPerfectSquare(5 * x * x + 4) || IsPerfectSquare(5 * x * x - 4);
        }

        public override void Start()
        {
            for (var i = start; i <= end; i++)
            {
                if (IsFibonacci(i) == true)
                {
                    results.Add(i);
                }
            }
        }
    }
}
