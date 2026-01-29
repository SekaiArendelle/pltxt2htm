using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

public static class Pltxt2Htm
{
#if DEBUG
    #if WINDOWS
    private const string DllName = "pltxt2htmd";
    #else
    private const string DllName = "pltxt2htm";
    #endif
#else
    private const string DllName = "pltxt2htm";
#endif

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr pltxt2htm_common_parser(string text);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr pltxt2htm_advanced_parser(string text);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr pltxt2htm_fixedadv_parser(string text, string host, string project, string visitor, string author, string coauthors);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern UIntPtr pltxt2htm_version_major();

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern UIntPtr pltxt2htm_version_minor();

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern UIntPtr pltxt2htm_version_patch();

    public static string GetStringFromIntPtr(IntPtr ptr)
    {
        var result = Marshal.PtrToStringAnsi(ptr);
        Debug.Assert(result != null);
        return result;
    }

    public static string CommonParser(string text)
    {
        return GetStringFromIntPtr(pltxt2htm_common_parser(text));
    }

    public static string AdvancedParser(string text)
    {
        return GetStringFromIntPtr(pltxt2htm_advanced_parser(text));
    }

    public static string FixedAdvParser(string text, string host, string project, string visitor, string author, string coauthors)
    {
        return GetStringFromIntPtr(pltxt2htm_fixedadv_parser(text, host, project, visitor, author, coauthors));
    }
}
