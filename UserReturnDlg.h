#pragma once


// UserReturnDlg 대화 상자

class UserReturnDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UserReturnDlg)

public:
	UserReturnDlg(CWnd *pParent = nullptr);   // 표준 생성자입니다.
	virtual ~UserReturnDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USER_RETURN };
#endif
	BOOL m_bAscending;
	struct SORTPARAM
	{
		int iSortColumn;
		bool bSortDirect;
		CListCtrl *pList;
	};

protected:
	virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_list;
	void PrintDB();
	afx_msg void OnButtonBookReturn();
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	int dateCalculation(int year, int month, int day);
	void ConnectDB();
	void ReturnDB();
};
