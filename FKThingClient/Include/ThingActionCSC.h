/**
*	created:		2012-7-4   1:47
*	filename: 		ThingActionCSC
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��Ӧ������������CSC������Ϣ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/Event/IEvent.h"
//------------------------------------------------------------------------
class IEvent;
class IEventActionListener;
//------------------------------------------------------------------------
// MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_DESTROY)
class CThingAction_Destroy_CSC : public IEventActionListener
{
public:
	//////////////////////////////////////////////////////////
	/*************** IEventActionListener *******************/
	virtual void OnAction(IEvent * pEvent);
	
	//////////////////////////////////////////////////////////
	/************  CThingAction_Destroy_CSC *****************/
};
//------------------------------------------------------------------------