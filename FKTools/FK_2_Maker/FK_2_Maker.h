
// FK_2_Maker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFK_2_MakerApp:
// �йش����ʵ�֣������ FK_2_Maker.cpp
//

class CFK_2_MakerApp : public CWinAppEx
{
public:
	CFK_2_MakerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFK_2_MakerApp theApp;