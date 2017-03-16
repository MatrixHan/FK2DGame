/**
*	created:		2012-6-29   20:21
*	filename: 		IEquipment
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IGoods.h"
//------------------------------------------------------------------------
// ����
struct IEffect;
struct ICreature;
//------------------------------------------------------------------------
// ����
struct IEquipment : public IGoods
{
	// ȡ���;������ֵ
	virtual void		GetEndure(int &nCur, int &nCurMax) = NULL;

	// ȡ��Ч��
	virtual IEffect *	GetEffect(int nIndex) = NULL;

	// ����Ƿ���װ������װ��
	virtual	BOOL		CanEquip(ICreature * pCreature, int nEquipPos) = NULL;

	// ȡ�þ����ȼ�
	virtual int			GetSmeltLevel(void) = NULL;

	// ȡ��װ���Ĳ����
	virtual DWORD		GetSlotLeft(void) = NULL;
};
//------------------------------------------------------------------------