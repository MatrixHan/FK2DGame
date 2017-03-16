/**
*	created:		2012-6-29   14:16
*	filename: 		IThingPart
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�߼����񲿼��ӿ�, �����߼����񲿼��Ľӿڱ���̳д˽ӿ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
typedef __int64 UID;
//------------------------------------------------------------------------
// �������кţ�THINGPARTID + Ӧ�õ��߼������ + ����Ӧ�ã�
enum
{
	// ��Ч�Ĳ������к�
	THINGPARTID_THING_INVALID = 0,
	
	// �����ƶ����� = 1
	THINGPARTID_CREATURE_MOVE,
	
	// ����·������������ = 2
	THINGPARTID_CREATURE_PATHMANAGER,
	
	// ��ҹ㲥���� = 3
	THINGPARTID_ACTOR_BROADCAST,

	// ���＼�ܿ��������� = 4
	THINGPARTID_CREATURE_SKILLCONTROL,

	// �߼�����㲥���� = 5
	THINGPARTID_THING_BROADCAST,

	// ����AI���� = 6
	THINGPARTID_CREATURE_AI,

	// ������������ = 7
	THINGPARTID_CREATURE_DIE,

	// �������ȴϵͳ = 8
	THINGPARTID_ACTOR_COLDTIMESYSTERM,

	// �������� = 9
	THINGPARTID_ACTOR_RELIVE,

	// ����״̬���� = 10
	THINGPARTID_CREATURE_STATE,

	// ����װ���� = 11
	THINGPARTID_ACTOR_EQUIP,

	// ������Ʒ�� = 12
	THINGPARTID_ACTOR_PACKET,

	// PK���� = 13
	THINGPARTID_CREATURE_PK,

	// ����ܲ���ID = 14
	THINGPARTID_ACTOR_WORKSKILL,

	// �ֿⲿ�� = 15
	THINGPARTID_ACTOR_WAREHOUSE,

	// �ٻ����� = 16
	THINGPARTID_CREATURE_CONJURE,

	// ʱ��������� = 17
	THINGPARTID_ACTOR_TIMEMARKING,

	// ���ϵͳ������ = 18
	THINGPARTID_MONSTER_ENMITY,	

	// ��̯���� = 19
	THINGPARTID_ACTOR_STALL,

	// ���Ͳ��� = 20
	THINGPARTID_MONSTER_PRIZEHOLD,

	// ����ͷ������ = 21
	THINGPARTID_ACTOR_CLANPATE,

	// ����ҹ㲥���� = 22
	THINGPARTID_NONACTOR_BROADCAST,

	// ��˲��� = 23
	THINGPARTID_ACTOR_RIDE,

	// ��֤���� = 24
	THINGPARTID_ACTOR_VALIDATION,

	// ���Ĳ������к�
	/****************************************************************
	******��Ҫ�������ID������DB��ʹ������Ʒ��ID��װ����ID***********
	****************************************************************/
	THINGPARTID_THING_MAXID
};
//------------------------------------------------------------------------
// �˺��Ƿ����ݸ��ͻ���Э�������ID
#define THINGPARTID_PROPID_LOWID		4000
//------------------------------------------------------------------------
// ����
struct IThing;
//------------------------------------------------------------------------
// �ӿڶ���
struct IThingPart
{
	// ����
	virtual BOOL		Create(IThing * pMaster, char * pContext, int nLen) = NULL;

	// �ͷ�
	virtual void		Release(void) = NULL;

	// ȡ�ò���ID
	virtual DWORD		GetPartID(void) = NULL;

	// ȡ�ñ�������
	virtual IThing *	GetMaster(void) = NULL;

	// ����
	virtual BOOL		Activation(LPCSTR pContext, int nLen) = NULL;

	// ����
	virtual BOOL		Freeze(void) = NULL;

	// ȡ��˽�ж����ֳ�
	virtual BOOL		OnGetPrivateContext(LPSTR buf, int &nLen) = NULL;
	
	// ȡ�ù��ö����ֳ�
	virtual BOOL		OnGetPublicContext(LPSTR buf, int &nLen) = NULL;	

	// ȡ�����ݿ��ֳ�
	virtual BOOL		OnGetDBContext(LPSTR buf, int &nLen) = NULL;

	// Message
	virtual	int			OnMessage(DWORD dwMsg, LPCSTR buf, int nLen) = NULL;

	// ��ʼ���ͻ���
	virtual BOOL		InitClient(UID uidClient) = NULL;
};
//------------------------------------------------------------------------