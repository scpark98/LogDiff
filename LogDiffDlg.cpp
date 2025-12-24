
// LogDiffDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LogDiff.h"
#include "LogDiffDlg.h"
#include "afxdialogex.h"

#include <regex>
#include <thread>

#include "Common/Functions.h"
#include "Common/CEdit/Scintilla/Scintilla_init.h"

#include "DateTimeOffsetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

KeywordStyle styles[] = {
	{"error", INDIC_ERROR0, RGB(220, 50, 47)},
	{"에러", INDIC_ERROR1, RGB(220, 50, 47)},
	{"fail", INDIC_FAIL0, RGB(220, 50, 47)},
	{"실패", INDIC_FAIL1, RGB(220, 50, 47)},
	{"warn",  INDIC_WARN0,  RGB(255, 140, 0)},
	{"경고",  INDIC_WARN1,  RGB(255, 140, 0)},
};


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLogDiffDlg 대화 상자



CLogDiffDlg::CLogDiffDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGDIFF_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLogDiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLogDiffDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDOK, &CLogDiffDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLogDiffDlg::OnBnClickedCancel)
	ON_WM_DROPFILES()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	//ON_REGISTERED_MESSAGE(Message_CRichEditCtrlEx, &CLogDiffDlg::on_message_CRichEditCtrlEx)
	ON_WM_TIMER()
	ON_COMMAND(ID_MENU_DATETIME_SHIFT, &CLogDiffDlg::OnMenuDatetimeShift)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_SAVE, &CLogDiffDlg::OnMenuSave)
	ON_COMMAND(ID_MENU_SAVE_AS, &CLogDiffDlg::OnMenuSaveAs)
	ON_COMMAND(ID_MENU_CURRENT_FOLDER, &CLogDiffDlg::OnMenuCurrentFolder)
	ON_COMMAND(ID_MENU_SORT, &CLogDiffDlg::OnMenuSort)
	ON_COMMAND(ID_MENU_CLOSE_DOC, &CLogDiffDlg::OnMenuCloseDoc)
	ON_COMMAND(ID_MENU_CLOSE_DOC_ALL, &CLogDiffDlg::OnMenuCloseDocAll)
END_MESSAGE_MAP()


// CLogDiffDlg 메시지 처리기

BOOL CLogDiffDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	init_lexilla_module(get_exe_directory(true));

	RestoreWindowPosition(&theApp, this);

	DragAcceptFiles();

	//레지스트리에서 최근 열었던 파일목록을 m_files에 채워준다.
	std::deque<CString> recent_list;
	get_registry_str_list(&theApp, _T("setting\\recent files"), recent_list);
	m_doc.resize(recent_list.size());

	for (int i = 0; i < recent_list.size(); i++)
	{
		m_doc[i].m_file = recent_list[i];
	}

	open_files();

#if 0
	m_doc.push_back(CLogDiffFile(get_exe_directory() + _T("\\test_log_data\\ManualLauncher_20250924.log")));
	m_doc.push_back(CLogDiffFile(get_exe_directory() + _T("\\test_log_data\\ManualLauncher_20250925.log")));
	m_doc.push_back(CLogDiffFile(get_exe_directory() + _T("\\test_log_data\\ManualLauncher_20251105.log")));
	open_files();
#elif 0
	m_doc.push_back(CLogDiffFile(get_exe_directory() + _T("\\test_log_data\\LMMViewer.log")));
	m_doc.push_back(CLogDiffFile(get_exe_directory() + _T("\\test_log_data\\LMMAgentService.log")));
	open_files();
#elif 0
	m_doc.push_back(CLogDiffFile(get_exe_directory() + _T("\\test_log_data\\LiveWebAgent[20251216].log")));
	m_doc.push_back(CLogDiffFile(get_exe_directory() + _T("\\test_log_data\\LivewebClient16.log")));
	open_files();
#endif

	CString str;
	str.Format(_T("LogDiff (ver %s)"), get_file_property());
	SetWindowText(str);

	SetTimer(timer_id_initial_focus, 10, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CLogDiffDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CLogDiffDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CLogDiffDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLogDiffDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this);
}

void CLogDiffDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void CLogDiffDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	release();

	CDialogEx::OnCancel();
}

