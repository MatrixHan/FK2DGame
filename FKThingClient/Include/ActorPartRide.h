/**
*	created:		2012-7-4   0:38
*	filename: 		ActorPartRide
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������˲���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IRidePart.h"
#include "IHorse.h"
#include "CHorse.h"
#include "../../FKGame/Event/IEvent.h"
#include <list>
using std::list;
//------------------------------------------------------------------------
class CActorPart_Ride : public IRidePart,
						public IEventActionListener
{
	struct HORSEINFOR
	{
		int		nIndex;
		CHorse	horse;
	};
	typedef list< HORSEINFOR >	TLIST_HORSE;
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

	// ȡ����ƥ
	virtual IHorse *		GetHorse(int nIndex);

	// �Ƿ���������
	virtual BOOL			IsRiding(void);


	virtual void			OnAction(IEvent * pEvent);

public:
	CActorPart_Ride(void);
	virtual ~CActorPart_Ride(void);

protected:
	// ��
	virtual void			Mount(SC_MSG_GETRIDEDATA_CONTEXT::HORSEDATA horse);
	// ��
	virtual void			Dismount(void);

private:
	// ����
	virtual void			OnAction_Mount(IEvent * pEvent);
	// ����
	virtual void			OnAction_Dismount(IEvent * pEvent);
	// ȡ���������
	virtual void			OnAction_GetRideData(IEvent * pEvent);

	virtual void			OnMessage_Show(LPCSTR buf, int nLen);
protected:
	IActor *				m_pMaster;
	TLIST_HORSE				m_listHorse;		// ӵ����ƥ�б�	
	int						m_nCurMountIndex;	// ��ǰ���index, Ϊ0��ʾδ��
	int						m_nOldPart1_no;
	int						m_nOldPart2_no;
};
//------------------------------------------------------------------------