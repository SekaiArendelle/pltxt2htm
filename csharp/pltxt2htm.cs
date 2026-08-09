/// <summary>
/// Source repository: https://github.com/SekaiArendelle/pltxt2htm
/// </summary>

using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

public static class Pltxt2Htm
{
#if UNITY_IOS
    private const string DllName = "__Internal";
#else
    private const string DllName = "pltxt2htm";
#endif

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr pltxt2htm_common_parser(string text);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr pltxt2htm_fixedadv_parser(string text, string host, string project, string visitor, string author, string coauthors);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr pltxt2htm_plrichtext_parser(string text, string project, string visitor, string author, string coauthors);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void pltxt2htm_free_u8string(IntPtr ptr);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern UIntPtr pltxt2htm_version_major();

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern UIntPtr pltxt2htm_version_minor();

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern UIntPtr pltxt2htm_version_patch();

    /// <summary>
    /// Decode a native UTF-8 buffer returned by the parser. The caller must still
    /// release the buffer with pltxt2htm_free_u8string afterwards.
    /// </summary>
    public static string GetStringFromIntPtr(IntPtr ptr) {
        Debug.Assert(ptr != IntPtr.Zero, "GetStringFromIntPtr received null pointer");
        return Marshal.PtrToStringUTF8(ptr);
    }

    public static string CommonParser(string text) {
        var ptr = pltxt2htm_common_parser(text);
        Debug.Assert(ptr != IntPtr.Zero, "pltxt2htm_common_parser returned null pointer");
        try {
            return GetStringFromIntPtr(ptr);
        }
        finally {
            pltxt2htm_free_u8string(ptr);
        }
    }

    public static string FixedAdvParser(string text, string host, string project, string visitor, string author, string coauthors) {
        var ptr = pltxt2htm_fixedadv_parser(text, host, project, visitor, author, coauthors);
        Debug.Assert(ptr != IntPtr.Zero, "pltxt2htm_fixedadv_parser returned null pointer");
        try {
            return GetStringFromIntPtr(ptr);
        }
        finally {
            pltxt2htm_free_u8string(ptr);
        }
    }

    public static string PlRichTextParser(string text, string project, string visitor, string author, string coauthors) {
        var ptr = pltxt2htm_plrichtext_parser(text ?? "", project ?? "", visitor ?? "", author ?? "", coauthors ?? "");
        Debug.Assert(ptr != IntPtr.Zero, "pltxt2htm_plrichtext_parser returned null pointer");
        try {
            return GetStringFromIntPtr(ptr);
        }
        finally {
            pltxt2htm_free_u8string(ptr);
        }
    }
}
