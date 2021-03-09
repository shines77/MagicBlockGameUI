
#include "stdafx.h"
#include "resource.h"

#include "TargetBoardWnd.h"

TargetBoardWnd::TargetBoardWnd()
    : m_hBrushBG(NULL), m_dwLastBringTick(0)
{
    m_bmpBoardBg.LoadBitmap(IDB_BITMAP_BOARD_BG_3x3);
    m_bmpGridColors.LoadBitmap(IDB_BITMAP_GRID_COLORS);

    int success = m_bmpBoardBg.GetSize(m_szBoardBg);
    if (success == 0) {
        m_szBoardBg.cx = 0;
        m_szBoardBg.cy = 0;
    }

    success = m_bmpGridColors.GetSize(m_szGridColors);
    if (success == 0) {
        m_szGridColors.cx = 0;
        m_szGridColors.cy = 0;
    }

    for (UINT y = 0; y < BoardY; y++) {
        for (UINT x = 0; x < BoardX; x++) {
            UINT grid = (rand() % 6) + 1;
            m_board.setGrid(x, y, grid);
        }
    }
}

TargetBoardWnd::~TargetBoardWnd()
{
    if (m_hBrushBG) {
        ::DeleteObject(m_hBrushBG);
        m_hBrushBG = NULL;
    }

    m_bmpGridColors.DeleteObject();
    m_bmpBoardBg.DeleteObject();

    m_dcMem.DeleteDC();
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

    if (m_hBrushBG == NULL) {
        m_hBrushBG = CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
    }

    return 0;
}

void TargetBoardWnd::OnClose()
{
    DestroyWindow();
}

void TargetBoardWnd::OnDestroy()
{
}

void TargetBoardWnd::OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther)
{
    if ((nState == WA_ACTIVE && !bMinimized) || (nState == WA_CLICKACTIVE)) {
        DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
        ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        if (!((dwExStyle & WS_EX_TOPMOST) == WS_EX_TOPMOST))
        {
            ::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        }
        this->BringWindowToTop();
        SetFocus();
    }
    else if (nState == WA_INACTIVE) {
        ::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }
}

void TargetBoardWnd::OnMove(CPoint ptPos)
{
    DWORD dwTickCount = GetTickCount();
    if ((dwTickCount - m_dwLastBringTick) > 300) {
        this->BringWindowToTop();
        m_dwLastBringTick = GetTickCount();
    }
}

LRESULT TargetBoardWnd::OnEraseBackground2(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	this->SetMsgHandled(TRUE);
	LRESULT lResult = (LRESULT)OnEraseBkgnd((HDC)wParam);
	if (this->IsMsgHandled())
		return TRUE;
    return lResult;
}

BOOL TargetBoardWnd::OnEraseBkgnd(CDCHandle dc)
{
    CRect rcWin;
    GetClientRect(&rcWin);

    if (m_hBrushBG != NULL) {
        dc.FillRect(&rcWin, m_hBrushBG);
    }
    return FALSE;
}

