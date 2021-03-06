﻿// AdminUserDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Book_Management.h"
#include "AdminUserDlg.h"
#include "afxdialogex.h"


// AdminUserDlg 대화 상자

IMPLEMENT_DYNAMIC(AdminUserDlg, CDialogEx)

AdminUserDlg::AdminUserDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADMIN_USER, pParent)
{

}

AdminUserDlg::~AdminUserDlg()
{
}

void AdminUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, m_list);
}


BEGIN_MESSAGE_MAP(AdminUserDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_USER_ADD, &AdminUserDlg::OnButtonUserAdd)
	ON_BN_CLICKED(IDC_BUTTON_USER_EDIT, &AdminUserDlg::OnButtonUserEdit)
	ON_BN_CLICKED(IDC_BUTTON_USER_DELETE, &AdminUserDlg::OnButtonUserDelete)
	ON_NOTIFY(HDN_ITEMCLICKA, 0, AdminUserDlg::OnHdnItemClickList)
	ON_NOTIFY(HDN_ITEMCLICKW, 0, AdminUserDlg::OnHdnItemClickList)
	ON_COMMAND(ID_MENU_ADD, &AdminUserDlg::OnButtonUserAdd)
	ON_COMMAND(ID_MENU_EDIT, &AdminUserDlg::OnButtonUserEdit)
	ON_COMMAND(ID_MENU_DELETE, &AdminUserDlg::OnButtonUserDelete)
	ON_COMMAND(ID_MENU_RESET, &AdminUserDlg::OnButtonUserReset)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_USER, &AdminUserDlg::OnCustomdrawList)
END_MESSAGE_MAP()


// AdminUserDlg 메시지 처리기


BOOL AdminUserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect rect;
	m_list.GetClientRect(rect);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, _T("ID"), LVCFMT_LEFT, int(rect.Width() * 0.15), -1);
	m_list.InsertColumn(1, _T("PW"), LVCFMT_LEFT, int(rect.Width() * 0.15), -1);
	m_list.InsertColumn(2, _T("이름"), LVCFMT_LEFT, int(rect.Width() * 0.15), -1);
	m_list.InsertColumn(3, _T("연락처"), LVCFMT_LEFT, int(rect.Width() * 0.25), -1);
	m_list.InsertColumn(4, _T("대여 권 수"), LVCFMT_LEFT, int(rect.Width() * 0.15), -1);
	m_list.InsertColumn(5, _T("연체 횟수"), LVCFMT_LEFT, int(rect.Width() * 0.15), -1);
	//칼럼 추가 인덱스, 칼람명, 정렬방향, 칼럼길이, 서브아이템 갯수

	PrintDB();

	/*for (int i = 0; i < m_list.GetHeaderCtrl()->GetItemCount(); ++i)
		m_list.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);*/

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL AdminUserDlg::PreTranslateMessage(MSG *pMsg)
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


void AdminUserDlg::OnButtonUserAdd()
{
	editmode = FALSE;
	SignupDlg signupDlg(this);
	signupDlg.DoModal();
}


void AdminUserDlg::OnButtonUserEdit()
{
	editmode = TRUE;
	SignupDlg signupDlg(this);
	signupDlg.DoModal();
}


void AdminUserDlg::OnButtonUserDelete()
{
	DeleteDB();
	PrintDB();
}


void AdminUserDlg::OnButtonUserReset()
{
	UINT uSelectedCount = m_list.GetSelectedCount();
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int nSelected = m_list.GetNextSelectedItem(pos);

	if (uSelectedCount <= 0)
	{
		MessageBox(_T("선택된 회원이 없습니다."));
		return;
	}
	else if (uSelectedCount > 1)
	{
		MessageBox(_T("한 개만 선택해주세요."));
		return;
	}

	CString query;
	query.Format(_T("UPDATE user SET overdue=0 WHERE id='%s'"), m_list.GetItemText(nSelected, 0));

	if (mysql_query(&Connect, (CStringA)query))
	{
		MessageBox(_T("Edit Error"));
	}

	PrintDB();
}


void AdminUserDlg::OnContextMenu(CWnd * /*pWnd*/, CPoint point)
{
	CMenu menu;

	// 팝업 메뉴를 생성한다.
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, ID_MENU_ADD, _T("추가"));
	menu.AppendMenu(MF_STRING, ID_MENU_EDIT, _T("수정"));
	menu.AppendMenu(MF_STRING, ID_MENU_DELETE, _T("삭제"));
	menu.AppendMenu(MF_STRING, ID_MENU_RESET, _T("연체 횟수 초기화"));

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	menu.DestroyMenu();
}


