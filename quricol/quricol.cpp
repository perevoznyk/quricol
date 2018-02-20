//===============================================================================
//= Copyright (c) Serhiy Perevoznyk.  All rights reserved.
//= THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY
//= OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT
//= LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//= FITNESS FOR A PARTICULAR PURPOSE.
//===============================================================================

// quricol.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "quricol.h"
#include "quricol_internal.h"
#include "qrencode.h"
#include "bitmaps.h"
#include "zlib.h"
#include "png.h"
#include "pnginfo.h"
#include "pngstruct.h"
#include <windows.h>

// Shared Data
#pragma data_seg(".shared")	// Make a new section that we'll make shared
COLORREF BackgroundColor = 0xFFFFFFFF;
COLORREF ForegroundColor = 0xFF000000;
#pragma data_seg()			// Back to regular, nonshared data
#pragma comment(linker, "/section:.shared,rws")

// Shift count and bit mask for A, R, G, B components


COLORREF WINAPI GetBackgroundColor(void)
{
	return BackgroundColor;
}

COLORREF WINAPI GetForegroundColor(void)
{
	return ForegroundColor;
}

void WINAPI SetBackgroundColor(COLORREF value)
{
	BackgroundColor = value;
}

void WINAPI SetForegroundColor(COLORREF value)
{
	ForegroundColor = value;
}

void WINAPI DestroyBuffer(void* buffer)
{
	free(buffer);
}

void WINAPI GeneratePNGW(LPWSTR fileName, LPWSTR text, int margin, int size, int level)
{
	if (text == NULL)
		return;

	QRcode* qrcode = MakeCode(text, (QRecLevel)level);
	if (qrcode == NULL)
		return;

	writePNG(qrcode, fileName, margin, size);

	QRcode_free(qrcode);
}

void WINAPI GetPNGW(LPWSTR text, int margin, int size, int level, LPINT bufSize, __deref_opt_out void **ppvBits)
{
	if (text == NULL)
		return;

	QRcode* qrcode = MakeCode(text, (QRecLevel)level);
	if (qrcode == NULL)
		return;

	/* static */
	struct mem_encode state;

	/* initialise - put this before png_write_png() call */
	state.buffer = NULL;
	state.size = 0;

	generatePNG(qrcode, &state, margin, size);
	* bufSize = (int)state.size;
	*ppvBits = (VOID *)state.buffer;

	QRcode_free(qrcode);
}

void WINAPI GetPNGA(LPSTR text, int margin, int size, int level, LPINT bufSize, __deref_opt_out void **ppvBits)
{
	if (text == NULL)
		return;

	QRcode* qrcode = MakeCodeA(text, (QRecLevel)level);
	if (qrcode == NULL)
		return;

	/* static */
	struct mem_encode state;

	/* initialise - put this before png_write_png() call */
	state.buffer = NULL;
	state.size = 0;

	generatePNG(qrcode, &state, margin, size);
	* bufSize = (int)state.size;
	*ppvBits = (VOID *)state.buffer;

	QRcode_free(qrcode);
}

void WINAPI GeneratePNGA(LPSTR fileName, LPSTR text, int margin, int size, int level)
{
	if (fileName == NULL)
		return;
	if (text == NULL)
		return;
	LPWSTR fileNameW = AllocateUnicodeString(fileName);
	LPWSTR textW = AllocateUnicodeString(text);
	GeneratePNGW(fileNameW, textW, margin, size, level);
	FreeUnicodeString(fileNameW);
	FreeUnicodeString(textW);

}

void WINAPI GenerateBMPA(LPSTR fileName, LPSTR text, int margin, int size, int level)
{
	if (fileName == NULL)
		return;
	if (text == NULL)
		return;
	LPWSTR fileNameW = AllocateUnicodeString(fileName);
	LPWSTR textW = AllocateUnicodeString(text);
	GenerateBMPW(fileNameW, textW, margin, size, level);
	FreeUnicodeString(fileNameW);
	FreeUnicodeString(textW);

}

