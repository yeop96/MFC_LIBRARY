// UserRentalDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Book_Management.h"
#include "UserRentalDlg.h"
#include "afxdialogex.h"
#include "Book_ManagementDlg.h"


// UserRentalDlg 대화 상자

IMPLEMENT_DYNAMIC(UserRentalDlg, CDialogEx)


UserRentalDlg::UserRentalDlg(CWnd *pParent /*=nullptr*/)
	: CDialogEx(IDD_USER_RENTAL, pParent)
	, m_strSearch(_T("검색어 입력하세요"))
{
	this->pParent = pParent;
}

UserRentalDlg::~UserRentalDlg()
{
}

void UserRentalDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOOKS, m_BookList);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_strSearch);
	DDX_Control(pDX, IDC_CHECK_AUTHOR, m_CheckAuthor);
	DDX_Control(pDX, IDC_CHECK_PUBLISHER, m_CheckPublisher);
	DDX_Control(pDX, IDC_CHECK_TITLE, m_CheckTitle);
}


BEGIN_MESSAGE_MAP(UserRentalDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_BOOKS, &UserRentalDlg::OnLvnItemchangedListBooks)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &UserRentalDlg::OnBntClickedButtonSearch)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BOOKS, &UserRentalDlg::OnClickListBooks)
END_MESSAGE_MAP()


// UserRentalDlg 메시지 처리기

BOOL UserRentalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu *pSysMenu = GetSystemMenu(FALSE);
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


	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//m_strSearch.SetCueBanner(_T("검색어를 입력하세요!"), TRUE);

	CRect rect;
	m_BookList.GetClientRect(rect);
	// 비트 연산
	m_BookList.SetExtendedStyle(m_BookList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_BookList.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_BookList.InsertColumn(0, _T("도서 제목"), LVCFMT_LEFT, int(rect.Width() * 0.4), -1);
	m_BookList.InsertColumn(1, _T("저자"), LVCFMT_LEFT, int(rect.Width() * 0.2), -1);
	m_BookList.InsertColumn(2, _T("출판사"), LVCFMT_LEFT, int(rect.Width() * 0.2), -1);
	m_BookList.InsertColumn(3, _T("ISBN"), LVCFMT_LEFT, int(rect.Width() * 0.1), -1);
	m_BookList.InsertColumn(4, _T("수량"), LVCFMT_LEFT, int(rect.Width() * 0.1), -1);

	ConnectDB();
	PrintDB("SELECT * FROM book;");
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void UserRentalDlg::ConnectDB()
{
	mysql_init(&Connect); // 초기화
	if (!mysql_real_connect(&Connect, "localhost", DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0)) // 연결
	{
		MessageBox(TEXT("DB 접속에 실패했습니다.\n응용 프로그램을 시작할 수 없습니다."), TEXT("접속 에러"), MB_OK);
		TRACE("Connection error %d: %s\n", mysql_errno(&Connect), mysql_error(&Connect));
	}
	else
	{
		mysql_query(&Connect, "set names euckr"); // 한글 인식

		TRACE("DB 연결 성공");
	}
}


void UserRentalDlg::PrintDB(CString strQ)
{
	CString query;
	query.Format(_T(strQ));
	int seq = 0;
	CString seq_string;
	seq_string.Format(_T("%d"), seq);

	if (mysql_query(&Connect, (CStringA)query))
	{
		TRACE("Connection error %d: %s\n", mysql_errno(&Connect), mysql_error(&Connect));
		return;
	}

	if ((Sql_Result = mysql_store_result(&Connect)) == NULL)
	{
		//쿼리저장
		TRACE("Connection error %d: %s\n", mysql_errno(&Connect), mysql_error(&Connect));
		return;
	}

	m_BookList.DeleteAllItems(); // 리스트 초기화

	while ((Sql_Row = mysql_fetch_row(Sql_Result)) != NULL)
	{
		CString title;
		CString author;
		CString publisher;
		CString isbn;
		CString quantity;

		title += Sql_Row[0];
		author += Sql_Row[1];
		publisher += Sql_Row[2];
		isbn += Sql_Row[3];
		quantity += Sql_Row[4];

		m_BookList.InsertItem(seq, _T(""));
		m_BookList.SetItem(seq, 0, LVIF_TEXT, title, 0, 0, 0, NULL);
		m_BookList.SetItem(seq, 1, LVIF_TEXT, author, 0, 0, 0, NULL);
		m_BookList.SetItem(seq, 2, LVIF_TEXT, publisher, 0, 0, 0, NULL);
		m_BookList.SetItem(seq, 3, LVIF_TEXT, isbn, 0, 0, 0, NULL);
		m_BookList.SetItem(seq, 4, LVIF_TEXT, quantity, 0, 0, 0, NULL);

		seq++;
		seq_string.Format(_T("%d"), seq);
	}
}

BOOLEAN UserRentalDlg::OverlapDB(CString isbn) //대여중복책검사
{
	CString query;
	query.Format(_T("SELECT * FROM rental;"));
	int seq = 0;
	CString seq_string;
	seq_string.Format(_T("%d"), seq);

	if (mysql_query(&Connect, (CStringA)query))
	{
		TRACE("Connection error %d: %s\n", mysql_errno(&Connect), mysql_error(&Connect));
		return false;
	}

	if ((Sql_Result = mysql_store_result(&Connect)) == NULL)
	{
		//쿼리저장
		TRACE("Connection error %d: %s\n", mysql_errno(&Connect), mysql_error(&Connect));
		return false;
	}

	while ((Sql_Row = mysql_fetch_row(Sql_Result)) != NULL)
	{
		if (((CString)Sql_Row[0]).Compare(id_static) == 0 && ((CString)Sql_Row[4]).Compare(isbn) == 0) //빌린 아이디 isbn이 같을경우
		{
			return true;
		}
	}

	return false;
}

BOOLEAN UserRentalDlg::LimitDB() //책권수 제한
{
	CString query;
	query.Format(_T("SELECT * FROM user where id = '%s' ;"), id_static);
	int seq = 0;
	CString seq_string;
	seq_string.Format(_T("%d"), seq);

	if (mysql_query(&Connect, (CStringA)query))
	{
		TRACE("Connection error %d: %s\n", mysql_errno(&Connect), mysql_error(&Connect));
		return false;
	}

	if ((Sql_Result = mysql_store_result(&Connect)) == NULL)
	{
		//쿼리저장
		TRACE("Connection error %d: %s\n", mysql_errno(&Connect), mysql_error(&Connect));
		return false;
	}


	while ((Sql_Row = mysql_fetch_row(Sql_Result)) != NULL)
	{
		int bookNum;
		CString str = (CString)Sql_Row[4];
		bookNum = _tstoi(str);
		if (bookNum >= 3) // 3권이 넘어갈시
		{
			return true;
		}
	}

	return false;
}


void UserRentalDlg::OnPaint()
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

	}
	else
	{
		CDialogEx::OnPaint();
	}
}


