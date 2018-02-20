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
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;

namespace Quricol.Barcode
{
    /// <summary>
    /// QR Code encoder class
    /// </summary>
    public static class QRCode
    {
        /// <summary>
        /// Gets or sets the color of the background.
        /// </summary>
        /// <value>
        /// The color of the background.
        /// </value>
        public static Color BackgroundColor
        {
            get
            {
                return NativeMethods.ColorFromCOLORREF(NativeMethods.GetBackgroundColor());
            }
            set
            {
                NativeMethods.SetBackgroundColor(NativeMethods.ColorToCOLORREF(value));
            }
        }

        /// <summary>
        /// Gets or sets the color of the foreground.
        /// </summary>
        /// <value>
        /// The color of the foreground.
        /// </value>
        public static Color ForegroundColor
        {
            get
            {
                return NativeMethods.ColorFromCOLORREF(NativeMethods.GetForegroundColor());
            }
            set
            {
                NativeMethods.SetForegroundColor(NativeMethods.ColorToCOLORREF(value));
            }
        }

        /// <summary>
        /// Generates the bitmap file with encoded string
        /// </summary>
        /// <param name="fileName">Name of the file.</param>
        /// <param name="text">The text to encode.</param>
        /// <param name="margin">The margin.</param>
        /// <param name="pixelSize">Size of the pixel.</param>
        /// <param name="level">The error correction level.</param>
        public static void GenerateBitmap(string fileName, string text, int margin, int pixelSize, ErrorCorrectionLevel level)
        {
            if (string.IsNullOrEmpty(fileName) || string.IsNullOrEmpty(text))
                return;

            NativeMethods.GenerateBMP(fileName, text, margin, pixelSize, (int)level);
        }

        /// <summary>
        /// Generates the bitmap.
        /// </summary>
        /// <param name="fileName">Name of the file.</param>
        /// <param name="text">The text.</param>
        /// <param name="level">The error correction level.</param>
        public static void GenerateBitmap(string fileName, string text, ErrorCorrectionLevel level)
        {
            if (string.IsNullOrEmpty(fileName) || string.IsNullOrEmpty(text))
                return;

            NativeMethods.GenerateBMP(fileName, text, 4, 3, (int)level);
        }


        /// <summary>
        /// Generates the PNG.
        /// </summary>
        /// <param name="fileName">Name of the file.</param>
        /// <param name="text">The text.</param>
        /// <param name="margin">The margin.</param>
        /// <param name="pixelSize">Size of the pixel.</param>
        /// <param name="level">The error correction level.</param>
        public static void GeneratePng(string fileName, string text, int margin, int pixelSize, ErrorCorrectionLevel level)
        {
            if (string.IsNullOrEmpty(fileName) || string.IsNullOrEmpty(text))
                return;

            NativeMethods.GeneratePNG(fileName, text, margin, pixelSize, (int)level);
        }

        /// <summary>
        /// Generates the PNG.
        /// </summary>
        /// <param name="fileName">Name of the file.</param>
        /// <param name="text">The text.</param>
        /// <param name="level">The error correction level.</param>
        public static void GeneratePng(string fileName, string text, ErrorCorrectionLevel level)
        {
            if (string.IsNullOrEmpty(fileName) || string.IsNullOrEmpty(text))
                return;

            NativeMethods.GeneratePNG(fileName, text, 4, 3, (int)level);
        }

        /// <summary>
        /// Gets the bitmap.
        /// </summary>
        /// <param name="text">The text.</param>
        /// <param name="margin">The margin.</param>
        /// <param name="pixelSize">Size of the pixel.</param>
        /// <param name="level">The error correction level.</param>
        /// <returns>Returns the QR Code image</returns>
        public static Bitmap GetBitmap(string text, int margin, int pixelSize, ErrorCorrectionLevel level)
        {
            if (string.IsNullOrEmpty(text))
                return null;

            IntPtr handle = NativeMethods.GetHBitmap(text, margin, pixelSize, (int)level);

            if (handle == IntPtr.Zero)
                return null;

            Bitmap result = Bitmap.FromHbitmap(handle);

            NativeMethods.DeleteObject(handle);
            return result;
        }

        /// <summary>
        /// Gets the bitmap.
        /// </summary>
        /// <param name="text">The text.</param>
        /// <param name="level">The error correction level.</param>
        /// <returns>Returns QR Code image</returns>
        public static Bitmap GetBitmap(string text, ErrorCorrectionLevel level)
        {
            return GetBitmap(text, 4, 3, level);
        }

        /// <summary>
        /// Write the QR code encoded as PNG image to the stream
        /// </summary>
        /// <param name="stream">The stream.</param>
        /// <param name="text">The text.</param>
        /// <param name="margin">The margin.</param>
        /// <param name="pixelSize">Size of the pixel.</param>
        /// <param name="level">The error correction level.</param>
        public unsafe static void GetPng(Stream stream, string text, int margin, int pixelSize, ErrorCorrectionLevel level)
        {
            if ((stream == null) || string.IsNullOrEmpty(text))
                return;

            IntPtr buffer;
            int size = 0;

            NativeMethods.GetPNG(text, margin, pixelSize, (int)level, ref size, out buffer);

            if (size == 0)
                return;

            byte* ps = (byte*)buffer.ToPointer();
            for (int i = 0; i < size; i++)
            {
                stream.WriteByte(*ps);
                ps++;
            }

            NativeMethods.DestroyBuffer(buffer);
        }
    }
}
