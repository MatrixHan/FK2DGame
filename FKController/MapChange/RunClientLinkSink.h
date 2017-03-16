/**
*	created:		2012-6-24   22:16
*	filename: 		RunClientLinkSink
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
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
#include "../../FKGame/Thread/IThreadRoute.h"
#include "../../FKFileSystem/Include/FKTempl.h"
//------------------------------------------------------------------------
struct stReceiverInfoNode
{
	char*	pData;
	int		nLen;
};
//------------------------------------------------------------------------
class CRunClientLinkSink : public IAppNetworkLink, public IEventSink
{
private:
	typedef list<IDataReceiver *>  RECEIVER_LIST;
	typedef vector<RECEIVER_LIST>  MESSAGE_LIST;

	//��Ϣ�б�  (������CMD_ROOT_THING ,  ������б�ᵥ������)
	MESSAGE_LIST m_MsgList;

	//CMD_ROOT_THING ����Ϣ�б�
	MESSAGE_LIST m_ThingMsgList;

	BOOL m_GracefulClose;

	CThreadRouteHelper		m_Thread;
	HANDLE					m_hEventWakeDecode;
	CList<stReceiverInfoNode,stReceiverInfoNode&> m_NodeList;
	CCriticalSectionLock	m_Lock;
	BOOL					m_bThreadRun;

public:
	void	LockReceiver(BOOL bLock);
	void	SetThreadRun(BOOL bRun);

	BOOL	Receiver();
	virtual void			OnEvent(DWORD dwEventID);

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

	//��Ҫ�������ȡ���ݵ�ģ��, ͨ���˺���ע���,�ſ��յ���Ϣ 
	BOOL AddReceiver(IDataReceiver *pReceiver, int MainMsgCode, int SubMsgCode) ;
	
	//�Ƴ�ģ�������
	BOOL RemoveReceiver(IDataReceiver *pReceiver, int MainMsgCode, int SubMsgCode);

	CRunClientLinkSink(void);
	virtual ~CRunClientLinkSink(void);

	//��Ϊƽ���ر�
	void SetGraceful(BOOL flag);

	int GetType(void);

public:	
	IClientLink *	m_pClientLink;
};
//------------------------------------------------------------------------
extern CRunClientLinkSink g_RunClientLinkSink;
//------------------------------------------------------------------------