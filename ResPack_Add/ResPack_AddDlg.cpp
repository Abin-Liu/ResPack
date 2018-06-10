// ResPack_AddDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResPack_Add.h"
#include "ResPack_AddDlg.h"
#include "..\\dll\\ResPack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CResPack_AddDlg dialog

CResPack_AddDlg::CResPack_AddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResPack_AddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResPack_AddDlg)
	m_bCompress = TRUE;
	m_sDefFmt = _T("%TITLE%_%EXT%");
	m_bGenDef = TRUE;
	m_dwIDStart = 1;
	m_bTruncate = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CResPack_AddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResPack_AddDlg)
	DDX_Control(pDX, IDC_PACKAGEPATH, m_wndPackagePath);
	DDX_Control(pDX, IDC_FILES, m_wndFiles);
	DDX_Control(pDX, IDC_DEFPATH, m_wndDefPath);
	DDX_Check(pDX, IDC_COMPRESS, m_bCompress);
	DDX_Text(pDX, IDC_DEFFMT, m_sDefFmt);
	DDX_Check(pDX, IDC_GENDEF, m_bGenDef);
	DDX_Text(pDX, IDC_IDSTART, m_dwIDStart);
	DDX_Check(pDX, IDC_TRUNCATE, m_bTruncate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CResPack_AddDlg, CDialog)
	//{{AFX_MSG_MAP(CResPack_AddDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResPack_AddDlg message handlers

BOOL CResPack_AddDlg::OnInitDialog()
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
	m_wndPackagePath.SetFileFlags(m_wndPackagePath.GetFileFlags() & ~OFN_ALLOWMULTISELECT);
	m_wndPackagePath.SetOpenSave(FALSE);
	m_wndFiles.SetFileFlags(m_wndFiles.GetFileFlags() | OFN_FILEMUSTEXIST);
	m_wndDefPath.SetFileFlags(m_wndDefPath.GetFileFlags() & ~OFN_ALLOWMULTISELECT);
	m_wndDefPath.SetOpenSave(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CResPack_AddDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CResPack_AddDlg::OnPaint() 
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
HCURSOR CResPack_AddDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CResPack_AddDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	if (!UpdateData(TRUE))
		return;
	
	CString sPackagePath = m_wndPackagePath.GetPathName();
	if (sPackagePath.IsEmpty())
	{
		MessageBox(_T("Please select the package file."), _T("Invalid Input"), MB_ICONEXCLAMATION);
		return;
	}

	CString sFiles = m_wndFiles.GetPathName();
	if (sFiles.IsEmpty())
	{
		MessageBox(_T("Please select source files to add."), _T("Invalid Input"), MB_ICONEXCLAMATION);
		return;
	}

	CString sDefPath = m_wndDefPath.GetPathName();
	if (m_bGenDef && sDefPath.IsEmpty())
	{
		MessageBox(_T("Please specify the definition file."), _T("Invalid Input"), MB_ICONEXCLAMATION);
		return;
	}

	CString sMsg;
	DWORD dwFlag = RESPACK_OPEN_READ | RESPACK_OPEN_WRITE;
	if (m_bTruncate)
		dwFlag |= RESPACK_OPEN_CREATE;
	HANDLE hRes = ResPackOpen(sPackagePath, dwFlag);
	if (hRes == NULL)
	{
		sMsg.Format(_T("Unable to open package file:\n%s"), (LPCTSTR)sPackagePath);
		MessageBox(sMsg, _T("File Open Failure"), MB_ICONEXCLAMATION);
		return;
	}

	CStdioFile file;
	if (m_bGenDef)
		file.Open(sDefPath, CFile::modeWrite | CFile::modeCreate);
	
	DWORD dwCurResID = m_dwIDStart;
	int nCount = 0;
	POSITION pos = m_wndFiles.GetStartPosition();
	while (pos)
	{
		CString sFile = m_wndFiles.GetNextPathName(pos);
		if (sFile.IsEmpty())
			continue;

		if (ResPackAdd(hRes, dwCurResID, sFile, m_bCompress))
		{
			if (m_bGenDef)
			{
				int n = sFile.ReverseFind(_T('\\'));
				if (n != -1)
					sFile = sFile.Mid(n + 1);

				sFile.Replace(_T(' '), _T('_'));
				sFile.Replace(_T('-'), _T('_'));
				sFile.Replace(_T('.'), _T('_'));
				sFile.MakeUpper();

				CString str;
				str.Format(_T("#define %s\t\t\t%d\n"), (LPCTSTR)sFile, dwCurResID);				
				file.WriteString(str);
			}

			nCount++;
			dwCurResID++;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	if (m_bGenDef)
		file.Close();

	ResPackCloseHandle(hRes);

	sMsg.Format(_T("%d files have been added to the package."), nCount);
	MessageBox(sMsg, _T("Add Completed"), MB_ICONINFORMATION);
}
