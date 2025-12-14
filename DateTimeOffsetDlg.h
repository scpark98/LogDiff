#pragma once
#include "afxdialogex.h"

#include "Common/CEdit/SCEdit/SCEdit.h"
#include "Common/data_types/CSCTime/SCTime.h"

enum OFFSET_METHOD
{
	method_offset_time_shift = 0,
	method_specific_datetime = 1,
};

// CDateTimeOffsetDlg 대화 상자

class CDateTimeOffsetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDateTimeOffsetDlg)

public:
	CDateTimeOffsetDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDateTimeOffsetDlg();

	int				m_method;	// 0: 시간 오프셋, 1: 특정 날짜/시간
	int				m_sign = 1; // time_shift 값이 양수인지 음수인지. datetime 지정시에는 사용안함.
	CSCTime			m_t;

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
	CSCEdit m_edit_time_shift;
	CButton m_radio_datetime;
	CDateTimeCtrl m_datetime;
	afx_msg void OnBnClickedRadioTimeShift();
	afx_msg void OnBnClickedRadioDatetime();
	virtual BOOL OnInitDialog();
};
