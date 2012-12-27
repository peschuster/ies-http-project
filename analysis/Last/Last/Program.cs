using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Last
{
    class Program
    {
        static void Main(string[] args)
        {
            while (true)
            {
                string input = Console.ReadLine();

                if (string.IsNullOrWhiteSpace(input))
                    return;

                string[] p = input.Split(':');
                int req = int.Parse(p[0]);
                if (req <= 0)
                    return;

                long count = 0;
                DateTime start = DateTime.Now;

                int parallel = int.Parse(p[1]);

                Parallel.For(0, req, new ParallelOptions { MaxDegreeOfParallelism = parallel }, i =>
                {
                    var http = HttpWebRequest.CreateHttp("http://192.168.2.125/10K.html?id=" + i);
                    
                    using (http.GetResponse())
                    {
                    }

                    double c = Interlocked.Increment(ref count);

                    if (i % 10 == 0)
                    {
                        Console.WriteLine(string.Format("{0, 10} {1:0.000}", c, c / (DateTime.Now - start).TotalSeconds));
                    }
                });
            }
        }
    }
}
