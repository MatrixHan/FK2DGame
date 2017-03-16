/**
*	created:		2012-7-4   1:16
*	filename: 		CCreatureActStateMachine
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������Ϊ״̬��.
					������Ϊ״̬����, ������·,�ܲ�,����,վ��,����
					��������״̬�µ���һ״̬���¼��Ķ��ģ��¼�������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "CActSitStateAction.h"
#include "CActStandStateAction.h"
#include "CActDieStateAction.h"
#include "CActRideStateAction.h"
#include "CActMoveStateAction.h"
//------------------------------------------------------------------------
class CCreatureActStateMachine
{
public:
	// ����
	BOOL							Create(ICreature * pMaster);
	// ���õ�ǰ�ж�״̬
	BOOL							GotoActState(DWORD dwActStateID, LPCSTR pContext, int nLen);
	// ȡ�õ�ǰ�ж�״̬
	ICreatureActState *				GetCurActState(void);
	// ȡ�õ�ǰ�ж�״̬ID
	DWORD							GetCurActStateID(void);
public:
	CCreatureActStateMachine(void);
	~CCreatureActStateMachine(void);
private:
	// ����״̬
	CCreatureActSitState			m_CreatureActSitState;	
	// վ��״̬
	CCreatureActStandState			m_CreatureActStandState;
	// ����״̬
	CCreatureActDieState			m_CreatureActDieState;
	// �ƶ�״̬
	CCreatureActMoveState			m_CreatureActMoveState;
	// ����״̬
	CCreatureActRideState			m_CreatureActRideState;

private:
	// ����
	ICreature *						m_pMaster;
	// ��ǰ��Ϊ״̬
	ICreatureActState *				m_pCurCreatureActState;	
};
//------------------------------------------------------------------------