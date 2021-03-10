#pragma once

template <typename T>
class CSkinToolWndImpl
{
public:
    static const LONG nCaptionHeight = 22;
    static const DWORD nCaptionBGColor = 0x00353535;

    static const LONG nWndBorder = 2;
    static const DWORD nBorderHighlightColor1 = 0x00474747;
    static const DWORD nBorderHighlightColor2 = 0x006a6a6a;
    static const DWORD nBorderShadowColor1 = 0x00282828;
    static const DWORD nBorderShadowColor2 = 0x003a3a3a;

    static const LONG nCloseBtnWidth = 20;
    static const LONG nCloseBtnHeight = 18;

public:
    // Overrideables
	void SkinToolWnd_DoPaint(CDCHandle /*dc*/)
	{
		// must be implemented in a derived class
		ATLASSERT(FALSE);
	}

    // Message map and handlers
	BEGIN_MSG_MAP(CSkinToolWndImpl)
		MESSAGE_HANDLER(WM_ERASEBKGND, SkinToolWnd_OnEraseBackground)
		MESSAGE_HANDLER(WM_PAINT, SkinToolWnd_OnPaint)
		MESSAGE_HANDLER(WM_PRINTCLIENT, SkinToolWnd_OnPaint)
	END_MSG_MAP()

	LRESULT SkinToolWnd_OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
	{
		return 1;   // no background painting needed
	}

	LRESULT SkinToolWnd_OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL & /*bHandled*/)
	{
		T * pT = static_cast<T *>(this);
		ATLASSERT(::IsWindow(pT->m_hWnd));

		if (wParam != NULL)
		{
			RECT rect = {};
			pT->GetClientRect(&rect);
			CMemoryDC dcMem((HDC)wParam, rect);
			pT->DoPaint(dcMem.m_hDC);
		}
		else
		{
			CPaintDC dc(pT->m_hWnd);
			CMemoryDC dcMem(dc.m_hDC, dc.m_ps.rcPaint);
			pT->DoPaint(dcMem.m_hDC);
		}

		return 0;
	}
};
