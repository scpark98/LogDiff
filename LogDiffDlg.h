
// LogDiffDlg.h: 헤더 파일
//

#pragma once

#include <deque>
#include "Common/CEdit/SCEdit/SCEdit.h"
#include "Common/CEdit/RichEditCtrlEx/RichEditCtrlEx.h"

// CLogDiffDlg 대화 상자
class CLogDiffDlg : public CDialogEx
{
// 생성입니다.
public:
	CLogDiffDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	std::deque<CString>				m_files;

	std::deque<CSCEdit*>			m_title;		//문서 타이틀
	std::deque<CRichEditCtrlEx*>	m_rich;

	std::deque<std::deque<CString>>	m_content;		//

	enum TIMER_ID
	{
		timer_id_initial_focus = 0,
	};

	void							open_files();
	void							release();

	void							arrange_controls();
	void							arrange_logs_by_timestamp();

	void							sync_scroll(MSG* pMsg);
	LRESULT							on_message_CRichEditCtrlEx(WPARAM wParam, LPARAM lParam);

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMenuDatetimeShift();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
