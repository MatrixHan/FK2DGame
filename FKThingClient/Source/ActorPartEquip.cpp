/**
*	created:		2012-7-4   0:26
*	filename: 		ActorPartEquip
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������װ������
*/
//------------------------------------------------------------------------
#include "../Include/ThingClientCommon.h"
#include "../Include/IRidePart.h"
#include "../../FKGame/SceneObj/IGoods.h"
#include "../../FKGame/SceneObj/IEquipment.h"
#include "../Include/CThingClientFacade.h"
#include "../Include/CreatureViewWrapper.h"
#include "../Include/ActorPartEquip.h"
#include "../../FKGame/Clan/IClanClient.h"
#include "../../FKGame/Common/ThingPart.h"
//------------------------------------------------------------------------
CActorPart_Equip::CActorPart_Equip(void)
{
	m_pMaster = NULL;
}
//------------------------------------------------------------------------
CActorPart_Equip::~CActorPart_Equip(void)
{
	
}
//------------------------------------------------------------------------
BOOL CActorPart_Equip::Create(IThing * pMaster, char * pContext, int nLen)
{
	if(pMaster == NULL || pMaster->GetThingClass()->GetThingClass() != THINGCLASS_ACTOR)
	{
		return FALSE;
	}

	m_pMaster = (IActor *)pMaster;

	// �����¼�
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}

	/*
	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_CLEAN_SHOW);
	pEventServer->AddListener(static_cast<IEventActionListener *>(this),
							  wMsgID,
							  SRC_TYPE_ID_ACTOR,
							  (DWORD)m_pMaster,
							  "CActorPart_Equip::Create");

	wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_CLAND_TITLE);
	pEventServer->AddListener(static_cast<IEventResponseListener *>(this),
							  wMsgID,
							  SRC_TYPE_ID_ACTOR,
							  (DWORD)m_pMaster,
							  "CActorPart_Equip::Create");
	*/
	return TRUE;
}
//------------------------------------------------------------------------
void CActorPart_Equip::Release(void)
{	
	// �˶��¼�
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer != NULL)
	{
		/*
		WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_CLEAN_SHOW);
		pEventServer->RemoveListener(static_cast<IEventActionListener *>(this),
									 wMsgID,
									 SRC_TYPE_ID_ACTOR,
									 (DWORD)m_pMaster);

		wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_CLAND_TITLE);
		pEventServer->RemoveListener(static_cast<IEventResponseListener *>(this),
									 wMsgID,
									 SRC_TYPE_ID_ACTOR,
									 (DWORD)m_pMaster);
		*/
	}

	Freeze();
	delete this;
}
//------------------------------------------------------------------------
DWORD CActorPart_Equip::GetPartID(void)
{
	return THINGPARTID_ACTOR_EQUIP;
}
//------------------------------------------------------------------------
IThing * CActorPart_Equip::GetMaster(void)
{
	return m_pMaster;
}
//------------------------------------------------------------------------
BOOL CActorPart_Equip::Activation(LPCSTR pContext, int nLen)
{	
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CActorPart_Equip::Freeze(void)
{
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CActorPart_Equip::OnGetPrivateContext(LPSTR buf, int &nLen)
{
	nLen = 0;
	return FALSE;
}
//------------------------------------------------------------------------
BOOL CActorPart_Equip::OnGetPublicContext(LPSTR buf, int &nLen)
{
	nLen = 0;
	return FALSE;
}
//------------------------------------------------------------------------
BOOL CActorPart_Equip::OnGetDBContext(LPSTR buf, int &nLen)
{
	nLen = 0;
	return FALSE;
}
//------------------------------------------------------------------------
int CActorPart_Equip::OnMessage(DWORD dwMsg, LPCSTR buf, int nLen)
{
	return 0;
}
//------------------------------------------------------------------------
BOOL CActorPart_Equip::InitClient(UID uidClient)
{
	return TRUE;
}
//------------------------------------------------------------------------
IContainer * CActorPart_Equip::GetContainer(void)
{
	return NULL;
}
//------------------------------------------------------------------------
BOOL CActorPart_Equip::IsExistByGoodsID(DWORD dwGoodsID, DWORD dwNum)
{
	return FALSE;
}
//------------------------------------------------------------------------
DWORD CActorPart_Equip::GetNumByGoodsID(DWORD dwGoodsID)
{
	return 0;
}
//------------------------------------------------------------------------
BOOL CActorPart_Equip::IsRemoveByGoodsID(DWORD dwGoodsID, DWORD dwNum)
{
	return FALSE;
}
//------------------------------------------------------------------------
DWORD CActorPart_Equip::GetRemoveNumByGoodsID(DWORD dwGoodsID)
{
	return 0;
}
//------------------------------------------------------------------------
void CActorPart_Equip::OnAction(IEvent *pEvent)
{
	if(pEvent == NULL)
	{
		return;
	}

	// ��֤
	int nContextLen = 0;
	char * pContext = (char *)pEvent->GetContext(nContextLen);
	if(nContextLen != sizeof(SC_MSG_CLEAN_SHOW_CONTEXT))
	{
		return;
	}
	SC_MSG_CLEAN_SHOW_CONTEXT * pShowRes = (SC_MSG_CLEAN_SHOW_CONTEXT *)(pContext);

	// ��������
	IConfigDataCenter * pDataCenter = g_pThingClientFacade->GetConfigDataCenter();
	if(pDataCenter == NULL)
	{
		return;
	}

	// ȡ�ÿ�����������
	long lOccupation = m_pMaster->GetPropNum(CREATURE_PROPID_OCCUPATION);
	DWORD dwAttackSoundID = 0, dwBeHitSoundID = 0;
	pDataCenter->GetFreeWeaponSoundID(lOccupation, dwAttackSoundID, dwBeHitSoundID);
	
	// ȡ���Ա�
	long lSex = m_pMaster->GetPropNum(CREATURE_PROPID_SEX);
	CCreatureViewWrapper viewwrapper(m_pMaster->GetMapItem());

	int	tempCurResID[CrtPartMax];
	int tempPalType[CrtPartMax];
	memset(tempCurResID, 0, sizeof(tempCurResID));
	memset(tempPalType, 0, sizeof(tempPalType));
	
	for(int i = 0; i < EQUIPPOS_INDEX_MAX; i++)
	{
		// �Ȼ�ԭ�ɵ��ֹ�������
		if(i == EQUIPPOS_INDEX_RIGHT_WEAPON)
		{
			viewwrapper.SetPropWeaponStyle(ActWeaponStyleSingle);
			viewwrapper.SetWeaponAttackSoundID(dwAttackSoundID);
			viewwrapper.SetWeaponBeHitSoundID(dwBeHitSoundID);
		}

		DWORD dwGoodsID   = pShowRes->stShowRes[i].dwGoodsID;
		int   nSmeltLevel = pShowRes->stShowRes[i].nSmeltLevel;
		nSmeltLevel = (nSmeltLevel < 0) ? 0 : (nSmeltLevel >= EQUIPMENT_MAX_SMELTLEVEL ? EQUIPMENT_MAX_SMELTLEVEL - 1 : nSmeltLevel);

		EQUIPMENT_BASEPROPERTY & equipmentbaseproperty = pDataCenter->GetEquipmentBaseProperty(dwGoodsID, nSmeltLevel);
		if(equipmentbaseproperty.lGoodsID == 0)
		{
			continue;
		}

		if(equipmentbaseproperty.lAniID1_partid >= 0 && equipmentbaseproperty.lAniID1_partid < CrtPartMax)
		{
			tempCurResID[equipmentbaseproperty.lAniID1_partid] = (lSex == ACTOR_SEX_MAN) ? equipmentbaseproperty.lAniID1_m : equipmentbaseproperty.lAniID1_w;
			tempPalType[equipmentbaseproperty.lAniID1_partid] = (lSex == ACTOR_SEX_MAN) ? equipmentbaseproperty.lPalette_m_s[nSmeltLevel] : equipmentbaseproperty.lPalette_w_s[nSmeltLevel];
		}

		if(equipmentbaseproperty.lAniID2_partid >= 0 && equipmentbaseproperty.lAniID2_partid < CrtPartMax)
		{
			tempCurResID[equipmentbaseproperty.lAniID2_partid] = (lSex == ACTOR_SEX_MAN) ? equipmentbaseproperty.lAniID2_m : equipmentbaseproperty.lAniID2_w;
			tempPalType[equipmentbaseproperty.lAniID2_partid] = (lSex == ACTOR_SEX_MAN) ? equipmentbaseproperty.lPalette_m_s[nSmeltLevel] : equipmentbaseproperty.lPalette_w_s[nSmeltLevel];
		}

		if(equipmentbaseproperty.lAniID3_partid >= 0 && equipmentbaseproperty.lAniID3_partid < CrtPartMax)
		{
			tempCurResID[equipmentbaseproperty.lAniID3_partid] = (lSex == ACTOR_SEX_MAN) ? equipmentbaseproperty.lAniID3_m : equipmentbaseproperty.lAniID3_w;
			tempPalType[equipmentbaseproperty.lAniID3_partid] = (lSex == ACTOR_SEX_MAN) ? equipmentbaseproperty.lPalette_m_s[nSmeltLevel] : equipmentbaseproperty.lPalette_w_s[nSmeltLevel];
		}

		if(equipmentbaseproperty.lAniID4_partid >= 0 && equipmentbaseproperty.lAniID4_partid < CrtPartMax)
		{
			tempCurResID[equipmentbaseproperty.lAniID4_partid] = (lSex == ACTOR_SEX_MAN) ? equipmentbaseproperty.lAniID4_m : equipmentbaseproperty.lAniID4_w;
			tempPalType[equipmentbaseproperty.lAniID4_partid] = (lSex == ACTOR_SEX_MAN) ? equipmentbaseproperty.lPalette_m_s[nSmeltLevel] : equipmentbaseproperty.lPalette_w_s[nSmeltLevel];
		}

		// ���¹�������
		if(i == EQUIPPOS_INDEX_RIGHT_WEAPON)
		{
			viewwrapper.SetPropWeaponStyle(equipmentbaseproperty.lActionMode);
			DWORD dwAttackSoundID = pDataCenter->GetGoodsSoundID(dwGoodsID, GOODSSOUND_TYPE_ATTACK);
			viewwrapper.SetWeaponAttackSoundID(dwAttackSoundID);
			DWORD dwBeHitSoundID = pDataCenter->GetGoodsSoundID(dwGoodsID, GOODSSOUND_TYPE_BEHIT);
			viewwrapper.SetWeaponBeHitSoundID(dwBeHitSoundID);
		}
	}

	// ֻ�Խ����޸ĵĵط�����������
	for(int i = 0; i < CrtPartMax; i++)
	{
		viewwrapper.ChangeModulePal(i, tempPalType[i]);
		viewwrapper.ChangeModule(i, tempCurResID[i]);
	}

	// ռ��Ǻ������������Ա���һ��������������͡������Ϊ������̬��ʱ��
	// ����������û��װ�����磬������һ����������硣ͨ����ɫϵͳ��ʵ�֡�3����
	// ͬ�ĳǷֱ�ӵ��3����ͬ������ɫ�����硣���ͬʱռ�����ǣ��������������
	// ʾ���ȼ�Ϊ����>�ٹ�>���¡�
	IThingPart * pThingPart = m_pMaster->GetPart(THINGPARTID_ACTOR_CLANPATE);
	if(pThingPart != NULL)
	{
		/*
		IClanClientPate * pClanClientPart = (IClanClientPate *)(pThingPart);
		SCASTELLANGOWN * pCastellanGown = NULL;
		if(pClanClientPart->IsCastellanClan(HX_CASTLE))
		{
			pCastellanGown = pDataCenter->GetCastellanGown(HX_CASTLE, lOccupation);	
		}
		else if(pClanClientPart->IsCastellanClan(BG_CASTLE))
		{
			pCastellanGown = pDataCenter->GetCastellanGown(BG_CASTLE, lOccupation);			
		}
		else if(pClanClientPart->IsCastellanClan(JY_CASTLE))
		{
			pCastellanGown = pDataCenter->GetCastellanGown(JY_CASTLE, lOccupation);
		}
		
		// �������
		if(pCastellanGown != NULL)
		{
			long lManteauResID = lSex == ACTOR_SEX_MAN ? pCastellanGown->lManManteauResID : pCastellanGown->lWomanManteauResID;
			long lManteauPalID = lSex == ACTOR_SEX_MAN ? pCastellanGown->lManManteauPalID : pCastellanGown->lWomanManteauPalID;
				
			viewwrapper.ChangeModulePal(pCastellanGown->lManteauPartID, lManteauPalID);
			viewwrapper.ChangeModule(pCastellanGown->lManteauPartID, lManteauResID);
		}
		*/
	}

	// �������
	pThingPart = m_pMaster->GetPart(THINGPARTID_ACTOR_RIDE);
	if(pThingPart != NULL)
	{
		IRidePart * pRidePart = (IRidePart *)(pThingPart);
		pRidePart->OnMessage(PART_MESSAGE_RIDE_SHOW, NULL, 0);
	}
}
//------------------------------------------------------------------------
void CActorPart_Equip::OnResponse(IEvent *pEvent)
{
	if(pEvent == NULL)
	{
		return;
	}

	// ��֤
	int nContextLen = 0;
	char * pContext = (char *)pEvent->GetContext(nContextLen);
	if(nContextLen != sizeof(WORD))
	{
		return;
	}
	WORD wSubMsg = *((WORD *)pContext);

	// ��������
	IConfigDataCenter * pDataCenter = g_pThingClientFacade->GetConfigDataCenter();
	if(pDataCenter == NULL)
	{
		return;
	}

	/*
	if(wSubMsg == MSG_UPDATE_CLANPATE)
	{
		// ����һ�³�����������
		// ȡ���Ա�
		long lSex = m_pMaster->GetPropNum(CREATURE_PROPID_SEX);
		long lOccupation = m_pMaster->GetPropNum(CREATURE_PROPID_OCCUPATION);
		CCreatureViewWrapper viewwrapper(m_pMaster->GetMapItem());

		// ռ��Ǻ������������Ա���һ��������������͡������Ϊ������̬��ʱ��
		// ����������û��װ�����磬������һ����������硣ͨ����ɫϵͳ��ʵ�֡�3����
		// ͬ�ĳǷֱ�ӵ��3����ͬ������ɫ�����硣���ͬʱռ�����ǣ��������������
		// ʾ���ȼ�Ϊ����>�ٹ�>���¡�
		IThingPart * pThingPart = m_pMaster->GetPart(THINGPARTID_ACTOR_CLANPATE);
		if(pThingPart != NULL)
		{
			IClanClientPate * pClanClientPart = (IClanClientPate *)(pThingPart);
			SCASTELLANGOWN * pCastellanGown = NULL;
			if(pClanClientPart->IsCastellanClan(HX_CASTLE))
			{
				pCastellanGown = pDataCenter->GetCastellanGown(HX_CASTLE, lOccupation);	
			}
			else if(pClanClientPart->IsCastellanClan(BG_CASTLE))
			{
				pCastellanGown = pDataCenter->GetCastellanGown(BG_CASTLE, lOccupation);			
			}
			else if(pClanClientPart->IsCastellanClan(JY_CASTLE))
			{
				pCastellanGown = pDataCenter->GetCastellanGown(JY_CASTLE, lOccupation);
			}
		
			// �������
			if(pCastellanGown != NULL)
			{
				long lManteauResID = lSex == ACTOR_SEX_MAN ? pCastellanGown->lManManteauResID : pCastellanGown->lWomanManteauResID;
				long lManteauPalID = lSex == ACTOR_SEX_MAN ? pCastellanGown->lManManteauPalID : pCastellanGown->lWomanManteauPalID;
				
				viewwrapper.ChangeModulePal(pCastellanGown->lManteauPartID, lManteauPalID);
				viewwrapper.ChangeModule(pCastellanGown->lManteauPartID, lManteauResID);
			}
			
		}
		
		// �������
		pThingPart = m_pMaster->GetPart(THINGPARTID_ACTOR_RIDE);
		if(pThingPart != NULL)
		{
			IRidePart * pRidePart = (IRidePart *)(pThingPart);
			pRidePart->OnMessage(PART_MESSAGE_RIDE_SHOW, NULL, 0);
		}
	}
	*/
}
//------------------------------------------------------------------------