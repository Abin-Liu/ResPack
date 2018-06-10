/////////////////////////////////////////////////////////////////////////
// ResPack.h
//
// This is a library that packs multiple resource data, either memory
// blocks or disk files, into one integrated resource package file,
// compression is supported by using Jean-loup Gailly and Mark Adler's
// "ZLIB" library.
// 
// Data in the package file can be found, enumerated, extracted(either to
// memory blocks or to disk files), or removed at anytime. An internal
// std::map is used to store data indices for fast lookup.
//
// Data stored in the package file are uniquely identify by DWORD type ID's,
// which means you cannot pack two resources with same ID, doing so in 
// "ResPackAddData", "ResPackAddFileA" or "ResPackAddFileW" will cause
// those functions to fail.
//
// Written by Abin (abinn32@yahoo.com)
// 24th Feb, 2007
/////////////////////////////////////////////////////////////////////////

#ifndef __RESPACK_H__
#define __RESPACK_H__

#include <windows.h>

/////////////////////////////////////////////////////////////////////////
// Open flags for "ResPackOpen"
/////////////////////////////////////////////////////////////////////////
#define RESPACK_OPEN_READ		0x80000000L // Open for read only.
#define RESPACK_OPEN_WRITE		0x40000000L // Open for write.
#define RESPACK_OPEN_CREATE		0x00000002L // Must be combined with RESPACK_OPEN_WRITE, if the
                                            // package file exists, its contents are truncated, 
                                            // otherwise the file is created.


/////////////////////////////////////////////////////////////////////////
// RESPACK_ENUM_PROC
//-----------------------------------------------------------------------
// The resource  enumeration callback function. After a cll to "ResPackEnumRes",
// this function is called for each resource data found in the package file.
//-----------------------------------------------------------------------
// Parameters
//
// dwResID:     The unique resource ID.
// dwResLength: Length of the resource, in bytes.
// lParam:      The 32-bit user data comes with a previous call to "ResPackEnumRes"
//
// Return values
//
// Returns non-zero to proceed, return zero to abort the enumeration
/////////////////////////////////////////////////////////////////////////
typedef BOOL (CALLBACK *RESPACK_ENUM_PROC)(DWORD dwResID, DWORD dwResLength, LPARAM lParam);


/////////////////////////////////////////////////////////////////////////
// ResPackOpen
//-----------------------------------------------------------------------
// Opens a resource package file from your storage disk. If dwOpenFlags
// does not include RESPACK_OPEN_CREATE, the function fails if the file
// does not exist.
//-----------------------------------------------------------------------
// Parameters
//
// lpszFileName: Full path of the resource package file.
// dwOpenFlags:  A combination of RESPACK_OPEN_READ, RESPACK_OPEN_WRITE and RESPACK_OPEN_CREATE.
//
// Return values
//
// Reutrns a handle to the resource package, or NULL if the function fails. The returned handle must be
// closed using "ResPackCloseHandle" to free all allocated memory.
/////////////////////////////////////////////////////////////////////////
HANDLE __declspec(dllexport) ResPackOpen(LPCSTR lpszFileName, DWORD dwOpenFlags);
HANDLE __declspec(dllexport) ResPackOpen(LPCWSTR lpszFileName, DWORD dwOpenFlags);

/////////////////////////////////////////////////////////////////////////
// ResPackCloseHandle
//-----------------------------------------------------------------------
// Closes an opened resource package file and free all allocated system
// resources. You MUST call this function to close every resource package
// handles that was returned by "ResPackOpen", failing to do so will cause
// serious memory leaks.
//-----------------------------------------------------------------------
// Parameters
//
// hResPack: The resource package handle returned by a previous call to "ResPackOpen".
//
// Return values
//
// Returns non-zero if succeeded, zero otherwise.
/////////////////////////////////////////////////////////////////////////
BOOL __declspec(dllexport) ResPackCloseHandle(HANDLE hResPack);

/////////////////////////////////////////////////////////////////////////
// ResPackAdd
//-----------------------------------------------------------------------
// Add a new resource data into an opened package. The data can be either
// a memory block of a disk file.
//-----------------------------------------------------------------------
// Parameters
//
// hResPack:     The resource package handle returned by a previous call to "ResPackOpen"
// dwResID:      ID of the resource which is to be added into the package.
// pData:        Address of the memory block which contains the resource data.
// dwDataSize:   Length of the resource data.
// lpszFileName: Full path of the file to be added into the package.
// bCompress:    Whether the new resource data should be compressed before being added into the package.
//
// Return values
//
// Returns non-zero if succeeded, zero otherwise.
/////////////////////////////////////////////////////////////////////////
BOOL __declspec(dllexport) ResPackAdd(HANDLE hResPack, DWORD dwResID, LPCVOID pData, DWORD dwDataSize, BOOL bCompress);
BOOL __declspec(dllexport) ResPackAdd(HANDLE hResPack, DWORD dwResID, LPCSTR lpszFileName, BOOL bCompress);
BOOL __declspec(dllexport) ResPackAdd(HANDLE hResPack, DWORD dwResID, LPCWSTR lpszFileName, BOOL bCompress);

