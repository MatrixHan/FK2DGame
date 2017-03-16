/**
*	created:		2012-6-29   21:37
*	filename: 		CreaturePropertyDef
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������������ID,��������,����,NPC
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Macro.h"
//------------------------------------------------------------------------
/*
**
** ���һ���ԣ����޸����µط���
** 1���������ID
** 2����SACTOR_PROPERTY_LIST��������Ա�����
** 3����SMONSTER_PROPERTY_LIST��������Ա�����
** 4����prop_table.h����޸ķ�ʽ��
** 5��CActor::CActor��ӱ�����ַ����
** 6��CActor::OnSetDBContext��Ӵ����ݿ����
** 7��CActor::OnGetDBContext���ȡ���ݿ����
** 8��CMonster::CMonster��ӱ�����ַ����
** 9���ͻ���CActor::SetPropNum�޸�
** 10���ͻ���CActor::GetPropNum�޸�
** 11���ͻ���CActor::SetPropStr�޸�
** 12���ͻ���CActor::GetPropStr�޸�
** 13���ͻ���CHero::SetPropNum�޸�
** 14���ͻ���CHero::GetPropNum�޸�
** 15���ͻ���CHero::SetPropStr�޸�
** 16���ͻ���CHero::GetPropStr�޸�
** 17���ͻ���CMonster::SetPropNum�޸�
** 18���ͻ���CMonster::GetPropNum�޸�
** 19���ͻ���CMonster::SetPropStr�޸�
** 20���ͻ���CMonster::GetPropStr�޸�
*/
//------------------------------------------------------------------------
#pragma pack(1)	
//------------------------------------------------------------------------
// ����Ӧ�ñ�־
#define PROP_APPFLAG_NOSAVE				0x00000001	// �������ݿ� (Ĭ��Ϊ�����ݿ�)
#define PROP_APPFLAG_LOCK				0x00000002	// �������޸� (ĬΪΪ������)

// ���Ը��±�־
#define PROP_UPDATEFLAG_UPDATE			0x00000001	// �������Ը��� (���)
#define PROP_UPDATEFLAG_SYNC			0x00000002	// �������Թ㲥 (���)(����)
#define PROP_UPDATEFLAG_BIG				0x00000004	// ˽�����Ը��� (���)
#define PROP_UPDATEFLAG_SYNC_UPDATE     0x00000008	// �������Թ㲥 (���)(����)

// ��������Ӧ�ñ�־
#define ISPROPNOSAVE(flag)				((flag & PROP_APPFLAG_NOSAVE) == PROP_APPFLAG_NOSAVE ? TRUE : FALSE)
#define ISPROPLOCK(flag)				((flag & PROP_APPFLAG_LOCK) == PROP_APPFLAG_LOCK ? TRUE : FALSE)

// һ����ɫ��������Ʒ����
#define PACKET_OBJECT_INITCOUNT			36
#define PACKET_OBJECT_MAXCOUNT			60

// һ����ɫ�ֿ�����������
#define WAREHOUSE_INCLUDEPACKET_COUNT	7

// ��ݼ�BUF����
#define CREATURE_SHORTCUT_BUF_LEN		80
//------------------------------------------------------------------------
// ������������ID,��������,����,NPC
enum
{
	CREATURE_PROPID_NAME = 1,			// 1	= ����
	CREATURE_PROPID_LOC,				// 2	= λ�� 
	CREATURE_PROPID_SEX,				// 3	= �Ա�
	CREATURE_PROPID_LEVEL,				// 4	= �ȼ�
	CREATURE_PROPID_PHYLE,				// 5	= ����
	CREATURE_PROPID_OCCUPATION,			// 6	= ְҵ	
	
	CREATURE_PROPID_HPCUR,				// 7	= ��ǰ����ֵ	
	CREATURE_PROPID_HPMAXCUR,			// 8	= ��ǰ�������ֵ
	CREATURE_PROPID_MPCUR,				// 9	= ��ǰ����ֵ	
	CREATURE_PROPID_MPMAXCUR,			// 10	= ��ǰ�����ֵ

