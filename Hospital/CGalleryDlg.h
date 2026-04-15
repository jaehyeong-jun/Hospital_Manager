#pragma once


// CGalleryDlg 대화 상자

class CGalleryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGalleryDlg)

public:
	CGalleryDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CGalleryDlg();


	// 갤러리 작동을 위한 필수 변수 2개
	int m_nCurrentIndex;        // 지금 화면에 떠 있는 사진 번호

	// 사진을 액자에 딱 맞춤
	void DrawGalleryImage(int nIndex);


	CString m_strCardNum;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GALLERY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnPrev();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnDblclickStaticPic0();
	afx_msg void OnStnDblclickStaticPic1();
	afx_msg void OnStnDblclickStaticPic2();
	afx_msg void OnStnDblclickStaticPic3();
	afx_msg void OnStnDblclickStaticPic4();
	afx_msg void OnStnDblclickStaticPic5();
	afx_msg void OnStnClickedStaticPic2();
	afx_msg void OnStnClickedStaticPic3();
};
