#pragma once

#include <tchar.h>

struct HitArea {
    enum {
        First,
        Caption,
        MinBtn,
        MaxBtn,
        CloseBtn,
        Client,
        Board,
        Last
    };
};

union HitInfo {
    struct {;
        WORD area;
        WORD index;
    };
    DWORD value;

    HitInfo() : value(0) {}
    HitInfo(DWORD _value) : value(_value) {}
    HitInfo(WORD _area, WORD _index) : area(_area), index(_index) {}
    ~HitInfo() {}
};

template <typename T>
class CSkinWndImpl
{
public:
    static const LONG nBorderTop = 3;
    static const LONG nBorderLeft = 3;
    static const LONG nBorderRight = 3;
    static const LONG nBorderBottom = 4;

    static const LONG nCaptionHeight = 22;
    static const LONG nCaptionBottomHeight = 2;

    static const DWORD nCaptionTextActiveColor = 0x00F0F0F0;
    static const DWORD nCaptionTextNonActiveColor = 0x00909090;

    static const DWORD nCaptionBGActiveColor = 0x00353535;
    static const DWORD nCaptionBGNonActiveColor = 0x00686868;

    static const DWORD nBorderHighlightColor1 = 0x00474747;
    static const DWORD nBorderHighlightColor2 = 0x006a6a6a;
    static const DWORD nBorderShadowColor1 = 0x00282828;
    static const DWORD nBorderShadowColor2 = 0x003a3a3a;

    static const LONG nCloseBtnWidth = 20;
    static const LONG nCloseBtnHeight = 18;

public:
    CSkinWndImpl()
        : m_hCursorArrow(NULL),
          m_hCursorSizeAll(NULL),
          m_bSkinActive(TRUE),
          m_bSkinLMouseMoving(FALSE),
          m_bSkinLMouseFirstMoving(FALSE),
          m_clrTitleActive(nCaptionTextActiveColor),
          m_clrTitleNonActive(nCaptionTextNonActiveColor),
          m_hSkinCaptionBGActiveBrush(NULL),
          m_hSkinCaptionBGNonActiveBrush(NULL),
          m_bSkinLMouseDown(FALSE),
          m_ptLMouseDownStart(0, 0),
          m_rcLMouseDownStart(0, 0, 0, 0) {
        m_hCursorArrow   = ::LoadCursor(NULL, IDC_ARROW);
        m_hCursorSizeAll = ::LoadCursor(NULL, IDC_SIZEALL);

        // Caption BG brush
        m_hSkinCaptionBGActiveBrush = ::CreateSolidBrush(nCaptionBGActiveColor);
        m_hSkinCaptionBGNonActiveBrush = ::CreateSolidBrush(nCaptionBGNonActiveColor);

        // Nice palace resource
        m_bmpSkinScale9PSprite.LoadBitmap(IDB_BITMAP_BTN_SCALE9PSPRITE_GRAY);

        // BGColor: 0x00686868, 0x00767676
        m_skinScale9PSprite.SetSprite(m_bmpSkinScale9PSprite.m_hBitmap,
                                      rcScale9PSprite,
                                      kDefaultBGColor);
    }

    virtual ~CSkinWndImpl() {
        this->Destroy();
    }

    void Destroy() {
        if (m_hSkinCaptionBGActiveBrush) {
            ::DeleteObject(m_hSkinCaptionBGActiveBrush);
            m_hSkinCaptionBGActiveBrush = NULL;
        }
        if (m_hSkinCaptionBGNonActiveBrush) {
            ::DeleteObject(m_hSkinCaptionBGNonActiveBrush);
            m_hSkinCaptionBGNonActiveBrush = NULL;
        }
        this->m_skinScale9PSprite.Destroy();
        m_bmpSkinScale9PSprite.DeleteObject();
    }

private:
	BOOL SkinWnd_IsMsgHandled() const {
        return FALSE;
	}

	void SkinWnd_SetMsgHandled(_In_ BOOL bHandled) {
        /* Do nothing */
	}

public:
#ifndef SetMsgHandled
#define SetMsgHandled(X)    SkinWnd_SetMsgHandled(X)
#endif

#ifndef IsMsgHandled
#define IsMsgHandled()      SkinWnd_IsMsgHandled()
#endif

