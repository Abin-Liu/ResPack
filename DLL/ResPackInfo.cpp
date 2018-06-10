#include "ResPackInfo.h"
#include "zlib\\ZLibExport.h"
#include "ResPack.h"

#define RESPACK_CONTENTS_START_POS	(sizeof(GUID) + sizeof(RESPACKHEADER))
#define RESPACK_SHIFT_BLOCK		32768

// An GUID that is written to the beginning of a ResPack package file, since GUID's are globally
// unique, we are able to tell if a file is a ResPack package at the very beginning.
static const GUID RESPACKGUID = { /* 2D79483F-D8FF-4002-A983-8FE609521C92 */
	0x2D79483F,
	0xD8FF,
	0x4002,
	{0xA9, 0x83, 0x8F, 0xE6, 0x09, 0x52, 0x1C, 0x92}
};

CResPackInfo::CResPackInfo(): m_hFile(INVALID_HANDLE_VALUE), m_bReadOnly(TRUE)
{
	::memcpy(&m_guid, &RESPACKGUID, sizeof(GUID));
	_Initialize();
}

CResPackInfo::~CResPackInfo()
{
	ClosePackageFile();
}

void CResPackInfo::_Initialize()
{
	m_hdr.dwCount = 0;
	m_hdr.dwIdxPos = sizeof(GUID) + sizeof(RESPACKHEADER);
	m_hdr.dwMajorVer = RESPACK_MAJOR_VER;
	m_hdr.dwMinorVer = RESPACK_MINOR_VER;
	m_table.clear();
}

CResPackInfo* CResPackInfo::VerifyHandle(HANDLE hResPack)
{
	CResPackInfo* p = (CResPackInfo*)hResPack;
	if (p == NULL || ::IsBadReadPtr(p, sizeof(CResPackInfo)) || ::memcmp(&p->m_guid, &RESPACKGUID, sizeof(GUID)))
	{
		::SetLastError(ERROR_INVALID_HANDLE);
		return NULL;
	}
	return p;
}

void CResPackInfo::ClosePackageFile()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	_Initialize();
}

