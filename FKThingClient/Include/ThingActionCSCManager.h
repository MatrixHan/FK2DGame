/**
*	created:		2012-7-4   1:50
*	filename: 		ThingActionCSCManager
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		CSC�߼����������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../Include/ThingActionCSC.h"
//------------------------------------------------------------------------
class CThingActionCSCManager
{
public:
	// ����
	BOOL								Build(void);

private:
	CThingAction_Destroy_CSC			m_ThingAction_Destroy_CSC;
};
//------------------------------------------------------------------------
extern CThingActionCSCManager			g_ThingActionCSCManager;
//------------------------------------------------------------------------