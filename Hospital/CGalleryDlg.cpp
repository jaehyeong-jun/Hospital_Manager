// CGalleryDlg.cpp: 구현 파일
#include "pch.h"
#include "health.h"
#include "CGalleryDlg.h"
#include "afxdialogex.h"

// CGalleryDlg 대화 상자

IMPLEMENT_DYNAMIC(CGalleryDlg, CDialogEx)

CGalleryDlg::CGalleryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_GALLERY, pParent)
{

}

CGalleryDlg::~CGalleryDlg()
{
}

void CGalleryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGalleryDlg, CDialogEx)
	ON_WM_TIMER()
	ON_STN_DBLCLK(IDC_STATIC_PIC0, &CGalleryDlg::OnStnDblclickStaticPic0)
	ON_STN_DBLCLK(IDC_STATIC_PIC1, &CGalleryDlg::OnStnDblclickStaticPic1)
	ON_STN_DBLCLK(IDC_STATIC_PIC2, &CGalleryDlg::OnStnDblclickStaticPic2)
	ON_STN_DBLCLK(IDC_STATIC_PIC3, &CGalleryDlg::OnStnDblclickStaticPic3)
	ON_STN_DBLCLK(IDC_STATIC_PIC4, &CGalleryDlg::OnStnDblclickStaticPic4)
	ON_STN_DBLCLK(IDC_STATIC_PIC5, &CGalleryDlg::OnStnDblclickStaticPic5)
END_MESSAGE_MAP()

BOOL CGalleryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_nCurrentIndex = 0;

	// 50ms 뒤에 울리는 타이머
	SetTimer(1, 50, NULL);

	return TRUE;
}

//고화질로 하는 함수
void CGalleryDlg::DrawGalleryImage(int nIndex)
{
	CTime curTime = CTime::GetCurrentTime();
	CString strDate;
	strDate.Format(_T("%d-%d-%d"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay());

	CString strPath;

	strPath.Format(_T("C:\\Users\\admin\\source\\repos\\health\\health\\x64\\Debug\\download\\%s\\%s\\face%d.jpg"), (LPCTSTR)m_strCardNum, (LPCTSTR)strDate, nIndex);

	CImage image;
	HRESULT hr = image.Load(strPath);

	int nControlID = 0;
	if (nIndex == 0) nControlID = IDC_STATIC_PIC0;
	else if (nIndex == 1) nControlID = IDC_STATIC_PIC1;
	else if (nIndex == 2) nControlID = IDC_STATIC_PIC2;
	else if (nIndex == 3) nControlID = IDC_STATIC_PIC3;
	else if (nIndex == 4) nControlID = IDC_STATIC_PIC4;
	else if (nIndex == 5) nControlID = IDC_STATIC_PIC5;

	// 결정된 액자를 가져옴
	CStatic* pStatic = (CStatic*)GetDlgItem(nControlID);
	if (pStatic == NULL) return;

	if (FAILED(hr)) {
		pStatic->SetBitmap(NULL);
		pStatic->Invalidate();
		return;
	}

	CRect rect;
	pStatic->GetClientRect(&rect);
	CDC* pDC = pStatic->GetDC();

	pStatic->Invalidate();
	pStatic->UpdateWindow();

	pDC->SetStretchBltMode(HALFTONE);
	SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);

	image.Draw(pDC->m_hDC, rect);

	ReleaseDC(pDC);
}
/*
//다음버튼
void CGalleryDlg::OnBnClickedBtnNext()
{
	m_nCurrentIndex++;

	if (m_nCurrentIndex > 5) {
		m_nCurrentIndex = 0;
	}

	DrawGalleryImage(m_nCurrentIndex);
}

//이전버튼
void CGalleryDlg::OnBnClickedBtnPrev()
{
	m_nCurrentIndex--;

	if (m_nCurrentIndex < 0) {
		m_nCurrentIndex = 5;
	}

	DrawGalleryImage(m_nCurrentIndex);
}
*/
void CGalleryDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) {
		KillTimer(1); // 타이머끄기

		for (int i = 0; i < 6; i++) {
			DrawGalleryImage(i);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CGalleryDlg::OnStnDblclickStaticPic0()
{
	CTime curTime = CTime::GetCurrentTime();
	CString strDate;
	strDate.Format(_T("%d-%d-%d"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay());

	CString strPath;
	strPath.Format(_T("C:\\Users\\admin\\source\\repos\\health\\health\\x64\\Debug\\download\\%s\\%s\\face0.jpg"), (LPCTSTR)m_strCardNum, (LPCTSTR)strDate);

	//윈도우가 가진 기본 앱 열기
	::ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_MAXIMIZE);
}


void CGalleryDlg::OnStnDblclickStaticPic1()
{
	CTime curTime = CTime::GetCurrentTime();
	CString strDate;
	strDate.Format(_T("%d-%d-%d"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay());

	CString strPath;
	strPath.Format(_T("C:\\Users\\admin\\source\\repos\\health\\health\\x64\\Debug\\download\\%s\\%s\\face1.jpg"), (LPCTSTR)m_strCardNum, (LPCTSTR)strDate);

	//윈도우가 가진 기본 앱 열기
	::ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWMAXIMIZED);
}


void CGalleryDlg::OnStnDblclickStaticPic2()
{
	CTime curTime = CTime::GetCurrentTime();
	CString strDate;
	strDate.Format(_T("%d-%d-%d"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay());

	CString strPath;
	strPath.Format(_T("C:\\Users\\admin\\source\\repos\\health\\health\\x64\\Debug\\download\\%s\\%s\\face2.jpg"), (LPCTSTR)m_strCardNum, (LPCTSTR)strDate);

	//윈도우가 가진 기본 앱 열기
	::ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWMAXIMIZED);
}

void CGalleryDlg::OnStnDblclickStaticPic3()
{
	CTime curTime = CTime::GetCurrentTime();
	CString strDate;
	strDate.Format(_T("%d-%d-%d"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay());

	CString strPath;
	strPath.Format(_T("C:\\Users\\admin\\source\\repos\\health\\health\\x64\\Debug\\download\\%s\\%s\\face3.jpg"), (LPCTSTR)m_strCardNum, (LPCTSTR)strDate);

	//윈도우가 가진 기본 앱 열기
	::ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWMAXIMIZED);
}

void CGalleryDlg::OnStnDblclickStaticPic4()
{
	CTime curTime = CTime::GetCurrentTime();
	CString strDate;
	strDate.Format(_T("%d-%d-%d"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay());

	CString strPath;
	strPath.Format(_T("C:\\Users\\admin\\source\\repos\\health\\health\\x64\\Debug\\download\\%s\\%s\\face4.jpg"), (LPCTSTR)m_strCardNum, (LPCTSTR)strDate);

	//윈도우가 가진 기본 앱 열기
	::ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWMAXIMIZED);
}

void CGalleryDlg::OnStnDblclickStaticPic5()
{
	CTime curTime = CTime::GetCurrentTime();
	CString strDate;
	strDate.Format(_T("%d-%d-%d"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay());

	CString strPath;
	strPath.Format(_T("C:\\Users\\admin\\source\\repos\\health\\health\\x64\\Debug\\download\\%s\\%s\\face5.jpg"), (LPCTSTR)m_strCardNum, (LPCTSTR)strDate);

	//윈도우가 가진 기본 앱 열기
	::ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWMAXIMIZED);
}