void UserRentalDlg::OnLvnItemchangedListBooks(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	*pResult = 0;
}


void UserRentalDlg::OnBntClickedButtonSearch() //검색
{
	// TODO: 여기에 구현 코드 추가.
	UpdateData(TRUE);

	BOOL chkTitle, chkAuthor, chkPublisher; // 체크박스 확인변수

	chkTitle = m_CheckTitle.GetCheck(); // 체크되어있으면 1, 아니면 0 제목
	chkAuthor = m_CheckAuthor.GetCheck(); // 체크되어있으면 1, 아니면 0 저자
	chkPublisher = m_CheckPublisher.GetCheck(); // 체크되어있으면 1, 아니면 0 출판사

	//GetDlgItemText(IDC_EDIT_SEARCH, m_strSearch);

	if (chkTitle == 0 && chkAuthor == 0 && chkPublisher == 0) //아무것도 체크 안되었을시
	{
		MessageBox(TEXT("검색사항 체크안할시 전체를 보여줍니다."));
		PrintDB("SELECT * FROM book;");
	}
	else if (chkTitle == 1 && chkAuthor == 0 && chkPublisher == 0) //제목
	{
		CString tmp;
		tmp.Format(_T("SELECT * FROM book WHERE title LIKE '%%%%%s%%%%'"), m_strSearch);
		PrintDB(tmp);

	}
	else if (chkTitle == 0 && chkAuthor == 1 && chkPublisher == 0) //저자
	{
		CString tmp;
		tmp.Format(_T("SELECT * FROM book WHERE author LIKE '%%%%%s%%%%'"), m_strSearch);
		PrintDB(tmp);
	}
	else if (chkTitle == 0 && chkAuthor == 0 && chkPublisher == 1) //출판사
	{
		CString tmp;
		tmp.Format(_T("SELECT * FROM book WHERE publisher LIKE '%%%%%s%%%%'"), m_strSearch);
		PrintDB(tmp);
	}
	else if (chkTitle == 1 && chkAuthor == 1 && chkPublisher == 0) //제목,저자
	{
		CString tmp;
		tmp.Format(_T("SELECT * FROM book WHERE title LIKE '%%%%%s%%%%' or author LIKE '%%%%%s%%%%'"), m_strSearch, m_strSearch);
		PrintDB(tmp);
	}
	else if (chkTitle == 1 && chkAuthor == 0 && chkPublisher == 1) //제목,출판사
	{
		CString tmp;
		tmp.Format(_T("SELECT * FROM book WHERE title LIKE '%%%%%s%%%%' or publisher LIKE '%%%%%s%%%%'"), m_strSearch, m_strSearch);
		PrintDB(tmp);
	}
	else if (chkTitle == 0 && chkAuthor == 1 && chkPublisher == 1) //저자,출판사
	{
		CString tmp;
		tmp.Format(_T("SELECT * FROM book WHERE author LIKE '%%%%%s%%%%' or publisher LIKE '%%%%%s%%%%'"), m_strSearch, m_strSearch);
		PrintDB(tmp);
	}
	else if (chkTitle == 1 && chkAuthor == 1 && chkPublisher == 1) //제목,저자,출판사
	{
		CString tmp;
		tmp.Format(_T("SELECT * FROM book WHERE title LIKE '%%%%%s%%%%' or author LIKE '%%%%%s%%%%' or publisher LIKE '%%%%%s%%%%'"), m_strSearch, m_strSearch, m_strSearch);
		PrintDB(tmp);
	}




}



