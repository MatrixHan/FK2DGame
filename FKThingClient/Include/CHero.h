/**
*	created:		2012-7-3   1:42
*	filename: 		CHero
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "CActor.h"
#include "../../FKGame/Common/CreatureViewDef.h"
#include "../../FKGame/SceneObj/IThingPart.h"
#include "CCreatureActStateMachine.h"
//------------------------------------------------------------------------
class CHero : public CActor
{
public:
	// �ͷ�
	virtual void				Release(void);

	// 	˽�ж����ֳ�����
	virtual BOOL				OnSetPrivateContext(LPCSTR buf, int nLen);

	// ��Ϣ֪ͨ, ��ʹ�߼����������ת
	virtual void				OnMessage(DWORD dwMsg, LPCSTR buf, int nLen);

	// ִ������
	virtual int					OnCommand(DWORD dwCommand, LPCSTR buf = NULL, int nLen = 0);

	// ������ﲿ��, ����Ѵ���,������ʧ��
	virtual BOOL				AddPart(IThingPart * pPart);
	
	// ɾ�����ﲿ��, �����ͷ�
	virtual BOOL				RemovePart(DWORD dwPartID);
	
	// ȡ�����ﲿ��
	virtual IThingPart *		GetPart(DWORD dwPartID);

	// ���õ�ǰ�ж�״̬
	virtual BOOL				GotoActState(DWORD dwActStateID, LPCSTR pContext = NULL, int nLen = 0);

	// ȡ�õ�ǰ�ж�״̬
	virtual ICreatureActState * GetCurActState(void);

	// �������������������	
	virtual int					SetPropNum(DWORD dwPropID, int nValue);

	// ȡ�����������������
	virtual int					GetPropNum(DWORD dwPropID);

	// ����������ַ�������	
	virtual LPCSTR				SetPropStr(DWORD dwPropID, LPCSTR strPropValue);

	// ȡ��������ַ�������
	virtual LPCSTR				GetPropStr(DWORD dwPropID);

	// ����һ��������
	virtual BOOL				AddCreatureCommand(ICreatureCommand * pCreatureCommand);

	// visitor
	virtual void				Accept(IMapItemVisitor &visitor);

	// ���ô�������״̬
	virtual void				SetDying(BOOL bDying);

	// �Ƿ�����������
	virtual BOOL				IsDying(void);


	virtual void				OnCreatureMoveToTileCenter(const POINTS &ptCurTile, 
														   const POINTS &ptNextTile);
public:	
	// ����
	BOOL						Create(void);

	CHero(void);
	~CHero(void);

private:
	ICreatureCommand *			m_CreatureCommand[CREATURE_COMMANDID_MAXID];
	CCreatureActStateMachine	m_CreatureActStateMachine;	
};
//------------------------------------------------------------------------