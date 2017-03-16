/**
*	created:		2012-7-4   1:19
*	filename: 		CActDieStateAction
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����������·״̬����Ӧ�¼�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/SceneObj/ICreatureActState.h"
//------------------------------------------------------------------------
class CCreatureActDieState : public ICreatureActState
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

	CCreatureActDieState(void);

	~CCreatureActDieState(void);

private:
	// MAKE_MSGID(MAINACTION_CIRCULTYPE_XXX, MAINACTION_MSGID_XXX)
	BOOL						OnGeneral_Vote_Message(LPCSTR buf, int nLen);

private:
	ICreature	*				m_pMaster;

public:
	// ��Ϣ���˺���
	typedef BOOL (CCreatureActDieState::*FILTER_MESSAGE_PROC)(LPCSTR buf, int nLen);
	static  FILTER_MESSAGE_PROC	m_FilterMessageProc[0xFFFF + 1];

	// ��ʼ����Ϣӳ��
	static	void				InitFilterMessageProc(void);
};
//------------------------------------------------------------------------