void WINAPI GenerateBMPW(LPWSTR fileName, LPWSTR text, int margin, int size, int level)
{
	if (fileName == NULL)
		return;

	HBITMAP bmp = GetHBitmap(text, margin, size, level);
	if (bmp == NULL)
		return;

	PBITMAPINFO info = CreateBitmapInfoStruct(bmp);
	if (info != NULL)
	{
		HDC hdc = GetDC(0);
		CreateBMPFile(fileName, info, bmp, hdc);
		ReleaseDC(0, hdc);
		DeleteObject(bmp);
		LocalFree(info);
	}

}

HBITMAP WINAPI GetHBitmapA(LPSTR text, int margin, int size, int level)
{
	if (text == NULL)
		return NULL;
	LPWSTR textW = AllocateUnicodeString(text);
	HBITMAP result = GetHBitmapW(textW, margin, size, level);
	FreeUnicodeString(textW);
	return result;
}

HBITMAP WINAPI GetHBitmapW(LPWSTR text, int margin, int size, int level)
{
	HBITMAP hbm = NULL;
	BITMAPINFO      bi;
	BYTE * pixelData;
	BYTE * p;
	BYTE * data;
	BYTE * row;
	BYTE * current;
	int width;
	int x, y, k;
	BOOL black;

	if (text == NULL)
		return NULL;

	QRcode* qrcode = MakeCode(text, (QRecLevel)level);
	if (qrcode == NULL)
		return NULL;

	unsigned int fg_color[4] = {0, 0, 0, 255};
	unsigned int bg_color[4] = {255, 255, 255, 255};

	fg_color[0] = GetBValue (ForegroundColor);
	fg_color[1] = GetGValue(ForegroundColor);
	fg_color[2] = GetRValue(ForegroundColor);

	bg_color[0] = GetBValue(BackgroundColor);
	bg_color[1] = GetGValue(BackgroundColor);
	bg_color[2] = GetRValue(BackgroundColor);

	width = (qrcode->width + margin * 2) * size;
	ZeroMemory(&bi.bmiHeader, sizeof(BITMAPINFOHEADER));
	bi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth      = width;
	bi.bmiHeader.biHeight      = -width;

	bi.bmiHeader.biPlanes      = 1;
	bi.bmiHeader.biBitCount    = 32;
	bi.bmiHeader.biCompression = BI_RGB;

	int memWidth = WIDTHBYTES(width * bi.bmiHeader.biBitCount);
	row = (BYTE*)malloc(memWidth);
	data = qrcode->data;
	hbm = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, (VOID **)&pixelData,  NULL, 0);
	if (hbm && pixelData)
	{
		memset(pixelData, 0xff, memWidth * width);

		p = pixelData;
		for (k = 0; k < margin * size * width; k++)
		{
			for (int i = 0; i < 3; i++)
			{
				*p = bg_color[i];
				p++;
			}
			p++;
		}


		p = pixelData + memWidth * margin * size;
		for (y = 0; y < qrcode->width; y++) 
		{
			memset(row, 0xff, memWidth);

			current = row;
			for (k = 0; k < margin * size; k++)
			{
				for (int i = 0; i < 3; i++)
				{
					*current = bg_color[i];
					current++;
				}
				current++;
			}

			current = row + margin * size * 4;
			for(x=0; x < qrcode->width; x++) 
			{
				black = ( (*data & 1) == 1);
				for (int y = 0; y < size; y++)
				{
					if (black)
					{
						for (int i = 0; i < 3; i++)
						{
							*current = fg_color[i];
							current++;
						}
					}
					else
						for (int i = 0; i < 3; i++)
						{
							*current = bg_color[i];
							current++;
						}
						current++; 
				}
				data++;
			}
			
			for (k = 0; k < margin * size; k++)
			{
				for (int i = 0; i < 3; i++)
				{
					*current = bg_color[i];
					current++;
				}
				current++;
			}
			
			for (k = 0; k < size; k++)
			{
				memcpy(p, row, memWidth);
				p += memWidth;
			}

		}

	}

	for (k = 0; k < margin * size * width; k++)
	{
		for (int i = 0; i < 3; i++)
		{
			*p = bg_color[i];
			p++;
		}
		p++;
	}

	free(row);

	QRcode_free(qrcode);

	return hbm;
}

