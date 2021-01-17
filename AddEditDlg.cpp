// AddEditDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Book_Management.h"
#include "AddEditDlg.h"
#include "afxdialogex.h"


// AddEditDlg 대화 상자

IMPLEMENT_DYNAMIC(AddEditDlg, CDialogEx)

AddEditDlg::AddEditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADD_EDIT, pParent)
{
	this->pParent = pParent;
}

AddEditDlg::~AddEditDlg()
{
}

void AddEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AddEditDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD_OK, &AddEditDlg::OnButtonAddEditOk)
END_MESSAGE_MAP()


// AddEditDlg 메시지 처리기

BOOL AddEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_hIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ICON_BOOK)); // favicon
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CString title;
	CString author;
	CString publisher;
	CString isbn;
	CString quantity;

	if (((AdminBookDlg *)pParent)->editmode == TRUE)
	{
		SetWindowText(_T("도서 수정"));

		UINT uSelectedCount = ((AdminBookDlg *)pParent)->m_list.GetSelectedCount();
		POSITION pos = ((AdminBookDlg *)pParent)->m_list.GetFirstSelectedItemPosition();
		int nSelected = ((AdminBookDlg *)pParent)->m_list.GetNextSelectedItem(pos);

		if (uSelectedCount <= 0)
		{
			MessageBox(_T("선택된 책이 없습니다."));
			this->EndDialog(IDNO);
			return TRUE;
		}
		else if (uSelectedCount > 1)
		{
			MessageBox(_T("한 개만 선택해주세요."));
			this->EndDialog(IDNO);
			return TRUE;
		}

		title = ((AdminBookDlg *)pParent)->m_list.GetItemText(nSelected, 0);
		author = ((AdminBookDlg *)pParent)->m_list.GetItemText(nSelected, 1);
		publisher = ((AdminBookDlg *)pParent)->m_list.GetItemText(nSelected, 2);
		isbn = ((AdminBookDlg *)pParent)->m_list.GetItemText(nSelected, 3);
		quantity = ((AdminBookDlg *)pParent)->m_list.GetItemText(nSelected, 4);

		SetDlgItemText(IDC_EDIT_TITLE, title);
		SetDlgItemText(IDC_EDIT_AUTHOR, author);
		SetDlgItemText(IDC_EDIT_PUBLISHER, publisher);
		SetDlgItemText(IDC_EDIT_ISBN, isbn);
		SetDlgItemText(IDC_EDIT_QUANTITY, quantity);

		GetDlgItem(IDC_EDIT_ISBN)->SendMessage(EM_SETREADONLY, 1, 0);
	}
	else
	{
		SetWindowText(_T("도서 추가"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void AddEditDlg::OnButtonAddEditOk()
{
	CString title;
	CString author;
	CString publisher;
	CString isbn;
	CString quantity;

	GetDlgItemText(IDC_EDIT_TITLE, title);
	GetDlgItemText(IDC_EDIT_AUTHOR, author);
	GetDlgItemText(IDC_EDIT_PUBLISHER, publisher);
	GetDlgItemText(IDC_EDIT_ISBN, isbn);
	GetDlgItemText(IDC_EDIT_QUANTITY, quantity);

	// 빈 칸이 있는 경우
	if (title.IsEmpty() && author.IsEmpty() && publisher.IsEmpty() && isbn.IsEmpty() && quantity.IsEmpty())
	{
		MessageBox(TEXT("모든 항목을 입력해주세요."), TEXT("시스템"), MB_OK);
		return;
	}

	if (((AdminBookDlg *)pParent)->editmode == TRUE)
		((AdminBookDlg *)pParent)->EditDB(title, author, publisher, isbn, quantity);
	else
		((AdminBookDlg *)pParent)->InsertDB(title, author, publisher, isbn, quantity);

	((AdminBookDlg *)pParent)->PrintDB();

	this->EndDialog(IDOK);
}
