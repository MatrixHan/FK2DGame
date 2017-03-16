/**
*	created:		2012-6-30   4:54
*	filename: 		ChatClient
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ������ģ��
*/
//------------------------------------------------------------------------
#include "../IPaoPao.h"
#include "../Include/ChatClient.h"
#include "../Include/PaoPao.h"
#include "../Include/TitleClient.h"
#include "../../FKTrace/IFKTrace.h"
#include "../../FKGame/GameGlobal/IClientGlobal.h"
#include "../../FKGame/Team/ITeamClient.h"
#include "../../FKGame/Friend/IFriendClient.h"
#include "../../FKGame/SceneObj/ThingDef.h"
#include "../../FKGame/ClientWorld/IThingTable.h"
#include "../../FKGame/ClientWorld/IClientWorld.h"
#include "../../FKGame/SceneObj/IThingFactory.h"
#include "../../FKGame/WndSys/WndDef.h"
#include "../../MsgCommon/MsgKeyRoot.h"
#include "../../MsgCommon/MsgKeySubAction.h"
#include "../../MsgCommon/MsgKeySystem.h"
//------------------------------------------------------------------------
CChatClient * CChatClient::s_pModule = NULL;
CFKTrace				g_DefaultTrace;
CWndSysHelper			g_wndHelper;
IClientGlobal *			g_pGlobal		= NULL;
//------------------------------------------------------------------------
CChatClient::CChatClient(void)
: m_pGlobal(NULL)
, m_pOutputWindow(NULL)
, m_bCreated(FALSE)
, m_pInfoWindow(NULL)
, m_pResScript(NULL)
, m_pBulletinWnd(NULL)
, m_pHelpInfoWnd(NULL)
{
	m_dwPingStartTime = GetTickCount();
}
//------------------------------------------------------------------------
CChatClient::~CChatClient(void)
{
	// �˶�������Ϣ
	IController * pController = m_pGlobal->GetController();
	if (pController != NULL)
	{	
		pController->RemoveReceiver(static_cast<IDataReceiver *>(this), 
		                            CMD_ROOT_CHAT, 0);
	}
	
	IEventServer * pEventServer = m_pGlobal->m_pEventServer;
	if (pEventServer != NULL)
	{
		// �˶�ϵͳ��ʾ��Ϣ
		WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_SYSTEMMESSAGE);
		pEventServer->RemoveListener(static_cast<IEventActionListener *>(&m_sysMsgReceiver),
			wMsgID, SRC_TYPE_ID_ACTOR);

		// �˶�������Ϸ��Ϣ
		pEventServer->RemoveListener(static_cast<IEventActionListener *>(this), 
			MSG_SYSTEM_CLIENT_PLAY, SRC_TYPE_ID_SYSTEM);
		
		// �˶�С����Ϣ
		pEventServer->RemoveListener(static_cast<IEventActionListener *>(this), 
			MSG_SYSTEM_CLIENT_BACKSELEHERO, SRC_TYPE_ID_SYSTEM);
	}	
	
	if (m_pOutputWindow != NULL)
	{
		delete m_pOutputWindow;
		m_pOutputWindow = NULL;
	}
	if (m_pInfoWindow != NULL)
	{
		delete m_pInfoWindow;
		m_pInfoWindow = NULL;
	}
	if (m_pBulletinWnd != NULL)
	{
		delete m_pBulletinWnd;
		m_pBulletinWnd = NULL;
	}
	if (m_pHelpInfoWnd != NULL)
	{
		delete m_pHelpInfoWnd;
		m_pHelpInfoWnd = NULL;
	}
}

//-------------------------------------------------------------------
// ��  ������ȡģ���Ψһʵ�� 
// ��  ����void
// ����ֵ��CChatClient * - ģ��ʵ��
//-------------------------------------------------------------------
CChatClient * CChatClient::Instance(void)
{
	if (s_pModule == NULL)
	{
		s_pModule = new CChatClient();
	}

	return s_pModule;
}

