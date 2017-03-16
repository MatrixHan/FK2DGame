/**
*	created:		2012-6-29   20:02
*	filename: 		IConfigDataCenter
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������������,ͨ����ȡ�ű���ø��ַ�������ֵ����, Ȼ��ͨ��ȫ�ֵ�ʵ����ȡ	
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "GoodsPropertyDef.h"
#include "Macro.h"
#include <vector>
using std::vector;
//------------------------------------------------------------------------
// ����
struct IConfigProp;
//------------------------------------------------------------------------
// ����ͷ������
enum
{
	ACTORHEAD_RESTYPE_TARGET = 0,					// Ŀ�껷ͷ��
	ACTORHEAD_RESTYPE_RESUME,						// ���ҽ���ͷ��
	ACTORHEAD_RESTYPE_TEAM_COLOR,					// ���ͷ�񣨲�ɫ��
	ACTORHEAD_RESTYPE_TEAM_INVALID,					// ���ͷ�񣨻�ɫ��
	ACTORHEAD_RESTYPE_FRIEND_COLOR,					// ����ͷ�񣨲�ɫ��
	ACTORHEAD_RESTYPE_FRIEND_INVALID,				// ����ͷ�񣨻�ɫ��
};
//------------------------------------------------------------------------
// ��Ʒ��Ч����
enum
{
	GOODSSOUND_TYPE_SNATCH = 0,						// ��������
	GOODSSOUND_TYPE_DROP,							// ��������
	GOODSSOUND_TYPE_THROW,							// ��������
	GOODSSOUND_TYPE_USE,							// ʹ������
	GOODSSOUND_TYPE_ATTACK,							// ��������
	GOODSSOUND_TYPE_BEHIT,							// ��������
	GOODSSOUND_TYPE_MANGLE,							// ������
};
//------------------------------------------------------------------------
// "scp\\mapinfor.scp"
struct MAP_INFOR
{	
	long			napplyserver;					// Ӧ�úθ�������
	long			nopen;							// Ҫ��Ҫ����
	long			nmapid;							// ��ͼID	
	char			szmapname[MAX_PATH];			// ��ͼ����
	char			szmapfile[MAX_PATH];			// .map�ļ���
	char			szmaifile[MAX_PATH];			// .mai�ļ���
	char			szminimapfile[MAX_PATH];		// С��ͼ��
	char			szworldmapfile[MAX_PATH];		// �����ͼ��
	long			lusedynamic;					// �ͻ����Ƿ�Ҫ��̬����[Ĭ�� = 1]
	long			lminimaprate;					// С��ͼ����С����, [Ĭ�� = 20]
	long			lstaticrelivepoint[3];			// �ڱ���̬��ͼ����������(staticmap, loc.x, loc.y)
	long			ldynamicrelivepoint[3];			// �ڱ���̬��ͼ����������(staticmap, loc.x, loc.y)
	long			lredstaticrelivepoint[3];		// �����ڱ���̬��ͼ����������(staticmap, loc.x, loc.y)
	long			lreddynamicrelivepoint[3];		// �����ڱ���̬��ͼ����������(staticmap, loc.x, loc.y)
	char			szmonstercreaterfile[MAX_PATH];	// �����������ű�����������ͳ��NPC��
	char			szchunnelcreaterfile[MAX_PATH];	// �������������ű���
	char			szboxcreaterfile[MAX_PATH];		// �����������ű���
	long			lbackgroundmusic;				// ��������
	long			lmapsizetileX;					// ��ͼ��С(TILE)
	long			lmapsizetileY;					// ��ͼ��С(TILE)
	long			llandfallpoint[2];				// ��½�㣬���κ�ʱ�򣬶�����Ч�ģ���Ҫ�������������ߣ�λ�ó������ڴ�����
};
//------------------------------------------------------------------------
// "scp\\monster.csv"
// �������֧�ֵļ���
#define MONSTER_SUPPORT_SKILL		4
struct MONSTER_CONFIG_INFOR
{
	char			szmonstername[MAX_NAME_LEN];	// ����
	long			lmonsterid;						// ����ID
	long			lresid;							// ��ԴID	
	long			lheadresid;						// ͷ��ͼƬ
	long			lpalette;						// ��ɫ��	
	long			ltype;							// ����, 1 �� ��ͨ��, 2 �� ��Ӣ��, 3 �� ϡ�й�, 4 �� BOSS��������DThingGlobal.h
	long			lphyle;							// ����, 1 = ����, 2 = ����, 3 = ����, 4 = ��Ȼ, 5 = ����, 6 = Ұ�� 
	long			lcamp;							// ��Ӫ, 
	long			llevel;							// �ȼ�
	long			lhp;							// ����
	long			lmp;							// ħ��
	long			lexp;							// ����ֵ
	long			lexpcoefficie;					// ��������ϵ��
	long			lmovespeed;						// �ƶ��ٶ�
	long			lattackspeed;					// �����ٶ�
	long			lmindam;						// ��С������
	long			lmaxdam;						// ���������
	long			lminmagdam;						// ��С��������
	long			lmaxmagdam;						// ���������
	long			lmindefend;						// ��С�������
	long			lmaxdefend;						// ����������
	long			lminmagdefend;					// ��С��������
	long			lmaxmagdefend;					// ���������
	int				lP_Suddenly;					// ��������
	int				lM_Suddenly;					// ����������
	long			leyeshot;						// ��Ұ
	long			lhit;							// ����
	long			ldodge;							// ����
	long			lfireresist;					// ��������
	long			lfrozenresist;					// ����������
	long			llightresist;					// �編������
	long			lpoisonresist;					// ����������
	long			luseskill[MONSTER_SUPPORT_SKILL * 3];// ʹ�ü��� [����id, ʹ�ø���, ʹ��ʱ��]
	long			lprizeholdstrategy;				// ����ӵ�в���
};
//------------------------------------------------------------------------
// ÿ����Ʒ������Ļ�������
struct GOODS_BASEPROPERTY
{
	long			lGoodsID;						// ��ƷID
	char			szName[MAX_NAME_LEN];			// ��Ʒ����
	long			lClass;							// ��Ʒ���ࣨ1 = װ�� 2 = ҩƷ 3 = ƾ֤ 4 = Ԫ����
	long			lSubClass;						// ��Ʒ����
	long			lAllowPhyle;					// ʹ������
	long			lAllowOccupation;				// ʹ��ְҵ
	long			lAllowLevel;					// ʹ�õȼ�
	long			lWeight;						// ����
	long			lPileNum;						// ��������
	long			lBuyPrice;						// ��������۸�
	long			lSellPrice;						// ���������۸�	
	long			lHoldNum;						// ���ӵ�и���0��������
	long			lRank;							// ������Ʒ����
	long			lIsShortcut;					// �ܷ�ſ����
	long			lHoldBind;						// ��ȡ��
	long			lUseBind;						// ʹ�ð�
	long			lBindFlag;						// �󶨱�־λ
	char			szDesc[256];					// ����
	long			lArticleID_z;					// ��Ʒ��ͼƬ��������
	long			lArticleID_g;					// ��Ʒ��ͼƬ��������
	long			lArticleID_w;					// ��Ʒ��ͼƬ����Ч��
	long			lPopedomTime;					// ���ڵ��ϵ�Ȩ��ʱ��
	long			lDestroyTime;					// ���ڵ��ϵ�����ʱ��
	long			lShowToTeam;					// ʰ����ͬ����㲥(0�����㲥��1���㲥)
	long			lShowToMap;						// ʰ����ͬ��ͼ�㲥(0�����㲥��1���㲥)
	long			lShowToWorld;					// ʰ����ͬ��Ϸ�㲥(0�����㲥��1���㲥)
	long			lShowToClan;					// ʰ����ͬ����㲥(0�����㲥��1���㲥)
};
//------------------------------------------------------------------------
// "scp\\Equipment.cvs"
struct EQUIPMENT_BASEPROPERTY : public GOODS_BASEPROPERTY
{
	long			lEquipKind;						// װ������
	long			lActionMode;					// ��������
	long			lEquipPos1;						// ��װ����λ��1����Ч��-1
	long			lEquipPos2;						// ��װ����λ��2����Ч��-1
	long			lEquipPos3;						// ��װ����λ��3����Ч��-1
	long			lAppraisePrice;					// ��������
	long			lAttackSpeed;					// �����ٶ�
	long			lP_AttackMin;					// ��������������Сֵ
	long			lP_AttackMax;					// ���������������ֵ
	long			lP_DefendMin;					// ���������������Сֵ
	long			lP_DefendMax;					// ����������������ֵ
	long			lM_AttackMin;					// ���ӷ�����������Сֵ
	long			lM_AttackMax;					// ���ӷ������������ֵ
	long			lM_DefendMin;					// ���ӷ�����������Сֵ
	long			lM_DefendMax;					// ���ӷ������������ֵ
	long			lEndureMax;						// ����;�����
	long			lEffectMax;						// �������������
	long			lAniID1_partid;					// ����ID
	long			lAniID1_m;						// ������1
	long			lAniID1_w;						// Ů����1
	long			lAniID2_partid;					// ����ID
	long			lAniID2_m;						// ������2
	long			lAniID2_w;						// Ů����2
	long			lAniID3_partid;					// ����ID
	long			lAniID3_m;						// ������3
	long			lAniID3_w;						// Ů����3
	long			lAniID4_partid;					// ����ID
	long			lAniID4_m;						// ������4
	long			lAniID4_w;						// Ů����4

	long			lAniID1_show_partid;			// ����ID
	long			lAniID1_show_m;					// ������1
	long			lAniID1_show_w;					// Ů����1
	long			lAniID2_show_partid;			// ����ID
	long			lAniID2_show_m;					// ������2
	long			lAniID2_show_w;					// Ů����2
	long			lAniID3_show_partid;			// ����ID
	long			lAniID3_show_m;					// ������3
	long			lAniID3_show_w;					// Ů����3
	long			lAniID4_show_partid;			// ����ID
	long			lAniID4_show_m;					// ������4
	long			lAniID4_show_w;					// Ů����4

	long			lPalette_m_s[EQUIPMENT_MAX_SMELTLEVEL];	// ���� �е�ɫ��
	long			lPalette_w_s[EQUIPMENT_MAX_SMELTLEVEL];	// ���� Ů��ɫ��

	long			lPurposeFlag;					// ��;, �������Ƿ���

	long			lSuitEquipID;					// ��װID
};
//------------------------------------------------------------------------
// "scp\\Medicament.cvs"
struct MEDICAMENT_BASEPROPERTY : public GOODS_BASEPROPERTY
{
	long			lUseNum;						// ʹ�ô���0�����޴�ʹ��
	long			lReactID;						// ����ID
	long			lSecretID;						// �䷽ID
};
//------------------------------------------------------------------------
// "scp\\Warrant.cvs"
struct WARRANT_BASEPROPERTY : public GOODS_BASEPROPERTY
{

};
//------------------------------------------------------------------------
// "scp\\Valuables.cvs"
struct VALUABLES_BASEPROPERTY : public GOODS_BASEPROPERTY
{

};
//------------------------------------------------------------------------
// "scp\\secret.res"
struct SECRET_INFOR
{
	struct STUFF_INFOR
	{
		long		lGoodsID;						// ��Ʒid
		long		lNum;							// ��������
		char		szSource[128];					// ��ȡ��ʽ
	};

	long			lSecretID;						// �䷽id
	char			szCondition[256];				// ����lua����
	
	long			lMagicBottleGoodsID;			// ����ƿ��ƷID
	char			szMagicBottleSource[128];		// ����ƿ��ȡ��ʽ

	long			lStuffTotal;					// ԭ������������
	STUFF_INFOR		StuffInfor[20];					// ԭ������Ϣ
	
	long			lEpurateTime;					// ����ʱ�䣬����
	char			szEpurateResult[128];			// �������˵��
	char			szEpurateLuaFunction[256];		// �����ɹ�ִ�е�lua����

	long			lCountOnlinePrecision;			// ͳ������ʱ�侫��
};
//------------------------------------------------------------------------
// "scp\\horse.cse"
struct HORSE_CONFIG_INFOR
{
	long			lid;							// ID
	char			szhorsename[MAX_NAME_LEN];		// ����
	long			lallowlevel;					// ��Ҫ�ȼ�
	long			lmovespeed;						// �ƶ��ٶȵ���
	long			lpart1_no;						// ����1_�ţ�����װ�ںθ�λ��
	long			lpart1_resid;					// ����1_��Դid
	long			lpart1_pal;						// ����1_��ɫ��
	long			lpart2_no;						// ����2_�ţ�����װ�ںθ�λ��
	long			lpart2_resid;					// ����2_��Դid
	long			lpart2_pal;						// ����2_��ɫ��
	char			szdesc[256];					// ����
};
//------------------------------------------------------------------------
// "scp\\suitequip.res"
struct SUITEQUIP_EFFECT
{
	long			lID;							// ��װID
	char			szName[MAX_NAME_LEN];			// ��װ����
	long			lMaxPiece;						// ��װ������
	long			lEffect[EQUIPPOS_INDEX_MAX][12];// ��װ��������
};
//------------------------------------------------------------------------
// "data\\scp\\CastellanGown.cse"
struct SCASTELLANGOWN
{
	long			lManteauPartID;					// ���粿��id
	long			lManManteauResID;				// �г��������Ա������Դid
	long			lManManteauPalID;				// �г��������Ա�����ɫ��id
	long			lWomanManteauResID;				// Ů���������Ա������Դid
	long			lWomanManteauPalID;				// Ů���������Ա�����ɫ��id
	long			lHorsePartID1;					// ������Ĳ���1id
	long			lManHorsePartResID1;			// �г�������1����Դid
	long			lManHorsePartPalID1;			// �г�������1�ĵ�ɫ��id
	long			lWomanHorsePartResID1;			// Ů��������1����Դid
	long			lWomanHorsePartPalID1;			// Ů��������1�ĵ�ɫ��id
	long			lHorsePartID2;					// ������Ĳ���2id
	long			lManHorsePartResID2;			// �г�������2����Դid 
	long			lManHorsePartPalID2;			// �г�������2�ĵ�ɫ��id
	long			lWomanHorsePartResID2;			// Ů��������2����Դid
	long			lWomanHorsePartPalID2;			// Ů��������2�ĵ�ɫ��id
};
//------------------------------------------------------------------------
// ����
struct IConfigDataCenter
{
	// �������Ե�����
	virtual IConfigProp *				GetConfigProp(void) = NULL;

	// ȡ�����е�ͼ����Ϣ, IN : pInfor �����ɵ�MAP_INFOR����, OUT : ��ǰ�ж����ŵ�ͼ
	virtual BOOL						GetMapInfor(MAP_INFOR * pInfor, DWORD &dwCount) = NULL;
	
	// ȡ��ĳһ�ŵ�ͼ����Ϣ
	virtual MAP_INFOR *					GetMapInfor(DWORD dwMapID) = NULL;
	
	// ȡ�õ�ǰ��ͼ����
	virtual DWORD						GetMapCount(void) = NULL;

	// ȡ������ĳһְҵ��ĳһ�������Ĺ�������ԴID
	// dwOccupation : ְҵ
	// dwPartID = ������
	virtual DWORD						GetNudePartForActor(DWORD dwOccupation, DWORD dwPartID, DWORD dwSex) = NULL;

	// ȡ������ĳһְҵ��ĳһ�������Ĺ�������ԴID show
	// dwOccupation : ְҵ
	// dwPartID = ������
	virtual DWORD						GetNudePartForActor_Show(DWORD dwOccupation, DWORD dwPartID, DWORD dwSex) = NULL;

	// ȡ��ĳ��������趨
	virtual MONSTER_CONFIG_INFOR &		GetMonsterInfor(long lMonsterID) = NULL;

	// ȡ����Ʒ��������
	virtual GOODS_BASEPROPERTY &		GetGoodsBaseProperty(long lGoodsID) = NULL;

	// ȡ��ҩƷ��������
	virtual MEDICAMENT_BASEPROPERTY &	GetMedicamentBaseProperty(long lGoodsID) = NULL;

	// ȡ��װ����������
	virtual EQUIPMENT_BASEPROPERTY &	GetEquipmentBaseProperty(long lGoodsID, long lSmeltLevel) = NULL;

	// ȡ��ƾ֤��������
	virtual WARRANT_BASEPROPERTY &		GetWarrantBaseProperty(long lGoodsID) = NULL;

	// ȡ��Ԫ����������
	virtual VALUABLES_BASEPROPERTY &	GetValuablesBaseProperty(long lGoodsID) = NULL;

	// ȡ������ĳһ�ȼ��ľ���
	virtual long						GetActorExp(long lLevel) = NULL;

	// ȡ������ĳһְҵ�����ֵĹ����ͻ�������
	virtual void						GetFreeWeaponSoundID(DWORD dwOccupation, DWORD &dwAttackSoundID, DWORD &dwBeHitSoundID) = NULL;

	// ͨ��ͷ��������ȡ�ø�������ͷ��ID
	virtual	DWORD						GetHeadResIDByIndex(DWORD dwIndex, DWORD dwType) = NULL;

	// ͨ��ְҵ���Ա��ȡ�ɹ�ʹ�õ�ͷ��������
	virtual vector<DWORD>				GetHeadResIDByOccupation(DWORD dwOccupation, DWORD dwSex) = NULL;

	// ͨ����ƷIDȡ����ص�����
	virtual DWORD						GetGoodsSoundID(DWORD dwGoodsID, DWORD dwSoundType) = NULL;

	// ȡ���䷽��Ϣ
	virtual SECRET_INFOR *				GetSecretInfor(long lSecretID) = NULL;

	// ȡ�þ���˥��, dwMasterLevel = ����ȼ�, nLevelDiff = �ȼ���
	virtual float						GetExpDegression(DWORD dwMasterLevel, int nLevelDiff) = NULL;

	// ȡ�����ݵ����ò���
	virtual HORSE_CONFIG_INFOR *		GetHorseInfor(long lID) = NULL;

	// ȡ����װ����Ч��
	virtual SUITEQUIP_EFFECT *			GetSuitEquipEffect(long lSuitEquipID) = NULL;

	// ��ȡ����������ҫװ��
	virtual SCASTELLANGOWN *			GetCastellanGown(long lCastellanID, long lOccupation) = NULL;
};
//------------------------------------------------------------------------