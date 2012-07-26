/****************************************************************************
**
** This file is part of a Qt Solutions component.
** 
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
** 
** Contact:  Qt Software Information (qt-info@nokia.com)
** 
** Commercial Usage  
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage 
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** 
****************************************************************************/

// QtMfc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "qtmfc.h"

#include "mainframe.h"

#include <qmfcapp.h>
#include <qwinwidget.h>
#include <QtGui/QMessageBox>
#include "ui_optionsdialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WindowsApp

BEGIN_MESSAGE_MAP(WindowsApp, CWinApp)
	//{{AFX_MSG_MAP(WindowsApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_EDIT_OPTIONS, OnAppOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WindowsApp construction

WindowsApp::WindowsApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only WindowsApp object

WindowsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// WindowsApp initialization

BOOL WindowsApp::InitInstance()
{
	// Standard initialization

#if _MFC_VER < 0x0700
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	// Qt initialization
	QMfcApp::instance(this);


	MainFrame* pFrame = new MainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);



	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return true;
}

BOOL WindowsApp::Run()
{
    int result = QMfcApp::run(this);
    delete qApp;
    return result;
}

/////////////////////////////////////////////////////////////////////////////
// WindowsApp message handlers


// App command to run the dialog
void WindowsApp::OnAppAbout()
{
    QWinWidget win(theApp.m_pMainWnd);
    win.showCentered();
    QMessageBox::about(&win, "About QtMfc", "QtMfc Version 1.0\nCopyright (C) 2003");
}

void WindowsApp::OnAppOptions()
{
    QWinWidget *win = new QWinWidget(theApp.m_pMainWnd);
    win->showCentered();

    QDialog *dialog = new QDialog(win);
    Ui::OptionsDialog ui;
    ui.setupUi(dialog);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}
