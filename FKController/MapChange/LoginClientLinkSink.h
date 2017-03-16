/**
*	created:		2012-6-24   21:37
*	filename: 		LoginClientLinkSink
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
//------------------------------------------------------------------------
class CLoginClientLinkSink : public IAppNetworkLink, protected CMACSender, public CExtraPasswordSaver
{
private:
	BOOL SendMsg(WORD wMain, LPCSTR buf, int nLen);
	char m_username[USER_NAME_MAX_LEN + 1];
	char m_password[PASSWORD_MAX_LEN + 1];  

	//����MAC, �ʺź����뵽������
	void __SendMACAccountAndPassword(void);
public:
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

	CLoginClientLinkSink(void);

	void setData(char *user, char *password);

	int GetType(void);
	
public:	
	IClientLink *	m_pClientLink;
};
//------------------------------------------------------------------------
extern CLoginClientLinkSink g_LoginClientLinkSink;
//------------------------------------------------------------------------