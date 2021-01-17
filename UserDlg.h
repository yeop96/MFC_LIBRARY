#pragma once

class UserRentalDlg;
class UserReturnDlg;

// UserDlg 대화 상자

class UserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UserDlg)

public:
	UserDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~UserDlg();

	UserRentalDlg *pUserRentalDlg;
	UserReturnDlg *pUserReturnDlg;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabUser(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG *pMsg);
};