	CREATURE_PROPID_P_A_MINCUR,			// 11	= ��С��������(��ǰ)
	CREATURE_PROPID_P_A_MAXCUR,			// 12	= �����������(��ǰ)
	CREATURE_PROPID_M_A_MINCUR,			// 13	= ��С����������(��ǰ)
	CREATURE_PROPID_M_A_MAXCUR,			// 14	= �����������(��ǰ)

	CREATURE_PROPID_F_A_MIN,			// 15	= ��С����������
	CREATURE_PROPID_F_A_MAX,			// 16	= ������������	
	CREATURE_PROPID_I_A_MIN,			// 17	= ��С������������
	CREATURE_PROPID_I_A_MAX,			// 18	= ��������������	
	CREATURE_PROPID_E_A_MIN,			// 19	= ��С�編��������
	CREATURE_PROPID_E_A_MAX,			// 20	= ���編��������
	CREATURE_PROPID_N_A_MIN,			// 21	= ��С������������
	CREATURE_PROPID_N_A_MAX,			// 22	= ��󰵷���������

	CREATURE_PROPID_P_D_MINCUR,			// 23	= ��С���������(��ǰ)
	CREATURE_PROPID_P_D_MAXCUR,			// 24	= ������������(��ǰ)
	CREATURE_PROPID_M_D_MINCUR,			// 25	= ��С����������(��ǰ)
	CREATURE_PROPID_M_D_MAXCUR,			// 26	= �����������(��ǰ)

	CREATURE_PROPID_F_D_MIN,			// 27	= ��С����������
	CREATURE_PROPID_F_D_MAX,			// 28	= ������������
	CREATURE_PROPID_I_D_MIN,			// 29	= ��С������������
	CREATURE_PROPID_I_D_MAX,			// 30	= ��������������
	CREATURE_PROPID_E_D_MIN,			// 31	= ��С�編��������
	CREATURE_PROPID_E_D_MAX,			// 32	= ���編��������
	CREATURE_PROPID_N_D_MIN,			// 33	= ��С������������
	CREATURE_PROPID_N_D_MAX,			// 34	= ��󰵷���������

	CREATURE_PROPID_VERACITY,			// 35	= ������
	CREATURE_PROPID_HEDGE,				// 36	= ������
	CREATURE_PROPID_M_SUDDENLY,			// 37	= ����������
	CREATURE_PROPID_P_SUDDENLY,			// 38	= ��������	

	CREATURE_PROPID_WALKSTYLE,			// 39	= ��·��ʽ
	CREATURE_PROPID_MOVESPEEDLEVEL,		// 40	= �ƶ��ٶȵȼ�
	CREATURE_PROPID_ATTACKSPEEDLEVEL,	// 41	= �����ٶȵȼ�

	CREATURE_PROPID_P_A_EFF,			// 42	= ��������(����)
	CREATURE_PROPID_M_A_EFF,			// 43	= ����������(���ӣ�
	
	CREATURE_PROPID_P_D_EFF,			// 44	= ���������(���ӣ�
	CREATURE_PROPID_M_D_EFF,			// 45	= ����������(���ӣ�

	CREATURE_PROPID_P_SUDDENLY_EFF,		// 46	= ������������(���ӣ�
	CREATURE_PROPID_M_SUDDENLY_EFF,		// 47	= ��������������(���ӣ�	
	
	CREATURE_PROPID_P_DAMAGE_EFF,		// 48	= �����˺����յ���(���ӣ�	
	CREATURE_PROPID_M_DAMAGE_EFF,		// 49	= �����˺����յ���(���ӣ�	
	CREATURE_PROPID_ALL_DAMAGE_EFF,		// 50	= �����˺����յ���(���ӣ�	
	CREATURE_PROPID_ECHO_DAMAGE_EFF,	// 51	= �������ӷ����˺�(���ӣ�

	CREATURE_PROPID_VERACITY_EFF,		// 52	= ������(���ӣ�
	CREATURE_PROPID_HEDGE_EFF,			// 53	= ������(���ӣ�

	CREATURE_PROPID_HPMAX_EFF,			// 54	= �������ֵ(���ӣ�
	CREATURE_PROPID_MPMAX_EFF,			// 55	= �����ֵ(���ӣ�

