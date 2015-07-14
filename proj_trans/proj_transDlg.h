// proj_transDlg.h : 头文件
//

#pragma once


// Cproj_transDlg 对话框
class Cproj_transDlg : public CDialog
{
// 构造
public:
	Cproj_transDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PROJ_TRANS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
