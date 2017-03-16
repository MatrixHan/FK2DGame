/**
*	created:		2012-6-29   21:16
*	filename: 		IContainerHolder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ӵ���ߣ�����װ��������Ʒ�����ֿⶼ��ʵ�֣�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../SceneObj/IThingPart.h"
//------------------------------------------------------------------------
// ����
struct IContainer;
//------------------------------------------------------------------------
// ����
struct IContainerHolder : public IThingPart
{
	// ȡ������
	virtual IContainer *	GetContainer(void) = NULL;

	// �Ƿ���ڣ���ͨ����Ʒid���������Ƿ�ﵽ��Ҫ��ĸ�����
	virtual BOOL			IsExistByGoodsID(DWORD dwGoodsID, DWORD dwNum) = NULL;

	// ��ȡ����Ʒid�����ڵĸ���
	virtual DWORD			GetNumByGoodsID(DWORD dwGoodsID) = NULL;

	// ���������Ƿ������ٸ���
	virtual BOOL			IsRemoveByGoodsID(DWORD dwGoodsID, DWORD dwNum) = NULL;

	// �������������ٵĸ���
	virtual DWORD			GetRemoveNumByGoodsID(DWORD dwGoodsID) = NULL;
};
//------------------------------------------------------------------------