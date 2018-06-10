#include "ResPackInfo.h"

HANDLE __declspec(dllexport) ResPackOpen(LPCSTR lpszFileName, DWORD dwOpenFlags)
{
	CResPackInfo* pInfo = new CResPackInfo;
	assert(pInfo);
	if (!pInfo->OpenPackageFileA(lpszFileName, dwOpenFlags))
	{
		delete pInfo;
		pInfo = NULL;
	}
	return (HANDLE)pInfo;
}

HANDLE __declspec(dllexport) ResPackOpen(LPCWSTR lpszFileName, DWORD dwOpenFlags)
{
	CResPackInfo* pInfo = new CResPackInfo;
	assert(pInfo);
	if (!pInfo->OpenPackageFileW(lpszFileName, dwOpenFlags))
	{
		delete pInfo;
		pInfo = NULL;
	}
	return (HANDLE)pInfo;
}

BOOL __declspec(dllexport) ResPackCloseHandle(HANDLE hResPack)
{
	CResPackInfo* pInfo = CResPackInfo::VerifyHandle(hResPack);
	if (pInfo == NULL)
		return FALSE;	
	
	// close the file
	pInfo->ClosePackageFile();
	delete pInfo;
	return TRUE;
}

DWORD  __declspec(dllexport) ResPackLookup(HANDLE hResPack, DWORD dwResID)
{
	CResPackInfo* pInfo = CResPackInfo::VerifyHandle(hResPack);
	if (pInfo == NULL)
		return 0;

	LPCRESPACKINDEX pIdx = pInfo->Lookup(dwResID);
	return pIdx ? pIdx->dwOldLength : 0;
}

DWORD __declspec(dllexport) ResPackGetCount(HANDLE hResPack)
{
	CResPackInfo* pInfo = CResPackInfo::VerifyHandle(hResPack);
	if (pInfo == NULL)
		return 0;

	return pInfo->GetCount();
}

BOOL __declspec(dllexport) ResPackEnumRes(HANDLE hResPack, RESPACK_ENUM_PROC lpfnCallback, LPARAM lParam)
{
	CResPackInfo* pInfo = CResPackInfo::VerifyHandle(hResPack);
	if (pInfo == NULL)
		return FALSE;	
	
	return pInfo->EnumResources(lpfnCallback, lParam);	
}

BOOL __declspec(dllexport) ResPackAdd(HANDLE hResPack, DWORD dwResID, LPCVOID pData, DWORD dwDataSize, BOOL bCompress)
{
	CResPackInfo* pInfo = CResPackInfo::VerifyHandle(hResPack);
	if (pInfo == NULL)
		return FALSE;	
	
	return pInfo->AddRes(dwResID, pData, dwDataSize, bCompress);
}

