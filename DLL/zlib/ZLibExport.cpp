#include "Zlib.h"
#include "ZlibExport.h"

DWORD ZCompressBound(DWORD dwSrcLen)
{
	return compressBound(dwSrcLen);
}

DWORD ZCompress(LPCVOID lpSrc, DWORD dwSrcLen, LPVOID lpBuffer, DWORD dwBufferLen)
{
	if (lpSrc == NULL || dwSrcLen == 0 || lpBuffer == NULL || dwBufferLen == 0)
		return 0;

	DWORD dwResult = dwBufferLen;
	if (Z_OK != compress((Bytef*)lpBuffer, (uLong*)&dwResult, (const Bytef*)lpSrc, dwSrcLen))
		return 0;
	return dwResult;
}

DWORD ZExtract(LPCVOID lpSrc, DWORD dwSrcLen, LPVOID lpBuffer, DWORD dwBufferLen)
{
	if (lpSrc == NULL || dwSrcLen == 0 || lpBuffer == NULL || dwBufferLen == 0)
		return 0;

	DWORD dwResult = dwBufferLen;
	if (Z_OK != uncompress((Bytef*)lpBuffer, (uLongf*)&dwResult, (const Bytef*)lpSrc, dwSrcLen))
		return 0;

	return dwResult;
}
