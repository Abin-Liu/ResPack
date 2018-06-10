// ResPack_Add.h : main header file for the RESPACK_ADD application
//

#if !defined(AFX_RESPACK_ADD_H__D838E70F_773B_4464_A252_DD9E37B22253__INCLUDED_)
#define AFX_RESPACK_ADD_H__D838E70F_773B_4464_A252_DD9E37B22253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CResPack_AddApp:
// See ResPack_Add.cpp for the implementation of this class
//

class CResPack_AddApp : public CWinApp
{
public:
	CResPack_AddApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResPack_AddApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CResPack_AddApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESPACK_ADD_H__D838E70F_773B_4464_A252_DD9E37B22253__INCLUDED_)