void UserRentalDlg::OnClickListBooks(NMHDR *pNMHDR, LRESULT *pResult) //리스트 행 클릭시 이벤트!!
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);


	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	int idx = pNMListView->iItem;

	CString user_id, title, author, publisher, isbn, rentalday, returnday, quantity;  //클릭한 행값들 가져오기

	user_id = id_static; //아이디값
	title = m_BookList.GetItemText(idx, 0);
	author = m_BookList.GetItemText(idx, 1);
	publisher = m_BookList.GetItemText(idx, 2);
	isbn = m_BookList.GetItemText(idx, 3);
	quantity = m_BookList.GetItemText(idx, 4);

	if (m_BookList.GetItemText(idx, 0) == "")
	{
		return;
	}

	int num; //권수
	num = _tstoi(quantity);

	if (num == 0)
	{ //권수 0 일시 리턴
		MessageBox(_T(title + "책은 남은 재고가 없습니다"));
		return;
	}


	CTime cTime = CTime::GetCurrentTime(); // 현재 시스템으로부터 날짜 및 시간을 얻어 온다.

	rentalday.Format("%04d-%02d-%02d", cTime.GetYear(), // 현재 년도 반환
		cTime.GetMonth(), // 현재 월 반환
		cTime.GetDay()); // 현재 일 반환

	returnday.Format("%04d-%02d-%02d", cTime.GetYear(), // 현재 년도 반환
		cTime.GetMonth(), // 현재 월 반환
		cTime.GetDay() + 14); // 현재 일 반환 +14일

	if (IDYES == AfxMessageBox(title + "책을 대여하시겠습니까?", MB_YESNO)) //책 대여
	{
		//OK Action
		if (OverlapDB(isbn)) //대여 중복 검사
		{
			MessageBox(TEXT("이미 대여한 책 입니다."), TEXT("중복 대여"), MB_OK);
			return;
		}

		if (LimitDB()) //대여 3권 넘어갈시
		{
			MessageBox(TEXT("북킹 도서관은 3권까지만 대여 가능합니다. "), TEXT("반납후대여해주세요"), MB_OK);
			return;
		}

		CString query; //대여하기
		query.Format(_T("INSERT INTO rental(user_id, title, author, publisher, isbn, rentalday, returnday) \
		values('%s', '%s', '%s', '%s', '%s', '%s', '%s')"), user_id, title, author, publisher, isbn, rentalday, returnday);
		MessageBox(_T(title + " 대여 완료"));

		if (mysql_query(&Connect, (CStringA)query))
		{
			MessageBox(_T("Insert Error!!!"));
		}

		CString query2; //권수하나줄이기
		query2.Format(_T("UPDATE book SET quantity = %d WHERE isbn='%s'"), \
			num - 1, isbn);

		if (mysql_query(&Connect, (CStringA)query2))
		{
			MessageBox(_T("Update Error"));
		}


		CString query3; //회원 대여 권수 증가
		query3.Format(_T("UPDATE user SET book = book + 1 WHERE id='%s'"), \
			user_id);
		if (mysql_query(&Connect, (CStringA)query3))
		{
			MessageBox(_T("Update Error"));
		}


		PrintDB("SELECT * FROM book;");
	}
	else if (IDNO)

	{

		// NO Action


	}


	*pResult = 0;
}

BOOL UserRentalDlg::PreTranslateMessage(MSG *pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetDlgItem(IDC_EDIT_SEARCH) == GetFocus())
		{
			if (pMsg->wParam == VK_RETURN)
			{
				OnBntClickedButtonSearch();
			}
		}
		switch (pMsg->wParam)
		{
			case VK_RETURN:
			case VK_ESCAPE:
				return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
