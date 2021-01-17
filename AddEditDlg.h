﻿#pragma once

#include "AdminBookDlg.h"

// AddEditDlg 대화 상자

class AddEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AddEditDlg)

public:
	AddEditDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~AddEditDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_EDIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAddEditOk();
	CWnd *pParent;
};
