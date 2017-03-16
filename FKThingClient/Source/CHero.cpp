/**
*	created:		2012-7-3   1:44
*	filename: 		CHero
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����
*/
//------------------------------------------------------------------------
#include <windows.h>
#include <mmsystem.h>
#include "../Include/ThingClientCommon.h"
#include "../Include/CHero.h"
#include "../Include/CThingClientFacade.h"
#include "../Include/CreatureViewWrapper.h"
#include "../../FKGame/MapInfo/Tile.h"
//------------------------------------------------------------------------
CHero::CHero(void)
{
	ZeroMemory(m_ThingPart, sizeof(m_ThingPart));
	ZeroMemory(m_CreatureCommand, sizeof(m_CreatureCommand));
}
//------------------------------------------------------------------------
CHero::~CHero(void)
{
	
}
//------------------------------------------------------------------------
BOOL CHero::Create(void)
{
	// ������ͼ����
	struct stTemp
	{
		int				nType;		// ����ID	
		ICreatureSink * pCrtSink;	// 
	};

	// ȡ���Ա��Ա�ȷ�����Σ���׼�����λ��׼Ů���Σ�
	int nSex = GetPropNum(CREATURE_PROPID_SEX);	
	stTemp tempdata;
	tempdata.nType = nSex == ACTOR_SEX_MAN ? MAN_CREATURE_TYPE : FEMALE_CREATURE_TYPE;
	tempdata.pCrtSink = static_cast<ICreatureSink *>(this);
	IItemClassFactory * pItemClassFactory = g_pThingClientFacade->GetItemFactory();
	if(pItemClassFactory == NULL)
	{
		return FALSE;
	}

	if(m_Property.ptLoc.x == 0 || m_Property.ptLoc.y == 0)
	{
		TraceLn( "%s����ʧ�ܣ�����Ϊ�����꣡", GetName());
		return FALSE;
	}
	
	int nDataLen = sizeof(tempdata);
	m_pItemView = (CMapItemView *)pItemClassFactory->CreateItem(miActorObj, (const char *)&tempdata, nDataLen);
	if(m_pItemView == NULL)
	{
		return FALSE;
	}

	// ָ�������ڲ����Ҳ���ͼ��ʱ�����ض���ͼ������
	IConfigDataCenter * pConfigDataCenter = g_pThingClientFacade->GetConfigDataCenter();
	if(pConfigDataCenter == NULL)
	{
		return FALSE;
	}

	int nOccupation = GetPropNum(CREATURE_PROPID_OCCUPATION);
	CCreatureViewWrapper viewwrapper(m_pItemView);
	for(int i = 0; i < CrtPartMax; i++)
	{
		DWORD dwResID = pConfigDataCenter->GetNudePartForActor(nOccupation, i, nSex);
		viewwrapper.SpecialModule(i, dwResID);
	}

	// ˢ������
	UpdateAppearance();
	
	// ���������������
	viewwrapper.SetData((DWORD)this);
	viewwrapper.AddToMap(m_Property.ptLoc);
	viewwrapper.SetTile(m_Property.ptLoc);
	viewwrapper.SetName(GetName());
	viewwrapper.SetNameColor(m_Property.nNameColor);
	viewwrapper.AddFlag(CMapItemView::flagCanbeSelected);
	viewwrapper.SetDirection(m_Property.nDir);
	int nHPCur = GetPropNum(CREATURE_PROPID_HPCUR);
	int nHPMax = GetPropNum(CREATURE_PROPID_HPMAXCUR);
	viewwrapper.SetBloodInfo(GetPropNum(CREATURE_PROPID_HPCUR), max(nHPCur, nHPMax));
	viewwrapper.SetFlagColorOnMinmap(0, 255, 0);

	viewwrapper.SetMoveStyle(ActMoveStyleRun);
	if(m_Property.nWalkStyle == CREATURE_MOVESTYLE_SIT)
	{			
		viewwrapper.Sit();
	}
	else if(m_Property.nWalkStyle == CREATURE_MOVESTYLE_WALK)
	{
		viewwrapper.SetMoveStyle(ActMoveStyleWalk);
	}

	viewwrapper.SetMoveSpeedLevel((int *)&m_Property.nMoveSpeedLevel);	
	m_CreatureActStateMachine.Create((ICreature *)this);
	
	g_pGlobal->m_pMapView->SetPlayerItem(m_pItemView);
	g_pGlobal->m_pPlayer = this;

	return TRUE;	
}
//------------------------------------------------------------------------
void CHero::Release(void)
{
	// ��һ�¼���ȥ,����ɾ��
	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_RELEASE);
	OnMessage(wMsgID, NULL, 0);

	// �ͷŲ���
	for(int i = 0; i < THINGPARTID_THING_MAXID; i++)
	{
		SAFE_RELEASE(m_ThingPart[i]);
	}

	if(g_pThingClientFacade->GetThingTable() == NULL)
	{
		delete this;
		return;
	}

	if(g_pThingClientFacade->GetMapView() == NULL)
	{
		delete this;
		return;
	}

	if(g_pGlobal->m_pPlayer == this)
	{
		g_pThingClientFacade->GetMapView()->SetPlayerItem(NULL);
		g_pGlobal->m_pPlayer = NULL;
	}
		
	// ��ThingTable�����
	if(g_pThingClientFacade->GetThingTable()->Remove(m_uid) == FALSE)
	{
		TraceLn("��ThingTable�����ʧ�ܣ�%s", GetName());
	}

	// �ͷ�ͷ��
	SAFE_RELEASE(m_pItemHead);

	// ���MapItem
	if(m_pItemView != NULL)
	{
		POINTS _ptLoc = GetLoc();
		POINT  ptLoc = { _ptLoc.x, _ptLoc.y };
		if(g_pThingClientFacade->GetMapView()->RemoveItem(ptLoc, m_pItemView) == FALSE)
		{
			TraceLn("���MapItemʧ�ܣ�%s[%d, %d]", GetName(), ptLoc.x, ptLoc.y);
		}

		SAFE_RELEASE(m_pItemView);
	}

	delete this;
}
//------------------------------------------------------------------------
// ˽�ж����ֳ�����
// ˵����
// ˽�ж����ֳ���ָ�û��Լ��Ľ�ɫ��װ���ȵ���ϸ��Ϣ���������ǿ�������
// ��dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ���
// �磺�û���¼ʱ��������ȡ����������ֳ�
// �ͻ��˸���dwClassID�������󣬵���OnGetPrivateContext()���ö����ֳ�
BOOL CHero::OnSetPrivateContext(LPCSTR buf, int nLen)
{
	if(buf == NULL || nLen != sizeof(SACTOR_PROPERTY_PRIVATE))
	{
		TraceLn("CHero::OnSetPrivateContext Failed! [nLen != sizeof(SACTOR_PROPERTY_PRIVATE)]");
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SACTOR_PROPERTY_PRIVATE));
	m_Property.nMoveSpeedLevel = m_Property.nMoveSpeedLevel < 0 ? 0 : m_Property.nMoveSpeedLevel;
	m_Property.nMoveSpeedLevel = m_Property.nMoveSpeedLevel >= MAX_MOVESPEED_LEVEL ? MAX_MOVESPEED_LEVEL - 1 : m_Property.nMoveSpeedLevel;

	return TRUE;
}
//------------------------------------------------------------------------
int	CHero::SetPropNum(DWORD dwPropID, int nValue)
{  	
	switch(dwPropID)
	{
		// �Ա� ��1 ���У�2 ��Ů��
	case CREATURE_PROPID_SEX:
		{
			m_Property.nSex = nValue;
		}
		break;
		// �ȼ�
	case CREATURE_PROPID_LEVEL:
		{
			m_Property.nLevel = nValue;
		}
		break;
		// ����
	case CREATURE_PROPID_PHYLE:
		{
			m_Property.nPhyle = nValue;
		}
		break;
		// ְҵ
	case CREATURE_PROPID_OCCUPATION:
		{
			m_Property.nOccupation = nValue;
		}
		break;
		// ��ǰ����ֵ
	case CREATURE_PROPID_HPCUR:
		{
			m_Property.nHPCur = nValue;
		}
		break;
		// ��ǰ�������ֵ
	case CREATURE_PROPID_HPMAXCUR:
		{
			m_Property.nHPMaxCur = nValue;
		}
		break;
		// ��ǰ����ֵ	
	case CREATURE_PROPID_MPCUR:
		{
			m_Property.nMPCur = nValue;
		}
		break;
		// ��ǰ�����ֵ
	case CREATURE_PROPID_MPMAXCUR:
		{
			m_Property.nMPMaxCur = nValue;
		}
		break;
		// ����(��ǰ)
	case CREATURE_PROPID_FORCECUR:
		{
			m_Property.nForceCur = nValue;
		}
		break;
		// ����(��ǰ)
	case CREATURE_PROPID_CONSTITCUR:
		{
			m_Property.nConstitCur = nValue;
		}
		break;	
		// ����(��ǰ)
	case CREATURE_PROPID_CELERITYCUR:
		{
			m_Property.nCelerityCur = nValue;
		}
		break;
		// ����(��ǰ)
	case CREATURE_PROPID_MENTALCUR:
		{
			m_Property.nMentalCur = nValue;
		}
		break;
		// �ǻ�(��ǰ)
	case CREATURE_PROPID_WISDOMCUR:
		{
			m_Property.nWisdomCur = nValue;
		}
		break;
		// ��С��������(��ǰ)
	case CREATURE_PROPID_P_A_MINCUR:
		{
			m_Property.nP_A_MinCur = nValue;
		}
		break;
		// �����������(��ǰ)
	case CREATURE_PROPID_P_A_MAXCUR:
		{
			m_Property.nP_A_MaxCur = nValue;
		}
		break;
		// ��С����������(��ǰ)
	case CREATURE_PROPID_M_A_MINCUR:
		{
			m_Property.nM_A_MinCur = nValue;
		}
		break;
		// �����������(��ǰ)
	case CREATURE_PROPID_M_A_MAXCUR:
		{
			m_Property.nM_A_MaxCur = nValue;
		}
		break;
		// ��С����������
	case CREATURE_PROPID_F_A_MIN:
		{
			m_Property.nF_A_Min = nValue;
		}
		break;
		// ������������
	case CREATURE_PROPID_F_A_MAX:
		{
			m_Property.nF_A_Max = nValue;
		}
		break;
		// ��С������������
	case CREATURE_PROPID_I_A_MIN:
		{
			m_Property.nI_A_Min = nValue;
		}
		break;
		// ��������������
	case CREATURE_PROPID_I_A_MAX:
		{
			m_Property.nI_A_Max = nValue;
		}
		break;
		// ��С�編��������
	case CREATURE_PROPID_E_A_MIN:
		{
			m_Property.nE_A_Min = nValue;
		}
		break;
		// ���編��������
	case CREATURE_PROPID_E_A_MAX:
		{
			m_Property.nE_A_Max = nValue;
		}
		break;
		// ��С������������
	case CREATURE_PROPID_N_A_MIN:
		{
			m_Property.nN_A_Min = nValue;
		}
		break;
		// ��󰵷���������
	case CREATURE_PROPID_N_A_MAX:
		{
			m_Property.nN_A_Max = nValue;
		}
		break;
		// ��С���������(��ǰ)
	case CREATURE_PROPID_P_D_MINCUR:
		{
			m_Property.nP_D_MinCur = nValue;
		}
		break;
		// ������������(��ǰ)
	case CREATURE_PROPID_P_D_MAXCUR:
		{
			m_Property.nP_D_MaxCur = nValue;
		}
		break;
		// ��С����������(��ǰ)
	case CREATURE_PROPID_M_D_MINCUR:
		{
			m_Property.nM_D_MinCur = nValue;
		}
		break;
		// �����������(��ǰ)
	case CREATURE_PROPID_M_D_MAXCUR:
		{
			m_Property.nM_D_MaxCur = nValue;
		}
		break;
		// ��С����������
	case CREATURE_PROPID_F_D_MIN:
		{
			m_Property.nF_D_Min = nValue;
		}
		break;
		// ������������
	case CREATURE_PROPID_F_D_MAX:
		{
			m_Property.nF_D_Max = nValue;
		}
		break;
		// ��С������������
	case CREATURE_PROPID_I_D_MIN:
		{	
			m_Property.nI_D_Min = nValue;
		}
		break;
		// ��������������
	case CREATURE_PROPID_I_D_MAX:
		{
			m_Property.nI_D_Max = nValue;
		}
		break;
		// ��С�編��������
	case CREATURE_PROPID_E_D_MIN:
		{
			m_Property.nE_D_Min = nValue;
		}
		break;
		// ���編��������
	case CREATURE_PROPID_E_D_MAX:
		{
			m_Property.nE_D_Max = nValue;
		}
		break;
		// ��С������������
	case CREATURE_PROPID_N_D_MIN:
		{
			m_Property.nN_D_Min = nValue;
		}
		break;
		// ��󰵷���������
	case CREATURE_PROPID_N_D_MAX:
		{
			m_Property.nN_D_Max = nValue;
		}
		break;
		// ��Ǯ
	case CREATURE_PROPID_HOLDMONEY_V1:
		{
			m_Property.nHoldMoney = nValue;
		}
		break;
		// �ֿ��Ǯ
	case CREATURE_PROPID_WAREMONEY_V1:
		{
			m_Property.nWareMoney = nValue;	
		}
		break;
		// ����
	case CREATURE_PROPID_EXP_V1:
		{
			m_Property.nExp = nValue;
		}
		break;
		// ��ǰ����
	case CREATURE_PROPID_BURTHENCUR:
		{
			m_Property.nBurthenCur = nValue;
		}
		break;
		// �����
	case CREATURE_PROPID_BURTHENMAX:
		{
			m_Property.nBurthenMax = nValue;
		}
		break;
		// PKֵ
	case CREATURE_PROPID_PKVALUE:
		{
			m_Property.nPKValue = nValue;
		}
		break;
		// ��ǰ��·��ʽ
	case CREATURE_PROPID_WALKSTYLE:
		{
			m_Property.nWalkStyle = nValue;
		}
		break;
		// �ƶ��ٶȵȼ�
	case CREATURE_PROPID_MOVESPEEDLEVEL:
		{
			m_Property.nMoveSpeedLevel = nValue;
			m_Property.nMoveSpeedLevel = m_Property.nMoveSpeedLevel < 0 ? 0 : m_Property.nMoveSpeedLevel;
			m_Property.nMoveSpeedLevel = m_Property.nMoveSpeedLevel >= MAX_MOVESPEED_LEVEL ? MAX_MOVESPEED_LEVEL - 1 : m_Property.nMoveSpeedLevel;	
		}
		break;
		// װ��������ID
	case CREATURE_PROPID_EQUIPCONTAINERID:
		{
			m_Property.nEquipContainerID = nValue;
		}
		break;
		// װ��������ID
	case CREATURE_PROPID_PACKETCONTAINERID:
		{
			m_Property.nPacketContainerID = nValue;
		}
		break;
		// ����ͷ��
	case CREATURE_PROPID_HEADRESID:
		{
			m_Property.nHeadResID = nValue;
		}
		break;
		// ������ɫ
	case CREATURE_PROPID_NAMECOLOR:
		{
			m_Property.nNameColor = nValue;
		}
		break;
		// ����ID
	case CREATURE_PROPID_CLANID:
		{
			m_Property.nClanID = nValue;
		}
		break;
	default:
		{
			PrintEventLn("error : CHero::SetPropNum : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return 0;
}
//------------------------------------------------------------------------
int CHero::GetPropNum(DWORD dwPropID)
{
	switch(dwPropID)
	{
		// �Ա� ��1 ���У�2 ��Ů��
	case CREATURE_PROPID_SEX:
		{
			return m_Property.nSex;
		}
		break;
		// �ȼ�
	case CREATURE_PROPID_LEVEL:
		{
			return m_Property.nLevel;
		}
		break;
		// ����
	case CREATURE_PROPID_PHYLE:
		{
			return m_Property.nPhyle;
		}
		break;
		// ְҵ
	case CREATURE_PROPID_OCCUPATION:
		{
			return m_Property.nOccupation;
		}
		break;
		// ��ǰ����ֵ
	case CREATURE_PROPID_HPCUR:
		{
			return m_Property.nHPCur;
		}
		break;
		// ��ǰ�������ֵ
	case CREATURE_PROPID_HPMAXCUR:
		{
			return m_Property.nHPMaxCur;
		}
		break;
		// ��ǰ����ֵ	
	case CREATURE_PROPID_MPCUR:
		{
			return m_Property.nMPCur;
		}
		break;
		// ��ǰ�����ֵ
	case CREATURE_PROPID_MPMAXCUR:
		{
			return m_Property.nMPMaxCur;
		}
		break;
		// UserID
	case CREATURE_PROPID_USERID:
		{
			return m_Property.nUserDBID;
		}
		break;
		// ActorID
	case CREATURE_PROPID_ACTORID:
		{
			return m_Property.nActorDBID;
		}
		break;
		// ����(��ǰ)
	case CREATURE_PROPID_FORCECUR:
		{
			return m_Property.nForceCur;
		}
		break;
		// ����(��ǰ)
	case CREATURE_PROPID_CONSTITCUR:
		{
			return m_Property.nConstitCur;
		}
		break;
		// ����(��ǰ)
	case CREATURE_PROPID_CELERITYCUR:
		{
			return m_Property.nCelerityCur;
		}
		break;
		// ����(��ǰ)
	case CREATURE_PROPID_MENTALCUR:
		{
			return m_Property.nMentalCur;
		}
		break;
		// �ǻ�(��ǰ)
	case CREATURE_PROPID_WISDOMCUR:
		{
			return m_Property.nWisdomCur;
		}
		break;
		// ��С��������(��ǰ)
	case CREATURE_PROPID_P_A_MINCUR:
		{
			return m_Property.nP_A_MinCur;
		}
		break;
		// �����������(��ǰ)
	case CREATURE_PROPID_P_A_MAXCUR:
		{
			return m_Property.nP_A_MaxCur;
		}
		break;
		// ��С����������(��ǰ)
	case CREATURE_PROPID_M_A_MINCUR:
		{
			return m_Property.nM_A_MinCur;
		}
		break;
		// �����������(��ǰ)
	case CREATURE_PROPID_M_A_MAXCUR:
		{
			return m_Property.nM_A_MaxCur;
		}
		break;
		// ��С����������
	case CREATURE_PROPID_F_A_MIN:
		{
			return m_Property.nF_A_Min;
		}
		break;
		// ������������
	case CREATURE_PROPID_F_A_MAX:
		{
			return m_Property.nF_A_Max;
		}
		break;
		// ��С������������
	case CREATURE_PROPID_I_A_MIN:
		{
			return m_Property.nI_A_Min;
		}
		break;
		// ��������������
	case CREATURE_PROPID_I_A_MAX:
		{
			return m_Property.nI_A_Max;
		}
		break;
		// ��С�編��������
	case CREATURE_PROPID_E_A_MIN:
		{
			return m_Property.nE_A_Min;
		}
		break;
		// ���編��������
	case CREATURE_PROPID_E_A_MAX:
		{
			return m_Property.nE_A_Max;
		}
		break;
		// ��С������������
	case CREATURE_PROPID_N_A_MIN:
		{
			return m_Property.nN_A_Min;
		}
		break;
		// ��󰵷���������
	case CREATURE_PROPID_N_A_MAX:
		{
			return m_Property.nN_A_Max;
		}
		break;

		// ��С���������(��ǰ)
	case CREATURE_PROPID_P_D_MINCUR:
		{
			return m_Property.nP_D_MinCur;
		}
		break;
		// ������������(��ǰ)
	case CREATURE_PROPID_P_D_MAXCUR:
		{
			return m_Property.nP_D_MaxCur;
		}
		break;
		// ��С����������(��ǰ)
	case CREATURE_PROPID_M_D_MINCUR:
		{
			return m_Property.nM_D_MinCur;
		}
		break;
		// �����������(��ǰ)
	case CREATURE_PROPID_M_D_MAXCUR:
		{
			return m_Property.nM_D_MaxCur;
		}
		break;
		// ��С����������
	case CREATURE_PROPID_F_D_MIN:
		{
			return m_Property.nF_D_Min;
		}
		break;
		// ������������
	case CREATURE_PROPID_F_D_MAX:
		{
			return m_Property.nF_D_Max;
		}
		break;
		// ��С������������
	case CREATURE_PROPID_I_D_MIN:
		{	
			return m_Property.nI_D_Min;
		}
		break;
		// ��������������
	case CREATURE_PROPID_I_D_MAX:
		{
			return m_Property.nI_D_Max;
		}
		break;
		// ��С�編��������
	case CREATURE_PROPID_E_D_MIN:
		{
			return m_Property.nE_D_Min;
		}
		break;
		// ���編��������
	case CREATURE_PROPID_E_D_MAX:
		{
			return m_Property.nE_D_Max;
		}
		break;
		// ��С������������
	case CREATURE_PROPID_N_D_MIN:
		{
			return m_Property.nN_D_Min;
		}
		break;
		// ��󰵷���������
	case CREATURE_PROPID_N_D_MAX:
		{
			return m_Property.nN_D_Max;
		}
		break;
		// ��Ǯ
	case CREATURE_PROPID_HOLDMONEY_V1:
		{
			return m_Property.nHoldMoney;
		}
		break;
		// ����
	case CREATURE_PROPID_EXP_V1:
		{
			return m_Property.nExp;
		}
		break;
		// ��ǰ����
	case CREATURE_PROPID_BURTHENCUR:
		{
			return m_Property.nBurthenCur;
		}
		break;
		// �����
	case CREATURE_PROPID_BURTHENMAX:
		{
			return m_Property.nBurthenMax;
		}
		break;
		// �ֿ��Ǯ
	case CREATURE_PROPID_WAREMONEY_V1:
		{
			return m_Property.nWareMoney;	
		}
		break;
		// װ��������ID
	case CREATURE_PROPID_EQUIPCONTAINERID:
		{
			return m_Property.nEquipContainerID;
		}
		break;
		// ����������ID
	case CREATURE_PROPID_PACKETCONTAINERID:
		{
			return m_Property.nPacketContainerID;
		}
		break;
		// PKֵ
	case CREATURE_PROPID_PKVALUE:
		{
			return m_Property.nPKValue;
		}
		break;
		// ��ǰ��·��ʽ
	case CREATURE_PROPID_WALKSTYLE:
		{
			return m_Property.nWalkStyle;
		}
		break;
		// �ƶ��ٶȵȼ�
	case CREATURE_PROPID_MOVESPEEDLEVEL:
		{
			return  m_Property.nMoveSpeedLevel;
		}
		break;
		// ����ͷ��
	case CREATURE_PROPID_HEADRESID:
		{
			return m_Property.nHeadResID;
		}
		break;
		// ������ɫ
	case CREATURE_PROPID_NAMECOLOR:
		{
			return m_Property.nNameColor;
		}
		break;
		// ����ID
	case CREATURE_PROPID_CLANID:
		{
			return m_Property.nClanID;
		}
		break;
		// ����
	case CREATURE_PROPID_DIR:
		{
			return m_Property.nDir;
		}
		break;
	default:
		{
			PrintEventLn("error : CHero::GetPropNum : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return 0;
}	
//------------------------------------------------------------------------
LPCSTR CHero::SetPropStr(DWORD dwPropID, LPCSTR strPropValue)
{
	switch(dwPropID)
	{
		// 1  = ����
	case CREATURE_PROPID_NAME:
		{
			lstrcpyn(m_Property.szName, strPropValue, sizeof(m_Property.szName));
			return m_Property.szName;
		}
		break;
	default:
		{
			PrintEventLn("error : CActor::GetStrProp : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return "";
}
//------------------------------------------------------------------------
LPCSTR CHero::GetPropStr(DWORD dwPropID)
{
	switch(dwPropID)
	{
		// 1  = ����
	case CREATURE_PROPID_NAME:
		{
			return m_Property.szName;
		}
		break;
	default:
		{
			PrintEventLn("error : CHero::GetStrProp : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return "";
}
//------------------------------------------------------------------------
void CHero::OnCreatureMoveToTileCenter(const POINTS &ptCurTile, const POINTS &ptNextTile)
{
	POINT ptsNextTile = { ptNextTile.x, 
						  ptNextTile.y};

	ICreatureActState * pCreatureActState = GetCurActState();
	if((pCreatureActState != NULL) &&
	   ((g_pGlobal->m_pMapView->IsValidTile(ptsNextTile) == FALSE) || 
	   ((g_pGlobal->m_pMapView->GetTile(ptsNextTile)->IsBlock() == TRUE))))
	{
		pCreatureActState->OnMoveItem(this, ptsNextTile);
	}
}
//------------------------------------------------------------------------
void CHero::OnMessage(DWORD dwMsg, LPCSTR buf, int nLen)
{
	// ״̬����
	ICreatureActState * pCreatureActState = m_CreatureActStateMachine.GetCurActState();
	if(pCreatureActState == NULL || !pCreatureActState->OnMessage(dwMsg, buf, nLen))
	{
		return;
	}

	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return;
	}

	/*
	// ���ж�����Ƿ�ͨ��
	if(pEventServer->FireVote((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_ACTOR, (DWORD)this) == FALSE)
	{
		return;
	}

	// Ȼ����Ϊ�¼�
	pEventServer->FireAction((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_ACTOR, (DWORD)this);
	*/
}
//------------------------------------------------------------------------
int CHero::OnCommand(DWORD dwCommand, LPCSTR buf, int nLen)
{
	if(dwCommand < 0 || dwCommand >= CREATURE_COMMANDID_MAXID || m_CreatureCommand[dwCommand] == NULL)
	{
		return 0;
	}

	return m_CreatureCommand[dwCommand]->OnCommand(this, buf, nLen);
}
//------------------------------------------------------------------------
BOOL CHero::GotoActState(DWORD dwActStateID, LPCSTR pContext, int nLen)
{
	return m_CreatureActStateMachine.GotoActState(dwActStateID, pContext, nLen);
}
//------------------------------------------------------------------------
ICreatureActState * CHero::GetCurActState(void)
{
	return m_CreatureActStateMachine.GetCurActState();
}
//------------------------------------------------------------------------
BOOL CHero::AddPart(IThingPart * pPart)
{
	if(pPart == NULL)
	{
		return FALSE;
	}

	if(pPart->GetPartID() < 0 || pPart->GetPartID() >= THINGPARTID_THING_MAXID)
	{
		return FALSE;
	}

	if(m_ThingPart[pPart->GetPartID()] != NULL)
	{
		return FALSE;
	}

	m_ThingPart[pPart->GetPartID()] = pPart;

	return TRUE;	
}
//------------------------------------------------------------------------
BOOL CHero::RemovePart(DWORD dwPartID)
{
	if(dwPartID < 0 || dwPartID >= THINGPARTID_THING_MAXID)
	{
		return FALSE;
	}

	m_ThingPart[dwPartID] = NULL;

	return TRUE;
}
//------------------------------------------------------------------------
IThingPart * CHero::GetPart(DWORD dwPartID)
{
	if(dwPartID < 0 || dwPartID >= THINGPARTID_THING_MAXID)
	{
		return NULL;
	}

	return m_ThingPart[dwPartID];
}
//------------------------------------------------------------------------
BOOL CHero::AddCreatureCommand(ICreatureCommand * pCreatureCommand)
{
	if(pCreatureCommand == NULL)
	{
		return FALSE;
	}

	if(pCreatureCommand->GetCommandID() < 0 || pCreatureCommand->GetCommandID() >= CREATURE_COMMANDID_MAXID)
	{
		return FALSE;
	}

	m_CreatureCommand[pCreatureCommand->GetCommandID()] = pCreatureCommand;	

	return TRUE;
}
//------------------------------------------------------------------------
void CHero::Accept(IMapItemVisitor &visitor)
{
	visitor.VisitHero();
}
//------------------------------------------------------------------------
void CHero::SetDying(BOOL bDying)
{

}
//------------------------------------------------------------------------
BOOL CHero::IsDying(void)
{
	return (m_CreatureActStateMachine.GetCurActStateID() == CREATURE_STATEID_DIE) ? TRUE : FALSE;
}
//------------------------------------------------------------------------