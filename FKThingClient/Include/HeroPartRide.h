/**
*	created:		2012-7-4   1:10
*	filename: 		HeroPartRide
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������˲���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "ActorPartRide.h"
//------------------------------------------------------------------------
class CHeroPart_Ride : public CActorPart_Ride
{
public:
	// ��
	virtual void			Mount(SC_MSG_GETRIDEDATA_CONTEXT::HORSEDATA horse);
	// ��
	virtual void			Dismount(void);
public:
	CHeroPart_Ride(void);
	virtual ~CHeroPart_Ride(void);
};
//------------------------------------------------------------------------