BOOL __declspec(dllexport) ResPackAdd(HANDLE hResPack, DWORD dwResID, LPCSTR lpszFileName, BOOL bCompress)
{
	CResPackInfo* pInfo = CResPackInfo::VerifyHandle(hResPack);
	if (pInfo == NULL)
		return FALSE;

	if (pInfo->IsReadOnly())
		return FALSE;
	
	HANDLE hFile = ::CreateFileA(lpszFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dwFileLen = ::GetFileSize(hFile, NULL);
	if (dwFileLen == 0)
	{
		::CloseHandle(hFile);
		::SetLastError(ERROR_EMPTY);
		return FALSE;
	}
	
	DWORD dwRead = 0;
	LPBYTE pBlock = new BYTE[dwFileLen];
	assert(pBlock);
	BOOL bOK = ::ReadFile(hFile, pBlock, dwFileLen, &dwRead, NULL);	
	::CloseHandle(hFile);
	if (bOK && dwRead)
		bOK = pInfo->AddRes(dwResID, pBlock, dwRead, bCompress);
	delete [] pBlock;	
	return bOK;
}

BOOL __declspec(dllexport) ResPackAdd(HANDLE hResPack, DWORD dwResID, LPCWSTR lpszFileName, BOOL bCompress)
{
	CResPackInfo* pInfo = CResPackInfo::VerifyHandle(hResPack);
	if (pInfo == NULL)
		return FALSE;

	if (pInfo->IsReadOnly())
		return FALSE;
	
	HANDLE hFile = ::CreateFileW(lpszFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dwFileLen = ::GetFileSize(hFile, NULL);
	if (dwFileLen == 0)
	{
		::CloseHandle(hFile);
		::SetLastError(ERROR_EMPTY);
		return FALSE;
	}
	
	DWORD dwRead = 0;
	LPBYTE pBlock = new BYTE[dwFileLen];
	assert(pBlock);
	BOOL bOK = ::ReadFile(hFile, pBlock, dwFileLen, &dwRead, NULL);	
	::CloseHandle(hFile);
	if (bOK && dwRead)
		bOK = pInfo->AddRes(dwResID, pBlock, dwRead, bCompress);
	delete [] pBlock;
	return bOK;
}

DWORD __declspec(dllexport) ResPackExtract(HANDLE hResPack, DWORD dwResID, LPVOID pBuffer, DWORD dwBufferSize)
{
	CResPackInfo* pInfo = CResPackInfo::VerifyHandle(hResPack);
	if (pInfo == NULL)
		return 0;

	if (pBuffer == NULL || dwBufferSize == 0)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}

	if (pInfo->Lookup(dwResID) == NULL)
	{
		::SetLastError(ERROR_NOT_FOUND);
		return 0;
	}

	return pInfo->ExtractRes(dwResID, pBuffer, dwBufferSize);
}

DWORD __declspec(dllexport) ResPackExtract(HANDLE hResPack, DWORD dwResID, LPCSTR lpszFileName)
{
	CResPackInfo* pInfo = CResPackInfo::VerifyHandle(hResPack);
	if (pInfo == NULL)
		return 0;

	if (lpszFileName == NULL || *lpszFileName == 0)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}

	LPCRESPACKINDEX pIdx = pInfo->Lookup(dwResID);
	if (pIdx == NULL)
	{
		::SetLastError(ERROR_NOT_FOUND);
		return 0;
	}

	HANDLE hFile = ::CreateFileA(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;

	DWORD dwWritten = 0;
	LPBYTE p = new BYTE[pIdx->dwOldLength];
	DWORD dwRead = pInfo->ExtractRes(dwResID, p, pIdx->dwOldLength);
	::WriteFile(hFile, p, dwRead, &dwWritten, NULL);
	delete [] p;
	::CloseHandle(hFile);
	return dwWritten;
}

DWORD __declspec(dllexport) ResPackExtract(HANDLE hResPack, DWORD dwResID, LPCWSTR lpszFileName)
{
	CResPackInfo* pInfo = CResPackInfo::VerifyHandle(hResPack);
	if (pInfo == NULL)
		return 0;

	if (lpszFileName == NULL || *lpszFileName == 0)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}

	LPCRESPACKINDEX pIdx = pInfo->Lookup(dwResID);
	if (pIdx == NULL)
	{
		::SetLastError(ERROR_NOT_FOUND);
		return 0;
	}

	HANDLE hFile = ::CreateFileW(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;

	DWORD dwWritten = 0;
	LPBYTE p = new BYTE[pIdx->dwOldLength];
	DWORD dwRead = pInfo->ExtractRes(dwResID, p, pIdx->dwOldLength);
	::WriteFile(hFile, p, dwRead, &dwWritten, NULL);
	delete [] p;
	::CloseHandle(hFile);
	return dwWritten;
}

BOOL __declspec(dllexport) ResPackRemove(HANDLE hResPack, DWORD dwResID)
{
	CResPackInfo* pInfo = CResPackInfo::VerifyHandle(hResPack);
	if (pInfo == NULL)
		return FALSE;
	
	return pInfo->RemoveRes(dwResID);
}

BOOL __declspec(dllexport) ResPackModify(HANDLE hResPack, DWORD dwResID, DWORD dwNewID)
{
	CResPackInfo* pInfo = CResPackInfo::VerifyHandle(hResPack);
	if (pInfo == NULL)
		return FALSE;

	return pInfo->ModifyResID(dwResID, dwNewID);
}

BOOL __declspec(dllexport) ResPackModify(HANDLE hResPack, DWORD dwResID, LPCVOID pData, DWORD dwDataSize, BOOL bCompress)
{
	return ResPackRemove(hResPack, dwResID) && ResPackAdd(hResPack, dwResID, pData, dwDataSize, bCompress);
}

BOOL __declspec(dllexport) ResPackModify(HANDLE hResPack, DWORD dwResID, LPCSTR lpszFileName, BOOL bCompress)
{
	return ResPackRemove(hResPack, dwResID) && ResPackAdd(hResPack, dwResID, lpszFileName, bCompress);
}

BOOL __declspec(dllexport) ResPackModify(HANDLE hResPack, DWORD dwResID, LPCWSTR lpszFileName, BOOL bCompress)
{
	return ResPackRemove(hResPack, dwResID) && ResPackAdd(hResPack, dwResID, lpszFileName, bCompress);
}

