//===============================================================================
//= Copyright (c) Serhiy Perevoznyk.  All rights reserved.
//= THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY
//= OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT
//= LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//= FITNESS FOR A PARTICULAR PURPOSE.
//===============================================================================

#pragma once

#include "qrencode.h"
#include "qrinput.h"
#include "qrspec.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WIDTHBYTES(i) (((i) + 31) / 32 * 4)

/* structure to store PNG image bytes */
struct mem_encode
{
  char *buffer;
  size_t size;
};

QRcode* MakeCode(LPWSTR text, int level);
QRcode* MakeCodeA(LPSTR text, int level);

int writePNG(QRcode *qrcode, LPWSTR outfile, int margin, int size);
int generatePNG(QRcode *qrcode, mem_encode* memory, int margin, int size);

int
AnsiToUnicodeString(
 LPSTR pAnsi,
 LPWSTR pUnicode,
 DWORD StringLength
 );

LPWSTR
AllocateUnicodeString(
 LPSTR  pAnsiString
 );

void
FreeUnicodeString(
 LPWSTR  pUnicodeString
 );

#ifdef __cplusplus
}
#endif
