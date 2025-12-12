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
	DDX_Control(pDX, IDC_DATE, m_date);
	DDX_Control(pDX, IDC_TIME, m_time);
}


BEGIN_MESSAGE_MAP(CDateTimeOffsetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDateTimeOffsetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDateTimeOffsetDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_TIME_SHIFT, &CDateTimeOffsetDlg::OnBnClickedRadioTimeShift)
	ON_BN_CLICKED(IDC_RADIO_DATETIME, &CDateTimeOffsetDlg::OnBnClickedRadioDatetime)
END_MESSAGE_MAP()


// CDateTimeOffsetDlg 메시지 처리기

void CDateTimeOffsetDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void CDateTimeOffsetDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CDateTimeOffsetDlg::OnBnClickedRadioTimeShift()
{
	m_edit_time_shift.EnableWindow(TRUE);
	m_date.EnableWindow(FALSE);
	m_time.EnableWindow(FALSE);
}

void CDateTimeOffsetDlg::OnBnClickedRadioDatetime()
{
	m_edit_time_shift.EnableWindow(FALSE);
	m_date.EnableWindow(FALSE);
	m_time.EnableWindow(FALSE);
}
