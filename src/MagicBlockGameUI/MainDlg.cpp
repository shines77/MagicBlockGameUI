// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "AboutDlg.h"
#include "MainDlg.h"

CMainDlg::CMainDlg()
    : targetBoardShowOnce_(FALSE), targetBoardWnd_(NULL),
      playerBoardShowOnce_(FALSE), playerBoardWnd_(NULL),
      m_pSkinToolbar(NULL)
{
    //
}

CMainDlg::~CMainDlg()
{
    if (targetBoardWnd_ != NULL)
    {
        delete targetBoardWnd_;
        targetBoardWnd_ = NULL;
    }

    if (playerBoardWnd_ != NULL)
    {
        delete playerBoardWnd_;
        playerBoardWnd_ = NULL;
    }

    if (m_pSkinToolbar) {
        delete m_pSkinToolbar;
        m_pSkinToolbar = NULL;
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
	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR,
        ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR,
        ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop * pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

    this->CreateBoardWnd();
    this->CreateSimpleToolBar();

	// center the dialog on the screen
	this->CenterWindow();

	this->UIAddChildWindowContainer(m_hWnd);

	return TRUE;
}

void CMainDlg::CreateBoardWnd()
{
    this->m_data.parent = m_hWnd;

    DWORD dwExStyle = WS_EX_TOOLWINDOW;

    if (targetBoardWnd_ == NULL) {
        targetBoardWnd_ = new TargetBoardWnd(&this->m_data);
        if (targetBoardWnd_ != NULL) {
            CRect rcTargetWnd = { 0, 0, 310, 420 };
            targetBoardWnd_->Create(this->m_hWnd, rcTargetWnd, _T("Ä¿±ê×´Ì¬"),
                WS_CHILD | WS_OVERLAPPED | WS_GROUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                dwExStyle);
        }
    }

    if (playerBoardWnd_ == NULL) {
        playerBoardWnd_ = new PlayerBoardWnd(&this->m_data);
        if (playerBoardWnd_ != NULL) {
            CRect rcPlayerWnd = { 0, 0, 480, 585 };
            playerBoardWnd_->Create(this->m_hWnd, rcPlayerWnd, _T("Íæ¼ÒÆåÅÌ"),
                WS_CHILD | WS_OVERLAPPED | WS_GROUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                dwExStyle);
        }
    }
}

void CMainDlg::CreateSimpleToolBar()
{
    DWORD dwExStyle = WS_EX_TOOLWINDOW;

    if (m_pSkinToolbar == NULL) {
        m_pSkinToolbar = new CSkinToolBar();
        if (m_pSkinToolbar != NULL) {
            CRect rcToolbar = { 0, 0, 400, 25 };
            m_pSkinToolbar->Create(this->m_hWnd, rcToolbar, _T("ToolBar"),
                WS_CHILD | WS_OVERLAPPED | WS_GROUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                dwExStyle);
            m_pSkinToolbar->AddItem(0, STBI_STYLE_BUTTON);
            m_pSkinToolbar->SetItemText(0, _T("111111"));
            m_pSkinToolbar->SetItemToolTipText(0, _T("111111"));
            m_pSkinToolbar->AddItem(1, STBI_STYLE_BUTTON);
            m_pSkinToolbar->SetItemText(1, _T("222222"));
            m_pSkinToolbar->SetItemToolTipText(1, _T("222222"));
            m_pSkinToolbar->AddItem(2, STBI_STYLE_BUTTON);
            m_pSkinToolbar->SetItemText(2, _T("333333"));
            m_pSkinToolbar->SetItemToolTipText(2, _T("333333"));
            m_pSkinToolbar->AddItem(3, STBI_STYLE_BUTTON);
            m_pSkinToolbar->SetItemText(3, _T("444444"));
            m_pSkinToolbar->SetItemToolTipText(3, _T("444444"));
        }
    }
}

void CMainDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    static const LONG nEdgeLeft = 30;
    static const LONG nEdgeMiddle = 45;
    static const LONG nEdgeRight = 30;
    static const LONG nEdgeTop = 30;
    static const LONG nEdgeTargetTop = 100;

    if (!targetBoardShowOnce_ && targetBoardWnd_ != NULL) {
        targetBoardShowOnce_ = TRUE;

        CRect rcClient;
        ::GetClientRect(this->m_hWnd, &rcClient);

        CRect rcBoard;
        ::GetClientRect(targetBoardWnd_->m_hWnd, &rcBoard);
        targetBoardWnd_->SetWindowPos(NULL,
            rcClient.left + nEdgeLeft,
            rcClient.top + nEdgeTargetTop,
            rcClient.left + nEdgeLeft + rcBoard.Width(),
            rcClient.top + nEdgeTargetTop + rcBoard.Height(),
            SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
    }

    if (!playerBoardShowOnce_ && playerBoardWnd_ != NULL) {
        playerBoardShowOnce_ = TRUE;

        CRect rcClient;
        ::GetClientRect(this->m_hWnd, &rcClient);

        CRect rcTarget;
        ::GetClientRect(targetBoardWnd_->m_hWnd, &rcTarget);

        CRect rcBoard;
        ::GetClientRect(playerBoardWnd_->m_hWnd, &rcBoard);

        playerBoardWnd_->SetWindowPos(NULL,
            rcClient.left + nEdgeLeft + rcTarget.Width() + nEdgeMiddle,
            rcClient.top + nEdgeTop,
            rcClient.left + nEdgeLeft + rcTarget.Width() + nEdgeMiddle + rcBoard.Width(),
            rcClient.top + nEdgeTop + rcBoard.Height(),
            SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);

        //playerBoardWnd_->SetActiveWindow();
    }

    if (m_pSkinToolbar != NULL) {
        CRect rcClient;
        ::GetClientRect(this->m_hWnd, &rcClient);

        CRect rcToolbar;
        ::GetClientRect(m_pSkinToolbar->m_hWnd, &rcToolbar);

        m_pSkinToolbar->SetWindowPos(NULL,
            rcClient.left,
            rcClient.top,
            rcClient.left + rcToolbar.Width(),
            rcClient.top + rcToolbar.Height(),
            SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
    }
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
{
    if (targetBoardWnd_ != NULL && targetBoardWnd_->m_hWnd != NULL)
    {
        targetBoardWnd_->DestroyWindow();
    }

    if (playerBoardWnd_ != NULL && playerBoardWnd_->m_hWnd != NULL)
    {
        playerBoardWnd_->DestroyWindow();
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
