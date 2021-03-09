#pragma once

#include <stdint.h>
#include <stddef.h>

#include "Board.h"

class TargetBoardWnd : public CWindowImpl<TargetBoardWnd>,
                       public CDoubleBufferImpl<TargetBoardWnd>
{
public:
    static const UINT BoardX = 3;
    static const UINT BoardY = 3;

    TargetBoardWnd();
    ~TargetBoardWnd();

   // Optionally specify name of the new Windows class
   DECLARE_WND_CLASS(_T("TargetBoardWnd"));

   BEGIN_MSG_MAP(TargetBoardWnd)
       //MESSAGE_HANDLER(WM_CREATE, OnCreate)
       //MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
       //MESSAGE_HANDLER(WM_PAINT, OnPaint)
       //MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)

       MSG_WM_CREATE(OnCreate)
       MSG_WM_DESTROY(OnDestroy)
       MSG_WM_CLOSE(OnClose)

       //MSG_WM_ERASEBKGND(OnEraseBackground)
       MSG_WM_ACTIVATE(OnActivate)
       MSG_WM_MOVE(OnMove)

       MSG_WM_LBUTTONDOWN(OnLButtonDown)
       MSG_WM_LBUTTONUP(OnLButtonUp)
       MSG_WM_MOUSEMOVE(OnMouseMove)

       CHAIN_MSG_MAP(CDoubleBufferImpl<TargetBoardWnd>)
   END_MSG_MAP()

   virtual void OnFinalMessage(HWND hWnd);

   //LRESULT OnCreate(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
   //LRESULT OnDestroy(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
   //LRESULT OnPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
   LRESULT OnEraseBackground2(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

   int OnCreate(LPCREATESTRUCT lpCreateStruct);
   void OnClose();
   void OnDestroy();

   void OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther);
   void OnMove(CPoint ptPos);

   BOOL OnEraseBkgnd(CDCHandle dc);
   void DoPaint(CDCHandle dc);

   void OnLButtonDown(UINT nFlags, CPoint point);
   void OnLButtonUp(UINT nFlags, CPoint point);

   void OnMouseMove(UINT nFlags, CPoint point);

protected:
   void PaintBoardGrid(CDCHandle & dc, CDC & dcMem, CPoint & ptBoardBg,
                       UINT x, UINT y, UINT grid);

private:
    HBRUSH m_hBrushBG;

    CDC m_dcMem;

    CBitmapHandle m_bmpBoardBg;
    CBitmapHandle m_bmpGridColors;

    CSize m_szBoardBg;
    CSize m_szGridColors;

    CButton m_btnRandomGen;
    CButton m_btnUserCustomize;
    CButton m_btnImportString;

    CEdit m_txtTargetString;

    Board<BoardX, BoardY> m_board;

    DWORD m_dwLastBringTick;
};
