#pragma once


// UserRentalDlg 대화 상자

class UserRentalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UserRentalDlg)

public:
	UserRentalDlg(CWnd *pParent = nullptr);   // 표준 생성자입니다.
	virtual ~UserRentalDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USER_RENTAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 지원입니다.

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedListBooks(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_BookList;
	CString m_strSearch;
	afx_msg void OnBntClickedButtonSearch();
	void ConnectDB();
	void PrintDB(CString strQ);
	BOOLEAN OverlapDB(CString isbn); // 중복 대여 함수
	BOOLEAN LimitDB(); // 권수 제한 함수
	CButton m_CheckAuthor;
	CButton m_CheckPublisher;
	CButton m_CheckTitle;
	CWnd *pParent;
	afx_msg void OnClickListBooks(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG *pMsg);
};