/////////////////////////////////////////////////////////////////////////
// ResPackRemove
//-----------------------------------------------------------------------
// Removes a resource data from an opened package.
//-----------------------------------------------------------------------
// Parameters
//
// hResPack: the resource package handle returned by a previous call to "ResPackOpen"
// dwResID: ID of the resource which is to be added into the package.
//
// Return values
//
// Returns non-zero if succeeded, zero otherwise.
/////////////////////////////////////////////////////////////////////////
BOOL __declspec(dllexport) ResPackRemove(HANDLE hResPack, DWORD dwResID);

/////////////////////////////////////////////////////////////////////////
// ResPackExtract
//-----------------------------------------------------------------------
// Extract a resource from an opened package. The extracted data can be either
// written to a memory block or saved into a disk file.
//-----------------------------------------------------------------------
// Parameters
//
// hResPack:     The resource package handle returned by a previous call to "ResPackOpen"
// dwResID:      ID of the resource which is to be added into the package.
// pBuffer:      Address of the memory block which will receives the resource data.
// dwBufferSize: Size of the memory block.
// lpszFileName: Full path of the file, to which the extracted resource data will be saved.
//               If the file exists, all of its contents are truncated, otherwise the file
//               will be created.
//
// Return values
//
// Returns length of the extracted resource data if succeeded, zero otherwise.
/////////////////////////////////////////////////////////////////////////
DWORD __declspec(dllexport) ResPackExtract(HANDLE hResPack, DWORD dwResID, LPVOID pBuffer, DWORD dwBufferSize);
DWORD __declspec(dllexport) ResPackExtract(HANDLE hResPack, DWORD dwResID, LPCSTR lpszFileName);
DWORD __declspec(dllexport) ResPackExtract(HANDLE hResPack, DWORD dwResID, LPCWSTR lpszFileName);

/////////////////////////////////////////////////////////////////////////
// ResPackModify
//-----------------------------------------------------------------------
// Modifies an existing resource data in an opened package, changes its
// resource ID or contents.
//-----------------------------------------------------------------------
// Parameters
//
// hResPack:     The resource package handle returned by a previous call to "ResPackOpen"
// dwResID:      ID of the existing resource which is to be modified.
// dwNewID:      The new ID for the resource.
// pData:        Address of the memory block which contains the new data.
// dwDataSize:   Length of the resource data.
// lpszFileName: Full path of the file whose contents is to be treated as the new contents of the existing resource.
// bCompress:    Whether the new resource data should be compressed before being applied into the package.
//
// Return values
//
// Returns size of the new resource if succeeded, zero otherwise.
/////////////////////////////////////////////////////////////////////////
BOOL __declspec(dllexport) ResPackModify(HANDLE hResPack, DWORD dwResID, DWORD dwNewID);
BOOL __declspec(dllexport) ResPackModify(HANDLE hResPack, DWORD dwResID, LPCVOID pData, DWORD dwDataSize, BOOL bCompress);
BOOL __declspec(dllexport) ResPackModify(HANDLE hResPack, DWORD dwResID, LPCSTR lpszFileName, BOOL bCompress);
BOOL __declspec(dllexport) ResPackModify(HANDLE hResPack, DWORD dwResID, LPCWSTR lpszFileName, BOOL bCompress);

/////////////////////////////////////////////////////////////////////////
// ResPackGetCount
//-----------------------------------------------------------------------
// Retrives number of resource data in an opened package.
//-----------------------------------------------------------------------
// Parameters
//
// hResPack: The resource package handle returned by a previous call to "ResPackOpen".
//
// Return values
//
// Returns number of resources in the package file.
/////////////////////////////////////////////////////////////////////////
DWORD __declspec(dllexport) ResPackGetCount(HANDLE hResPack);

/////////////////////////////////////////////////////////////////////////
// ResPackLookup
//-----------------------------------------------------------------------
// Searches for a particular resource in an opened package and returns the
// actual length of the found resource data. You can use this function to
// determine size of the memory needed to be allocated before extracting
// a resource data. Since all resource data stored in a package file are
// stored in a std::map using unique index ID's, this operation is fast even
// when number of resources are large.
//-----------------------------------------------------------------------
// Parameters
//
// hResPack: The resource package handle returned by a previous call to "ResPackOpen"
// dwResID:  ID of the resource which is to be searched for.
//
// Return values
//
// Returns length of the resource data, zero  if it does not exist.
/////////////////////////////////////////////////////////////////////////
DWORD __declspec(dllexport) ResPackLookup(HANDLE hResPack, DWORD dwResID);

/////////////////////////////////////////////////////////////////////////
// ResPackEnumRes
//-----------------------------------------------------------------------
// Enumerates all existing resource data in an opened package.
//-----------------------------------------------------------------------
// Parameters
//
// hResPack:     The resource package handle returned by a previous call to "ResPackOpen"
// lpfnCallback: Address of the callback function.
// lParam:       A 32-bit user defined data.
//
// Return values
//
// Returns non-zero if succeeded, zero otherwise.
/////////////////////////////////////////////////////////////////////////
BOOL __declspec(dllexport) ResPackEnumRes(HANDLE hResPack, RESPACK_ENUM_PROC lpfnCallback, LPARAM lParam);

#endif // __RESPACK_H__