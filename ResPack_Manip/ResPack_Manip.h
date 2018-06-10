// ResPack_Manip.h : main header file for the RESPACK_MANIP application
//

#if !defined(AFX_RESPACK_MANIP_H__99AEE2B6_B2BA_4024_88C8_61FCC2DE9E40__INCLUDED_)
#define AFX_RESPACK_MANIP_H__99AEE2B6_B2BA_4024_88C8_61FCC2DE9E40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CResPack_ManipApp:
// See ResPack_Manip.cpp for the implementation of this class
//

class CResPack_ManipApp : public CWinApp
{
public:
	CResPack_ManipApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResPack_ManipApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CResPack_ManipApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESPACK_MANIP_H__99AEE2B6_B2BA_4024_88C8_61FCC2DE9E40__INCLUDED_)
