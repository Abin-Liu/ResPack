// ResPack_ManipDlg.h : header file
//

#if !defined(AFX_RESPACK_MANIPDLG_H__4CFDDA96_5F4C_4112_90B1_1266553EC72B__INCLUDED_)
#define AFX_RESPACK_MANIPDLG_H__4CFDDA96_5F4C_4112_90B1_1266553EC72B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CResPack_ManipDlg dialog

#include "BrowseCtrl.h"
#include "..\\dll\\ResPack.h"

class CResPack_ManipDlg : public CDialog
{
// Construction
public:
	CResPack_ManipDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CResPack_ManipDlg)
	enum { IDD = IDD_RESPACK_MANIP_DIALOG };
	CButton	m_wndRemove;
	CBrowseCtrl	m_wndPackagePath;
	CListCtrl	m_wndList;
	CButton	m_wndExtract;
	CString	m_sCount;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResPack_ManipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CResPack_ManipDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRemove();
	afx_msg void OnExtract();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static BOOL CALLBACK ResEnumProc(DWORD dwResID, DWORD dwResLength, LPARAM lParam);
	LRESULT OnFileSelected(WPARAM wParam, LPARAM lParam);

	HANDLE m_hRes;	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESPACK_MANIPDLG_H__4CFDDA96_5F4C_4112_90B1_1266553EC72B__INCLUDED_)
