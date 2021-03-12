
#include "stdafx.h"
#include "resource.h"

#include <string.h>

#include "TargetBoardWnd.h"

static const LONG nBoardBgTop = 16;
static const LONG nBoardBgBottom = 20;

static const LONG nBtnRandomGenWidth = 60;
static const LONG nBtnUserCustomizeWidth = 70;
static const LONG nBtnImportStringWidth = 100;
static const LONG nBtnHeight = 30;
static const LONG nEditHeight = 26;

static const LONG nBtnIntervalX = 10;
static const LONG nBtnIntervalY = 10;

static const LONG nBtnTotalWidth = nBtnRandomGenWidth + nBtnUserCustomizeWidth +
                                   nBtnImportStringWidth + nBtnIntervalX * 2;

TargetBoardWnd::TargetBoardWnd(SharedData<BoardX, BoardY, TargetX, TargetY> * pData)
    : m_pData(pData), m_hBrushBG(NULL), m_dwLastBringTick(0)
{
    m_bmpBoardBg.LoadBitmap(IDB_BITMAP_BOARD_BG_3x3);
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

    for (UINT y = 0; y < TargetY; y++) {
        for (UINT x = 0; x < TargetX; x++) {
            UINT grid = (rand() % 6) + 1;
            m_pData->targetBoard.setGrid(x, y, grid);
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
    m_bmpScale9PSprite.DeleteObject();

    m_btnFont.DeleteObject();
    m_editFont.DeleteObject();

    m_scale9PSprite.Destroy();

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
        m_hBrushBG = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
    }

    ShowWindow(SW_SHOWNORMAL);

    return 0;
}

void TargetBoardWnd::OnClose()
{
    //DestroyWindow();
    ShowWindow(SW_HIDE);
}

void TargetBoardWnd::OnDestroy()
{
}

void TargetBoardWnd::OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther)
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

void TargetBoardWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CRect rcWin;
    GetClientRect(&rcWin);

    CPoint ptBoardBg;
    ptBoardBg.x = (rcWin.Width() - m_szBoardBg.cx) / 2;
    ptBoardBg.y = nBoardBgTop;
    if (ptBoardBg.x < 0)
        ptBoardBg.x = 0;
    if (ptBoardBg.y < 0)
        ptBoardBg.y = 0;

    if (m_btnFont.m_hFont == NULL) {
        int nFontSize = 9;
        int PixelsY = ::GetDeviceCaps(this->GetDC(), LOGPIXELSY);
        int nPointSize = int((nFontSize / 96.0) * PixelsY * 10);
        m_btnFont.CreatePointFont(nPointSize, _T("宋体"), this->GetDC());
    }
    if (m_editFont.m_hFont == NULL) {
        int nFontSize = 12;
        int PixelsY = ::GetDeviceCaps(this->GetDC(), LOGPIXELSY);
        int nPointSize = int((nFontSize / 96.0) * PixelsY * 10);
        m_editFont.CreatePointFont(nPointSize, _T("宋体"), this->GetDC());
    }

    DWORD dwBtnStyle = WS_CHILD | WS_GROUP | WS_TABSTOP;
    DWORD dwBtnExStyle = 0;

    if (m_btnRandomGen.m_hWnd == NULL) {
        CRect rcBtn(0, 0, nBtnRandomGenWidth, nBtnHeight);
        m_btnRandomGen.Create(m_hWnd, &rcBtn, _T("随机"), dwBtnStyle, dwBtnExStyle);
        m_btnRandomGen.SetFont(m_btnFont.m_hFont);
    }
    if (m_btnUserCustomize.m_hWnd == NULL) {
        CRect rcBtn(0, 0, nBtnUserCustomizeWidth, nBtnHeight);
        m_btnUserCustomize.Create(m_hWnd, &rcBtn, _T("自定义"), dwBtnStyle, dwBtnExStyle);
        m_btnUserCustomize.SetFont(m_btnFont.m_hFont);
    }
    if (m_btnImportString.m_hWnd == NULL) {
        CRect rcBtn(0, 0, nBtnImportStringWidth, nBtnHeight);
        m_btnImportString.Create(m_hWnd, &rcBtn, _T("从字符串导入"), dwBtnStyle, dwBtnExStyle);
        m_btnImportString.SetFont(m_btnFont.m_hFont);
    }

    DWORD dwEditStyle = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    DWORD dwEditExStyle = 0;

    if (m_editTargetString.m_hWnd == NULL) {
        CRect rcBtn(0, 0, nBtnTotalWidth, nEditHeight);
        m_editTargetString.Create(m_hWnd, &rcBtn, _T(""), dwEditStyle, dwEditExStyle);
        m_editTargetString.SetFont(m_editFont.m_hFont);
    }

    CPoint ptButtonRect;
    ptButtonRect.x = (rcWin.Width() - nBtnTotalWidth) / 2;
    ptButtonRect.y = ptBoardBg.y + m_szBoardBg.cy + nBoardBgBottom;

    CPoint ptMove;
    CRect rcBtn1;
    if (m_btnRandomGen.m_hWnd != NULL) {
        rcBtn1.SetRect(0, 0, nBtnRandomGenWidth, nBtnHeight);
        ptMove.x = ptButtonRect.x;
        ptMove.y = ptButtonRect.y;
        rcBtn1.OffsetRect(ptMove);
        m_btnRandomGen.MoveWindow(&rcBtn1, FALSE);
        if (!m_btnRandomGen.IsWindowVisible())
            m_btnRandomGen.ShowWindow(SW_SHOWNORMAL);
    }

    CRect rcBtn2;
    if (m_btnUserCustomize.m_hWnd != NULL) {
        rcBtn2.SetRect(0, 0, nBtnUserCustomizeWidth, nBtnHeight);
        ptMove.x = rcBtn1.right + nBtnIntervalX;
        ptMove.y = ptButtonRect.y;
        rcBtn2.OffsetRect(ptMove);
        m_btnUserCustomize.MoveWindow(&rcBtn2, FALSE);
        if (!m_btnUserCustomize.IsWindowVisible())
            m_btnUserCustomize.ShowWindow(SW_SHOWNORMAL);
    }

    CRect rcBtn3;
    if (m_btnImportString.m_hWnd != NULL) {
        rcBtn3.SetRect(0, 0, nBtnImportStringWidth, nBtnHeight);
        ptMove.x = rcBtn2.right + nBtnIntervalX;
        ptMove.y = ptButtonRect.y;
        rcBtn3.OffsetRect(ptMove);
        m_btnImportString.MoveWindow(&rcBtn3, FALSE);
        if (!m_btnImportString.IsWindowVisible())
            m_btnImportString.ShowWindow(SW_SHOWNORMAL);
    }

    CRect rcEdit;
    if (m_editTargetString.m_hWnd != NULL) {
        m_editTargetString.SetParent(this->m_hWnd);
        rcEdit.SetRect(0, 0, nBtnTotalWidth, nEditHeight);
        ptMove.x = ptButtonRect.x;
        ptMove.y = ptButtonRect.y + nBtnHeight + nBtnIntervalY;
        rcEdit.OffsetRect(ptMove);
        m_editTargetString.MoveWindow(&rcEdit, FALSE);
        if (!m_editTargetString.IsWindowVisible()) {
            m_editTargetString.SetWindowText(_T("RGBRGBRGB"));
            m_editTargetString.EnableWindow(TRUE);
            m_editTargetString.ShowWindow(SW_SHOWNORMAL);
            m_editTargetString.SetFocus();
        }
    }
}

void TargetBoardWnd::OnMove(CPoint ptPos)
{
    DWORD dwTickCount = GetTickCount();
    if ((dwTickCount - m_dwLastBringTick) > 333) {
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
#if 0
    CRect rcWin;
    GetClientRect(&rcWin);

    if (m_hBrushBG != NULL) {
        dc.FillRect(&rcWin, m_hBrushBG);
    }
#endif
    return FALSE;
}

void TargetBoardWnd::DoPaint(CDCHandle dc)
{
    CRect rcWin;
    GetClientRect(&rcWin);

    CPoint ptBoardBg;
    ptBoardBg.x = (rcWin.Width() - m_szBoardBg.cx) / 2;
    //ptBoardBg.y = (rcWin.Height() - m_szBoardBg.cy) / 2;
    ptBoardBg.y = 20;
    if (ptBoardBg.x < 0)
        ptBoardBg.x = 0;
    if (ptBoardBg.y < 0)
        ptBoardBg.y = 0;

    dc.SaveDC();

    /*
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
    //*/

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
                for (UINT y = 0; y < TargetY; y++) {
                    for (UINT x = 0; x < TargetX; x++) {
                        UINT grid = m_pData->targetBoard.getGrid(x, y);
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
