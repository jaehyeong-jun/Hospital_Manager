#include "pch.h"          
#include "framework.h"    
#include "health.h"
#include "healthDlg.h"
#include "afxdialogex.h"
#include "CGalleryDlg.h"

#pragma comment(lib, "libmysql.lib")
//데이터 1크기로 담기
#pragma pack(push, 1)

typedef struct
{
	char name[11];
	char birthday[7];
	char cardnumber[20];
	char phone[14];
	float height;
} ST_PROTOCOL;

#pragma pack(pop)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ChealthDlg::ChealthDlg(CWnd* pParent)
	: CDialogEx(IDD_HEALTH_DIALOG, pParent)
	, m_strName(_T(""))       
	, m_strBirthday(_T(""))
	, m_strCard_number(_T(""))
	, m_strPhone(_T(""))       
	, m_strHeight(_T(""))    
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); 
}

void ChealthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_BIRTHDAY, m_strBirthday);
	DDX_Text(pDX, IDC_EDIT_CARD_NUMBER, m_strCard_number);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_strPhone);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_strHeight);
	DDX_Control(pDX, IDC_LIST_CUSTOMERS, m_listCustomers);
}

BEGIN_MESSAGE_MAP(ChealthDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &ChealthDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &ChealthDlg::OnBnClickedButton1) // 버튼1(저장) 
	ON_BN_CLICKED(IDC_BUTTON2, &ChealthDlg::OnBnClickedButton2) // 버튼2(조회)
	ON_BN_CLICKED(IDC_BUTTON3, &ChealthDlg::OnBnClickedButton3) // 버튼3(삭제)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CUSTOMERS, &ChealthDlg::OnNMClickListCustomers) // 리스트
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &ChealthDlg::OnBnClickedButtonConnect) // 통신
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CUSTOMERS, &ChealthDlg::OnNMDblclkListCustomers)
END_MESSAGE_MAP()