QRcode* MakeCode(LPWSTR text, int level)
{
	if (text == NULL)
		return NULL;

	int ulen = WideCharToMultiByte(CP_UTF8, 0, text, (int)wcslen(text), NULL, 0, NULL, NULL);
	LPSTR utfText = (LPSTR)malloc(ulen+1);
	memset(utfText, 0, ulen+1);
	WideCharToMultiByte(CP_UTF8, 0, text, (int)wcslen(text), utfText, ulen, NULL, NULL);

	QRcode* result = QRcode_encodeString(utfText, 0, (QRecLevel)level, QR_MODE_8, 1);

	free(utfText);

	return result;
}


QRcode* MakeCodeA(LPSTR text, int level)
{
	if (text == NULL)
		return NULL;

	QRcode* result = QRcode_encodeString(text, 0, (QRecLevel)level, QR_MODE_8, 1);

	return result;
}

int writePNG(QRcode *qrcode, LPWSTR outfile, int margin, int size)
{
	unsigned int fg_color[4] = {0, 0, 0, 255};
	unsigned int bg_color[4] = {255, 255, 255, 255};

	fg_color[0] = GetRValue (ForegroundColor);
	fg_color[1] = GetGValue(ForegroundColor);
	fg_color[2] = GetBValue(ForegroundColor);

	bg_color[0] = GetRValue(BackgroundColor);
	bg_color[1] = GetGValue(BackgroundColor);
	bg_color[2] = GetBValue(BackgroundColor);

	static FILE *fp; // avoid clobbering by setjmp.
	png_structp png_ptr;
	png_infop info_ptr;
	png_colorp palette;
	png_byte alpha_values[2];
	unsigned char *row, *p, *q;
	int x, y, xx, yy, bit;
	int realwidth;
	errno_t err;

	realwidth = (qrcode->width + margin * 2) * size;
	row = (unsigned char *)malloc((realwidth + 7) / 8);
	if(row == NULL) {
		exit(EXIT_FAILURE);
	}

	err = _wfopen_s(&fp, outfile, L"wb");
	if (err != 0)
		exit(EXIT_FAILURE);

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL) {
		exit(EXIT_FAILURE);
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL) {
		exit(EXIT_FAILURE);
	}

	if(setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		exit(EXIT_FAILURE);
	}

	palette = (png_colorp) malloc(sizeof(png_color) * 2);
	palette[0].red   = fg_color[0];
	palette[0].green = fg_color[1];
	palette[0].blue  = fg_color[2];
	palette[1].red   = bg_color[0];
	palette[1].green = bg_color[1];
	palette[1].blue  = bg_color[2];
	alpha_values[0] = fg_color[3];
	alpha_values[1] = bg_color[3];
	png_set_PLTE(png_ptr, info_ptr, palette, 2);
	png_set_tRNS(png_ptr, info_ptr, alpha_values, 2, NULL);

	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr,
		realwidth, realwidth,
		1,
		PNG_COLOR_TYPE_PALETTE,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);

	/* top margin */
	memset(row, 0xff, (realwidth + 7) / 8);
	for(y=0; y<margin * size; y++) {
		png_write_row(png_ptr, row);
	}

	/* data */
	p = qrcode->data;
	for(y=0; y<qrcode->width; y++) {
		bit = 7;
		memset(row, 0xff, (realwidth + 7) / 8);
		q = row;
		q += margin * size / 8;
		bit = 7 - (margin * size % 8);
		for(x=0; x<qrcode->width; x++) {
			for(xx=0; xx<size; xx++) {
				*q ^= (*p & 1) << bit;
				bit--;
				if(bit < 0) {
					q++;
					bit = 7;
				}
			}
			p++;
		}
		for(yy=0; yy<size; yy++) {
			png_write_row(png_ptr, row);
		}
	}
	/* bottom margin */
	memset(row, 0xff, (realwidth + 7) / 8);
	for(y=0; y<margin * size; y++) {
		png_write_row(png_ptr, row);
	}

	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(fp);
	free(row);

	return 0;
}


