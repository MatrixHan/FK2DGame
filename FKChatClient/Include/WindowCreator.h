/**
*	created:		2012-6-28   10:28
*	filename: 		WindowCreator
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���ڴ�����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
#include "../../FKGame/WndSys/WndSys.h"
#include "../../FKResManager/IFKResManager.h"
//------------------------------------------------------------------------
// ���ڴ�����
class CWindowCreator  
{
public:
	CWindowCreator(void);
	virtual ~CWindowCreator(void);

public:
	static BOOL CreateMainWnd(XWindow * pParent, XWindow * pMe, IResObject * pResObject);
	static BOOL CreateControl(XWindow * pParent, XWindow * pMe, IResObject * pResObject);
	static BOOL CreateGeneralWnd(XWindow * pParent, XWindow * pMe, IResObject * pResObject);
};
//------------------------------------------------------------------------