BOOL ChealthDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_font.CreateFont(-12, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("맑은 고딕"));

	m_listCustomers.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listCustomers.InsertColumn(0, _T("이름"), LVCFMT_LEFT, 80);
	m_listCustomers.InsertColumn(1, _T("생년월일"), LVCFMT_LEFT, 160);
	m_listCustomers.InsertColumn(2, _T("카드번호"), LVCFMT_LEFT, 160);
	m_listCustomers.InsertColumn(3, _T("전화번호"), LVCFMT_RIGHT, 160);
	m_listCustomers.InsertColumn(4, _T("키(cm)"), LVCFMT_RIGHT, 100);

	m_listCustomers.SetFont(&m_font);
	if (GetDlgItem(IDC_EDIT_NAME)) GetDlgItem(IDC_EDIT_NAME)->SetFont(&m_font);
	if (GetDlgItem(IDC_EDIT_BIRTHDAY)) GetDlgItem(IDC_EDIT_BIRTHDAY)->SetFont(&m_font);
	if (GetDlgItem(IDC_EDIT_CARD_NUMBER)) GetDlgItem(IDC_EDIT_CARD_NUMBER)->SetFont(&m_font);
	if (GetDlgItem(IDC_EDIT_PHONE)) GetDlgItem(IDC_EDIT_PHONE)->SetFont(&m_font);
	if (GetDlgItem(IDC_EDIT_HEIGHT)) GetDlgItem(IDC_EDIT_HEIGHT)->SetFont(&m_font);

	

	for (int i = 0; i < 5; i++) {
		m_listCustomers.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

MYSQL* ChealthDlg::ConnectDB()
{
	MYSQL* conn = mysql_init(NULL); 

	if (mysql_real_connect(conn, "localhost", "root", "bym1101", "BYM", 3306, NULL, 0)) {
		mysql_set_character_set(conn, "euckr"); 
		return conn; 
	}
	return NULL; 
}
// FTP 서버 다운로드
void ChealthDlg::DownloadUserImage(CString strImageName)
{
	strImageName.Trim(); // 공백제거
	UpdateWindow();
	CWaitCursor wait;

	if (m_ftp.Login(_T("192.168.0.229"), _T("Sys_user"), _T("bym1101"), 21, NULL, TRUE, TRUE) == TRUE)
	{
		CTime curTime = CTime::GetCurrentTime();
		CString strDate;
		strDate.Format(_T("%d-%d-%d"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay());

		CString strBaseDir = _T("C:\\Users\\admin\\source\\repos\\health\\health\\x64\\Debug\\download");
		CreateDirectory(strBaseDir, NULL);

		CString strCardDir;
		strCardDir.Format(_T("%s\\%s"), (LPCTSTR)strBaseDir, (LPCTSTR)strImageName);
		CreateDirectory(strCardDir, NULL);

		CString strFirstDir;
		strFirstDir.Format(_T("%s\\first"), (LPCTSTR)strCardDir);

		bool bIsFirstTime = false;
		
		if (GetFileAttributes(strFirstDir) == INVALID_FILE_ATTRIBUTES) {
			bIsFirstTime = true; 
			CreateDirectory(strFirstDir, NULL); // First 폴더 생성
		}

		// 날짜 폴더 공사
		CString strDateDir;
		strDateDir.Format(_T("%s\\%s"), (LPCTSTR)strCardDir, (LPCTSTR)strDate);
		CreateDirectory(strDateDir, NULL);

		// 사진 6장 다운로드
		for (int i = 0; i < 6; i++)
		{
			CString strRemotePath;
			strRemotePath.Format(_T("/%s/%s/image%d.jpg"), (LPCTSTR)strImageName, (LPCTSTR)strDate, i);

			CString strLocalPath;
			strLocalPath.Format(_T("%s\\face%d.jpg"), (LPCTSTR)strDateDir, i);

			// 다운로드 시작
			if (m_ftp.DownloadFile(strRemotePath, strLocalPath) == TRUE) {


				if (bIsFirstTime == true) {
					CString strFirstCopyPath;
					strFirstCopyPath.Format(_T("%s\\face%d.jpg"), (LPCTSTR)strFirstDir, i);
					CopyFile(strLocalPath, strFirstCopyPath, FALSE);
				}
			}
		}
		m_ftp.LogOut();
	}
	else {
		AfxMessageBox(_T("❌ FTP 서버 접속 실패!"));
	}
}
// 옆에 처음 사진 띄우기
void ChealthDlg::DisplayImage(CString strPath)
{
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_FACE);
	if (!pStatic) return;

	CRect rect;
	pStatic->GetClientRect(&rect);
	CDC* pDC = pStatic->GetDC();

	pDC->FillSolidRect(rect, GetSysColor(COLOR_3DFACE));

	CImage image;
	HRESULT hr = image.Load(strPath);

	if (FAILED(hr)) {
		ReleaseDC(pDC);
		return;
	}

	pDC->SetStretchBltMode(HALFTONE);
	SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
	image.Draw(pDC->m_hDC, rect);

	ReleaseDC(pDC);
}

//저장
void ChealthDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);

	m_strPhone.Remove('-');

	if (m_strPhone.GetLength() == 11) {
		CString strAutoPhone;

		strAutoPhone.Format(_T("%s-%s-%s"), m_strPhone.Mid(0, 3), m_strPhone.Mid(3, 4), m_strPhone.Mid(7, 4));
		m_strPhone = strAutoPhone; // 조립된 번호를 덮어씌움!
	}
	else if (m_strPhone.GetLength() == 10) {
		CString strAutoPhone;
		strAutoPhone.Format(_T("%s-%s-%s"), m_strPhone.Mid(0, 3), m_strPhone.Mid(3, 3), m_strPhone.Mid(6, 4));
		m_strPhone = strAutoPhone;
	}

	UpdateData(FALSE);
	if (m_strCard_number.IsEmpty()) {
		AfxMessageBox(_T("카드번호를 입력해주세요."));
		return;
	}

	MYSQL* conn = ConnectDB();
	if (conn != NULL) {
		CString strCheckSQL, strSQL;
		bool bIsExist = false; // 이미 있는 사람인지 기억할 변수

		// DB에 이 카드번호를 가진 사람이 이미 있는지 물어봄
		strCheckSQL.Format(_T("SELECT card_number FROM customer WHERE card_number='%s'"), (LPCTSTR)m_strCard_number);

		if (mysql_query(conn, (CT2A)strCheckSQL) == 0) {
			MYSQL_RES* res = mysql_store_result(conn);
			if (res != NULL) {
				if (mysql_num_rows(res) > 0) {
					bIsExist = true; // DB에 이미 존재함
				}
				mysql_free_result(res);
			}
		}
		// 기존회원
		if (bIsExist == true) {
			strSQL.Format(_T("UPDATE customer SET name='%s', birthday='%s', phone='%s', height=%s WHERE card_number='%s'"),
				(LPCTSTR)m_strName, (LPCTSTR)m_strBirthday, (LPCTSTR)m_strPhone, (LPCTSTR)m_strHeight, (LPCTSTR)m_strCard_number);
		}
		else {
			// 신규 회원
			CTime curTime = CTime::GetCurrentTime();
			CString strRegDate;
			strRegDate.Format(_T("%02d%02d%02d"), curTime.GetYear()%100, curTime.GetMonth(), curTime.GetDay());


			strSQL.Format(_T("INSERT INTO customer (name, birthday, phone, height, card_number, reg_date) VALUES ('%s', '%s', '%s', %s, '%s', '%s')"),
				(LPCTSTR)m_strName, (LPCTSTR)m_strBirthday, (LPCTSTR)m_strPhone, (LPCTSTR)m_strHeight, (LPCTSTR)m_strCard_number, (LPCTSTR)strRegDate);
		}

		if (mysql_query(conn, (CT2A)strSQL) == 0) {
			if (bIsExist) AfxMessageBox(_T("회원 정보가 수정되었습니다."));
			else AfxMessageBox(_T("새 회원이 등록되었습니다."));

			// 화면 입력 칸 비우기
			m_strName = _T(""); m_strBirthday = _T(""); m_strCard_number = _T("");
			m_strPhone = _T(""); m_strHeight = _T("");
			UpdateData(FALSE);

			// 전체 목록 새로고침
			OnBnClickedButton2();
		}
		else {
			CString strErr;	
			strErr.Format(_T("DB 작업 실패: %S"), mysql_error(conn));
			AfxMessageBox(strErr);
		}

		mysql_close(conn);
	}
}
//조회
void ChealthDlg::OnBnClickedButton2()
{
	UpdateData(TRUE); 

	MYSQL* conn = ConnectDB();
	if (conn != NULL) {

		m_listCustomers.DeleteAllItems(); 

		CString strSQL;
		
		if (m_strName.IsEmpty() && m_strCard_number.IsEmpty()) {
			
			strSQL = _T("SELECT name, birthday, card_number, phone, height FROM customer ORDER BY CAST(card_number AS UNSIGNED) ASC");
		}
		else {
			
			strSQL.Format(_T("SELECT name, birthday, card_number, phone, height FROM customer WHERE name='%s' or card_number='%s'"),
				(LPCTSTR)m_strName, (LPCTSTR)m_strCard_number);
		}

		if (mysql_query(conn, (CT2A)strSQL) == 0) {
			MYSQL_RES* res = mysql_store_result(conn); 
			MYSQL_ROW row; 
			int nIndex = 0; 

			
			while ((row = mysql_fetch_row(res)) != NULL) {
				
				CString name = (row[0] ? (LPCTSTR)CA2W(row[0], 949) : _T(""));
				CString birthday = (row[1] ? (LPCTSTR)CA2W(row[1], 949) : _T(""));
				CString card = (row[2] ? (LPCTSTR)CA2W(row[2], 949) : _T(""));
				CString phone = (row[3] ? (LPCTSTR)CA2W(row[3], 949) : _T(""));
				CString height = (row[4] ? (LPCTSTR)CA2W(row[4], 949) : _T(""));

				m_listCustomers.InsertItem(nIndex, name);    
				m_listCustomers.SetItemText(nIndex, 1, birthday);
				m_listCustomers.SetItemText(nIndex, 2, card);
				m_listCustomers.SetItemText(nIndex, 3, phone); 
				m_listCustomers.SetItemText(nIndex, 4, height); 
				nIndex++; 
			}

			for (int i = 0; i < 5; i++) {
				m_listCustomers.SetColumnWidth(i, LVSCW_AUTOSIZE);
				if (m_listCustomers.GetColumnWidth(i) < 80)
					m_listCustomers.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
			}

			mysql_free_result(res); 
		}
		mysql_close(conn);
	}
}
//삭제
void ChealthDlg::OnBnClickedButton3()
{
	int nIndex = m_listCustomers.GetNextItem(-1, LVNI_SELECTED);
	if (nIndex == -1) { 
		AfxMessageBox(_T("삭제할 회원을 리스트에서 선택해주세요."));
		return;
	}

	CString strSelectedCard = m_listCustomers.GetItemText(nIndex, 2);
	CString strSelectedName = m_listCustomers.GetItemText(nIndex, 0);

	CString strMsg;
	strMsg.Format(_T("[%s] 회원의 정보를 삭제하시겠습니까?"), (LPCTSTR)strSelectedName);
	if (AfxMessageBox(strMsg, MB_YESNO | MB_ICONQUESTION) == IDNO) {
		return;
	}

	MYSQL* conn = ConnectDB();
	if (conn != NULL) {

		CString strSQL;
		
		strSQL.Format(_T("DELETE FROM customer WHERE card_number = '%s'"), (LPCTSTR)strSelectedCard);

		if (mysql_query(conn, (CT2A)strSQL) == 0) {
			
			if (mysql_affected_rows(conn) > 0) {
				AfxMessageBox(_T("회원정보 삭제 완료"));
			}
			else {
				AfxMessageBox(_T("회원을 찾을 수 없습니다."));
			}

			OnBnClickedButton2();

			m_strName = _T(""); m_strBirthday = _T("");  m_strCard_number = _T("");
			m_strPhone = _T(""); m_strHeight = _T("");
			UpdateData(FALSE);
		}
		else {
			
			CString strErr;
			strErr.Format(_T("삭제 실패: %S"), mysql_error(conn));
			AfxMessageBox(strErr);
		}
		mysql_close(conn);
	}
}


