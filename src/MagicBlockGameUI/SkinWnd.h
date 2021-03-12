#pragma once

#include <tchar.h>

#ifndef SetMsgHandled
#define SetMsgHandled(X)    SkinWnd_SetMsgHandled(X)
#endif

#ifndef IsMsgHandled
#define IsMsgHandled()      SkinWnd_IsMsgHandled()
#endif

template <typename T>
class CSkinWndImpl
{
public:
    static const LONG nCaptionHeight = 22;
    static const DWORD nCaptionBGColor = 0x00353535;

    static const LONG nBorderTop = 2;
    static const LONG nBorderLeft = 2;
    static const LONG nBorderRight = 2;
    static const LONG nBorderBottom = 3;

    static const DWORD nBorderHighlightColor1 = 0x00474747;
    static const DWORD nBorderHighlightColor2 = 0x006a6a6a;
    static const DWORD nBorderShadowColor1 = 0x00282828;
    static const DWORD nBorderShadowColor2 = 0x003a3a3a;

    static const LONG nCloseBtnWidth = 20;
    static const LONG nCloseBtnHeight = 18;

public:
    CSkinWndImpl() :
        m_bSkinLMouseDown(FALSE),
        m_ptSkinLMouseDown(0, 0),
        m_rcSkinLMouseDown(0, 0, 0, 0) {
        //
    }

    virtual ~CSkinWndImpl() {
    }

	BOOL SkinWnd_IsMsgHandled() const {
        return FALSE;
	}

	void SkinWnd_SetMsgHandled(_In_ BOOL bHandled) {
        /* Do nothing */
	}

    // Message map and handlers
	BEGIN_MSG_MAP(CSkinWndImpl)
        MSG_WM_NCACTIVATE(SkinWnd_OnNcActivate)
        MSG_WM_ACTIVATE(SkinWnd_OnActivate)

        MSG_WM_LBUTTONDOWN(SkinWnd_OnLButtonDown)
        MSG_WM_LBUTTONUP(SkinWnd_OnLButtonUp)
        MSG_WM_MOUSEMOVE(SkinWnd_OnMouseMove)
	END_MSG_MAP()

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

    BOOL GetTitleRect(CRect & rcTitle) {
        CRect rcWin;
        BOOL success = ::GetClientRect(this->GetSafeHwnd(), &rcWin);
        if (success) {
            rcTitle.left   = nBorderLeft;
            rcTitle.right  = rcWin.right - nBorderRight;
            rcTitle.top    = nBorderTop;
            rcTitle.bottom = nBorderTop + nCaptionHeight;
        }
        return success;
    }

    void SkinWnd_DrawTitle() {
        //
    }

    void SkinWnd_DrawBackgroud() {
        //
    }

    void SkinWnd_DrawFrame() {
        //
    }

    BOOL SkinWnd_OnNcActivate(BOOL bActive) {
        return FALSE;
    }

    void SkinWnd_OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther) {
        //
    }

    void SkinWnd_OnLButtonDown(UINT nFlags, CPoint point) {
        CRect rcTitle;
        if (GetTitleRect(rcTitle)) {
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
                    CRect rcWin;
                    if (::GetWindowRect(this->GetSafeHwnd(), &rcWin)) {
                        CPoint offset = point - m_ptSkinLMouseDown;
                        m_rcSkinLMouseDown.OffsetRect(offset);
                        rcWin = m_rcSkinLMouseDown;
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
                                           SWP_NOSIZE | SWP_NOACTIVATE)) {
                            return;
                        }
#else
                        if (::MoveWindow(this->GetSafeHwnd(), rcWin.left, rcWin.top, rcWin.Width(), rcWin.Height(), TRUE)) {
                            //m_ptSkinLMouseDown = point;
                            //m_ptSkinLMouseDownWndPos = rcWin.TopLeft();
                            return;
                        }
#endif
                    }
                }
            }
        }

        m_ptSkinLMouseDown = point;
    }

protected:
    //

private:
    BOOL    m_bSkinLMouseDown;
    CPoint  m_ptSkinLMouseDown;
    CRect   m_rcSkinLMouseDown;
};

#ifdef SetMsgHandled
#undef SetMsgHandled
//#define SetMsgHandled(X)    SetMsgHandled(X)
#endif

#ifdef IsMsgHandled
#undef IsMsgHandled
//#define IsMsgHandled()      IsMsgHandled()
#endif
