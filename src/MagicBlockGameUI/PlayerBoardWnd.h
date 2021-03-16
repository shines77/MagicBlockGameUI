#pragma once

#include <stdint.h>
#include <stddef.h>

#include <gdiplus.h>
#include <atlimage.h>

#include "SharedData.h"
#include "Color.h"
#include "Board.h"
#include "Scale9Sprite.h"
#include "SkinWnd.h"

class PlayerBoardWnd : public CWindowImpl<PlayerBoardWnd>,
                       public CDoubleBufferImpl<PlayerBoardWnd>,
                       public CSkinWndImpl<PlayerBoardWnd>
{
public:
    PlayerBoardWnd(SharedData<BoardX, BoardY, TargetX, TargetY> * pData);
    ~PlayerBoardWnd();

    // Optionally specify name of the new Windows class
    DECLARE_WND_CLASS(_T("PlayerBoardWnd"));

    BEGIN_MSG_MAP(PlayerBoardWnd)
        CHAIN_MSG_MAP(CSkinWndImpl<PlayerBoardWnd>)
        //MESSAGE_HANDLER(WM_CREATE, OnCreate)
        //MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        //MESSAGE_HANDLER(WM_PAINT, OnPaint)
        //MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)

        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_CLOSE(OnClose)

        MSG_WM_ACTIVATE(OnActivate)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_MOVE(OnMove)

        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONUP(OnLButtonUp)
        MSG_WM_MOUSEMOVE(OnMouseMove)

        CHAIN_MSG_MAP(CDoubleBufferImpl<PlayerBoardWnd>)
    END_MSG_MAP()

    virtual void OnFinalMessage(HWND hWnd);

    //LRESULT OnCreate(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
    //LRESULT OnDestroy(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
    //LRESULT OnPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void OnClose();
    void OnDestroy();

    void OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther);
    void OnShowWindow(BOOL bShow, UINT nStatus);
    void OnMove(CPoint ptPos);

    void DoPaint(CDCHandle dc);

    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);

    void OnMouseMove(UINT nFlags, CPoint point);

protected:
    HitInfo OnHitTest(const CRect & rect, const CPoint & point);

    void GetBoardBgPoint(const CRect & rect, CPoint & ptBoardBg);
    void GetBoardBgRect(const CRect & rect, CRect & rcBoardBg);
    void GetBoardBgRect(const CRect & rect, CPoint & ptBoardBg, CRect & rcBoardBg);

    void GetBoardGridPoint(const CRect & rect, CPoint & ptBoardGrid);
    void GetBoardGridRect(const CRect & rect, CPoint & ptBoardGrid, CRect & rcBoardGrid);

    void PaintBoardGrid(CDCHandle & dc, CDC & dcMem, CPoint & ptBoardBg,
                        UINT x, UINT y, UINT grid);
    void PaintBoardArrow(CDCHandle & dc, Gdiplus::Image * pImage, CPoint & ptBoardBg,
                         UINT x, UINT y, int arrow, bool is_opposite);

    BOOL MoveBoardBlock(const CRect & rect, int index, BOOL bRepaint = TRUE);

private:
    SharedData<BoardX, BoardY, TargetX, TargetY> * m_pData;

    CDC m_dcMem;

    CBitmap          m_bmpBoardBg;
    CBitmap          m_bmpGridColors;
    Gdiplus::Image * m_pImgArrows;

    CSize m_szBoardBg;
    CSize m_szGridColors;

    CFont m_btnFont;
    CFont m_editFont;

    CButton m_btnRandomGen;
    CButton m_btnUserCustomize;
    CButton m_btnImportString;

    CEdit m_editTargetString;

    HBRUSH m_hBrushBG;
    DWORD m_dwLastBringTick;
};
