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
        : m_bSkinActive(TRUE),
          m_clrTitleActive(nCaptionTextActiveColor),
          m_clrTitleNonActive(nCaptionTextNonActiveColor),
          m_hSkinCaptionBGActiveBrush(NULL),
          m_hSkinCaptionBGNonActiveBrush(NULL),
          m_bSkinLMouseDown(FALSE),
          m_ptSkinLMouseDown(0, 0),
          m_rcSkinLMouseDown(0, 0, 0, 0) {
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

            TCHAR szText[128];
            _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                         _T("(%u) SkinWnd_DrawTitle(): hWnd = 0x%p, m_bSkinActive = %d\n"),
                         GetTickCount(), this->GetSafeHwnd(), m_bSkinActive);
            ::OutputDebugString(szText);

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
        this->m_skinScale9PSprite.DrawBackgroud(dc, dcMem, rect);
    }

    void SkinWnd_DrawFrame(CDC & dc, CDC & dcMem, CRect & rect) {
        this->SkinWnd_DrawFrame(dc.m_hDC, dcMem.m_hDC, rect);
    }

    void SkinWnd_DrawFrame(CDCHandle dc, CDC & dcMem, CRect & rect) {
        this->SkinWnd_DrawFrame(dc, dcMem.m_hDC, rect);
    }

    void SkinWnd_DrawFrame(CDCHandle dc, CDCHandle dcMem, CRect & rect) {
        this->m_skinScale9PSprite.DrawFrame(dc, dcMem, rect);
        this->SkinWnd_DrawTitle(dc, dcMem, rect);
    }

    void SkinWnd_UpdateTitle() {
        CRect rcTitle;
        if (this->GetTitleRect(rcTitle)) {
            ::InvalidateRect(this->GetSafeHwnd(), &rcTitle, TRUE);
        }
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

    void SkinWnd_OnLButtonDown(UINT nFlags, CPoint point) {
        CRect rcTitle;
        if (this->GetCaptionRect(rcTitle)) {
            if (::PtInRect(&rcTitle, point)) {
                if (!m_bSkinLMouseDown) {
                    m_bSkinLMouseDown = TRUE;
                    ::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
                    //::SendMessage(this->GetSafeHwnd(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
                    CRect rcWin;
                    if (::GetWindowRect(this->GetSafeHwnd(), &rcWin)) {
                        m_rcSkinLMouseDown = rcWin;
                        m_ptSkinLMouseDown = point;
                    }
                    if (!this->m_bSkinActive) {
                        this->SkinWnd_UpdateTitle();
                    }
                    this->m_bSkinActive = TRUE;
                    ::SetCapture(this->GetSafeHwnd());
                }
            }
        }
    }

    void SkinWnd_OnLButtonUp(UINT nFlags, CPoint point) {
        if (m_bSkinLMouseDown) {
            if (::ReleaseCapture()) {
                m_bSkinLMouseDown = FALSE;
                ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
            }
        }
    }

    void SkinWnd_OnMouseMove(UINT nFlags, CPoint point) {
        if (m_bSkinLMouseDown) {
            if ((nFlags & MK_LBUTTON) == MK_LBUTTON) {
                if (point != m_ptSkinLMouseDown) {
                    //CRect rcWin;
                    //if (::GetWindowRect(this->GetSafeHwnd(), &rcWin))
                    {
                        CPoint offset = point - m_ptSkinLMouseDown;
                        m_rcSkinLMouseDown.OffsetRect(offset);
                        CRect & rcWin = m_rcSkinLMouseDown;
#if 0
                        TCHAR szText[128];
                        _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                     _T("OnMouseMove(): point.x = %ld, point.y = %ld\n"), point.x, point.y);
                        ::OutputDebugString(szText);
                        _sntprintf_s(szText, _countof(szText) - 1, _countof(szText),
                                     _T("OnMouseMove(): offset.x = %ld, offset.y = %ld\n\n"), offset.x, offset.y);
                        ::OutputDebugString(szText);
#endif
#if 1
                        if (::SetWindowPos(this->GetSafeHwnd(), NULL,
                                           rcWin.left,
                                           rcWin.top,
                                           0, 0,
                                           SWP_NOSIZE)) {
                            return;
                        }
#else
                        if (::MoveWindow(this->GetSafeHwnd(), rcWin.left, rcWin.top,
                                         rcWin.Width(), rcWin.Height(), TRUE)) {
                            return;
                        }
#endif
                    }
                }
                else {
                    return;
                }
            }
        }

        //m_ptSkinLMouseDown = point;
    }

protected:
    //

private:
    BOOL         m_bSkinActive;
    COLORREF     m_clrTitleActive;
    COLORREF     m_clrTitleNonActive;

    HBRUSH       m_hSkinCaptionBGActiveBrush;
    HBRUSH       m_hSkinCaptionBGNonActiveBrush;

    BOOL         m_bSkinLMouseDown;
    CPoint       m_ptSkinLMouseDown;
    CRect        m_rcSkinLMouseDown;

    CBitmap      m_bmpSkinScale9PSprite;
    Scale9Sprite m_skinScale9PSprite;
};
