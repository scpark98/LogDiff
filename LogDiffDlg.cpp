
// LogDiffDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LogDiff.h"
#include "LogDiffDlg.h"
#include "afxdialogex.h"

#include "Common/Functions.h"

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
	for (int i = 0; i < m_rich.size(); i++)
	{
		m_rich[i]->DestroyWindow();
		delete m_rich[i];
	}

	CDialogEx::OnCancel();
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

void CLogDiffDlg::open_files()
{
	for (int i = 0; i < m_files.size(); i++)
	{
		CRichEditCtrlEx* rich = new CRichEditCtrlEx();
		rich->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY | ES_AUTOHSCROLL, CRect(0, 0, 1, 1), this, 0);
		rich->use_popup_menu(false);
		rich->ShowTimeInfo(false);
		rich->SetFontSize(9);
		rich->SetFontName(_T("Consolas"));
		rich->load(m_files[i]);
		m_rich.push_back(rich);
	}

	arrange_controls();
}

void CLogDiffDlg::arrange_controls()
{
	CRect rc;
	GetClientRect(rc);

	if (m_rich.size() == 0)
		return;

	int gap = 4;
	CSize m_sz_scrollbar(8, rc.Height());
	CRect margin(8, 8, 8, 8);

	int w = (rc.Width() - (m_rich.size() - 1) * gap - margin.left - margin.right - m_sz_scrollbar.cx) / m_rich.size();

	CRect r = make_rect(margin.left, margin.top, w, rc.Height() - margin.top - margin.bottom);

	for (int i = 0; i < m_rich.size(); i++)
	{
		m_rich[i]->MoveWindow(r);
		r.OffsetRect(w + gap, 0);
	}
}

void CLogDiffDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	arrange_controls();
}