//-------------------------------------------------------------------
// ��  ������������ģ�飬������ص���Դ 
// ��  ����[����] IClientGlobal *pGlobal - ȫ�ֹ�����
// ����ֵ��BOOL - �����ɹ�����TRUE�����򷵻�FALSE
//-------------------------------------------------------------------
BOOL CChatClient::Create(IClientGlobal * pGlobal)
{
	if (m_bCreated)
	{
		return TRUE;
	}

	if (pGlobal == NULL)
	{
		return FALSE;
	}
	m_pGlobal = pGlobal;
	g_pGlobal = pGlobal;
	// ����ű���Դ		
#ifdef _DEBUG
	m_pResScript = m_pGlobal->m_ResMgrHelper.m_pResMgr->LoadResScript("Data\\Scp\\Chat.scp");
#else
	BYTE pKey[] = "abcd";
	m_pResScript = m_pGlobal->m_ResMgrHelper.m_pResMgr->LoadResScript("Data\\Scp\\Chat.res", FALSE, pKey);
#endif
	
	if (m_pResScript == NULL)
	{
		PrintEventLn("���� ChatClient ʧ�ܣ�ȱ����Դ�����ļ���");
		return FALSE;
	}
	TraceLn("ChatClient - Chat.res LOADED");

	// ������Ϣ�������
	m_pOutputWindow = new COutputWindow();
	if (m_pOutputWindow == NULL 
		|| !m_pOutputWindow->Create(m_pResScript->GetResObject(OUTPUTWINDOW_ID)))
	{
		PrintEventLn("ChatClient (FAIL) - m_pOutputWindow->Create");
		return FALSE;
	}	
	
	// ����װ��������ʾ����
	m_pInfoWindow = new CInfoWindow();
	if (m_pInfoWindow == NULL 
		|| !m_pInfoWindow->Create(m_pResScript->GetResObject(INFOWINDOW_ID)))
	{
		PrintEventLn("ChatClient (FAIL) - m_pInfoWindow->Create");
		return FALSE;
	}

	// ��ʼ��ϵͳ��Ϣ������
	if (!m_sysMsgReceiver.Init(m_pResScript->GetResObject(PROMPTWINDOW_ID)))
	{
		PrintEventLn("ChatClient (FAIL) - m_sysMsgReceiver.Init");
		return FALSE;
	}

	// ����ϵͳ���洰��
	m_pBulletinWnd = new CBulletinWnd();
	if (m_pBulletinWnd == NULL
		|| !m_pBulletinWnd->Create(m_pResScript->GetResObject(BULLETINWINDOW_ID)))
	{
		PrintEventLn("ChatClient (FAIL) - m_pBulletinWnd->Create");
		return FALSE;
	}
	
	// ����������ʾ��
	m_pHelpInfoWnd = new CPromptWnd();
	if (m_pHelpInfoWnd == NULL
		|| !m_pHelpInfoWnd->Create(m_pResScript->GetResObject(HELPINFOWINDOW_ID)))
	{
		PrintEventLn("ChatClient (FAIL) - m_pHelpInfoWnd->Create");
		return FALSE;
	}
	
	// �����ƺ�ϵͳ
	if( !CTitleClient::Instance()->Create(pGlobal,m_pResScript))
	{
		PrintEventLn("ChatClient (FAIL) - Create TitleClient Error");
		return FALSE;
	}
    

	// ����������Ϣ
	IController * pController = m_pGlobal->GetController();
	if (pController == NULL
		|| !pController->AddReceiver(static_cast<IDataReceiver *>(this), CMD_ROOT_CHAT, 0))
	{
		return FALSE;
	}	
		
	IEventServer * pEventServer = m_pGlobal->m_pEventServer;
	if (pEventServer == NULL)
	{
		return FALSE;
	}

	//  ��������˵�
	m_pMenu = new CChatMenu();
	if (m_pMenu == NULL || !m_pMenu->Create(m_pResScript->GetResObject(CHAT_MENU_WND_ID)))
	{
		PrintEventLn("ChatClient (FAIL) - Create ChatMenu Error");
		return FALSE;
	}
	SetMenu(g_menu);
	m_pMenu->SetEventReceiver(static_cast<IMenuEventReceiver *>(this));

	// ����ϵͳ��ʾ��Ϣ
	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_SYSTEMMESSAGE);
	if (!pEventServer->AddListener(static_cast<IEventActionListener *>(&m_sysMsgReceiver),
		wMsgID, SRC_TYPE_ID_ACTOR, 0, "CChatClient::Create"))
	{
		return FALSE;
	}	
	
	// ���Ľ�����Ϸ��Ϣ
	if (!pEventServer->AddListener(static_cast<IEventActionListener *>(this), 
		MSG_SYSTEM_CLIENT_PLAY, SRC_TYPE_ID_SYSTEM, 0, "CChatClient::Create"))
	{
		return FALSE;
	}

	// ����С����Ϣ
	if (!pEventServer->AddListener(static_cast<IEventActionListener *>(this), 
		MSG_SYSTEM_CLIENT_BACKSELEHERO, SRC_TYPE_ID_SYSTEM, 0, "CChatClient::Create"))
	{
		return FALSE;
	}

	TraceLn("ChatClient - CChatClient::Create OK");
	m_bCreated = TRUE;
	return TRUE;
}

