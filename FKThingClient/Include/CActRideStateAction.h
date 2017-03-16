/**
*	created:		2012-7-4   1:25
*	filename: 		CActRideStateAction
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��Ӧ�ڴ�״̬�£�������Ϊ�¼��ķ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/SceneObj/ICreatureActState.h"
//------------------------------------------------------------------------
class CCreatureActRideState : public ICreatureActState
{	
public:
	// ����
	virtual BOOL				Create(ICreature * pCreature);
	
	// ȡ�ñ�״̬ID
	virtual DWORD				GetStateID(void);

	// ��Ϣ����
	virtual BOOL				OnMessage(DWORD dwMsg, LPCSTR buf, int nLen);

	// ״̬��ʼ,�����¼�
	virtual void				Start(LPCSTR pContext, int nContextLen);
	
	// ״̬����,ȡ�������¼�
	virtual void				End(LPCSTR pContext, int nContextLen);

	// ��ʾ���¼�
	virtual BOOL				OnMoveItem(ICreature * pCreature, POINT ptTile);

	CCreatureActRideState(void);

	~CCreatureActRideState(void);

private:
	// MAKE_MSGKID(MAINACTION_CIRCULTYPE_IC, MAINACTION_MSGID_MOVE)
	BOOL						OnMove_IC_Message(LPCSTR buf, int nLen);

	// MAKE_MSGKID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_DIE)
	BOOL						OnDie_CSC_Message(LPCSTR buf, int nLen);

private:
	ICreature	*				m_pMaster;

public:
	// ��Ϣ���˺���
	typedef BOOL (CCreatureActRideState::*FILTER_MESSAGE_PROC)(LPCSTR buf, int nLen);
	static  FILTER_MESSAGE_PROC	m_FilterMessageProc[0xFFFF + 1];

	// ��ʼ����Ϣӳ��
	static	void				InitFilterMessageProc(void);
};
//------------------------------------------------------------------------