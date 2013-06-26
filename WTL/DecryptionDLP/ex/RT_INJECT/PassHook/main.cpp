//��ʾ*�����ݵĹ��ӳ���
#include "main.h"

//---------------------
#pragma data_seg("ALLDATA")
HHOOK g_hHook = NULL;
HINSTANCE g_hDllIns = NULL;
HWND g_hHostWnd = NULL;
#pragma data_seg()

//DLL��ں���
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{

    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        g_hDllIns = hinstDLL;
        DisableThreadLibraryCalls(hinstDLL);

        break;
    }
    return TRUE;
}

//��ȡ������
HWND WINAPI GetParentFromHwnd(HWND hWnd)
{
    HWND hParWnd;
    try
    {
        do
        {
            hParWnd = GetParent(hWnd);
            if (hParWnd)
            {
                hWnd = hParWnd;
            }
        }
        while (hParWnd);
    }
    catch (...)
    {
    }

    return hWnd;
}

//��ȡpt���ϵĴ���
HWND WINAPI GetWindowFromPoint(POINT pt)
{
    HWND hWnd = NULL;
    try
    {
        hWnd = ::WindowFromPoint(pt);

        if (!hWnd)return NULL;

        HWND hParWnd = GetParent(hWnd);
        if (!hParWnd)
            return hWnd;

        HWND hFindWnd = hWnd;
        RECT r, rFind;

        while (hFindWnd)
        {
            GetWindowRect(hWnd, &r);
            hFindWnd = ::GetWindow(hFindWnd, GW_HWNDNEXT);
            if (hFindWnd)
            {
                GetWindowRect(hFindWnd, &rFind);
                if (PtInRect(&rFind, pt) && GetParent(hFindWnd) == hParWnd && IsWindowVisible(hFindWnd) &&
                        (rFind.right - rFind.left) * (rFind.bottom - rFind.top) <
                        (r.right - r.left) * (r.bottom - r.top))
                {
                    hWnd = hFindWnd;
                }
            }
        }
    }
    catch (...)
    {
        hWnd = NULL;
    }

    return hWnd;
}
//���߿�
void WINAPI DrawBorder(HWND hWnd)
{
    if(!IsWindow(hWnd))
        return;

    RECT rect;

    GetWindowRect(hWnd, &rect);

    HDC hDC = GetWindowDC(hWnd);

    int iMode = SetROP2(hDC, R2_NOT);

    HPEN hPen = CreatePen(PS_INSIDEFRAME, 3 * GetSystemMetrics(SM_CXBORDER), RGB(0, 0, 0));

    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));

    Rectangle(hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top);

    SelectObject(hDC, hOldBrush);
    SelectObject(hDC, hOldPen);

    SetROP2(hDC, iMode);

    ReleaseDC(hWnd, hDC);

    DeleteObject(hPen);
}

//���ͻ�ȡpass��text����Ϣ
void WINAPI GetPassText(HWND hWndPass, HWND hWndShowPass)
{
    try
    {
        HWND hParWnd = GetParentFromHwnd(hWndPass);
        ::PostMessage(hParWnd, UM_GETPASS, (WPARAM)hWndPass, (LPARAM)hWndShowPass);
    }
    catch(...)
    {
    }
}

//���Ӻ���
LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
    try
    {
        if (code >= 0)
        {

            LPMSG pMsg = (LPMSG)lParam;

            HWND hShowPassWnd = (HWND)pMsg->lParam;

            if (pMsg->message == UM_GETPASS && g_hHostWnd == hShowPassWnd)  //��ȡ*���е�����
            {

                HWND hPassWnd = (HWND)pMsg->wParam; //*�Ŵ��ڵ�HWND

                if (hPassWnd)
                {
                    TCHAR szPassText[MAX_TEXTLEN];
                    ZeroMemory(szPassText, MAX_TEXTLEN);
                    ::SendMessage(hPassWnd, WM_GETTEXT, MAX_TEXTLEN, (LPARAM)szPassText); //��ȡ����

                    COPYDATASTRUCT cd;
                    ZeroMemory(&cd, sizeof(cd));
                    cd.dwData = (DWORD)hPassWnd;
                    cd.cbData = strlen(szPassText) + 1;
                    cd.lpData = szPassText;

                    SendMessage(hShowPassWnd, WM_COPYDATA, (WPARAM)hPassWnd, (LPARAM)&cd);//���͸�ShowPass����
                }
            }
        }
    }
    catch(...)
    {
    }

    return CallNextHookEx(g_hHook, code, wParam, lParam);
}

//��װ
BOOL WINAPI InitHook(HWND hWnd)
{
    try
    {
        if (!hWnd)
        {
            return FALSE;
        }

        g_hHostWnd = hWnd;
        g_hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, g_hDllIns, 0);
        return g_hHook != NULL;
    }
    catch (...)
    {
        return FALSE;
    }
}

//ж��
BOOL WINAPI ReleaseHook()
{
    BOOL bSuccess = FALSE;
    try
    {
        if(g_hHook != NULL)
        {
            bSuccess = UnhookWindowsHookEx(g_hHook) ? TRUE : FALSE;
            g_hHook = NULL;
            g_hHostWnd = NULL;
        }
    }
    catch (...)
    {
    }
    return bSuccess;
}

