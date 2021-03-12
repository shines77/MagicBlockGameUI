
#include "stdafx.h"
#include "resource.h"

#include "PlayerBoardWnd.h"

PlayerBoardWnd::PlayerBoardWnd(SharedData<BoardX, BoardY, TargetX, TargetY> * pData)
    : m_pData(pData), m_hBrushBG(NULL), m_dwLastBringTick(0)
{
    m_bmpBoardBg.LoadBitmap(IDB_BITMAP_BOARD_BG_5x5);
    m_bmpGridColors.LoadBitmap(IDB_BITMAP_GRID_COLORS);
    m_bmpScale9PSprite.LoadBitmap(IDB_BITMAP_BTN_SCALE9PSPRITE_GRAY);

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

    // 0x00686868, 0x00767676
    m_scale9PSprite.SetSprite(m_bmpScale9PSprite.m_hBitmap, rcScale9PSprite, kDefaultBGColor);

    for (UINT y = 0; y < BoardY; y++) {
        for (UINT x = 0; x < BoardX; x++) {
            UINT pos = y * BoardY + x;
            if (pos < (BoardY * BoardY)) {
                UINT grid = Color::strToColor(DefaultPlayerBoard[pos]);
                m_pData->playerBoard.setGrid(x, y, grid);
            }
        }
    }

#if 0
    for (UINT y = 0; y < BoardY; y++) {
        for (UINT x = 0; x < BoardX; x++) {
            UINT grid = (rand() % 6) + 1;
            m_pData->playerBoard.setGrid(x, y, grid);
        }
    }
#endif
}

PlayerBoardWnd::~PlayerBoardWnd()
{
    if (m_hBrushBG) {
        ::DeleteObject(m_hBrushBG);
        m_hBrushBG = NULL;
    }

    m_bmpGridColors.DeleteObject();
    m_bmpBoardBg.DeleteObject();
    m_bmpScale9PSprite.DeleteObject();

    m_scale9PSprite.Destroy();

    m_dcMem.DeleteDC();
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

    if (m_hBrushBG == NULL) {
        m_hBrushBG = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
    }

    ShowWindow(SW_SHOWNORMAL);

    return 0;
}

void PlayerBoardWnd::OnClose()
{
    //DestroyWindow();
    ShowWindow(SW_HIDE);
}

void PlayerBoardWnd::OnDestroy()
{
}

void PlayerBoardWnd::OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther)
{
    if ((nState == WA_ACTIVE && !bMinimized) || (nState == WA_CLICKACTIVE)) {
        //DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
        //::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        //if (!((dwExStyle & WS_EX_TOPMOST) == WS_EX_TOPMOST))
        //{
        //    ::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        //}
        this->BringWindowToTop();
        SetFocus();
    }
    else if (nState == WA_INACTIVE) {
        //::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }
}

void PlayerBoardWnd::OnMove(CPoint ptPos)
{
    DWORD dwTickCount = GetTickCount();
    if ((dwTickCount - m_dwLastBringTick) > 333) {
        this->BringWindowToTop();
        m_dwLastBringTick = GetTickCount();
    }
}

LRESULT PlayerBoardWnd::OnEraseBackground2(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	this->SetMsgHandled(TRUE);
	LRESULT lResult = (LRESULT)OnEraseBkgnd((HDC)wParam);
	if (this->IsMsgHandled())
		return TRUE;
    return lResult;
}

BOOL PlayerBoardWnd::OnEraseBkgnd(CDCHandle dc)
{
    CRect rcWin;
    GetClientRect(&rcWin);

    if (m_hBrushBG != NULL) {
        dc.FillRect(&rcWin, m_hBrushBG);
    }
    return FALSE;
}

void PlayerBoardWnd::DoPaint(CDCHandle dc)
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

    m_scale9PSprite.DrawBackgroud(dc, rcWin);

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
                        UINT grid = m_pData->playerBoard.getGrid(x, y);
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

    m_scale9PSprite.DrawFrame(dc, m_dcMem, rcWin);

    dc.RestoreDC(-1);
}

void PlayerBoardWnd::PaintBoardGrid(CDCHandle & dc, CDC & dcMem, CPoint & ptBoardBg,
                                    UINT x, UINT y, UINT grid)
{
    static const UINT nGridColorsOffsetX = 25;
    static const UINT nGridColorsOffsetY = 27;

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