//-------------------------------------------------------------------
// ��  �����û����� 
// ��  ����[����] int nChannel - ����Ƶ��
// ��  ����[����] const char * pszReceiver - ��Ϣ�Ľ�����
// ��  ����[����] const char * pszText - ���Ե��ı�
// ����ֵ��BOOL - ��Ҫ˵�Ļ������Ч�ҿ��Է��ͷ���TRUE�����򷵻�FALSE
//-------------------------------------------------------------------
BOOL CChatClient::Speak(int nChannel, const char * pszReceiver, const char * pszText)
{
	return m_controller.Speak(nChannel, pszReceiver, pszText);
}

//-------------------------------------------------------------------
// ��  ���� ��������Ƶ���Ŀ���
// ��  ����[����] int nChannel - ����Ƶ��
// ��  ����[����] BOOL bOnOff - �Ƿ��
// ����ֵ��void
//-------------------------------------------------------------------
void CChatClient::ChannelOn(int nChannel, BOOL bOnOff)
{
	m_pOutputWindow->ChannelOn(nChannel, bOnOff);
}

//-------------------------------------------------------------------
// ��  �����趨�Ƿ���ʾ������Ϣ���� 
// ��  ����[����] int nType - ��ʾ��ʽ
// ����ֵ��void
//-------------------------------------------------------------------
void CChatClient::ShowWindow(int nType)
{	
	if (nType == CW_ALTERNATE)
	{	
		// ������ʾ��Ϣ�������
		if (m_pOutputWindow->IsVisible())
		{
			m_pOutputWindow->ShowWindow(FALSE);
		}
		else
		{
			m_pOutputWindow->ShowWindow(TRUE);
		}
	}
	else if (nType == CW_HIDE)
	{
		m_pOutputWindow->ShowWindow(FALSE);
	}
	else if (nType == CW_SHOW)
	{
		m_pOutputWindow->ShowWindow(TRUE);
	}
}

///-------------------------------------------------------------------
// ��  ����������Ϣ��ʾ���ڵ�͸���� 
// ��  ����[����] int nLevel - ͸����
// ����ֵ��void
//-------------------------------------------------------------------
void CChatClient::SetWindowTrans(int nLevel)
{
	m_pOutputWindow->SetTransparence(nLevel);
}