BOOL CResPackInfo::OpenPackageFileA(LPCSTR lpszFileName, DWORD dwOpenFlags)
{
	ClosePackageFile();
	DWORD dwDesiredAccess = GENERIC_READ;
	DWORD dwCreationDisposition = OPEN_EXISTING;
	if (dwOpenFlags & RESPACK_OPEN_WRITE)
	{
		dwDesiredAccess |= GENERIC_WRITE;
		if (dwOpenFlags & RESPACK_OPEN_CREATE)
			dwCreationDisposition = CREATE_ALWAYS;
	}	

	m_hFile = ::CreateFileA(lpszFileName, dwDesiredAccess, 0, NULL, dwCreationDisposition, 0, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;	
	
	m_bReadOnly = ((dwOpenFlags & RESPACK_OPEN_WRITE) == 0);
	_ReadHeaderAndTable();
	return TRUE;
}

BOOL CResPackInfo::OpenPackageFileW(LPCWSTR lpszFileName, DWORD dwOpenFlags)
{
	ClosePackageFile();
	DWORD dwDesiredAccess = GENERIC_READ;
	DWORD dwCreationDisposition = OPEN_EXISTING;
	if (dwOpenFlags & RESPACK_OPEN_WRITE)
	{
		dwDesiredAccess |= GENERIC_WRITE;
		if (dwOpenFlags & RESPACK_OPEN_CREATE)
			dwCreationDisposition = CREATE_ALWAYS;
	}	

	m_hFile = ::CreateFileW(lpszFileName, dwDesiredAccess, 0, NULL, dwCreationDisposition, 0, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;	
	
	m_bReadOnly = ((dwOpenFlags & RESPACK_OPEN_WRITE) == 0);
	_ReadHeaderAndTable();
	return TRUE;
}

BOOL CResPackInfo::_IsValidIndex(LPCRESPACKINDEX pIdx) const
{
	return pIdx != NULL
		&& pIdx->dwLength > 0
		&& pIdx->dwPosition >= RESPACK_CONTENTS_START_POS
		&& pIdx->dwPosition + pIdx->dwLength <= m_hdr.dwIdxPos;
}

BOOL CResPackInfo::IsReadOnly() const
{
	if (m_bReadOnly)
		::SetLastError(ERROR_FILE_READ_ONLY);
	return m_bReadOnly;		
}

DWORD CResPackInfo::GetCount() const
{
	return m_hdr.dwCount;
}

LPRESPACKINDEX CResPackInfo::Lookup(DWORD dwResID)
{
	RESPACKINDEXTABLE_IT it = m_table.find(dwResID);
	if (it == m_table.end())
	{
		::SetLastError(ERROR_NOT_FOUND);
		return NULL;
	}

	LPRESPACKINDEX pIdx = &it->second;
	if (_IsValidIndex(pIdx))
	{
		::SetLastError(ERROR_ALREADY_EXISTS);
		return pIdx;
	}
	else
	{
		::SetLastError(ERROR_NOT_FOUND);
		return NULL;
	}
}

LPCRESPACKINDEX CResPackInfo::Lookup(DWORD dwResID) const
{
	RESPACKINDEXTABLE_CIT it = m_table.find(dwResID);
	if (it == m_table.end())
	{
		::SetLastError(ERROR_NOT_FOUND);
		return NULL;
	}

	LPCRESPACKINDEX pIdx = &it->second;
	if (_IsValidIndex(pIdx))
	{
		::SetLastError(ERROR_ALREADY_EXISTS);
		return pIdx;
	}
	else
	{
		::SetLastError(ERROR_NOT_FOUND);
		return NULL;
	}
}

BOOL CResPackInfo::EnumResources(RESPACK_ENUM_PROC lpfnCallback, LPARAM lParam) const
{
	if (lpfnCallback == NULL)
		return TRUE;

	for (RESPACKINDEXTABLE_CIT it = m_table.begin(); it != m_table.end(); it++)
	{			
		DWORD dwResID = it->first;
		LPCRESPACKINDEX pIdx = &it->second;
		assert(pIdx);
		
		if (!lpfnCallback(dwResID, pIdx->dwLength, lParam))
			return FALSE;
	}
	return TRUE;	
}

BOOL CResPackInfo::_ReadHeaderAndTable()
{
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		assert(FALSE);
		return FALSE;
	}

	_Initialize();
	DWORD dwFileLen = ::GetFileSize(m_hFile, NULL);
	if (dwFileLen < RESPACK_CONTENTS_START_POS)
		return FALSE;		

	::SetFilePointer(m_hFile, 0, 0, FILE_BEGIN);
	DWORD dwRead = 0;
	
	// Load the GUID
	GUID guid;
	BOOL bOK = ::ReadFile(m_hFile, &guid, sizeof(GUID), &dwRead, NULL);
	if (!bOK || dwRead != sizeof(GUID) || ::memcmp(&guid, &RESPACKGUID, sizeof(GUID)))
		return FALSE;

	// Load the pack header
	bOK = ::ReadFile(m_hFile, &m_hdr, sizeof(RESPACKHEADER), &dwRead, NULL);
	if (!bOK 
		|| dwRead != sizeof(RESPACKHEADER)
		|| m_hdr.dwIdxPos < RESPACK_CONTENTS_START_POS
		|| m_hdr.dwIdxPos >= dwFileLen)
	{
		_Initialize();
		return FALSE;
	}	

	// Load the index table
	::SetFilePointer(m_hFile, m_hdr.dwIdxPos, 0, FILE_BEGIN);
	RESPACKINDEX idx;
	BOOL bInvalid = FALSE;
	for (DWORD i = 0; i < m_hdr.dwCount; i++)
	{
		DWORD dwResID = 0;
		bOK = ::ReadFile(m_hFile, &dwResID, sizeof(DWORD), &dwRead, NULL);
		if (!bOK || dwRead != sizeof(DWORD))
		{
			bInvalid = TRUE;
			break;
		}

		bOK = ::ReadFile(m_hFile, &idx, sizeof(RESPACKINDEX), &dwRead, NULL);		
		if (!bOK
			|| dwRead != sizeof(RESPACKINDEX)
			|| m_table.find(dwResID) != m_table.end()
			|| !_IsValidIndex(&idx))
		{
			bInvalid = TRUE;
			break;
		}
		
		m_table[dwResID] = idx;
	}

	if (bInvalid || m_hdr.dwCount != m_table.size())
	{
		_Initialize();
		return FALSE;
	}

	return TRUE;
}

BOOL CResPackInfo::_WriteHeaderAndTable() const
{
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		assert(FALSE);
		return FALSE;
	}

	assert(!m_bReadOnly);
	DWORD dwWritten = 0;

	// write guid
	::SetFilePointer(m_hFile, 0, 0, FILE_BEGIN);
	::WriteFile(m_hFile, &RESPACKGUID, sizeof(GUID), &dwWritten, NULL);

	// write header
	::WriteFile(m_hFile, &m_hdr, sizeof(RESPACKHEADER), &dwWritten, NULL);

	// write index table
	::SetFilePointer(m_hFile, m_hdr.dwIdxPos, 0, FILE_BEGIN);
	for (RESPACKINDEXTABLE_CIT it = m_table.begin(); it != m_table.end(); it++)
	{			
		DWORD dwResID = it->first;
		LPCRESPACKINDEX pIdx = &it->second;
		assert(pIdx);
		::WriteFile(m_hFile, &dwResID, sizeof(DWORD), &dwWritten, NULL);
		::WriteFile(m_hFile, pIdx, sizeof(RESPACKINDEX), &dwWritten, NULL);
	}

	// Truncate the file if is calling "ResPackRemove".
	::SetEndOfFile(m_hFile);
	return TRUE;
}

