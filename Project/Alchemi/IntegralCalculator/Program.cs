using System;
using System.Collections.Generic;
using System.Text;
using Alchemi.Core;
using Alchemi.Core.Owner;

namespace IntegralCalculator
{
    delegate double MathFunction(double x);

    class Program
    {
        static public GApplication App = new GApplication();
        static public DateTime startTime;
        static public double Sum_res = 0;

        [STAThread]
        static public void Main(string[] args)
        {
            Console.WriteLine("[Integral Computation Grid Application]");
            Console.WriteLine("---------------------------------------");

            Console.Write("Enter the upper bound of the Integral (the lower is 0): ");
            var n = Double.Parse(Console.ReadLine());
            Console.Write("Enter the number of threads: ");
            var num_threads = Int32.Parse(Console.ReadLine());

            var step = n / num_threads;
            for (int i = 0; i < num_threads; i++)
            {
                App.Threads.Add(new IntegralCal(Math.Cos, i*step, (i+1)*step)); // CHANGE function in here!
            }

            GConnection gconn   = GConnection.FromConsole("locahost", "9000", "user", "user");
            App.ApplicationName = "Integral Approximation - Alchemi";
            App.Connection      = gconn;

            App.Manifest.Add(new ModuleDependency(typeof(IntegralCal).Module));
            App.ThreadFinish += App_ThreadFinish;
            App.ApplicationFinish += new GApplicationFinish(App_ApplicationFinish);

            startTime = DateTime.Now;
            Console.WriteLine("Thread Started!");
            App.Start();
            Console.ReadLine();
        }

        static private void App_ThreadFinish(GThread thread)
        {
            IntegralCal work = thread as IntegralCal;
            Console.Write("Thread {0} ({1}:{2}): ", thread.Id, work.start, work.end);
            Sum_res += work.results;
            Console.Write(work.results + " ");
            Console.WriteLine("\n");
        }

        static private void App_ApplicationFinish()
        {
            Console.WriteLine("The result of the Integral: {0}", Sum_res);
            Console.WriteLine("Calculation finished after {0} second", DateTime.Now - startTime);
        }
    }

    [Serializable]
    class IntegralCal : GThread
    {
        public double start, end;
        public double results;
        public MathFunction Function { get; set; }

        public IntegralCal(MathFunction Function, double start, double end)
        {
            this.Function = Function;
            this.start    = start;
            this.end      = end;
        }

        public override void Start()
        {
            int m  = 1000;
            var dx = (end-start)/m;
            double x = start, y, sum = 0;
            for (int i = 1; i <= m; i++)
            {
                y    = Function(x);
                sum += y * dx;
                x   += dx;
            }
            results = sum;
        }
    }
}
