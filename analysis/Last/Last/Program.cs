using System;
using System.Net;
using System.Threading;
using System.Threading.Tasks;

class Program
{
    static void Main(string[] args)
    {
        while (true)
        {
            Console.WriteLine("Specify params. Form: [number]:[threads]");
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
            string baseUrl = "http://192.168.2.125/10K.html?id=";
            var parellelOptions = new ParallelOptions { MaxDegreeOfParallelism = parallel };

            Parallel.For(0, req, parellelOptions, i =>
            {
                var http = HttpWebRequest.CreateHttp(baseUrl + i);
                    
                using (http.GetResponse())
                {
                }

                double c = Interlocked.Increment(ref count);

                if (i % 10 == 0)
                {
                    Console.WriteLine(
                        "{0, 8} {1:0.000}", 
                        c, 
                        c / (DateTime.Now - start).TotalSeconds);
                }
            });
        }
    }
}