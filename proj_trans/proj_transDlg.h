// proj_transDlg.h : ͷ�ļ�
//

#pragma once


// Cproj_transDlg �Ի���
class Cproj_transDlg : public CDialog
{
// ����
public:
	Cproj_transDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PROJ_TRANS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	void BrowseFile();

private:
	CString srcFilePath_;
	afx_msg void OnBnClickedOk();
};
