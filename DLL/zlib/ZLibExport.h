//////////////////////////////////////////////////////////////////////
// Jean-loup Gailly and Mark Adler's zlib library.
//////////////////////////////////////////////////////////////////////

#ifndef __ZLIB_H__
#define __ZLIB_H__

#include <windows.h>

DWORD ZCompressBound(DWORD dwSrcLen);
DWORD ZCompress(LPCVOID lpSrc, DWORD dwSrcLen, LPVOID lpBuffer, DWORD dwBufferLen);
DWORD ZExtract(LPCVOID lpSrc, DWORD dwSrcLen, LPVOID lpBuffer, DWORD dwBufferLen);

#endif // __ZLIB_H__