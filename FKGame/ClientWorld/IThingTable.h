/**
*	created:		2012-6-25   23:59
*	filename: 		IThingTable
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ͻ����߼���������б�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <vector>
using namespace std;
//------------------------------------------------------------------------
// ����
struct  IThing;
typedef vector<IThing *>	TVECTOR_THING;
//------------------------------------------------------------------------
// ����
struct IThingTable
{
	// �ر�
	virtual void			Close(void) = NULL;

	// ����
	virtual IThing *		Find(UID uid) = NULL;

	// ���
	virtual IThing *		Add(IThing * pThing) = NULL;

	// �Ƴ�
	virtual BOOL			Remove(UID uid) = NULL;

	// �Ƴ�
	virtual BOOL			Remove(IThing* pThing) = NULL;

	// ȡ��thing�б�
	virtual TVECTOR_THING *	GetThingList(void) = NULL;
};
//------------------------------------------------------------------------