	CREATURE_PROPID_CUREOUTPUT_PER_EFF,	// 56   = ҽ������ӳɰٷֱ�(����)
	CREATURE_PROPID_CUREINPUT_PER_EFF,	// 57   = ����ҽ�Ƽӳɰٷֱ�(����)
	CREATURE_PROPID_ALL_DAMAGE_PER_EFF,	// 58	= �����˺����յ����ٷֱ�(���ӣ�

	CREATURE_PROPID_DIR,				// 59   = ����

	CREATURE_PROPID_BASEHIGHID,			// 60	= ������߻�������ID		
	
	//////////////////////////////////////////////////////////
	/*********************** ���� ***************************/	
	CREATURE_PROPID_ACTORLOWID = 1000,	// 1000 = �������ר������ID
	CREATURE_PROPID_USERID,				// 1001 = UserID
	CREATURE_PROPID_ACTORID,			// 1002 = ActorID
	CREATURE_PROPID_HOLDMONEY_V1,		// 1003 = ���Ͻ�Ǯ
	CREATURE_PROPID_WAREMONEY_V1,		// 1004 = �ֿ��Ǯ
	CREATURE_PROPID_EXP_V1,				// 1005 = ����
	CREATURE_PROPID_PKVALUE,			// 1006 = PKֵ
	CREATURE_PROPID_BURTHENCUR,			// 1007 = ��ǰ����
	CREATURE_PROPID_BURTHENMAX,			// 1008 = �����

	CREATURE_PROPID_FORCEBASE,			// 1009 = ����(����)
	CREATURE_PROPID_FORCECUR,			// 1010 = ����(��ǰ)	
	CREATURE_PROPID_CONSTITBASE,		// 1011 = ����(����)
	CREATURE_PROPID_CONSTITCUR,			// 1012 = ����(��ǰ)
	CREATURE_PROPID_CELERITYBASE,		// 1013 = ����(����)
	CREATURE_PROPID_CELERITYCUR,		// 1014 = ����(��ǰ)
	CREATURE_PROPID_MENTALBASE,			// 1015 = ����(����)
	CREATURE_PROPID_MENTALCUR,			// 1016 = ����(��ǰ)
	CREATURE_PROPID_WISDOMBASE,			// 1017 = �ǻ�(����)
	CREATURE_PROPID_WISDOMCUR,			// 1018 = �ǻ�(��ǰ)

	CREATURE_PROPID_HPMAX_BASE,			// 1019 = �������ֵ(����)
	CREATURE_PROPID_MPMAX_BASE,			// 1020 = ���ħ��ֵ(����)

	CREATURE_PROPID_P_A_MINBASE,		// 1021 = ��С��������(����)
	CREATURE_PROPID_P_A_MAXBASE,		// 1022 = �����������(����)
	CREATURE_PROPID_M_A_MINBASE,		// 1023 = ��С����������(����)
	CREATURE_PROPID_M_A_MAXBASE,		// 1024 = �����������(����)
	CREATURE_PROPID_P_D_MINBASE,		// 1025 = ��С���������(����)
	CREATURE_PROPID_P_D_MAXBASE,		// 1026 = ������������(����)
	CREATURE_PROPID_M_D_MINBASE,		// 1027 = ��С����������(����)
	CREATURE_PROPID_M_D_MAXBASE,		// 1028 = �����������(����)

	CREATURE_PROPID_FORCE_EFF,			// 1029 = ����(����)
	CREATURE_PROPID_CONSTIT_EFF,		// 1030 = ����(����)
	CREATURE_PROPID_CELERITY_EFF,		// 1031 = ����(����)
	CREATURE_PROPID_MENTAL_EFF,			// 1032 = ����(����)
	CREATURE_PROPID_WISDOM_EFF,			// 1033 = �ǻ�(����)

	CREATURE_PROPID_P_A_MIN_EFF,		// 1034 = ��С��������(����)
	CREATURE_PROPID_P_A_MAX_EFF,		// 1035 = �����������(����)

	CREATURE_PROPID_F_A_EFF,			// 1036 = ����������(����)
	CREATURE_PROPID_I_A_EFF,			// 1037 = ������������(����)
	CREATURE_PROPID_E_A_EFF,			// 1038 = �編��������(����)	
	CREATURE_PROPID_N_A_EFF,			// 1039 = ������������(����)

