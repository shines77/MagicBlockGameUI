#pragma once

#include <stdint.h>
#include <stddef.h>

#include "SharedData.h"
#include "Color.h"
#include "Board.h"
#include "Scale9Sprite.h"
#include "SkinWnd.h"

#define ID_BTN_RANDOM_GEN       0
#define ID_BTN_USER_CUSTOMIZE   1
#define ID_BTN_IMPORT_STRING    2

class TargetBoardWnd : public CWindowImpl<TargetBoardWnd>,
                       public CDoubleBufferImpl<TargetBoardWnd>,
                       public CSkinWndImpl<TargetBoardWnd>
{
public:
    TargetBoardWnd(SharedData<BoardX, BoardY, TargetX, TargetY> * pData);
    ~TargetBoardWnd();

    // Optionally specify name of the new Windows class
    DECLARE_WND_CLASS(_T("TargetBoardWnd"));

    BEGIN_MSG_MAP(TargetBoardWnd)
        CHAIN_MSG_MAP(CSkinWndImpl<TargetBoardWnd>)
        //MESSAGE_HANDLER(WM_CREATE, OnCreate)
        //MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        //MESSAGE_HANDLER(WM_PAINT, OnPaint)
        
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_CLOSE(OnClose)

        MSG_WM_ACTIVATE(OnActivate)
        MSG_WM_SHOWWINDOW(OnShowWindow)
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
    void GetBoardBgPoint(CRect & rcWin, CPoint & ptBoardBg);
    void PaintBoardGrid(CDCHandle & dc, CDC & dcMem, CPoint & ptBoardBg,
                        UINT x, UINT y, UINT grid);

private:
    SharedData<BoardX, BoardY, TargetX, TargetY> * m_pData;

    CDC m_dcMem;

    CBitmap m_bmpBoardBg;
    CBitmap m_bmpGridColors;

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