void ChealthDlg::OnBnClickedOk() { CDialogEx::OnOK(); }
void ChealthDlg::OnPaint() { CDialogEx::OnPaint(); }
HCURSOR ChealthDlg::OnQueryDragIcon() { return static_cast<HCURSOR>(m_hIcon); }


void ChealthDlg::OnBnClickedButtonConnect()
{
	
	if (m_bIsRunning == TRUE)
	{
		m_bIsRunning = FALSE;    
		m_net.ServerDisconnect(); 

		Sleep(300); 

		SetDlgItemText(IDC_BUTTON_CONNECT, _T("이더넷 연결"));

		SetWindowText(_T("회원 관리 프로그램 - [연결 종료됨]"));
		AfxMessageBox(_T("통신을 완전히 종료했습니다."));
		return; 
	}

	m_bIsRunning = TRUE; 
	SetDlgItemText(IDC_BUTTON_CONNECT, _T("연결 끊기"));
	AfxMessageBox(_T("회원 관리 프로그램 - [접속 대기 중...]"));

	AfxBeginThread(ThreadAccept, this);
}

UINT ChealthDlg::ThreadAccept(LPVOID pParam)
{
	ChealthDlg* pDlg = (ChealthDlg*)pParam;

	pDlg->m_net.ServerConnect(9998);

	if (pDlg->m_net.GetConnectStats() == 1) {
		pDlg->SetWindowTextW(_T("통신 연결 성공"));
		//ThreadReceive 진입
		AfxBeginThread(ThreadReceive, pDlg);
	}
	else {
		pDlg->m_bIsRunning = FALSE;
		pDlg->SetDlgItemText(IDC_BUTTON_CONNECT, _T("이더넷 연결"));
		AfxMessageBox(_T("통신 연결 실패"));
	}
	return 0; 
}