//-------------------------------------------------------------------
// ��  ������ʾ���յ���������Ϣ 
// ��  ����[����] const char * pData - ���յ�������
// ��  ����[����] int nLen - ���ݵĳ���
// ����ֵ��BOOL - ���������Ϣ������ȷ��ʾ�򷵻�TRUE�����򷵻�FALSE
//-------------------------------------------------------------------
BOOL CChatClient::OnReceived(const char * pData, int nLen)
{
	return m_pOutputWindow->Display(pData, nLen);
}

//-------------------------------------------------------------------
// ��  �����ͷ�����ģ��ʹ�õ���Դ
// ��  ����void
// ����ֵ��void
//-------------------------------------------------------------------
void CChatClient::Release(void)
{
	delete this;
}

//-------------------------------------------------------------------
// ��  �������յ�С����Ϣ 
//-------------------------------------------------------------------
void CChatClient::OnAction(IEvent *pEvent)
{
	switch (pEvent->GetID())
	{
	case MSG_SYSTEM_CLIENT_PLAY:
		m_sysMsgReceiver.Reset();
		m_sysMsgReceiver.SetPromptOn();
		m_pHelpInfoWnd->ClearMessage();
		break;
		
	case MSG_SYSTEM_CLIENT_BACKSELEHERO:
		m_sysMsgReceiver.Reset();
		m_pHelpInfoWnd->ClearMessage();
		m_pOutputWindow->Clear();
		m_InvlidThingMgr.ReleaseAll();
		break;

	default:
		break;
	}
}

//-------------------------------------------------------------------
// ��  ������ʾϵͳ����ʾ��Ϣ
// ��  ����const char * pszText - ��ʾ�ı�
// ����ֵ��void
//-------------------------------------------------------------------
void CChatClient::DisplaySysMessage(const char * pszText)
{
	char buf[1024];
	int nLen = strlen(pszText);

	// ��ʾ�ı��ĳ��Ȳ��ܳ����������Ĵ�С
	if (sizeof(SSystemChatMsg) + nLen > sizeof(buf))
	{
		return;
	}

	SSystemChatMsg * pMsg = (SSystemChatMsg *)buf;
	pMsg->header.nChannel = CHANNEL_SYSTEM;
	pMsg->nCustomCmd = CHAT_CMD_NONE;
	pMsg->wTextLen = nLen + 1;
	strncpy(pMsg->szText, pszText, nLen + 1);
	
	m_pOutputWindow->Display(buf, sizeof(SSystemChatMsg) + nLen);
}

//-------------------------------------------------------------------
// ��  ��������һ������ 
//-------------------------------------------------------------------
IPaoPao * CChatClient::CreatePaoPao(UID uid)
{
	return new CPaoPao(uid);
}

//-------------------------------------------------------------------
// ��  ������ȡʱ���� 
//-------------------------------------------------------------------
ITimeAxis * CChatClient::GetTimeAxis(void)
{
	return m_pGlobal->m_TimerAxisHelper.m_pTimeAxis;
}

//-------------------------------------------------------------------
// ��  ������ʾ������ʾ��Ϣ
//-------------------------------------------------------------------
void CChatClient::DisplayFloatMessage(int nType, const char * lpszText)
{
	m_sysMsgReceiver.ShowMessage(nType, lpszText);
}

//-------------------------------------------------------------------
// ��  ������ʾ����ͼ 
//-------------------------------------------------------------------
void CChatClient::OnSysMsgDraw(ICanvas * pCanvas)
{
	m_sysMsgReceiver.OnSysMsgDraw(pCanvas);
}

//-------------------------------------------------------------------
// ��  ����ͨ��UID��ȡ���ָ��
//-------------------------------------------------------------------
IActor * CChatClient::GetPlayer(UID uidPlayer)
{
	IThingTable * pThingTable = m_pGlobal->m_pClientWorld->GetThingTable();
	if (pThingTable == NULL)
	{
		return NULL;
	}
	
	IThing * pThing = pThingTable->Find(uidPlayer);
	if (pThing == NULL)
	{
		return NULL;		
	}
	
	if (pThing->GetThingClass() == NULL 
		|| pThing->GetThingClass()->GetThingClass() != THINGCLASS_ACTOR)
	{
		return NULL;		
	}
	
	return static_cast<IActor *>(pThing);
}

