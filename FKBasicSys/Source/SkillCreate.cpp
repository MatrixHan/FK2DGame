/**
*	created:		2012-7-2   1:56
*	filename: 		SkillCreate
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/BasicCommon.h"
#include "../Include/SkillCreate.h"
#include "../Include/ResLoader.h"
#include "../Include/Magic.h"
#include "../Include/CreatureView.h"
#include "../Include/MagicMgrCirculDir.h"
#include "../Include/MagicMgrSingleDir.h"
#include "../Include/MagicMgrMultiDir.h"
#include "../Include/MagicMgrWanJian.h"
#include "../Include/MagicMgrGeneral.h"
#include "../Include/MagicMgrMultiUnit.h"
#include "../Include/MagicMgrOnePart.h"
#include "../Include/MagicMgrCommonAttack.h"
#include "../Include/MagicMgrChangeLoc.h"
#include "../Include/MagicMgrMultiUnitSingleTile.h"
#include "../Include/MagicMgrShadow.h"
//------------------------------------------------------------------------
CSkillCreate::CSkillCreate()
{

}
//------------------------------------------------------------------------
CSkillCreate::~CSkillCreate()
{

}
//------------------------------------------------------------------------
void* CSkillCreate::getSkillScpObj(int nAniType, int nIndex)
{
	IResObject* pScpSkill = g_ResLoader.m_pScpSkill->
		GetResObjectRoot()->
		GetSubObjectByIndex(nAniType)->
		GetSubObjectByIndex(nIndex);
	return pScpSkill;
}
//------------------------------------------------------------------------
void* CSkillCreate::getSkillLevelScpObj(int nAniType, int nIndex, int nLevel)
{	
	IResObject* pScpSkill = g_ResLoader.m_pScpSkill->
		GetResObjectRoot()->
		GetSubObjectByIndex(nAniType)->
		GetSubObjectByIndex(nIndex);
	if(pScpSkill == NULL)
		return NULL;

	IResObject* pScpSkillLv = pScpSkill->GetSubObjectByIndex(nLevel - 1);

	return pScpSkillLv;
}
//------------------------------------------------------------------------
// ����ͨ�ü���
BOOL CSkillCreate::CreateGenericSkill(SSkillCreateParam* pParam, CCreatureView *pMaster)
{
	// ��ȡ�ű�
	IResObject* pScpSkill = (IResObject *)getSkillLevelScpObj(pParam->nAniType, pParam->nIndex, pParam->nLevel);
	if (!pScpSkill)
		return FALSE;

	// ��ȡħ������ID
	DWORD dwMagicID[2];
	pScpSkill->GetPropertyLong("Ex_MagicIDRun"		,(long&)dwMagicID[0], 0);
	pScpSkill->GetPropertyLong("Ex_MagicIDExplode"	,(long&)dwMagicID[1], 0);

	// ��������̬��ħ������������ע���������̬�Ĳ����������ħ����
	IResObject* pMagicObj = NULL;
	SMagicArchive_General stRun;
	if (dwMagicID[0])
	{
		pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(dwMagicID[0]);
		if (!pMagicObj)
			return FALSE;

		memset(&stRun, 0, sizeof(stRun));
		g_ResLoader.ReadGeneralMagic(pMagicObj, &stRun);
		stRun.dwResID = pMagicObj->GetID();
	}

	// ���뱬ը̬��ħ����������
	SMagicArchive_General stExplore;
	if (dwMagicID[1])
	{
		pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(dwMagicID[1]);
		if (!pMagicObj) return FALSE;
		memset(&stExplore, 0, sizeof(stExplore));
		g_ResLoader.ReadGeneralMagic(pMagicObj, &stExplore);
		stExplore.dwResID = pMagicObj->GetID();
	}

	// ���촴������
	CMagicMgrGeneral::SMagicMgrArchive_General st;
	st.ptTile = pParam->ptTile;
	st.ptAttackTile = pParam->ptAttack;
	st.bNeedAttackDelay = TRUE;
	st.pRun		= (dwMagicID[0] > 0) ? &stRun		: NULL;
	st.pExplode = (dwMagicID[1] > 0) ? &stExplore	: NULL;
	pScpSkill->GetPropertyLong("Ex_nLoopTime", (long&)st.nLoopTime, 0);
	pScpSkill->GetPropertyLong("Ex_nCanCtrl", (long&)st.bCanCtrl, 0);
	pScpSkill->GetPropertyLong("Ex_IsOnceAttack", (long&)st.bOnceAttack, 0);

	// ����ħ��������
	CMagicMgrGeneral* pMagicMgr = new CMagicMgrGeneral;
	// ���ӱ�����Ч
	DWORD dwMagicBeHitID;
	pScpSkill->GetPropertyLong("Base_MagicIDBeHit", (long&)dwMagicBeHitID, 0);
	pMagicMgr->SetMagicIDBeHit(dwMagicBeHitID);
	pMagicMgr->SetMasterView(pMaster);
	pMagicMgr->SetAttackView(pParam->pAttackView);
	pMagicMgr->SetCheckID(pParam->nCheckId);
	if (!pMagicMgr->Create(&st, sizeof(st)))
	{
		delete pMagicMgr;
		return FALSE;
	}

	// ��ħ���������������
	pMaster->AddMagic(pMagicMgr);
	return TRUE;
}
//------------------------------------------------------------------------
// ��ͨ�����Ĵ�������
BOOL CSkillCreate::CreateCommonAttackSkill(SSkillCreateParam* pParam, CCreatureView *pMaster)
{
	// ����ħ��������
	CMagicMgrCommonAttack* pMagicMgr = new CMagicMgrCommonAttack;
	pMagicMgr->SetMasterView(pMaster);
	pMagicMgr->SetCheckID(pParam->nCheckId);
	if (!pMagicMgr->Create(NULL, 0))
	{
		delete pMagicMgr;
		return FALSE;
	}

	// ��ħ���������������
	pMaster->AddMagic(pMagicMgr);
	return TRUE;
}
//------------------------------------------------------------------------
// ����Զ�̹������ͼ��ܵĴ�������(����򡢱�ʸ��)
BOOL CSkillCreate::CreateSingleDirSkill(SSkillCreateParam* pParam, CCreatureView *pMaster)
{
	// ��ȡ�ű�
	IResObject* pScpSkill = (IResObject*)getSkillLevelScpObj(pParam->nAniType, pParam->nIndex, pParam->nLevel);
	if (!pScpSkill)
		return FALSE;

	// ��ȡħ������ID
	DWORD dwMagicID[2];
	pScpSkill->GetPropertyLong("Ex_MagicIDRun", (long&)dwMagicID[0], 0);
	pScpSkill->GetPropertyLong("Ex_MagicIDExplode", (long&)dwMagicID[1], 0);

	// �����ƶ��׶ε�ħ������
	IResObject* pMagicObj = NULL;
	SMagicArchive_SingleDir st1;
	if(dwMagicID[0] == 0)
		return FALSE;

	pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(dwMagicID[0]);
	if (!pMagicObj)
		return FALSE;

	memset(&st1, 0, sizeof(st1));
	g_ResLoader.ReadSingleDirMagic(pMagicObj, &st1);
	st1.dwResID = pMagicObj->GetID();
	st1.ptStartTile = pParam->ptTile;
	st1.pAttackView = pParam->pAttackView;
	if(st1.pAttackView != NULL)
		st1.ptAttackTile = st1.pAttackView->GetTile();
	else
		st1.ptAttackTile = pParam->ptAttack;
	st1.ptTileOffset = pMaster->m_ptTileOffset;
	pScpSkill->GetPropertyLong("Ex_FlyTileTime", (long&)(st1.nTimeTile), 60);

	// ���뱬ը�׶ε�ħ������
	SMagicArchive_General st2;
	if (dwMagicID[1])
	{
		pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(dwMagicID[1]);
		if (!pMagicObj)
			return FALSE;

		memset(&st2, 0, sizeof(st2));
		g_ResLoader.ReadGeneralMagic(pMagicObj, &st2);
		st2.dwResID = pMagicObj->GetID();
	}

	// ���촴������
	CMagicMgrSingleDir::SMagicMgrArchive_SingleDir st;
	st.pRun = (dwMagicID[0]) ? &st1 : NULL;
	st.pExplode = (dwMagicID[1]) ? &st2 : NULL;

	// ����ħ��������
	CMagicMgrSingleDir* pMagicMgr = new CMagicMgrSingleDir;
	pMagicMgr->SetMasterView(pMaster);
	pMagicMgr->SetCheckID(pParam->nCheckId);
	if (!pMagicMgr->Create(&st, sizeof(st)))
	{
		delete pMagicMgr;
		return FALSE;
	}

	// ��ħ���������������
	pMaster->AddMagic(pMagicMgr);

	return TRUE;
}
//------------------------------------------------------------------------
// ����Ķ൥Ԫ���ͼ��ܵĴ������̣����һ����Ƕ෽��ģ�����Ŀ�ĵ���һ������
BOOL CSkillCreate::CreateMultiDirSkillEx(SSkillCreateParam* pParam, CCreatureView *pMaster)
{
	// ��ȡ�ű�
	IResObject* pScpSkill = (IResObject*)getSkillLevelScpObj(pParam->nAniType, pParam->nIndex, pParam->nLevel);
	if (!pScpSkill)
		return FALSE;

	// ��ȡħ������ID
	DWORD dwMagicID[2];
	pScpSkill->GetPropertyLong("Ex_MagicIDRun"		,(long&)dwMagicID[0], 0);
	pScpSkill->GetPropertyLong("Ex_MagicIDExplode"	,(long&)dwMagicID[1], 0);

	// �����ƶ��׶ε�ħ������
	IResObject* pMagicObj = NULL;
	SMagicArchive_SingleDir st1;
	if(dwMagicID[0] == 0)
		return FALSE;

	pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(dwMagicID[0]);
	if (!pMagicObj)
		return FALSE;

	memset(&st1, 0, sizeof(st1));
	g_ResLoader.ReadSingleDirMagic(pMagicObj, &st1);
	st1.dwResID = pMagicObj->GetID();
	st1.ptStartTile = pParam->ptTile;
	st1.pAttackView = pParam->pAttackView;
	if(st1.pAttackView != NULL)
		st1.ptAttackTile = st1.pAttackView->GetTile();
	else
		st1.ptAttackTile = pParam->ptAttack;
	st1.ptTileOffset = pMaster->m_ptTileOffset;
	pScpSkill->GetPropertyLong("Ex_FlyTileTime", (long&)(st1.nTimeTile), 60);

	// ���뱬ը�׶ε�ħ������
	SMagicArchive_General st2;
	if (dwMagicID[1])
	{
		pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(dwMagicID[1]);
		if (!pMagicObj)
			return FALSE;

		memset(&st2, 0, sizeof(st2));
		g_ResLoader.ReadGeneralMagic(pMagicObj, &st2);
		st2.dwResID = pMagicObj->GetID();
	}

	// ���촴������
	CMagicMgrMultiDirEx::SMagicMgrArchive_MultiDirEx st;
	st.ptAttackTile = pParam->ptAttack;
	pScpSkill->GetPropertyLong("Ex_Width", (long&)st.nWidth, 0);
	pScpSkill->GetPropertyLong("Ex_Delay", (long&)st.nDelay, 0);
	st.pRun	= (dwMagicID[0]) ? &st1 : NULL;
	st.pExplode = (dwMagicID[1]) ? &st2 : NULL;

	// ����ħ��������
	CMagicMgrMultiDirEx* pMagicMgr = new CMagicMgrMultiDirEx;
	DWORD dwMagicBeHitID;
	pScpSkill->GetPropertyLong("Base_MagicIDBeHit", (long&)dwMagicBeHitID, 0);
	pMagicMgr->SetMagicIDBeHit(dwMagicBeHitID);

	// ��ħ���������������
	pMagicMgr->SetMasterView(pMaster);
	pMagicMgr->SetCheckID(pParam->nCheckId);
	if (!pMagicMgr->Create(&st, sizeof(st)))
	{
		delete pMagicMgr;
		return FALSE;
	}

	// ���ӱ�����Ч
	pMaster->AddMagic(pMagicMgr);
	return TRUE;
}
//------------------------------------------------------------------------
// �൥Ԫ�����ͼ��ܵĴ�������(�������ꡢ���籩�����׵�)
BOOL CSkillCreate::CreateRainSkill(SSkillCreateParam* pParam, CCreatureView *pMaster)
{
	// ��ȡ�ű�
	IResObject* pScpSkill = (IResObject*)getSkillLevelScpObj(pParam->nAniType, pParam->nIndex, pParam->nLevel);
	if (!pScpSkill)
		return FALSE;

	// ��ȡħ������ID
	DWORD dwMagicID;
	pScpSkill->GetPropertyLong("Ex_MagicIDRun", (long&)dwMagicID, 0);	

	if(dwMagicID == 0)
		return FALSE;

	// �������е�Ԫħ��������
	IResObject* pMagicObj;
	SMagicArchive_Rain st1;
	pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(dwMagicID);
	if (!pMagicObj)
		return FALSE;

	memset(&st1, 0, sizeof(st1));
	g_ResLoader.ReadRainMagic(pMagicObj, &st1);
	st1.bNeedRandomOffset =	TRUE;
	st1.dwResID = pMagicObj->GetID();

	// ���촴������
	CMagicMgrMultiUnit::SMagicMgrArchive_MultiUnit st;
	st.ptAttackTile = pParam->ptAttack;
	pScpSkill->GetPropertyLong("Ex_Width", (long&)st.nWidth, 0);
	pScpSkill->GetPropertyLong("Ex_Delay", (long&)st.nDelay, 0);
	pScpSkill->GetPropertyLong("Ex_nLoopTime", (long&)st.nLoopTime, -1);
	st.pUnit = &st1;

	// ����ħ��������
	CMagicMgrMultiUnit* pMagicMgr = new CMagicMgrMultiUnit;
	// ���ӱ�����Ч
	DWORD dwMagicBeHitID;
	pScpSkill->GetPropertyLong("Base_MagicIDBeHit", (long&)dwMagicBeHitID, 0);
	pMagicMgr->SetMagicIDBeHit(dwMagicBeHitID);
	pMagicMgr->SetMasterView(pMaster);
	pMagicMgr->SetCheckID(pParam->nCheckId);
	if (!pMagicMgr->Create(&st, sizeof(st)))
	{
		delete pMagicMgr;
		return FALSE;
	}

	// ��ħ���������������
	pMaster->AddMagic(pMagicMgr);

	return TRUE;
}
//------------------------------------------------------------------------
// �򽣳����༼�ܵĴ�������
BOOL CSkillCreate::CreateWanJianSkill(SSkillCreateParam* pParam, CCreatureView *pMaster)
{
	// ��ȡ�ű�
	IResObject* pScpSkill = (IResObject*)getSkillLevelScpObj(pParam->nAniType, pParam->nIndex, pParam->nLevel);
	if (!pScpSkill)
		return FALSE;

	// ���촴������
	CMagicMgrWanJian::SMagicMgrArchive_WanJian st;
	st.ptTile = pParam->ptTile;
	pScpSkill->GetPropertyLong("Ex_Delay", (long&)st.nDelay, 0);
	pScpSkill->GetPropertyLong("Ex_nLoopTime", (long&)st.nLoopTime, -1);

	// ����ħ��������
	CMagicMgrWanJian* pMagicMgr = new CMagicMgrWanJian;
	// ���ӱ�����Ч
	DWORD dwMagicBeHitID;
	pScpSkill->GetPropertyLong("Base_MagicIDBeHit", (long&)dwMagicBeHitID, 0);
	pMagicMgr->SetMagicIDBeHit(dwMagicBeHitID);
	pMagicMgr->SetMasterView(pMaster);
	pMagicMgr->SetCheckID(pParam->nCheckId);
	if (!pMagicMgr->Create(&st, sizeof(st)))
	{
		delete pMagicMgr;
		return FALSE;
	}

	// ��ħ���������������
	pMaster->AddMagic(pMagicMgr);

	return TRUE;
}
//------------------------------------------------------------------------
// ��ͬ��Ŀ��Ķ����Ԫħ�������������ȣ�
BOOL CSkillCreate::CreateMultiUnitSingleTileSkill(SSkillCreateParam* pParam, CCreatureView *pMaster)
{
	// ��ȡ�ű�
	IResObject* pScpSkill = (IResObject*)getSkillLevelScpObj(pParam->nAniType, pParam->nIndex, pParam->nLevel);
	if (!pScpSkill)
		return FALSE;

	// ��ȡħ������ID
	DWORD dwMagicID;
	pScpSkill->GetPropertyLong("Ex_MagicIDRun", (long&)dwMagicID, 0);	

	if(dwMagicID == 0)
		return FALSE;

	// �������е�Ԫħ��������
	IResObject* pMagicObj;
	SMagicArchive_Rain st1;
	pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(dwMagicID);
	if (!pMagicObj)
		return FALSE;

	memset(&st1, 0, sizeof(st1));
	g_ResLoader.ReadRainMagic(pMagicObj, &st1);
	st1.bNeedRandomOffset =	FALSE;
	st1.dwResID = pMagicObj->GetID();

	// ���촴������
	CMagicMgrMultiUnitSingleTile::SMagicMgrArchive_MultiUnitSingleTile st;
	st.ptAttackTile = pParam->ptAttack;
	pScpSkill->GetPropertyLong("Ex_Delay", (long&)st.nDelay, 0);
	pScpSkill->GetPropertyLong("Ex_Time",	(long&)st.nTime,  0);
	st.pUnit = &st1;

	// ����ħ��������
	CMagicMgrMultiUnitSingleTile* pMagicMgr = new CMagicMgrMultiUnitSingleTile;

	// ���ӱ�����Ч
	DWORD dwMagicBeHitID;
	pScpSkill->GetPropertyLong("Base_MagicIDBeHit", (long&)dwMagicBeHitID, 0);
	pMagicMgr->SetMagicIDBeHit(dwMagicBeHitID);
	// ��ħ���������������
	pMagicMgr->SetMasterView(pMaster);
	pMagicMgr->SetAttackView(pParam->pAttackView);
	pMagicMgr->SetCheckID(pParam->nCheckId);
	if (!pMagicMgr->Create(&st, sizeof(st)))
	{
		delete pMagicMgr;
		return FALSE;
	}

	pMaster->AddMagic(pMagicMgr);

	return TRUE;
}
//------------------------------------------------------------------------
// 8��������μ��ܵĴ�������
BOOL CSkillCreate::CreateCirculDirSkill(SSkillCreateParam* pParam, CCreatureView *pMaster, int nDir)
{
	// ��ȡ�ű�
	IResObject* pScpSkill = (IResObject*)getSkillLevelScpObj(pParam->nAniType, pParam->nIndex, pParam->nLevel);
	if (!pScpSkill)
		return FALSE;

	// ��ȡħ������ID
	DWORD dwMagicID;
	pScpSkill->GetPropertyLong("Ex_MagicIDRun", (long&)dwMagicID, 0);

	if(dwMagicID == 0)
		return FALSE;

	// �����ƶ��׶ε�ħ������
	SMagicArchive_EightDir st1;
	IResObject *pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(dwMagicID);
	if (!pMagicObj)
		return FALSE;
	memset(&st1, 0, sizeof(st1));
	g_ResLoader.ReadEightDirMagic(pMagicObj, &st1);
	st1.dwResID = pMagicObj->GetID();
	st1.ptTile = pParam->ptTile;

	// ���촴������
	CMagicMgrCirculDir::SMagicMgrArchive_CirculDir st;
	st.ptTile		= pParam->ptTile;
	st.pRun			= &st1;
	st.nDirNum		= nDir;
	pScpSkill->GetPropertyLong("Ex_bRunMagicRealTime", (long&)st.bRealTime, 0);

	// ����ħ��������
	CMagicMgrCirculDir* pMagicMgr = new CMagicMgrCirculDir;
	// ���ӱ�����Ч
	DWORD dwMagicBeHitID;
	pScpSkill->GetPropertyLong("Base_MagicIDBeHit", (long&)dwMagicBeHitID, 0);
	pMagicMgr->SetMagicIDBeHit(dwMagicBeHitID);
	pMagicMgr->SetMasterView(pMaster);
	pMagicMgr->SetAttackView(pParam->pAttackView);
	pMagicMgr->SetCheckID(pParam->nCheckId);
	if (!pMagicMgr->Create(&st, sizeof(st)))
	{
		delete pMagicMgr;
		return FALSE;
	}

	// ��ħ���������������
	pMaster->AddMagic(pMagicMgr);
	return TRUE;
}
//------------------------------------------------------------------------
// ֻ���Ű˸�������ĳ������ļ��ܵĴ�������
BOOL CSkillCreate::CreateChangeLoc(POINT pt, CCreatureView *pMaster)
{
	// ����ħ��������
	CMagicMgrChangeLoc* pMagicMgr = new CMagicMgrChangeLoc;
	if (!pMagicMgr->Create(pt, pMaster))
	{
		delete pMagicMgr;
		return FALSE;
	}

	return TRUE;
}
//------------------------------------------------------------------------
// ��ӰЧ��
BOOL CSkillCreate::CreateShadow(CCreatureView *pMaster)
{
	if(!pMaster)
		return FALSE;

	SAFE_DELETE(pMaster->m_pMagicShadow);

	// ����ħ��������
	pMaster->m_pMagicShadow = new CMagicMgrShadow;
	if (!pMaster->m_pMagicShadow->Create(pMaster))
	{
		delete pMaster->m_pMagicShadow;
		return FALSE;
	}

	return TRUE;
}
//------------------------------------------------------------------------
// ����ͨ�ö���
int CSkillCreate::CreateGenericMagic(SSkillCreateParam* pParam)
{
	// ��ȡ�ű�
	IResObject* pScpSkill = (IResObject*)getSkillScpObj(pParam->nAniType, pParam->nIndex);
	if (!pScpSkill)
		return FALSE;

	// ��ȡħ������ID
	DWORD dwMagicID[] = {0,0};
	int nLen = pScpSkill->GetArrayLong("MagicID");
	if (nLen != pScpSkill->GetArrayLong("MagicID", (long*)dwMagicID, nLen))
		return FALSE;

	// ��������̬��ħ����������
	IResObject* pMagicObj = NULL;
	SMagicArchive_General stRun;
	if (dwMagicID[0])
	{
		pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(dwMagicID[0]);
		if (!pMagicObj) return FALSE;
		memset(&stRun, 0, sizeof(stRun));
		g_ResLoader.ReadGeneralMagic(pMagicObj, &stRun);
		stRun.dwResID = pMagicObj->GetID();
	}

	// ���뱬ը̬��ħ����������
	SMagicArchive_General stExplore;
	if (dwMagicID[1])
	{
		pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(dwMagicID[1]);
		if (!pMagicObj) return FALSE;
		memset(&stExplore, 0, sizeof(stExplore));
		g_ResLoader.ReadGeneralMagic(pMagicObj, &stExplore);
		stExplore.dwResID = pMagicObj->GetID();
	}

	// ���촴������
	CMagicMgrGeneral::SMagicMgrArchive_General st;
	st.ptTile = pParam->ptTile;
	st.ptAttackTile = pParam->ptAttack;
	st.pRun = (dwMagicID[0]) ? &stRun : NULL;
	st.pExplode = (dwMagicID[1]) ? &stExplore : NULL;
	st.bNeedAttackDelay = FALSE;
	pScpSkill->GetPropertyLong("Ex_nLoopTime", (long&)st.nLoopTime, 0);
	pScpSkill->GetPropertyLong("Ex_nCanCtrl", (long&)st.bCanCtrl, 0);

	// ����ħ��������
	CMagicMgrGeneral* pMagicMgr = new CMagicMgrGeneral;
	if (!pMagicMgr->Create(&st, sizeof(st)))
	{
		delete pMagicMgr;
		return FALSE;
	}

	return TRUE;
}
//------------------------------------------------------------------------
// ������ӡ����
int CSkillCreate::CreateFootprintAnimate(SSkillCreateParam* pParam, int nDirection, int nLeftRight)
{
	if (nDirection < 0 || nDirection > CrtDirMaxDirCount)
		return FALSE;
	// ��ȡ�ű�
	IResObject* pScpSkill = (IResObject*)getSkillScpObj(pParam->nAniType, pParam->nIndex);
	if (!pScpSkill) return FALSE;

	// ��ȡħ������ID
	DWORD dwMagicID = 0;
	pScpSkill->GetPropertyLong("MagicID", (long&)dwMagicID);
	if (dwMagicID == 0)
		return FALSE;
	POINT ptOffset[CrtDirMaxDirCount];
	if (nLeftRight == 0) // left
		pScpSkill->GetArrayLong("LLegOffset", (long*)ptOffset, CrtDirMaxDirCount*2);
	else
		pScpSkill->GetArrayLong("RLegOffset", (long*)ptOffset, CrtDirMaxDirCount*2);

	// ��������̬��ħ����������
	IResObject* pMagicObj = NULL;
	SMagicArchive_FootPrint stRun;
	pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(dwMagicID);
	if (!pMagicObj) return FALSE;
	memset(&stRun, 0, sizeof(stRun));
	g_ResLoader.ReadLifeMagic(pMagicObj, &stRun);
	stRun.dwResID = pMagicObj->GetID();
	stRun.nDirection = nDirection;
	stRun.ptOffset = ptOffset[nDirection];

	// ���촴������
	CMagicMgrOnePart<CMagicFootprint, SMagicArchive_FootPrint>::SMagicMgrArchive_OnePart ar;
	ar.ptTile = pParam->ptTile;
	ar.ptAttackTile = pParam->ptAttack;
	ar.pCreate = &stRun;

	// ����ħ��������
	CMagicMgrOnePart<CMagicFootprint, SMagicArchive_FootPrint>* pMagicMgr = 
		new CMagicMgrOnePart<CMagicFootprint, SMagicArchive_FootPrint>;
	if (!pMagicMgr->Create(&ar, sizeof(ar)))
	{
		delete pMagicMgr;
		return FALSE;
	}

	return TRUE;
}
//------------------------------------------------------------------------