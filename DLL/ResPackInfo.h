#ifndef __RESPACKINFO_H__
#define __RESPACKINFO_H__

#define RESPACK_MAJOR_VER		1
#define RESPACK_MINOR_VER		0

// Disable some STL warnings
#pragma warning(disable: 4786)
#pragma warning(disable: 4100)
#pragma warning(disable: 4512)
#pragma warning(disable: 4710)

#include <windows.h>
#include <map>
#include "ResPack.h"
#include <assert.h>

// The ResPack header, stores basic information about the package
typedef struct _tagResPackHeader
{
	DWORD dwMajorVer; // Project major version
	DWORD dwMinorVer; // Project minor version
	DWORD dwCount; // Resource count
	DWORD dwIdxPos; // Start position of index table
} RESPACKHEADER, *LPRESPACKHEADER;
typedef const _tagResPackHeader* LPCRESPACKHEADER;

// Resource index, stores information for a resource data block resides in the package
typedef struct _tagResPackIndex
{
	DWORD dwPosition; // Resource start position
	DWORD dwLength; // Resource length
	BOOL bCompressed; // Is resource compressed?
	DWORD dwOldLength; // Resource original(uncompressed) length, if bCompressed is zero, this is same as dwLength
} RESPACKINDEX, *LPRESPACKINDEX;
typedef const _tagResPackIndex* LPCRESPACKINDEX;

typedef std::map<DWORD, RESPACKINDEX> RESPACKINDEXTABLE;
typedef RESPACKINDEXTABLE::iterator RESPACKINDEXTABLE_IT;
typedef RESPACKINDEXTABLE::const_iterator RESPACKINDEXTABLE_CIT;

// What "ResPackOpen" function returns is actually a pointer to such a class... 
class CResPackInfo
{
public:

	CResPackInfo();
	~CResPackInfo();

	static CResPackInfo* VerifyHandle(HANDLE hResPack);
	BOOL OpenPackageFileA(LPCSTR lpszFileName, DWORD dwOpenFlags);
	BOOL OpenPackageFileW(LPCWSTR lpszFileName, DWORD dwOpenFlags);
	void ClosePackageFile();

	BOOL IsReadOnly() const;
	DWORD GetCount() const;
	LPRESPACKINDEX Lookup(DWORD dwResID);
	LPCRESPACKINDEX Lookup(DWORD dwResID) const;
	BOOL EnumResources(RESPACK_ENUM_PROC lpfnCallback, LPARAM lParam) const;
	BOOL AddRes(DWORD dwResID, LPCVOID pData, DWORD dwDataSize, BOOL bCompress);
	DWORD ExtractRes(DWORD dwResID, LPVOID pBuffer, DWORD dwBufferSize) const;
	BOOL RemoveRes(DWORD dwResID);
	BOOL ModifyResID(DWORD dwResID, DWORD dwNewID);

private:

	void _Initialize();
	BOOL _IsValidIndex(LPCRESPACKINDEX pIdx) const;
	BOOL _ReadHeaderAndTable();
	BOOL _WriteHeaderAndTable() const;

private:

	GUID m_guid;
	BOOL m_bReadOnly; // Is the file read-only?
	HANDLE m_hFile; // Handle to the file
	RESPACKHEADER m_hdr; // File header
	RESPACKINDEXTABLE m_table; // The resource index table
};

#endif // __RESPACKINFO_H__
