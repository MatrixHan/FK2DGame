/**
*	created:		2012-6-23   0:32
*	filename: 		IActor
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "ICreature.h"
//------------------------------------------------------------------------
// �ӿڶ���
struct IActor : public ICreature
{
	// ȡ���ʺŵ�DB ID
	virtual DWORD	GetUserDBID(void) = NULL;
	// ȡ�ý�ɫ��DB ID
	virtual DWORD	GetActorDBID(void) = NULL;
	// ȡ�ý�ɫ������ID
	virtual DWORD	GetActorClanID(void) = NULL;
};
//------------------------------------------------------------------------