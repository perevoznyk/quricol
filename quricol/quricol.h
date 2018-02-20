//===============================================================================
//= Copyright (c) Serhiy Perevoznyk.  All rights reserved.
//= THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY
//= OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT
//= LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//= FITNESS FOR A PARTICULAR PURPOSE.
//===============================================================================

#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

void WINAPI GeneratePNGW(LPWSTR fileName, LPWSTR text, int margin, int size, int level);
void WINAPI GeneratePNGA(LPSTR fileName, LPSTR text, int margin, int size, int level);
#ifdef UNICODE
#define GeneratePNG  GeneratePNGW
#else
#define GeneratePNG  GeneratePNGA
#endif // !UNICODE

HBITMAP WINAPI GetHBitmapW(LPWSTR text, int margin, int size, int level);
HBITMAP WINAPI GetHBitmapA(LPSTR text, int margin, int size, int level);
#ifdef UNICODE
#define GetHBitmap  GetHBitmapW
#else
#define GetHBitmap  GetHBitmapA
#endif // !UNICODE

void WINAPI GenerateBMPW(LPWSTR fileName, LPWSTR text, int margin, int size, int level);
void WINAPI GenerateBMPA(LPSTR fileName, LPSTR text, int margin, int size, int level);
#ifdef UNICODE
#define GenerateBMP  GenerateBMPW
#else
#define GenerateBMP  GenerateBMPA
#endif // !UNICODE

void WINAPI GetPNGW(LPWSTR text, int margin, int size, int level, LPINT bufSize, __deref_opt_out void **ppvBits);
void WINAPI GetPNGA(LPSTR text, int margin, int size, int level, LPINT bufSize, __deref_opt_out void **ppvBits);
#ifdef UNICODE
#define GetPNG  GetPNGW
#else
#define GetPNG  GetPNGA
#endif // !UNICODE

void WINAPI DestroyBuffer(void* buffer);

COLORREF WINAPI GetBackgroundColor(void);
COLORREF WINAPI GetForegroundColor(void);
void WINAPI SetBackgroundColor(COLORREF value);
void WINAPI SetForegroundColor(COLORREF value);

#ifdef __cplusplus
}
#endif
