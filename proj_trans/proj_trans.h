// proj_trans.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cproj_transApp:
// �йش����ʵ�֣������ proj_trans.cpp
//

class Cproj_transApp : public CWinApp
{
public:
	Cproj_transApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cproj_transApp theApp;