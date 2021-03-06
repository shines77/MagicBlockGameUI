#pragma once

class TargetBoardWnd : public CWindowImpl<TargetBoardWnd>
{
public:
    TargetBoardWnd();
    ~TargetBoardWnd();

   // Optionally specify name of the new Windows class
   DECLARE_WND_CLASS(_T("TargetBoardWnd"));

   BEGIN_MSG_MAP(TargetBoardWnd)
       MESSAGE_HANDLER(WM_CREATE, OnCreate)
       MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
       MESSAGE_HANDLER(WM_PAINT, OnPaint)
   END_MSG_MAP()

   virtual void OnFinalMessage(HWND hWnd);

   LRESULT OnCreate(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
   LRESULT OnDestroy(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
   LRESULT OnPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
};