void CLogDiffDlg::release(int index)
{
	if (index < 0 || index >= m_doc.size())
	{
		for (int i = 0; i < m_doc.size(); i++)
		{
			if (m_doc[i].m_rich)
			{
				m_doc[i].m_rich->DestroyWindow();
				delete m_doc[i].m_rich;
			}

			if (m_doc[i].m_title)
			{
				m_doc[i].m_title->DestroyWindow();
				delete m_doc[i].m_title;
			}

			if (m_doc[i].m_statusbar)
			{
				m_doc[i].m_statusbar->DestroyWindow();
				delete m_doc[i].m_statusbar;
			}

			if (m_doc[i].m_progress)
			{
				m_doc[i].m_progress->DestroyWindow();
				delete m_doc[i].m_progress;
			}
		}

		m_doc.clear();
	}
	else
	{
		if (m_doc[index].m_rich)
		{
			m_doc[index].m_rich->DestroyWindow();
			delete m_doc[index].m_rich;
		}

		if (m_doc[index].m_title)
		{
			m_doc[index].m_title->DestroyWindow();
			delete m_doc[index].m_title;
		}

		if (m_doc[index].m_progress)
		{
			m_doc[index].m_progress->DestroyWindow();
			delete m_doc[index].m_progress;
		}

		m_doc.erase(m_doc.begin() + index);
	}
}

void CLogDiffDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TCHAR sfile[MAX_PATH];
	int count = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);

	if (!IsCtrlPressed())
		release();

	for (int i = 0; i < count; i++)
	{
		DragQueryFile(hDropInfo, i, sfile, MAX_PATH);

		if (PathIsDirectory(sfile))
			continue;

		m_doc.push_back(CLogDiffFile(sfile));
	}

	arranged_by_timestamp = false;
	open_files();

	CDialogEx::OnDropFiles(hDropInfo);
}

void CLogDiffDlg::open_files()
{
	int i;

	for (i = 0; i < m_doc.size(); i++)
	{
		if (m_doc[i].m_rich == NULL)
			open_file(i);
	}

	//파일수에 따라 레이아웃을 재조정한다.
	arrange_layout();

	//현재 문서들의 경로를 레지스트리에 기억한다.
	//반드시 "count"를 리셋시켜 준 후 add_registry_str()를 호출해야 한다.
	theApp.WriteProfileInt(_T("setting\\recent files"), _T("count"), 0);

	for (i = 0; i < m_doc.size(); i++)
	{
		add_registry_str(&theApp, _T("setting\\recent files"), m_doc[i].m_file);
	}
}