BOOL CResPackInfo::AddRes(DWORD dwResID, LPCVOID pData, DWORD dwDataSize, BOOL bCompress)
{
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		assert(FALSE);
		return FALSE;
	}

	if (IsReadOnly())
		return FALSE;

	if (pData == NULL || dwDataSize == 0)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	if (Lookup(dwResID))
		return FALSE;

	::SetFilePointer(m_hFile, m_hdr.dwIdxPos, 0, FILE_BEGIN);
	DWORD dwWritten = 0;
	DWORD dwCompressedLen = dwDataSize;

	if (bCompress)
	{
		DWORD dwEstimate = ZCompressBound(dwDataSize);
		assert(dwEstimate);
		LPBYTE p = new BYTE[dwEstimate];
		assert(p);
		if (p)
			dwCompressedLen = ZCompress(pData, dwDataSize, p, dwEstimate);
		::WriteFile(m_hFile, p, dwCompressedLen, &dwWritten, NULL);
		delete [] p;
	}
	else
	{
		::WriteFile(m_hFile, pData, dwDataSize, &dwWritten, NULL);
	}	
	
	if (dwWritten != dwCompressedLen)
		return FALSE;

	RESPACKINDEX idx = { 0 };
	idx.dwLength = dwWritten;
	idx.dwPosition = m_hdr.dwIdxPos;
	idx.bCompressed = bCompress;
	idx.dwOldLength = dwDataSize;
	m_hdr.dwCount++;
	m_hdr.dwIdxPos += dwWritten;
	m_table[dwResID] = idx;	
	return _WriteHeaderAndTable();
}

DWORD CResPackInfo::ExtractRes(DWORD dwResID, LPVOID pBuffer, DWORD dwBufferSize) const
{
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		assert(FALSE);
		return 0;
	}

	LPCRESPACKINDEX pIdx = Lookup(dwResID);
	if (pIdx == NULL)
		return 0;

	::SetFilePointer(m_hFile, pIdx->dwPosition, 0, FILE_BEGIN);
	DWORD dwRead = 0;

	if (pIdx->bCompressed)
	{
		LPBYTE p = new BYTE[pIdx->dwLength];
		::ReadFile(m_hFile, p, pIdx->dwLength, &dwRead, NULL);
		dwRead = ZExtract(p, dwRead, pBuffer, dwBufferSize);
		delete [] p;
	}
	else
	{
		::ReadFile(m_hFile, pBuffer, min(dwBufferSize, pIdx->dwLength), &dwRead, NULL);
	}
	
	return dwRead;
}

BOOL CResPackInfo::RemoveRes(DWORD dwResID)
{
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		assert(FALSE);
		return FALSE;
	}

	if (IsReadOnly())
		return FALSE;

	LPCRESPACKINDEX pDelIdx = Lookup(dwResID);	
	if (pDelIdx == NULL)
		return FALSE;

	const DWORD SHIFTLOW = pDelIdx->dwPosition;
	const DWORD SHIFTHIGH = pDelIdx->dwPosition + pDelIdx->dwLength;
	const DWORD SHIFTLEN = pDelIdx->dwLength;
	pDelIdx = NULL;	

	// Left-shift file contents, all contents start from SHIFTHIGH, shift left-ward to SHIFTLOW,
	// to avoid possible huge memory allocation, we move RESPACK_SHIFT_BLOCK of bytes a time.
	LPBYTE p = new BYTE[RESPACK_SHIFT_BLOCK];
	DWORD dwDest = SHIFTLOW;
	DWORD dwSrc = SHIFTHIGH;
	DWORD dwRead = RESPACK_SHIFT_BLOCK;
	while (dwRead == RESPACK_SHIFT_BLOCK && dwSrc < m_hdr.dwIdxPos)
	{
		DWORD dwDummy = 0;		
		::SetFilePointer(m_hFile, dwSrc, 0, FILE_BEGIN);
		::ReadFile(m_hFile, p, min(RESPACK_SHIFT_BLOCK, m_hdr.dwIdxPos - dwSrc), &dwRead, NULL);
		if (dwRead)
		{
			::SetFilePointer(m_hFile, dwDest, 0, FILE_BEGIN);
			::WriteFile(m_hFile, p, dwRead, &dwDummy, NULL);
			dwDest += dwRead;
			dwSrc += dwRead;
		}
	}
	delete [] p;

	// Update index table
	m_table.erase(dwResID);
	for (RESPACKINDEXTABLE_IT it = m_table.begin(); it != m_table.end(); it++)
	{			
		LPRESPACKINDEX pIdx = &it->second;
		assert(pIdx);
		if (pIdx->dwPosition >= SHIFTHIGH)
			pIdx->dwPosition -= SHIFTLEN;	
	}

	m_hdr.dwCount--;
	m_hdr.dwIdxPos -= SHIFTLEN;
	
	return _WriteHeaderAndTable();
}

BOOL CResPackInfo::ModifyResID(DWORD dwResID, DWORD dwNewID)
{
	if (IsReadOnly())
		return FALSE;
	
	LPRESPACKINDEX pIdx = Lookup(dwResID);
	if (pIdx == NULL)
		return FALSE; // old resource not found

	if (Lookup(dwNewID))
		return FALSE; // new resource exists

	RESPACKINDEX idx;
	::memcpy(&idx, pIdx, sizeof(RESPACKINDEX));
	m_table.erase(dwResID);
	m_table[dwNewID] = idx;
	_WriteHeaderAndTable();
	return TRUE;
}
