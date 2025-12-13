
// LogDiffDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LogDiff.h"
#include "LogDiffDlg.h"
#include "afxdialogex.h"

#include <regex>
#include "Common/Functions.h"

#include "DateTimeOffsetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	RestoreWindowPosition(&theApp, this);

	DragAcceptFiles();

	m_files.push_back(_T("C:\\Users\\Public\\Documents\\LinkMeMine\\Log\\ManualLauncher\\ManualLauncher_20250924.log"));
	//m_files.push_back(_T("C:\\Users\\Public\\Documents\\LinkMeMine\\Log\\ManualLauncher\\ManualLauncher_20250925.log"));
	m_files.push_back(_T("C:\\Users\\Public\\Documents\\LinkMeMine\\Log\\ManualLauncher\\ManualLauncher_20251105.log"));

	open_files();

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

void CLogDiffDlg::release()
{
	for (int i = 0; i < m_rich.size(); i++)
	{
		m_rich[i]->DestroyWindow();
		delete m_rich[i];

		m_title[i]->DestroyWindow();
		delete m_title[i];
	}

	m_rich.clear();
	m_title.clear();
	m_content.clear();
}

void CLogDiffDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TCHAR sfile[MAX_PATH];
	int count = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);

	if (!IsCtrlPressed())
		m_files.clear();

	for (int i = 0; i < count; i++)
	{
		DragQueryFile(hDropInfo, i, sfile, MAX_PATH);

		if (PathIsDirectory(sfile))
			continue;

		m_files.push_back(sfile);
	}

	open_files();

	CDialogEx::OnDropFiles(hDropInfo);
}

//m_files에 채워진 파일들을 로딩한다.
void CLogDiffDlg::open_files()
{
	release();

	if (m_files.size() == 0)
		return;

	m_content.resize(m_files.size());

	for (int i = 0; i < m_files.size(); i++)
	{
		CRichEditCtrlEx* rich = new CRichEditCtrlEx();
		rich->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN | ES_READONLY |
					 WS_HSCROLL | ES_AUTOHSCROLL | WS_VSCROLL | ES_AUTOVSCROLL | WS_EX_STATICEDGE,
			CRect(0, 0, 1, 1), this, 0);
		rich->use_popup_menu(false);
		rich->ShowTimeInfo(false);
		CFont* font = GetFont();
		rich->SetFont(font);
		//rich->SetFontSize(9);
		//rich->SetFontName(_T("Consolas"));
		//rich->load(m_files[i]);
		//rich->SetSel(-1, 0);
		//rich->LineScroll(0);
		read_file(m_files[i], &m_content[i], true);
		m_rich.push_back(rich);

		//edit에는 fullpath가 표시되는데 width가 작아졌을 때 word-wrap되므로 ES_AUTOHSCROLL을 줘야 한다.
		CSCEdit* edit = new CSCEdit();
		edit->create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_BORDER | ES_AUTOHSCROLL, CRect(0, 0, 1, 1), this, 1);
		edit->SetFont(font);
		edit->set_font_weight(500);
		edit->set_text(m_files[i]);
		edit->set_text_color(Gdiplus::Color::RoyalBlue);
		edit->set_back_color(Gdiplus::Color::Ivory);// get_sys_color(COLOR_3DFACE));
		edit->set_use_readonly_color(false);
		edit->set_line_align(DT_VCENTER);
		m_title.push_back(edit);
	}

	arrange_controls();

	Wait(10);

	arrange_logs_by_timestamp();
}

void CLogDiffDlg::arrange_controls()
{
	CRect rc;
	GetClientRect(rc);

	if (m_rich.size() == 0)
		return;

	int gap = 8;
	int edit_height = 32;
	CSize m_sz_scrollbar(24, rc.Height());
	CRect margin(8, 8, 8, 8);

	int w = (rc.Width() - (m_rich.size() - 1) * gap - margin.left - margin.right - m_sz_scrollbar.cx) / m_rich.size();

	CRect rtitle = CRect(margin.left, margin.top, margin.left + w, margin.top + edit_height);
	CRect rrich = CRect(margin.left, rtitle.bottom + gap, margin.left + w, rc.bottom - margin.bottom);

	for (int i = 0; i < m_rich.size(); i++)
	{
		m_rich[i]->MoveWindow(rrich);
		rrich.OffsetRect(w + gap, 0);

		m_title[i]->MoveWindow(rtitle);
		rtitle.OffsetRect(w + gap, 0);
	}
}

void CLogDiffDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	arrange_controls();
}

BOOL CLogDiffDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_HSCROLL ||
		pMsg->message == WM_VSCROLL ||
		pMsg->message == WM_MOUSEHWHEEL ||
		pMsg->message == WM_MOUSEWHEEL)
	{
		TRACE(_T("PreTranslateMessage: pMsg->message = %d\n"), pMsg->message);
		sync_scroll(pMsg);
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
	CRichEditCtrlEx* rich = (CRichEditCtrlEx*)CWnd::FromHandle(pMsg->hwnd);

	int hpos = rich->GetScrollPos(SB_HORZ);
	int vpos = rich->GetScrollPos(SB_VERT);
	TRACE(_T("old hpos=%d, vpos=%d\n"), hpos, vpos);

	for (int i = 0; i < m_rich.size(); i++)
	{
		if (rich == m_rich[i])
			continue;

		if (pMsg->message == WM_MOUSEHWHEEL || pMsg->message == WM_HSCROLL)
			::SendMessage(m_rich[i]->m_hWnd, WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, hpos), (LPARAM)(m_rich[i]->m_hWnd));
		else if (pMsg->message == WM_MOUSEWHEEL || pMsg->message == WM_VSCROLL)
			::SendMessage(m_rich[i]->m_hWnd, WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, vpos), (LPARAM)(m_rich[i]->m_hWnd));
	}
}