void CLogDiffDlg::set_default_styles(CScintillaCtrl* rich)
{
	COLORREF cr_back = ivory;// RGB(255, 0, 0);
	rich->StyleSetFore(STYLE_DEFAULT, get_color_from_hexa_str(_T("DCDCCC")));
	rich->StyleSetBack(STYLE_DEFAULT, get_color_from_hexa_str(_T("3F3F3F")));
	rich->StyleSetSize(STYLE_DEFAULT, 10);
	rich->StyleSetFont(STYLE_DEFAULT, "Consolas");

	//custom keyword color를 사용하므로 Lexer는 사용하지 않는다.
	//rich->SetILexer(m_pCLexer);

	//선택 영역 색상
	rich->SetSelFore(TRUE, get_color_from_hexa_str(_T("3F3F3F")));
	rich->SetSelBack(TRUE, get_color_from_hexa_str(_T("DCDCCC")));

	//caret color
	rich->SetCaretFore(get_color_from_hexa_str(_T("DCDCCC")));
	//rich->SetCaretLineBack(get_color_from_hexa_str(_T("DCDCCC")));

	//라인 앞뒤 간격 픽셀 크기
	rich->SetExtraAscent(1);
	rich->SetExtraDescent(1);

	//rich->ClearCmdKey(SCK_ESCAPE);
	//rich->ClearCmdKey(VK_ESCAPE);
	//rich->ClearAllCmdKeys();

	//StyleClearAll()을 호출해줘야 기본 글자색, 바탕색등이 제대로 표시된다.
	//근데 라인번호 영역의 색 설정은 이 함수 뒤에 해줘야 적용된다??
	rich->StyleClearAll();

	//라인 넘버 글자색, 배경색
	rich->StyleSetFore(STYLE_LINENUMBER, get_color_from_hexa_str(_T("8A8A8A")));
	rich->StyleSetBack(STYLE_LINENUMBER, get_color_from_hexa_str(_T("0C0C0C")));
	//라인 넘버외에 폴드, 마커 영역 모두 제거한다.
	for (int i = 1; i < 5; i++)
	{
		rich->SetMarginWidthN(i, 0);
		rich->SetMarginSensitiveN(i, false);
	}

	rich->SendMessage(SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
	int charWidth = rich->TextWidth(STYLE_LINENUMBER, "9");
	rich->SetMarginWidthN(0, charWidth + 8);

	//rich->SetProperty("fold", "0");
	rich->MarkerDeleteAll(-1);

	rich->UsePopUp(Scintilla::PopUp::Never);


	init_keyword_style(rich);
}

void CLogDiffDlg::init_keyword_style(CScintillaCtrl* rich)
{
	int i;
	int style_count = sizeof(styles) / sizeof(styles[0]);

	for (i = 0; i < style_count; i++)
	{
		//rich->SendMessage(SCI_INDICSETSTYLE, i, INDIC_TEXTFORE);
		rich->IndicSetStyle(i, Scintilla::IndicatorStyle::TextFore);
		//rich->SendMessage(SCI_INDICSETFORE, i, styles[i].cr);
		rich->IndicSetFore(i, styles[i].cr);
	}

	for (i = 0; i < style_count; i++)
		highlight_keyword(rich, i);
}

void CLogDiffDlg::highlight_keyword(CScintillaCtrl* rich, int indicator)
{
	int length = (int)rich->SendMessage(SCI_GETTEXTLENGTH, 0, 0);

	rich->SendMessage(SCI_SETINDICATORCURRENT, indicator, 0);
	rich->SendMessage(SCI_INDICATORCLEARRANGE, 0, length);

	CString all_text = rich->GetText(length);
	CString keyword(styles[indicator].text);

	all_text.MakeLower();
	keyword.MakeLower();
	int pos = 0;
	int line;
	CString str;

	while ((pos = all_text.Find(keyword, pos)) != -1)
	{
		line = rich->LineFromPosition(pos);// SendMessage(SCI_LINEFROMPOSITION, pos, 0);
		str = rich->GetLine(line);
		TRACE(_T("keyword = %s, pos = %d, line = %d, str = %s\n"), keyword, pos, line, str);
		rich->SendMessage(SCI_INDICATORFILLRANGE, pos, keyword.GetLength());
		pos += keyword.GetLength();
	}
}



//m_files에 채워진 파일들을 로딩한다.
void CLogDiffDlg::open_file(int index)
{
	CScintillaCtrl* rich = new CScintillaCtrl();
	rich->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP /*| WS_BORDER*/, CRect(0, 0, 1, 1), this, index);

	//CString str = read(m_doc[index].m_file);
	//rich->SetText(str);
	rich->read_file(m_doc[index].m_file);
	m_doc[index].m_rich = rich;

	set_default_styles(rich);


	//문서 타이틀 표시
	//edit에는 fullpath가 표시되는데 width가 작아졌을 때 word-wrap되므로 ES_AUTOHSCROLL을 줘야 한다.
	CSCEdit* edit = new CSCEdit();
	edit->create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_BORDER | ES_AUTOHSCROLL, CRect(0, 0, 1, 1), this, 1);
	edit->SetFont(GetFont());
	edit->set_font_name(_T("Consolas"));// (_T("Noto Sans KR"));
	edit->set_font_size(10);
	edit->set_font_weight(700);
	edit->set_text(m_doc[index].m_file);
	edit->set_text_color(Gdiplus::Color::RoyalBlue);
	edit->set_back_color(Gdiplus::Color::Ivory);// get_sys_color(COLOR_3DFACE));
	edit->set_use_readonly_color(false);
	edit->set_line_align(DT_VCENTER);
	m_doc[index].m_title = edit;

	CSCStatic* statusbar = new CSCStatic();
	statusbar->create(m_doc[index].m_file, WS_CHILD | WS_VISIBLE, CRect(0, 0, 1, 1), this, 2);
	statusbar->SetFont(GetFont());
	statusbar->set_font_name(_T("Arial"));
	statusbar->set_font_size(8);
	m_doc[index].m_statusbar = statusbar;

	CMacProgressCtrl* progress = new CMacProgressCtrl();
	progress->Create(WS_CHILD, CRect(0, 0, 1, 1), this, 3);
	m_doc[index].m_progress = progress;

	update_status_info(rich);
}

void CLogDiffDlg::arrange_layout()
{
	CRect rc;
	GetClientRect(rc);

	if (m_doc.size() == 0)
		return;

	int gap = 8;
	int edit_height = 32;
	int statusbar_height = 16;
	int progress_height = 6;
	CSize sz_scrollbar(0, rc.Height());
	CRect margin(8, 8, 8, 8);

	int w = (rc.Width() - (m_doc.size() - 1) * gap - margin.left - margin.right - sz_scrollbar.cx) / m_doc.size();

	CRect rtitle = CRect(margin.left, margin.top, margin.left + w, margin.top + edit_height);
	CRect rrich = CRect(margin.left, rtitle.bottom + gap, margin.left + w, rc.bottom - margin.bottom - statusbar_height);
	CRect rstatusbar = CRect(margin.left, rrich.bottom + 0, margin.left + w, rrich.bottom + statusbar_height);
	CRect rprogress = CRect(margin.left, rrich.bottom + 2, margin.left + w, rrich.bottom + 2 + progress_height);

	for (int i = 0; i < m_doc.size(); i++)
	{
		m_doc[i].m_rich->MoveWindow(rrich);
		rrich.OffsetRect(w + gap, 0);

		m_doc[i].m_title->MoveWindow(rtitle);
		rtitle.OffsetRect(w + gap, 0);

		m_doc[i].m_statusbar->MoveWindow(rstatusbar);
		rstatusbar.OffsetRect(w + gap, 0);

		m_doc[i].m_progress->MoveWindow(rprogress);
		rprogress.OffsetRect(w + gap, 0);
	}
}

void CLogDiffDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	arrange_layout();
}

BOOL CLogDiffDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_HSCROLL ||
		pMsg->message == WM_VSCROLL ||
		pMsg->message == WM_MOUSEHWHEEL ||
		pMsg->message == WM_MOUSEWHEEL)
	{
		//TRACE(_T("PreTranslateMessage: pMsg->message = %d\n"), pMsg->message);
		//sync_scroll(pMsg);
	}
	else if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_ESCAPE:
				OnBnClickedCancel();
				return TRUE;
		}
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}

void CLogDiffDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TRACE(_T("OnHScroll: code=%d, pos=%d\n"), nSBCode, nPos);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLogDiffDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TRACE(_T("OnVScroll: code=%d, pos=%d\n"), nSBCode, nPos);

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CLogDiffDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//TRACE(_T("OnHScroll: code=%d, pos=%d\n"), nSBCode, nPos);
	traceonly;

	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}

BOOL CLogDiffDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	traceonly;

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CLogDiffDlg::sync_scroll(MSG* pMsg)
{
	CScintillaCtrl* rich = (CScintillaCtrl*)CWnd::FromHandle(pMsg->hwnd);

	//int hpos = rich->GetScrollPos(SB_HORZ);
	//int vpos = rich->GetScrollPos(SB_VERT);
	int hpos = rich->GetXOffset();
	int vpos = rich->GetFirstVisibleLine();
	TRACE(_T("old hpos=%d, vpos=%d\n"), hpos, vpos);

	for (int i = 0; i < m_doc.size(); i++)
	{
		if (rich == m_doc[i].m_rich)
			continue;

		//if (pMsg->message == WM_MOUSEHWHEEL || pMsg->message == WM_HSCROLL)
			m_doc[i].m_rich->LineScroll(hpos, vpos);
		//else if (pMsg->message == WM_MOUSEWHEEL || pMsg->message == WM_VSCROLL)
		//	::SendMessage(m_doc[i].m_rich->m_hWnd, WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, vpos), (LPARAM)(m_doc[i].m_rich->m_hWnd));
	}
}

class CForSortLog
{
public:
	CForSortLog(int _index = 0, CSCTime _time_stamp = CSCTime(), CString _full_log = _T(""))
	{
		index = _index;
		time_stamp = _time_stamp;
		full_log = _full_log;
	}

	int index;
	CSCTime time_stamp;
	CString full_log;
};

