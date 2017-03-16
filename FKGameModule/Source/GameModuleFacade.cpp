/**
*	created:		2012-6-26   0:01
*	filename: 		GameModuleFacade
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/GameModuleFacade.h"
#include "../Include/GameModuleCommon.h"
#include "../../FKGame/GameGlobal/IClientGlobal.h"
#include "../../FKGame/ClientWorld/IClientWorld.h"
#include "../../FKGame/ClientWorld/IThingTable.h"
#include "../../FKGame/SceneObj/IThingClass.h"
//------------------------------------------------------------------------
CGameModuleFacade * g_pGameModuleFacade;
//------------------------------------------------------------------------
/////////////////////////////////////////////////////////
////
//// ��ȡ�ű�������
////
/////////////////////////////////////////////////////////
IScriptUpdateManager * CGameModuleFacade::GetScriptUpdateManager(void)
{	
	return NULL;
	//return g_pGlobal->m_pScriptUpdateManager;	
}

/////////////////////////////////////////////////////////
////
//// SCP�ű�������
////
/////////////////////////////////////////////////////////
IResMgr	* CGameModuleFacade::GetResMgr(void)
{
	return g_pGlobal->m_ResMgrHelper.m_pResMgr;
}

/////////////////////////////////////////////////////////
////
////
////
/////////////////////////////////////////////////////////
char * CGameModuleFacade::GetWorkDir(void)
{
	return g_pGlobal->m_szWorkDir;
}

/////////////////////////////////////////////////////////
////
//// �¼�������
////
/////////////////////////////////////////////////////////
IEventServer * CGameModuleFacade::GetEventServer(void)
{
	return NULL;
	//return g_pGlobal->m_pEventServer;
}

/////////////////////////////////////////////////////////
////
//// �ռ������
////
/////////////////////////////////////////////////////////
CAccountSpace<ACCOUNTSPACE_WIDTH, ACCOUNTSPACE_HIGH> * CGameModuleFacade::GetAccountSpace(void)
{
	return &m_pAccountSpace;
}

/////////////////////////////////////////////////////////
////
//// ���ö�ʱ��
////
/////////////////////////////////////////////////////////
BOOL CGameModuleFacade::SetTimer(DWORD dwEventID, ITimerSink * pTimerSink, DWORD dwElapse, char *szTimerDesc)
{
	return g_pGlobal->m_TimerAxisHelper.m_pTimeAxis->SetTimer(dwEventID, pTimerSink, dwElapse, szTimerDesc);
}

/////////////////////////////////////////////////////////
////
//// ���ٶ�ʱ��
////
/////////////////////////////////////////////////////////
void CGameModuleFacade::KillTimer(DWORD dwEventID, ITimerSink * pTimerSink)
{
	g_pGlobal->m_TimerAxisHelper.m_pTimeAxis->KillTimer(dwEventID, pTimerSink);
}

/////////////////////////////////////////////////////////
////
//// ȡ��������������
////
/////////////////////////////////////////////////////////
IConfigDataCenter * CGameModuleFacade::GetConfigDataCenter(void)
{
	return g_pGlobal->m_pDataCenter;
}

/////////////////////////////////////////////////////////
//// 
//// ȡ�õ�ǰ��ͼ����
////
/////////////////////////////////////////////////////////
IMapView *	CGameModuleFacade::GetMapView(void)
{
	return g_pGlobal->m_pMapView;
}

/////////////////////////////////////////////////////////
//// ��  �����ͻ����߼���������б�
//// ��  ����
//// ����ֵ��
/////////////////////////////////////////////////////////
IThingTable * CGameModuleFacade::GetThingTable(void)
{
	IClientWorld * pClientWorld = GetClientWorld();
	if(pClientWorld == NULL)
	{
		return NULL;
	}

	return pClientWorld->GetThingTable();	
}

/////////////////////////////////////////////////////////
//// ��  �����ͻ�����Ϸ����
//// ��  ����
//// ����ֵ��
/////////////////////////////////////////////////////////
IClientWorld * CGameModuleFacade::GetClientWorld(void)
{
	return g_pGlobal->m_pClientWorld;
}

/////////////////////////////////////////////////////////
////
//// CMapItemView������
////
/////////////////////////////////////////////////////////
IItemClassFactory *	CGameModuleFacade::GetItemFactory(void)
{
	return g_pGlobal->m_pItemClassFactory;
}

/////////////////////////////////////////////////////////
////
//// ȡ��[���ʱ���ʱ�䣬������Ϣ�з����ͻ���]
////
/////////////////////////////////////////////////////////
DWORD CGameModuleFacade::GetMsgTimeTick(void)
{
	return (g_pGlobal->m_TimerAxisHelper.m_pTimeAxis->GetTimeTick() - g_pGlobal->m_dwLoginTimeTick);
}

/////////////////////////////////////////////////////////
////
//// ȡ��һ����ϵͳ�������
////
/////////////////////////////////////////////////////////
DWORD CGameModuleFacade::GetRandomNumber(void)
{
	static BOOL bInit = FALSE;
	if(bInit == FALSE)
	{
		m_RandomGenerator.Seed();
		bInit = TRUE;
	}
	
	return m_RandomGenerator.GetDWord();
}

/////////////////////////////////////////////////////////
//// ��  ����ȡ��һ���߼�����
//// ��  ����
//// ����ֵ��
/////////////////////////////////////////////////////////
IThing * CGameModuleFacade::GetThing(UID uidThing)
{
	if(g_pGlobal->m_pClientWorld == NULL || g_pGlobal->m_pClientWorld->GetThingTable() == NULL)
	{
		return NULL;
	}

	return g_pGlobal->m_pClientWorld->GetThingTable()->Find(uidThing);
}
