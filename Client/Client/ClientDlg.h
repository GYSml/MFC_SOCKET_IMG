// ClientDlg.h : ͷ�ļ�
//

#pragma once


// CClientDlg �Ի���
class CClientDlg : public CDialog
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	


// ʵ��
protected:
	HICON m_hIcon;

	CListCtrl* list;

	void RecursiveFindFile(CString& strPath);


	void SendFile();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDispaly();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSend();
};
