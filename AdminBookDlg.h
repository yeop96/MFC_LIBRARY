#pragma once

#include "AddEditDlg.h"
#include <vector>
using namespace std;


// AdminBookDlg 대화 상자

class AdminBookDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AdminBookDlg)

public:
	AdminBookDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~AdminBookDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADMIN_BOOK };
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
		ID_MENU_DELETE
	};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBookAdd();
	afx_msg void OnButtonBookDelete();
	afx_msg void OnButtonBookEdit();
	afx_msg void OnHdnItemClickList(NMHDR *nMNHDR, LRESULT *pResult);
	static int CALLBACK CompareItem(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void ConnectDB();
	void PrintDB();
	void InsertDB(CString title, CString author, CString publisher, CString isbn, CString quantity);
	void EditDB(CString title, CString author, CString publisher, CString isbn, CString quantity);
	void DeleteDB();
	BOOL editmode;
	afx_msg void OnContextMenu(CWnd * /*pWnd*/, CPoint point);
	virtual BOOL PreTranslateMessage(MSG *pMsg);
};
