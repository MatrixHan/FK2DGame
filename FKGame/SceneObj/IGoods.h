/**
*	created:		2012-6-28   2:43
*	filename: 		IGoods
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��Ʒ�ӿ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IMapThing.h"
//------------------------------------------------------------------------
// ����ӿ�
struct IGoods : public IMapThing
{
	// ȡ����ƷID
	virtual DWORD		GetGoodsID(void) = NULL;

	// ȡ����Ʒ����
	virtual LPCSTR		GetName(void) = NULL;

	// ȡ����Ʒ������������
	virtual int			GetPropNum(DWORD dwPropID) = NULL;

	// ȡ����Ʒ���ַ�������
	virtual LPCSTR		GetPropStr(DWORD dwPropID) = NULL;

	// ȡ����Ʒ����
	virtual DWORD		GetNumber(void) = NULL;

	// ȡ����Ʒ��ʹ������
	virtual DWORD		GetUseLock(void) = NULL;

	// ȡ�ð󶨱�־�������鿴�󶨱�־
	virtual  BOOL		GetBindFlag(DWORD dwFlag) = NULL;  
};
//------------------------------------------------------------------------

