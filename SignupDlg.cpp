// SignupDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Book_Management.h"
#include "SignupDlg.h"
#include "afxdialogex.h"

MYSQL_ROW Sql_Row;
MYSQL_RES *Sql_Result;
MYSQL Connect;


// SignupDlg 대화 상자

IMPLEMENT_DYNAMIC(SignupDlg, CDialogEx)

SignupDlg::SignupDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIGNUP, pParent)
	, m_strID(_T(""))
	, m_strPW(_T(""))
	, m_strCPW(_T(""))
	, m_strName(_T(""))
	, m_strTel(_T(""))
{
	this->pParent = pParent;
	pParent->GetWindowText(parentTitle);
}

SignupDlg::~SignupDlg()
{
}

void SignupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIGNUP_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_SIGNUP_PW, m_strPW);
	DDX_Text(pDX, IDC_EDIT_SIGNUP_CPW, m_strCPW);
	DDX_Text(pDX, IDC_EDIT_SIGNUP_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_SIGNUP_PHONE, m_strTel);
}


BEGIN_MESSAGE_MAP(SignupDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SIGNUP_OK, &SignupDlg::OnButtonSignupOk)
	ON_BN_CLICKED(IDC_BUTTON_SIGNUP_CANCLE, &SignupDlg::OnButtonSignupCancle)
END_MESSAGE_MAP()


// SignupDlg 메시지 처리기
BOOL SignupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_hIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ICON_BOOK)); // favicon
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	if (parentTitle.Compare("관리자") == 1) // 열린 창이 관리자가 아니면(관리자면 -1)
		return TRUE;

	CString id;
	CString pw;
	CString name;
	CString tel;

	if (((AdminUserDlg *)pParent)->editmode)
	{
		SetWindowText(_T("회원 수정"));

		UINT uSelectedCount = ((AdminUserDlg *)pParent)->m_list.GetSelectedCount();
		POSITION pos = ((AdminUserDlg *)pParent)->m_list.GetFirstSelectedItemPosition();
		int nSelected = ((AdminUserDlg *)pParent)->m_list.GetNextSelectedItem(pos);

		if (uSelectedCount <= 0)
		{
			MessageBox(_T("선택된 회원이 없습니다."));
			this->EndDialog(IDNO);
			return TRUE;
		}
		else if (uSelectedCount > 1)
		{
			MessageBox(_T("한 개만 선택해주세요."));
			this->EndDialog(IDNO);
			return TRUE;
		}

		id = ((AdminUserDlg *)pParent)->m_list.GetItemText(nSelected, 0);
		pw = ((AdminUserDlg *)pParent)->m_list.GetItemText(nSelected, 1);
		name = ((AdminUserDlg *)pParent)->m_list.GetItemText(nSelected, 2);
		tel = ((AdminUserDlg *)pParent)->m_list.GetItemText(nSelected, 3);

		SetDlgItemText(IDC_EDIT_SIGNUP_ID, id);
		SetDlgItemText(IDC_EDIT_SIGNUP_PW, pw);
		SetDlgItemText(IDC_EDIT_SIGNUP_CPW, pw);
		SetDlgItemText(IDC_EDIT_SIGNUP_NAME, name);
		SetDlgItemText(IDC_EDIT_SIGNUP_PHONE, tel);

		GetDlgItem(IDC_EDIT_SIGNUP_ID)->SendMessage(EM_SETREADONLY, 1, 0);
		GetDlgItem(IDC_EDIT_SIGNUP_NAME)->SendMessage(EM_SETREADONLY, 1, 0);
	}
	else
	{
		SetWindowText(_T("회원가입"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void SignupDlg::OnButtonSignupOk()
{
	UpdateData(TRUE);

	// 빈칸이 있을 경우
	if ((CStringA)m_strID == "" || (CStringA)m_strPW == "" || (CStringA)m_strCPW == "" || (CStringA)m_strName == "" || (CStringA)m_strTel == "")
	{
		MessageBox(TEXT("모든 항목을 입력해주세요."), TEXT("회원가입 실패"), MB_OK);
		return;
	}
	// 비밀번호 확인
	else if ((CStringA)m_strPW != (CStringA)m_strCPW)
	{
		MessageBox(TEXT("비밀번호를 확인해주세요."), TEXT("회원가입 실패"), MB_OK);
		return;
	}

	mysql_init(&Connect);
	mysql_real_connect(&Connect, MY_IP, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	mysql_query(&Connect, "set names euckr");

	CString query;

	if (parentTitle.Compare("관리자") == -1 && ((AdminUserDlg *)pParent)->editmode)
	{
		query.Format(_T("UPDATE user SET pw='%s', tel='%s' WHERE id='%s'"), m_strPW, m_strTel, m_strID);
	}
	else
	{
		query.Format(_T("SELECT * FROM user WHERE id='%s'"), m_strID);
		mysql_query(&Connect, (CStringA)query);

		Sql_Result = mysql_store_result(&Connect);

		while ((Sql_Row = mysql_fetch_row(Sql_Result)) != NULL)
		{
			if (((CString)Sql_Row[0]).Compare(m_strID) == 0)
			{
				MessageBox(TEXT("이미 존재하는 아이디입니다."), TEXT("회원가입 실패"), MB_OK);
				return;
			}
		}
		query.Format(_T("INSERT INTO user (id, pw, name, tel, book, overdue) VALUES ('%s', '%s', '%s', '%s', 0, 0)"), m_strID, m_strPW, m_strName, m_strTel);
	}
	mysql_query(&Connect, (CStringA)query);

	if (parentTitle.Compare("관리자") == -1)
		((AdminUserDlg *)pParent)->PrintDB();

	mysql_close(&Connect);
	this->EndDialog(IDOK);
}

void SignupDlg::OnButtonSignupCancle()
{
	this->EndDialog(IDNO);
}