    // Message map and handlers
	BEGIN_MSG_MAP(CSkinWndImpl)
        MSG_WM_NCACTIVATE(SkinWnd_OnNcActivate)
        MSG_WM_ACTIVATE(SkinWnd_OnActivate)

        MSG_WM_LBUTTONDOWN(SkinWnd_OnLButtonDown)
        MSG_WM_LBUTTONUP(SkinWnd_OnLButtonUp)
        MSG_WM_MOUSEMOVE(SkinWnd_OnMouseMove)
	END_MSG_MAP()

#ifdef SetMsgHandled
#undef SetMsgHandled
#endif

#ifdef IsMsgHandled
#undef IsMsgHandled
#endif

    HWND GetSafeHwnd() {
		T * pT = static_cast<T *>(this);
		ATLASSERT(::IsWindow(pT->m_hWnd));
        return (pT->m_hWnd);
    }

    HWND GetSafeHwnd() const {
		const T * pT = const_cast<const T *>(static_cast<T *>(this));
		ATLASSERT(::IsWindow(pT->m_hWnd));
        return (pT->m_hWnd);
    }

    LONG GetTitleHeight() const {
        return (nBorderTop + nCaptionHeight + nCaptionBottomHeight);
    }

    BOOL GetCaptionRect(CRect & rcTitle) {
        CRect rcWin;
        BOOL success = ::GetClientRect(this->GetSafeHwnd(), &rcWin);
        if (success) {
            rcTitle.left   = rcWin.left + nBorderLeft;
            rcTitle.right  = rcWin.right - nBorderRight;
            rcTitle.top    = rcWin.top + nBorderTop;
            rcTitle.bottom = rcWin.top + nBorderTop + nCaptionHeight;
        }
        return success;
    }

    BOOL GetTitleRect(CRect & rcTitle) {
        CRect rcWin;
        BOOL success = ::GetClientRect(this->GetSafeHwnd(), &rcWin);
        if (success) {
            rcTitle.left   = rcWin.left;
            rcTitle.right  = rcWin.right;
            rcTitle.top    = rcWin.top;
            rcTitle.bottom = rcWin.top + this->GetTitleHeight();
        }
        return success;
    }

    void SkinWnd_DrawTitle(CDC & dc, CDC & dcMem, CRect & rect) {
        this->SkinWnd_DrawTitle(dc.m_hDC, dcMem.m_hDC, rect);
    }

    void SkinWnd_DrawTitle(CDCHandle dc, CDC & dcMem, CRect & rect) {
        this->SkinWnd_DrawTitle(dc, dcMem.m_hDC, rect);
    }

