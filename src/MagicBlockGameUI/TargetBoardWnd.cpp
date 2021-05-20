
#include "stdafx.h"
#include "resource.h"

#include <string.h>

#include "TargetBoardWnd.h"
#include "Move.h"

static const LONG nBoardBgTop = 20;
static const LONG nBoardBgBottom = 20;

static const LONG nBtnRandomGenWidth = 60;
static const LONG nBtnUserCustomizeWidth = 70;
static const LONG nBtnImportStringWidth = 100;
static const LONG nBtnHeight = 30;
static const LONG nEditHeight = 26;

static const LONG nBtnLeftEdgeX  = 0;
static const LONG nBtnIntervalX  = 10;
static const LONG nBtnIntervalY  = 13;
static const LONG nBtnRightEdgeX = 0;

static const LONG nBtnTotalWidth = nBtnLeftEdgeX +
                                   nBtnRandomGenWidth + nBtnUserCustomizeWidth +
                                   nBtnImportStringWidth + nBtnIntervalX * 2 +
                                   nBtnRightEdgeX;

static const UINT nGridColorsOffsetX = 22;
static const UINT nGridColorsOffsetY = 23;

static const UINT nGridColorsLeft = 2;
static const UINT nGridColorsTop = 2;
static const UINT nGridColorsInterval = 2;

static const UINT nGridColorsWidth = 71;
static const UINT nGridColorsHeight = 71;

TargetBoardWnd::TargetBoardWnd(SharedData<BoardX, BoardY, TargetX, TargetY> * pData)
    : m_pData(pData), m_hBrushBG(NULL), m_dwLastBringTick(0)
{
    m_hBrushBG = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));

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

#if 1
    for (UINT y = 0; y < TargetY; y++) {
        for (UINT x = 0; x < TargetX; x++) {
            UINT pos = y * TargetY + x;
            if (pos < (TargetX * TargetY)) {
                UINT grid = Color::strToColor(DefaultTargetBoard[pos]);
                m_pData->targetBoard.setGrid(x, y, grid);
            }
        }
    }
#else
    for (UINT y = 0; y < TargetY; y++) {
        for (UINT x = 0; x < TargetX; x++) {
            UINT grid = (rand() % 6) + 1;
            m_pData->targetBoard.setGrid(x, y, grid);
        }
    }
#endif
}

TargetBoardWnd::~TargetBoardWnd()
{
    if (m_hBrushBG) {
        ::DeleteObject(m_hBrushBG);
        m_hBrushBG = NULL;
    }

    m_bmpGridColors.DeleteObject();
    m_bmpBoardBg.DeleteObject();

    m_btnFont.DeleteObject();
    m_editFont.DeleteObject();

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

    ShowWindow(SW_SHOWNORMAL);

    return 0;
}

void TargetBoardWnd::OnClose()
{
    ShowWindow(SW_HIDE);
}

void TargetBoardWnd::OnDestroy()
{
}

void TargetBoardWnd::GetBoardBgPoint(const CRect & rect, CPoint & ptBoardBg)
{
    ptBoardBg.x = (rect.Width() - m_szBoardBg.cx) / 2;
    ptBoardBg.y = CSkinWndImpl<TargetBoardWnd>::GetTitleHeight() + nBoardBgTop;
    if (ptBoardBg.x < 0)
        ptBoardBg.x = 0;
    if (ptBoardBg.y < 0)
        ptBoardBg.y = 0;
}

void TargetBoardWnd::GetBoardBgRect(const CRect & rect, CRect & rcBoardBg)
{
    CPoint ptBoardBg;
    this->GetBoardBgPoint(rect, ptBoardBg);

    rcBoardBg.left   = ptBoardBg.x;
    rcBoardBg.right  = ptBoardBg.x + m_szBoardBg.cx;
    rcBoardBg.top    = ptBoardBg.y;
    rcBoardBg.bottom = ptBoardBg.y + m_szBoardBg.cy;
}

void TargetBoardWnd::GetBoardBgRect(const CRect & rect, CPoint & ptBoardBg, CRect & rcBoardBg)
{
    this->GetBoardBgPoint(rect, ptBoardBg);

    rcBoardBg.left   = ptBoardBg.x;
    rcBoardBg.right  = ptBoardBg.x + m_szBoardBg.cx;
    rcBoardBg.top    = ptBoardBg.y;
    rcBoardBg.bottom = ptBoardBg.y + m_szBoardBg.cy;
}

