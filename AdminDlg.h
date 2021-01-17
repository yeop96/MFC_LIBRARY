#pragma once

class AdminBookDlg;
class AdminUserDlg;

// AdminDlg 대화 상자

class AdminDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AdminDlg)

public:
	AdminDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~AdminDlg();
	AdminBookDlg *pAdminBookDlg;
	AdminUserDlg *pAdminUserDlg;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADMIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabAdmin(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG *pMsg);
};
