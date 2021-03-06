// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "AboutDlg.h"
#include "MainDlg.h"

CMainDlg::CMainDlg() : targetBoardShowOnce_(FALSE), targetBoardWnd_(NULL)
{

}

CMainDlg::~CMainDlg()
{
    if (targetBoardWnd_ != NULL)
    {
        delete targetBoardWnd_;
        targetBoardWnd_ = NULL;
    }
}

BOOL CMainDlg::PreTranslateMessage(MSG * pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop * pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

    if (targetBoardWnd_ == NULL) {
        targetBoardWnd_ = new TargetBoardWnd();
        if (targetBoardWnd_ != NULL) {
            CRect rcTargetWnd = { 0, 0, 200, 200 };
            targetBoardWnd_->Create(this->m_hWnd, rcTargetWnd, _T("Ä¿±ê"),
                WS_CAPTION | WS_POPUP | WS_SYSMENU | WS_BORDER | WS_THICKFRAME,
                WS_EX_CLIENTEDGE | WS_EX_TOOLWINDOW);
        }
    }

	return TRUE;
}

void CMainDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    if (!targetBoardShowOnce_ && targetBoardWnd_ != NULL) {
        targetBoardShowOnce_ = TRUE;

        CRect rcClient;
        CPoint ptClient;
        ::GetClientRect(this->m_hWnd, &rcClient);
        ::ClientToScreen(this->m_hWnd, &ptClient);
        rcClient.OffsetRect(ptClient);

        CRect rcBoard;
        ::GetClientRect(targetBoardWnd_->m_hWnd, &rcBoard);
        targetBoardWnd_->SetWindowPos(NULL,
            rcClient.right - rcBoard.Width() - 20, rcClient.top + 20,
            rcClient.right - 20, rcClient.top + 20 + rcBoard.Height(),
            SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
    }
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
{
    if (targetBoardWnd_ != NULL && targetBoardWnd_->m_hWnd != NULL)
    {
        targetBoardWnd_->DestroyWindow();
    }

	// unregister message filtering and idle updates
	CMessageLoop * pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
