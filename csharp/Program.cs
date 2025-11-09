using System;

class Program
{
    static void Main()
    {
        string markdownText = @"
# Hello Quantum PhysicsLab
With Markdown supports
";
        string host = "localhost:5173";

        string html = Pltxt2Htm.AdvancedParser(markdownText, host);
        Console.WriteLine(html);
    }
}
