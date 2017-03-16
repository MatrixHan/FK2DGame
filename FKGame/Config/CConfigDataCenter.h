/**
*	created:		2012-6-30   23:17
*	filename: 		CConfigDataCenter
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ͨ����ȡ�ű���ø��ַ�������ֵ����, Ȼ��ͨ��ȫ�ֵ�ʵ����ȡ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IConfigDataCenter.h"
//------------------------------------------------------------------------
#include "CConfigMapInfor.h"
//#include "CConfigProp.h"
//#include "CConfigCreatureNude.h"
//#include "CConfigMonster.h"
//#include "CConfigGoodsProperty.h"
//#include "CConfigActorExp.h"
//#include "CConfigActorHeadRes.h"
//#include "CConfigGoodsSound.h"
//#include "CConfigSecret.h"
//#include "CConfigExpDegression.h"
//#include "CConfigHorse.h"
//#include "CConfigSuitEquip.h"
//#include "CConfigCastellanGown.h"
//------------------------------------------------------------------------
class CConfigDataCenter : public IConfigDataCenter
{
public:
	// �������Ե�����
	virtual IConfigProp *				GetConfigProp(void);

	// ȡ�����е�ͼ����Ϣ, IN : pInfor �����ɵ�MAP_INFOR����, OUT : ��ǰ�ж����ŵ�ͼ
	virtual BOOL						GetMapInfor(MAP_INFOR * pInfor, DWORD &dwCount);

	// ȡ��ĳһ�ŵ�ͼ����Ϣ
	virtual MAP_INFOR *					GetMapInfor(DWORD dwMapID);

	// ȡ�õ�ǰ��ͼ����
	virtual DWORD						GetMapCount(void);

	// ȡ������ĳһְҵ��ĳһ�������Ĺ�������ԴID
	// dwOccupation : ְҵ
	// dwPartID = ������
	virtual DWORD						GetNudePartForActor(DWORD dwOccupation, DWORD dwPartID, DWORD dwSex);

	// ȡ������ĳһְҵ��ĳһ�������Ĺ�������ԴID show
	// dwOccupation : ְҵ
	// dwPartID = ������
	virtual DWORD						GetNudePartForActor_Show(DWORD dwOccupation, DWORD dwPartID, DWORD dwSex);

	// ȡ��ĳ��������趨
	virtual MONSTER_CONFIG_INFOR &		GetMonsterInfor(long lMonsterID);

	// ȡ����Ʒ��������
	virtual GOODS_BASEPROPERTY &		GetGoodsBaseProperty(long lGoodsID);

	// ȡ��ҩƷ��������
	virtual MEDICAMENT_BASEPROPERTY &	GetMedicamentBaseProperty(long lGoodsID);

	// ȡ��װ����������
	virtual EQUIPMENT_BASEPROPERTY &	GetEquipmentBaseProperty(long lGoodsID, long lSmeltLevel);

	// ȡ��ƾ֤��������
	virtual WARRANT_BASEPROPERTY &		GetWarrantBaseProperty(long lGoodsID);

	// ȡ��Ԫ����������
	virtual VALUABLES_BASEPROPERTY &	GetValuablesBaseProperty(long lGoodsID);

	// ȡ������ĳһ�ȼ��ľ���
	virtual long						GetActorExp(long lLevel);

	// ȡ������ĳһְҵ�����ֵĹ����ͻ�������
	virtual void						GetFreeWeaponSoundID(DWORD dwOccupation, DWORD &dwAttackSoundID, DWORD &dwBeHitSoundID);

	// ͨ��ͷ��������ȡ�ø�������ͷ��ID
	virtual	DWORD						GetHeadResIDByIndex(DWORD dwIndex, DWORD dwType);

	// ͨ��ְҵ���Ա��ȡ�ɹ�ʹ�õ�ͷ��������
	virtual vector<DWORD>				GetHeadResIDByOccupation(DWORD dwOccupation, DWORD dwSex);

	// ͨ����ƷIDȡ����ص�����
	virtual DWORD						GetGoodsSoundID(DWORD dwGoodsID, DWORD dwSoundType);

	// ȡ���䷽��Ϣ
	virtual SECRET_INFOR *				GetSecretInfor(long lSecretID);

	// ȡ�þ���˥��, dwMasterLevel = ����ȼ�, nLevelDiff = �ȼ���
	virtual float						GetExpDegression(DWORD dwMasterLevel, int nLevelDiff);

	// ȡ�����ݵ����ò���
	virtual HORSE_CONFIG_INFOR *		GetHorseInfor(long lID);

	// ȡ����װ����Ч��
	virtual SUITEQUIP_EFFECT *			GetSuitEquipEffect(long lSuitEquipID);

	// ��ȡ����������ҫװ��
	virtual SCASTELLANGOWN *			GetCastellanGown(long lCastellanID, long lOccupation);
public:
	// ������ؽű�
	BOOL								LoadScript(void);

	CConfigDataCenter(void);

	~CConfigDataCenter(void);

private:
	// ��ͼ��Ϣ����
	CConfigMapInfor						m_ConfigMapInfor;

	/*
	// �����������ýű�
	CConfigProp							m_ConfigProp;

	// �����������
	CConfigCreatureNude					m_ConfigCreatureNude;

	// ������Ϣ���ýű�
	CConfigMonster						m_ConfigMonster;

	// ��Ʒ���ýű�
	CConfigGoodsProperty				m_ConfigGoodsProperty;

	// ���ﾭ��ű�
	CConfigActorExp						m_ConfigActorExp;

	// ����ͷ��ű�
	CConfigActorHeadRes					m_ConfigActorHeadRes;	

	// ��Ʒ�����ű�
	CConfigGoodsSound					m_ConfigGoodsSound;

	// �䷽�ű�
	CConfigSecret						m_ConfigSecret;

	// ����˥���ű�
	CConfigExpDegression				m_ConfigExpDegression;

	// �������ò���
	CConfigHorse						m_ConfigHorse;

	// ��װ�ű�
	CConfigSuitEquip					m_ConfigSuitEquip;

	// ������ҫ�ű�
	CConfigCastellanGown				m_ConfigCastellanGown;
	*/
};
//------------------------------------------------------------------------