void CLogDiffDlg::arrange_logs_by_timestamp()
{
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

		m_rich[i]->SetRedraw(FALSE);
	}

	//나머지 로그들도 동일한 라인수로 맞춰준다?
	std::deque<int> cur_line(3, 0);

	for (i = 0; i < m_content.size(); i++)
	{
		for (j = 0; j < m_content[max_line_index].size(); j++)
		{
			if (j < m_content[i].size())
			{
				//라인이 존재하면 해당 라인을 추가
				m_rich[i]->add(-1, m_content[i][j]);
			}
		}
	}

	for (i = 0; i < m_rich.size(); i++)
	{
		m_rich[i]->SetRedraw(TRUE);
	}
}

void CLogDiffDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == timer_id_initial_focus)
	{
		KillTimer(timer_id_initial_focus);

		for (int i = 0; i < m_rich.size(); i++)
		{
			m_rich[i]->SetSel(-1, 0);
			//Wait(1000);
			//m_rich[0]->LineScroll(0);	//왜 이 함수호출로는 안먹히고 SendMessage()는 동작하는지...
			::SendMessage(m_rich[i]->m_hWnd, WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, 0), (LPARAM)(m_rich[i]->m_hWnd));
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CLogDiffDlg::OnMenuDatetimeShift()
{
	CDateTimeOffsetDlg dlg;

	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	//SYSTEMTIME t;
	//ZeroMemory(&t, sizeof(SYSTEMTIME));

	//t = dlg.m_t;

	auto it = std::find(m_rich.begin(), m_rich.end(), m_context_menu_hwnd);
	if (it == m_rich.end())
		return;

	int index = std::distance(m_rich.begin(), it);
	trace(index);
	
	CString line;
	SYSTEMTIME time_stamp;
	SYSTEMTIME time_first_log;	//맨 첫번째 로그의 시간값. 특정 시간으로 변경 시 1번 로그부터 상대시간값을 더해줘야 한다.
	SYSTEMTIME time_current_log;	

	//"yyyy-MM-dd hh:mm:ss.???" 형태가 표준이지만 로그에 따라서
	//날짜 구분자가 '-', '/' 이거나 없을수도 있다.
	//우선 날짜의 '-' 또는 '/'와 시간의 ':'는 반드시 존재하고 날짜와 시간 사이의 공백은 n개이어도 인식된다.
	//std::regex datetime_pattern("([0-9]{4})[-/]([0-9]{2})[-/]([0-9]{2})[ ]*([0-9]{2})[:]([0-9]{2})[:]([0-9]{2})");
	std::regex datetime_pattern("(\\d{4})[-/](\\d{2})[-/](\\d{2}) (\\d{2}):(\\d{2}):(\\d{2}).(\\d{3})");

	for (int i = 0; i < 10/*m_content[index].size()*/; i++)
	{
		line = m_content[index][i];
		line.Trim();

		if (line.IsEmpty())
		{
			TRACE(_T("%d. empty line\n"), i);
			continue;
		}

		std::smatch matches;
		std::string ssline = CString2string(line);

		if (std::regex_search(ssline, matches, datetime_pattern))
		{
			TRACE(_T("%d. Matched datetime: '%S' src = %s\n"), i, matches[0].str().c_str(), line);

			if (i == 0)
				time_first_log = get_SYSTEMTIME_from_datetime_str(CString(matches[0].str().c_str()));
			else
				time_current_log = get_SYSTEMTIME_from_datetime_str(CString(matches[0].str().c_str()));

			//datetime을 추출하고 shift 또는 set 한 후 다시 라인에 반영한다.
			CString prefix(matches.prefix().str().c_str());
			CString suffix(matches.suffix().str().c_str());

			if (dlg.m_method == method_offset_time_shift)
			{
				
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
					SYSTEMTIME time_diff = time_current_log - time_first_log;
					time_current_log = dlg.m_t + time_diff;
				}

				//다시 라인에 반영
				CString new_datetime_str = get_datetime_str(time_current_log);
				m_content[index][i] = prefix + new_datetime_str + suffix;
			}
		}
		else
		{
			TRACE(_T("%d. No match found in line: %s\n"), i, line);
		}

		//std::regex_search(line.GetString(), datetime_pattern);
		//extract_timestamp(line, time_stamp);
		//TRACE(_T("%s\n"), get_datetime_str(time_stamp));
		//shift_datetime_in_log_line(m_content[index][i], dlg.m_t);
	}
}

void CLogDiffDlg::extract_timestamp(CString line, SYSTEMTIME& time_stamp)
{
	int end_bracket = line.Find(']');
	line = line.Left(end_bracket);
	line.Remove('[');
	time_stamp = get_SYSTEMTIME_from_datetime_str(line);
}

void CLogDiffDlg::shift_datetime_in_log_line(CString& line, SYSTEMTIME tOffset)
{

}

void CLogDiffDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	m_context_menu_hwnd = (CRichEditCtrlEx*)pWnd;

	CMenu menu;
	CMenu* pMenu = NULL;

	menu.LoadMenu(IDR_MENU_CONTEXT);
	pMenu = menu.GetSubMenu(0);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}
