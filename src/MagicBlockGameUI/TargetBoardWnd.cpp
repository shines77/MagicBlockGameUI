
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
	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR,
        ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR,
        ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

    return 0;
}

void TargetBoardWnd::OnClose()
{
    DestroyWindow();
}

void TargetBoardWnd::OnDestroy()
{
}

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