    void SkinWnd_DrawTitle(CDCHandle dc, CDCHandle dcMem, CRect & rect) {
        // Paint caption BG
        CRect rcCaption;
        if (GetCaptionRect(rcCaption)) {
            if (dc.m_hDC != NULL) {
                if (m_bSkinActive) {
                    if (this->m_hSkinCaptionBGActiveBrush != NULL) {
                        dc.FillRect(&rcCaption, this->m_hSkinCaptionBGActiveBrush);
                    }
                }
                else {
                    if (this->m_hSkinCaptionBGNonActiveBrush != NULL) {
                        dc.FillRect(&rcCaption, this->m_hSkinCaptionBGNonActiveBrush);
                    }
                }
            }

            TCHAR szTitle[256];
            int nTitleSize = ::GetWindowText(this->GetSafeHwnd(), szTitle, _countof(szTitle) - 1);
	        if (nTitleSize > 0) {
                szTitle[nTitleSize] = '\0';
            }
            else {
                _tcscpy_s(szTitle, _countof(szTitle) - 1, _T("Î´ÃüÃû..."));
                nTitleSize = (int)_tcslen(szTitle);
            }

	        if (m_bSkinActive)
		        dc.SetTextColor(m_clrTitleActive);
	        else
		        dc.SetTextColor(m_clrTitleNonActive);
#if 0
            TCHAR szText[128];
            _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                         _T("(%u) SkinWnd_DrawTitle(): hWnd = 0x%p, m_bSkinActive = %d\n"),
                         GetTickCount(), this->GetSafeHwnd(), m_bSkinActive);
            ::OutputDebugString(szText);
#endif
            CFont font;
	        font.CreatePointFont(::GetSystemMetrics(SM_CYSMCAPTION), _T("System"));
            HFONT pOldFont = dc.SelectFont(font.m_hFont);
            if (pOldFont != NULL) {
                dc.SetBkMode(TRANSPARENT);
	            dc.DrawText(szTitle, nTitleSize, &rcCaption,
                            DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_WORD_ELLIPSIS);
	            dc.SelectFont(pOldFont);
            }
	        font.DeleteObject();
        }
    }

    void SkinWnd_DrawBackgroud(CDC & dc, CDC & dcMem, CRect & rect) {
        this->SkinWnd_DrawBackgroud(dc.m_hDC, dcMem.m_hDC, rect);
    }

    void SkinWnd_DrawBackgroud(CDCHandle dc, CDC & dcMem, CRect & rect) {
        this->SkinWnd_DrawBackgroud(dc, dcMem.m_hDC, rect);
    }

    void SkinWnd_DrawBackgroud(CDCHandle dc, CDCHandle dcMem, CRect & rect) {
        if (1) {
            this->m_skinScale9PSprite.DrawBackgroud(dc, dcMem, rect);
        }
    }

    void SkinWnd_DrawFrame(CDC & dc, CDC & dcMem, CRect & rect) {
        this->SkinWnd_DrawFrame(dc.m_hDC, dcMem.m_hDC, rect);
    }

    void SkinWnd_DrawFrame(CDCHandle dc, CDC & dcMem, CRect & rect) {
        this->SkinWnd_DrawFrame(dc, dcMem.m_hDC, rect);
    }

    void SkinWnd_DrawFrame(CDCHandle dc, CDCHandle dcMem, CRect & rect) {
        if (1) {
            this->m_skinScale9PSprite.DrawFrame(dc, dcMem, rect);
        }
        this->SkinWnd_DrawTitle(dc, dcMem, rect);
    }

    void SkinWnd_UpdateTitle() {
#if 0
        ::InvalidateRect(this->GetSafeHwnd(), NULL, FALSE);
        ::UpdateWindow(this->GetSafeHwnd());
#else
        CRect rcTitle;
        if (this->GetTitleRect(rcTitle)) {
            ::InvalidateRect(this->GetSafeHwnd(), &rcTitle, FALSE);
            ::UpdateWindow(this->GetSafeHwnd());
        }
#endif
    }

    BOOL SkinWnd_OnNcActivate(BOOL bActive) {
        if (this->m_bSkinActive != bActive) {
            this->m_bSkinActive = bActive;
            this->SkinWnd_UpdateTitle();
        }
        TCHAR szText[128];
        _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                     _T("SkinWnd_OnNcActivate(): bActive = %d\n"), bActive);
        ::OutputDebugString(szText);
        return FALSE;
    }

    void SkinWnd_OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther) {
        BOOL bActive;
        if ((nState == WA_ACTIVE && !bMinimized) || (nState == WA_CLICKACTIVE)) {
            bActive = TRUE;
        }
        else {
            bActive = FALSE;
        }

        if (this->m_bSkinActive != bActive) {
            this->m_bSkinActive = bActive;
            this->SkinWnd_UpdateTitle();
        }

        TCHAR szText[128];
        _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                     _T("SkinWnd_OnActivate(): bActive = %d\n"), bActive);
        ::OutputDebugString(szText);
    }

    bool GetScreenOffset(HWND hWnd, CPoint & ptScreen, CPoint & ptParentScreen) {
        bool success;
        if (::ClientToScreen(hWnd, &ptScreen)) {
            CPoint ptParentOffset;
            HWND hwndParent = ::GetParent(hWnd);
            while (hwndParent != NULL) {
                CPoint ptOffset;
                if (::ClientToScreen(hwndParent, &ptOffset)) {
                    ptParentOffset.Offset(ptOffset);
                    hwndParent = ::GetParent(hwndParent);
                }
                else {
                    break;
                }
            }
            ptParentScreen = ptParentOffset;
            success = true;
        }
        else {
            ptScreen.x = 0;
            ptScreen.y = 0;
            ptParentScreen.x = 0;
            ptParentScreen.y = 0;
            success = false;
        }
        return success;
    }

    void SkinWnd_OnLButtonDown(UINT nFlags, CPoint point) {
        CRect rcTitle;
        if (this->GetCaptionRect(rcTitle)) {
            if (::PtInRect(&rcTitle, point)) {
                if (!m_bSkinLMouseDown) {
                    CPoint ptStart = point;
                    CPoint ptAbsoluteStart = point;
                    CRect rcStart;
                    if (::GetWindowRect(this->GetSafeHwnd(), &rcStart)) {
                        CPoint ptScreen, ptParentScreen;
                        if (this->GetScreenOffset(this->GetSafeHwnd(), ptScreen, ptParentScreen)) {
                            ptAbsoluteStart = ptScreen;
                            ptAbsoluteStart += ptParentScreen;
                            ptAbsoluteStart += point;
                        }
                        m_ptParentScreen = ptParentScreen;
#if 0
                        TCHAR szText[128];
                        OutputDebugString(_T("-----------------------------------------------\n"));
                        _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                     _T("(%u) SkinWnd_OnLButtonDown(): m_ptParentScreen = (%d, %d)\n"),
                                     ::GetTickCount(), (int)m_ptParentScreen.x, (int)m_ptParentScreen.y);
                        OutputDebugString(szText);
                        _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                     _T("(%u) SkinWnd_OnLButtonDown(): point = (%d, %d)\n"),
                                     ::GetTickCount(), (int)point.x, (int)point.y);
                        OutputDebugString(szText);
                        _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                     _T("(%u) SkinWnd_OnLButtonDown(): ptScreen = (%d, %d)\n"),
                                     ::GetTickCount(), (int)ptScreen.x, (int)ptScreen.y);
                        OutputDebugString(szText);
                        _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                     _T("(%u) SkinWnd_OnLButtonDown(): ptStart = (%d, %d)\n"),
                                     ::GetTickCount(), (int)ptStart.x, (int)ptStart.y);
                        OutputDebugString(szText);
                        _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                     _T("(%u) SkinWnd_OnLButtonDown(): ptAbsoluteStart = (%d, %d)\n"),
                                     ::GetTickCount(), (int)ptAbsoluteStart.x, (int)ptAbsoluteStart.y);
                        OutputDebugString(szText);
                        _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                     _T("(%u) SkinWnd_OnLButtonDown(): rcStart = (%d, %d)\n"),
                                     ::GetTickCount(), (int)rcStart.left, (int)rcStart.top);
                        OutputDebugString(szText);
                        OutputDebugString(_T("-----------------------------------------------\n"));