	CREATURE_PROPID_F_D_EFF,			// 1040 = ����������(����)
	CREATURE_PROPID_I_D_EFF,			// 1041 = ������������(����)
	CREATURE_PROPID_E_D_EFF,			// 1042 = �編��������(����)
	CREATURE_PROPID_N_D_EFF,			// 1043 = ������������(����)

	CREATURE_PROPID_P_A_PER_EFF,		// 1044 = ���������ٷֱ�(����)
	CREATURE_PROPID_M_A_PER_EFF,		// 1045 = �����������ٷֱ�(����)
	CREATURE_PROPID_CURE_PER_EFF,		// 1046 = ����Ч�����Ӱٷֱ�(����)
	CREATURE_PROPID_CURE_EFF,			// 1047 = ����Ч������(����)
	CREATURE_PROPID_P_D_PER_EFF,		// 1048 = ����������ٷֱ�(����)
	CREATURE_PROPID_M_D_PER_EFF,		// 1049 = �����������ٷֱ�(����)	
	CREATURE_PROPID_HPMAX_PER_EFF,		// 1050 = �������ֵ�ٷֱ�(����)
	CREATURE_PROPID_MPMAX_PER_EFF,		// 1051 = �����ֵ�ٷֱ�(����)

	CREATURE_PROPID_P_A_MIN_EQUIP,		// 1052 = װ����С������
	CREATURE_PROPID_P_A_MAX_EQUIP,		// 1053 = װ�����������
	CREATURE_PROPID_M_A_MIN_EQUIP,		// 1054 = װ����С��������
	CREATURE_PROPID_M_A_MAX_EQUIP,		// 1055 = װ�����������
	CREATURE_PROPID_P_D_MIN_EQUIP,		// 1056 = װ����С������
	CREATURE_PROPID_P_D_MAX_EQUIP,		// 1057 = װ�����������
	CREATURE_PROPID_M_D_MIN_EQUIP,		// 1058 = װ����С��������
	CREATURE_PROPID_M_D_MAX_EQUIP,		// 1059 = װ�����������

	CREATURE_PROPID_EQUIPCONTAINERID,	// 1060 = װ��������ID
	CREATURE_PROPID_PACKETCONTAINERID,	// 1061 = ����������ID	

	CREATURE_PROPID_HEADRESID,			// 1062 = ����ͷ��
	CREATURE_PROPID_EXP_EFF,			// 1063 = ���鸽�ӣ�����ϵ����
	CREATURE_PROPID_NAMECOLOR,			// 1064 = ������ɫ
	CREATURE_PROPID_CLANID,				// 1065 = ����ID
	CREATURE_PROPID_GMPOWER,			// 1066 = GMȨ��
	CREATURE_PROPID_WARELEVEL,			// 1067 = �ֿ�ȼ�

	CREATURE_PROPID_HIDE,				// 1068 = �Ƿ�Ϊ����(0:������1:����)
	CREATURE_PROPID_CR_HIDE,			// 1069 = �Ƿ�Ϊ������(0:û������1:�з�����)

	CREATURE_PROPID_ACTORHIGHID,		// �������ר������ID

	/*********************** ���� ***************************/	
	CREATURE_PROPID_MONSTERLOWID = 2000,// 2000 = �������ר������ID
	CREATURE_PROPID_MONSTERID,			// 2001 = ����ID
	CREATURE_PROPID_MONSTEREYESHOT,		// 2002 = ������Ұ
	CREATURE_PROPID_MONSTERENMITY,		// 2003 = ������ֵ
	CREATURE_PROPID_MONSTERTRAILRADIUS,	// 2004 = ������ٰ뾶
	CREATURE_PROPID_MONSTERCAMP,		// 2005 = ������Ӫֵ
	CREATURE_PROPID_MONSTERHIGHID,		// �������ר������ID
	
