/**
*	created:		2012-6-25   23:48
*	filename: 		GameModuleFacade
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/SceneObj/IThing.h"
#include "../../FKGame/Common/IConfigDataCenter.h"
#include "Random.h"
#include "AccountSpace.h"
//------------------------------------------------------------------------
// ����
struct IScriptUpdateManager;
class  IEventServer;
struct IMapView;
struct IResMgr;
struct ITimerSink;
struct IThingTable;
struct IClientWorld;
struct IItemClassFactory;
//------------------------------------------------------------------------
#define ACCOUNTSPACE_WIDTH		32
#define ACCOUNTSPACE_HIGH		32
//------------------------------------------------------------------------
// ����
class CGameModuleFacade
{
public:
	// �ű����뼰��̬���¹�����
	virtual IScriptUpdateManager *	GetScriptUpdateManager(void);	

	// SCP�ű�������
	virtual IResMgr	*				GetResMgr(void);

	// ����·��
	virtual char *					GetWorkDir(void);

	// �¼�������
	virtual IEventServer *			GetEventServer(void);

	// ���ö�ʱ��
	virtual BOOL					SetTimer(DWORD dwEventID, ITimerSink * pTimerSink, DWORD dwElapse, char *szTimerDesc = NULL);	
	
	// ���ٶ�ʱ��
	virtual void					KillTimer(DWORD dwEventID, ITimerSink * pTimerSink);

	// ȡ��������������
	virtual IConfigDataCenter *		GetConfigDataCenter(void);	

	// �ռ������
	virtual CAccountSpace<ACCOUNTSPACE_WIDTH, ACCOUNTSPACE_HIGH> *	GetAccountSpace(void);

	// ȡ�õ�ǰ��ͼ����
	virtual IMapView *				GetMapView(void);

	// �ͻ����߼���������б�
	virtual IThingTable *			GetThingTable(void);

	// �ͻ�����Ϸ����
	virtual IClientWorld *			GetClientWorld(void);

	// CMapItemView������
	virtual IItemClassFactory *		GetItemFactory(void);

	// ȡ��[���ʱ���ʱ�䣬������Ϣ�з����ͻ���]
	virtual DWORD					GetMsgTimeTick(void);

	// ȡ��һ����ϵͳ�������
	virtual DWORD					GetRandomNumber(void);

	// ȡ��һ���߼�����
	virtual IThing *				GetThing(UID uidThing);

private:
	CAccountSpace<ACCOUNTSPACE_WIDTH, ACCOUNTSPACE_HIGH>	m_pAccountSpace;
	CRandom													m_RandomGenerator;
};
//------------------------------------------------------------------------
extern CGameModuleFacade *			g_pGameModuleFacade;
//------------------------------------------------------------------------