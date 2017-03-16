/**
*	created:		2012-6-24   22:22
*	filename: 		RunClientLinkSink
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "RunClientLinkSink.h"
//------------------------------------------------------------------------
CRunClientLinkSink g_RunClientLinkSink;
//------------------------------------------------------------------------
LPCSTR  szRunErrDesc = "�������(S)���ӶϿ�, �����µ�¼��Ϸ! ������ٶȳ���,����ͷ���ϵ!";
//------------------------------------------------------------------------
CRunClientLinkSink::CRunClientLinkSink(void)
		//:m_MsgList(CMD_ROOT_MAX), m_ThingMsgList(MAX_ROOTTHING_MAIN_KEY)
{
	m_bThreadRun = FALSE;
	m_pClientLink = NULL;
	m_GracefulClose = FALSE;
	m_hEventWakeDecode = NULL;
	
	m_Thread.Close();
	if(!m_Thread.Create(NULL))
	{
		return;
	}

	DWORD dwEventID;
	if(!m_Thread.m_pThreadRoute->AddEvent(static_cast<IEventSink*>(this),dwEventID))
		return;

	m_hEventWakeDecode = m_Thread.m_pThreadRoute->GetEventHandle(dwEventID);

	if(!m_Thread.m_pThreadRoute->Start())
	{
		m_Thread.Close();
		return;
	}

	return;
}
//------------------------------------------------------------------------
CRunClientLinkSink::~CRunClientLinkSink(void)
{
	m_Thread.Close();
}
//------------------------------------------------------------------------
// ��  �������ùرձ���, ����رձ���ΪTRUE, ��Ͽ�����ʱ���ᵯ��������Ϣ
void CRunClientLinkSink::SetGraceful(BOOL flag)
{
	m_GracefulClose = flag;
}
//------------------------------------------------------------------------
// ��  �������ӷ�����ʧ��
// ��  ����szMsgΪ������Ϣ�������ݡ�dwErrorCodeΪsocket������룬�μ�Win32 API : GetLastError()
void CRunClientLinkSink::OnConnectError(DWORD dwSocketID, LPCSTR szMsg, DWORD dwErrorCode)
{
	//char szTemp[256];
	//sprintf(szTemp, "CRunClientLinkSink::OnConnectError, �������ͨ�ŷ�������ErrorCode��%d", dwErrorCode);
	//tPrintEventLn(szTemp);
	//
	//g_pGlobal->m_pWndMgr->CustomMsgBox(szRunErrDesc);
	//g_pGlobal->GetSoundEngine()->PlaySound(504);
}
//------------------------------------------------------------------------
// ��  �������ӽ���
void CRunClientLinkSink::OnConnectOK(DWORD dwSocketID)
{
	//char szTemp[256];
	//sprintf(szTemp, "���ӷ������ɹ���[SockID = %d]", dwSocketID);
	//MessageBox(NULL, szTemp, "����ɹ�", MB_OK);
}
//------------------------------------------------------------------------
// ��  ������ǰ���ӶϿ�
// ��  ����szMsgΪ������Ϣ�������ݡ�dwErrorCodeΪsocket������룬�μ�Win32 API : GetLastError()
void CRunClientLinkSink::OnConnectClose(DWORD dwSocketID, LPCSTR szMsg, DWORD dwErrorCode)
{
	//if (!m_GracefulClose)
	//{
	//	char szTemp[256];
	//	sprintf(szTemp, "CRunClientLinkSink::OnConnectClose, ������������ӹرա�ErrorCode��%d", dwErrorCode);
	//	tPrintEventLn(szTemp);
	//	g_pGlobal->m_pWndMgr->CustomMsgBox(szRunErrDesc);
	//	g_pGlobal->GetSoundEngine()->PlaySound(504);
	//}
}
//------------------------------------------------------------------------
// ��  �����������ݰ�
int	CRunClientLinkSink::Send(LPCSTR buf, int nLen)
{
	if(m_pClientLink == NULL)
	{
		return 0;
	}

	return m_pClientLink->Send(buf, nLen);
}
//------------------------------------------------------------------------
// ��  ����ȡ��IClientLinkָ��, ʹ�÷������ḻ
IClientLink * CRunClientLinkSink::GetClientLink(void)
{
	return m_pClientLink;
}
//------------------------------------------------------------------------
//��Ҫ�������ȡ���ݵ�ģ�� , ͨ���˺���ע���,�ſ��յ���Ϣ 
BOOL CRunClientLinkSink::AddReceiver(IDataReceiver *pReceiver, int MainMsgCode, int SubMsgCode)
{
	////��ȷ����CMD_ROOT_THING ����,���Ƕ���ͨ�����,��Ҫ����ı�ȷ����
	//MESSAGE_LIST  *pList  = NULL;
	//int           MsgCode = 0;
	//if (MainMsgCode == CMD_ROOT_THING)
	//{
	//	pList   = &m_ThingMsgList;
	//	MsgCode = SubMsgCode;
	//}
	//else
	//{
	//	pList = &m_MsgList;
	//	MsgCode = MainMsgCode;
	//}


	////�ڱ��������ע����
	//RECEIVER_LIST::iterator itor = find((*pList)[MsgCode].begin(), 
	//	(*pList)[MsgCode].end(), pReceiver);
	//
	//if (itor != (*pList)[MsgCode].end())
	//{//�������Ѿ�ע����
	//	return FALSE;
	//}

	//(*pList)[MsgCode].push_back(pReceiver);
	return TRUE;
}
//------------------------------------------------------------------------
//�Ƴ�ģ�������
BOOL CRunClientLinkSink::RemoveReceiver(IDataReceiver *pReceiver, int MainMsgCode, int SubMsgCode) 
{
	////��ȷ����CMD_ROOT_THING ����,���Ƕ���ͨ�����,��Ҫ����ı�ȷ����
	//MESSAGE_LIST  *pList  = NULL;
	//int           MsgCode = 0;
	//if (MainMsgCode == CMD_ROOT_THING)
	//{
	//	pList   = &m_ThingMsgList;
	//	MsgCode = SubMsgCode;
	//}
	//else
	//{
	//	pList = &m_MsgList;
	//	MsgCode = MainMsgCode;
	//}
	//

	//(*pList)[MsgCode].remove(pReceiver);
	return TRUE;
}
//------------------------------------------------------------------------
// ��  �����յ�һ�����ݰ�
// ��  ��������������ݰ����߳���IThreadRoute����ͬһ�̣߳�
// ����ֵ������һ������(ILinkWnd)�������ͨѶ����ILinkWnd���븺����һ���������ݰ�����
BOOL CRunClientLinkSink::OnRecv(DWORD dwSocketID, LPCSTR buf, int nLen)
{
	if(nLen < sizeof(WORD))
	{
		return FALSE;
	}
	
	stReceiverInfoNode node;
	node.pData = new char[nLen];
	memcpy(node.pData, buf, nLen);
	node.nLen = nLen;

	m_NodeList.AddTail(node);
	
	if (!m_bThreadRun)
	{
		while(m_NodeList.GetCount() != 0)
			Receiver();
		return TRUE;
	}

	SetEvent(m_hEventWakeDecode);

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CRunClientLinkSink::Receiver()
{
	/*
	int nCount = m_NodeList.GetCount();
	if(nCount == 0)
	{
		return FALSE;
	}

	stReceiverInfoNode node = m_NodeList.RemoveHead();
	LPCSTR buf = node.pData;
	
	WORD wMsgRoot = *((WORD *)(buf));
	int hsize = sizeof(WORD);         //��Ϣ��ĳ���

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
			g_pGlobal->m_pWndMgr->CustomMsgBox(pErrDesc);
		}
		break;
	case CMD_ROOT_THING:
		{
			hsize *= 2;   //THING��������Ϣ��,��������Ҫ��2
			WORD wMsgSub = *((WORD *)(buf) + 1);
			if (wMsgSub >= MAX_ROOTTHING_MAIN_KEY)
			{
				break;
			}
			
			for (RECEIVER_LIST::iterator itor = m_ThingMsgList[wMsgSub].begin();
			itor != m_ThingMsgList[wMsgSub].end(); ++itor)
			{
				(*itor)->OnReceived(buf + hsize, node.nLen - hsize);
			}
		}
		 
		break;
	default:
		{
			if (wMsgRoot >= CMD_ROOT_MAX)
			{
				break;
			}
			for (RECEIVER_LIST::iterator itor = m_MsgList[wMsgRoot].begin();
			itor != m_MsgList[wMsgRoot].end(); ++itor)
			{
				(*itor)->OnReceived(buf + hsize, node.nLen - hsize);
			} 
		}
		break;
	}

	delete[] node.pData;
	*/
	return TRUE;
}
//------------------------------------------------------------------------
void CRunClientLinkSink::OnEvent(DWORD dwEventID)
{
	m_Lock.Lock();

	if(Receiver())
		SetEvent(m_hEventWakeDecode);

	m_Lock.Unlock();

	return;
}
//------------------------------------------------------------------------
void CRunClientLinkSink::LockReceiver(BOOL bLock)
{
	if(bLock)
	{
		m_Lock.Lock();		
		if (!m_bThreadRun)
		{
			while(m_NodeList.GetCount() != 0)
				Receiver();
		}
	}
	else
		m_Lock.Unlock();
}
//------------------------------------------------------------------------
void CRunClientLinkSink::SetThreadRun(BOOL bRun)
{
	m_bThreadRun = bRun;
}
//------------------------------------------------------------------------
int CRunClientLinkSink::GetType(void)
{
	return NETWORKLINKTYPE_RUNNING;
}
//------------------------------------------------------------------------