	CREATURE_PROPID_CHUNKLOWID = 10000,	// 10000 = DataChunk���ר������ID
	CREATURE_PROPID_CHUNKHIGHID,		// DataChunk���ר������ID         
};
//------------------------------------------------------------------------
#define ACTOR_PRIVATE_PROP_INTERZONE	(CREATURE_PROPID_ACTORHIGHID - CREATURE_PROPID_ACTORLOWID)
#define MONSTER_PRIVATE_PROP_INTERZONE	(CREATURE_PROPID_MONSTERHIGHID - CREATURE_PROPID_MONSTERLOWID)
//------------------------------------------------------------------------
// ��  ����ACTOR�����Ա�
struct SACTOR_PROPERTY_PUBLIC
{
	char	szName[MAX_NAME_LEN - 15];	// ���֣�ȥ��15���ֽ���Ϊ���Ż�
	POINTS  ptLoc;						// λ�� 
	int		nSex;						// �Ա�
	int		nLevel;						// �ȼ�
	int		nOccupation;				// ְҵ
	
	int		nHPCur;						// ��ǰ����ֵ
	int		nHPMaxCur;					// ��ǰ�������ֵ
	int		nMPCur;						// ��ǰ����ֵ
	int		nMPMaxCur;					// ��ǰ�����ֵ
	
	int		nWalkStyle;					// ��·��ʽ
	int		nMoveSpeedLevel;			// �ƶ��ٶȵȼ�	
	
	int		nUserDBID;					// UserDBID
	int		nActorDBID;					// ActorDBID
	int		nPKValue;					// PKֵ	
	
	int		nHeadResID;					// ����ͷ��
	int		nCurActState;				// ��ǰ״̬	
	int		nNameColor;					// ������ɫ
	
	int		nClanID;					// ����ID
	
	int		nDir;						// ����
};
//------------------------------------------------------------------------
struct SACTOR_PROPERTY_PRIVATE : public SACTOR_PROPERTY_PUBLIC
{	
	int     nPhyle;						// ����

	int		nForceCur;					// ����(��ǰ)	
	int		nConstitCur;				// ����(��ǰ)	
	int		nCelerityCur;				// ����(��ǰ)	
	int		nMentalCur;					// ����(��ǰ)	
	int		nWisdomCur;					// �ǻ�(��ǰ)

	int		nP_A_MinCur;				// ��С��������(��ǰ)
	int		nP_A_MaxCur;				// �����������(��ǰ)
	int		nM_A_MinCur;				// ��С����������(��ǰ)
	int		nM_A_MaxCur;				// �����������(��ǰ)	
	int		nF_A_Min;					// ��С����������
	int		nF_A_Max;					// ������������
	int		nI_A_Min;					// ��С������������
	int		nI_A_Max;					// ��������������
	int		nE_A_Min;					// ��С�編��������
	int		nE_A_Max;					// ���編��������
	int		nN_A_Min;					// ��С������������
	int		nN_A_Max;					// ��󰵷���������

	int		nP_D_MinCur;				// ��С���������(��ǰ)
	int		nP_D_MaxCur;				// ������������(��ǰ)
	int		nM_D_MinCur;				// ��С����������(��ǰ)
	int		nM_D_MaxCur;				// �����������(��ǰ)
	int		nF_D_Min;					// ��С����������
	int		nF_D_Max;					// ������������
	int		nI_D_Min;					// ��С������������
	int		nI_D_Max;					// ��������������
	int		nE_D_Min;					// ��С�編��������
	int		nE_D_Max;					// ���編��������
	int		nN_D_Min;					// ��С������������
	int		nN_D_Max;					// ��󰵷���������

	int		nHoldMoney;					// ���Ͻ�Ǯ
	int		nWareMoney;					// �ֿ��Ǯ
	int		nExp;						// ����
	int		nBurthenCur;				// ��ǰ����
	int		nBurthenMax;				// �����
	
	int		nEquipContainerID;			// װ��������ID	
	int		nPacketContainerID;			// ����������ID

	char	szShortcutBuf[CREATURE_SHORTCUT_BUF_LEN];// �����buffer
};
//------------------------------------------------------------------------
struct SACTOR_PROPERTY_LIST : public SACTOR_PROPERTY_PRIVATE
{
	int		nForceBase;					// ����(����)
	int		nConstitBase;				// ����(����)
	int		nCelerityBase;				// ����(����)
	int		nMentalBase;				// ����(����)
	int		nWisdomBase;				// �ǻ�(����)

