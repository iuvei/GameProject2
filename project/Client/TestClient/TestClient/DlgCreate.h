#pragma once


// CDlgCreate �Ի���

class CDlgCreate : public CDialog
{
	DECLARE_DYNAMIC(CDlgCreate)

public:
	CDlgCreate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCreate();

// �Ի�������
	enum { IDD = IDD_DLG_CREATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnZhan();
	afx_msg void OnBnClickedBtnFa();
	afx_msg void OnBnClickedBtnDao();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_strCharName;
	int m_nSex;
	int m_nCareer;
};
