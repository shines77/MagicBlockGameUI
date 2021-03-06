#pragma once

class PlayerBoardWnd : public CWindowImpl<PlayerBoardWnd>,
                       public CDoubleBufferImpl<PlayerBoardWnd>
{
public:
    PlayerBoardWnd();
    ~PlayerBoardWnd();

   // Optionally specify name of the new Windows class
   DECLARE_WND_CLASS(_T("PlayerBoardWnd"));

   BEGIN_MSG_MAP(PlayerBoardWnd)
       CHAIN_MSG_MAP(CDoubleBufferImpl<PlayerBoardWnd>)

       //MESSAGE_HANDLER(WM_CREATE, OnCreate)
       //MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
       //MESSAGE_HANDLER(WM_PAINT, OnPaint)

       MSG_WM_CREATE(OnCreate)
       MSG_WM_DESTROY(OnDestroy)
       MSG_WM_CLOSE(OnClose)

       MSG_WM_LBUTTONDOWN(OnLButtonDown)
       MSG_WM_LBUTTONUP(OnLButtonUp)
       MSG_WM_MOUSEMOVE(OnMouseMove)
   END_MSG_MAP()

   virtual void OnFinalMessage(HWND hWnd);

   //LRESULT OnCreate(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
   //LRESULT OnDestroy(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
   //LRESULT OnPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

   int OnCreate(LPCREATESTRUCT lpCreateStruct);
   void OnClose();
   void OnDestroy();

   void DoPaint(CDCHandle dc);

   void OnLButtonDown(UINT nFlags, CPoint point);
   void OnLButtonUp(UINT nFlags, CPoint point);

   void OnMouseMove(UINT nFlags, CPoint point);
};