UINT ChealthDlg::ThreadReceive(LPVOID pParam)
{
	ChealthDlg* pDlg = (ChealthDlg*)pParam;

	// 1. [DB 파트] 연결되자마자 데이터 한 번 쫙 보내기
	MYSQL* conn = pDlg->ConnectDB();
	if (conn != NULL) {
		CString strSQL = _T("SELECT name, birthday, card_number, phone, height FROM customer");

		if (mysql_query(conn, (CT2A)strSQL) == 0) {
			MYSQL_RES* res = mysql_store_result(conn);
			MYSQL_ROW row;
			ST_PROTOCOL data;

			while ((row = mysql_fetch_row(res)) != NULL) {
				memset(&data, 0, sizeof(ST_PROTOCOL));

				// 데이터 복사 (안전하게 인덱스 0~4번 매칭)
				strncpy(data.name, (CT2A)CA2W(row[0] ? row[0] : "", 949), 10);
				strncpy(data.birthday, (CT2A)CA2W(row[1] ? row[1] : "", 949), 7);
				strncpy(data.cardnumber, (CT2A)CA2W(row[2] ? row[2] : "", 949), 19);
				strncpy(data.phone, (CT2A)CA2W(row[3] ? row[3] : "", 949), 13);
				data.height = (float)atof(row[4] ? row[4] : "0");

				// 전송
				pDlg->m_net.SendData((char*)&data, sizeof(ST_PROTOCOL));

				Sleep(50);
			}
			mysql_free_result(res);
		}
		mysql_close(conn); // 데이터 다 보냈으니 DB는 미리 닫습니다.
	}

	// 내 화면 리스트도 최신으로 갱신
	pDlg->PostMessage(WM_COMMAND, IDC_BUTTON2);

	// 수신 및 대기 루프
	while (pDlg->m_bIsRunning == TRUE) {

		// 데이터를 받을 바구니 준비 (헤더파일에 있는 BUFF_SIZE 15000 사용)
		char szBuffer[BUFF_SIZE];
		memset(szBuffer, 0, BUFF_SIZE);

		int nLen = pDlg->m_net.RecvData(szBuffer);

		if (nLen <= 0) {
			break;
		}
		else {

		}
	}

	//  [재가동 파트] 연결이 끊겼으니 다시 대기 상태로 전환
	if (pDlg->m_bIsRunning == TRUE) {
		pDlg->m_net.ServerDisconnect(); // 현재 소켓 정리
		pDlg->SetWindowText(_T("회원 관리 프로그램 - [재접속 대기 중...]"));
		AfxBeginThread(ThreadAccept, pDlg);
	}

	return 0;
}
//원클릭
void ChealthDlg::OnNMClickListCustomers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nIndex = pNMLV->iItem;

	if (nIndex != -1) {
		m_strName = m_listCustomers.GetItemText(nIndex, 0);
		m_strBirthday = m_listCustomers.GetItemText(nIndex, 1);
		m_strCard_number = m_listCustomers.GetItemText(nIndex, 2);
		m_strPhone = m_listCustomers.GetItemText(nIndex, 3);
		m_strHeight = m_listCustomers.GetItemText(nIndex, 4);

		if (!m_strCard_number.IsEmpty()) {

			CString strFirstPath;
			strFirstPath.Format(_T("C:\\Users\\admin\\source\\repos\\health\\health\\x64\\Debug\\download\\%s\\first\\face0.jpg"),
				(LPCTSTR)m_strCard_number);
			DownloadUserImage(m_strCard_number);

			DisplayImage(strFirstPath);
		}
		UpdateData(FALSE);
	}
	*pResult = 0;
}
//더블클릭
void ChealthDlg::OnNMDblclkListCustomers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int nRow = pNMItemActivate->iItem;
	
	if (nRow >= 0)
	{
		CString strImageName = m_listCustomers.GetItemText(nRow, 2);
		
		CGalleryDlg dlg;
		dlg.m_strCardNum = strImageName;

		// 창 띄우기
		dlg.DoModal();
	}
	*pResult = 0;
}
