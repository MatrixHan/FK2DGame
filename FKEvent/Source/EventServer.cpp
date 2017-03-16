/**
*	created:		2012-6-30   21:19
*	filename: 		EventServer
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/EventServer.h"
#include "../../FKResManager/IFKResManager.h"
//------------------------------------------------------------------------
CFKTrace		g_DefaultTrace;
//------------------------------------------------------------------------
CEventServer::CEventServer()
{	
	memset(m_bTypeIDStructure, 0, MAX_TYPE_ID);
	memset(m_bTypeIDCheck, 0, MAX_TYPE_ID);
}
//------------------------------------------------------------------------
CEventServer::~CEventServer()
{
	memset(m_bTypeIDStructure, 0, MAX_TYPE_ID);
	memset(m_bTypeIDCheck, 0, MAX_TYPE_ID);
}
//------------------------------------------------------------------------
BOOL CEventServer::FireVote(IEvent *pEvent)
{
	return m_VoteServer.Fire(pEvent);	
}
//------------------------------------------------------------------------
BOOL CEventServer::FireVote(BYTE *pContext, int nLen,
							WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer)
{
	CMiniEvent event;
	event.SetParams(pContext, nLen, wEventID, bSrcTypeID, dwSrcPointer);
	return FireVote(&event);
}
//------------------------------------------------------------------------
void CEventServer::FireAction(IEvent *pEvent)
{
	m_ActionServer.Fire(pEvent);
}
//------------------------------------------------------------------------
void CEventServer::FireAction(BYTE *pContext, int nLen,
							  WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer)
{
	CMiniEvent event;
	event.SetParams(pContext, nLen, wEventID, bSrcTypeID, dwSrcPointer);
	FireAction(&event);
}
//------------------------------------------------------------------------
void CEventServer::FireResponse(IEvent *pEvent)
{
	m_ResponseServer.Fire(pEvent);
}
//------------------------------------------------------------------------
void CEventServer::FireResponse(BYTE *pContext, int nLen,
								WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer)
{
	CMiniEvent event;
	event.SetParams(pContext, nLen, wEventID, bSrcTypeID, dwSrcPointer);
	FireResponse(&event);
}
//------------------------------------------------------------------------
BOOL CEventServer::AddListener(IEventVoteListener *pListener,
							   WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer,
							   const char *pDescription)
{
	return m_VoteServer.AddListener(pListener, wEventID, bSrcTypeID,
		dwSrcPointer, pDescription);
}
//------------------------------------------------------------------------
BOOL CEventServer::AddListener(IEventActionListener *pListener,
							   WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer,
							   const char *pDescription)
{
	return m_ActionServer.AddListener(pListener, wEventID, bSrcTypeID,
		dwSrcPointer, pDescription);
}
//------------------------------------------------------------------------
BOOL CEventServer::AddListener(IEventResponseListener *pListener,
							   WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer,
							   const char *pDescription)
{
	return m_ResponseServer.AddListener(pListener, wEventID, bSrcTypeID, 
		dwSrcPointer, pDescription);
}
//------------------------------------------------------------------------
BOOL CEventServer::RemoveListener(IEventVoteListener *pListener,
								  WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer)
{
	return m_VoteServer.RemoveListener(pListener, wEventID, bSrcTypeID, 
		dwSrcPointer);
}
//------------------------------------------------------------------------
BOOL CEventServer::RemoveListener(IEventActionListener *pListener,
								  WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer)
{
	return m_ActionServer.RemoveListener(pListener, wEventID, bSrcTypeID,
		dwSrcPointer);
}
//------------------------------------------------------------------------
BOOL CEventServer::RemoveListener(IEventResponseListener *pListener,
								  WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer)
{
	return m_ResponseServer.RemoveListener(pListener, wEventID, bSrcTypeID,
		dwSrcPointer);
}
//------------------------------------------------------------------------
void CEventServer::Release()
{
	delete this;
}
//------------------------------------------------------------------------
const char SCP_CONFIG_FILE[]="scp\\Event.scp";
const char RES_CONFIG_FILE[]="scp\\Event.res";
//------------------------------------------------------------------------
BOOL CEventServer::Create(IResMgr *pResMgr, const char *pFileName, BOOL bEncrypt)
{
	if (pResMgr == NULL)
	{
		TraceLn("pResMgr == NULL");
		return FALSE;
	}

	if (pFileName == NULL)
	{
		if (bEncrypt) 
		{
			//����
			pFileName = RES_CONFIG_FILE;
			bEncrypt = TRUE;
		}		
		else
		{
			pFileName = SCP_CONFIG_FILE;
			bEncrypt = FALSE;
		}
	}	

	IResScript *pResScript = NULL;

	if (bEncrypt)
	{
		BYTE pKey[] = "abcd";	
		pResScript = pResMgr->LoadResScript(pFileName, FALSE, pKey);
	}
	else
	{
		pResScript=pResMgr->LoadResScript(pFileName);
	}

	if (pResScript != NULL)
	{
		IResObject *pRootObject = pResScript->GetResObjectRoot();

		BOOL bResult = TRUE;
		if (pRootObject != NULL)
		{			
			m_bTypeIDStructure[SRC_TYPE_ID_ROOT] = 0;//���ø��ڵ�
			m_bTypeIDCheck[SRC_TYPE_ID_ROOT]=1;//�������ڵ��Ѿ���ʹ��

			int nSubObjectCount = pRootObject->GetSubObjectCount();
			for(int i=0; i<nSubObjectCount; i++)
			{				
				bResult &= Parse(pRootObject->GetSubObjectByIndex(i), 0);
				if (!bResult) 
				{
					//��������
					TraceLn("�����ű�����");
					break;
				}
			}

			if (bResult) 
			{
				//�����ű��ɹ�
				m_VoteServer.SetTreeNode(m_bTypeIDStructure, MAX_TYPE_ID);
				m_ActionServer.SetTreeNode(m_bTypeIDStructure, MAX_TYPE_ID);
				m_ResponseServer.SetTreeNode(m_bTypeIDStructure, MAX_TYPE_ID);
			}
		}
		else
		{
			TraceLn("pRootObject == NULL");
			bResult = FALSE;
		}

		bResult &= pResScript->Release();

		return bResult;
	}
	return FALSE;
}
//------------------------------------------------------------------------
BOOL CEventServer::Parse(IResObject *pResObject, int nParentTypeID)
{
	if (pResObject != NULL)
	{
		long nTypeID = 0;
		BOOL bResult = pResObject->GetPropertyLong("TypeID", nTypeID);
		if (bResult)
		{
			//��ȡ���Գɹ�
			if (nTypeID>0 && nTypeID<MAX_TYPE_ID)
			{
				if(m_bTypeIDCheck[nTypeID])
				{
					//�Ѿ���ʹ��,��ʾID�ظ�
					TraceLn("����ID�ظ�");
					return FALSE;
				}
				m_bTypeIDStructure[nTypeID] = nParentTypeID;
				m_bTypeIDCheck[nTypeID] = 1;

				//ѭ���ӽڵ�
				int nSubObjectCount = pResObject->GetSubObjectCount();
				for(int i=0; i<nSubObjectCount; i++)
				{
					bResult &= Parse(pResObject->GetSubObjectByIndex(i), nTypeID);
					if (!bResult) 
					{
						//��������
						break;
					}
				}
				return bResult;
			}
			else
			{
				//����ֵTypeID���÷�Χ����
				TraceLn("����ֵTypeID���÷�Χ����,����>0 && %d",MAX_TYPE_ID);
				return FALSE;
			}
		}
		else
		{
			TraceLn("�ű��ڵ㲻������TypeID����");
			return FALSE;
		}
	}

	TraceLn("�ű��ڵ�ΪNULL");
	return FALSE;
}
//------------------------------------------------------------------------
extern "C" __declspec(dllexport) 
BOOL CreateEventServer(IEventServer **ppEventServer, 
					   IResMgr *pResMgr,ITrace *pTrace, const char *pFileName, BOOL bEncrypt)
{
	if(!g_DefaultTrace.Create(pTrace))
	{
		return FALSE;
	}

	CEventServer *pEventServer = new CEventServer();
	if (pEventServer == NULL)
	{
		TraceLn("new CEventServer ʧ��");
		return FALSE;
	}

	if (!pEventServer->Create(pResMgr, pFileName, bEncrypt))
	{
		TraceLn("pEventServer->Create(pResMgr, pFileName, bEncrypt) fail.");
		return FALSE;
	}
	*ppEventServer = static_cast<IEventServer*>(pEventServer);

	return TRUE;
}
//------------------------------------------------------------------------