	int		nP_A_MinBase;				// ��С��������(����)
	int		nP_A_MaxBase;				// �����������(����)
	int		nM_A_MinBase;				// ��С����������(����)
	int		nM_A_MaxBase;				// �����������(����)
	int		nP_D_MinBase;				// ��С���������(����)
	int		nP_D_MaxBase;				// ������������(����)
	int		nM_D_MinBase;				// ��С����������(����)
	int		nM_D_MaxBase;				// �����������(����)

	int		nHPMax_Base;				// �������ֵ(����)
	int		nMPMax_Base;				// ���ħ��ֵ(����)	

	int		nVeracity;					// ������
	int		nHedge;						// ������
	int		nP_Suddenly;				// ��������	
	int		nM_Suddenly;				// ����������

	int     nForce_Eff;					// ����(����)
	int     nConstit_Eff;				// ����(����)
	int     nCelerity_Eff;				// ����(����)
	int     nMental_Eff;				// ����(����)
	int     nWisdom_Eff;				// �ǻ�(����)

	int     nP_A_Eff;					// ������������(����)
	int     nP_A_Per_Eff;				// ���������ٷֱ�(����)

	int     nP_A_Min_Eff;				// ��С��������(����)
	int     nP_A_Max_Eff;				// �����������(����)	

	int     nP_Suddenly_Eff;			// ������������(����)
	int     nM_Suddenly_Eff;			// ��������������(����)

	int     nF_A_Eff;					// ����������(����)
	int     nI_A_Eff;					// ������������(����)
	int     nE_A_Eff;					// �編��������(����)	
	int     nN_A_Eff;					// ������������(����)
	
	int     nM_A_Eff;					// ����������(����)
	int     nM_A_Per_Eff;				// �����������ٷֱ�(����)

	int     nCure_Eff;					// ����Ч������(����)
	int     nCure_Per_Eff;				// ����Ч�����Ӱٷֱ�(����)

	int     nF_D_Eff;					// ����������(����)
	int     nI_D_Eff;					// ������������(����)
	int     nE_D_Eff;					// �編��������(����)
	int     nN_D_Eff;					// ������������(����)

	int     nP_D_Eff;					// ���������(����)
	int     nP_D_Per_Eff;				// ����������ٷֱ�(����)

	int     nM_D_Eff;					// ����������(����)
	int     nM_D_Per_Eff;				// �����������ٷֱ�(����)

	int     nP_Damage_Eff;				// �����˺����յ���(����)	
	int     nM_Damage_Eff;				// �����˺����յ���(����)	
	int     nAll_Damage_Eff;			// �����˺����յ���(����)	

	int     nEcho_Damage_Eff;			// �������ӷ����˺�(����)

	int     nVeracity_Eff;				// �����ʰٷֱ�(����)
	int     nHedge_Eff;					// �����ʰٷֱ�(����)

	int     nHPMax_Eff;					// �������ֵ(����)
	int     nMPMax_Eff;					// �����ֵ(����)
	int     nHPMax_Per_Eff;				// �������ֵ�ٷֱ�(����)
	int     nMPMax_Per_Eff;				// �����ֵ�ٷֱ�(����)

	int		nP_A_Min_Equip;				// װ����С������
	int		nP_A_Max_Equip;				// װ�����������
	int		nM_A_Min_Equip;				// װ����С��������
	int		nM_A_Max_Equip;				// װ�����������
	int		nP_D_Min_Equip;				// װ����С�������
	int		nP_D_Max_Equip;				// װ������������
	int		nM_D_Min_Equip;				// װ����С��������
	int		nM_D_Max_Equip;				// װ�����������

	int		nExp_Eff;					// ���鸽�ӣ�����ϵ����

	int		nCureOutput_Per_Eff;		// ҽ������ӳɰٷֱ�(����)
	int		nCureInput_Per_Eff;			// ����ҽ�Ƽӳɰٷֱ�(����)
	int		nAll_Damage_Per_Eff;		// �����˺����յ����ٷֱ�(���ӣ�

	int		nGMPower;					// GMȨ��

	int		nAttackSpeedLevel;			// �����ٶȵȼ������õģ�
	int		nWareLevel;					// �ֿ�ȼ�

	int		nHide;						// �Ƿ�Ϊ����
	int		nCr_Hide;					// ������
};
//------------------------------------------------------------------------
// ��  ����MONSTER�����Ա�
struct SMONSTER_PROPERTY_PUBLIC
{
	char	szName[MAX_NAME_LEN];		// ����
	POINTS  ptLoc;						// λ�� 	

