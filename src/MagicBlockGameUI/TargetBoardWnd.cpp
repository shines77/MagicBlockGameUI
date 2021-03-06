
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
}

int TargetBoardWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    return 0;
}

void TargetBoardWnd::OnClose()
{
    DestroyWindow();
}

void TargetBoardWnd::OnDestroy()
{
}

/*
LRESULT TargetBoardWnd::OnPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
    PAINTSTRUCT ps;
    HDC hDC = GetDC();
    BeginPaint(&ps);
    TextOut(hDC, 0, 0, _T("Hello world"), 11);
    EndPaint(&ps);
    return 0;
}
//*/

void TargetBoardWnd::DoPaint(CDCHandle dc)
{
    dc.SaveDC();
    dc.TextOut(0, 0, _T("Hello world"), 11);
    dc.RestoreDC(-1);
}

void TargetBoardWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    //
}

void TargetBoardWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    //
}

void TargetBoardWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    //
}
