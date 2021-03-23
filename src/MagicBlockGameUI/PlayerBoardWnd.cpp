
#include "stdafx.h"
#include "resource.h"

#include <mutex>

#include "PlayerBoardWnd.h"
#include "Move.h"

#include "GdiPlusUtils.h"

///////////////////////////////////////////////////

static const LONG nBoardBgTop = 24;
static const LONG nBoardBgBottom = 24;

static const LONG nBtnRandomGenWidth = 50;
static const LONG nBtnUserCustomizeWidth = 60;
static const LONG nBtnImportStringWidth = 100;
static const LONG nBtnHeight = 30;
static const LONG nEditHeight = 26;

static const LONG nBtnLeftEdgeX  = 4;
static const LONG nBtnIntervalX  = 10;
static const LONG nBtnIntervalY  = 13;
static const LONG nBtnRightEdgeX = 4;

static const LONG nBtnTotalWidth = nBtnLeftEdgeX +
                                   nBtnRandomGenWidth + nBtnUserCustomizeWidth +
                                   nBtnImportStringWidth + nBtnIntervalX * 2 +
                                   nBtnRightEdgeX;

///////////////////////////////////////////////////

static const UINT nGridColorsOffsetX = 25;
static const UINT nGridColorsOffsetY = 27;

static const UINT nGridColorsLeft = 2;
static const UINT nGridColorsTop = 2;
static const UINT nGridColorsInterval = 2;

static const UINT nGridColorsWidth = 71;
static const UINT nGridColorsHeight = 71;

///////////////////////////////////////////////////

static const UINT nArrowsOffsetX = 25;
static const UINT nArrowsOffsetY = 27;

static const UINT nArrowsLeft = 0;
static const UINT nArrowsTop = 0;
static const UINT nArrowsInterval = 0;

static const UINT nArrowsWidth = 72;
static const UINT nArrowsHeight = 72;

///////////////////////////////////////////////////

PlayerBoardWnd::PlayerBoardWnd(SharedData<BoardX, BoardY, TargetX, TargetY> * pData)
    : m_pData(pData), m_pImgArrows(NULL), m_hBrushBG(NULL), m_dwLastBringTick(0)
{
    m_hBrushBG = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));

    m_bmpBoardBg.LoadBitmap(IDB_BITMAP_BOARD_BG_5x5);
    m_bmpGridColors.LoadBitmap(IDB_BITMAP_GRID_COLORS);
    m_pImgArrows = gdiplus::LoadFromResource(IDB_PNG_ARROWS, _T("PNG"));
    if (m_pImgArrows == NULL) {
        if (gdiplus::GetGdiPlusLastError() < 0) {
            //
        }
    }

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
            UINT pos = y * BoardY + x;
            if (pos < (BoardX * BoardY)) {
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
    if (m_pImgArrows != NULL) {
        gdiplus::DestroyImage(m_pImgArrows);
        m_pImgArrows = NULL;
    }

    m_btnFont.DeleteObject();
    m_editFont.DeleteObject();

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

    m_cmdMgr.SetOwner(this->m_hWnd);
    m_cmdMgr.SetInitialBoard();

    ShowWindow(SW_SHOWNORMAL);

    return 0;
}

void PlayerBoardWnd::OnClose()
{
    ShowWindow(SW_HIDE);
}

void PlayerBoardWnd::OnDestroy()
{
}

void PlayerBoardWnd::GetBoardBgPoint(const CRect & rect, CPoint & ptBoardBg)
{
    ptBoardBg.x = (rect.Width() - m_szBoardBg.cx) / 2;
    ptBoardBg.y = CSkinWndImpl<PlayerBoardWnd>::GetTitleHeight() + nBoardBgTop;
    if (ptBoardBg.x < 0)
        ptBoardBg.x = 0;
    if (ptBoardBg.y < 0)
        ptBoardBg.y = 0;
}

