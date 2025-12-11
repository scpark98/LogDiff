
// LogDiffDlg.h: 헤더 파일
//

#pragma once

#include <deque>
#include "Common/CEdit/RichEditCtrlEx/RichEditCtrlEx.h"

// CLogDiffDlg 대화 상자
class CLogDiffDlg : public CDialogEx
{
// 생성입니다.
public:
	CLogDiffDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	std::deque<CString>				m_files;

	std::deque<CRichEditCtrlEx*>	m_rich;
	void							open_files();

	void							arrange_controls();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGDIFF_DIALOG };
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
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
