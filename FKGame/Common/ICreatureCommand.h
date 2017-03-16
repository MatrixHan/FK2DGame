/**
*	created:		2012-7-2   22:03
*	filename: 		ICreatureCommand
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�������������,����Ҫ����ִ��ĳһ����ʱ��ֱ�ӵ��ô˽ӿ�����ڵ�������ķ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../SceneObj/ICreature.h"
//------------------------------------------------------------------------
// �������к�
enum
{
	// ��Ч�������к�
	CREATURE_COMMANDID_INVALID = 0,

	// ��������վ��֮�������к�
	CREATURE_COMMANDID_STAND,

	CREATURE_COMMANDID_CHANGEMOVESTYLE,

	// �����������к�
	CREATURE_COMMANDID_MAXID
};
//------------------------------------------------------------------------
// ��������ӿ�
struct ICreatureCommand
{	
	// ȡ�ò���ID
	virtual DWORD		GetCommandID(void) = NULL;

	// ִ������
	virtual int			OnCommand(ICreature * pCreature, LPCSTR pContext = NULL, int nLen = 0) = NULL;	
};
//------------------------------------------------------------------------