//-------------------------------------------------------------------
// ��  ����ͨ��UID��ȡ����ָ��
//-------------------------------------------------------------------
ICreature * CChatClient::GetCreature(UID uidCreature)
{
	IThingTable * pThingTable = m_pGlobal->m_pClientWorld->GetThingTable();
	if (pThingTable == NULL)
	{
		return NULL;
	}
	
	IThing * pThing = pThingTable->Find(uidCreature);
	if (pThing == NULL)
	{
		return NULL;		
	}
	return static_cast<ICreature *>(pThing);
}

//-------------------------------------------------------------------
// ��  ������ʼping
//-------------------------------------------------------------------
void CChatClient::PingStart(void)
{
	m_dwPingStartTime = GetTickCount();
}

//-------------------------------------------------------------------
// ��  ������ȡping�Ŀ�ʼʱ��
//-------------------------------------------------------------------
DWORD CChatClient::PingStop(void)
{
	return (GetTickCount() - m_dwPingStartTime) / 2;
}

void CChatClient::ClearFloatMessage(int nType)
{
    m_sysMsgReceiver.Clear(-1);
}


void CChatClient::ShowTitleWnd(BOOL bTrue,BOOL bTurn)
{
	g_pGlobal->m_pWndMgr->OnEvent(TITLE_WND,405,0,0);
}

void CChatClient::Clicked(int nId)
{
    switch(nId)
	{
	case CHAT_PRIVATE:
		{
      
			m_controller.SetChatPlayer(m_strMenu);
			break;
		}
	case CHAT_TEAM:
		{
            ITeamClient *pTeamClient = g_pGlobal->GetTeamClient();
			if( pTeamClient!= NULL)
			{
                pTeamClient->InviteJionTeam(m_strMenu.c_str());
			}
			break;
		}
	case CHAT_FRIEND:
		{
			IFriendClient *pFriendClient = g_pGlobal->GetFriendClient();
			if( pFriendClient!= NULL)
			{
                pFriendClient->AddFriend(m_strMenu.c_str());
			}
		    break;	
		}
	case CHAT_INFO:
		{
			//ί�и�����ģ�����
			g_pGlobal->m_pWndMgr->OnEvent(FRIEND_WND,403,(WPARAM)(m_strMenu.c_str()),0);

		    break;	
		}
		
	}
}

//-------------------------------------------------------------------
// ��ȡ���֮��ľ���
//-------------------------------------------------------------------	
int CChatClient::GetDistance(UID uid)
{
    return m_controller.GetDistance(uid);
}
//------------------------------------------------------------------------
void CChatClient::SetMenu(const SMenuItem menu[])
{
	m_pMenu->RemoveAll();
	
	for (int i = 0; menu[i].nId != 0; i++)
	{
		m_pMenu->AddItem(menu[i].nId, menu[i].pszName);
	}
}
//------------------------------------------------------------------------
void CChatClient::ShowMenu(POINT pt,LPCSTR lpszPlayer)
{
   m_pMenu->Show(&pt,TRUE);
   m_strMenu = lpszPlayer;
}
//------------------------------------------------------------------------
IThing *CChatClient::GetThing(UID uid)
{
	IThingTable * pThingTable = m_pGlobal->m_pClientWorld->GetThingTable();
	if (pThingTable == NULL)
	{
		return NULL;
	}
	
	IThing * pThing = pThingTable->Find(uid);
	return pThing;
}
//------------------------------------------------------------------------
IThing *CChatClient::CreateGoods(char *buf,int nLen,UID uid)
{
	IThing *pThing = g_pGlobal->m_pThingFactory->CreatePublicThing(THINGCLASS_GOODS,uid,buf,nLen);
	m_InvlidThingMgr.AddThing(pThing);
	return pThing;
}
//------------------------------------------------------------------------