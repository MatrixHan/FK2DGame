/**
*	created:		2012-6-24   21:38
*	filename: 		LoginClientLinkSink
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "LoginClientLinkSink.h"
//------------------------------------------------------------------------
CLoginClientLinkSink g_LoginClientLinkSink;
//------------------------------------------------------------------------
LPCSTR szLoginErrDesc = "�޷����ӷ�����(L), ���Ժ��ٵ�¼! ��������û����, ���µ�ͷ�!";
//------------------------------------------------------------------------
void CLoginClientLinkSink::setData(char *user, char *password)
{
	strncpy(m_username, user, USER_NAME_MAX_LEN + 1)  ;
	if (password)  //������ǵ�һ�ε�¼, ����Ϊ��, ������һ�ε�����Ϳ�����
	{
		strncpy(m_password, password, PASSWORD_MAX_LEN);  //ֻ��32���ַ�
		m_password[PASSWORD_MAX_LEN] = '\0';
	}	
}
//------------------------------------------------------------------------
CLoginClientLinkSink::CLoginClientLinkSink(void)
{
	m_pClientLink = NULL;
	m_username[0] = '\0';
	m_password[0] = '\0';

}
//------------------------------------------------------------------------
// ��  �������ӷ�����ʧ��
// ��  ����szMsgΪ������Ϣ�������ݡ�dwErrorCodeΪsocket������룬�μ�Win32 API : GetLastError()
void CLoginClientLinkSink::OnConnectError(DWORD dwSocketID, LPCSTR szMsg, DWORD dwErrorCode)
{
	//char szTemp[256];
	//sprintf(szTemp, "����LOGIN������ʧ�ܡ�[SockID = %d�� Msg��%s��ErrorCode��%d]", dwSocketID, szMsg, dwErrorCode);
	//tPrintEventLn(szTemp);
	//g_pGlobal->m_pWndMgr->OnEvent(WND_LOGIN, WE_CW_LOGIN_ERRORINFO, 
	//	(DWORD)szLoginErrDesc, 0);	

	//g_pGlobal->GetSoundEngine()->PlaySound(504);
}
//------------------------------------------------------------------------
// ��  �������ӽ���
void CLoginClientLinkSink::OnConnectOK(DWORD dwSocketID)
{	
	/*
	//�Ѿ��ɹ�����, ֪ͨ��¼�����������
	g_pGlobal->m_pWndMgr->OnEvent(WND_LOGIN, WE_CW_LOGIN_CLEARPASSWORD, 0, 0);
	
	//�汾�������Ч, ���ϵ�¼, �����Ч, ���������ȡ�汾��,�Ա�����ٵ�¼
	if (g_DllVersion.isValid())
	{
		SendMsg(ROOTLOGIN_CS_MAIN_CLIENTVER, NULL, 0);
	}
	else
	{
		g_pGlobal->m_pWndMgr->OnEvent(WND_LOGIN, WE_CW_LOGIN_ERRORINFO, 
			(DWORD)"�޷���ȡ��Ϸ�汾��Ϣ, �������Զ�����.����������,����ͷ���ϵ!", 0);	
	}
	*/
}
//------------------------------------------------------------------------
void CLoginClientLinkSink::__SendMACAccountAndPassword(void)
{	
	/*
	IQQEnterGame *pQQ = g_pGlobal->GetQQEnterGame();
	if (pQQ == NULL)
	{
		tPrintEventLn("QQ��¼ģ�����");
		return;
	}
 
	if (pQQ->IsQQEnter())
	{ 
		SSUserLoginContext_QQ Context;
		strcpy(Context.szUserName, pQQ->GetQQ());
		//ȡQQ����
		int nQQDataLen;
		const char *ptQQData = pQQ->GetQQData(nQQDataLen);
		memcpy(Context.szQQData, ptQQData, nQQDataLen);	
		Context.nQQDataLen  = nQQDataLen;

		memcpy(Context.szNetworkCard, GetMacPackage(), 512);	
		char temp[256];
		memcpy(temp, ptQQData, nQQDataLen);
		MD5 md5;		
		md5.update((unsigned char *)temp, nQQDataLen);
		char szMAC[20];
		NETTOOLS::GetMACAddress(szMAC);
		md5.update((unsigned char *)szMAC, 17);
		md5.finalize();
		strncpy(Context.szQQDataMD5, md5.hex_digest().c_str(), 32);
		Context.lPassPodPwd = atol(g_PassportMgr.GetPassport());

		strncpy(Context.szMD5SureCode, GetExtraPassword(), 32);  //������

		SendMsg(ROOTLOGIN_CS_MAIN_USERLOGIN_QQ, (LPCSTR)&Context, sizeof(SSUserLoginContext_QQ));		
		 
	}
	else
	{
		SSUserLoginContext Context;	
		strcpy(Context.szUserName, m_username);
		memcpy(Context.szPassword, m_password, 32);		
		memcpy(Context.szNetworkCard, GetMacPackage(), 512);		
		Context.lPassPodPwd = atol(g_PassportMgr.GetPassport());
		
		strncpy(Context.szMD5SureCode, GetExtraPassword(), 32);  //������		
		
		SendMsg(ROOTLOGIN_CS_MAIN_USERLOGIN, (LPCSTR)&Context, sizeof(SSUserLoginContext));
	}
	*/
}
//------------------------------------------------------------------------
BOOL CLoginClientLinkSink::SendMsg(WORD wMain, LPCSTR buf, int nLen)
{
	/*
	if (g_LoginClientLinkSink.m_pClientLink == NULL)
	{
		return FALSE;
	}
	
	CS_LOGIN_HEAD LoginHead(wMain);
	
	char temp[2048];
	CDataArchive DataArchive(temp, 2048);
	DataArchive.Write(&LoginHead, sizeof(CS_LOGIN_HEAD));
	DataArchive.Write(buf, nLen);
	
	return g_LoginClientLinkSink.Send(temp, DataArchive.GetBufferOffset());	
	*/

	return TRUE;
}
//------------------------------------------------------------------------
// ��  ������ǰ���ӶϿ�
// ��  ����szMsgΪ������Ϣ�������ݡ�dwErrorCodeΪsocket������룬�μ�Win32 API : GetLastError()
void CLoginClientLinkSink::OnConnectClose(DWORD dwSocketID, LPCSTR szMsg, DWORD dwErrorCode)
{
	//char szTemp[256];
	//sprintf(szTemp, "���ӷ�����ʧ�ܡ�[SockID = %d�� Msg��%s��ErrorCode��%d]", dwSocketID, szMsg, dwErrorCode);
	//tPrintEventLn(szTemp);
	//g_pGlobal->m_pWndMgr->OnEvent(WND_LOGIN, WE_CW_LOGIN_ERRORINFO, 
	//	(DWORD)szLoginErrDesc, 0);	

	//g_pGlobal->GetSoundEngine()->PlaySound(504);
}
//------------------------------------------------------------------------
// ��  �����������ݰ�
int	CLoginClientLinkSink::Send(LPCSTR buf, int nLen)
{
	if(m_pClientLink == NULL)
	{
		return 0;
	}

	return m_pClientLink->Send(buf, nLen);
}
//------------------------------------------------------------------------
// ��  ����ȡ��IClientLinkָ��, ʹ�÷������ḻ
IClientLink * CLoginClientLinkSink::GetClientLink(void)
{
	return m_pClientLink;
}
//------------------------------------------------------------------------
// ��  �����յ�һ�����ݰ�
// ��  ��������������ݰ����߳���IThreadRoute����ͬһ�̣߳�
// ����ֵ������һ������(ILinkWnd)�������ͨѶ����ILinkWnd���븺����һ���������ݰ�����
BOOL CLoginClientLinkSink::OnRecv(DWORD dwSocketID, LPCSTR buf, int nLen)
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
			SC_ERRORCODE_MSGFORMAT * pError = (SC_ERRORCODE_MSGFORMAT *)(buf);
			
			//TraceLn(pError->m_szErrorDesc);
			char *pErrDesc = pError->m_szErrorDesc;
			//û���ش�����Ϣ,Ҫ�Լ����  #_#
			if (strlen(pErrDesc) == 0)
			{				
				pErrDesc = ErrDescTable[pError->m_wErrorCode];				
			}
			
			tPrintEventLn("%s   (%d) ", pErrDesc, pError->m_wErrorCode);

			if (g_pGlobal->GetQQEnterGame()->IsQQEnter())
			{
				//ɾ��"QQ��¼��"�Ĵ���
				g_pGlobal->m_pWndMgr->OnEvent(WND_CUSTOMMSGBOX, WE_AW_MSGBOX_CLEARONCE, MAKECOURSE(POPCOURSE_QQLOGIN, 0), 0);
				g_pGlobal->m_pWndMgr->CustomMsgBox(pErrDesc, 
					1, 0, 0, new CQQLoginErrorCallBack);				
			}
			else
			{
				g_pGlobal->m_pWndMgr->OnEvent(WND_LOGIN, WE_CW_LOGIN_ERRORINFO, 
					(DWORD)pErrDesc, 0);
			}

						
		}
		break;

	case CMD_ROOT_LOGIN:
		{
			buf += sizeof(WORD);
			nLen -= sizeof(WORD);
			
			WORD wMain = *(WORD*)buf;
			buf += sizeof(WORD);
			nLen -= sizeof(WORD);
			
			switch(wMain) {
			case ROOTLOGIN_SC_MAIN_TURNSELECT:
				{						
					//���������ݵ��շ�����ת���� "��ɫѡ��" ״̬
					g_SelectActorClientLinkSink.m_pClientLink = m_pClientLink;
					m_pClientLink->GotoNextState(&g_SelectActorClientLinkSink);
					g_pGlobal->m_pAppNetworkLink = &g_SelectActorClientLinkSink;

					//������ʾ����
					g_pGlobal->m_pWndMgr->CreateWnd(WND_ACTORSELECT_BAK);
					g_pGlobal->m_pWndMgr->CreateWnd(WND_ACTORSELECT);					
					g_ActorWndController.Create();
					g_ActorWndController.refreshActor(buf, nLen);
					g_pGlobal->m_pWndMgr->ShowWnd(WND_LOGIN, SW_HIDE);
					g_pGlobal->m_pWndMgr->ShowWnd(WND_LOGIN_BAK, SW_HIDE);
					//g_pGlobal->m_pWndMgr->ShowWnd(WND_LOGIN_BAK_NOTICE, SW_HIDE);
					g_pGlobal->m_pWndMgr->ShowWnd(WND_ACTORSELECT_BAK, SW_SHOW);
					g_pGlobal->m_pWndMgr->ShowWnd(WND_ACTORSELECT, SW_SHOW);					
					
					//��¼���ڿ�����Ϊ�ų�������,����������һ��
					g_pGlobal->m_pWndMgr->OnEvent(WND_LOGIN, WE_CW_LOGIN_UNLOCK, 0, 0);					
					g_pGlobal->m_pWndMgr->OnEvent(WND_CUSTOMMSGBOX, WE_CW_MSGBOX_CLEAR, 0, 0);
					
					break; 
				}
			case ROOTLOGIN_SC_MAIN_LISTNUM:
				{
					if (buf != NULL && nLen == sizeof(SSListLocationContext))
					{
						g_pGlobal->m_pWndMgr->OnEvent(WND_LOGIN, WE_CW_LOGIN_LOCK, 0, 0);//������¼����,��ֹ�û�����
						SSListLocationContext *pContext = (SSListLocationContext*)buf;
						g_pGlobal->m_pWndMgr->OnEvent(WND_LOGIN, WE_CW_LOGIN_WAIT, pContext->nLocation + 1, 0);
					}
					
				}
				break;
 
			case ROOTLOGIN_SC_MAIN_CLIENTVER:
				{					
					if (buf == NULL)
					{
						return FALSE;
					}

					if (!g_DllVersion.isVersionPass(buf + 4, nLen - 4))  //��ʱ����4�ֽ������
					{
						g_pGlobal->m_pWndMgr->OnEvent(WND_LOGIN, WE_CW_LOGIN_ERRORINFO, 
							(DWORD)"��Ϸ�汾����,�뼰ʱ����! ����º��������, ����ϵ�ͷ�!", 0);
						return TRUE;
					}						
					
					DWORD dwMacPos;
					memcpy(&dwMacPos, buf, 4);
					SetMacPosition(dwMacPos);
					__SendMACAccountAndPassword();     //���������ʺ�MAC
					break;
				}

			default:
				break;
			}
		}
		break;
	default:
		{
			tPrintEventLn("CLoginClientLinkSink::OnRecv�յ�������Ϣ %d", wMsgRoot);
		}
		break;
	}
	*/
	return TRUE;
}
//------------------------------------------------------------------------
int CLoginClientLinkSink::GetType(void)
{
	return NETWORKLINKTYPE_LOGIN;
}
//------------------------------------------------------------------------