#endif
                        m_ptLMouseDownStart = ptStart;
                        m_ptLMouseDownAbsStart = ptAbsoluteStart;
                        m_rcLMouseDownStart = rcStart;
                    }
                    else {
                        return;
                    }
                    m_bSkinLMouseDown = TRUE;
                    m_bSkinLMouseFirstMoving = TRUE;
                    //::SendMessage(this->GetSafeHwnd(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
                    HWND hwndSaveCapture = ::SetCapture(this->GetSafeHwnd());
                    if (hwndSaveCapture == NULL) {
                        ::SetCursor(m_hCursorSizeAll);
                        if (!this->m_bSkinActive) {
                            this->SkinWnd_UpdateTitle();
                        }
                        this->m_bSkinActive = TRUE;
                    }
                }
            }
        }
    }

    void SkinWnd_OnLButtonUp(UINT nFlags, CPoint point) {
        bool lockReleased = false;
        if (m_bSkinLMouseDown) {
            if (::ReleaseCapture()) {
                ::SetCursor(m_hCursorArrow);
            }
            m_bSkinLMouseFirstMoving = FALSE;
            m_bSkinLMouseDown = FALSE;
        }
    }

    void SkinWnd_OnMouseMove(UINT nFlags, CPoint point) {
        if ((nFlags & MK_LBUTTON) == MK_LBUTTON) {
            try {
                if (m_bSkinLMouseDown) {
                    if (!m_bSkinLMouseMoving) {
                        CPoint ptScreen;
                        CPoint ptMoveTo = point;
                        CPoint ptAbsoluteMoveTo = point;
                        if (::ClientToScreen(this->GetSafeHwnd(), &ptScreen)) {
                            ptAbsoluteMoveTo.Offset(ptScreen);
                            ptAbsoluteMoveTo.Offset(m_ptParentScreen);
                        }
                        CPoint ptOffset = ptMoveTo - m_ptLMouseDownStart;
                        CPoint ptAbsoluteOffset = ptAbsoluteMoveTo - m_ptLMouseDownAbsStart;
                        CRect rcMoveTo = m_rcLMouseDownStart;
                        rcMoveTo.OffsetRect(ptAbsoluteOffset);
                        rcMoveTo.OffsetRect(-m_ptParentScreen.x, -m_ptParentScreen.y);
#if 0
                        if (m_bSkinLMouseFirstMoving) {
                            TCHAR szText[128];
                            _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                         _T("(%u) SkinWnd_OnMouseMove(): point = (%d, %d)\n"),
                                         ::GetTickCount(), (int)point.x, (int)point.y);
                            OutputDebugString(szText);
                            _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                         _T("(%u) SkinWnd_OnMouseMove(): ptScreen = (%d, %d)\n"),
                                         ::GetTickCount(), (int)ptScreen.x, (int)ptScreen.y);
                            OutputDebugString(szText);
                            _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                         _T("(%u) SkinWnd_OnMouseMove(): ptOffset = (%d, %d)\n"),
                                         ::GetTickCount(), (int)ptOffset.x, (int)ptOffset.y);
                            OutputDebugString(szText);
                            _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                         _T("(%u) SkinWnd_OnMouseMove(): ptAbsoluteOffset = (%d, %d)\n"),
                                         ::GetTickCount(), (int)ptAbsoluteOffset.x, (int)ptAbsoluteOffset.y);
                            OutputDebugString(szText);
                            _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                         _T("(%u) SkinWnd_OnMouseMove(): rcMoveTo = (%d, %d)\n"),
                                         ::GetTickCount(), (int)rcMoveTo.left, (int)rcMoveTo.top);
                            OutputDebugString(szText);
                            OutputDebugString(_T("-----------------------------------------------\n"));
                            //m_bSkinLMouseFirstMoving = FALSE;
                        }
