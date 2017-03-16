/**
*	created:		2012-6-24   21:12
*	filename: 		MapChangingLinkSink
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "MapChangingLinkSink.h"
//------------------------------------------------------------------------
CMapChangingLinkSink g_MapChangingLinkSink;
//------------------------------------------------------------------------
CMapChangingLinkSink::CMapChangingLinkSink()
{
	m_pClientLink = NULL;	
	m_username[0] = '\0';
	m_password[0] = '\0';
	m_lActorID = 0;
	memset(&m_info, 0, sizeof(SServerKey));
	m_dwpActor = 0;

	m_nRetryCount = 0;
	m_nErrorRetryCount = 0;
	m_bReConnectingFlag = FALSE;
}
//------------------------------------------------------------------------
CMapChangingLinkSink::~CMapChangingLinkSink()
{

}
//------------------------------------------------------------------------
void CMapChangingLinkSink::OnConnectError(DWORD dwSocketID, LPCSTR szMsg, DWORD dwErrorCode)
{
	/*
	char szTemp[256];
	sprintf(szTemp, "CMapChangingLinkSink::OnConnectError���ӷ�����ʧ�ܡ�[SockID = %d�� Msg��%s��ErrorCode��%d]", dwSocketID, szMsg, dwErrorCode);
	tPrintEventLn(szTemp);

	if (!m_bReConnectingFlag)
	{
		g_pGlobal->m_pWndMgr->CustomMsgBox("��ͼ�л�״̬����(ERROR)");
		g_pGlobal->GetSoundEngine()->PlaySound(504);
	}	
	*/
}
//------------------------------------------------------------------------
void CMapChangingLinkSink::OnConnectOK(DWORD dwSocketID)
{
	tPrintEventLn("����OK,  CMapChangingLinkSink::OnConnectOK ");
}
//------------------------------------------------------------------------
void CMapChangingLinkSink::OnConnectClose(DWORD dwSocketID, LPCSTR szMsg, DWORD dwErrorCode)
{
	/*
	m_nErrorRetryCount--;
	if (m_nErrorRetryCount >= 0)
	{
		m_bReConnectingFlag = TRUE;
		__ConnectToTargetMap();
	}
	else
	{
		char szTemp[256];
		sprintf(szTemp, "CMapChangingLinkSink::OnConnectClose���ӷ�����ʧ�ܡ�[SockID = %d�� Msg��%s��ErrorCode��%d]", dwSocketID, szMsg, dwErrorCode);
		tPrintEventLn(szTemp);
		g_pGlobal->m_pWndMgr->CustomMsgBox("��ͼ�л�״̬����(Close)");
		g_pGlobal->GetSoundEngine()->PlaySound(504);
	}
	*/
}
//------------------------------------------------------------------------
int	CMapChangingLinkSink::Send(LPCSTR buf, int nLen)
{
	if(m_pClientLink == NULL)
	{
		return 0;
	}

	return m_pClientLink->Send(buf, nLen);
}
//------------------------------------------------------------------------
IClientLink * CMapChangingLinkSink::GetClientLink(void)
{
	return m_pClientLink;
}
//------------------------------------------------------------------------
void CMapChangingLinkSink::setData(char *user, char *password)
{
	//strncpy(m_username, user, USER_NAME_MAX_LEN + 1);	
	//if (password)  //������ǵ�һ�ε�¼, ����Ϊ��, ������һ�ε�����Ϳ�����
	//{
	//	strncpy(m_password, password, PASSWORD_MAX_LEN);
	//	m_password[PASSWORD_MAX_LEN] = '\0';
	//}	
}
//------------------------------------------------------------------------
void CMapChangingLinkSink::setActorID(int ActorID)
{
	m_lActorID = ActorID;
}
//------------------------------------------------------------------------
BOOL CMapChangingLinkSink::OnRecv(DWORD dwSocketID, LPCSTR buf, int nLen)
{
	/*
	if(nLen < sizeof(WORD))
	{
		return FALSE;
	}

	WORD wMsgRoot = *((WORD *)(buf));
	switch(wMsgRoot)
	{
	case CMD_ROOT_ERROR:
		{
			TraceLn("�л���ͼ������ , CMapChangingLinkSink::OnRecv, CMD_ROOT_ERROR");

			SC_ERRORCODE_MSGFORMAT * pError = (SC_ERRORCODE_MSGFORMAT *)(buf);
			//TraceLn(pError->m_szErrorDesc);
			char *pErrDesc = pError->m_szErrorDesc;
			//û���ش�����Ϣ,Ҫ�Լ����  #_#
			if (strlen(pErrDesc) == 0)
			{				
				pErrDesc = ErrDescTable[pError->m_wErrorCode];				
			}

			tPrintEventLn("%s   (%d) ", pErrDesc, pError->m_wErrorCode);

			g_pGlobal->m_pWndMgr->OnEvent(WND_LOGIN, WE_CW_LOGIN_ERRORINFO, 
				(DWORD)pErrDesc, 0);
		}
		break;
	case CMD_ROOT_SELECTACTOR:
		{
			if(nLen < sizeof(SC_SELECTACTOR_HEAD))
			{
				return FALSE;
			}

			SC_SELECTACTOR_HEAD * pSelectMsg = (SC_SELECTACTOR_HEAD *)(buf);			
			switch(pSelectMsg->m_wKeyMain)
			{
			case ROOTSELECTACTOR_SC_MAIN_TURNRUN:
				{
					tPrintEventLn("�л���ͼ���ת����̬ , CMapChangingLinkSink::OnRecv, ROOTSELECTACTOR_SC_MAIN_TURNRUN");

					//�л�������̬ȥ
					g_RunClientLinkSink.m_pClientLink = m_pClientLink;
					m_pClientLink->GotoNextState(&g_RunClientLinkSink);
					g_pGlobal->m_pAppNetworkLink = &g_RunClientLinkSink;

					//�ر�ƽ���ر�ģʽ, ���ӶϿ�ʱ����ʾ����
					g_RunClientLinkSink.SetGraceful(FALSE);   

					//����ģ��
					g_pGlobal->GetChatClient()->ShowWindow(CW_SHOW);

					//ʹ�õ�һ�ֲ���ģʽ
					g_pGameControl->changeControlMode(1);	

				}
				break;		

			case ROOTSELECTACTOR_SC_MAIN_ENTERCODE:
				{
					//�յ�ENTERCODE
					tPrintEventLn("CMapChangingLinkSink::OnRecv �յ�  ENTERCODE");	
					if (buf == NULL  || nLen != sizeof(SC_SELECTACTOR_ENTERCODE))
					{
						return FALSE;
					}

					//ͣ����ʱ��, ����ʱ�䵽�������
					g_pGlobal->m_TimerAxisHelper.m_pTimeAxis->KillTimer(TIMER_EVENT_WAITENTERCODE_EXPIRE, this);
					m_bReConnectingFlag = FALSE;

					SC_SELECTACTOR_ENTERCODE *pEnterCode = (SC_SELECTACTOR_ENTERCODE*)buf;
					SetMacPosition(pEnterCode->m_dwEnterCode);
					__SendMacToServer();
					__SendAccountAndPassword();

				}
				break;
			}
		}
		break;

	default:break;
	}
	*/
	return TRUE;
}
//------------------------------------------------------------------------
void CMapChangingLinkSink::OnTimer(DWORD dwEventID)
{
	/*
	if (dwEventID == TIMER_EVENT_WAITENTERCODE_EXPIRE)
	{
		g_pGlobal->m_TimerAxisHelper.m_pTimeAxis->KillTimer(TIMER_EVENT_WAITENTERCODE_EXPIRE, this);
		m_nRetryCount--;
		if (m_nRetryCount >= 0)
		{
			tPrintEventLn("MapChangingLinkSink�ȴ���ʱ, ����...");
			m_bReConnectingFlag = TRUE;
			__ConnectToTargetMap();
		}
		else
		{
			tPrintEventLn("MapChangingLinkSink�ȴ���ʱ, ��������ʧ��...");			
			g_pGlobal->m_pAppNetworkLink->GetClientLink()->CloseConnect();
			g_pGlobal->m_pWndMgr->ShowWnd(WND_CHAT, SW_HIDE);			
			g_pGlobal->m_pWndMgr->ShowWnd(WND_LOADING, SW_HIDE);			
			g_pGlobal->m_pWndMgr->ShowWnd(WND_LOGIN_BAK, SW_SHOW);
			g_pGlobal->m_pWndMgr->ShowWnd(WND_LOGIN, SW_SHOW);			
			g_pGlobal->m_pWndMgr->OnEvent(WND_LOGIN, WE_CW_LOGIN_ERRORINFO, 
				(DWORD)"���ӷ�������ʱ, �����µ�¼!", 0);	
		}
	}
	*/
}
//------------------------------------------------------------------------
void CMapChangingLinkSink::SetServer(const char *pData, int nLen)
{
	if (nLen != sizeof(SServerKey))
	{
		tPrintEventLn("CMapChangingLinkSink::SetServer, Ŀ����������ݳ���");
		return;
	}

	memcpy(&m_info, pData, nLen);
}
//------------------------------------------------------------------------
void CMapChangingLinkSink::__ConnectToTargetMap(void)
{
	/*
	if (m_info.wPort != 0)
	{
		try
		{
			g_pGlobal->m_TimerAxisHelper.m_pTimeAxis->SetTimer(TIMER_EVENT_WAITENTERCODE_EXPIRE, this, 10000, "CMapChangingLinkSink::__ConnectToTargetMap");

			g_pGameControl->createConnection(SINKSTATE_MAPCHINGING);		
			g_MapChangingLinkSink.m_pClientLink->Connect(m_info.szIP, m_info.wPort);	
		}
		catch (...) {
			tPrintEventLn("CMapChangingLinkSink::__ConnectToTargetMap�����Ƿ�");			
		}

	}
	else
	{
		tPrintEventLn("CMapChangingLinkSink::__ConnectToTargetMap, ���������ݴ�");
	}
	*/
}
//------------------------------------------------------------------------
void CMapChangingLinkSink::OnAction(IEvent *pEvent)
{	 
	/*
	if (pEvent == NULL)
	{
		tPrintEventLn("CMapChangingLinkSink::OnAction, pEvent == NULL");
		return;
	}

	int nLen;
	SC_MSG_PLAYERLOGOUT_CONTEXT *pContext;
	try
	{
		pContext = (SC_MSG_PLAYERLOGOUT_CONTEXT *)pEvent->GetContext(nLen);
	}
	catch (...) {
		tPrintEventLn("CMapChangingLinkSink::OnAction()    pEvent->GetContext() �Ƿ�");

	}

	if (pContext && nLen == sizeof(SC_MSG_PLAYERLOGOUT_CONTEXT))
	{		
		if (pContext->dwStateFlag == 3)
		{
			WORD wEventID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_PLAYERLOGOUT);
			try
			{
				g_pGlobal->m_pEventServer->RemoveListener(this, wEventID, SRC_TYPE_ID_ACTOR, m_dwpActor);
			}
			catch(...)
			{
				tPrintEventLn("CMapChangingLinkSink::OnAction()    pEventServer->RemoveListener() �Ƿ�");
			}

			m_dwpActor = 0;		

			m_nRetryCount = 1;   //������������, ��5����ղ���entercode������		
			m_nErrorRetryCount = 1; //������������, ���ӳ��������� 1 ��
			__ConnectToTargetMap();	
		}
	}
	else
	{
		tPrintEventLn("CMapChangingLinkSink::OnAction, ���յ��ֳ�����");
	}
	*/
}
//------------------------------------------------------------------------
void CMapChangingLinkSink::RegisterLogonMsg(void)
{
	//if (g_pGlobal->m_pEventServer)
	//{
	//	WORD wEventID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_PLAYERLOGOUT);
	//	m_dwpActor = (DWORD)g_pGlobal->m_pPlayer;
	//	g_pGlobal->m_pEventServer->AddListener(this, wEventID, SRC_TYPE_ID_ACTOR, m_dwpActor, "CMapChangingLinkSink::RegisterLogonMsg111");
	//}
}
//------------------------------------------------------------------------
int CMapChangingLinkSink::GetType(void)
{
	return NETWORKLINKTYPE_MAPCHANGING;
}
//------------------------------------------------------------------------
void CMapChangingLinkSink::__SendMacToServer(void)
{
	//CS_SELECTACTOR_MAC_HEAD  MacHead(const_cast<char *>(GetMacPackage()));
	//char temp[2048];
	//CDataArchive DataArchive(temp, 2048);
	//DataArchive.Write(&MacHead, sizeof(CS_SELECTACTOR_MAC_HEAD));
	//m_pClientLink->Send(temp, DataArchive.GetBufferOffset());
}
//------------------------------------------------------------------------
void CMapChangingLinkSink::__SendAccountAndPassword(void)
{
	//IQQEnterGame *pQQ = g_pGlobal->GetQQEnterGame();
	//if (pQQ == NULL)
	//{
	//	tPrintEventLn("QQ��¼ģ�����");
	//	return;
	//}

	//char szMsgBuf[1024];
	//CDataArchive dataarchive(szMsgBuf, sizeof(szMsgBuf));		  

	//if (pQQ->IsQQEnter())
	//{
	//	CS_SELECTACTOR_SELECT_HEAD_QQ selecthead(m_lActorID);	
	//	selecthead.lFirstLogin = 0;
	//	NETTOOLS::GetMACAddress(selecthead.szSerialNO);
	//	strcpy(selecthead.m_szQQ, pQQ->GetQQ());
	//	int nQQDataLen;
	//	const char *ptQQData = pQQ->GetQQData(nQQDataLen);
	//	memcpy(selecthead.m_szQQData, ptQQData, nQQDataLen);
	//	selecthead.m_nQQDataLen = nQQDataLen;

	//	MD5 md5;
	//	char temp[256];
	//	memcpy(temp, ptQQData, nQQDataLen);
	//	md5.update((unsigned char *)temp, nQQDataLen);
	//	char szMAC[20];
	//	NETTOOLS::GetMACAddress(szMAC);
	//	md5.update((unsigned char *)szMAC, 17);
	//	md5.finalize();		
	//	strncpy(selecthead.m_szQQDataMD5, md5.hex_digest().c_str(), 32);
	//	strncpy(selecthead.m_szSureCode, GetExtraPassword(), 32);  //������
	//	dataarchive.Write(&selecthead, sizeof(CS_SELECTACTOR_SELECT_HEAD_QQ));	
	//} 
	//else
	//{
	//	CS_SELECTACTOR_SELECT_HEAD selecthead(m_lActorID);	
	//	selecthead.lFirstLogin = 0;
	//	NETTOOLS::GetMACAddress(selecthead.szSerialNO);
	//	strncpy(selecthead.m_szUserName, m_username, USER_NAME_MAX_LEN);
	//	strncpy(selecthead.m_szPassword, m_password, PASSWORD_MAX_LEN);	
	//	strncpy(selecthead.m_szSureCode, GetExtraPassword(), 32);  //������
	//	dataarchive.Write(&selecthead, sizeof(CS_SELECTACTOR_SELECT_HEAD));	
	//}

	//if (m_pClientLink->Send(szMsgBuf, dataarchive.GetBufferOffset()) == FALSE)
	//{
	//	tPrintEventLn("CMapChangingLinkSink �����ʺ�����ʧ��");
	//}	
}
//------------------------------------------------------------------------