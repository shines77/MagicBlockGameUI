
#include "stdafx.h"
#include "resource.h"

#include "PlayerBoardWnd.h"

PlayerBoardWnd::PlayerBoardWnd()
{
}

PlayerBoardWnd::~PlayerBoardWnd()
{
}

void PlayerBoardWnd::OnFinalMessage(HWND hWnd)
{
    CWindowImpl::OnFinalMessage(hWnd);
}

int PlayerBoardWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void PlayerBoardWnd::OnClose()
{
    DestroyWindow();
}

void PlayerBoardWnd::OnDestroy()
{
}

void PlayerBoardWnd::DoPaint(CDCHandle dc)
{
    dc.SaveDC();
    dc.TextOut(0, 0, _T("Hello world"), 11);
    dc.RestoreDC(-1);
}

void PlayerBoardWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    //
}

void PlayerBoardWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    //
}

void PlayerBoardWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    //
}