void
my_png_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	struct mem_encode* p=(struct mem_encode*)png_ptr->io_ptr;
	size_t nsize = p->size + length;

	/* allocate or grow buffer */
	if(p->buffer)
		p->buffer = (char*)realloc(p->buffer, nsize);
	else
		p->buffer = (char*)malloc(nsize);

	if(!p->buffer)
		png_error(png_ptr, "Write Error");

	/* copy new bytes to end of buffer */
	memcpy(p->buffer + p->size, data, length);
	p->size += length;
}

/* This is optional but included to show how png_set_write_fn() is called */
void
my_png_flush(png_structp png_ptr)
{
}


int generatePNG(QRcode *qrcode, mem_encode* memory, int margin, int size)
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned char *row, *p, *q;
	int x, y, xx, yy, bit;
	int realwidth;
	png_colorp palette;
	png_byte alpha_values[2];

	unsigned int fg_color[4] = {0, 0, 0, 255};
	unsigned int bg_color[4] = {255, 255, 255, 255};

	fg_color[0] = GetRValue (ForegroundColor);
	fg_color[1] = GetGValue(ForegroundColor);
	fg_color[2] = GetBValue(ForegroundColor);

	bg_color[0] = GetRValue(BackgroundColor);
	bg_color[1] = GetGValue(BackgroundColor);
	bg_color[2] = GetBValue(BackgroundColor);

	realwidth = (qrcode->width + margin * 2) * size;
	row = (unsigned char *)malloc((realwidth + 7) / 8);
	if(row == NULL) {
		exit(EXIT_FAILURE);
	}


	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL) {
		exit(EXIT_FAILURE);
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL) {
		exit(EXIT_FAILURE);
	}

	if(setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		exit(EXIT_FAILURE);
	}

	png_set_write_fn(png_ptr, memory, my_png_write_data, my_png_flush);

	palette = (png_colorp) malloc(sizeof(png_color) * 2);
	palette[0].red   = fg_color[0];
	palette[0].green = fg_color[1];
	palette[0].blue  = fg_color[2];
	palette[1].red   = bg_color[0];
	palette[1].green = bg_color[1];
	palette[1].blue  = bg_color[2];
	alpha_values[0] = fg_color[3];
	alpha_values[1] = bg_color[3];
	png_set_PLTE(png_ptr, info_ptr, palette, 2);
	png_set_tRNS(png_ptr, info_ptr, alpha_values, 2, NULL);


	png_set_IHDR(png_ptr, info_ptr,
		realwidth, realwidth,
		1,
		PNG_COLOR_TYPE_PALETTE,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	png_write_info(png_ptr, info_ptr);

	/* top margin */
	memset(row, 0xff, (realwidth + 7) / 8);
	for(y=0; y<margin * size; y++) {
		png_write_row(png_ptr, row);
	}

	/* data */
	p = qrcode->data;
	for(y=0; y<qrcode->width; y++) {
		bit = 7;
		memset(row, 0xff, (realwidth + 7) / 8);
		q = row;
		q += margin * size / 8;
		bit = 7 - (margin * size % 8);
		for(x=0; x<qrcode->width; x++) {
			for(xx=0; xx<size; xx++) {
				*q ^= (*p & 1) << bit;
				bit--;
				if(bit < 0) {
					q++;
					bit = 7;
				}
			}
			p++;
		}
		for(yy=0; yy<size; yy++) {
			png_write_row(png_ptr, row);
		}
	}
	/* bottom margin */
	memset(row, 0xff, (realwidth + 7) / 8);
	for(y=0; y<margin * size; y++) {
		png_write_row(png_ptr, row);
	}

	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	free(row);

	return 0;
}
