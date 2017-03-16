/**
*	created:		2012-6-29   15:26
*	filename: 		ThingDef
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
// thing����
enum
{
	THINGCLASS_NONE	= 0,			// 0 = δ��ȷ
	THINGCLASS_ACTOR,				// 1 = ����
	THINGCLASS_MONSTER,				// 2 = ����
	THINGCLASS_CONTAINER,			// 3 = ����
	THINGCLASS_GOODS,				// 4 = ��Ʒ
	THINGCLASS_CHUNNEL,				// 5 = ��ͼͨ��
	THINGCLASS_BOX,					// 6 = ����
	THINGCLASS_STALL,				// 7 = ̯λ
	THINGCLASS_MAX					// ���
};
//------------------------------------------------------------------------
// ����ְҵ
enum
{
	ACTOR_OCCUPATION_NONE = 0,		// 0 = δ��ȷ��
	ACTOR_OCCUPATION_WARRIOR,		// 1 = սʿ
	ACTOR_OCCUPATION_RABBI,			// 2 = ��ʦ
	ACTOR_OCCUPATION_PRIEST,		// 3 = ��ʦ
	ACTOR_OCCUPATION_NECROMANCER,	// 4 = ����
	ACTOR_OCCUPATION_MAX
};
//------------------------------------------------------------------------
// ����
enum
{
	CREATURE_PHYLE_NONE = 0,		// 0 = δ��ȷ��
	CREATURE_PHYLE_HUMAN,			// 1 = ����
	CREATURE_PHYLE_PERI,			// 2 = ����
	CREATURE_PHYLE_PSYCHE,			// 3 = ����
	CREATURE_PHYLE_NATURE,			// 4 = ��Ȼ
	CREATURE_PHYLE_ORGAN,			// 5 = ����
	CREATURE_PHYLE_WILDANIMAL,		// 6 = Ұ��
	CREATURE_PHYLE_MAX
};
//------------------------------------------------------------------------
// ��������
enum
{
	MONSTER_TYPE_NONE = 0,			// 0 = δ��ȷ��
	MONSTER_TYPE_GENERAL,			// 1 = ��ͨ��
	MONSTER_TYPE_EXCELLENT,			// 2 = ��Ӣ��
	MONSTER_TYPE_RARE,				// 3 = ϡ�й�
	MONSTER_TYPE_BOSS,				// 4 = BOSS
	MONSTER_TYPE_BEASTIE,			// 5 = ��Ȼ��Ĺ��Сè��С���ȣ�
	MONSTER_TYPE_SAFEGUARD,			// 6 = ������ٻ���
	MONSTER_TYPE_HATCHETMAN,		// 7 = �������ٻ���
	MONSTER_TYPE_NOATTACKNPC,		// 8 = ���ɹ�����NPC
	MONSTER_TYPE_ATTACKNPC,			// 9 = �ɹ���NPC
	MONSTER_TYPE_GUARD,				// 10 = ����
	MONSTER_TYPE_MPAWN_GEN,			// 11 = �����ٻ��ޣ���ͨAI��
	MONSTER_TYPE_MPAWN_HIGH,		// 12 = �����ٻ��ޣ�����AI��
	MONSTER_TYPE_ATTACK_TOTEM,		// 13 = �ɹ�����ͼ��
	MONSTER_TYPE_SAFE_TOTEM,		// 14 = ���ɹ�����ͼ��
	MONSTER_TYPE_BATTLE_BEAST,		// 15 = ս����(���ڳ�ս)
};
//------------------------------------------------------------------------
// �����Ա�
enum
{
	ACTOR_SEX_MAN = 1,				// 1 = ��
	ACTOR_SEX_WOMAN,				// 2 = Ů
	ACTOR_SEX_OTHER,				// 3 = ����
};
//------------------------------------------------------------------------
// �����ƶ�����
enum
{
	CREATURE_MOVESTYLE_WALK = 0,	// 0 = ��·
	CREATURE_MOVESTYLE_RUN,			// 1 = �ܲ�
	CREATURE_MOVESTYLE_SIT,			// 2 = ����	

	CREATURE_MOVESTYLE_RACE,		// 3 = ���� ���������Ч��
};
//------------------------------------------------------------------------
// ����ID
enum
{
	PRIZEHOLD_STRATEGY_SIMPLE = 0,	// �򵥲���
	PRIZEHOLD_STRATEGY_PAWN,		// צ�����ԣ�����ʲô�ô���û��
	PRIZEHOLD_STRATEGY_LAST,		// ���һ�ι���Ȩ
	PRIZEHOLD_STRATEGY_CONTINUAL,	// ��������Ȩ
	PRIZEHOLD_STRATEGY_BORN,		// ��������
};
//------------------------------------------------------------------------