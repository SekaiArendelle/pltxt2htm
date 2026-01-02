using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

public static class Pltxt2Htm
{
    private const string DllName = "pltxt2htm";

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr pltxt2htm_common_parser(string text);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr pltxt2htm_advanced_parser(string text);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr pltxt2htm_fixedadv_parser(string text, string host);

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

    public static string FixedAdvParser(string text, string host)
    {
        return GetStringFromIntPtr(pltxt2htm_fixedadv_parser(text, host));
    }
}