void CLogDiffDlg::arrange_logs_by_timestamp()
{
	//문서 index, time_stamp, full_log를 하나의 객체로 만들어서 리스트에 모두 넣고 시간값으로 정렬시킨다.
	//0 ~ n까지 순차적으로 나타나는지 체크한다.
	//만약 index가 0 1 2로 나타나야 하는데 0 1 1로 나타났다면 2번 문서에 공백을 추가시켜준다.
	//이렇게 끝까지 순차 검사를 완료했다면 i번은 m_rich[i]에 다시 넣어준다.
	//단, 몇 만 라인의 로그일 경우는 Debug 모드에서는 매우 느리며 Release 모드에서는 기다릴 만 하다.
	std::vector<CForSortLog> list;

	int i, j;
	int total_elements = 0;

	for (i = 0; i < m_doc.size(); i++)
	{
		total_elements += m_doc[i].m_rich->GetLineCount();
	}
	list.resize(total_elements);

	int total_index = 0;

	for (i = 0; i < m_doc.size(); i++)
	{
		CSCTime prev_timestamp;

		int total_lines = m_doc[i].m_rich->GetLineCount();
		m_doc[i].m_progress->SetRange(0, total_lines);
		m_doc[i].m_statusbar->ShowWindow(SW_HIDE);
		m_doc[i].m_progress->ShowWindow(SW_SHOW);

		for (j = 0; j < total_lines; j++)
		{
			m_doc[i].m_progress->SetPos(j);

			//만약 공백이나 timestamp가 없는 라인이라면 timestamp는 이전값보다 ms를 1만큼 큰 값으로 변경해준다.
			//그래야만 정렬시에 그 위치가 유지된다.
			CString log = m_doc[i].m_rich->GetLine(j);
			CSCTime time_stamp(log);

			if (j > 0 && time_stamp.is_empty())
			{
				time_stamp = prev_timestamp + CSCTime(0, 0, 0, 0, 0, 0, 1);
			}

			list[total_index++] = CForSortLog(i, time_stamp, log);
			prev_timestamp = time_stamp;
		}

		m_doc[i].m_progress->SetPos(0);
	}


	std::sort(list.begin(), list.end(),
		[](CForSortLog a, CForSortLog b)
		{
			return (a.time_stamp < b.time_stamp);
		});

	/*
	FILE* fp = NULL;
	_tfopen_s(&fp, _T("D:\\log_list_before_adjust.txt"), _T("wt")CHARSET);
	for (i = 0; i < list.size(); i++)
	{
		_ftprintf(fp, _T("%d|%s|%s"), list[i].index, list[i].time_stamp.to_string(), list[i].full_log);
	}

	fclose(fp);
	*/

	//0 ~ n까지 순차적으로 나타나는지 체크한다.
	//만약 index가 0 1 2로 순차적으로 나타나야 하는데 0 1 1이었다면 두번째에 있는 1 뒤에 2번 항목으로 공백을 추가시켜준다.
	//이렇게 끝까지 순차 검사를 완료했다면 각각 인덱스대로 m_content[i]에 다시 넣어준다.
	int sequence = 0;
	i = 0;

	while (true)
	{
		if (list[i].index != sequence)
		{
			if (i > 0)
			{
				//i 자리에 한 라인을 추가하되 그 시간값은 바로 전 항목의 시간값과 동일하게 한다.
				list.insert(list.begin() + i, CForSortLog(sequence, list[i - 1].time_stamp, _T("\n")));
			}
		}

		i++;
		sequence++;

		//m_progress.SetPos(i);

		if (sequence == m_doc.size())
			sequence = 0;

		//index i가 list.size()에 도달하면 종료시킨다.
		if (i >= list.size())
			break;
	}

	/*
	fp = NULL;
	_tfopen_s(&fp, _T("D:\\log_list_after_adjust.txt"), _T("wt")CHARSET);
	for (i = 0; i < list.size(); i++)
	{
		_ftprintf(fp, _T("%d|%s|%s"), list[i].index, list[i].time_stamp.to_string(), list[i].full_log);
	}

	fclose(fp);
	*/

	//위에서 보정된 list를 하나의 문자열로 합치고 다시 rich에 넣어준다.
	//우선 기존 content를 모두 clear하고
	for (i = 0; i < m_doc.size(); i++)
	{
		m_doc[i].m_rich->ClearAll();

		CString new_text;

		for (j = 0; j < list.size(); j++)
		{
			if (list[j].index == i)
				new_text += list[j].full_log;
		}

		m_doc[i].m_rich->SetText(new_text);
		m_doc[i].m_progress->ShowWindow(SW_HIDE);
		m_doc[i].m_statusbar->ShowWindow(SW_SHOW);
	}

	/*
	//각 라인 인덱스를 증가시키면서 timestamp를 추출하고 비교하여 timestamp 순으로 출력시킨다.
	//timestamp가 없는 라인은 빈 라인을 추가해준다.

	int i, j;
	int max_line_count = 0;
	int max_line_index = 0;
	std::deque<int> total_lines(m_rich.size(), 0);

	//로그들 중 최대 라인수를 구하고
	for (i = 0; i < m_rich.size(); i++)
	{
		total_lines[i] = m_content[i].size();
		if (max_line_count < total_lines[i])
		{
			max_line_count = total_lines[i];
			max_line_index = i;
		}

		m_doc[i].m_rich->SetRedraw(FALSE);
	}

	//while문을 돌면서 각 로그의 현재 라인 로그 시각을 추출하고
	//가장 빠른 로그가 아니라면
	std::deque<int> index(m_rich.size(), 0);
	std::deque<CSCTime> timestamp(m_rich.size(), CSCTime());

	//우선 각 로그의 첫번째 로그를 구하고
	for (i = 0; i < m_rich.size(); i++)
		timestamp[i] = CSCTime(m_content[i][index[i]]);

	while (true)
	{
		//n개 중 제일 빠른 timestamp인 항목을 제외한 나머지 항목들의 앞에 '\n'을 추가하고
		//제일 빠른 timestamp의 index를 하나 증가시킨 후 다시 위와 같이 반복.

		//n개 중 가장 늦은 항목을 찾고 그 항목의 index에 '\n'을 추가하고 그 인덱스는 ++,
		//다시 n개 중 가장 늦은 항목을 찾는데 모두 동일하여 없다면
		//동일한 놈들의 인덱스를 ++ 시키고 다시 반복.

		int max_time_index = -1;
		CSCTime max_time;

		//n개 중 가장 늦은 timestamp를 찾고
		for (i = 0; i < m_rich.size(); i++)
		{
			if (timestamp[i] > max_time)
			{
				max_time_index = i;
				max_time = timestamp[i];
			}
		}

		//찾았으면 현재 index에 '\n'을 삽입해주고 index를 증가시킨다. 이 때 timestamp값은 변화없다.
		if (max_time_index >= 0)
		{
			for (i = 0; i < m_rich.size(); i++)
			{
				if (i == max_time_index)
				{
					m_content[max_time_index].insert(m_content[max_time_index].begin() + index[max_time_index], _T("\n"));
					index[max_time_index]++;
				}
				else
				{
					index[i]++;
					timestamp[i] = CSCTime(m_content[i][index[i]]);
				}

			}
		}
		//동일한 값들이라서 max를 못찾았다면 그 항목들의 index를 하나씩 증가시켜주고 timestamp 값도 갱신시킨다.
		else
		{
		}
	}

	for (i = 0; i < m_rich.size(); i++)
	{
		m_doc[i].m_rich->SetRedraw(TRUE);
	}

	*/

	arranged_by_timestamp = true;
}

void CLogDiffDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == timer_id_initial_focus)
	{
		KillTimer(timer_id_initial_focus);

		for (int i = 0; i < m_doc.size(); i++)
		{
			m_doc[i].m_rich->SetSel(-1, 0);
			//Wait(1000);
			//m_rich[0]->LineScroll(0);	//왜 이 함수호출로는 안먹히고 SendMessage()는 동작하는지...
			::SendMessage(m_doc[i].m_rich->m_hWnd, WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, 0), (LPARAM)(m_doc[i].m_rich->m_hWnd));
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

CSCTime CLogDiffDlg::get_time_stamp(CString log_line)
{
	std::regex datetime_pattern("(\\d{4})[-/](\\d{2})[-/](\\d{2}) (\\d{2}):(\\d{2}):(\\d{2}).(\\d{3})");

	std::smatch matches;
	std::string ssline = CString2string(log_line);

	if (std::regex_search(ssline, matches, datetime_pattern))
		return CSCTime(CString(matches[0].str().c_str()));

	return CSCTime();
}

void CLogDiffDlg::OnMenuDatetimeShift()
{
	CDateTimeOffsetDlg dlg;

	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	int index = m_context_menu_doc_index;
	trace(index);
	
	CString line;
	CSCTime time_stamp;
	CSCTime time_first_log;	//맨 첫번째 로그의 시간값. 특정 시간으로 변경 시 1번 로그부터 상대시간값을 더해줘야 한다.
	CSCTime time_current_log;	

	//"yyyy-MM-dd hh:mm:ss.???" 형태가 표준이지만 로그에 따라서
	//날짜 구분자가 '-', '/' 이거나 없을수도 있다.
	//우선 날짜의 '-' 또는 '/'와 시간의 ':'는 반드시 존재하고 날짜와 시간 사이의 공백은 n개이어도 인식된다.
	//std::regex datetime_pattern("([0-9]{4})[-/]([0-9]{2})[-/]([0-9]{2})[ ]*([0-9]{2})[:]([0-9]{2})[:]([0-9]{2})");
	std::regex datetime_pattern("(\\d{4})[-/](\\d{2})[-/](\\d{2}) (\\d{2}):(\\d{2}):(\\d{2}).(\\d{3})");

	int total_lines = m_doc[index].m_rich->GetLineCount();

	for (int i = 0; i < total_lines; i++)
	{
		line = m_doc[index].m_rich->GetLine(i);

		if (line.IsEmpty() || (line.GetLength() == 1 && line.Right(1) == '\n'))
		{
			Trace(_T("%d. empty line\n"), i);
			//한줄단위로 처리되므로 empty line은 그냥 continue만 해도 되지만
			//다른 모든 라인들이 Trim된 상태에서 추가되는 방식이므로 empty line도 Trim된 문자열로 변경시킨다.
			//m_content[index][i] = line;
			continue;
		}

		std::smatch matches;
		std::string ssline = CString2string(line);

		if (std::regex_search(ssline, matches, datetime_pattern))
		{
			Trace(_T("%d. Matched datetime: '%S' src = %s\n"), i, matches[0].str().c_str(), line);

			if (i == 0 && (dlg.m_method == method_specific_datetime))
				time_first_log.from_string(CString(matches[0].str().c_str()));
			else
				time_current_log.from_string(CString(matches[0].str().c_str()));

			//datetime을 추출하고 shift 또는 set 한 후 다시 라인에 반영한다.
			CString prefix(matches.prefix().str().c_str());
			CString suffix(matches.suffix().str().c_str());

			if (dlg.m_method == method_offset_time_shift)
			{
				//현재 로그 시간을 변경한 후 다시 적용시킨다.
				if (dlg.m_sign > 0)
					time_current_log = time_current_log + dlg.m_t;
				else
					time_current_log = time_current_log - dlg.m_t;
			}
			else if(dlg.m_method == method_specific_datetime)
			{
				//0번째 로그 시간과 현재 로그 시간과의 차이를 구하고
				if (i == 0)
				{
					//첫번째 로그는 지정한 특정 시간으로 변경
					time_current_log = dlg.m_t;
				}
				else
				{
					//두번째 이후 로그들은 첫번째 로그와의 시간차이를 구해서 설정된 시간값에 더해준다.
					//현재 로그 시간 = 지정한 특정 시간 + (현재 로그 시간 - 첫번째 로그 시간)
					CSCTime time_diff = time_current_log - time_first_log;
					Trace(_T("cur_stamp = %s, diff = %s\n"), time_current_log.to_string(), time_diff.to_string());
					time_current_log = dlg.m_t + time_diff;
					Trace(_T("new_stamp = %s\n"), time_current_log.to_string());
				}

			}

			//다시 라인에 반영
			CString new_datetime_str = time_current_log.to_string();
			int start = m_doc[index].m_rich->PositionFromLine(i);
			int end = m_doc[index].m_rich->PositionFromLine(i + 1);
			m_doc[index].m_rich->SetTargetStart(start);
			m_doc[index].m_rich->SetTargetEnd(end);
			m_doc[index].m_rich->ReplaceTarget(-1, prefix + new_datetime_str + suffix);
		}
		else
		{
			Trace(_T("%d. No match found in line: %s\n"), i, line);
		}
	}
}

void CLogDiffDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	m_context_menu_hwnd = (CScintillaCtrl*)pWnd;

	auto it = std::find_if(m_doc.begin(), m_doc.end(),
		[&](const CLogDiffFile& doc)
		{
			return (doc.m_rich == m_context_menu_hwnd);
		});

	if (it == m_doc.end())
		return;

	m_context_menu_doc_index = std::distance(m_doc.begin(), it);


	CMenu menu;
	CMenu* pMenu = NULL;

	menu.LoadMenu(IDR_MENU_CONTEXT);
	pMenu = menu.GetSubMenu(0);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CLogDiffDlg::OnMenuSave()
{
	int index = m_context_menu_doc_index;
	if (index < 0 || index >= m_doc.size())
		return;

	m_doc[index].m_rich->save_file(m_doc[index].m_file);

	//CString str = m_doc[index].m_rich->GetText(m_doc[index].m_rich->GetTextLength());
	//save(m_doc[index].m_file, str);
}

void CLogDiffDlg::OnMenuSaveAs()
{
	int index = m_context_menu_doc_index;
	if (index < 0 || index >= m_doc.size())
		return;

	CFileDialog dlg(FALSE, get_part(m_doc[index].m_file, fn_ext), m_doc[index].m_file, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Log Files (*.log)|*.log|All Files (*.*)|*.*||"), this);
	if (dlg.DoModal() != IDOK)
		return;

	m_doc[index].m_file = dlg.GetPathName();
	m_doc[index].m_rich->save_file(m_doc[index].m_file);
	m_doc[index].m_title->set_text(dlg.GetPathName());

	/*
	CString str = m_doc[index].m_rich->GetText(m_doc[index].m_rich->GetTextLength());

	if (save(dlg.GetPathName(), str) == false)
	{
		AfxMessageBox(dlg.GetPathName() + _T("\n\n파일 저장 실패"), MB_ICONSTOP);
		return;
	}

	//파일명 정보를 갱신한다.
	m_doc[index].m_file = dlg.GetPathName();
	m_doc[index].m_title->set_text(dlg.GetPathName());
	*/
}

void CLogDiffDlg::OnMenuCurrentFolder()
{
	int index = m_context_menu_doc_index;
	if (index < 0 || index >= m_doc.size())
		return;

	ShellExecute(NULL, _T("open"), _T("explorer"), _T("/select,") + m_doc[index].m_file, NULL, SW_SHOW);
}

void CLogDiffDlg::OnMenuSort()
{
	std::thread th(&CLogDiffDlg::arrange_logs_by_timestamp, this);
	th.detach();
}

void CLogDiffDlg::OnMenuCloseDoc()
{
	int index = m_context_menu_doc_index;
	if (index < 0 || index >= m_doc.size())
		return;

	release(index);
	arrange_layout();
}

void CLogDiffDlg::OnMenuCloseDocAll()
{
	release();
	arrange_layout();
}

//
void CLogDiffDlg::update_status_info(CScintillaCtrl* rich)
{
	int pos = rich->GetCurrentPos();
	int line = rich->LineFromPosition(pos);
	int line_start = rich->PositionFromLine(line);
	int column = pos - line_start;//rich->GetColumn(pos) + 1;
	int total_lines = rich->GetLineCount();

	line++;
	line_start++;
	column++;
	pos++;

	for (int i = 0; i < m_doc.size(); i++)
	{
		if (rich == m_doc[i].m_rich && m_doc[i].m_statusbar)
		{
			CString str;
			str.Format(_T("line: %d / %d      col: %d      pos: %s      %s"), line, total_lines, column, i2S(pos, true), m_doc[i].m_rich->get_encoding_str());
			m_doc[i].m_statusbar->set_text(str);
			break;
		}
	}

	//TRACE(_T("line = %d (line_start = %d), column = %d, pos = %d\n"), line, line_start, column, pos);
}

BOOL CLogDiffDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
#pragma warning(suppress: 26490)
	const NotifyHeader* pNMHdr{ reinterpret_cast<NotifyHeader*>(lParam) };
#pragma warning(suppress: 26496)
	AFXASSUME(pNMHdr != nullptr);

	auto pSCNotification{ reinterpret_cast<NotificationData*>(lParam) };
	SCNotification* scn = (SCNotification*)pNMHdr;
	CScintillaCtrl* rich = (CScintillaCtrl*)(CWnd::FromHandle((HWND)(scn->nmhdr.hwndFrom)));

	//TRACE(_T("scn->nmhdr.code = %d, scn->ch = %d\n"), scn->nmhdr.code, scn->ch);
	//Notify()를 통해 esc키 종료 처리를 하려 했으나
	//모든 키를 무시하도록 설정하면 여기에서 esc를 처리할 수 있으나 에디터의 기본 편집 관련키도 모두 무시되고
	//esc키만 무시하려 해도 되지 않았다.
	//결국 CScintillaCtrl::PreTranslateMessage()에서 esc키를 그대로 통과시키고
	//CTestScintillaDlg::PreTranslateMessage()에서 처리하도록 함.
	if (scn->nmhdr.code == SCN_CHARADDED)
	{
		if (scn->ch == VK_ESCAPE)
		{
			OnBnClickedCancel();
			return TRUE;
		}
	}
	else if (scn->nmhdr.code == SCN_PAINTED)
	{
		if (!rich->is_initial_loading_completed())
		{
			rich->initial_loading_completed(true);
			TRACE(_T("document_loading completed\n"));

			rich->ClearSelections();
			rich->ScrollToStart();
		}
	}
	else if (scn->nmhdr.code == SCN_UPDATEUI)
	{
		auto* notify = (SCNotification*)pNMHdr;
		trace(notify->updated);

		if ((notify->updated > SC_UPDATE_NONE) && (notify->updated % 2 == 0))
			update_status_info(rich);

		//scroll sync와는 무관하게 여기서 처리해야 할 기능은
		//라인넘버 최대 자릿수에 따라 라인넘버 영역의 width가 자동 조정되어야 한다.
		int first = rich->GetFirstVisibleLine();
		int last = rich->LinesOnScreen();
		CString sdigit = i2S(first + last);
		int charWidth = rich->TextWidth(STYLE_LINENUMBER, "9");
		rich->SetMarginWidthN(0, sdigit.GetLength() * charWidth + 8);


		if (m_scroll_syncing)
			return __super::OnNotify(wParam, lParam, pResult);


		if ((notify->updated != SC_UPDATE_V_SCROLL) && (notify->updated != SC_UPDATE_H_SCROLL) && (notify->updated != 6))
			return __super::OnNotify(wParam, lParam, pResult);


		if (m_scroll_syncing || !arranged_by_timestamp)
			return __super::OnNotify(wParam, lParam, pResult);

		CScintillaCtrl* src = nullptr;

		for (auto doc : m_doc)
		{
			if (doc.m_rich->GetSafeHwnd() == notify->nmhdr.hwndFrom)
			{
				src = doc.m_rich;
				break;
			}
		}

		if (!src)
			return __super::OnNotify(wParam, lParam, pResult);

		m_scroll_syncing = true;

		// === Vertical ===
		int srcLine = src->GetFirstVisibleLine();

		for (auto doc : m_doc)
		{
			int dstLine = doc.m_rich->GetFirstVisibleLine();
			int delta = srcLine - dstLine;

			CString sdigit = i2S(dstLine);
			int charWidth = rich->TextWidth(STYLE_LINENUMBER, "9");
			rich->SetMarginWidthN(0, sdigit.GetLength() * charWidth + 8);

			if (doc.m_rich == src) continue;

			if (delta != 0)
				doc.m_rich->LineScroll(0, delta);
		}

		// === Horizontal (optional) ===
		int x = src->GetXOffset();
		for (auto doc : m_doc)
		{
			if (doc.m_rich == src) continue;
			doc.m_rich->SetXOffset(x);
		}

		m_scroll_syncing = false;
		*pResult = 0;
	}
	/*
	switch (pNMHdr->code)
	{
		case Notification::Key:
		{
			if (pSCNotification->ch == VK_ESCAPE)
				OnBnClickedCancel();
			break;
		}
		case Notification::StyleNeeded:
		{
	#pragma warning(suppress: 26486)
			//m_edit->OnStyleNeeded(pSCNotification);
			break;
		}
	}
	*/

	return __super::OnNotify(wParam, lParam, pResult);
}
