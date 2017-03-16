/**
*	created:		2012-7-2   23:43
*	filename: 		CThingClientFacade
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ThingClient�����.���ฺ���ⲿ�ӿڴ��,�����һ��,����Ϸ�߼��޹�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/SceneObj/IThing.h"
#include "../../FKGame/Common/IConfigDataCenter.h"
#include "../../FKGameModule/Include/Random.h"
#include "../../FKGameModule/Include/AccountSpace.h"
//------------------------------------------------------------------------
// ����
struct IScriptUpdateManager;
class  IEventServer;
struct IMapView;
struct IContainerMgr;
struct IGoods;
struct IEquipment;
struct IStatusClient;
struct IClientPKSystem;
struct ISkillManager;
struct IResMgr;
struct ITimerSink;
struct IThingTable;
struct IClientWorld;
struct IWorkSkillSystem;
struct IClanClient;
struct IEffectFactory;
struct IItemClassFactory;
struct IColdTimeSystem;
//------------------------------------------------------------------------
#define ACCOUNTSPACE_WIDTH		32
#define ACCOUNTSPACE_HIGH		32
//------------------------------------------------------------------------
// ����
class CThingClientFacade
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

	// ȡ����ȴϵͳ
	virtual IColdTimeSystem *		GetColdTimeSystem(void);

	// ȡ������������
	virtual IContainerMgr *			GetContainerMgr(void);

	// ȡ����Ʒ
	virtual IGoods *				GetGoods(UID uidGoods);

	// ȡ��״̬ϵͳ
	virtual IStatusClient *			GetStatusClient(void);

	// ȡ��pkϵͳ
	virtual IClientPKSystem *		GetPKSystem(void);

	// ȡ��Ч��ϵͳ
	virtual IEffectFactory *		GetEffectFactory(void);

	// ȡ�ù�������ϵͳ
	virtual IWorkSkillSystem *		GetWorkSkillSystem(void);

	// ȡ�ü��ܹ�����
	virtual ISkillManager *			GetSkillManager(void);

	// ȡ��װ��
	virtual IEquipment *			GetEquipment(UID uidEquipment);

	// ȡ�����岿��
	virtual IClanClient *			GetClanClient(void);

private:
	CAccountSpace<ACCOUNTSPACE_WIDTH, ACCOUNTSPACE_HIGH>	m_pAccountSpace;
	CRandom													m_RandomGenerator;
};
//------------------------------------------------------------------------
extern CThingClientFacade *			g_pThingClientFacade;
//------------------------------------------------------------------------