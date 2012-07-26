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

#include <qwinhost.h>

#include <windows.h>

class HostWindow : public QWinHost
{
    Q_OBJECT
public:
    HostWindow(QWidget *parent = 0, Qt::WFlags f = 0)
	: QWinHost(parent, f)
    {
        setFocusPolicy(Qt::StrongFocus);
    }

    HWND createWindow(HWND parent, HINSTANCE instance)
    {
	static ATOM windowClass = 0;
	if (!windowClass) {
	    WNDCLASSEX wcex;
	    wcex.cbSize		= sizeof(WNDCLASSEX);
	    wcex.style		= CS_HREDRAW | CS_VREDRAW;
	    wcex.lpfnWndProc	= (WNDPROC)WndProc;
	    wcex.cbClsExtra	= 0;
	    wcex.cbWndExtra	= 0;
	    wcex.hInstance	= instance;
	    wcex.hIcon		= NULL;
	    wcex.hCursor	= LoadCursor(NULL, IDC_ARROW);
	    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	    wcex.lpszMenuName	= NULL;
	    wcex.lpszClassName	= L"qtest";
	    wcex.hIconSm	= NULL;

	    windowClass = RegisterClassEx(&wcex);
	}

	HWND hwnd = CreateWindow((TCHAR*)windowClass, 0, WS_CHILD|WS_CLIPSIBLINGS|WS_TABSTOP, 
	    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, parent, NULL, instance, NULL);

	return hwnd;
    }

signals:
    void message(const QString &msg, int timeout);

public slots:
    void returnPressed()
    {
        QMessageBox::information(topLevelWidget(), "Message from Qt", "Return pressed in QLineEdit!");
    }

protected:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
	QWidget *widget = QWidget::find(GetParent(hWnd));
	HostWindow *window = qobject_cast<HostWindow*>(widget);

	if (window) switch (message) {
	case WM_SETFOCUS:
	    window->message("SetFocus for Win32 window!", 1000);
	    break;
	case WM_KILLFOCUS:
	    window->message("KillFocus for Win32 window!", 1000);
	    break;
        case WM_MOUSEMOVE:
            window->message("Moving the mouse, aren't we?", 200);
            break;
	case WM_KEYDOWN:
	    if (wParam != VK_TAB)
		window->message("Key Pressed!", 500);
	    break;
	default:
	    return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
    }
};

#include "main.moc"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    QMainWindow mw;
    mw.menuBar()->addMenu("&File")->addAction("&Exit", &a, SLOT(quit()));

    QWidget central(&mw);

    QLineEdit edit(&central);
    HostWindow host(&central);
    QObject::connect(&host, SIGNAL(message(const QString&,int)), mw.statusBar(), SLOT(showMessage(const QString&,int)));
    QObject::connect(&edit, SIGNAL(returnPressed()), &host, SLOT(returnPressed()));

    QVBoxLayout vbox(&central);
    vbox.addWidget(&edit);
    vbox.addWidget(&host);

    mw.setCentralWidget(&central);
    mw.show();
    return a.exec();
}
