#pragma once
#include "afxdialogex.h"


// CDateTimeOffsetDlg 대화 상자

class CDateTimeOffsetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDateTimeOffsetDlg)

public:
	CDateTimeOffsetDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDateTimeOffsetDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATETIME_OFFSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CButton m_radio_time_shift;
	CEdit m_edit_time_shift;
	CButton m_radio_datetime;
	CDateTimeCtrl m_date;
	CDateTimeCtrl m_time;
	afx_msg void OnBnClickedRadioTimeShift();
	afx_msg void OnBnClickedRadioDatetime();
};