void TargetBoardWnd::GetBoardGridPoint(const CRect & rect, CPoint & ptBoardGrid)
{
    ptBoardGrid.x = (rect.Width() - m_szBoardBg.cx) / 2 + nGridColorsOffsetX;
    ptBoardGrid.y = CSkinWndImpl<TargetBoardWnd>::GetTitleHeight() + nBoardBgTop + nGridColorsOffsetY;
    if (ptBoardGrid.x < 0)
        ptBoardGrid.x = 0;
    if (ptBoardGrid.y < 0)
        ptBoardGrid.y = 0;
}

void TargetBoardWnd::GetBoardGridRect(const CRect & rect, CPoint & ptBoardGrid, CRect & rcBoardGrid)
{
    this->GetBoardGridPoint(rect, ptBoardGrid);

    rcBoardGrid.left   = ptBoardGrid.x;
    rcBoardGrid.right  = ptBoardGrid.x + m_szBoardBg.cx - nGridColorsOffsetX;
    rcBoardGrid.top    = ptBoardGrid.y;
    rcBoardGrid.bottom = ptBoardGrid.y + m_szBoardBg.cy - nGridColorsOffsetY;
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
    GetBoardBgPoint(rcWin, ptBoardBg);

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
#if 0
    DWORD dwTickCount = GetTickCount();
    if ((dwTickCount - m_dwLastBringTick) > 333) {
        this->BringWindowToTop();
        m_dwLastBringTick = GetTickCount();
    }
#endif
}

void TargetBoardWnd::DoPaint(CDCHandle dc)
{
    CRect rcWin;
    GetClientRect(&rcWin);

    CPoint ptBoardBg;
    GetBoardBgPoint(rcWin, ptBoardBg);

    dc.SaveDC();

#if 0
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
#endif

    CSkinWndImpl<TargetBoardWnd>::SkinWnd_DrawBackgroud(dc, m_dcMem, rcWin);
   
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

    CSkinWndImpl<TargetBoardWnd>::SkinWnd_DrawFrame(dc, m_dcMem, rcWin);

    dc.RestoreDC(-1);
}

void TargetBoardWnd::PaintBoardGrid(CDCHandle & dc, CDC & dcMem, CPoint & ptBoardBg,
                                    UINT x, UINT y, UINT grid)
{
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

HitInfo TargetBoardWnd::OnHitTest(const CRect & rect, const CPoint & point)
{
    HitInfo hitInfo;
    CPoint ptBoardGrid;
    CRect rcBoardGrid;
    GetBoardGridRect(rect, ptBoardGrid, rcBoardGrid);
    if (::PtInRect(&rcBoardGrid, point)) {
        hitInfo.area = HitArea::Board;
        int x = (point.x - ptBoardGrid.x) / nGridColorsWidth;
        int y = (point.y - ptBoardGrid.y) / nGridColorsHeight;
        int index;
        if (x >= 0 && y >= 0)
            index = y * TargetY + x;
        else
            index = -1;
        hitInfo.index = (WORD)index;
    }
    else {
        hitInfo.area = HitArea::Client;
        hitInfo.index = 0;
    }
    return hitInfo;
}

void TargetBoardWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    CRect rect;
    ::GetClientRect(this->m_hWnd, &rect);
    HitInfo hitInfo = this->OnHitTest(rect, point);
    WORD index;
    switch (hitInfo.area) {
        case HitArea::Caption:
            break;
        case HitArea::CloseBtn:
            ShowWindow(SW_HIDE);
            break;
        case HitArea::Client:
            break;
        case HitArea::Board:
            index = hitInfo.index;
            if (index != (WORD)-1) {
                // Move block
                this->MoveBoardBlock(rect, index, TRUE);
            }
            break;
        default:
            break;
    }
}

void TargetBoardWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    //
}

void TargetBoardWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    //
}

BOOL TargetBoardWnd::MoveBoardBlock(const CRect & rect, int index, BOOL bRepaint /* = TRUE */)
{
    if (index >= 0 && index < TargetX * TargetY) {
        int x1 = index % TargetY;
        int y1 = index / TargetY;
        std::lock_guard<std::mutex> lock(this->m_pData->uiMutex);
        for (int dir = 0; dir < MAX_DIR; dir++) {
            int x2 = x1 + Dir_Offset[dir].x;
            if (x2 < 0 || x2 >= TargetX)
                continue;
            int y2 = y1 + Dir_Offset[dir].y;
            if (y2 < 0 || y2 >= TargetY)
                continue;
            int move_to = y2 * TargetY + x2;
            if (this->m_pData->targetBoard.grids[move_to] == Color::Empty) {
                std::swap(this->m_pData->targetBoard.grids[index],
                          this->m_pData->targetBoard.grids[move_to]);
                lock.~lock_guard();
                InvalidateRect(rect, bRepaint);
                break;
            }
        }
    }
    return TRUE;
}
