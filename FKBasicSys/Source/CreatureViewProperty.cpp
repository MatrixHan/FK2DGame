/**
*	created:		2012-7-2   2:46
*	filename: 		CreatureViewProperty
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/BasicCommon.h"
#include "../Include/CreatureView.h"
#include "../Include/ResLoader.h"
#include "../Include/SkillCreate.h"
//------------------------------------------------------------------------
// ����֧�ֵ�����
CCreatureView::ProcProperty CCreatureView::m_Property[CrtPropMaxPropCount] =
{
	NULL,
	&CCreatureView::propMoveStype,
	&CCreatureView::propTile,
	&CCreatureView::propUserName,
	&CCreatureView::propShowName,
	&CCreatureView::propNameColor,
	&CCreatureView::propAbsolutePosition,
	&CCreatureView::propWeaponStyle,
	&CCreatureView::propWeaponSoundID,
	&CCreatureView::propBeHitSoundID,
	&CCreatureView::propIsMoving,
	&CCreatureView::propState,
	&CCreatureView::propDirection,
	&CCreatureView::propMoveSpeedLevel,
	&CCreatureView::propAttackSpeedLevel,
	&CCreatureView::propGroupInfoImage,
};
//------------------------------------------------------------------------
// �ƶ�����
// ���û򷵻�: DWORD* (ActMoveStyleWalk or ActMoveStyleRun or ActMoveStyleRide)
void CCreatureView::propMoveStype(BOOL bSetProp, DWORD dwPropVal)
{
	if (bSetProp)
	{
		if(*(DWORD*)dwPropVal != ActMoveStyleWalk
			&& *(DWORD*)dwPropVal != ActMoveStyleRun
			&& *(DWORD*)dwPropVal != ActMoveStyleRide)
			return;

		if(GetMoveStyle() == *(DWORD*)dwPropVal)
			return;

		SetMoveStyle(*(DWORD*)dwPropVal);

		if(IsMoving())
		{
			cmdMove((DWORD)(m_pPathList + m_nCurNodeIndex), m_nPathNodeCount - m_nCurNodeIndex);
		}
		else
		{
			SetState(CrtStateStand);
		}
	}
	else
		*(DWORD*)dwPropVal = GetMoveStyle();
}
//------------------------------------------------------------------------
// �����TILE����(�ͻ���ֻ������·ʱ����������ԣ���Ϊ������·�����Ż���
// �������ʹ��propAbsolutePositionȻ��ScreenToTile)
// ���û򷵻�: POINT*
void CCreatureView::propTile(BOOL bSetProp, DWORD dwPropVal)
{
	if (bSetProp)
	{
		m_ptTile = *(POINT*)dwPropVal;
	}
	else
	{
		*(POINT*)dwPropVal = m_ptTile;
	}
}
//------------------------------------------------------------------------
// ���������
// ���û򷵻�: char*
void CCreatureView::propUserName(BOOL bSetProp, DWORD dwPropVal)
{
	if (bSetProp)
	{
		LPCSTR pStr = (LPCSTR)dwPropVal;
		int nLen = strlen(pStr);
		lstrcpyn(m_szUserName, (LPCSTR)dwPropVal, nLen + 1);
	}
	else
	{
		int nLen = strlen(m_szUserName);
		lstrcpyn((char*)dwPropVal, m_szUserName, nLen + 1);
	}
}
//------------------------------------------------------------------------
// �Ƿ���������ʾ�Լ�������
// ���û򷵻�: BOOL*
void CCreatureView::propShowName(BOOL bSetProp, DWORD dwPropVal)
{
	if (bSetProp)
		m_bShowUserName = *(BOOL*)dwPropVal;
	else
		*(BOOL*)dwPropVal = m_bShowUserName;
}
//------------------------------------------------------------------------
// �������ֵ���ɫ
// ���û򷵻�: WPixel*
void CCreatureView::propNameColor(BOOL bSetProp, DWORD dwPropVal)
{
	if (bSetProp)
		m_clrName = *(WPixel*)dwPropVal;
	else
		*(WPixel*)dwPropVal = m_clrName;
}
//------------------------------------------------------------------------
// ����ľ���λ��(ptTileCenter+ptOffset)
// ����: POINT*
void CCreatureView::propAbsolutePosition(BOOL bSetProp, DWORD dwPropVal)
{
	if (bSetProp)
	{
		TraceLn("CCreatureView::propAbsolutePosition.Set() is not implemented!");
	}
	else
	{
		POINT pt;
		Tile_To_Screen(m_ptTile, pt);
		pt.x += m_ptTileOffset.x;
		pt.y += m_ptTileOffset.y;
		*(POINT*)dwPropVal = pt;
	}
}
//------------------------------------------------------------------------
// �����Ƿ������ƶ�
// ����: BOOL*
void CCreatureView::propIsMoving(BOOL bSetProp, DWORD dwPropVal)
{
	if (bSetProp)
		TraceLn("CCreatureView::propIsMoving.Set() is not implemented!");
	else
		*(BOOL*)dwPropVal = IsMoving();
}
//------------------------------------------------------------------------
// �����״̬
// ����: int*
void CCreatureView::propState(BOOL bSetProp, DWORD dwPropVal)
{
	if (bSetProp)
		SetState(*(int*)dwPropVal);
	else 
		*(int*)dwPropVal = GetState();

}
//------------------------------------------------------------------------
// ����ķ���
// ���û򷵻�: int*
void CCreatureView::propDirection(BOOL bSetProp, DWORD dwPropVal)
{
	if (bSetProp) // ����ɫ�ߵ��ǵж�NPC�Ա�ʱ���ͻ��˿�������NPC�ķ�����ʹ֮��Խ�ɫ
		SetDirection(*(int*)dwPropVal);
	else 
		*(int*)dwPropVal = GetDirection();
}
//------------------------------------------------------------------------
// ������ƶ��ٶȵȼ�(0->8)
// ���û򷵻�: int*
void CCreatureView::propMoveSpeedLevel(BOOL bSetProp, DWORD dwPropVal)
{
	if (bSetProp)
	{
		ASSERT(*(BYTE*)dwPropVal > 0 && *(BYTE*)dwPropVal < MAX_DELAY_TYPE);

		if (*(BYTE*)dwPropVal != m_nMoveSpeedLevel)
		{
			m_nMoveSpeedLevel = *(BYTE*)dwPropVal;
			
			int nDelay[3];
			nDelay[0] = g_ResLoader.GetWalkDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
			nDelay[1] = g_ResLoader.GetRunDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
			nDelay[2] = g_ResLoader.GetRideDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
			SetAllMoveDelay(nDelay);
			if(IsMoving())
			{
				CalcSpeedMove();
				SetCurSpeed(GetSpeedMove(GetDirection()));
				RestartCurState();
			}
		}
	}
	else 
	{
		*(int*)dwPropVal = m_nMoveSpeedLevel;
	}
}
//------------------------------------------------------------------------
// ����Ĺ����ٶȵȼ�(0->8)
// ���û򷵻�: int*
void CCreatureView::propAttackSpeedLevel(BOOL bSetProp, DWORD dwPropVal)
{
/*
	if (bSetProp)
	{
		ASSERT(*(BYTE*)dwPropVal > 0 && *(BYTE*)dwPropVal < MAX_DELAY_TYPE);

		int nOldSpdLvl = GetAttackSpeedLevel();
		SetAttackSpeedLevel(*(int*)dwPropVal);
		if (nOldSpdLvl != GetAttackSpeedLevel() && State() == ActStateAttack)
		{
			GetCurState()->End();
			GetCurState()->Start();
		}
	}
	else 
		*(int*)dwPropVal = GetAttackSpeedLevel();
*/
}
//------------------------------------------------------------------------