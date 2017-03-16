/**
*	created:		2012-6-24   21:12
*	filename: 		MapChangingLinkSink
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��������л���ͼʱ,�����������
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
const int TIMER_EVENT_WAITENTERCODE_EXPIRE = 99;  //��ʱ���¼�, �ȴ�������
//------------------------------------------------------------------------
class CMapChangingLinkSink : public IAppNetworkLink ,public ITimerSink, public IEventActionListener, protected CMACSender, public CExtraPasswordSaver
{
private:
	struct SServerKey
	{
		char szIP[128];
		WORD wPort;				//���������ŵĶ˿�
	};		 
	SServerKey m_info;			//Ŀ���ͼ���ڵķ�����
	int m_nRetryCount;			//��Ϊ�Ȳ���������, ���������ļ���
	int m_nErrorRetryCount;		//��Ϊ���ӳ���, ���������ļ���
	BOOL m_bReConnectingFlag;	//��������
	char m_username[USER_NAME_MAX_LEN + 1];
	char m_password[PASSWORD_MAX_LEN + 1];
	int m_lActorID;
private:
	//�յ���������������READY��Ϣ, ��ʱ�������·�����
	void __ConnectToTargetMap(void);
	//��������ָ��, �����ڼ���������Ϣ
	DWORD m_dwpActor;
	//�����ʺź����뵽������
	void __SendAccountAndPassword(void);
	//��MAC���͵�������
	void __SendMacToServer(void);
public:
	CMapChangingLinkSink();
	virtual ~CMapChangingLinkSink();

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
	//�����ʺź�����
	void setData(char *user, char *password);
	//���ý�����Ϸ�Ľ�ɫID
	void setActorID(int ActorID);
	//��ʱ���ص�
	void	OnTimer(DWORD dwEventID);
	//����Ŀ���ͼ���ڵķ�����
	void    SetServer(const char *pData, int nLen);
	virtual void OnAction(IEvent *pEvent);
	//ע�����������Ϣ 
	void RegisterLogonMsg(void);
	int GetType(void);
public:	
	IClientLink *	m_pClientLink;
};
//------------------------------------------------------------------------
extern CMapChangingLinkSink g_MapChangingLinkSink;
//------------------------------------------------------------------------