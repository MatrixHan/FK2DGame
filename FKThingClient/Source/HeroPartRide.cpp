/**
*	created:		2012-7-4   1:12
*	filename: 		HeroPartRide
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������˲���
*/
//------------------------------------------------------------------------
#include "../Include/ThingClientCommon.h"
#include "../Include/HeroPartRide.h"
//------------------------------------------------------------------------
CHeroPart_Ride::CHeroPart_Ride(void)
{

}
//------------------------------------------------------------------------
CHeroPart_Ride::~CHeroPart_Ride(void)
{

}
//------------------------------------------------------------------------
void CHeroPart_Ride::Mount(SC_MSG_GETRIDEDATA_CONTEXT::HORSEDATA horse)
{
	// �л�״̬
	m_pMaster->GotoActState(CREATURE_STATEID_RIDE, NULL, 0);
	
	// �任���
	CActorPart_Ride::Mount(horse);
}
//------------------------------------------------------------------------
void CHeroPart_Ride::Dismount(void)
{
	// �л�״̬
	int nWalkStyle = m_pMaster->GetPropNum(CREATURE_PROPID_WALKSTYLE);
	if(nWalkStyle == CREATURE_MOVESTYLE_SIT)
	{
		m_pMaster->GotoActState(CREATURE_STATEID_SIT, NULL, 0);
	}
	else
	{
		m_pMaster->GotoActState(CREATURE_STATEID_MOVE, NULL, 0);
	}

	// �任���
	CActorPart_Ride::Dismount();
}
//------------------------------------------------------------------------