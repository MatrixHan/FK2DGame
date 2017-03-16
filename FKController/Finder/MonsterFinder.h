/**
*	created:		2012-6-24   23:11
*	filename: 		MonsterFinder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <set>
#include <Windows.h>
using std::set;
//------------------------------------------------------------------------
struct IThing;
//------------------------------------------------------------------------
typedef  __int64 UID;
//------------------------------------------------------------------------
//�����Ұ뾶
const int MAX_SEARCHRADIUS = 13;
//------------------------------------------------------------------------
class CMonsterFinder  
{
private:
	set<UID> setUIDMonster;
	//ȡ����δ��ѡ����Thing (�ڶ�������ʾ,�Ƿ�ѡ��û�й����Ĺ�)
	IThing * __GetFreeThing(POINTS ptActor, BOOL bHPMustFull, BOOL bExcludeNPC);
	//�ҵ��ĵ�һֻ�ֵ�UID
	UID m_uidFirstMonster;
public:
	CMonsterFinder();
	virtual ~CMonsterFinder();

	IThing * GetNextMonster(BOOL bHPMustFull = FALSE, BOOL bExcludeNPC = FALSE);
};
//------------------------------------------------------------------------
