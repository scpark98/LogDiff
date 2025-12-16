
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
	ON_COMMAND(ID_MENU_SAVE, &CLogDiffDlg::OnMenuSave)
	ON_COMMAND(ID_MENU_SAVE_AS, &CLogDiffDlg::OnMenuSaveAs)
	ON_COMMAND(ID_MENU_CURRENT_FOLDER, &CLogDiffDlg::OnMenuCurrentFolder)
	ON_COMMAND(ID_MENU_SORT, &CLogDiffDlg::OnMenuSort)
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

#if 1
	m_files.push_back(get_exe_directory() + _T("\\test_log_data\\ManualLauncher_20250924.log"));
	m_files.push_back(get_exe_directory() + _T("\\test_log_data\\ManualLauncher_20250925.log"));
	m_files.push_back(get_exe_directory() + _T("\\test_log_data\\ManualLauncher_20251105.log"));
	open_files();
#else
	m_files.push_back(get_exe_directory() + _T("\\test_log_data\\LMMViewer.log"));
	m_files.push_back(get_exe_directory() + _T("\\test_log_data\\LMMAgentService.log"));
	open_files();
#endif

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
		rich->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN | ES_NOHIDESEL | ES_READONLY |
					 WS_HSCROLL | ES_AUTOHSCROLL | WS_VSCROLL | ES_AUTOVSCROLL | WS_EX_STATICEDGE,
			CRect(0, 0, 1, 1), this, 0);
		rich->use_popup_menu(false);
		rich->ShowTimeInfo(false);
		CFont* font = GetFont();
		rich->SetFont(font);
		rich->add_keyword_format(CSCKeywordFormat(_T("error"), red, true));
		rich->add_keyword_format(CSCKeywordFormat(_T("에러"), red, true));
		rich->add_keyword_format(CSCKeywordFormat(_T("fail"), red, true));
		rich->add_keyword_format(CSCKeywordFormat(_T("실패"), red, true));
		rich->add_keyword_format(CSCKeywordFormat(_T("warning"), orange, true));
		rich->add_keyword_format(CSCKeywordFormat(_T("경고"), orange, true));
		rich->add_keyword_format(CSCKeywordFormat(_T("success"), royalblue, true));
		rich->add_keyword_format(CSCKeywordFormat(_T("성공"), royalblue, true));
		rich->set_font_size(10);
		//rich->set_font_name(_T("Consolas"));
		rich->set_font_name(_T("Noto Sans KR"));
		read_file(m_files[i], &m_content[i]);
		rich->set_text(&m_content[i]);
		rich->SetOptions(ECOOP_XOR, ECO_SAVESEL);
		m_rich.push_back(rich);

		//문서 타이틀 표시
		//edit에는 fullpath가 표시되는데 width가 작아졌을 때 word-wrap되므로 ES_AUTOHSCROLL을 줘야 한다.
		CSCEdit* edit = new CSCEdit();
		edit->create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_BORDER | ES_AUTOHSCROLL, CRect(0, 0, 1, 1), this, 1);
		edit->SetFont(font);
		edit->set_font_name(_T("Consolas"));// (_T("Noto Sans KR"));
		edit->set_font_size(10);
		edit->set_font_weight(700);
		edit->set_text(m_files[i]);
		edit->set_text_color(Gdiplus::Color::RoyalBlue);
		edit->set_back_color(Gdiplus::Color::Ivory);// get_sys_color(COLOR_3DFACE));
		edit->set_use_readonly_color(false);
		edit->set_line_align(DT_VCENTER);
		m_title.push_back(edit);
	}

	//파일수에 따라 레이아웃을 재조정한다.
	arrange_layout();

	//맨 처음 파일을 연 후에는 바로 정렬하지 않는다.
	//처음 실행 후 파일 내용을 표시한 후에는 drag&drop 등의 액션으로 파일이 추가되거나 빠지거나 달라지면
	//그 때는 바로 정렬을 수행한다.
	//arrange_logs_by_timestamp();
}

void CLogDiffDlg::arrange_layout()
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
	//TRACE(_T("old hpos=%d, vpos=%d\n"), hpos, vpos);

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

class CForSortLog
{
public:
	CForSortLog(int _index, CSCTime _time_stamp, CString _full_log)
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
	//문서 index와 시간문자열을 pair로 하여 리스트에 모두 넣고 시간값으로 정렬시킨다.
	//0 ~ n까지 순차적으로 나타나는지 체크한다.
	//만약 index가 0 1 2로 증가해야 하는데 0 1 1이었다면 2번에 공백을 추가시켜준다.
	//이렇게 끝까지 순차 검사를 완료했다면 i번은 m_content[i]에 다시 넣어준다.
	std::deque<CForSortLog> list;

	int i, j;

	for (i = 0; i < m_content.size(); i++)
	{
		CSCTime prev_timestamp;

		for (j = 0; j < m_content[i].size(); j++)
		{
			//만약 공백이나 timestamp가 없는 라인이라면 timestamp는 이전값보다 ms를 1만큼 큰 값으로 변경해준다.
			//그래야만 정렬시에 그 위치가 유지된다.
			CString log = m_content[i][j];
			CSCTime time_stamp(m_content[i][j]);

			if (j > 0 && time_stamp.is_empty())
			{
				time_stamp = prev_timestamp + CSCTime(0, 0, 0, 0, 0, 0, 1);
			}
			list.push_back(CForSortLog(i, time_stamp, m_content[i][j]));
			prev_timestamp = time_stamp;
		}
	}

