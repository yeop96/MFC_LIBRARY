
// Book_ManagementDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Book_Management.h"
#include "Book_ManagementDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnButtonLogin();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBookManagementDlg 대화 상자



CBookManagementDlg::CBookManagementDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
	, m_strID(_T(""))
	, m_strPW(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_BOOK); // favicon
}

void CBookManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_PW, m_strPW);
	DDX_Control(pDX, IDC_RADIO_USER, m_rUser);
	DDX_Control(pDX, IDC_STATIC_BACKGROUND, m_imgBg);
}

BEGIN_MESSAGE_MAP(CBookManagementDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_USER, &CBookManagementDlg::OnButtonUser)
	ON_BN_CLICKED(IDC_BUTTON_ADMIN, &CBookManagementDlg::OnButtonAdmin)
	ON_BN_CLICKED(IDC_BUTTON_SIGNUP, &CBookManagementDlg::OnButtonSignup)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CBookManagementDlg::OnButtonLogin)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CBookManagementDlg 메시지 처리기

BOOL CBookManagementDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_rUser.SetCheck(TRUE);

	HBITMAP hBit = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BACKGROUND));
	m_imgBg.SetBitmap(hBit);
	CRect rt;
	GetClientRect(&rt);
	m_imgBg.SetWindowPos(NULL, 0, 0, rt.Width(), rt.Height(), SWP_SHOWWINDOW);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBookManagementDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBookManagementDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBookManagementDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBookManagementDlg::OnButtonUser()
{
	// 방법 1
	UserDlg userDlg;
	userDlg.DoModal();

	// 방법 2
	/*UserDlg *userDlg = new UserDlg(this);
	userDlg->Create(IDD_USER);
	userDlg->ShowWindow(SW_SHOW);
	this->ShowWindow(SW_HIDE);*/
}


void CBookManagementDlg::OnButtonAdmin()
{
	// 방법 1
	AdminDlg adminDlg;
	adminDlg.DoModal();
}


void CBookManagementDlg::OnButtonSignup()
{
	// 방법 1
	SignupDlg signupDlg(this);
	signupDlg.DoModal();
}


void CBookManagementDlg::OnButtonLogin()
{
	UpdateData(TRUE);

	// 빈칸이 있을 경우
	if ((CStringA)m_strID == "" && (CStringA)m_strPW == "")
		MessageBox(TEXT("아이디와 패스워드를 입력해주세요."), TEXT("로그인 실패"), MB_OK);
	else if ((CStringA)m_strID == "")
		MessageBox(TEXT("아이디를 입력해주세요."), TEXT("로그인 실패"), MB_OK);
	else if ((CStringA)m_strPW == "")
		MessageBox(TEXT("패스워드를 입력해주세요."), TEXT("로그인 실패"), MB_OK);

	// 회원으로 로그인
	else if (m_rUser.GetCheck())
	{
		id_static = (CStringA)m_strID;
		mysql_init(&Connect);
		mysql_real_connect(&Connect, MY_IP, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
		mysql_query(&Connect, "set names euckr");
		CString query;
		query.Format(_T("select * from user where id = '%s' and pw = '%s'"), m_strID, m_strPW);
		mysql_query(&Connect, (CStringA)query);
		Sql_Result = mysql_store_result(&Connect);
		if ((Sql_Row = mysql_fetch_row(Sql_Result)) == NULL)
		{
			MessageBox(TEXT("일치하는 회원이 없습니다."), TEXT("로그인 실패"), MB_OK);
		}
		else
		{
			UserDlg userDlg;
			userDlg.DoModal();
		}
		mysql_close(&Connect);
	}

	// 관리자로 로그인
	else
	{
		if ((CStringA)(m_strPW) == "admin" && (CStringA)(m_strPW) == "admin")
		{
			AdminDlg adminDlg;
			adminDlg.DoModal();
		}
		else
		{
			MessageBox(TEXT("잘못된 접근입니다."), TEXT("경고"), MB_OK);
		}
	}
}


HBRUSH CBookManagementDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	/*if (IDC_STATIC_GROUP_BOX == pWnd->GetDlgCtrlID())
	{
		CPoint ul(0, 6);
		CRect rect;
		pWnd->GetWindowRect(&rect);
		CPoint lr((rect.right - rect.left - 2), (rect.bottom - rect.top - 2));
		pDC->FillSolidRect(CRect(ul, lr), RGB(255, 255, 255));
		pWnd->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}*/
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_STATIC_GROUP_BOX:
		case IDC_RADIO_USER:
		case IDC_RADIO_ADMIN:
		case IDC_STATIC_ID:
		case IDC_STATIC_PW:
			pDC->SetBkMode(TRANSPARENT);
			return reinterpret_cast<HBRUSH>(::GetStockObject(NULL_BRUSH));
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


BOOL CBookManagementDlg::PreTranslateMessage(MSG *pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_RETURN:
			case VK_ESCAPE:
				return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
