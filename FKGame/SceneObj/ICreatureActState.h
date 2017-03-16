/**
*	created:		2012-6-23   0:37
*	filename: 		ICreatureActState
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
// ����
struct ICreature;
//------------------------------------------------------------------------
// ����״̬��������
// δ֪״̬
#define CREATURE_STATEID_UNKNOW			1
// �ƶ�״̬
#define CREATURE_STATEID_MOVE			2
// ����״̬
#define CREATURE_STATEID_SIT			3
// վ��״̬
#define CREATURE_STATEID_STAND			4
// ����״̬
#define CREATURE_STATEID_DIE			5
// ����״̬
#define CREATURE_STATEID_RIDE			6
//------------------------------------------------------------------------
// ����
struct ICreatureActState
{
	// ����
	virtual BOOL				Create(ICreature * pCreature) = NULL;
	// ȡ�ñ�״̬ID
	virtual DWORD				GetStateID(void) = NULL;
	// ��Ϣ����
	virtual BOOL				OnMessage(DWORD dwMsg, LPCSTR buf, int nLen) = NULL;
	// ״̬��ʼ,�����¼�
	virtual void				Start(LPCSTR pContext, int nContextLen) = NULL;
	// ״̬����,ȡ�������¼�
	virtual void				End(LPCSTR pContext, int nContextLen) = NULL;
	// ��ʾ���¼�
	virtual BOOL				OnMoveItem(ICreature * pCreature, POINT ptTile) = NULL;
};
//------------------------------------------------------------------------