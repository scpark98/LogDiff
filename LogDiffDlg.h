
// LogDiffDlg.h: 헤더 파일
//

#pragma once

#include <deque>
#include "Common/CStatic/SCStatic/SCStatic.h"
#include "Common/CEdit/SCEdit/SCEdit.h"
#include "Common/CEdit/Scintilla/ScintillaCtrl.h"
#include "Common/data_types/CSCTime/SCTime.h"
#include "Common/CProgressCtrl/MacProgressCtrl/MacProgressCtrl.h"

using namespace Scintilla;

enum {
	INDIC_ERROR0 = 0,
	INDIC_ERROR1,
	INDIC_FAIL0,
	INDIC_FAIL1,
	INDIC_WARN0,
	INDIC_WARN1,
};

struct KeywordStyle {
	const char* text;
	int indicator;
	COLORREF cr;
};

class CLogDiffFile
{
public:
	CLogDiffFile(CString file = _T(""), CSCEdit* title = NULL, CScintillaCtrl* rich = NULL)
		: m_file(file), m_title(title), m_rich(rich)
	{
	}

	CString				m_file;
	CSCEdit*			m_title = NULL;
	CScintillaCtrl*		m_rich = NULL;
	CSCStatic*			m_statusbar = NULL;
	CMacProgressCtrl*	m_progress = NULL;
};

// CLogDiffDlg 대화 상자
class CLogDiffDlg : public CDialogEx
{
// 생성입니다.
public:
	CLogDiffDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	std::deque<CLogDiffFile>		m_doc;

	CScintillaCtrl*					m_context_menu_hwnd = NULL;
	int								m_context_menu_doc_index = -1;

	enum TIMER_ID
	{
		timer_id_initial_focus = 0,
		timer_sync_scroll,
	};

	//m_file에 명시된 파일들을 순차적으로 open한다.
	void							open_files();
	//m_file의 각 파일들을 개별 open
	void							open_file(int index);
	void							set_default_styles(CScintillaCtrl* rich);
	void							init_keyword_style(CScintillaCtrl* rich);
	void							highlight_keyword(CScintillaCtrl* rich, int indicator);


	void							arrange_layout();
	void							release(int index = -1);

	CSCTime							get_time_stamp(CString log_line);

	bool							arranged_by_timestamp = false;
	void							arrange_logs_by_timestamp();

	//윈도우 기본 scroll event로는 제대로 catch되지 않으므로 OnNotify()에서 동기화한다.
	//단, arrange_logs_by_timestamp()가 호출되기 전 상태라면 동기화시키지 않는다.
	bool							m_scroll_syncing = false;

	void							sync_scroll(MSG* pMsg);
	LRESULT							on_message_CRichEditCtrlEx(WPARAM wParam, LPARAM lParam);

	void							update_status_info(CScintillaCtrl* rich);

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
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) override;
};
