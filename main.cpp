// License: MIT
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "TaskbarProgress.h"

HWND g_hMainWnd = NULL;
HINSTANCE g_hInst = NULL;
TaskbarProgress *g_pTaskbarProgress = NULL;

#define CLASSNAME TEXT("TaskbarProgress")

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    g_pTaskbarProgress = new TaskbarProgress(hwnd);
    g_pTaskbarProgress->Set(0);

    SetTimer(hwnd, 999, 50, NULL);
    return TRUE;
}

void OnTimer(HWND hwnd, UINT id)
{
    if (id == 999)
    {
        static INT s_counter = 0;
        if (s_counter < 100)
        {
            if (g_pTaskbarProgress)
            {
                g_pTaskbarProgress->Set(-1);
                //g_pTaskbarProgress->Set(s_counter);
            }
            ++s_counter;
        }
        else
        {
            KillTimer(hwnd, id);
            if (g_pTaskbarProgress)
            {
                g_pTaskbarProgress->Set(100);
                //g_pTaskbarProgress->Clear();
            }
        }
    }
}

void OnDestroy(HWND hwnd)
{
    if (g_pTaskbarProgress)
    {
        delete g_pTaskbarProgress;
        g_pTaskbarProgress = NULL;
    }

    PostQuitMessage(0);
}

LRESULT CALLBACK
WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hwnd, WM_TIMER, OnTimer);
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

BOOL InitInstance(HINSTANCE hInst, INT nCmdShow)
{
    InitCommonControls();

    WNDCLASS wc = { CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
    wc.lpszClassName = CLASSNAME;
    if (!RegisterClass(&wc))
    {
        MessageBoxA(NULL, "RegisterClass failed.", NULL, MB_ICONERROR);
        return FALSE;
    }

    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD exstyle = 0;
    HWND hwnd = CreateWindowEx(exstyle, CLASSNAME, CLASSNAME, style,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL, NULL, hInst, NULL);
    if (!hwnd)
    {
        MessageBoxA(NULL, "CreateWindowEx failed.", NULL, MB_ICONERROR);
        return FALSE;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return TRUE;
}

INT Run()
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (INT)msg.wParam;
}

INT WINAPI
WinMain(
    HINSTANCE   hInstance,
    HINSTANCE   hPrevInstance,
    LPSTR       lpCmdLine,
    INT         nCmdShow)
{
    g_hInst = hInstance;

    if (!InitInstance(hInstance, nCmdShow))
        return 1;

    return Run();
}
