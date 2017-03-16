/**
*	created:		2012-6-29   20:00
*	filename: 		GoodsPropertyDef
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��Ʒ���Զ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <time.h>
//------------------------------------------------------------------------
#pragma pack(1)	
//------------------------------------------------------------------------
#define GOODS_CLASS_EQUIPMENT				1	// װ��
#define GOODS_CLASS_MEDICAMENT				2	// ҩƷ
#define GOODS_CLASS_WARRANT					3	// ƾ֤
#define GOODS_CLASS_VALUABLES				4	// Ԫ��

#define GOODS_EFFECT_MAXCOUNT				10	// һ����Ʒ����฽�����Ը���

#define EQUIPMENT_MAX_SMELTLEVEL			8	// װ����ྫ���ȼ�	

#define EQUIPMENT_MAX_SLOTNUM				3	// װ�����Ĳ����

#define EQUIPMENT_ENDURE_SHOWSPACE			256	// װ����ʾ�;� �� ʵ���;� / 256
//------------------------------------------------------------------------
// ��Ʒ�󶨱�־λ
enum
{
	GOODS_BINDFLAG_SETHOLD	= 0x00000001,		// �Ƿ����˻�ȡ��
	GOODS_BINDFLAG_SETUSE	= 0x00000002,		// �Ƿ�����ʹ�ð�
	GOODS_BINDFLAG_DIEDROP	= 0x00000004,		// ��������
	GOODS_BINDFLAG_DOFF		= 0x00000008,		// ����
	GOODS_BINDFLAG_TRADE	= 0x00000010,		// ��̯����
	GOODS_BINDFLAG_DESTROY	= 0x00000020,		// ����
	GOODS_BINDFLAG_SALE		= 0x00000040,		// ���۸�NPC
	GOODS_BINDFLAG_WAREHOUSE= 0x00000080,		// ���˲ֿ�
	GOODS_BINDFLAG_PACKET	= 0x00000100,		// ������
	GOODS_BINDFLAG_EQUIP	= 0x00000200,		// װ����
	GOODS_BINDFLAG_CLANWARE	= 0x00000400,		// ����ֿ�
};
//------------------------------------------------------------------------
// װ���������
enum
{
	EQUIPPOS_INDEX_HEADPIECE = 0,				// 0  = ͷ��	
	EQUIPPOS_INDEX_SCAPULAR,					// 1  = ���
	EQUIPPOS_INDEX_ARMATURE,					// 2  = ����
	EQUIPPOS_INDEX_PANTS,						// 3  = ����
	EQUIPPOS_INDEX_ACCOUTERMENT,				// 4  = ��Ʒ
	EQUIPPOS_INDEX_RESERVE1,					// 5  = ����1
	EQUIPPOS_INDEX_FINGERRING1,					// 6  = ��ָ1
	EQUIPPOS_INDEX_RIGHT_WEAPON,				// 7  = ��������
	EQUIPPOS_INDEX_LEFT_WEAPON,					// 8  = ��������	
	EQUIPPOS_INDEX_FINGERRING2,					// 9  = ��ָ2
	EQUIPPOS_INDEX_RESERVE2,					// 10 = ����2
	EQUIPPOS_INDEX_SHOES,						// 11 = Ь��
	EQUIPPOS_INDEX_CUFF,						// 12 = ����
	EQUIPPOS_INDEX_GLOVE,						// 13 = ����
	EQUIPPOS_INDEX_MANTEAU,						// 14 = ����
	EQUIPPOS_INDEX_NECKLACE,					// 15 = ����	
	EQUIPPOS_INDEX_MAX							// 16 = ���
};
//------------------------------------------------------------------------
// װ������
enum
{
	EQUIPMENT_SUBCLASS_HEADPIECE = 1,			// 1  = ͷ��	
	EQUIPMENT_SUBCLASS_SCAPULAR,				// 2  = ���
	EQUIPMENT_SUBCLASS_ARMATURE,				// 3  = ����
	EQUIPMENT_SUBCLASS_PANTS,					// 4  = ����
	EQUIPMENT_SUBCLASS_ACCOUTERMENT,			// 5  = ��Ʒ
	EQUIPMENT_SUBCLASS_FINGERRING,				// 6  = ��ָ
	EQUIPMENT_SUBCLASS_LEFT_WEAPON,				// 7  = ��������
	EQUIPMENT_SUBCLASS_RIGHT_WEAPON,			// 8  = ��������
	EQUIPMENT_SUBCLASS_SHOES,					// 9  = Ь��
	EQUIPMENT_SUBCLASS_CUFF,					// 10 = ����
	EQUIPMENT_SUBCLASS_GLOVE,					// 11 = ����
	EQUIPMENT_SUBCLASS_MANTEAU,					// 12 = ����
	EQUIPMENT_SUBCLASS_NECKLACE,				// 13 = ����
	EQUIPMENT_SUBCLASS_PAIR_WEAPON				// 14 = ˫������
};
//------------------------------------------------------------------------
// װ����;
enum
{
	EQUIPMENT_PURPOSE_WEAPON = 0,				// 0 : ����
    EQUIPMENT_PURPOSE_ARMOUR,					// 1 : ����
};
//------------------------------------------------------------------------
// װ������
enum
{
	EQUIPMENT_KIND_HEADPIECE = 1,				// 1  = ͷ��	
	EQUIPMENT_KIND_SCAPULAR,					// 2  = ���
	EQUIPMENT_KIND_ARMATURE,					// 3  = ����
	EQUIPMENT_KIND_PANTS,						// 4  = ����
	EQUIPMENT_KIND_ACCOUTERMENT,				// 5  = ��Ʒ
	EQUIPMENT_KIND_FINGERRING1,					// 6  = ��ָ
	EQUIPMENT_KIND_SHOES,						// 7  = Ь��
	EQUIPMENT_KIND_CUFF,						// 8  = ����
	EQUIPMENT_KIND_GLOVE,						// 9  = ����
	EQUIPMENT_KIND_MANTEAU,						// 10 = ����
	EQUIPMENT_KIND_NECKLACE,					// 11 = ����
	EQUIPMENT_KIND_FALCHION,					// 12 = ��
	EQUIPMENT_KIND_SWORD,						// 13 = ��
	EQUIPMENT_KIND_SPEAR,						// 14 = ǹ
	EQUIPMENT_KIND_AX,							// 15 = ��
	EQUIPMENT_KIND_BOW,							// 16 = ��
	EQUIPMENT_KIND_STAFF,						// 17 = ��
	EQUIPMENT_KIND_SHIELD						// 18 = ����
};
//------------------------------------------------------------------------
// ҩƷ����
enum
{
	MEDICAMENT_SUBCLASS_UNKNOWN = 0,			// 0  = δ֪��
	MEDICAMENT_SUBCLASS_MEDICAMENT,				// 1  = ҩƷ (����С����, ������...)
	MEDICAMENT_SUBCLASS_ROULEAU,				// 2  = ���� (�سǾ���...)
	MEDICAMENT_SUBCLASS_SEAL,					// 3  = ��ӡ (��ħ��...)
	MEDICAMENT_SUBCLASS_BOOK,					// 4  = ���� (����, ��ǶҪ��...)
	MEDICAMENT_SUBCLASS_SECRET,					// 5  = �䷽ (�����䷽...)
	MEDICAMENT_SUBCLASS_MAGICBOTTLE,			// 6  = ħƿ (����ƿ...)
	MEDICAMENT_SUBCLASS_FACIA,					// 7  = ����
	MEDICAMENT_SUBCLASS_VALUABLESGOLD,			// 8  = ��Ԫ��
	MEDICAMENT_SUBCLASS_VALUABLESSILVER,		// 9  = ��Ԫ��
	MEDICAMENT_SUBCLASS_VALUABLESCUPRUM,		// 10 = ͭԪ��
};
//------------------------------------------------------------------------
// ҩƷ���ö���
enum
{
	MEDICAMENT_ONTARGET_SELF = 0,				// 0  = ����
	MEDICAMENT_ONTARGET_SELELOC,				// 1  = ����λ��
	MEDICAMENT_ONTARGET_ASSIGNER,				// 2  = ָ������
	MEDICAMENT_ONTARGET_AREA,					// 3  = ָ��������
	MEDICAMENT_ONTARGET_TEAM,					// 4  = ����
	MEDICAMENT_ONTARGET_MARRIAGE,				// 5  = ����
	MEDICAMENT_ONTARGET_TEACH,					// 6  = ʦͽ	
};
//------------------------------------------------------------------------
// ҩƷ��������
enum
{
	MEDICAMENT_ONTYPE_STATE = 0,				// 0  = ״̬
	MEDICAMENT_ONTYPE_SKILL,					// 1  = ����
	MEDICAMENT_ONTYPE_LUA,						// 2  = lua
	MEDICAMENT_ONTYPE_SPECIAL,					// 3  = �ض���
};
//------------------------------------------------------------------------
enum
{
	GOODS_PROPID_GOODSID = 1,					// 1  = ��ƷID
	GOODS_PROPID_NAME,							// 2  = ����
	GOODS_PROPID_LOC,							// 3  = λ�� ����������������޷�ȡ��ֵ�ģ�
	GOODS_PROPID_NUMBER,						// 4  = ����
	GOODS_PROPID_ENDURE_CUR,					// 5  = ��ǰ�;�
	GOODS_PROPID_ENDURE_CURMAX,					// 6  = ��ǰ����;�	
	GOODS_PROPID_SMELTLEVEL,					// 7  = �����ȼ�
	GOODS_PROPID_SLOTLEFT						// 8  = ʣ������
};
//------------------------------------------------------------------------
// ƾ֤����
enum
{
	WARRANT_SUBCLASS_ORE = 1,					// 1  = ��ʯ
	WARRANT_SUBCLASS_HERBAGE,					// 2  = ��ҩ
	WARRANT_SUBCLASS_MATERIAL,					// 3  = ����
	WARRANT_SUBCLASS_STONE,						// 4  = ��ʯ
	WARRANT_SUBCLASS_WARRANT,					// 5  = ������Ʒ
	WARRANT_SUBCLASS_OTHER,						// 6  = ����
	WARRANT_SUBCLASS_DEBRIS,					// 7  = �к�
	WARRANT_SUBCLASS_RARE_ORE,					// 8  = ϡ�п�ʯ
	WARRANT_SUBCLASS_RARE_HERBAGE,				// 9  = ϡ�в�ҩ
	WARRANT_SUBCLASS_RARE_STONE,				// 10 = ϡ�о�ʯ
};
//------------------------------------------------------------------------
// ��  ������Ʒ�Ļ�������
struct SGOODS_PROPERTY_BASE
{
	DWORD	dwGoodsID;							// 1  = ��ƷID
	DWORD	dwNumber;							// 4  = ����
	DWORD	dwUseBind;							//      ʹ�ð�
	DWORD	dwUseLock;							//		ʹ������
};
//------------------------------------------------------------------------
// ��  ����װ��
struct SEQUIPMENT_PROPERTY_PUBLIC : public SGOODS_PROPERTY_BASE
{
	int		nEndureCur;							// 4  = ��ǰ�;�
	int		nEndureCurMax;						// 5  = ��ǰ����;�
	WORD	wEffect[GOODS_EFFECT_MAXCOUNT];		// 6  = ��������
	int		nSmeltLevel;						// 7  = �����ȼ�
	int 	nSlotLeft;							// 8  = ʣ������	
};
//------------------------------------------------------------------------
struct SEQUIPMENT_PROPERTY_PRIVATE : public SEQUIPMENT_PROPERTY_PUBLIC
{

};
//------------------------------------------------------------------------
struct SEQUIPMENT_PROPERTY_LIST : public SEQUIPMENT_PROPERTY_PRIVATE
{
	
};
//------------------------------------------------------------------------
// ��  ����ҩƷ
struct SMEDICAMENT_PROPERTY_PUBLIC : public SGOODS_PROPERTY_BASE
{
	BYTE	byExtend[20];						// ����չ��buf��ÿ����������������ݲ�һ��

	int		nValidFlag;							// ��Ч��0 = ��Ч��1 = ��Ч��
};
//------------------------------------------------------------------------
struct SMEDICAMENT_PROPERTY_PRIVATE : public SMEDICAMENT_PROPERTY_PUBLIC
{

};
//------------------------------------------------------------------------
struct SMEDICAMENT_PROPERTY_LIST : public SMEDICAMENT_PROPERTY_PRIVATE
{
	
};
//------------------------------------------------------------------------
struct SMEDICAMENT_EXTEND_FOR_MAGICBOTTLE
{
	time_t	tmStartTime;						// ��ʼ����ʱ��, 0��ʾδ��ʼ����
	time_t	tmCurrentTime;						// ��ǰʱ��ûʲô�ã���Ҫ������ʾ�ͻ���tooltip
	BYTE	btCountOnlineFlag;					// Ҫ��Ҫ��������ʱ��
	WORD	wOnlineTime;						// ����ʱ��, ����Ϊ��λ
	WORD	wSecretID;							// �䷽ID	
};
//------------------------------------------------------------------------
// ��  ����ƾ֤
struct SWARRAN_PROPERTY_PUBLIC : public SGOODS_PROPERTY_BASE
{
	WORD	wEffect[GOODS_EFFECT_MAXCOUNT];		// 6  = ��������
};
//------------------------------------------------------------------------
struct SWARRAN_PROPERTY_PRIVATE : public SWARRAN_PROPERTY_PUBLIC
{

};
//------------------------------------------------------------------------
struct SWARRAN_PROPERTY_LIST : public SWARRAN_PROPERTY_PRIVATE
{
	
};
//------------------------------------------------------------------------
// ��  ����Ԫ��
struct SVALUABLES_PROPERTY_PUBLIC : public SGOODS_PROPERTY_BASE
{
	
};
//------------------------------------------------------------------------
struct SVALUABLES_PROPERTY_PRIVATE : public SVALUABLES_PROPERTY_PUBLIC
{

};
//------------------------------------------------------------------------
struct SVALUABLES_PROPERTY_LIST : public SVALUABLES_PROPERTY_PRIVATE
{
	
};
//------------------------------------------------------------------------
#pragma pack()	
//------------------------------------------------------------------------