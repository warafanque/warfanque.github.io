// SphereGouraud.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "SphereGouraud.h"
#include "SphereGouraudDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSphereGouraudApp

BEGIN_MESSAGE_MAP(CSphereGouraudApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSphereGouraudApp construction

CSphereGouraudApp::CSphereGouraudApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSphereGouraudApp object

CSphereGouraudApp theApp;


// CSphereGouraudApp initialization

BOOL CSphereGouraudApp::InitInstance()
{
	CWinApp::InitInstance();

	// Standard initialization
	CSphereGouraudDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
