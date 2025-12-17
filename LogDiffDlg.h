
// LogDiffDlg.h: 헤더 파일
//

#pragma once

#include <deque>
#include "Common/CEdit/SCEdit/SCEdit.h"
#include "Common/CEdit/RichEditCtrlEx/RichEditCtrlEx.h"
#include "Common/data_types/CSCTime/SCTime.h"
#include "Common/CProgressCtrl/MacProgressCtrl/MacProgressCtrl.h"

class CLogDiffFile
{
public:
	CLogDiffFile(CString file, CSCEdit* title = NULL, CRichEditCtrlEx* rich = NULL)
		: m_file(file), m_title(title), m_rich(rich)
	{
	}

	CString				m_file;
	CSCEdit*			m_title = NULL;
	CRichEditCtrlEx*	m_rich = NULL;
	CMacProgressCtrl*	m_progress = NULL;
	std::deque<CString>	m_content;
};

// CLogDiffDlg 대화 상자
class CLogDiffDlg : public CDialogEx
{
// 생성입니다.
public:
	CLogDiffDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	//std::deque<CString>				m_files;
	//std::deque<CSCEdit*>			m_title;		//문서 타이틀
	//std::deque<CRichEditCtrlEx*>	m_rich;
	//std::deque<std::deque<CString>>	m_content;		//

	std::deque<CLogDiffFile>		m_doc;

	CRichEditCtrlEx*				m_context_menu_hwnd = NULL;
	int								m_context_menu_doc_index = -1;

	enum TIMER_ID
	{
		timer_id_initial_focus = 0,
	};

	void							open_file(int index);
	void							thread_parse_log(int index);

	void							release(int index = -1);

	void							arrange_layout();

	CSCTime							get_time_stamp(CString log_line);

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
	afx_msg void OnMenuSave();
	afx_msg void OnMenuSaveAs();
	afx_msg void OnMenuCurrentFolder();
	afx_msg void OnMenuSort();
	afx_msg void OnMenuCloseDoc();
	afx_msg void OnMenuCloseDocAll();
	CMacProgressCtrl m_progress;
};