void PlayerBoardWnd::GetBoardBgRect(const CRect & rect, CRect & rcBoardBg)
{
    CPoint ptBoardBg;
    this->GetBoardBgPoint(rect, ptBoardBg);

    rcBoardBg.left   = ptBoardBg.x;
    rcBoardBg.right  = ptBoardBg.x + m_szBoardBg.cx;
    rcBoardBg.top    = ptBoardBg.y;
    rcBoardBg.bottom = ptBoardBg.y + m_szBoardBg.cy;
}

void PlayerBoardWnd::GetBoardBgRect(const CRect & rect, CPoint & ptBoardBg, CRect & rcBoardBg)
{
    this->GetBoardBgPoint(rect, ptBoardBg);

    rcBoardBg.left   = ptBoardBg.x;
    rcBoardBg.right  = ptBoardBg.x + m_szBoardBg.cx;
    rcBoardBg.top    = ptBoardBg.y;
    rcBoardBg.bottom = ptBoardBg.y + m_szBoardBg.cy;
}

void PlayerBoardWnd::GetBoardGridPoint(const CRect & rect, CPoint & ptBoardGrid)
{
    ptBoardGrid.x = (rect.Width() - m_szBoardBg.cx) / 2 + nGridColorsOffsetX;
    ptBoardGrid.y = CSkinWndImpl<PlayerBoardWnd>::GetTitleHeight() + nBoardBgTop + nGridColorsOffsetY;
    if (ptBoardGrid.x < 0)
        ptBoardGrid.x = 0;
    if (ptBoardGrid.y < 0)
        ptBoardGrid.y = 0;
}

