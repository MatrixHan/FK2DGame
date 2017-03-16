/**
*	created:		2012-7-4   0:24
*	filename: 		ActorPartEquip
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������װ������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/Container/IContainerHolder.h"
#include "../../FKGame/SceneObj/IActor.h"
#include "../../FKGame/Event/IEvent.h"
#include "../../FKGame/Common/CreatureViewDef.h"
//------------------------------------------------------------------------
class CActorPart_Equip : public IContainerHolder,
						 public IEventActionListener,
						 public IEventResponseListener
{
public:
	// ����
	virtual BOOL			Create(IThing * pMaster, char * pContext, int nLen);
	
	// �ͷ�
	virtual void			Release(void);
	
	// ȡ�ò���ID
	virtual DWORD			GetPartID(void);
	
	// ȡ�ñ�������
	virtual IThing *		GetMaster(void);
	
	// ����
	virtual BOOL			Activation(LPCSTR pContext, int nLen);
	
	// ����
	virtual BOOL			Freeze(void);

	// ȡ��˽�ж����ֳ����������ѵĿͻ���
	virtual BOOL			OnGetPrivateContext(LPSTR buf, int &nLen);
	
	// ȡ�ù��ö����ֳ����㲥���ͻ���	
	virtual BOOL			OnGetPublicContext(LPSTR buf, int &nLen);

	// ȡ�����ݿ��ֳ�
	virtual BOOL			OnGetDBContext(LPSTR buf, int &nLen);

	// Message
	virtual	int				OnMessage(DWORD dwMsg, LPCSTR buf, int nLen);

	// ��ʼ���ͻ���
	virtual BOOL			InitClient(UID uidClient);


	virtual IContainer *	GetContainer(void);

	// �Ƿ���ڣ���ͨ����Ʒid���������Ƿ�ﵽ��Ҫ��ĸ�����
	virtual BOOL			IsExistByGoodsID(DWORD dwGoodsID, DWORD dwNum);

	// ��ȡ����Ʒid�����ڵĸ���
	virtual DWORD			GetNumByGoodsID(DWORD dwGoodsID);

	// ���������Ƿ������ٸ���
	virtual BOOL			IsRemoveByGoodsID(DWORD dwGoodsID, DWORD dwNum);

	// �������������ٵĸ���
	virtual DWORD			GetRemoveNumByGoodsID(DWORD dwGoodsID);

	virtual void			OnAction(IEvent * pEvent);

	virtual void			OnResponse(IEvent *pEvent);

public:
	CActorPart_Equip(void);
	virtual ~CActorPart_Equip(void);

private:
	IActor *				m_pMaster;	
};
//------------------------------------------------------------------------