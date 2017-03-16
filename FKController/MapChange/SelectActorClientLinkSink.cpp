/**
*	created:		2012-6-24   21:34
*	filename: 		SelectActorClientLinkSink
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "SelectActorClientLinkSink.h"
//------------------------------------------------------------------------
CSelectActorClientLinkSink g_SelectActorClientLinkSink;
//------------------------------------------------------------------------
CSelectActorClientLinkSink::CSelectActorClientLinkSink(void)
{
	m_pClientLink = NULL;
	m_lActorID = 0;
	
}
//------------------------------------------------------------------------
// ��  �������ӷ�����ʧ��
// ��  ����szMsgΪ������Ϣ�������ݡ�dwErrorCodeΪsocket������룬�μ�Win32 API : GetLastError()
void CSelectActorClientLinkSink::OnConnectError(DWORD dwSocketID, LPCSTR szMsg, DWORD dwErrorCode)
{
	//char szTemp[256];
	//sprintf(szTemp, "���ӷ�����ʧ�ܡ�[SockID = %d�� Msg��%s��ErrorCode��%d]", dwSocketID, szMsg, dwErrorCode);
	//g_pGlobal->GetSoundEngine()->PlaySound(504);
}
//------------------------------------------------------------------------
// ��  �������ӽ���
void CSelectActorClientLinkSink::OnConnectOK(DWORD dwSocketID)
{
	//char szTemp[256];
	//sprintf(szTemp, "���ӷ������ɹ���[SockID = %d]", dwSocketID);
	//MessageBox(NULL, szTemp, "����ɹ�", MB_OK);
}
//------------------------------------------------------------------------
// ��  ������ǰ���ӶϿ�
// ��  ����szMsgΪ������Ϣ�������ݡ�dwErrorCodeΪsocket������룬�μ�Win32 API : GetLastError()
void CSelectActorClientLinkSink::OnConnectClose(DWORD dwSocketID, LPCSTR szMsg, DWORD dwErrorCode)
{	
	//char szTemp[256];
	//sprintf(szTemp, "����SelectActor������ʧ�ܡ�[SockID = %d�� Msg��%s��ErrorCode��%d]", dwSocketID, szMsg, dwErrorCode);	
	//g_pGlobal->m_pWndMgr->OnEvent(WND_ACTORSELECT, WE_CW_ACTOR_ERROR,
	//	(DWORD)szTemp, 0);
	//g_pGlobal->GetSoundEngine()->PlaySound(504);
}
//------------------------------------------------------------------------
// ��  �����������ݰ�
int	CSelectActorClientLinkSink::Send(LPCSTR buf, int nLen)
{
	if(m_pClientLink == NULL)
	{
		return 0;
	}

	return m_pClientLink->Send(buf, nLen);
}
//------------------------------------------------------------------------
// ��  ����ȡ��IClientLinkָ��, ʹ�÷������ḻ
IClientLink * CSelectActorClientLinkSink::GetClientLink(void)
{
	return m_pClientLink;
}
//------------------------------------------------------------------------
// ��  �����յ�һ�����ݰ�
// ��  ��������������ݰ����߳���IThreadRoute����ͬһ�̣߳�
// ����ֵ������һ������(ILinkWnd)�������ͨѶ����ILinkWnd���븺����һ���������ݰ�����
BOOL CSelectActorClientLinkSink::OnRecv(DWORD dwSocketID, LPCSTR buf, int nLen)
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
			char *pErrDesc = pError->m_szErrorDesc;
			//û���ش�����Ϣ,Ҫ�Լ����  #_#
			if (strlen(pErrDesc) == 0)
			{				
				pErrDesc = ErrDescTable[pError->m_wErrorCode];				
			}

			tPrintEventLn("%s   (%d) ", pErrDesc, pError->m_wErrorCode);

			if (pError->m_wErrorCode == 12)
			{   
				if (g_pGlobal->m_pWndMgr->IsWindowOpened(WND_ACTORCREATE))
				{
					g_pGlobal->m_pWndMgr->OnEvent(WND_ACTORCREATE,
						WE_CW_ACTORCREATE_ERROR, (DWORD)pErrDesc,0);					
					break;
				}				
			}

			g_pGlobal->m_pWndMgr->OnEvent(WND_ACTORSELECT, WE_CW_ACTOR_ERROR,
				(DWORD)pErrDesc, 0);

			//TraceLn(pError->m_szErrorDesc);
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
				//�����л���ѡ��̬, 
				case ROOTSELECTACTOR_SC_MAIN_SELECT:
				{						
					if (g_pGlobal->m_pWndMgr->IsWindowOpened(WND_ACTORCREATE))
					{//���if��Ҫ��Ϊ������ɫʱ�����
						g_pGlobal->m_pWndMgr->ShowWnd(WND_ACTORCREATE, SW_HIDE);
						g_pGlobal->m_pWndMgr->ShowWnd(WND_ACTORSELECT_BAK, SW_SHOW);
						g_pGlobal->m_pWndMgr->ShowWnd(WND_ACTORSELECT, SW_SHOW);
					}
					//�ش�ʱ,Ҫȥ������Ϣ���,��Щ���÷��͵�������
					g_ActorWndController.refreshActor(buf + 2 * sizeof(WORD), nLen - 2 * sizeof(WORD));
				}
				break;


				case ROOTSELECTACTOR_SC_MAIN_TURNRUN:
				{
					buf += 2*sizeof(WORD);
					nLen -= 2*sizeof(WORD);
					if (buf == NULL  || nLen != sizeof(SSActorSelectRes))
					{
						return FALSE;
					}					
					SSActorSelectRes *pRes = (SSActorSelectRes*)buf;
					
					//���½�ɫID, Ϊ�Ժ��ͼ�л�ʱ��׼��
					g_MapChangingLinkSink.setActorID(m_lActorID);  					

					//���ӳ�����
					m_pClientLink->CloseConnect();
					g_pGameControl->createConnection(SINKSTATE_SCENE);
					g_SceneClientLinkSink.setActorID(m_lActorID);						
					g_SceneClientLinkSink.m_pClientLink->Connect(pRes->szServerIP, pRes->wPort);					
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
BOOL CSelectActorClientLinkSink::SendMsg(WORD wMain, LPCSTR buf, int nLen)
{
	//if (g_SelectActorClientLinkSink.m_pClientLink == NULL)
	//{
	//	return FALSE;
	//}
	//
	//CS_SELECTACTOR_HEAD SelectActorHead;
	//SelectActorHead.m_wKeyMain = wMain;
	//
	//char temp[2048];
	//CDataArchive DataArchive(temp, 2048);
	//DataArchive.Write(&SelectActorHead, sizeof(CS_SELECTACTOR_HEAD));
	//DataArchive.Write(buf, nLen);	
	//return g_SelectActorClientLinkSink.Send(temp, DataArchive.GetBufferOffset());	
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CSelectActorClientLinkSink::startGame(int lActorID)
{
	//SSActorSelectContext ActorSelect;
	//ActorSelect.lActorID = lActorID;
	//m_lActorID = lActorID;
	//return SendMsg(ROOTSELECTACTOR_CS_MAIN_SELECT, (LPCSTR)&ActorSelect, sizeof(SSActorSelectContext));	
	return TRUE;
}
//------------------------------------------------------------------------
int CSelectActorClientLinkSink::GetType(void) 
{
	return NETWORKLINKTYPE_SELACTOR;
}
//------------------------------------------------------------------------