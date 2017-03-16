/**
*	created:		2012-6-30   23:19
*	filename: 		CConfigDataCenter
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "PreInclude.h"
#include "CConfigDataCenter.h"
#include "../GameGlobal/ClientGlobal.h"
//------------------------------------------------------------------------
CConfigDataCenter::CConfigDataCenter(void)
{
	
}
//------------------------------------------------------------------------
CConfigDataCenter::~CConfigDataCenter(void)
{

}
//------------------------------------------------------------------------
// ������ؽű�
BOOL CConfigDataCenter::LoadScript(void)
{
	if(!m_ConfigMapInfor.Create())
	{
		return FALSE;
	}

	/*
	if(!m_ConfigProp.Create())
	{
		return FALSE;
	}

	if(!m_ConfigCreatureNude.Create())
	{
		return FALSE;
	}

	if(!m_ConfigMonster.Create())
	{
		return FALSE;
	}

	if(!m_ConfigGoodsProperty.Create())
	{
		return FALSE;
	}

	if(!m_ConfigActorExp.Create())
	{
		return FALSE;
	}

	if(!m_ConfigActorHeadRes.Create())
	{
		return FALSE;
	}

	if(!m_ConfigGoodsSound.Create())
	{
		return FALSE;
	}

	if(!m_ConfigSecret.Create())
	{
		return FALSE;
	}

	if(!m_ConfigExpDegression.Create())
	{
		return FALSE;
	}

	if(!m_ConfigHorse.Create())
	{
		return FALSE;
	}

	if(!m_ConfigSuitEquip.Create())
	{
		return FALSE;
	}

	if(!m_ConfigCastellanGown.Create())
	{
		return FALSE;
	}
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// �������Ե�����
IConfigProp * CConfigDataCenter::GetConfigProp(void)
{
	return NULL;
	// return static_cast<IConfigProp *>(&m_ConfigProp);
}
//------------------------------------------------------------------------
// ȡ�����е�ͼ����Ϣ, IN : pInfor �����ɵ�MAP_INFOR����, OUT : ��ǰ�ж����ŵ�ͼ
BOOL CConfigDataCenter::GetMapInfor(MAP_INFOR * pInfor, DWORD &dwCount)
{
	return FALSE;
	// return m_ConfigMapInfor.GetMapInfor(pInfor, dwCount);
}
//------------------------------------------------------------------------
// ȡ��ĳһ�ŵ�ͼ����Ϣ
MAP_INFOR *	CConfigDataCenter::GetMapInfor(DWORD dwMapID)
{
	return m_ConfigMapInfor.GetMapInfor(dwMapID);
}
//------------------------------------------------------------------------
// ȡ�õ�ǰ��ͼ����
DWORD CConfigDataCenter::GetMapCount(void)
{
	return 0;
	// return m_ConfigMapInfor.GetMapCount();
}
//------------------------------------------------------------------------
// ȡ������ĳһְҵ��ĳһ�������Ĺ�������ԴID
// dwOccupation : ְҵ
// dwPartID = ������
DWORD CConfigDataCenter::GetNudePartForActor(DWORD dwOccupation, DWORD dwPartID, DWORD dwSex)
{
	return 0;
	//return m_ConfigCreatureNude.GetNudePartForActor(dwOccupation, dwPartID, dwSex);
}
//------------------------------------------------------------------------
// ��  ����ȡ������ĳһְҵ��ĳһ�������Ĺ�������ԴID show
// ��  ����dwOccupation : ְҵ
// ����ֵ��dwPartID = ������
DWORD CConfigDataCenter::GetNudePartForActor_Show(DWORD dwOccupation, DWORD dwPartID, DWORD dwSex)
{
	return 0;
	// return m_ConfigCreatureNude.GetNudePartForActor_Show(dwOccupation, dwPartID, dwSex);
}
//------------------------------------------------------------------------
// ��  ����ȡ��ĳ��������趨
MONSTER_CONFIG_INFOR & CConfigDataCenter::GetMonsterInfor(long lMonsterID)
{
	MONSTER_CONFIG_INFOR tmp;
	return tmp;
	// return m_ConfigMonster.GetMonsterInfor(lMonsterID);
}
//------------------------------------------------------------------------
// ��  ����ȡ����Ʒ��������
GOODS_BASEPROPERTY & CConfigDataCenter::GetGoodsBaseProperty(long lGoodsID)
{
	GOODS_BASEPROPERTY tmp;
	return tmp;
	// return m_ConfigGoodsProperty.GetGoodsBaseProperty(lGoodsID);
}
//------------------------------------------------------------------------
// ��  ����ȡ��ҩƷ��������
MEDICAMENT_BASEPROPERTY & CConfigDataCenter::GetMedicamentBaseProperty(long lGoodsID)
{
	MEDICAMENT_BASEPROPERTY tmp;
	return tmp;
	// return m_ConfigGoodsProperty.GetMedicamentBaseProperty(lGoodsID);
}
//------------------------------------------------------------------------
// ��  ����ȡ��װ����������
EQUIPMENT_BASEPROPERTY & CConfigDataCenter::GetEquipmentBaseProperty(long lGoodsID, long lSmeltLevel)
{
	EQUIPMENT_BASEPROPERTY tmp;
	return tmp;
	// return m_ConfigGoodsProperty.GetEquipmentBaseProperty(lGoodsID, lSmeltLevel);
}
//------------------------------------------------------------------------
// ��  ����ȡ��ƾ֤��������
WARRANT_BASEPROPERTY & CConfigDataCenter::GetWarrantBaseProperty(long lGoodsID)
{
	WARRANT_BASEPROPERTY tmp;
	return tmp;
	// return m_ConfigGoodsProperty.GetWarrantBaseProperty(lGoodsID);
}
//------------------------------------------------------------------------
// ��  ����ȡ��Ԫ����������
VALUABLES_BASEPROPERTY & CConfigDataCenter::GetValuablesBaseProperty(long lGoodsID)
{
	VALUABLES_BASEPROPERTY tmp;
	return tmp;
	// return m_ConfigGoodsProperty.GetValuablesBaseProperty(lGoodsID);
}
//------------------------------------------------------------------------
// ��  ����ȡ������ĳһ�ȼ��ľ���
long CConfigDataCenter::GetActorExp(long lLevel)
{
	return 0;
	// return m_ConfigActorExp.GetActorExp(lLevel);
}
//------------------------------------------------------------------------
// ��  ����ȡ������ĳһְҵ�����ֵĹ����ͻ�������
void CConfigDataCenter::GetFreeWeaponSoundID(DWORD dwOccupation, DWORD &dwAttackSoundID, DWORD &dwBeHitSoundID)
{
	return;
	// m_ConfigCreatureNude.GetFreeWeaponSoundID(dwOccupation, dwAttackSoundID, dwBeHitSoundID);
}
//------------------------------------------------------------------------
// ��  ����ͨ��ͷ��������ȡ�ø�������ͷ��ID
DWORD CConfigDataCenter::GetHeadResIDByIndex(DWORD dwIndex, DWORD dwType)
{
	return 0;
	// return m_ConfigActorHeadRes.GetHeadResIDByIndex(dwIndex, dwType);
}
//------------------------------------------------------------------------
// ��  ����ͨ��ְҵ���Ա��ȡ�ɹ�ʹ�õ�ͷ��������
vector<DWORD> CConfigDataCenter::GetHeadResIDByOccupation(DWORD dwOccupation, DWORD dwSex)
{
	vector< DWORD > tmp;
	return tmp;
	// return m_ConfigActorHeadRes.GetHeadResIDByOccupation(dwOccupation, dwSex);
}
//------------------------------------------------------------------------
// ��  ����ͨ����ƷIDȡ����ص�����
DWORD CConfigDataCenter::GetGoodsSoundID(DWORD dwGoodsID, DWORD dwSoundType)
{
	return 0;
	// return m_ConfigGoodsSound.GetGoodsSoundID(dwGoodsID, dwSoundType);
}
//------------------------------------------------------------------------
// ��  ����ȡ���䷽��Ϣ
SECRET_INFOR * CConfigDataCenter::GetSecretInfor(long lSecretID)
{
	return NULL;
	//return m_ConfigSecret.GetSecretInfor(lSecretID);
}
//------------------------------------------------------------------------
// ��  ����ȡ�þ���˥��, dwMasterLevel = ����ȼ�, nLevelDiff = �ȼ���
float CConfigDataCenter::GetExpDegression(DWORD dwMasterLevel, int nLevelDiff)
{
	return 0.0f;
	//return m_ConfigExpDegression.GetExpDegression(dwMasterLevel, nLevelDiff);
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ݵ����ò���
HORSE_CONFIG_INFOR * CConfigDataCenter::GetHorseInfor(long lID)
{
	return NULL;
	//return m_ConfigHorse.GetHorseInfor(lID);
}
//------------------------------------------------------------------------
// ��  ����ȡ����װ����Ч��
SUITEQUIP_EFFECT * CConfigDataCenter::GetSuitEquipEffect(long lSuitEquipID)
{
	return NULL;
	//return m_ConfigSuitEquip.GetSuitEquipEffect(lSuitEquipID);
}
//------------------------------------------------------------------------
// ��  ������ȡ����������ҫװ��
SCASTELLANGOWN * CConfigDataCenter::GetCastellanGown(long lCastellanID, long lOccupation)
{
	return NULL;
	//return m_ConfigCastellanGown.GetCastellanGown(lCastellanID, lOccupation);
}
//------------------------------------------------------------------------