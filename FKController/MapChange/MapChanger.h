/**
*	created:		2012-6-24   20:55
*	filename: 		MapChanger
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����糡�����л���ͼ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../FKControllerInclude.h"
#include "../../FKGame/Controller/IController.h"
#include "../../FKGame/Network/IClientSocket.h"
#include "../../FKGame/GameGlobal/IClientGlobal.h"
#include "../../FKGame/Network/IAppNetworkLink.h"
//------------------------------------------------------------------------
class CMapChanger : public IDataReceiver , public ITimerSink
{
private:
	struct SServerKey
	{
		char szIP[128];
		WORD wPort;				//���������ŵĶ˿�
	};		 
private:
	void BoardCast_Client_Play(void);   //������Ϸʱ,���¼�������Ϣ ,������ģ�鴦��
	void __DisConnectNetwork(void);
public:
	CMapChanger();
	virtual ~CMapChanger();

	BOOL OnReceived(const char* pData, int nLen);	

	virtual void	OnTimer(DWORD dwEventID); 	
};
//------------------------------------------------------------------------
extern CMapChanger  g_MapChanger;
//------------------------------------------------------------------------
