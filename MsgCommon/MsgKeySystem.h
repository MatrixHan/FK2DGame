/**
*	created:		2012-6-30   1:51
*	filename: 		MsgKeySystem
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ϵͳ�¼�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
// �ͻ��˽�����Ϸ
#define MSG_SYSTEM_CLIENT_PLAY						1

// �ͻ���С��
#define	MSG_SYSTEM_CLIENT_BACKSELEHERO				2

// ������Ϣ����
#define MSG_SYSTEM_CLIENT_MSGCENTER_POP				3

// ��������¼���ķ�
#define MSG_SYSTEM_ZONESERVER_LOGIN_CENTER			4

// �����������ķ��Ͽ�
#define MSG_SYSTEM_ZONESERVER_LOGOUT_CENTER			5

// �ͻ���С�����
#define	MSG_SYSTEM_CLIENT_BACKSELEHERO_COMPLETE		6
//------------------------------------------------------------------------
struct SYSTEM_CLIENT_MSGCENTER_CONTEXT
{
	int  nQueueID;
	int	 nIndex;	
	char Buffer[512];
};
//------------------------------------------------------------------------
/////////////////////////////////////////////////////////
//// ROOTTHING_SC_MAIN_GAMEWORLD ����Ϣ
/////////////////////////////////////////////////////////
/*///////////////////////////////////////////////////////
��  Ϣ    �룺GAMEWORLD_MSGID_LOOK_ACTOR_EQUIP
��        ��������ͻ�������
///////////////////////////////////////////////////////*/
#define MAINACTION_MSGID_CHANGEWEATHER				1
//------------------------------------------------------------------------