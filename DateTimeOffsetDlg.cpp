// DateTimeOffsetDlg.cpp: 구현 파일
//

#include "pch.h"
#include "LogDiff.h"
#include "afxdialogex.h"
#include "DateTimeOffsetDlg.h"


// CDateTimeOffsetDlg 대화 상자

IMPLEMENT_DYNAMIC(CDateTimeOffsetDlg, CDialogEx)

CDateTimeOffsetDlg::CDateTimeOffsetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATETIME_OFFSET, pParent)
{

}

CDateTimeOffsetDlg::~CDateTimeOffsetDlg()
{
}

void CDateTimeOffsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_TIME_SHIFT, m_radio_time_shift);
	DDX_Control(pDX, IDC_EDIT_TIME_SHIFT, m_edit_time_shift);
	DDX_Control(pDX, IDC_RADIO_DATETIME, m_radio_datetime);
	DDX_Control(pDX, IDC_DATETIME, m_datetime);
}


BEGIN_MESSAGE_MAP(CDateTimeOffsetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDateTimeOffsetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDateTimeOffsetDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_TIME_SHIFT, &CDateTimeOffsetDlg::OnBnClickedRadioTimeShift)
	ON_BN_CLICKED(IDC_RADIO_DATETIME, &CDateTimeOffsetDlg::OnBnClickedRadioDatetime)
END_MESSAGE_MAP()


// CDateTimeOffsetDlg 메시지 처리기

BOOL CDateTimeOffsetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//m_edit_time_shift.set_draw_border(true, 3, Gdiplus::Color::Red);
	m_datetime.SetFormat(_T("yyyy-MM-dd  HH:mm:ss"));

	m_method = AfxGetApp()->GetProfileInt(_T("setting\\offset dlg"), _T("m_method"), method_offset_time_shift);
	m_sign = AfxGetApp()->GetProfileInt(_T("setting\\offset dlg"), _T("m_sign"), 1);
	m_t.load_profile_value(_T("setting\\offset dlg"), _T("m_t"), CSCTime());
	CString shift_value = AfxGetApp()->GetProfileString(_T("setting\\offset dlg"), _T("shift value"), _T(""));
	m_edit_time_shift.set_text(shift_value);

	//특정 날짜 시간으로 설정한 이력이 있어도
	//m_method가 time_shift방식이면 m_datetime에는 기존 설정했던 시간이 아닌 현재 시간으로 표시된다.
	//특정 날짜 시간으로 설장한 적이 있다면 method에 무관하게 그 값을 저장하고 표시해줘야 한다.
	if (m_method == method_offset_time_shift)
	{
		m_radio_time_shift.SetCheck(BST_CHECKED);
		m_edit_time_shift.EnableWindow(TRUE);
		m_datetime.EnableWindow(FALSE);

		float f = (float)m_t.second + (float)m_t.milliseconds / 1000.0f;
		if (m_sign < 0)
			f *= -1.0f;
		CString text;
		text.Format(_T("%.3f"), f);
		m_edit_time_shift.set_text(text);

		CSCTime t_specific;
		t_specific.load_profile_value(_T("setting\\offset dlg"), _T("m_t_specific"), CSCTime());
		if (t_specific.year != 0)
			m_datetime.SetTime(t_specific.to_SYSTEMTIME());
	}
	else
	{
		m_radio_datetime.SetCheck(BST_CHECKED);
		m_edit_time_shift.EnableWindow(FALSE);
		m_datetime.EnableWindow(TRUE);

		//COleDateTime dt(
		//	m_t.wYear,
		//	m_t.wMonth,
		//	m_t.wDay,
		//	m_t.wHour,
		//	m_t.wMinute,
		//	m_t.wSecond
		//);
		if (!m_t.is_empty())
			m_datetime.SetTime(m_t.to_SYSTEMTIME());
		//m_time.SetTime(&dt);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDateTimeOffsetDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_sign = 1;

	if (m_method == method_offset_time_shift)
	{
		CString text = m_edit_time_shift.get_text();
		
		text.Trim();
		if (text.IsEmpty() || (text == _T("0.000")))
			return;

		AfxGetApp()->WriteProfileString(_T("setting\\offset dlg"), _T("shift value"), text);

		float f = _tstof(text);
		if (f < 0.0f)
		{
			m_sign = -1;
			f *= -1.0f;
		}

		m_t = CSCTime(0, 0, 0, 0, 0, (int)f, (int)(f * 1000) % 1000);
	}
	else
	{
		m_t.get_time(m_datetime);
		m_t.save_profile_value(_T("setting\\offset dlg"), _T("m_t_specific"));
	}

	AfxGetApp()->WriteProfileInt(_T("setting\\offset dlg"), _T("m_method"), m_method);
	AfxGetApp()->WriteProfileInt(_T("setting\\offset dlg"), _T("m_sign"), m_sign);
	m_t.save_profile_value(_T("setting\\offset dlg"), _T("m_t"));

	CDialogEx::OnOK();
}

void CDateTimeOffsetDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CDateTimeOffsetDlg::OnBnClickedRadioTimeShift()
{
	m_method = method_offset_time_shift;
	m_edit_time_shift.EnableWindow(TRUE);
	m_datetime.EnableWindow(FALSE);
}

void CDateTimeOffsetDlg::OnBnClickedRadioDatetime()
{
	m_method = method_specific_datetime;
	m_edit_time_shift.EnableWindow(FALSE);
	m_datetime.EnableWindow(TRUE);
}