	std::sort(list.begin(), list.end(),
		[](CForSortLog a, CForSortLog b)
		{
			return (a.time_stamp < b.time_stamp);
		});

	FILE* fp = NULL;
	_tfopen_s(&fp, _T("D:\\log_list_before_adjust.txt"), _T("wt")CHARSET);
	for (i = 0; i < list.size(); i++)
	{
		_ftprintf(fp, _T("%d|%s|%s"), list[i].index, list[i].time_stamp.to_string(), list[i].full_log);
	}

	fclose(fp);

	//0 ~ n까지 순차적으로 나타나는지 체크한다.
	//만약 index가 0 1 2로 순차적으로 나타나야 하는데 0 1 1이었다면 두번째에 있는 1 뒤에 2번 항목으로 공백을 추가시켜준다.
	//이렇게 끝까지 순차 검사를 완료했다면 각각 인덱스대로 m_content[i]에 다시 넣어준다.
	int sequence = 0;
	i = 0;

	while (true)
	{
		if (list[i].index != sequence)
		{
			//현재 timestamp가 다음 timestamp보다 작을때만 추가한다. 동일하다면 굳이 새 라인을 추가할 필요가 없다.
			if (false)//(i > 0) && (list[i].time_stamp == list[i - 1].time_stamp))
			{
				i--;
			}
			else if ((i > 0) && true)//(list[i].time_stamp > list[i - 1].time_stamp))
			{
				//i 자리에 한 라인을 추가하되 그 시간값은 바로 전 항목의 시간값과 동일하게 한다.
				list.insert(list.begin() + i, CForSortLog(sequence, list[i - 1].time_stamp, _T("\n")));
			}
			else
			{
				//i--;
			}
		}

		i++;
		sequence++;

		if (sequence == m_rich.size())
			sequence = 0;

		//index i가 list.size()에 도달하면 종료시킨다.
		if (i >= list.size())
			break;
	}

	fp = NULL;
	_tfopen_s(&fp, _T("D:\\log_list_after_adjust.txt"), _T("wt")CHARSET);
	for (i = 0; i < list.size(); i++)
	{
		_ftprintf(fp, _T("%d|%s|%s"), list[i].index, list[i].time_stamp.to_string(), list[i].full_log);
	}

	fclose(fp);


	//위에서 보정된 list를 각 content에 순차적으로 넣어준다.
	//우선 기존 content를 모두 clear하고
	for (i = 0; i < m_content.size(); i++)
		m_content[i].clear();

	for (i = 0; i < list.size(); i++)
		m_content[list[i].index].push_back(list[i].full_log);

	for (i = 0; i < m_rich.size(); i++)
		m_rich[i]->set_text(&m_content[i]);

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

		m_rich[i]->SetRedraw(FALSE);
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
		m_rich[i]->SetRedraw(TRUE);
	}

	*/
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

	for (int i = 0; i < m_content[index].size(); i++)
	{
		line = m_content[index][i];

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

				//다시 라인에 반영
				CString new_datetime_str = time_current_log.to_string();
				m_content[index][i] = prefix + new_datetime_str + suffix;
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

				//다시 라인에 반영
				CString new_datetime_str = time_current_log.to_string();
				m_content[index][i] = prefix + new_datetime_str + suffix;
			}
		}
		else
		{
			Trace(_T("%d. No match found in line: %s\n"), i, line);
		}
	}

	//변경된 내용을 다시 rich에 적용시킨다.
	m_rich[index]->set_text(&m_content[index]);

	arrange_logs_by_timestamp();
}

void CLogDiffDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	m_context_menu_hwnd = (CRichEditCtrlEx*)pWnd;

	auto it = std::find(m_rich.begin(), m_rich.end(), m_context_menu_hwnd);
	if (it == m_rich.end())
		return;

	m_context_menu_doc_index = std::distance(m_rich.begin(), it);


	CMenu menu;
	CMenu* pMenu = NULL;

	menu.LoadMenu(IDR_MENU_CONTEXT);
	pMenu = menu.GetSubMenu(0);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CLogDiffDlg::OnMenuSave()
{
	int index = m_context_menu_doc_index;
	if (index < 0 || index >= m_files.size())
		return;

	m_rich[index]->save(m_files[index]);
}

void CLogDiffDlg::OnMenuSaveAs()
{
	int index = m_context_menu_doc_index;
	if (index < 0 || index >= m_files.size())
		return;

	CFileDialog dlg(FALSE, get_part(m_files[index], fn_ext), m_files[index], OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Log Files (*.log)|*.log|All Files (*.*)|*.*||"), this);
	if (dlg.DoModal() != IDOK)
		return;

	if (m_rich[index]->save(dlg.GetPathName()) == false)
	{
		AfxMessageBox(dlg.GetPathName() + _T("\n\n파일 저장 실패"), MB_ICONSTOP);
		return;
	}

	//파일명 정보를 갱신한다.
	m_files[index] = dlg.GetPathName();
	m_title[index]->set_text(dlg.GetPathName());
}

void CLogDiffDlg::OnMenuCurrentFolder()
{
	int index = m_context_menu_doc_index;
	if (index < 0 || index >= m_files.size())
		return;

	ShellExecute(NULL, _T("open"), _T("explorer"), _T("/select,") + m_files[index], NULL, SW_SHOW);
}

void CLogDiffDlg::OnMenuSort()
{
	arrange_logs_by_timestamp();
}
