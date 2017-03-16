/**
*	created:		2012-7-3   0:40
*	filename: 		BoxProp
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�������Զ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Macro.h"
//------------------------------------------------------------------------
enum
{
	BOX_CREATE  , // ����״̬
	BOX_OPENING , // ���ڴ�״̬
	BOX_OPENED  , // �Ѿ���
	BOX_CLOSED  , // ���ڹر�״̬
	BOX_COMMAND   // ��������
};
//------------------------------------------------------------------------
enum
{
	BOX_COMMAND_CLEARRIGHT = 0,
	BOX_COMMAND_ADDRIGHT      ,
	BOX_COMMAND_DELRIGHT      ,
	BOX_COMMAND_MAX
};
//------------------------------------------------------------------------
enum
{
	BOX_TYPE_MINE     = 1, // ��
	BOX_TYPE_MEDICINE    , // ��ҩ
	BOX_TYPE_TREASURE    , // ����
	BOX_TYPE_TASK        , // �������
	BOX_TYPE_DYNAMIC       // ��̬���ɵĵ�װ����
};
//------------------------------------------------------------------------
enum
{
	BOX_ID_UNKNOWN           = 0, // δ֪���߲��ù��ĵ�
	BOX_ID_CREATE_BY_MONSTER    , // ����ɱ������
	BOX_ID_CREATE_BY_ACTOR1     , // ��ɱ������
	BOX_ID_CREATE_BY_ACTOR2     , // ��ɱ������
	BOX_ID_CREATE_BY_TEAM1      , // ��ɱ������(���)
	BOX_ID_CREATE_BY_TEAM2        // ��ɱ������(���)
};
//------------------------------------------------------------------------
struct SBOX_CONTEXT
{
	UID uidBox; // ����
	DWORD dwState; // ����״̬
};
//------------------------------------------------------------------------
struct SBOX_PROPERTY_PUBLIC
{
	char	szName[MAX_NAME_LEN]; // ����
	POINTS  ptLoc; // λ��
	DWORD	dwResID1; // ����ID
	DWORD   dwResID2; // ����id
	DWORD   dwTypeId; // ���ӵ����ͣ��󡢲�ҩ�����ϱ��䡢�����װ
	DWORD   dwBoxId; // �����µ�id�������װ����Ҫ��̬����
	DWORD   dwState; // ״̬���رա����ڴ�(������)����
};
//------------------------------------------------------------------------
struct SBOX_PROPERTY_LIST : public SBOX_PROPERTY_PUBLIC
{
	DWORD   dwOpenTime; // ��ʱ�䣬�򿪽���������Ҫ
	DWORD   dwCloseTime; // ǿ�ƹر�ʱ�䣬��ֹ��Ҵ򿪲��ر�
	DWORD   dwRefresh; // ����ʱ�䣬���Ϊ-1������
	DWORD   dwRightTime; // ��̬���ӵ�Ȩ����Ϣʱ��
};
//------------------------------------------------------------------------