#endif
                        m_bSkinLMouseMoving = TRUE;
#if 0
                        TCHAR szText[128];
                        _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                        _T("OnMouseMove(): point.x = %ld, point.y = %ld\n"), point.x, point.y);
                        ::OutputDebugString(szText);
                        _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                        _T("OnMouseMove(): offset.x = %ld, offset.y = %ld\n\n"), offset.x, offset.y);
                        ::OutputDebugString(szText);
#endif
                        if (m_bSkinLMouseFirstMoving) {
                            m_bSkinLMouseDown = FALSE;
                            //m_bSkinLMouseFirstMoving = FALSE;
                        }
#if 1
                        if (::SetWindowPos(this->GetSafeHwnd(), NULL,
                                           rcMoveTo.left,
                                           rcMoveTo.top,
                                           0, 0,
                                           SWP_NOSIZE)) {
                            //m_rcLMouseDownStart = rcMoveTo;
                            m_bSkinLMouseMoving = FALSE;
                            m_bSkinLMouseDown = TRUE;
                            return;
                        }
#else
                        if (::MoveWindow(this->GetSafeHwnd(), rcMoveTo.left, rcMoveTo.top,
                                         rcMoveTo.Width(), rcMoveTo.Height(), TRUE)) {
                            //m_rcLMouseDownStart = rcMoveTo;
                            m_bSkinLMouseMoving = FALSE;
                            m_bSkinLMouseDown = TRUE;
                            return;
                        }
#endif
                    }
                    else {
                        return;
                    }
                }
            }
            catch (const std::exception & ex) {
                std::string err_info = "Exception: ";
                err_info += std::string(ex.what());
                OutputDebugStringA(err_info.c_str());
                MessageBoxA(this->GetSafeHwnd(), err_info.c_str(), "Magic Block", MB_ICONINFORMATION | MB_OK);
            }
        }
    }

protected:
    //

private:
    HCURSOR      m_hCursorArrow;
    HCURSOR      m_hCursorSizeAll;

    BOOL         m_bSkinActive;
    COLORREF     m_clrTitleActive;
    COLORREF     m_clrTitleNonActive;

    HBRUSH       m_hSkinCaptionBGActiveBrush;
    HBRUSH       m_hSkinCaptionBGNonActiveBrush;

    BOOL         m_bSkinLMouseDown;
    BOOL         m_bSkinLMouseMoving;
    BOOL         m_bSkinLMouseFirstMoving;

    CPoint       m_ptLMouseDownStart;
    CPoint       m_ptLMouseDownAbsStart;
    CPoint       m_ptParentScreen;
    CRect        m_rcLMouseDownStart;

    CBitmap      m_bmpSkinScale9PSprite;
    Scale9Sprite m_skinScale9PSprite;
};
