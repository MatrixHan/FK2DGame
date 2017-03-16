/**
*	created:		2012-6-24   21:32
*	filename: 		SelectActorClientLinkSink
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ڵ�¼״̬��������������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../FKControllerInclude.h"
#include "../MACSender/MACSender.h"
#include "../ExtraPassword/ExtraPasswordSaver.h"
#include "../../FKGame/Controller/IController.h"
#include "../../FKGame/Network/IClientSocket.h"
#include "../../FKGame/GameGlobal/IClientGlobal.h"
#include "../../FKGame/Network/IAppNetworkLink.h"
#include "../../FKGame/Event/IEvent.h"
//------------------------------------------------------------------------
class CSelectActorClientLinkSink : public IAppNetworkLink
{
private:
	int m_lActorID;

public:
	//��ѡ�������￪ʼ��Ϸ
	BOOL startGame(int lActorID);
	//������������Ϣ,���������װ��send����
	BOOL SendMsg(WORD wMain, LPCSTR buf, int nLen);
	// ���ӷ�����ʧ��
	// szMsgΪ������Ϣ�������ݡ�dwErrorCodeΪsocket������룬�μ�Win32 API : GetLastError()
	virtual void			OnConnectError(DWORD dwSocketID, LPCSTR szMsg, DWORD dwErrorCode);
	// ���ӽ���
	virtual void			OnConnectOK(DWORD dwSocketID);
	// �յ�һ�����ݰ�
	// ע��	����������ݰ����߳���IThreadRoute����ͬһ�̣߳�
	//		����һ������(ILinkWnd)�������ͨѶ����ILinkWnd���븺����һ���������ݰ�����
	virtual BOOL			OnRecv(DWORD dwSocketID, LPCSTR buf, int nLen);
	// ��ǰ���ӶϿ�
	// szMsgΪ������Ϣ�������ݡ�dwErrorCodeΪsocket������룬�μ�Win32 API : GetLastError()
	virtual void			OnConnectClose(DWORD dwSocketID, LPCSTR szMsg, DWORD dwErrorCode);
	// �������ݰ�
	virtual int				Send(LPCSTR buf, int nLen);
	// ȡ��IClientLinkָ��, ʹ�÷������ḻ
	virtual IClientLink *	GetClientLink(void);
	int GetType(void);
public:
	CSelectActorClientLinkSink(void);
public:	
	IClientLink *	m_pClientLink;
};
//------------------------------------------------------------------------
extern CSelectActorClientLinkSink g_SelectActorClientLinkSink;
//------------------------------------------------------------------------