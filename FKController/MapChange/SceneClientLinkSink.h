/**
*	created:		2012-6-24   21:25
*	filename: 		SceneClientLinkSink
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���ӵ�������ʱ��ͨ�Ž���̬
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
class CSceneClientLinkSink : public IAppNetworkLink, protected CMACSender, public CExtraPasswordSaver
{
private:
	char m_username[USER_NAME_MAX_LEN + 1];
	char m_password[PASSWORD_MAX_LEN + 1];
	int m_lActorID;

	//����Ƿ���������, ����������ӵĻ�,�򲻻������������������
	//�Խ��ͷ������ĸ���
	BOOL m_bConnecting;   

	//�����ʺź����뵽������
	void __SendAccountAndPassword(void);
	
	//��MAC���͵�������
	void __SendMacToServer(void);

public:
	CSceneClientLinkSink();
	virtual ~CSceneClientLinkSink();


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

	//����ȥ����ĳ����ַ, �����ʱ�Ѿ�������,��û�з�Ӧ
	void TryConnect(LPCSTR szIP, int nPort);

	virtual int GetType(void) ;

public:	
	IClientLink *	m_pClientLink;


};
//------------------------------------------------------------------------
extern CSceneClientLinkSink g_SceneClientLinkSink;
//------------------------------------------------------------------------
