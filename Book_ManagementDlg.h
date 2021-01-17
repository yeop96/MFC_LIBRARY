
// Book_ManagementDlg.h: 헤더 파일
#include "UserDlg.h"
#include "AdminDlg.h"
#include "SignupDlg.h"
//

#pragma once


// CBookManagementDlg 대화 상자
class CBookManagementDlg : public CDialogEx
{
// 생성입니다.
public:
	CBookManagementDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonUser();
	afx_msg void OnButtonAdmin();
	afx_msg void OnButtonSignup();
	afx_msg void OnButtonLogin();
	CString m_strID;
	CString m_strPW;
	CButton m_rUser;
	CStatic m_imgBg;
	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG *pMsg);
};
