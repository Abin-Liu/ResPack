; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CResPack_ManipDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ResPack_Manip.h"

ClassCount=3
Class1=CResPack_ManipApp
Class2=CResPack_ManipDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_RESPACK_MANIP_DIALOG

[CLS:CResPack_ManipApp]
Type=0
HeaderFile=ResPack_Manip.h
ImplementationFile=ResPack_Manip.cpp
Filter=N

[CLS:CResPack_ManipDlg]
Type=0
HeaderFile=ResPack_ManipDlg.h
ImplementationFile=ResPack_ManipDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST1

[CLS:CAboutDlg]
Type=0
HeaderFile=ResPack_ManipDlg.h
ImplementationFile=ResPack_ManipDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_RESPACK_MANIP_DIALOG]
Type=1
Class=CResPack_ManipDlg
ControlCount=7
Control1=IDC_STATIC,static,1342308352
Control2=IDC_PACKAGEPATH,button,1342242816
Control3=IDC_COUNT,static,1342308352
Control4=IDC_LIST1,SysListView32,1350664205
Control5=IDC_EXTRACT,button,1476460544
Control6=IDC_REMOVE,button,1476460544
Control7=IDCANCEL,button,1342242816

