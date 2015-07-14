// proj_transDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// Cproj_transDlg �Ի���




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


// Cproj_transDlg ��Ϣ�������

BOOL Cproj_transDlg::OnInitDialog()
{
	setlocale(LC_ALL,"");
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cproj_transDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR Cproj_transDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
bool BrowseForFolder(CString &sFolderPath, CString sTitle)
{
	BROWSEINFO bi;
	char Buffer[_MAX_PATH];
	//��ʼ����ڲ���bi
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;
	bi.lpszTitle = sTitle;
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
	//�û�ȡ��ѡ��
	if(!pIDList)
		return false;
	SHGetPathFromIDList(pIDList, Buffer);
	sFolderPath = Buffer;
	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc)))
		return false;
	//�ͷ��ڴ�
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
		//MessageBox("��ʧ��",NULL,MB_OK);
	}
}
void Cproj_transDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sFolderPath;
	CString sTitle;

	//BrowseForFolder( sFolderPath,  sTitle );	
	//m_strSourceDir = sFolderPath.GetBuffer();
	BrowseFile();
	UpdateData( FALSE );
}



void Cproj_transDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	xmlutil xu;
	
	xu.parsexml(srcFilePath_);
	xu.parseFilter(srcFilePath_);
	string targetpath = xu.getTargetFilePath(srcFilePath_);

	if (xu.generatexml(targetpath.c_str()))
	{
		MessageBox("ת�����̳ɹ�","�ɹ�",MB_OK);

	}
	else
	{
	
		MessageBox( "ת������ʧ��","ʧ��",MB_OK);
	}
	
	OnOK();
}
