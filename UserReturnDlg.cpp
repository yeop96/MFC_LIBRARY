// UserReturnDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Book_Management.h"
#include "UserReturnDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <iostream>
using namespace std;


// UserReturnDlg 대화 상자

IMPLEMENT_DYNAMIC(UserReturnDlg, CDialogEx)

CTime date = CTime::GetCurrentTime();

UserReturnDlg::UserReturnDlg(CWnd *pParent /*=nullptr*/)
	: CDialogEx(IDD_USER_RETURN, pParent)
{

}

UserReturnDlg::~UserReturnDlg()
{
}

void UserReturnDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RETURN, m_list);
}


BEGIN_MESSAGE_MAP(UserReturnDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RETURN, &UserReturnDlg::OnButtonBookReturn)
END_MESSAGE_MAP()


// UserReturnDlg 메시지 처리기
void UserReturnDlg::PrintDB()
{
	CString query;
	query.Format(_T("SELECT * FROM rental where user_id='%s'"), id_static);
	int seq = 0;
	CString seq_string;
	seq_string.Format(_T("%d"), seq);

	if (mysql_query(&Connect, (CStringA)query))
	{

		TRACE("2Connection error %d: %s\n", mysql_errno(&Connect), mysql_error(&Connect));
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

		CString bookname;
		CString bookauthor;
		CString bookpublisher;
		CString bookid;
		CString bookRentday;
		CString bookReturnday;

		bookname += Sql_Row[1];
		bookauthor += Sql_Row[2];
		bookpublisher += Sql_Row[3];
		bookid += Sql_Row[4];
		bookRentday += Sql_Row[5];
		bookReturnday += Sql_Row[6];


		m_list.InsertItem(seq, _T(""));
		m_list.SetItem(seq, 0, LVIF_TEXT, bookname, 0, 0, 0, NULL);
		m_list.SetItem(seq, 1, LVIF_TEXT, bookauthor, 0, 0, 0, NULL);
		m_list.SetItem(seq, 2, LVIF_TEXT, bookpublisher, 0, 0, 0, NULL);
		m_list.SetItem(seq, 3, LVIF_TEXT, bookid, 0, 0, 0, NULL);
		m_list.SetItem(seq, 4, LVIF_TEXT, bookRentday, 0, 0, 0, NULL);
		m_list.SetItem(seq, 5, LVIF_TEXT, bookReturnday, 0, 0, 0, NULL);

		seq++;
		seq_string.Format(_T("%d"), seq);
	}
}

BOOL UserReturnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;
	m_list.GetClientRect(rect);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, _T("도서 제목"), LVCFMT_LEFT, int(rect.Width() * 0.2), -1);
	m_list.InsertColumn(1, _T("저자"), LVCFMT_LEFT, int(rect.Width() * 0.1), -1);
	m_list.InsertColumn(2, _T("출판사"), LVCFMT_LEFT, int(rect.Width() * 0.1), -1);
	m_list.InsertColumn(3, _T("ISBN"), LVCFMT_LEFT, int(rect.Width() * 0.2), -1);
	m_list.InsertColumn(4, _T("대여날"), LVCFMT_LEFT, int(rect.Width() * 0.2), -1);
	m_list.InsertColumn(5, _T("반납 기한"), LVCFMT_LEFT, int(rect.Width() * 0.2), -1);


	PrintDB();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}





void UserReturnDlg::OnButtonBookReturn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	ReturnDB();
	PrintDB();
}

void UserReturnDlg::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
void UserReturnDlg::ConnectDB()
{
	mysql_init(&Connect); // 초기화
	if (!mysql_real_connect(&Connect, "localhost", DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0))
	{
		MessageBox(TEXT("DB 접속에 실패했습니다.\n응용 프로그램을 시작할 수 없습니다."), TEXT("접속 에러"), MB_OK);
		TRACE("Connection error %d: %s\n", mysql_errno(&Connect), mysql_error(&Connect));
	}
	else
	{
		mysql_query(&Connect, "set names euckr");
		TRACE("DB 연결 성공");
	}
}


