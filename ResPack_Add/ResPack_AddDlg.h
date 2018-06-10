// ResPack_AddDlg.h : header file
//

#if !defined(AFX_RESPACK_ADDDLG_H__88BB725D_BE51_4947_9C07_5C775B998C61__INCLUDED_)
#define AFX_RESPACK_ADDDLG_H__88BB725D_BE51_4947_9C07_5C775B998C61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CResPack_AddDlg dialog

#include "BrowseCtrl.h"

class CResPack_AddDlg : public CDialog
{
// Construction
public:
	CResPack_AddDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CResPack_AddDlg)
	enum { IDD = IDD_RESPACK_ADD_DIALOG };
	CBrowseCtrl	m_wndPackagePath;
	CBrowseCtrl	m_wndFiles;
	CBrowseCtrl	m_wndDefPath;
	BOOL	m_bCompress;
	CString	m_sDefFmt;
	BOOL	m_bGenDef;
	DWORD	m_dwIDStart;
	BOOL	m_bTruncate;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResPack_AddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CResPack_AddDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESPACK_ADDDLG_H__88BB725D_BE51_4947_9C07_5C775B998C61__INCLUDED_)