void AdminUserDlg::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString strType;
	BOOL bNoticeFlag = FALSE;
	BOOL bWarnningFlag = FALSE;

	NMLVCUSTOMDRAW *pLVCD = (NMLVCUSTOMDRAW *)pNMHDR;

	strType = m_list.GetItemText(pLVCD->nmcd.dwItemSpec, 5);

	if (_ttoi(strType) >= 5)
	{
		bWarnningFlag = TRUE;
	}

	else if (_ttoi(strType) >= 3)
	{
		bNoticeFlag = TRUE;
	}

	*pResult = 0;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYITEMDRAW;

	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		if (bWarnningFlag)
		{
			pLVCD->clrText = RGB(255, 0, 0);  // 글자 색 변경 
			//pLVCD->clrTextBk = RGB(0, 0, 0);  // 배경 색 변경 
		}
		else if (bNoticeFlag)
		{
			pLVCD->clrText = RGB(0, 0, 255);
			//pLVCD->clrTextBk = RGB(237, 255, 255);
		}
		else
		{
			pLVCD->clrText = RGB(0, 0, 0);
		}

		*pResult = CDRF_DODEFAULT;
	}
}


void AdminUserDlg::ConnectDB()
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


void AdminUserDlg::PrintDB()
{
	CString query;
	query.Format(_T("SELECT * FROM user"));
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

	m_list.DeleteAllItems(); // 리스트 초기화

	while ((Sql_Row = mysql_fetch_row(Sql_Result)) != NULL)
	{
		CString id;
		CString pw;
		CString name;
		CString tel;
		CString book;
		CString overdue;

		id += Sql_Row[0];
		pw += Sql_Row[1];
		name += Sql_Row[2];
		tel += Sql_Row[3];
		book += Sql_Row[4];
		overdue += Sql_Row[5];

		m_list.InsertItem(seq, _T(""));
		m_list.SetItem(seq, 0, LVIF_TEXT, id, 0, 0, 0, NULL);
		m_list.SetItem(seq, 1, LVIF_TEXT, pw, 0, 0, 0, NULL);
		m_list.SetItem(seq, 2, LVIF_TEXT, name, 0, 0, 0, NULL);
		m_list.SetItem(seq, 3, LVIF_TEXT, tel, 0, 0, 0, NULL);
		m_list.SetItem(seq, 4, LVIF_TEXT, book, 0, 0, 0, NULL);
		m_list.SetItem(seq, 5, LVIF_TEXT, overdue, 0, 0, 0, NULL);

		seq++;
		seq_string.Format(_T("%d"), seq);
	}
}


void AdminUserDlg::DeleteDB()
{
	UINT uSelectedCount = m_list.GetSelectedCount();
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	vector<int> id;
	int nSelected;

	id.clear();

	if (uSelectedCount <= 0)
	{
		MessageBox(_T("선택된 회원이 없습니다."));
		return;
	}

	while (pos)
	{
		nSelected = m_list.GetNextSelectedItem(pos);
		id.push_back(nSelected);
	}

	for (int i = (int)id.size() - 1; i >= 0; --i)
	{
		CString query;
		query.Format(_T("DELETE FROM user WHERE id='%s'"), m_list.GetItemText(id[i], 0));
		if (mysql_query(&Connect, (CStringA)query))
		{
			MessageBox(_T("Delete Error"));
		}
	}
}


void AdminUserDlg::OnHdnItemClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR); // 리스트컨트롤 칼럼 클릭할 때 데이터 정렬

	// 클릭한 칼럼의 인덱스
	//int nColumn = pNMLV->iSubItem;
	int nColumn = pNMLV->iItem;

	// 현재 리스트 컨트롤에 있는 데이터 총 자료 개수만큼 저장
	for (int i = 0; i < (m_list.GetItemCount()); i++)
	{
		m_list.SetItemData(i, i);
	}

	// 정렬 방식 지정(Ascend, Descend)
	m_bAscending = !m_bAscending;

	// 정렬 관련된 구조체 변수 생성 및 데이터 초기화
	SORTPARAM sortparams;
	sortparams.pList = &m_list;
	sortparams.iSortColumn = nColumn;
	sortparams.bSortDirect = m_bAscending;

	// 정렬 함수 호출
	m_list.SortItems(&CompareItem, (LPARAM)&sortparams);

	*pResult = 0;
}

int CALLBACK AdminUserDlg::CompareItem(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl *pList = ((SORTPARAM *)lParamSort)->pList;
	int iSortColumn = ((SORTPARAM *)lParamSort)->iSortColumn;
	bool bSortDirect = ((SORTPARAM *)lParamSort)->bSortDirect;

	LVFINDINFO info1, info2;
	info1.flags = LVFI_PARAM;
	info1.lParam = lParam1;
	info2.flags = LVFI_PARAM;
	info2.lParam = lParam2;
	int irow1 = pList->FindItem(&info1, -1);
	int irow2 = pList->FindItem(&info2, -1);

	CString strItem1 = pList->GetItemText(irow1, iSortColumn);
	CString strItem2 = pList->GetItemText(irow2, iSortColumn);

	return bSortDirect ? strItem1.Compare(strItem2) : -strItem1.Compare(strItem2);
}