	int		nHPCur;						// ��ǰ����ֵ
	int		nMPCur;						// ��ǰ����ֵ

	int		nMonsterID;					// ����ID
	
	int		nWalkStyle;					// ��·��ʽ
	int		nMoveSpeedLevel;			// �ƶ��ٶȵȼ�	

	int		nCurActState;				// ��ǰ״̬

	int		nDir;						// ����

	int		nMonsterCamp;				// ������Ӫ
};
//------------------------------------------------------------------------
struct SMONSTER_PROPERTY_PRIVATE : public SMONSTER_PROPERTY_PUBLIC
{

};
//------------------------------------------------------------------------
struct SMONSTER_PROPERTY_LIST : public SMONSTER_PROPERTY_PRIVATE
{
	int		nLevel;						// �ȼ�
	int		nHPMaxCur;					// ��ǰ�������ֵ
	int		nMPMaxCur;					// ��ǰ�����ֵ

	int		nSex;						// �Ա�
	int     nPhyle;						// ����
	int		nOccupation;				// ְҵ

	int		nP_A_MinCur;				// ��С��������(��ǰ)
	int		nP_A_MaxCur;				// �����������(��ǰ)
	int		nM_A_MinCur;				// ��С����������(��ǰ)
	int		nM_A_MaxCur;				// �����������(��ǰ)
	
	int		nF_A_Min;					// ��С����������
	int		nF_A_Max;					// ������������
	int		nI_A_Min;					// ��С������������
	int		nI_A_Max;					// ��������������
	int		nE_A_Min;					// ��С�編��������
	int		nE_A_Max;					// ���編��������
	int		nN_A_Min;					// ��С������������
	int		nN_A_Max;					// ��󰵷���������

	int		nP_D_MinCur;				// ��С���������(��ǰ)
	int		nP_D_MaxCur;				// ������������(��ǰ)
	int		nM_D_MinCur;				// ��С����������(��ǰ)
	int		nM_D_MaxCur;				// �����������(��ǰ)

	int		nF_D_Min;					// ��С����������
	int		nF_D_Max;					// ������������
	int		nI_D_Min;					// ��С������������
	int		nI_D_Max;					// ��������������
	int		nE_D_Min;					// ��С�編��������
	int		nE_D_Max;					// ���編��������
	int		nN_D_Min;					// ��С������������
	int		nN_D_Max;					// ��󰵷���������

	int		nVeracity;					// ������
	int		nHedge;						// ������
	int		nP_Suddenly;				// ��������
	int		nM_Suddenly;				// ����������
	
	int     nP_A_Eff;					// ��������(����)
	int     nM_A_Eff;					// ����������(����)

	int     nP_D_Eff;					// ���������(����)
	int     nM_D_Eff;					// ����������(����)

	int     nP_Suddenly_Eff;			// ������������(����)
	int     nM_Suddenly_Eff;			// ��������������(����)

	int     nP_Damage_Eff;				// �����˺����յ���(����)	
	int     nM_Damage_Eff;				// �����˺����յ���(����)	
	int     nAll_Damage_Eff;			// �����˺����յ���(����)
	int     nEcho_Damage_Eff;			// �������ӷ����˺�(����)

	int     nVeracity_Eff;				// ������(����)
	int     nHedge_Eff;					// ������(����)

	int     nHPMax_Eff;					// �������ֵ(����)
	int     nMPMax_Eff;					// �����ֵ(����)

	int		nEyeShot;					// ������Ұ
	int		nEnmity;					// ������ֵ
	int     nTrailRadius;				// ������ٰ뾶

	int		nCureOutput_Per_Eff;		// ҽ������ӳɰٷֱ�(����)
	int		nCureInput_Per_Eff;			// ����ҽ�Ƽӳɰٷֱ�(����)
	int		nAll_Damage_Per_Eff;		// �����˺����յ����ٷֱ�(���ӣ�

	int		nAttackSpeedLevel;			// �����ٶȵȼ������õģ�
};
//------------------------------------------------------------------------
#pragma pack()
//------------------------------------------------------------------------