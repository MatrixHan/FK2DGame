/**
*	created:		2012-6-24   23:12
*	filename: 		MonsterFinder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "MonsterFinder.h"
//------------------------------------------------------------------------
CMonsterFinder::CMonsterFinder()
{

}
//------------------------------------------------------------------------
CMonsterFinder::~CMonsterFinder()
{

}
//------------------------------------------------------------------------
IThing * CMonsterFinder::GetNextMonster(BOOL bHPMustFull, BOOL bExcludeNPC)
{
	/*
	if (g_pGlobal->m_pPlayer == NULL
		|| g_pGlobal->m_pMapView == NULL)
	{
		return NULL;
	}

	m_uidFirstMonster = INVALID_UID;

	POINTS ActorLoc = g_pGlobal->m_pPlayer->GetLoc();
	IThing *pThing = __GetFreeThing(ActorLoc, bHPMustFull, bExcludeNPC);
	if (pThing)
	{
		return pThing;
	}
	
	//ȫ�����ҹ���, ��������
	setUIDMonster.clear();
	if (m_uidFirstMonster != INVALID_UID)
	{
		setUIDMonster.insert(m_uidFirstMonster);
	}
	
	return g_MiscFacade.GetThingFromUID(m_uidFirstMonster);
	*/
	return NULL;
}
//------------------------------------------------------------------------
IThing * CMonsterFinder::__GetFreeThing(POINTS ptActor, BOOL bHPMustFull, BOOL bExcludeNPC)
{
	/*
	if (g_pGlobal->m_pClientWorld == NULL)
	{
		return NULL;
	}

	IThingTable *pThingTable = g_pGlobal->m_pClientWorld->GetThingTable();
	if (pThingTable == NULL)
	{
		return NULL;
	}

	TVECTOR_THING *pVectThing = pThingTable->GetThingList();
	if (pVectThing == NULL)
	{
		return NULL;
	}

	TVECTOR_THING &vectThings = *pVectThing;
	for (int i = 0; i < vectThings.size(); ++i)
	{
		IThing *pThing = vectThings[i];
		static CAccountSpace<> s_DistanceCounter;
		if (g_MiscFacade.GetThingClass(pThing->GetUID()) != THINGCLASS_MONSTER	//�ҵ��Ĳ��ǹ�
			|| s_DistanceCounter.Distance(ptActor.x, ptActor.y, pThing->GetLoc().x, pThing->GetLoc().y) > MAX_SEARCHRADIUS) //����̫Զ
		{
			continue;
		}
		
		IMonster *pMonster = (IMonster *)pThing;	
		MONSTER_CONFIG_INFOR &MonsterInfo = g_pGlobal->m_pDataCenter->GetMonsterInfor(pMonster->GetMonsterID());		
		if (MonsterInfo.ltype == MONSTER_TYPE_SAFEGUARD
			|| MonsterInfo.ltype == MONSTER_TYPE_HATCHETMAN)
		{
			continue;
		}

		if (bHPMustFull)  //���������жϹ��Ƿ��й�����
		{			 
			int nMaxHP = pMonster->GetPropNum(CREATURE_PROPID_HPMAXCUR);
			int nCurHP = pMonster->GetPropNum(CREATURE_PROPID_HPCUR);
			if (nCurHP != nMaxHP)
			{
				continue;
			}			
		}

		if (bExcludeNPC)
		{
			MONSTER_CONFIG_INFOR MonsterInfo = g_pGlobal->m_pDataCenter->GetMonsterInfor( pMonster->GetMonsterID() );
			if (MonsterInfo.ltype == MONSTER_TYPE_NOATTACKNPC
				|| MonsterInfo.ltype == MONSTER_TYPE_GUARD)
			{
				continue;
			}			
		}
 
		UID uidThing = pThing->GetUID();

		if (m_uidFirstMonster == INVALID_UID)
		{//���������ĵ�һֻ�ּ���, ����ѭ��ʱѡ��
			m_uidFirstMonster = uidThing;
		}

		if (setUIDMonster.find(uidThing) != setUIDMonster.end())
		{
			//Щ���Ѿ�ѡ�й�
			continue;
		}
		
		setUIDMonster.insert(uidThing);
		return pThing;
	}
	*/
	return NULL;
}
//------------------------------------------------------------------------