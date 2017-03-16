/**
*	created:		2012-7-2   22:43
*	filename: 		CClientThingFactory
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����һ���߼���������ͣ����ֳ�������һ���߼�����ָ��
					�߼����������ͼ�ϵ���,����,NPC,װ��,���͵��
*/
//------------------------------------------------------------------------
#include "../Include/CClientThingFactory.h"
#include "../Include/ThingClientCommon.h"
#include "../Include/CHero.h"
#include "../Include/CActor.h"
#include "../Include/CMonster.h"
#include "../Include/CThingClientFacade.h"
#include "../Include/CreatureCommandBase.h"
#include "../Include/CEquipment.h"
#include "../Include/CMedicament.h"
#include "../Include/CValuables.h"
#include "../Include/CWarrent.h"
#include "../Include/CChunnel.h"
#include "../Include/HeroPartEquip.h"
#include "../Include/ActorPartEquip.h"
#include "../Include/HeroPartPacket.h"
#include "../Include/CBox.h"
#include "../Include/CStall.h"
#include "../Include/CreaturePartConjure.h"
#include "../Include/ActorPartRide.h"
#include "../Include/HeroPartRide.h"
//------------------------------------------------------------------------
typedef IThing * (CThingFactory:: *THING_CREATE_PROC)(UID uid, LPCSTR buf, int nLen);
//------------------------------------------------------------------------
// ÿһ���߼����������ѡ��������ϸ���ֳ����������ô��Ե��ֳ�����
// ����Ӧ������Ϸ�е�����������Ҫ�����Լ��Ĺ������������ȸ�������
// ������Ʒ����ÿһ����Ʒ�����㿴����ֻ�ܿ������ǵĽ�ɫ����,�Ա�
// װ���ȣ� ���ǵ���ϸ���Ի�����Ʒ������Ʒ�������ǲ��ᷢ������
// ������Ҫ֧�����ַ�ʽ����
IThing * CThingFactory::CreatePrivateThing(DWORD dwThingClassID, UID uid, const char * buf, int nLen)
{
	static THING_CREATE_PROC createproc[THINGCLASS_MAX] =
	{
		0											,	// THINGCLASS_NONE	
		(THING_CREATE_PROC)&CThingFactory::CreateActor_private		,	// THINGCLASS_ACTOR
		0											,	// THINGCLASS_MONSTER
		0											,	// THINGCLASS_CONTAINER
		(THING_CREATE_PROC)&CThingFactory::CreateGoods_private		,	// THINGCLASS_GOODS
		0											,	// THINGCLASS_CHUNNEL
		0											,   // THINGCLASS_BOX
		0											,	// THINGCLASS_STALL
	};

	if(dwThingClassID < 0 || dwThingClassID >= THINGCLASS_MAX || createproc[dwThingClassID] == NULL)
	{
		TraceLn("CreateThing() : dwThingClassID = %d is Invalid!", dwThingClassID);
		return NULL;
	}

	IThing * pThing = (this->*createproc[dwThingClassID])(uid, buf, nLen);

	if(pThing == NULL)
	{
		TraceLn("CreateThing() : dwThingClassID = %d Failed!", dwThingClassID);
	}
	else
	{
		// �㲥һ�¼���ȥ����֪����һ�µ��߼�����
		WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_BORN);
		pThing->OnMessage(wMsgID, (LPCSTR)buf, nLen);
	}

	return pThing;
}
//------------------------------------------------------------------------
IThing * CThingFactory::CreatePublicThing(DWORD dwThingClassID, UID uid, const char * buf, int nLen)
{
	static THING_CREATE_PROC createproc[THINGCLASS_MAX] =
	{
		0											,	// THINGCLASS_NONE	
		(THING_CREATE_PROC)&CThingFactory::CreateActor_public		,	// THINGCLASS_ACTOR
		(THING_CREATE_PROC)&CThingFactory::CreateMonster_public		,	// THINGCLASS_MONSTER
		0											,	// THINGCLASS_CONTAINER
		(THING_CREATE_PROC)&CThingFactory::CreateGoods_public		,	// THINGCLASS_GOODS
		(THING_CREATE_PROC)&CThingFactory::CreateChunnel_public		,	// THINGCLASS_CHUNNEL
		(THING_CREATE_PROC)&CThingFactory::CreateBox_public         ,   // THINGCLASS_BOX
		(THING_CREATE_PROC)&CThingFactory::CreateStall_public		,	// THINGCLASS_STALL
	};

	if(dwThingClassID < 0 || dwThingClassID >= THINGCLASS_MAX || createproc[dwThingClassID] == NULL)
	{
		TraceLn("CreateThing() : dwThingClassID = %d is Invalid!", dwThingClassID);
		return NULL;
	}

	IThing * pThing = (this->*createproc[dwThingClassID])(uid, buf, nLen);

	if(pThing == NULL)
	{
		TraceLn("CreateThing() : dwThingClassID = %d Failed!", dwThingClassID);
	}
	else
	{		
		// ����һ�¼���ȥ����֪����һ�µ��߼�����
		WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_BORN);
		pThing->OnMessage(wMsgID, (LPCSTR)buf, nLen);
	}
		
	return pThing;
}
//------------------------------------------------------------------------
void CThingFactory::Release(void)
{
	delete this;
}
//------------------------------------------------------------------------
IThing * CThingFactory::CreateActor_private(UID uid, LPCSTR buf, int nLen)
{
	if(uid == INVALID_UID || buf == NULL || nLen <= 0)
	{
		return NULL;
	}
	PrintEventLn("��ʼ��������! [uid = %08X%08X]", uid);

	// �����ֳ�
	CHero * pHero = new CHero();
	pHero->SetUID(uid);
	if(pHero->OnSetPrivateContext(buf, nLen) == FALSE)
	{
		PrintEventLn("������������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pHero);
		return NULL;
	}

	// ����
	if(pHero->Create() == FALSE)
	{
		PrintEventLn("��������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pHero);
		return NULL;
	}
	/*
	// ����װ��������
	CHeroPart_Equip * pHeroPart_Equip = new CHeroPart_Equip();
	if(!pHeroPart_Equip->Create(pHero, NULL, 0))
	{
		PrintEventLn("����װ��������ʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);		
		return NULL;
	}	

	// ��������������
	CHeroPart_Packet * pHeroPart_Packet = new CHeroPart_Packet();
	if(!pHeroPart_Packet->Create(pHero, NULL, 0))
	{
		PrintEventLn("��������������ʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);		
		return NULL;
	}

	// ������ȴϵͳ����

	IColdTimeMgr * pColdTimeMgr = g_pThingClientFacade->GetColdTimeSystem()->CreateColdTimeMgr();
	if(pColdTimeMgr == NULL)
	{
		PrintEventLn("������ȴϵͳ����ʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);
		return NULL;
	}
	pColdTimeMgr->Create(pHero, NULL, 0);

	// ����״̬����
	IStatusClient * pStatusClient = g_pThingClientFacade->GetStatusClient();
	if(pStatusClient == NULL)
	{
		PrintEventLn("ȡ��״̬ģ��ʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pColdTimeMgr);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);
		return NULL;
	}

	IStatusMgr * pStatusMgr = pStatusClient->CreateStatusMgr();
	if(pStatusMgr == NULL)
	{
		PrintEventLn("����״̬����ʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pColdTimeMgr);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);
		return NULL;
	}
	pStatusMgr->Create(pHero, NULL, 0);	

	// ����PK����
	IClientPKSystem * pClientPKSystem = g_pThingClientFacade->GetPKSystem();
	if(pClientPKSystem == NULL)
	{
		PrintEventLn("ȡ��PKϵͳʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pStatusMgr);
		SAFE_RELEASE(pColdTimeMgr);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);
		return NULL;
	}
	IClientPKManager * pPKManager = pClientPKSystem->CreatePlayerPKManager(pHero, 0, 0);
	if(pPKManager == NULL)
	{
		PrintEventLn("����PK����ʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pStatusMgr);
		SAFE_RELEASE(pColdTimeMgr);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);
		return NULL;
	}

	// �����������ܲ���
	IWorkSkillSystem * pWorkSkillSystem = g_pThingClientFacade->GetWorkSkillSystem();
	if(pWorkSkillSystem == NULL)
	{
		PrintEventLn("ȡ�ù�������ϵͳʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pPKManager);
		SAFE_RELEASE(pStatusMgr);
		SAFE_RELEASE(pColdTimeMgr);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);
		return NULL;
	}
	IWorkSkillMgr * pWorkSkillMgr = NULL;	
	if(!pWorkSkillSystem->CreateWorkSkillMgr(&pWorkSkillMgr) || pWorkSkillMgr == NULL)
	{
		PrintEventLn("�����������ܲ���ʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pWorkSkillMgr);
		SAFE_RELEASE(pPKManager);
		SAFE_RELEASE(pStatusMgr);
		SAFE_RELEASE(pColdTimeMgr);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);
		return NULL;
	}
	pWorkSkillMgr->Create(pHero, NULL, 0);	

	// �������ܿ�����
	ISkillManager * pSkillManager = g_pThingClientFacade->GetSkillManager();
	if(pSkillManager == NULL)
	{
		PrintEventLn("ȡ�ü���ϵͳʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pWorkSkillMgr);
		SAFE_RELEASE(pPKManager);
		SAFE_RELEASE(pStatusMgr);
		SAFE_RELEASE(pColdTimeMgr);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);
		return NULL;
	}
	ISkillControl * pSkillControl = pSkillManager->CreateSkillControl();
	if(pSkillControl == NULL)
	{
		PrintEventLn("�������ܲ���ʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pWorkSkillMgr);
		SAFE_RELEASE(pPKManager);
		SAFE_RELEASE(pStatusMgr);
		SAFE_RELEASE(pColdTimeMgr);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);
		return NULL;
	}

	// ���岿��
	IClanClient * pClanClient = g_pThingClientFacade->GetClanClient();
	IClanClientPate * pClanPart = NULL;
	if(pClanClient == NULL || (pClanPart = pClanClient->GetClanPatePart()) == NULL)
	{
		PrintEventLn("�������岿��ʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pSkillControl);
		SAFE_RELEASE(pWorkSkillMgr);
		SAFE_RELEASE(pPKManager);
		SAFE_RELEASE(pStatusMgr);
		SAFE_RELEASE(pColdTimeMgr);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);
		return NULL;
	}
	pClanPart->Create(pHero, NULL, 0);
	

	// �ٻ�����
	CCreaturePart_Conjure * pConjurePart = new CCreaturePart_Conjure();
	if(!pConjurePart->Create(pHero, NULL, 0))
	{
		PrintEventLn("�����ٻ�����ʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pConjurePart);
		//SAFE_RELEASE(pClanClient);
		//SAFE_RELEASE(pSkillControl);
		//SAFE_RELEASE(pWorkSkillMgr);
		//SAFE_RELEASE(pPKManager);
		//SAFE_RELEASE(pStatusMgr);
		//SAFE_RELEASE(pColdTimeMgr);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);
		return NULL;
	}

	// ��˲���
	CHeroPart_Ride * pRidePart = new CHeroPart_Ride();
	if(!pRidePart->Create(pHero, NULL, 0))
	{
		PrintEventLn("������˲���ʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pRidePart);
		SAFE_RELEASE(pConjurePart);
		//SAFE_RELEASE(pClanClient);
		//SAFE_RELEASE(pSkillControl);
		//SAFE_RELEASE(pWorkSkillMgr);
		//SAFE_RELEASE(pPKManager);
		//SAFE_RELEASE(pStatusMgr);
		//SAFE_RELEASE(pColdTimeMgr);
		SAFE_RELEASE(pHeroPart_Packet);
		SAFE_RELEASE(pHeroPart_Equip);
		SAFE_RELEASE(pHero);
		return NULL;
	}

	// ��Ӳ���
	pHero->AddPart(static_cast<IThingPart *>(pHeroPart_Equip));
	pHero->AddPart(static_cast<IThingPart *>(pHeroPart_Packet));
	//pHero->AddPart(static_cast<IThingPart *>(pColdTimeMgr));
	//pHero->AddPart(static_cast<IThingPart *>(pStatusMgr));
	//pHero->AddPart(static_cast<IThingPart *>(pPKManager));
	//pHero->AddPart(static_cast<IThingPart *>(pWorkSkillMgr));
	//pHero->AddPart(static_cast<IThingPart *>(pSkillControl));
	//pHero->AddPart(static_cast<IThingPart *>(pClanPart));
	pHero->AddPart(static_cast<IThingPart *>(pConjurePart));
	pHero->AddPart(static_cast<IThingPart *>(pRidePart));

	// �������
	pHero->AddCreatureCommand(CCreatureCommand_Stand::GetInstance());
	pHero->AddCreatureCommand(CCreatureCommand_ChanageMoveStyle::GetInstance());
*/
	PrintEventLn("�������ǳɹ���");
	return pHero;
}
//------------------------------------------------------------------------
#define CREATE_THING_PUBLIC_TEMPLATE(pThing)							\
		if(pThing == NULL)												\
		{																\
			return NULL;												\
		}																\
																		\
		pThing->SetUID(uid);											\
		if(pThing->OnSetPublicContext(buf, nLen) == FALSE)				\
		{																\
			SAFE_RELEASE(pThing);										\
			return NULL;												\
		}																\
																		\
		if(pThing->Create() == FALSE)									\
		{																\
			SAFE_RELEASE(pThing);										\
			return NULL;												\
		}

//------------------------------------------------------------------------
#define CREATE_THING_PRIVATE_TEMPLATE(pThing)							\
		if(pThing == NULL)												\
		{																\
			return NULL;												\
		}																\
																		\
		pThing->SetUID(uid);											\
		if(pThing->OnSetPrivateContext(buf, nLen) == FALSE)				\
		{																\
			SAFE_RELEASE(pThing);										\
			return NULL;												\
		}																\
																		\
		if(pThing->Create() == FALSE)									\
		{																\
			SAFE_RELEASE(pThing);										\
			return NULL;												\
		}

//------------------------------------------------------------------------
IThing * CThingFactory::CreateActor_public(UID uid, LPCSTR buf, int nLen)
{
	CActor * pActor = new CActor();
	pActor->SetUID(uid);
	if(!pActor->OnSetPublicContext(buf, nLen))
	{
		PrintEventLn("���ý�ɫ����ʧ��! [uid = %08X%08X]", uid);
		SAFE_RELEASE(pActor);
		return NULL;
	}
	
	if(!pActor->Create())
	{
		PrintEventLn("������ɫʧ��! [uid = %08X%08X] [%s]", uid, pActor->GetName());
		SAFE_RELEASE(pActor);
		return NULL;
	}

	// ����װ��������
	CActorPart_Equip * pActorPart_Equip = new CActorPart_Equip();
	if(!pActorPart_Equip->Create(pActor, NULL, 0))
	{
		PrintEventLn("����װ��������ʧ�ܣ�[uid = %08X%08X] [%s]", uid, pActor->GetName());		
		SAFE_RELEASE(pActorPart_Equip);
		SAFE_RELEASE(pActor);
		return NULL;
	}
/*
	// ����״̬����
	IStatusClient * pStatusClient = g_pThingClientFacade->GetStatusClient();
	if(pStatusClient == NULL)
	{
		PrintEventLn("ȡ��״̬����ϵͳʧ�ܣ�[uid = %08X%08X] [%s]", uid, pActor->GetName());		
		SAFE_RELEASE(pActorPart_Equip);
		SAFE_RELEASE(pActor);
		return NULL;
	}
	IStatusMgr * pStatusMgr = pStatusClient->CreateStatusMgr();
	if(pStatusMgr == NULL)
	{
		PrintEventLn("����״̬����ʧ�ܣ�[uid = %08X%08X] [%s]", uid, pActor->GetName());		
		SAFE_RELEASE(pActorPart_Equip);
		SAFE_RELEASE(pActor);
		return NULL;
	}
	pStatusMgr->Create(pActor, NULL, 0);	

	// ���岿��
	IClanClient * pClanClient = g_pThingClientFacade->GetClanClient();
	IClanClientPate * pClanPart = NULL;
	if(pClanClient == NULL || (pClanPart = pClanClient->GetClanPatePart()) == NULL)
	{
		PrintEventLn("�������岿��ʧ�ܣ�[uid = %08X%08X] [%s]", uid, pActor->GetName());
		SAFE_RELEASE(pStatusMgr);		
		SAFE_RELEASE(pActorPart_Equip);
		SAFE_RELEASE(pActor);
		return NULL;
	}
	pClanPart->Create(pActor, NULL, 0);
*/
	// �ٻ�����
	CCreaturePart_Conjure * pConjurePart = new CCreaturePart_Conjure();
	if(!pConjurePart->Create(pActor, NULL, 0))
	{
		PrintEventLn("�����ٻ�����ʧ�ܣ�[uid = %08X%08X] [%s]", uid, pActor->GetName());
		SAFE_RELEASE(pConjurePart);
		// SAFE_RELEASE(pStatusMgr);		
		SAFE_RELEASE(pActorPart_Equip);
		SAFE_RELEASE(pActor);
		return NULL;
	}

	// ��˲���
	CHeroPart_Ride * pRidePart = new CHeroPart_Ride();
	if(!pRidePart->Create(pActor, NULL, 0))
	{
		PrintEventLn("������˲���ʧ�ܣ�[uid = %08X%08X] [%s]", uid, pActor->GetName());
		SAFE_RELEASE(pRidePart);
		SAFE_RELEASE(pConjurePart);
		// SAFE_RELEASE(pStatusMgr);		
		SAFE_RELEASE(pActorPart_Equip);
		SAFE_RELEASE(pActor);
		return NULL;
	}
/*
	// ����PK����
	IClientPKSystem * pClientPKSystem = g_pThingClientFacade->GetPKSystem();
	if(pClientPKSystem == NULL)
	{
		PrintEventLn("ȡ��PKϵͳʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pRidePart);
		SAFE_RELEASE(pConjurePart);
		SAFE_RELEASE(pStatusMgr);		
		SAFE_RELEASE(pActorPart_Equip);
		SAFE_RELEASE(pActor);
		return NULL;
	}
	IClientPKManager * pPKManager = pClientPKSystem->CreatePlayerPKManager(pActor, 0, 0);
	if(pPKManager == NULL)
	{
		PrintEventLn("����PK����ʧ�ܣ���������ʧ�ܣ�[uid = %08X%08X]", uid);
		SAFE_RELEASE(pRidePart);
		SAFE_RELEASE(pConjurePart);
		SAFE_RELEASE(pStatusMgr);		
		SAFE_RELEASE(pActorPart_Equip);
		SAFE_RELEASE(pActor);
		return NULL;
	}
*/	
	// ��Ӳ���
	pActor->AddPart(static_cast<IThingPart *>(pActorPart_Equip));
	// pActor->AddPart(static_cast<IThingPart *>(pStatusMgr));
	// pActor->AddPart(static_cast<IThingPart *>(pClanPart));
	pActor->AddPart(static_cast<IThingPart *>(pConjurePart));
	pActor->AddPart(static_cast<IThingPart *>(pRidePart));
	// pActor->AddPart(static_cast<IThingPart *>(pPKManager));

	return pActor;
}
//------------------------------------------------------------------------
// ��  ������������
IThing * CThingFactory::CreateMonster_public(UID uid, LPCSTR buf, int nLen)
{
	CMonster * pMonster = new CMonster();
	CREATE_THING_PUBLIC_TEMPLATE(pMonster);	

/*
	// ����״̬����
	IStatusClient * pStatusClient = g_pThingClientFacade->GetStatusClient();
	if(pStatusClient == NULL)
	{
		PrintEventLn("ȡ��״̬����ϵͳʧ�ܣ�[uid = %08X%08X] [%s]", uid, pMonster->GetName());
		SAFE_RELEASE(pMonster);
		return NULL;
	}
	IStatusMgr * pStatusMgr = pStatusClient->CreateStatusMgr();
	if(pStatusMgr == NULL)
	{
		PrintEventLn("����״̬����ʧ�ܣ�[uid = %08X%08X] [%s]", uid, pMonster->GetName());
		SAFE_RELEASE(pMonster);
		return NULL;
	}
	pStatusMgr->Create(pMonster, NULL, 0);
*/

	// �ٻ�����
	CCreaturePart_Conjure * pConjurePart = new CCreaturePart_Conjure();
	if(!pConjurePart->Create(pMonster, NULL, 0))
	{
		PrintEventLn("�����ٻ�����ʧ�ܣ�[uid = %08X%08X] [%s]", uid, pMonster->GetName());
		SAFE_RELEASE(pConjurePart);
		// SAFE_RELEASE(pStatusMgr);
		SAFE_RELEASE(pMonster);
		return NULL;
	}

	// ��Ӳ���
	// pMonster->AddPart(static_cast<IThingPart *>(pStatusMgr));
	pMonster->AddPart(static_cast<IThingPart *>(pConjurePart));

	return static_cast<IThing *>(pMonster);;
}
//------------------------------------------------------------------------
// ��  ����������Ʒ
IThing * CThingFactory::CreateGoods_private(UID uid, LPCSTR buf, int nLen)
{
	if(nLen < sizeof(SGOODS_PROPERTY_BASE))
	{
		return NULL;
	}

	SGOODS_PROPERTY_BASE * pPropertyBase = (SGOODS_PROPERTY_BASE *)(buf);
	// ȡ���Ƿ�Ϊ��װ����ҩƷ��ƾ֤��Ԫ����
	IConfigDataCenter * pConfigDataCenter = g_pThingClientFacade->GetConfigDataCenter();
	if(pConfigDataCenter == NULL)
	{
		return NULL;
	}

	GOODS_BASEPROPERTY baseproperty = pConfigDataCenter->GetGoodsBaseProperty(pPropertyBase->dwGoodsID);
	if(baseproperty.lClass == GOODS_CLASS_EQUIPMENT)
	{
		CEquipment * pEquipment = new CEquipment();
		CREATE_THING_PRIVATE_TEMPLATE(pEquipment);

		return static_cast<IThing *>(pEquipment);		
	}
	else if(baseproperty.lClass == GOODS_CLASS_MEDICAMENT)
	{
		CMedicament * pMedicament = new CMedicament();
		CREATE_THING_PRIVATE_TEMPLATE(pMedicament);

		return static_cast<IThing *>(pMedicament);	
	}
	else if(baseproperty.lClass == GOODS_CLASS_WARRANT)
	{
		CWarrant * pWarrant = new CWarrant();
		CREATE_THING_PRIVATE_TEMPLATE(pWarrant);

		return static_cast<IThing *>(pWarrant);	
	}
	else if(baseproperty.lClass == GOODS_CLASS_VALUABLES)
	{
		CValuables * pValuables = new CValuables();
		CREATE_THING_PRIVATE_TEMPLATE(pValuables);

		return static_cast<IThing *>(pValuables);	
	}
	else
	{
		return NULL;
	}

	return NULL;
}
//------------------------------------------------------------------------
IThing * CThingFactory::CreateGoods_public(UID uid, LPCSTR buf, int nLen)
{
	if(nLen < sizeof(SGOODS_PROPERTY_BASE))
	{
		return NULL;		
	}

	SGOODS_PROPERTY_BASE * pPropertyBase = (SGOODS_PROPERTY_BASE *)(buf);
	// ȡ���Ƿ�Ϊ��װ����ҩƷ��ƾ֤��Ԫ����
	IConfigDataCenter * pConfigDataCenter = g_pThingClientFacade->GetConfigDataCenter();
	if(pConfigDataCenter == NULL)
	{
		return NULL;
	}

	GOODS_BASEPROPERTY baseproperty = pConfigDataCenter->GetGoodsBaseProperty(pPropertyBase->dwGoodsID);
	if(baseproperty.lClass == GOODS_CLASS_EQUIPMENT)
	{
		CEquipment * pEquipment = new CEquipment();
		CREATE_THING_PUBLIC_TEMPLATE(pEquipment);

		return static_cast<IThing *>(pEquipment);
	}
	else if(baseproperty.lClass == GOODS_CLASS_MEDICAMENT)
	{
		CMedicament * pMedicament = new CMedicament();
		CREATE_THING_PUBLIC_TEMPLATE(pMedicament);

		return static_cast<IThing *>(pMedicament);
	}
	else if(baseproperty.lClass == GOODS_CLASS_WARRANT)
	{
		CWarrant * pWarrant = new CWarrant();
		CREATE_THING_PUBLIC_TEMPLATE(pWarrant);

		return static_cast<IThing *>(pWarrant);
	}
	else if(baseproperty.lClass == GOODS_CLASS_VALUABLES)
	{
		CValuables * pValuables = new CValuables();
		CREATE_THING_PUBLIC_TEMPLATE(pValuables);

		return static_cast<IThing *>(pValuables);
	}
	else
	{
		return NULL;
	}

	return NULL;
}
//------------------------------------------------------------------------
// ��  ����������ͼͨ��
IThing * CThingFactory::CreateChunnel_public(UID uid, LPCSTR buf, int nLen)
{
	CChunnel * pChunnel = new CChunnel();
	CREATE_THING_PUBLIC_TEMPLATE(pChunnel);

	return static_cast<IThing *>(pChunnel);
}
//------------------------------------------------------------------------
// ��  ������������
IThing * CThingFactory::CreateBox_public(UID uid, LPCSTR buf, int nLen)
{
	CBox* pBox = new CBox;
	CREATE_THING_PUBLIC_TEMPLATE(pBox);
	return pBox;
}
//------------------------------------------------------------------------
// ��  ��������̯λ
IThing * CThingFactory::CreateStall_public(UID uid, LPCSTR buf, int nLen)
{
	CStall * pStall = new CStall;
	CREATE_THING_PUBLIC_TEMPLATE(pStall);
	return pStall;
}
//------------------------------------------------------------------------