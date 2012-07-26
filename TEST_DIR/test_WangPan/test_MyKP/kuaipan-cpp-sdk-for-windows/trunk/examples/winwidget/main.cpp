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

#include <QtGui>
#include <qwinwidget.h>

#include <windows.h>

HWND winId = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
    {
    case WM_LBUTTONUP:
	{
	    QWinWidget w(hWnd, 0, 0);
	    w.showCentered();
	    QMessageBox mb("Qt on Win32 - modal",
		"Is this dialog modal?",
		QMessageBox::NoIcon,
		QMessageBox::Yes | QMessageBox::Default,
		QMessageBox::No  | QMessageBox::Escape,
		QMessageBox::NoButton, &w);
	    int result = mb.exec();
	    Q_UNUSED(result);
	}
	break;

    case WM_RBUTTONUP:
	{
	    QWinWidget *w = new QWinWidget(hWnd, 0, 0);
	    w->showCentered();
	    QMessageBox *mb = new QMessageBox("Qt on Win32 - modeless",
		"Is this dialog modal?",
		QMessageBox::NoIcon,
		QMessageBox::Yes | QMessageBox::Default,
		QMessageBox::No  | QMessageBox::Escape,
		QMessageBox::NoButton, w);
            mb->setModal(false);
            mb->setAttribute(Qt::WA_DeleteOnClose);
	    mb->show();
	}
	break;

    case WM_KEYDOWN:
        if (wParam != VK_TAB)
            return DefWindowProc(hWnd, message, wParam, lParam);

        SetFocus(winId);

        break;

    case WM_SETFOCUS:
        {
            QString str("Got focus");
            QWidget *widget = QWidget::find(HWND(wParam));
            if (widget)
                str += QString(" from %1 (%2)").arg(widget->objectName()).arg(widget->metaObject()->className());
            str += "\n";
            OutputDebugStringA(str.toLocal8Bit().data());
        }
        break;

    case WM_KILLFOCUS:
        {
            QString str("Lost focus");
            QWidget *widget = QWidget::find(HWND(wParam));
            if (widget)
                str += QString(" to %1 (%2)").arg(widget->objectName()).arg(widget->metaObject()->className());
            str += "\n";

            OutputDebugStringA(str.toLocal8Bit().data());
        }
        break;

    case WM_DESTROY:
	PostQuitMessage(0);
	break;

    default:
	return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance,
		      HINSTANCE /*hPrevInstance*/,
		      LPTSTR    /*lpCmdLine*/,
		      int       nCmdShow)
{
    WNDCLASSEX wcex;
    
    wcex.cbSize = sizeof(WNDCLASSEX); 
    
    wcex.style		= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= (WNDPROC)WndProc;
    wcex.cbClsExtra	= 0;
    wcex.cbWndExtra	= 0;
    wcex.hInstance	= hInstance;
    wcex.hIcon		= NULL;
    wcex.hCursor	= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= L"qtest";
    wcex.hIconSm	= NULL;
    
    ATOM windowClass = RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow((TCHAR*)windowClass, L"Windows Migration Framework Example", 
	WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 0, 0, hInstance, 0);
    if (!hWnd)
	return FALSE;

    int argc = 0;
    QApplication a(argc, 0);

    QWinWidget win(hWnd);
    winId = win.winId();
    QHBoxLayout hbox(&win);
    hbox.setSpacing(5);
    hbox.setMargin(0);
    QPushButton *pb = new QPushButton("Qt command button", &win);
    pb->setObjectName("pb");
    hbox.addWidget(pb);
    QLabel *label = new QLabel("Some label", &win);
    label->setObjectName("label");
    hbox.addWidget(label);
    QLineEdit *le1 = new QLineEdit(&win);
    le1->setObjectName("le1");
    hbox.addWidget(le1);
    QLineEdit *le2 = new QLineEdit(&win);
    le1->setObjectName("le2");
    hbox.addWidget(le2);
    QLineEdit *le3 = new QLineEdit(&win);
    le1->setObjectName("le3");
    hbox.addWidget(le3);
    
    win.move(0, 0);
    win.show();

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return a.exec();
}