void TargetBoardWnd::DoPaint(CDCHandle dc)
{
    CRect rcWin;
    GetClientRect(&rcWin);

    CPoint ptBoardBg;
    ptBoardBg.x = (rcWin.Width() - m_szBoardBg.cx) / 2;
    ptBoardBg.y = (rcWin.Height() - m_szBoardBg.cy) / 2;
    if (ptBoardBg.x < 0)
        ptBoardBg.x = 0;
    if (ptBoardBg.y < 0)
        ptBoardBg.y = 0;

    dc.SaveDC();

    if (m_hBrushBG != NULL) {
        // Top
        if (ptBoardBg.y > 1) {
            CRect rcTop(rcWin.left, rcWin.top, rcWin.right, rcWin.top + ptBoardBg.y);
            dc.FillRect(&rcTop, m_hBrushBG);
        }
        // Left
        if (ptBoardBg.x > 1) {
            CRect rcLeft(rcWin.left, ptBoardBg.y, rcWin.left + ptBoardBg.x, ptBoardBg.y + m_szBoardBg.cy);
            dc.FillRect(&rcLeft, m_hBrushBG);
        }
        // Right
        LONG right_x = rcWin.Width() - m_szBoardBg.cx - ptBoardBg.x;
        if (right_x > 1) {
            CRect rcRight(rcWin.right - right_x, ptBoardBg.y, rcWin.right, ptBoardBg.y + m_szBoardBg.cy);
            dc.FillRect(&rcRight, m_hBrushBG);
        }
        // Bottom
        LONG bottom_y = rcWin.Height() - m_szBoardBg.cy - ptBoardBg.y;
        if (bottom_y > 1) {
            CRect rcBottom(rcWin.left, ptBoardBg.y + m_szBoardBg.cy, rcWin.right, rcWin.bottom);
            dc.FillRect(&rcBottom, m_hBrushBG);
        }
    }
   
    if (m_dcMem.m_hDC == NULL) {
        m_dcMem.CreateCompatibleDC(dc.m_hDC);
    }
   
    if (m_bmpBoardBg.m_hBitmap) {
        if (m_dcMem.m_hDC != NULL) {
            HBITMAP hBitmapOld = m_dcMem.SelectBitmap(m_bmpBoardBg.m_hBitmap);
            if (hBitmapOld != NULL) {
                BOOL paintOK = dc.BitBlt(ptBoardBg.x, ptBoardBg.y,
                                         ptBoardBg.x + m_szBoardBg.cx,
                                         ptBoardBg.y + m_szBoardBg.cy,
                                         m_dcMem.m_hDC, 0, 0, SRCCOPY);
                m_dcMem.SelectBitmap(hBitmapOld);
                hBitmapOld = NULL;
            }
        }
    }

    if (m_bmpGridColors.m_hBitmap) {
        if (m_dcMem.m_hDC != NULL) {
            HBITMAP hBitmapOld = m_dcMem.SelectBitmap(m_bmpGridColors.m_hBitmap);
            if (hBitmapOld != NULL) {
                for (UINT y = 0; y < BoardY; y++) {
                    for (UINT x = 0; x < BoardX; x++) {
                        UINT grid = m_board.getGrid(x, y);
                        if (grid >= 0 && grid < 7) {
                            PaintBoardGrid(dc, m_dcMem, ptBoardBg, x, y, grid);
                        }
                    }
                }
                m_dcMem.SelectBitmap(hBitmapOld);
                hBitmapOld = NULL;
            }
        }
    }

    dc.RestoreDC(-1);
}

void TargetBoardWnd::PaintBoardGrid(CDCHandle & dc, CDC & dcMem, CPoint & ptBoardBg,
                                    UINT x, UINT y, UINT grid)
{
    static const UINT nGridColorsOffsetX = 22;
    static const UINT nGridColorsOffsetY = 23;

    static const UINT nGridColorsLeft = 2;
    static const UINT nGridColorsTop = 2;
    static const UINT nGridColorsInterval = 2;

    static const UINT nGridColorsWidth = 71;
    static const UINT nGridColorsHeight = 71;

    ATLASSERT(grid >= 0 && grid < 7);
    if (grid != 0) {
        BOOL paintOK = dc.BitBlt(ptBoardBg.x + nGridColorsOffsetX + x * nGridColorsWidth,
                                 ptBoardBg.y + nGridColorsOffsetY + y * nGridColorsHeight,
                                 nGridColorsWidth + 1,
                                 nGridColorsHeight,
                                 dcMem.m_hDC,
                                 nGridColorsLeft + (grid - 1) * (nGridColorsWidth + nGridColorsInterval),
                                 nGridColorsTop,
                                 SRCCOPY);
    }
    else {
        // Paint empty grid
    }
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
