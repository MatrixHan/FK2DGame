/**
*	created:		2012-6-28   9:19
*	filename: 		TitleController
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/TitleClient.h"
#include "../Include/TitleController.h"
#include "../Include/ChatClient.h"
#include "../../FKGame/DataArchive/IDataArchive.h"
#include "../../MsgCommon/MsgKeySubAction.h"
#include "../../MsgCommon/MsgKeySystem.h"
#include "../../FKGame/WndSys/WndDef.h"
//------------------------------------------------------------------------
CTitleController::CTitleController()
{

}
//------------------------------------------------------------------------
CTitleController::~CTitleController()
{

}
//------------------------------------------------------------------------
// ��ȡ�ƺ�
void CTitleController::GetTitle()
{
	//char buf[256];
	//int nLen = sizeof(buf);
	//CDataArchive archive(buf, nLen);

	//CS_ACTION_HEAD actionhead;	
	//actionhead.m_wKeyRoot = CMD_ROOT_THING;
	//actionhead.m_wKeyMain = ROOTTHING_CS_MAIN_ACTION;
	//actionhead.m_wKeySub  = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_TITLE);

	//// ͷ
	//archive.Write(&actionhead, sizeof(CS_ACTION_HEAD));

	//// ����
	//int nCmd = TITLE_CMD_GET;
	//archive.Write(&nCmd, sizeof(int));

	//// ����
	//g_pGlobal->m_pAppNetworkLink->Send(buf, archive.GetBufferOffset());    
}
//------------------------------------------------------------------------
// ʹ�óƺ�
void CTitleController::UserTitle(long lTitleId)
{
	//char buf[256];
	//int nLen = sizeof(buf);
	//CDataArchive archive(buf, nLen);

	//CS_ACTION_HEAD actionhead;	
	//actionhead.m_wKeyRoot = CMD_ROOT_THING;
	//actionhead.m_wKeyMain = ROOTTHING_CS_MAIN_ACTION;
	//actionhead.m_wKeySub  = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_TITLE);

	//// ͷ
	//archive.Write(&actionhead, sizeof(CS_ACTION_HEAD));

	//// ����
	//int nCmd = TITLE_CMD_USE;
	//archive.Write(&nCmd, sizeof(int));

	//// �ƺ�ID
	//archive.Write(&lTitleId, sizeof(long));

	//// ����
	//g_pGlobal->m_pAppNetworkLink->Send(buf, archive.GetBufferOffset()); 	
}
//------------------------------------------------------------------------
// ��ʹ�óƺ�
void CTitleController::UnUseTitle(long lTitleId)
{
	//char buf[256];
	//int nLen = sizeof(buf);
	//CDataArchive archive(buf, nLen);

	//CS_ACTION_HEAD actionhead;	
	//actionhead.m_wKeyRoot = CMD_ROOT_THING;
	//actionhead.m_wKeyMain = ROOTTHING_CS_MAIN_ACTION;
	//actionhead.m_wKeySub  = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_TITLE);

	//// ͷ
	//archive.Write(&actionhead, sizeof(CS_ACTION_HEAD));

	//// ����
	//int nCmd = TITLE_CMD_UNUSE;
	//archive.Write(&nCmd, sizeof(int));

	//// �ƺ�ID
	//archive.Write(&lTitleId, sizeof(long));

	//// ����
	//g_pGlobal->m_pAppNetworkLink->Send(buf, archive.GetBufferOffset()); 
}
//------------------------------------------------------------------------
// ɾ���ƺ�
void CTitleController::DeleteTitle(long lTitleId)
{
	//char buf[256];
	//int nLen = sizeof(buf);
	//CDataArchive archive(buf, nLen);

	//CS_ACTION_HEAD actionhead;	
	//actionhead.m_wKeyRoot = CMD_ROOT_THING;
	//actionhead.m_wKeyMain = ROOTTHING_CS_MAIN_ACTION;
	//actionhead.m_wKeySub  = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_TITLE);

	//// ͷ
	//archive.Write(&actionhead, sizeof(CS_ACTION_HEAD));

	//// ����
	//int nCmd = TITLE_CMD_DELETE;
	//archive.Write(&nCmd, sizeof(int));

	//// �ƺ�ID
	//archive.Write(&lTitleId, sizeof(long));

	//// ����
	//g_pGlobal->m_pAppNetworkLink->Send(buf, archive.GetBufferOffset()); 	
}
//------------------------------------------------------------------------
// ��ȡ�����˵ĳƺ�
void CTitleController::GetOtherTitle(UID uid)
{
	//char buf[256];
	//int nLen = sizeof(buf);
	//CDataArchive archive(buf, nLen);

	//CS_ACTION_HEAD actionhead;	
	//actionhead.m_wKeyRoot = CMD_ROOT_THING;
	//actionhead.m_wKeyMain = ROOTTHING_CS_MAIN_ACTION;
	//actionhead.m_wKeySub  = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_TITLE);

	//// ͷ
	//archive.Write(&actionhead, sizeof(CS_ACTION_HEAD));

	//// ����
	//int nCmd = TITLE_CMD_GETOTHER;
	//archive.Write(&nCmd, sizeof(int));

	//// UID
	//archive.Write(&uid, sizeof(UID));

	//// ����
	//g_pGlobal->m_pAppNetworkLink->Send(buf, archive.GetBufferOffset());
}
//------------------------------------------------------------------------
void CTitleController::OnAction(IEvent *pEvent)
{
	typedef void (CTitleController::*TITLE_PROC)(LPCSTR, int);
	static TITLE_PROC __proc__[TITLE_CMD_MAX] = {
		&CTitleController::GetTitleResponse,
		&CTitleController::UseTitleResponse,
		&CTitleController::UnUseTitleResponse,
		&CTitleController::DeleteTitleResponse,
		&CTitleController::GetOtherTitleResponse,
		&CTitleController::TitleStatusChange,
		&CTitleController::AddTitleResponse
	};		

	// ���ж���ʲô��Ϣ
	WORD  wMsgID = pEvent->GetID();
	if( wMsgID == MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_BORN))
	{
		// ��������������Ϣ��ȥ��ȡ����˵ĳƺ�
		IActor *pActor = (IActor *)pEvent->GetSrcPointer();
		if(pActor != NULL)
		{
			GetOtherTitle(pActor->GetUID());
		}
		return;
	}else if(wMsgID == MSG_SYSTEM_CLIENT_BACKSELEHERO)
	{
		// С��:
		g_pGlobal->m_pWndMgr->OnEvent(TITLE_WND,404,0,0);
		return;
	}
	// ����ǳƺ���Ϣ
	int nLen;
	char* pBuf = (char*)pEvent->GetContext(nLen);
	if (pBuf == NULL
		|| nLen < sizeof(int))
	{
		return;
	}	

	// �õ���������
	int nCmd = *(int*)pBuf;	
	pBuf += sizeof(int);
	nLen -= sizeof(int);
	if (nCmd < 0 || nCmd >= TITLE_CMD_MAX)
	{
		return;
	}
	// ����
	if (__proc__[nCmd] != NULL)
	{
		(this->*__proc__[nCmd])(pBuf, nLen);
	}
}
//------------------------------------------------------------------------
BOOL CTitleController::OnReceived(const char* pData, int nLen)
{
	g_pGlobal->m_pWndMgr->OnEvent(TITLE_WND,404,0,0);
	return TRUE;   
}
//------------------------------------------------------------------------
// ��ȡ�ƺŷ���
void CTitleController::GetTitleResponse(LPCSTR pBuf,int nLen)
{
	if (nLen < sizeof(int))
	{
		return;
	}
	// �ƺŸ���
	int  nSize = *(int*)pBuf;
	pBuf += sizeof(int);
	nLen -= sizeof(int);

	for(int i=0;i<nSize;i++)
	{
		if( nLen < sizeof(SActorTitle2)) 
		{
			break;
		}
		g_pGlobal->m_pWndMgr->OnEvent(TITLE_WND,400,(WPARAM)pBuf,0);
		pBuf += sizeof(SActorTitle2);
		nLen -= sizeof(SActorTitle2);

	}
	// ��ʾ�ƺŴ���
	g_pGlobal->m_pWndMgr->OnEvent(TITLE_WND,406,0,0);
}
//------------------------------------------------------------------------
// ʹ�óƺŷ���
void CTitleController::UseTitleResponse(LPCSTR pBuf,int nLen)
{
	// �ƺ�ID;
	if (nLen < sizeof(int)+sizeof(int))
	{
		return;
	}	
	long  lTitleId = *(long*)pBuf;
	pBuf += sizeof(long);
	nLen -= sizeof(long);

	int nTrue = *(int*)pBuf; 
	pBuf += sizeof(int);
	nLen -= sizeof(int);

	if( nTrue )
	{
		g_pGlobal->m_pWndMgr->OnEvent(TITLE_WND,402,(WPARAM)lTitleId,0);
	}
}
//------------------------------------------------------------------------
// ��ʹ�óƺ�
void CTitleController::UnUseTitleResponse(LPCSTR pBuf,int nLen)
{
	// �ƺ�ID;
	if (nLen < sizeof(int)+sizeof(int))
	{
		return;
	}	
	long  lTitleId = *(long*)pBuf;
	pBuf += sizeof(long);
	nLen -= sizeof(long);

	int nTrue = *(int*)pBuf; 
	pBuf += sizeof(int);
	nLen -= sizeof(int);

	if( nTrue )
	{
		g_pGlobal->m_pWndMgr->OnEvent(TITLE_WND,403,(WPARAM)lTitleId,0);
	}
}
//------------------------------------------------------------------------
//ɾ���ƺ���Ӧ
void CTitleController::DeleteTitleResponse(LPCSTR pBuf,int nLen)
{
	// �ƺ�ID;
	if (nLen < sizeof(int)+sizeof(int))
	{
		return;
	}	
	long  lTitleId = *(long*)pBuf;
	pBuf += sizeof(long);
	nLen -= sizeof(long);

	int nTrue = *(int*)pBuf; 
	pBuf += sizeof(int);
	nLen -= sizeof(int);

	if( nTrue )
	{
		g_pGlobal->m_pWndMgr->OnEvent(TITLE_WND,401,(WPARAM)lTitleId,0);
	}
}
//------------------------------------------------------------------------
void CTitleController::GetOtherTitleResponse(LPCSTR pBuf,int nLen)
{
	//��Ϣ��ʽ���UID+�ƺ�����+�ƺ�����
	if( nLen < sizeof(UID)+sizeof(long))
	{
		return;
	}

	UID uid = *(UID *)pBuf;
	pBuf += sizeof(UID);
	nLen -= sizeof(UID);

	long lTitleId = *(long *)pBuf;
	pBuf += sizeof(long);
	nLen -= sizeof(long);
	if( lTitleId <0 )
	{
		CTitleClient::Instance()->SetPlayerTitle(uid,lTitleId);
	}else{
		if( nLen < 32)
		{
			return;
		}
		CTitleClient::Instance()->SetPlayerTitle(uid,lTitleId,pBuf);
	}
}
//------------------------------------------------------------------------
// ״̬�ı�֪ͨ
void CTitleController::TitleStatusChange(LPCSTR pBuf,int nLen)
{
	if(nLen<sizeof(long)+sizeof(int))
	{
		return;
	}

	long lTitleId = *(long *)pBuf;
	pBuf +=sizeof(long);
	nLen -=sizeof(long);

	int nStatus = *(int *)pBuf;
	pBuf +=sizeof(int);
	nLen -=sizeof(int);
	g_pGlobal->m_pWndMgr->OnEvent(TITLE_WND,407,(WPARAM)lTitleId,nStatus);   
}
//------------------------------------------------------------------------
// ����һ���ƺ�
void CTitleController::AddTitleResponse(LPCSTR pBuf,int nLen)
{
	if (nLen < sizeof(int))
	{
		return;
	}
	// �ƺŸ���
	int  nSize = *(int*)pBuf;
	pBuf += sizeof(int);
	nLen -= sizeof(int);

	for(int i=0;i<nSize;i++)
	{
		if( nLen < sizeof(SActorTitle2)) 
		{
			break;
		}
		g_pGlobal->m_pWndMgr->OnEvent(TITLE_WND,400,(WPARAM)pBuf,0);
		pBuf += sizeof(SActorTitle2);
		nLen -= sizeof(SActorTitle2);

	}
}
//------------------------------------------------------------------------