void PlayerBoardWnd::GetBoardGridRect(const CRect & rect, CPoint & ptBoardGrid, CRect & rcBoardGrid)
{
    this->GetBoardGridPoint(rect, ptBoardGrid);

    rcBoardGrid.left   = ptBoardGrid.x;
    rcBoardGrid.right  = ptBoardGrid.x + m_szBoardBg.cx - nGridColorsOffsetX;
    rcBoardGrid.top    = ptBoardGrid.y;
    rcBoardGrid.bottom = ptBoardGrid.y + m_szBoardBg.cy - nGridColorsOffsetY;
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

void PlayerBoardWnd::OnShowWindow(BOOL bShow, UINT nStatus)
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
        CRect rcBtn(0, 0, m_szBoardBg.cy - nBtnLeftEdgeX - nBtnRightEdgeX, nEditHeight);
        m_editTargetString.Create(m_hWnd, &rcBtn, _T(""), dwEditStyle, dwEditExStyle);
        m_editTargetString.SetFont(m_editFont.m_hFont);
    }

    CPoint ptButtonRect;
    //ptButtonRect.x = (rcWin.Width() - nBtnTotalWidth) / 2;
    ptButtonRect.x = (rcWin.Width() - m_szBoardBg.cx) / 2;
    ptButtonRect.y = ptBoardBg.y + m_szBoardBg.cy + nBoardBgBottom;

    CPoint ptMove;
    CRect rcBtn1;
    if (m_btnRandomGen.m_hWnd != NULL) {
        rcBtn1.SetRect(0, 0, nBtnRandomGenWidth, nBtnHeight);
        ptMove.x = ptButtonRect.x + nBtnLeftEdgeX;
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
        rcEdit.SetRect(0, 0, m_szBoardBg.cy - nBtnLeftEdgeX - nBtnRightEdgeX, nEditHeight);
        ptMove.x = ptButtonRect.x + nBtnLeftEdgeX;
        ptMove.y = ptButtonRect.y + nBtnIntervalY + nBtnHeight;
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

void PlayerBoardWnd::OnMove(CPoint ptPos)
{
#if 0
    DWORD dwTickCount = GetTickCount();
    if ((dwTickCount - m_dwLastBringTick) > 333) {
        this->BringWindowToTop();
        m_dwLastBringTick = GetTickCount();
    }
#endif
}

void PlayerBoardWnd::DoPaint(CDCHandle dc)
{
    dc.SaveDC();

    CRect rcWin;
    GetClientRect(&rcWin);

    CPoint ptBoardBg;
    GetBoardBgPoint(rcWin, ptBoardBg);

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

    if (m_dcMem.m_hDC == NULL) {
        m_dcMem.CreateCompatibleDC(dc.m_hDC);
    }

    if (m_dcMem.m_hDC != NULL) {
        CSkinWndImpl<PlayerBoardWnd>::SkinWnd_DrawBackgroud(dc, m_dcMem, rcWin);
   
        if (m_bmpBoardBg.m_hBitmap != NULL) {
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

        if (this->m_pData != NULL) {
            if (m_bmpGridColors.m_hBitmap != NULL) {
                HBITMAP hBitmapOld = m_dcMem.SelectBitmap(m_bmpGridColors.m_hBitmap);
                if (hBitmapOld != NULL) {
                    for (UINT y = 0; y < BoardY; y++) {
                        for (UINT x = 0; x < BoardX; x++) {
                            UINT grid = this->m_pData->playerBoard.getGrid(x, y);
                            if (grid >= 0 && grid < 7) {
                                PaintBoardGrid(dc, m_dcMem, ptBoardBg, x, y, grid);
                            }
                        }
                    }
                    m_dcMem.SelectBitmap(hBitmapOld);
                    hBitmapOld = NULL;
                }
            }

            std::vector<MoveInfo> move_path;
            MoveInfo move_arrow[BoardY * BoardX][Direction::Last];
            for (ptrdiff_t i = 0; i < BoardY * BoardX; i++) {
                for (ptrdiff_t j = 0; j < Direction::Last; j++) {
                    move_arrow[i][j].color = 0;
                    move_arrow[i][j].dir = uint8_t(-1);
                }
            }

            {
                std::lock_guard<std::mutex> lock(this->m_pData->uiMutex);
                auto const & cmove_path = this->m_pData->move_path;
                size_t move_cnt = 0;
                for (ptrdiff_t i = cmove_path.size() - 1; i >= 0; i--) {
                    const MoveInfo move_info = cmove_path[i];
                    Position from_pos = move_info.from_pos;
                    Position move_to = move_info.move_to_pos;
                    UINT dir = move_info.dir;
                    if (move_arrow[from_pos.value][dir].dir == uint8_t(-1)) {
                        move_arrow[from_pos.value][dir].from_pos = from_pos;
                        move_arrow[from_pos.value][dir].move_to_pos = move_to;
                        move_arrow[from_pos.value][dir].dir = dir;
                        if (move_arrow[from_pos.value][dir].color == 0) {
                            move_arrow[from_pos.value][dir].color = 1;
                            move_cnt++;
                            if (move_cnt > 20)
                                break;
                        }
                    }
                }
                // Copy move_path[]
                move_path = this->m_pData->move_path;
            }

            if (m_pImgArrows != NULL) {
                size_t draw_cnt = 0;
                for (ptrdiff_t i = move_path.size() - 1; i >= 0; i--) {
                    const MoveInfo move_info = move_path[i];
                    Position from_pos = move_info.from_pos;
                    Position move_to = move_info.move_to_pos;
                    UINT dir = move_info.dir;
                    ATLASSERT(move_arrow[from_pos.value][dir].dir != uint8_t(-1));
                    int opp_dir = Direction::getOppDir(dir);
                    bool is_opposite = (move_arrow[move_to.value][opp_dir].dir != uint8_t(-1));
                    if (dir >= Direction::First && dir < Direction::Last) {
                        UINT x = from_pos.value % BoardY;
                        UINT y = from_pos.value / BoardY;
                        PaintBoardArrow(dc, m_pImgArrows, ptBoardBg, x, y, dir, is_opposite);
                        if (move_arrow[from_pos.value][dir].color == 1) {
                            move_arrow[from_pos.value][dir].color = 0;
                            draw_cnt++;
                            if (draw_cnt > 15)
                                break;
                        }
                    }
                }
            }
        }

        CSkinWndImpl<PlayerBoardWnd>::SkinWnd_DrawFrame(dc, m_dcMem, rcWin);
    }

    dc.RestoreDC(-1);
}

void PlayerBoardWnd::PaintBoardGrid(CDCHandle & dc, CDC & dcMem, CPoint & ptBoardBg,
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

void PlayerBoardWnd::PaintBoardArrow(CDCHandle & dc, Gdiplus::Image * pImage, CPoint & ptBoardBg,
                                     UINT x, UINT y, int arrow, bool is_opposite)
{
    ATLASSERT(pImage != NULL);
    ATLASSERT(arrow >= Direction::First && arrow < Direction::Last);

    Gdiplus::Rect rcDest, rcSrc;
    rcDest.X = ptBoardBg.x + nArrowsOffsetX + x * (nArrowsWidth - 1);
    rcDest.Y = ptBoardBg.y + nArrowsOffsetY + y * (nArrowsHeight - 1);
    rcDest.Width = nArrowsWidth;
    rcDest.Height = nArrowsHeight - 1;

    rcSrc.X = nArrowsLeft + arrow * (nArrowsWidth + nArrowsInterval);
    rcSrc.Y = nArrowsTop;
    rcSrc.Width = nArrowsWidth;
    rcSrc.Height = nArrowsHeight - 1;

    if (!is_opposite) {
        if (arrow == Direction::Down) {
            rcDest.Y += (nArrowsHeight - 1) / 2;
        }
        else if (arrow == Direction::Left) {
            rcDest.X -= (nArrowsWidth - 1) / 2;
        }
        else if (arrow == Direction::Up) {
            rcDest.Y -= (nArrowsHeight - 1) / 2;
        }
        else if (arrow == Direction::Right) {
            rcDest.X += (nArrowsWidth - 1) / 2;
        }
    }
    else {
        if (arrow == Direction::Down) {
            rcDest.X -= (nArrowsWidth - 1) / 4;
            rcDest.Y += (nArrowsHeight - 1) / 2;
        }
        else if (arrow == Direction::Left) {
            rcDest.X -= (nArrowsWidth - 1) / 2;
            rcDest.Y += (nArrowsHeight - 1) / 4;
        }
        else if (arrow == Direction::Up) {
            rcDest.X += (nArrowsWidth - 1) / 4;
            rcDest.Y -= (nArrowsHeight - 1) / 2;
        }
        else if (arrow == Direction::Right) {
            rcDest.X += (nArrowsWidth - 1) / 2;
            rcDest.Y -= (nArrowsHeight - 1) / 4;
        }
    }
    
    Gdiplus::Graphics graphics(dc.m_hDC);
    Gdiplus::Status status = graphics.DrawImage(pImage, rcDest, rcSrc.X, rcSrc.Y,
                                                rcSrc.Width, rcSrc.Height, Gdiplus::UnitPixel);
}

HitInfo PlayerBoardWnd::OnHitTest(const CRect & rect, const CPoint & point)
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
            index = y * BoardY + x;
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

void PlayerBoardWnd::OnLButtonDown(UINT nFlags, CPoint point)
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

void PlayerBoardWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
}

void PlayerBoardWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    //
}

BOOL PlayerBoardWnd::MoveBoardBlock(const CRect & rect, int index, BOOL bRepaint /* = TRUE */)
{
    if (index >= 0 && index < BoardX * BoardY) {
        int x1 = index % BoardY;
        int y1 = index / BoardY;
        std::lock_guard<std::mutex> lock(this->m_pData->uiMutex);
        for (int dir = Direction::First; dir < Direction::Last; dir++) {
            int x2 = x1 + Dir_Offset[dir].x;
            if (x2 < 0 || x2 >= BoardX)
                continue;
            int y2 = y1 + Dir_Offset[dir].y;
            if (y2 < 0 || y2 >= BoardY)
                continue;
            int move_to = y2 * BoardY + x2;
            if (this->m_pData->playerBoard.grids[move_to] == Color::Empty) {
                std::swap(this->m_pData->playerBoard.grids[index],
                          this->m_pData->playerBoard.grids[move_to]);

                MoveInfo move_info;
                move_info.from_pos = index;
                move_info.move_to_pos = move_to;
                move_info.color = this->m_pData->playerBoard.grids[index];
                move_info.dir = dir;
                this->m_pData->move_path.push_back(move_info);

                lock.~lock_guard();
                InvalidateRect(rect, bRepaint);
                break;
            }
        }
    }
    return TRUE;
}
