
#include "stdafx.h"
#include "resource.h"

#include "TargetBoardWnd.h"

TargetBoardWnd::TargetBoardWnd()
{
}

TargetBoardWnd::~TargetBoardWnd()
{
}

void TargetBoardWnd::OnFinalMessage(HWND hWnd)
{
    CWindowImpl::OnFinalMessage(hWnd);
    ::PostQuitMessage(0);
}

LRESULT TargetBoardWnd::OnCreate(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
    return 0;
}

LRESULT TargetBoardWnd::OnDestroy(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
    PostQuitMessage(0);
    return 0;
}

LRESULT TargetBoardWnd::OnPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
    PAINTSTRUCT ps;
    HDC hDC = GetDC();
    BeginPaint(&ps);
    TextOut(hDC, 0, 0, _T("Hello world"), 11);
    EndPaint(&ps);
    return 0;
}
