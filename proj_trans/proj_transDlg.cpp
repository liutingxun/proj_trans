// proj_transDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "proj_trans.h"
#include "proj_transDlg.h"
#include <locale>
#include "xmlutil.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cproj_transDlg 对话框




Cproj_transDlg::Cproj_transDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cproj_transDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cproj_transDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cproj_transDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &Cproj_transDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &Cproj_transDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Cproj_transDlg 消息处理程序

BOOL Cproj_transDlg::OnInitDialog()
{
	setlocale(LC_ALL,"");
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cproj_transDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cproj_transDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR Cproj_transDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
bool BrowseForFolder(CString &sFolderPath, CString sTitle)
{
	BROWSEINFO bi;
	char Buffer[_MAX_PATH];
	//初始化入口参数bi
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;
	bi.lpszTitle = sTitle;
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
	//用户取消选择
	if(!pIDList)
		return false;
	SHGetPathFromIDList(pIDList, Buffer);
	sFolderPath = Buffer;
	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc)))
		return false;
	//释放内存
	lpMalloc->Free(pIDList);
	lpMalloc->Release();
	return true;
}

void Cproj_transDlg::BrowseFile()
{

	CFileDialog Dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "VCXPROJ(*.vcxproj)|*.vcxproj|ALL Files(*.*)|*.*");
	if (Dlg.DoModal() == IDOK)
	{
		srcFilePath_ = Dlg.GetPathName();
		SetDlgItemText(IDC_EDIT1,srcFilePath_);
	}
	else
	{
		//MessageBox("打开失败",NULL,MB_OK);
	}
}
void Cproj_transDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sFolderPath;
	CString sTitle;

	//BrowseForFolder( sFolderPath,  sTitle );	
	//m_strSourceDir = sFolderPath.GetBuffer();
	BrowseFile();
	UpdateData( FALSE );
}



void Cproj_transDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	xmlutil xu;
	
	xu.parsexml(srcFilePath_);
	xu.parseFilter(srcFilePath_);
	string targetpath = xu.getTargetFilePath(srcFilePath_);

	if (xu.generatexml(targetpath.c_str()))
	{
		MessageBox("转换工程成功","成功",MB_OK);

	}
	else
	{
	
		MessageBox( "转换工程失败","失败",MB_OK);
	}
	
	OnOK();
}
