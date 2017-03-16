/**
*	created:		2012-7-2   2:30
*	filename: 		CreatureViewCommond
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/BasicCommon.h"
#include "../Include/CreatureView.h"
#include "../Include/ResLoader.h"
#include "../Include/PalMgr.h"
#include "../Include/SkillCreate.h"
#include "../../FKGame/MapInfo/Tile.h"
//------------------------------------------------------------------------
extern CMagic* g_pSkillSelMagic;
//------------------------------------------------------------------------
// ����֧�ֵ�����
CCreatureView::ProcCommand CCreatureView::m_Command[CrtCmdMaxCommandCount] =
{
	NULL,
	&CCreatureView::cmdSetProperty,
	&CCreatureView::cmdGetProperty,
	&CCreatureView::cmdMove,
	&CCreatureView::cmdBeHit,
	&CCreatureView::cmdDie,
	&CCreatureView::cmdFallow,
	&CCreatureView::cmdChangeModule,
	&CCreatureView::cmdSpecialModule,
	&CCreatureView::cmdBorn,
	&CCreatureView::cmdUpgrade,
	&CCreatureView::cmdStand,
	&CCreatureView::cmdAppearance,
	&CCreatureView::cmdBloodInfo,
	&CCreatureView::cmdChatInfo,
	&CCreatureView::cmdMouseAni,
	&CCreatureView::cmdModulePalType,
	&CCreatureView::cmdModuleAlpha,
	&CCreatureView::cmdSetting,
	&CCreatureView::cmdAddTitle,
	&CCreatureView::cmdClearTitle,

	&CCreatureView::cmdPreAttack,
	&CCreatureView::cmdStopPreAttack,
	&CCreatureView::cmdAttack,
	&CCreatureView::cmdStopAttack,
	&CCreatureView::cmdChangeLocForSkill,
	&CCreatureView::cmdSkillSelAni,

	&CCreatureView::cmdSetNpcType,
	&CCreatureView::cmdSetTaskType,
	&CCreatureView::cmdSetClanName,
	&CCreatureView::cmdSetCityName,
};
//------------------------------------------------------------------------
// ��ɫ����������
// dwParam1: ��������
// dwParam2: ����ֵ(�ο���ɫ�����Զ���)
int CCreatureView::cmdSetProperty(DWORD dwParam1, DWORD dwParam2)
{
	if (dwParam1 >= CrtPropMaxPropCount)
	{
		TraceColorLn(MY_TRACE_COLOR, "CCreatureView::cmdSetProperty(), Unknow Property ID = %d", dwParam1);
		return FALSE;
	}
	if (m_Property[dwParam1] == NULL)
		return FALSE;
	
	(this->*m_Property[dwParam1])(TRUE, dwParam2);
	return TRUE;
}
//------------------------------------------------------------------------
// ��ȡ��ɫ������
// dwParam1: ��������
// dwParam2: ����ֵ(�ο���ɫ�����Զ���)
int CCreatureView::cmdGetProperty(DWORD dwParam1, DWORD dwParam2)
{
	if (dwParam1 >= CrtPropMaxPropCount)
	{
		TraceColorLn(MY_TRACE_COLOR, "CCreatureView::cmdSetProperty(), Unknow Property ID = %d", dwParam1);
		return FALSE;
	}
	if (m_Property[dwParam1] == NULL)
		return FALSE;
	
	(this->*m_Property[dwParam1])(FALSE, dwParam2);
	return TRUE;
}
//------------------------------------------------------------------------
// ��ɫ����ָ����·���ƶ�
// dwParam1: ·���б�ָ��(POINTS*);
// dwParam2: ·���б�ڵ���Ŀ��
int CCreatureView::cmdMove(DWORD dwParam1, DWORD dwParam2)
{
	m_bNeedWait = FALSE;
	m_nWaitTime = 0;
	POINTS* pList = (POINTS*)dwParam1;


	int dwNodeCount=dwParam2;

	if (dwNodeCount < 2)
		return FALSE;
	
	// ·���б��Ƿ�ռ��㹻
	if (dwNodeCount > m_nMaxPathNodeCount)
	{
		m_nMaxPathNodeCount = dwNodeCount;
		if (m_pPathList) delete[] m_pPathList;
		m_pPathList = new POINTS[dwNodeCount];
	}

	m_nPathNodeCount = dwNodeCount;
	m_nCurNodeIndex = 0;

	memcpy(m_pPathList, (POINTS*)dwParam1, dwNodeCount*sizeof(POINTS));

	POINT ptNewTile = {m_pPathList[0].x, m_pPathList[0].y};
	if (m_ptTile.x != ptNewTile.x || m_ptTile.y != ptNewTile.y)
	{
		if (g_pMapView->MoveItem(m_ptTile, ptNewTile, this))
		{
			TraceColorLn(MY_TRACE_COLOR, "���ˡ�����");
			m_ptTile.x = m_pPathList[0].x;
			m_ptTile.y = m_pPathList[0].y;
			m_ptTileOffset.x = m_ptTileOffset.y = 0;
		}
		else
		{
		}
	}

	if (m_ptTileOffset.x != 0 || m_ptTileOffset.y != 0) // û������ͣ������û��վ��TILE�м�
	{
		POINT ptFrom, ptTo;
		POINT ptTileNext, ptTileCenter;
		ptTileNext.x = m_pPathList[m_nCurNodeIndex+1].x;
		ptTileNext.y = m_pPathList[m_nCurNodeIndex+1].y;
		int nTileDir = Fun_CalcDirection8_tile(&m_ptTile, &ptTileNext);
		{
			Tile_To_World(m_ptTile, ptTileCenter);
			ptFrom.x = ptTileCenter.x + m_ptTileOffset.x;
			ptFrom.y = ptTileCenter.y + m_ptTileOffset.y;
			Tile_To_World(ptTileNext, ptTo);
			
			int nDir = Fun_CalcDirection8_45(&ptFrom, &ptTo);
			
			float nStepX, nStepY;
			GetMoveStepEx(nDir, nStepX, nStepY);
			int dx = ptTo.x - ptFrom.x;
			int dy = ptTo.y - ptFrom.y;
			int nMaxCountX=0, nMaxCountY=0;
			int nCount = 0;
			float fStepX=0.0f, fStepY=0.0f;
			if (dx == 0)
			{
				nCount = dy / nStepY;
				fStepY = (float)nStepY;
			}
			else if (dy == 0)
			{
				nCount = dx / nStepX;
				fStepX = (float)nStepX;
			}
			else
			{
				if (nStepX != 0)
					nMaxCountX = dx / nStepX;
				if (nStepY != 0)
					nMaxCountY = dy / nStepY;
				if (nMaxCountX > nMaxCountY)
				{
					nCount = nMaxCountX;
					fStepX = nStepX;
					fStepY = fStepX * ((float)dy) / ((float)dx);
				}
				else
				{
					nCount = nMaxCountY;
					fStepY = nStepY;
					fStepX = fStepY * ((float)dx) / ((float)dy);
				}
			}

			if (nCount == 0) // ����tile�����ĵ�̫��
			{
				if (m_nMaxPointNodeCount <= 0)
				{
					m_nMaxPointNodeCount = 1;
					if (m_pPointList) delete[] m_pPointList;
					m_pPointList = new SDblPoint[1];
				}
				m_pPointList[0].ptTile = m_ptTile;
				m_pPointList[0].ptOff.x = 0;
				m_pPointList[0].ptOff.y = 0;
				
				m_nCurPointNodeIndex = 0;
				m_nPointNodeCount = 0;	
			}
			else
			{
				if (nCount > m_nMaxPointNodeCount)
				{
					m_nMaxPointNodeCount = nCount;
					if (m_pPointList) delete[] m_pPointList;
					m_pPointList = new SDblPoint[nCount];
				}
				
				POINT ptOldTile = m_ptTile;
				for (int i=0; i<nCount-1; i++)
				{
					POINT pt,ptTile,ptCenter;
					pt.x = ptFrom.x + fStepX*(i+1);
					pt.y = ptFrom.y + fStepY*(i+1);
					World_To_Tile(pt, ptTile);
					if (g_pMapView->GetTile(ptTile)->IsBlock()) // ���ռλ������ǰ���Ǹ�TILE
					{
						ptTile = ptOldTile;
					}
					ptOldTile = ptTile;
					Tile_To_World(ptTile, ptCenter);
					pt.x -= ptCenter.x;
					pt.y -= ptCenter.y;
					m_pPointList[i].ptTile = ptTile;
					m_pPointList[i].ptOff = pt;
				}
				m_pPointList[nCount-1].ptTile = ptTileNext;
				m_pPointList[nCount-1].ptOff.x = 0;
				m_pPointList[nCount-1].ptOff.y = 0;

				m_nCurPointNodeIndex = 0;
				m_nPointNodeCount = nCount;				
			}

			m_bNormalMove = FALSE;		
			SetDirection(nDir);
			m_nLastMoveDir = nDir;
		}
	}
	else
	{
		OnChangeDir();
	}

	ResetMoveInfo();
	
	SetState(GetMoveStateByStyle());
	
	return TRUE;
}
//------------------------------------------------------------------------
// ��ɫվ��
// dwParam1: ?
// dwParam2: ?
int CCreatureView::cmdStand(DWORD dwParam1, DWORD dwParam2)
{
	
	// if is moving, stop move
	if (IsMoving()) StopMove();

	POINT ptNewTile = *(POINT*)dwParam1;
	if (m_ptTile.x != ptNewTile.x || m_ptTile.y != ptNewTile.y)
	{
		TraceLn("cmdStand: ��ǰ(%d,%d),����(%d,%d)", m_ptTile.x, m_ptTile.y, ptNewTile.x, ptNewTile.y);
		if (g_pMapView->MoveItem(m_ptTile, ptNewTile, this))
			SetTile(ptNewTile, g_pMapView);

		if(GetState() >= CrtStateJuqi1 && GetState() <= CrtStateArrowAttack)
			return TRUE;
	}
	SetState(CrtStateStand);
 
	return TRUE;
}
//------------------------------------------------------------------------
// ��������
// dwParam1: BOOL(��ʶ���»��ǲ�����)
// dwParam2: ?
int	CCreatureView::cmdSetting(DWORD dwParam1, DWORD dwParam2)
{
	if (IsMoving())
		StopMove();
	
	if(dwParam1 == 1)
	{
		m_ptTileOffset.x = 0;
		m_ptTileOffset.y = 0;
		SetState(CrtStateSit);
	}
	
	return TRUE;
}
//------------------------------------------------------------------------
// ��������(��Ҫ��һЩ������)
// dwParam1: �����û���ȡ��(TRUR or FALSE)
// dwParam2: ���ID (DWORD)
int CCreatureView::cmdAppearance(DWORD dwParam1, DWORD dwParam2)
{
	if ((BOOL)dwParam1)
		return CreateAppearance(dwParam2);
	return CancelAppearance(dwParam2);
}
//------------------------------------------------------------------------
// ����ı�Ѫ��(��������������Ѫ����װ����)
// dwParam1: ���ﵱǰ���ж���Ѫ(int*)
// dwParam2: ���ﵱǰ�ܹ������ɶ���Ѫ(int*)
int CCreatureView::cmdBloodInfo(DWORD dwParam1, DWORD dwParam2)
{
	ASSERT(dwParam1 && dwParam2);
	int nCurBlood = *(int*)dwParam1;
	int nTotalBlood = *(int*)dwParam2;
	if (nTotalBlood <= 0)
		nTotalBlood = 1;
	m_nBloodVol = nCurBlood*100/nTotalBlood;
	
	return TRUE;
}
//------------------------------------------------------------------------
// ��ɫͷ����������Ϣ
// dwParam1: ��������(LPSTR)
// dwParam2: ������ɫ(WPixel*)
int	CCreatureView::cmdChatInfo(DWORD dwParam1, DWORD dwParam2)
{
	typedef IPaoPao** MyType;

	*(MyType*)dwParam1 = (MyType)&m_pPaoPao;
	return TRUE;
}
//------------------------------------------------------------------------
// ����(ת������״̬��������Ч����)
// dwParam1: ħ��(����)�Ĵ�������(SSkillCreateParam*)
// dwParam2: ?
int	CCreatureView::cmdPreAttack(DWORD dwParam1, DWORD dwParam2)
{
	// ͣ����
	if (IsMoving())
	{
		StopMove();
	}

	SetState(CrtStateStand);
	// ��ȡ���ܲ���
	SSkillCreateParam* pParam = (SSkillCreateParam*)dwParam1;
	pParam->nAniType = anitypeSkillAni;
	pParam->ptTile = m_ptTile;
	if(pParam->pAttackView != NULL)
		pParam->ptAttack = pParam->pAttackView->GetTile();

	// ����У��
	if (pParam->nIndex < 1 || pParam->nIndex > SKILL_MAX_COUNT)
	{
		TraceLn("ERROR: CCreatureView::cmdPreAttack(): invalid skill, ID = %d", pParam->nIndex);
		return FALSE;
	}

	// ���ű�
	IResObject* pScpSkill = (IResObject*)CSkillCreate::getSkillLevelScpObj(pParam->nAniType, pParam->nIndex, pParam->nLevel);
	if (!pScpSkill) 
	{
		return TRUE;
	}
	long lTemp;

	// У������
	pScpSkill->GetPropertyLong("Base_NotAdjustDir", lTemp, 0);
	if(lTemp == 0)
	{
		POINT ptTileCenter,ptAttackCenter;
		Tile_To_Screen(m_ptTile, ptTileCenter);
		ptTileCenter.x += m_ptTileOffset.x;
		ptTileCenter.y += m_ptTileOffset.y;

		Tile_To_Screen(pParam->ptAttack, ptAttackCenter);
		if(pParam->pAttackView != NULL)
		{
			ptAttackCenter.x += pParam->pAttackView->m_ptTileOffset.x;
			ptAttackCenter.y += pParam->pAttackView->m_ptTileOffset.y;
		}

		if (ptTileCenter.x != ptAttackCenter.x || ptTileCenter.y != ptAttackCenter.y)
			SetDirection(Fun_CalcDirection8_45(&ptTileCenter, &ptAttackCenter));
	}

	// ����������
	pScpSkill->GetPropertyLong("Base_ActIDPreAttackUseType", lTemp, 0);
	if(lTemp == 0)		// ���creature�ű�����������������������Ǹ��������ü��ܽű���
	{
		lTemp = GetJuqiActID();
		if(lTemp < 0)
		{
			pScpSkill->GetPropertyLong("Base_ActIDPreAttack", lTemp, 0);
		}
	}
	else				// ���skill�ű����������������������������creature�ű���
	{
		pScpSkill->GetPropertyLong("Base_ActIDPreAttack", lTemp, -1);
		if(lTemp <= 0)
		{
			lTemp = GetJuqiActID();
		}
	}

	SetState(lTemp);
	SetStatusPreAttack();

	// �����׶ε�����
	pScpSkill->GetPropertyLong("Base_SoundIDPreAttack", lTemp, 0);
	if(lTemp > 0)
	{
		long lSoundLoop;
		pScpSkill->GetPropertyLong("Base_bSoundLoopPreAttack", lSoundLoop, 0);
		m_nAttackSoundChunnelID = Fun_PlaySound(m_ptTile, lTemp,lSoundLoop);
	}

	// �����׶εļ��ܹ�Ч
	DWORD dwMagicID[] = {0,0,0};
	int nLen = pScpSkill->GetArrayLong("Base_MagicIDPreAttack");
	if (nLen != pScpSkill->GetArrayLong("Base_MagicIDPreAttack", (long*)dwMagicID, nLen))
		return FALSE;

	int nTime = 0;
	while(dwMagicID[nTime] > 0 && nTime <= 3)
	{
		SAttachStateObjBuffer st;
		memset(&st, 0, sizeof(st));
		st.dwResID = dwMagicID[nTime];

		IResObject* pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(st.dwResID);
		if (pMagicObj)
		{
			g_ResLoader.ReadAttachMagic(pMagicObj, &st);
			
			pMagicObj->GetPropertyLong("LoopNum", (long &)st.m_nLoopNum, 1);
			pMagicObj->GetPropertyLong("NoShowBegin", (long &)st.m_nNoShowBegin, -1);
			pMagicObj->GetPropertyLong("NoShowEnd", (long &)st.m_nNoShowEnd, -1);

			st.dwAppearanceID = -1;
			
			CAttachStateObj* pObj = CreateAttachObj(&st);
			if(pObj != NULL)
				m_vAttackAttachObjs.push_back(pObj);
		}
		nTime++;
	}

	return TRUE;
}
//------------------------------------------------------------------------
// ����(ת������״̬������˲��ʩ��������ʩ������ͨ������ʩ����Ч������������������)
// dwParam1: ħ��(����)�Ĵ�������(SSkillCreateParam*)
// dwParam2: ?
int	CCreatureView::cmdAttack(DWORD dwParam1, DWORD dwParam2)
{
	if (IsMoving())
		StopMove();
	
	SetState(CrtStateStand);
	// ȡ����ǰ�ļ���ħ��
	StopMagic();

	SSkillCreateParam* pParam = (SSkillCreateParam*)dwParam1;
	pParam->nAniType = anitypeSkillAni;
	pParam->ptTile = m_ptTile;
	if(pParam->pAttackView != NULL)
		pParam->ptAttack = pParam->pAttackView->GetTile();
	
	if (pParam->nIndex < 0 || pParam->nIndex > SKILL_MAX_COUNT || m_funcSkill[pParam->nIndex] == NULL)
	{
		TraceLn("ERROR: CCreatureView::cmdAttack(): invalid skill, ID = %d", pParam->nIndex);
		return FALSE;
	}

	// ��ͨ����У��
	if(pParam->nIndex == 0)
		pParam->nLevel = 1;

	// ���ű�
	IResObject* pScpSkill = (IResObject*)CSkillCreate::getSkillLevelScpObj(pParam->nAniType, pParam->nIndex, pParam->nLevel);
	if (!pScpSkill) 
	{
		return TRUE;
	}
	long lTemp;

	// У������
	pScpSkill->GetPropertyLong("Base_NotAdjustDir", lTemp, 0);
	if(lTemp == 0)
	{
		POINT ptTileCenter,ptAttackCenter;
		Tile_To_Screen(m_ptTile, ptTileCenter);
		ptTileCenter.x += m_ptTileOffset.x;
		ptTileCenter.y += m_ptTileOffset.y;

		Tile_To_Screen(pParam->ptAttack, ptAttackCenter);
		if(pParam->pAttackView != NULL)
		{
			ptAttackCenter.x += pParam->pAttackView->m_ptTileOffset.x;
			ptAttackCenter.y += pParam->pAttackView->m_ptTileOffset.y;
		}

		if (ptTileCenter.x != ptAttackCenter.x || ptTileCenter.y != ptAttackCenter.y)
			SetDirection(Fun_CalcDirection8_45(&ptTileCenter, &ptAttackCenter));
	}

	// ���ܴ���
	if(!(this->*m_funcSkill[pParam->nIndex])(pParam))
	{
		TraceLn("ERROR: CFashiView::cmdUseMagic(): skill create failed, ID = %d", pParam->nIndex);
		SetState(CrtStateStand);
		return FALSE;
	}

	// ����������
	if(pParam->nIndex == 0)	// ��ͨ�����޹�Ч����ָ����������
	{
		lTemp = GetNormalAttackActID();
		if (lTemp < 0)
			return TRUE;

		SetState(lTemp);
		SetStatusMagicCheckID(pParam->nCheckId);
		SetStatusAttackOnce();
		SetStatusPlayCommondAttackSound();
		return TRUE;
	}
	
	int nLen;

	// ����������
	pScpSkill->GetPropertyLong("Base_ActIDAttackUseType", lTemp, 0);
	if(lTemp == 0)		// ���creature�ű���������Ĺ������������Ǹ��������ü��ܽű���
	{
		lTemp = GetSkillAttackActID();
		if(lTemp < 0)
		{
			DWORD dwActAttackID[] = {0,0};
			
			nLen = pScpSkill->GetArrayLong("Base_ActIDAttack");
			if(nLen < 1 || nLen > 2)
				return FALSE;

			if (nLen != pScpSkill->GetArrayLong("Base_ActIDAttack", (long*)dwActAttackID, nLen))
				return FALSE;

			if(nLen == 2 && GetWeaponStyle() == ActWeaponStyleDouble)
			{
				lTemp = dwActAttackID[1];
			}
			else
			{
				lTemp = dwActAttackID[0];
			}
		}
	}
	else				// ���skill�ű����˹����������������������creature�ű���
	{
		DWORD dwActAttackID[] = {0,0};
		
		nLen = pScpSkill->GetArrayLong("Base_ActIDAttack");
		if(nLen < 1 || nLen > 2
			|| nLen != pScpSkill->GetArrayLong("Base_ActIDAttack", (long*)dwActAttackID, nLen))
		{
			lTemp = GetSkillAttackActID();
		}
		else
		{
			if(nLen == 2 && GetWeaponStyle() == ActWeaponStyleDouble)
			{
				lTemp = dwActAttackID[1];
			}
			else
			{
				lTemp = dwActAttackID[0];
			}
		}
	}
	
	SetState(lTemp);
	SetStatusMagicCheckID(pParam->nCheckId);

	pScpSkill->GetPropertyLong("Base_AttackOnceOrCtrl", lTemp, 0);
	if(lTemp == 0)
	{
		SetStatusAttackOnce();
	}
	else
	{
		SetStatusAttackCtrl();
	}

	// �����׶ε�����
	pScpSkill->GetPropertyLong("Base_SoundIDAttack", lTemp, 0);
	if(lTemp > 0)
	{
		long lSoundLoop;
		pScpSkill->GetPropertyLong("Base_bSoundLoopeAttack", lSoundLoop, 0);
		m_nAttackSoundChunnelID = Fun_PlaySound(m_ptTile, lTemp,lSoundLoop);
	}

	// �����׶εļ��ܹ�Ч
	DWORD dwMagicID[] = {0,0,0};
	nLen = pScpSkill->GetArrayLong("Base_MagicIDAttack");
	if (nLen != pScpSkill->GetArrayLong("Base_MagicIDAttack", (long*)dwMagicID, nLen))
		return FALSE;

	int nTime = 0;
	while(dwMagicID[nTime] > 0 && nTime <= 3)
	{
		SAttachStateObjBuffer st;
		memset(&st, 0, sizeof(st));
		st.dwResID = dwMagicID[nTime];

		IResObject* pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(st.dwResID);
		if (pMagicObj)
		{
			g_ResLoader.ReadAttachMagic(pMagicObj, &st);
			
			pMagicObj->GetPropertyLong("LoopNum", (long &)st.m_nLoopNum, 1);
			pMagicObj->GetPropertyLong("NoShowBegin", (long &)st.m_nNoShowBegin, -1);
			pMagicObj->GetPropertyLong("NoShowEnd", (long &)st.m_nNoShowEnd, -1);

			st.dwAppearanceID = -1;
			
			CAttachStateObj* pObj = CreateAttachObj(&st);
			if(pObj != NULL)
				m_vAttackAttachObjs.push_back(pObj);
		}
		nTime++;
	}

	return TRUE;
	
}
//------------------------------------------------------------------------
// ֹͣ����(ת��վ��״̬��������Чֹͣ)
// dwParam1: ?
// dwParam2: ?
int	CCreatureView::cmdStopPreAttack(DWORD dwParam1, DWORD dwParam2)
{
	SetState(CrtStateStand);
	return TRUE;
}
//------------------------------------------------------------------------
// ֹͣ������ֻ�Գ���ʩ����Ч��ת��վ��״̬��ʩ����Чֹͣ����������ֹͣ��
// dwParam1: ?
// dwParam2: ?
int	CCreatureView::cmdStopAttack(DWORD dwParam1, DWORD dwParam2)
{
	SetState(CrtStateStand);
	return TRUE;
}
//------------------------------------------------------------------------
// ֹͣ������ֻ�Գ���ʩ����Ч��ת��վ��״̬��ʩ����Чֹͣ����������ֹͣ��
// dwParam1: ?
// dwParam2: ?
int	CCreatureView::cmdChangeLocForSkill(DWORD dwParam1, DWORD dwParam2)
{
	skill_ChangeLoc(*(POINT*)dwParam1);
	return TRUE;
}
//------------------------------------------------------------------------
// ��Χ���ܵ�ѡ���Ч
// dwParam1: BOOL(��ʶ���û���ȡ��)
// dwParam2: ����ID
int	CCreatureView::cmdSkillSelAni(DWORD dwParam1, DWORD dwParam2)
{
	if(dwParam1 == 1)
	{
		g_pSkillSelMagic = CreateSkillSelMagic(dwParam2);
	}
	else
	{
		SAFE_RELEASE(g_pSkillSelMagic);
	}
	return TRUE;
}
//------------------------------------------------------------------------
// ��������
// dwParam1: 0��ʾ�����������ж�����1��ʾ�Ѿ��������޶���
// dwParam2: ?
int CCreatureView::cmdDie(DWORD dwParam1, DWORD dwParam2)
{
	m_bDeaded = TRUE;
	
	if (IsMoving())
		StopMove();

	g_pMapView->RemoveItem(m_ptTile, this);
	SetState(CrtStateDie);
	g_pMapView->AddItem(m_ptTile, this);

	if (dwParam1 == 1)
	{
		m_AniContext.nCurFrame = - 1;
	}

	return TRUE;
}
//------------------------------------------------------------------------
// ���ﱻ��
// dwParam1: ���ﱻ��ʱ�������ز���(SBehitParam*)
// dwParam2: ?
int CCreatureView::cmdBeHit(DWORD dwParam1, DWORD dwParam2)
{
	SBehitParam* pParam = (SBehitParam*)dwParam1;

	// ����������
	if(pParam->bDoAct)
	{
		if(IsMoving())
			StopMove();
		SetState(CrtStateBehit);
	}

	// ͷ��ƮѪѹ���˺���ջ
	AddDmgStack(pParam);
	return TRUE;
}
//------------------------------------------------------------------------
// ������������������
// dwParam1: LPCSTR(��������)
// dwParam2: ?
int CCreatureView::cmdSetClanName(DWORD dwParam1, DWORD dwParam2)
{
	LPCSTR pStr = (LPCSTR)dwParam1;
	int nLen = strlen(pStr);
	if(nLen > 0)
	{
		m_bShowClanName = TRUE;
		char szTemp[CREATURE_MAX_NAME];
		lstrcpyn(szTemp, (LPCSTR)dwParam1, nLen + 1);
		sprintf(m_szClanName, "[%s] ", szTemp);
	}
	else
	{
		m_bShowClanName = FALSE;
	}

	return TRUE;
}
//------------------------------------------------------------------------
// ���������óǳ�����
// dwParam1: LPCSTR(�ǳ�����)
// dwParam2: ?
int CCreatureView::cmdSetCityName(DWORD dwParam1, DWORD dwParam2)
{
	LPCSTR pStr = (LPCSTR)dwParam1;
	int nLen = strlen(pStr);
	if(nLen > 0)
	{
		m_bShowCityName = TRUE;
		char szTemp[CREATURE_MAX_NAME];
		lstrcpyn(szTemp, (LPCSTR)dwParam1, nLen + 1);
		sprintf(m_szCityName, "��%s��", szTemp);
	}
	else
	{
		m_bShowCityName = FALSE;
	}

	return TRUE;
}
//------------------------------------------------------------------------