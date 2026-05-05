using System;

class Program
{
    static void Main()
    {
        string markdownText = @"
# Hello Quantum PhysicsLab
With Markdown supports
";
        string html = Pltxt2Htm.PlRichTextParser(
            markdownText,
            "project",
            "visitor",
            "author",
            "coauthors"
        );
        Console.WriteLine(html);
    }
}
