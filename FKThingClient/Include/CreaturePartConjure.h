/**
*	created:		2012-7-4   0:48
*	filename: 		CreaturePartConjure
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ٻ�����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/SceneObj/IConjurePart.h"
#include "../../FKGame/Event/IEvent.h"
//------------------------------------------------------------------------
class CCreaturePart_Conjure : public IConjurePart,
							  public IEventActionListener
{
public:
	// ����
	virtual BOOL				Create(IThing * pMaster, char * pContext, int nLen);

	// �ͷ�
	virtual void				Release(void);

	// ȡ�ò���ID
	virtual DWORD				GetPartID(void);

	// ȡ�ñ�������
	virtual IThing *			GetMaster(void);

	// ����
	virtual BOOL				Activation(LPCSTR pContext, int nLen);

	// ����
	virtual BOOL				Freeze(void);

	// ȡ��˽�ж����ֳ�
	virtual BOOL				OnGetPrivateContext(LPSTR buf, int &nLen);

	// ȡ�ù��ö����ֳ�
	virtual BOOL				OnGetPublicContext(LPSTR buf, int &nLen);	

	// ȡ�����ݿ��ֳ�
	virtual BOOL				OnGetDBContext(LPSTR buf, int &nLen);

	// Message
	virtual	int					OnMessage(DWORD dwMsg, LPCSTR buf, int nLen);

	// ��ʼ���ͻ���
	virtual BOOL				InitClient(UID uidClient);

	// ȡ������
	virtual UID					GetChief(void);

	// �ܷ��ҵ�
	virtual BOOL				FindPawn(UID uidPawm);
	
	// ȡ��צ���б�
	virtual list< UID >			GetPawn(void);
 
	virtual void				OnAction(IEvent *pEvent);
public:
	CCreaturePart_Conjure(void);
	~CCreaturePart_Conjure(void);

private:
	// ����
	UID							m_uidChief;
	// צ���б�
	list< UID >					m_listPawn;
	// ����
	ICreature *					m_pMaster;
};
//------------------------------------------------------------------------