int UserReturnDlg::dateCalculation(int year, int month, int day)
{//전체날짜->일 수로 변환
	int sum = 0;
	int dayMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	for (int i = 0; i < month; i++)
	{
		sum += dayMonth[i];
	}
	sum += (year * 365) + day;

	return sum;

}


void UserReturnDlg::ReturnDB()
{
	int currentday, rentday;
	int nCount = m_list.GetItemCount();
	vector<CString> bookname;
	vector<CString> bookreturn;

	vector<int> id;
	int nSelected;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	UINT uSelectedCount = m_list.GetSelectedCount();
	if (uSelectedCount <= 0)
	{
		MessageBox(_T("선택된 책이 없습니다."));
		return;
	}

	while (pos)
	{
		nSelected = m_list.GetNextSelectedItem(pos);
		id.push_back(nSelected);
	}

	for (int i = (int)id.size() - 1; i >= 0; --i)
	{
		bookname.push_back(m_list.GetItemText(id[i], 0));
		bookreturn.push_back(m_list.GetItemText(id[i], 5));
	}

	/*for (int i = nCount - 1; i >= 0; i--)
	{
		if (m_list.GetItemState(i, LVIS_SELECTED) != 0)
		{
			bookname.push_back(m_list.GetItemText(i, 0));
			bookreturn.push_back(m_list.GetItemText(i, 5));
		}
		else
		{
			MessageBox(_T("선택된 도서가 없습니다."));
			return;
		}
	}*/

	//반납 날짜 CsTRING->int로 변환




	while (!bookname.empty())
	{
		CString query1, query2, query3, query4;
		int cnt = 0;
		int year = 0, month = 0, day = 0;
		for (int j = 0; j < bookreturn[bookreturn.size() - 1].GetLength(); j++)
		{//날짜계산
			if (j < 4)
			{
				year = (year * 10) + (bookreturn[bookreturn.size() - 1][j] - '0');
			}
			else if (4 < j && j < 7)
			{
				month = (month * 10) + (bookreturn[bookreturn.size() - 1][j] - '0');
			}
			else if (7 < j)
			{
				day = (day * 10) + (bookreturn[bookreturn.size() - 1][j] - '0');
			}
		}

		//일수 계산 함수
		rentday = dateCalculation(year, month, day);//반납날짜
		currentday = dateCalculation(date.GetYear(), date.GetMonth(), date.GetDay());//현재날짜




		if (currentday > rentday)
		{//현재 날짜보다 반납 날짜가 적으면 메시지창

			query1.Format(_T("DELETE FROM rental WHERE title = '%s'"), bookname.back());
			query2.Format(_T("UPDATE USER set book = book-1 where id ='%s'"), id_static);
			query3.Format(_T("UPDATE BOOK set quantity=quantity+1 where title ='%s'"), bookname.back());
			query4.Format(_T("UPDATE USER set overdue = overdue+1 where id ='%s'"), id_static);
			cnt++;
			MessageBox(_T("'" + bookname.back() + "' 책의" + "반납 기간을 초과하였습니다"));
		}
		else
		{
			query1.Format(_T("DELETE FROM rental WHERE title = '%s'"), bookname.back());
			query2.Format(_T("UPDATE USER set book = book-1 where id ='%s'"), id_static);
			query3.Format(_T("UPDATE BOOK set quantity=quantity+1 where title ='%s'"), bookname.back());
			MessageBox(_T("반납이 완료되었습니다."));
		}

		if (mysql_query(&Connect, (CStringA)query1))
		{

			//MessageBox(_T("Delete Error"));
		}
		if (mysql_query(&Connect, (CStringA)query2))
		{

			//MessageBox(_T("update Error."));
		}
		if (mysql_query(&Connect, (CStringA)query3))
		{

			MessageBox(_T("update Error."));
		}
		if (mysql_query(&Connect, (CStringA)query4) && cnt > 0)
		{

			MessageBox(_T("update Error."));
		}
		bookname.pop_back();
		bookreturn.pop_back();
	}

}
