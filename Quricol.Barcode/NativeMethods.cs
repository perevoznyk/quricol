//===============================================================================
// Copyright (c) Serhiy Perevoznyk.  All rights reserved.
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE.
//===============================================================================
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;

namespace Quricol.Barcode
{
    internal static class NativeMethods
    {
        public static bool IsWOW64Process()
        {
            return IntPtr.Size == 8;
        }

        [DllImport("gdi32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool DeleteObject(IntPtr hObject);

        [DllImport("quricol32.dll", EntryPoint = "GeneratePNGW", CharSet = CharSet.Unicode)]
        private static extern void GeneratePNG32(string fileName, string text, int margin, int size, int level);

        [DllImport("quricol32.dll", EntryPoint = "GetHBitmapW", CharSet = CharSet.Unicode)]
        private static extern IntPtr GetHBitmap32(string text, int margin, int size, int level);

        [DllImport("quricol32.dll", EntryPoint = "GenerateBMPW", CharSet = CharSet.Unicode)]
        private static extern void GenerateBMP32(string fileName, string text, int margin, int size, int level);

        [DllImport("quricol32.dll", EntryPoint = "GetPNGW", CharSet = CharSet.Unicode)]
        private static extern void GetPNG32(string text, int margin, int size, int level, ref int bufSize, out IntPtr ppvBits);

        [DllImport("quricol32.dll", EntryPoint = "GetBackgroundColor", CharSet = CharSet.Unicode)]
        private static extern int GetBackgroundColor32();

        [DllImport("quricol32.dll", EntryPoint = "GetForegroundColor", CharSet = CharSet.Unicode)]
        private static extern int GetForegroundColor32();

        [DllImport("quricol32.dll", EntryPoint = "SetBackgroundColor", CharSet = CharSet.Unicode)]
        private static extern void SetBackgroundColor32(int value);

        [DllImport("quricol32.dll", EntryPoint = "SetForegroundColor", CharSet = CharSet.Unicode)]
        private static extern void SetForegroundColor32(int value);

        [DllImport("quricol64.dll", EntryPoint = "GetBackgroundColor", CharSet = CharSet.Unicode)]
        private static extern int GetBackgroundColor64();

        [DllImport("quricol64.dll", EntryPoint = "GetForegroundColor", CharSet = CharSet.Unicode)]
        private static extern int GetForegroundColor64();

        [DllImport("quricol64.dll", EntryPoint = "SetBackgroundColor", CharSet = CharSet.Unicode)]
        private static extern void SetBackgroundColor64(int value);

        [DllImport("quricol64.dll", EntryPoint = "SetForegroundColor", CharSet = CharSet.Unicode)]
        private static extern void SetForegroundColor64(int value);

        [DllImport("quricol64.dll", EntryPoint = "GeneratePNGW", CharSet = CharSet.Unicode)]
        private static extern void GeneratePNG64(string fileName, string text, int margin, int size, int level);

        [DllImport("quricol64.dll", EntryPoint = "GetHBitmapW", CharSet = CharSet.Unicode)]
        private static extern IntPtr GetHBitmap64(string text, int margin, int size, int level);

        [DllImport("quricol64.dll", EntryPoint = "GenerateBMPW", CharSet = CharSet.Unicode)]
        private static extern void GenerateBMP64(string fileName, string text, int margin, int size, int level);

        [DllImport("quricol64.dll", EntryPoint = "GetPNGW", CharSet = CharSet.Unicode)]
        private static extern void GetPNG64(string text, int margin, int size, int level, ref int bufSize, out IntPtr ppvBits);

        [DllImport("quricol32.dll", EntryPoint = "DestroyBuffer", CharSet = CharSet.Unicode)]
        private static extern void DestroyBuffer32(IntPtr buffer);

        [DllImport("quricol64.dll", EntryPoint = "DestroyBuffer", CharSet = CharSet.Unicode)]
        private static extern void DestroyBuffer64(IntPtr buffer);

        public static int GetBackgroundColor()
        {
            if (IsWOW64Process())
                return GetBackgroundColor64();
            else
                return GetBackgroundColor32();
        }

        public static int GetForegroundColor()
        {
            if (IsWOW64Process())
                return GetForegroundColor64();
            else
                return GetForegroundColor32();
        }

        public static void SetBackgroundColor(int value)
        {
            if (IsWOW64Process())
                SetBackgroundColor64(value);
            else
                SetBackgroundColor32(value);
        }

        public static void SetForegroundColor(int value)
        {
            if (IsWOW64Process())
                SetForegroundColor64(value);
            else
                SetForegroundColor32(value);
        }

        public static void DestroyBuffer(IntPtr buffer)
        {
            if (IsWOW64Process())
                DestroyBuffer64(buffer);
            else
                DestroyBuffer32(buffer);
        }

        public static void GeneratePNG(string fileName, string text, int margin, int size, int level)
        {
            if (IsWOW64Process())
                GeneratePNG64(fileName, text, margin, size, level);
            else
                GeneratePNG32(fileName, text, margin, size, level);
        }

        public static IntPtr GetHBitmap(string text, int margin, int size, int level)
        {
            if (IsWOW64Process())
                return GetHBitmap64(text, margin, size, level);
            else
                return GetHBitmap32(text, margin, size, level);

        }

        public static void GenerateBMP(string fileName, string text, int margin, int size, int level)
        {
            if (IsWOW64Process())
                GenerateBMP64(fileName, text, margin, size, level);
            else
                GenerateBMP32(fileName, text, margin, size, level);
        }

        public static void GetPNG(string text, int margin, int size, int level, ref int bufSize, out IntPtr ppvBits)
        {
            if (IsWOW64Process())
                GetPNG64(text, margin, size, level, ref bufSize, out ppvBits);
            else
                GetPNG32(text, margin, size, level, ref bufSize, out ppvBits);
        }

        public static Color ColorFromCOLORREF(int colorref)
        {
            int red = colorref & 0xff;
            int green = (colorref >> 8) & 0xff;
            int blue = (colorref >> 0x10) & 0xff;
            return Color.FromArgb(red, green, blue);
        }

        public static int ColorToCOLORREF(Color color)
        {
            return ((color.R | (color.G << 8)) | (color.B << 0x10));
        }

    }
}
