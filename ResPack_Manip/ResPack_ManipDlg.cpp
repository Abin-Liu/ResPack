// ResPack_ManipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResPack_Manip.h"
#include "ResPack_ManipDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_FILE_SELECTED	(WM_APP + 100)

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResPack_ManipDlg dialog

CResPack_ManipDlg::CResPack_ManipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResPack_ManipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResPack_ManipDlg)
	m_sCount = _T("Found 0 resources in the selected file.");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hRes = NULL;
}

void CResPack_ManipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResPack_ManipDlg)
	DDX_Control(pDX, IDC_REMOVE, m_wndRemove);
	DDX_Control(pDX, IDC_PACKAGEPATH, m_wndPackagePath);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Control(pDX, IDC_EXTRACT, m_wndExtract);
	DDX_Text(pDX, IDC_COUNT, m_sCount);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CResPack_ManipDlg, CDialog)
	//{{AFX_MSG_MAP(CResPack_ManipDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_EXTRACT, OnExtract)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FILE_SELECTED, OnFileSelected)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResPack_ManipDlg message handlers

BOOL CResPack_ManipDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_wndPackagePath.SetNotifyMessageID(WM_FILE_SELECTED);
	m_wndList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(1, _T("Length (Bytes)"), LVCFMT_RIGHT, 150);
	m_wndList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CResPack_ManipDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CResPack_ManipDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CResPack_ManipDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CResPack_ManipDlg::OnDestroy() 
{
	if (m_hRes)
		ResPackCloseHandle(m_hRes);
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

LRESULT CResPack_ManipDlg::OnFileSelected(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDOK)
	{
		if (m_hRes)
			ResPackCloseHandle(m_hRes);

		m_hRes = ResPackOpen(m_wndPackagePath.GetPathName(), RESPACK_OPEN_READ | RESPACK_OPEN_WRITE);
		m_sCount.Format(_T("Found %d resources in the selected file."), ResPackGetCount(m_hRes));
		UpdateData(FALSE);

		m_wndList.DeleteAllItems();
		ResPackEnumRes(m_hRes, ResEnumProc, (LPARAM)&m_wndList);
	}

	return 0;
}

BOOL CResPack_ManipDlg::ResEnumProc(DWORD dwResID, DWORD dwResLength, LPARAM lParam)
{
	CListCtrl* p = (CListCtrl*)lParam;
	ASSERT(p);
	ASSERT(p->GetSafeHwnd());
	CString str;
	str.Format(_T("%d"), dwResID);
	int nIdx = p->InsertItem(INT_MAX, str);
	str.Format(_T("%d"), dwResLength);
	p->SetItemText(nIdx, 1, str);
	p->SetItemData(nIdx, dwResID);
	return TRUE;
}

void CResPack_ManipDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	m_wndExtract.EnableWindow(pos != NULL);
	m_wndRemove.EnableWindow(pos != NULL);
	*pResult = 0;
}

void CResPack_ManipDlg::OnRemove() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	int nIdx = m_wndList.GetNextSelectedItem(pos);
	if (nIdx == -1)
		return;

	if (ResPackRemove(m_hRes, m_wndList.GetItemData(nIdx)))
		m_wndList.DeleteItem(nIdx);	
}

void CResPack_ManipDlg::OnExtract() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	int nIdx = m_wndList.GetNextSelectedItem(pos);
	if (nIdx == -1)
		return;

	CFileDialog dlg(FALSE);
	if (dlg.DoModal() == IDOK)
	{
		if (ResPackExtract(m_hRes, m_wndList.GetItemData(nIdx), dlg.GetPathName()))
			MessageBox(_T("1 file extracted successfully."), _T("ResPack Extract"), MB_ICONINFORMATION);
		else
			MessageBox(_T("File extraction failed!"), _T("ResPack Extract"), MB_ICONEXCLAMATION);
	}
}
