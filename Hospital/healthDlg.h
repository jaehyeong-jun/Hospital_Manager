#pragma once
#include "BYMFtp.h"

#include "afxcmn.h"
#include "BYMEthernet.h"

#include <winsock2.h>
#include <mysql.h>




class ChealthDlg : public CDialogEx
{
public:
	MYSQL* ConnectDB();

	ChealthDlg(CWnd* pParent = nullptr);
	BYMEthernet m_net; // 이더넷 객체 생성

	void DownloadUserImage(CString strImageName);
	void DisplayImage(CString strPath);


	static UINT ThreadAccept(LPVOID pParam);
	static UINT ThreadReceive(LPVOID pParam);
	BOOL m_bIsRunning; // 스레드 작동 제어 플래그
	
	BYMFtp m_ftp;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HEALTH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	
protected:
	HICON m_hIcon;	
	CFont m_font; // 폰트 객체

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// 에디트 박스 연결 변수
	CString m_strName;
	CString m_strBirthday;
	CString m_strPhone;
	CString m_strCard_number;
	CString m_strHeight;

	// 리스트 컨트롤 (딱 한 번만 선언되어야 함)
	CListCtrl m_listCustomers;

	// 버튼 및 이벤트 함수
	afx_msg void OnBnClickedButton1(); // 저장
	afx_msg void OnBnClickedButton2(); // 조회
	afx_msg void OnBnClickedButton3(); // 삭제
	afx_msg void OnBnClickedOk();

	// 리스트 클릭 이벤트 함수 선언
	afx_msg void OnNMClickListCustomers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnStnClickedStaticFace();
	afx_msg void OnNMDblclkListCustomers(NMHDR *pNMHDR, LRESULT *pResult);
};
