#pragma once

#include <vector>
#include "SignupDlg.h"
using namespace std;

// AdminUserDlg 대화 상자

class AdminUserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AdminUserDlg)

public:
	AdminUserDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~AdminUserDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADMIN_USER };
#endif

	BOOL m_bAscending;
	struct SORTPARAM
	{
		int iSortColumn;
		bool bSortDirect;
		CListCtrl *pList;
	};

	enum
	{
		ID_MENU_BASE = 2000,
		ID_MENU_ADD = ID_MENU_BASE,
		ID_MENU_EDIT,
		ID_MENU_DELETE,
		ID_MENU_RESET
	};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void PrintDB();
	virtual BOOL OnInitDialog();
	CListCtrl m_list;
	afx_msg void OnButtonUserAdd();
	afx_msg void OnButtonUserEdit();
	afx_msg void OnButtonUserDelete();
	afx_msg void OnContextMenu(CWnd * /*pWnd*/, CPoint point);
	afx_msg void OnHdnItemClickList(NMHDR *nMNHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	static int CALLBACK CompareItem(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void ConnectDB();
	void DeleteDB();
	BOOL editmode;
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	void OnButtonUserReset();
};
