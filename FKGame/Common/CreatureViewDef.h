/**
*	created:		2012-6-29   19:49
*	filename: 		CreatureViewDef
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������صĽӿڡ��������Ϣ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#define		MAX_CREATURE_TYPES		40	//�����������
#define		MAX_PAL_TYPES			15	//����ɫ������

#define		MAN_CREATURE_TYPE		0	//��׼�����е�type
#define		FEMALE_CREATURE_TYPE	1	//��׼����Ů��type

#define		MAX_ACTOR_NUM			3	//һ���˺��ܴ���������ɫ��
//------------------------------------------------------------------------
class CCreatureView;
//------------------------------------------------------------------------
struct SSkillCreateParam
{
	SSkillCreateParam()
	{
		memset(this, 0, sizeof(SSkillCreateParam));
	}

	// ��ͼģ�����ı���
	POINT	ptTile;						// ������ʼ��(��������Ľŵ�TILE�������TILE��)
	int		nAniType;					// �������ͣ��ο�enum AniType
	
	// �߼�ģ����Ҫ���ı���
	int		nIndex;						// ������(����ID)
	int		nLevel;						// ���ܵȼ�(���ܶ����õ�)
	int		nCheckId;					// ���ܵĽϼ�ID��Ψһȷ��
	POINT	ptAttack;					// ������(�е������������TILE���е���������������TILE)

	CCreatureView	*pAttackView;		// �������ߵ�ָ��

	DWORD	dwReserve1;					// ����1
	DWORD	dwReserve2;					// ����2
	DWORD	dwReserve3;					// ����3
	DWORD	dwReserve4;					// ����4
	
};
//------------------------------------------------------------------------
// ƮѪ����
enum enFlowType
{
	FlowTypeCommon = 0,					// Ʈ��������
	FlowTypeCrazy,						// Ʈ����
	FlowTypeResist,						// Ʈ�ֿ�
	FlowTypeHedge,						// Ʈ����
};
//------------------------------------------------------------------------
// ƮѪ��ħ��������
struct SFlowParam
{
	SFlowParam()
	{
		memset(this, 0, sizeof(SFlowParam));
	}
	enFlowType	nFlowType;				// Ʈ������
	int			nHP;					// ƮHP
};
//------------------------------------------------------------------------
// ���ﱻ��ʱ����Ĳ���
struct SBehitParam
{
	SBehitParam()
	{
		memset(this, 0, sizeof(SBehitParam));
	}

	int				nCheckId;			// ���ܵĽϼ�ID��Ψһȷ��
	BOOL			bDoAct;				// �Ƿ�Ҫ����������
	SFlowParam		FlowParam;			// ƮѪ��ħ��������
	CCreatureView	*pSourceView;		// �����ߵ�ָ��
};
//------------------------------------------------------------------------
// ��ɫ�ƶ���ʽ
enum enActorMoveStyle
{
	ActMoveStyleWalk = 0,				// ����
	ActMoveStyleRun,					// �ܲ�
	ActMoveStyleRide,					// ������
};
// ��ɫЯ�������ķ�ʽ
enum enActorWeaponStyle
{
	ActWeaponStyleSingle = 0,			// �����������������֣�
	ActWeaponStyleDouble,				// ˫������
	ActWeaponStyleBow,					// ������
	
	ActWeaponStyleMax,
};
//------------------------------------------------------------------------
// NPC����Ӧ��ͷ����Ч
enum enNpcType
{
	NpcTypeNone = 0,					// ���趥��Ч
	NpcTypeTrans,						// ħ����	������Ա��
	NpcTypeSkill,						// ħ����	������ѧϰʦ��
	NpcTypeMedicinal,					// ��ҩƿ	��ҩƷ�̣�
	NpcTypeWeapon,						// ����		�������̣�
	NpcTypeArmor,						// ����		��װ���̣�
	NpcTypeRide,						// ��		������̣�
	NpcTypeSmith,						// ����		��������
	NpcTypeGrocer,						// ����		���ӻ��̣�
	
	NpcTypeMax,
};

// ����NPCͷ���Ĺ�Ч
enum enTaskType
{
	TaskTypeNone = 0,					// ���趥��Ч
	TaskType_Get_Can,					// С����-��		�������񣬲��������������
	TaskType_Get_NoCanGet,				// С����-��		�������񣬵�δ�������������
	TaskType_Give_Complete,				// �պ�С����-��	���пɽ������񣬲�����ɣ�
	TaskType_Give_NoComplete,			// �պ�С����-��	���пɽ������񣬵���δ��ɣ�
	
	TaskTypeMax,
};
//------------------------------------------------------------------------
// ��ɫ�������(�����ڽ�ɫ�����NPC)
enum enCreaturePart
{
	CrtPartHead = 0,					//0		ͷ
	CrtPartHeadEffect,					//1		ͷ��Ч
	CrtPartUpBody,						//2		�ϰ���
	CrtPartUpBodyEffect,				//3		�ϰ�����Ч
	CrtPartDownBody,					//4		�°���
	CrtPartDownBodyEffect,				//5		�°�����Ч
	CrtPartBody,						//6		���壨Ҫע��ָ����������
	CrtPartBodyEffect,					//7		������Ч��Ҫע��ָ����������
	CrtPartLeftShoulder,				//8		���
	CrtPartLeftShoulderEffect,			//9		�����Ч
	CrtPartRightShoulder,				//10	�Ҽ�
	CrtPartRightShoulderEffect,			//11	�Ҽ���Ч
	CrtPartLeftHand,					//12	����
	CrtPartLeftHandEffect,				//13	������Ч
	CrtPartRightHand,					//14	����
	CrtPartRightHandEffect,				//15	������Ч
	CrtPartLeftEquip,					//16	����װ��
	CrtPartLeftEquipEffect,				//17	����װ����Ч
	CrtPartRightEquip,					//18	����װ��
	CrtPartRightEquipEffect,			//19	����װ����Ч
	CrtPartBothHandsEquip,				//20	˫��װ��
	CrtPartBothHandsEquipEffect,		//21	˫��װ����Ч
	CrtPartBackEquip,					//22	����װ��
	CrtPartBackEquipEffect,				//23	����װ����Ч
	CrtPartTail,						//24	β��
	CrtPartTailEffect,					//25	β����Ч
	CrtPartWing,						//26	���
	CrtPartWingEffect,					//27	�����Ч
	CrtPartWholeBody,					//28	ȫ����
	CrtPartWHoleBodyEffect,				//29	ȫ������Ч
	CrtPartExternsion,					//30	����
	CrtPartExternsionEffect,			//31	������Ч

	CrtPartMax = 32,					//32	������󲿼���
};
//------------------------------------------------------------------------
// ��ɫ״̬
enum enCreatureState
{
	CrtStateStand = 0,					//0		վ��
	CrtStateWalk,						//1		����
	CrtStateRun,						//2		����
	CrtStateDie,						//3		����
	CrtStateFallow,						//4		����
	CrtStateBehit,						//5		����
	CrtStateSit,						//6		����
	CrtStateJuqi1,						//7		����1
	CrtStateJuqi2,						//8		����2
	CrtStateMagic1,						//9		ʩ��1
	CrtStateMagic2,						//10	ʩ��2
	CrtStateSingleAttack1,				//11	������������1
	CrtStateSingleAttack2,				//12	������������2
	CrtStateSingleAttack3,				//13	������������3
	CrtStateDoubleAttack1,				//14	˫����������1
	CrtStateDoubleAttack2,				//15	˫����������2
	CrtStateArrowAttack,				//16	������������
	CrtStateRideStand,					//17	����վ��
	CrtStateRideRun,					//18	������
	CrtStateCommonAttack1,				//19	��ͨ����1
	CrtStateCommonAttack2,				//20	��ͨ����2
	CrtStateSkillAttack1,				//21	���ܹ���1
	CrtStateSkillAttack2,				//22	���ܹ���2

	CrtStateMax = 23,					//23	�������״̬��
};
//------------------------------------------------------------------------
// ���﷽��
enum enCreatureDir
{
	CrtDirNorth = 0,					// ��
	CrtDirEastNorth,					// ����
	CrtDirEast,							// ��
	CrtDirEastSouth,					// ����
	CrtDirSouth,						// ��
	CrtDirWestSouth,					// ����
	CrtDirWest,							// ��
	CrtDirWestNorth,					// ����

	CrtDirMaxDirCount = 8,				// ������������Ŀ
};
//------------------------------------------------------------------------
// ��������Զ���(�����"���û򷵻�"��ʾ������OnCommandʱdwParam2���������û򷵻ص���������)
enum enCreatureProp
{
	CrtPropNull = 0,					// ������
	CrtPropMoveStyle,					// �ƶ�����						���û򷵻�: DWORD* (ActMoveStyleWalk or ActMoveStyleRun or ActMoveStyleRide)
	CrtPropTile,						// �����TILE����				���û򷵻�: POINT*
	CrtPropUserName,					// ���������					���û򷵻�: char*
	CrtPropShowName,					// �Ƿ���������ʾ�Լ�������		���û򷵻�: BOOL*
	CrtPropNameColor,					// �������ֵ���ɫ				���û򷵻�: WPixel*
	CrtPropAbsolutePosition,			// ����ľ���λ��				����: POINT*
	CrtPropWeaponStyle,					// ��ɫЯ�������ķ�ʽ(��������?˫������?����������?)	���û򷵻�: int* enActorWeaponStyle ����
	CrtPropWeaponSoundID,				// ��ɫ�����Ĺ�����ЧID			���û򷵻�: int*
	CrtPropWeaponBeHitSoundID,			// ��ɫ�����Ļ���Ŀ����ЧID		���û򷵻�: int*
	CrtPropIsMoving,					// �����Ƿ������ƶ�?			����: BOOL*
	CrtPropState,						// �����״̬					���û򷵻�: int*
	CrtPropDirection,					// ����ķ���					���û򷵻�: int*
	CrtPropMoveSpeedLevel,				// ������ƶ��ٶȵȼ�(0->8)		���û򷵻�: int*
	CrtPropAttackSpeedLevel,			// ����Ĺ����ٶȵȼ�(0->8)		���û򷵻�: int
	CrtPropGroupInfoImage,  			// �����ϢͼƬ					���û򷵻�: IBitmap**

	CrtPropMaxPropCount, 				// ������������Ŀ
};
//------------------------------------------------------------------------
// ���������Ľ�����������˵��,����?��ʾ�˲���û��ʹ��(�봫��0)
enum enCreatureCommand
{
	CrtCmdNull = 0,						// ������
										// dwParam1: ?
										// dwParam2: ?

	CrtCmdSetProperty,					// ����ĳЩ���Ե�����
										// dwParam1: ��������
										// dwParam2: ����ֵ(�ο���ɫ�����Զ���)

	CrtCmdGetProperty,					// �õ�ĳЩ���Ե�����
										// dwParam1: ��������
										// dwParam2: ����ֵ(�ο���ɫ�����Զ���)
				
	CrtCmdMove,							// �ƶ�����,�Ե�ǰ���ƶ���ʽ�ƶ�
										// dwParam1: ·���б�ָ��(POINTS*);
										// dwParam2: ·���б�ڵ���Ŀ
				
	CrtCmdBeHit,						// ���ﱻ��
										// dwParam1: ���ﱻ��ʱ�������ز���(SBehitParam*)
										// dwParam2: ?
					
	CrtCmdDie,							// ��������
										// dwParam1: 0��ʾ�����������ж�����1��ʾ�Ѿ��������޶���
										// dwParam2: ?
					
	CrtCmdFallow,						// ��������
										// dwParam1: ?
										// dwParam2: ?

	CrtCmdChangeModule,					// ��װ���任���������
										// dwParam1: ��ƷID
										// dwParam2: 
				
	CrtCmdSpecialModule,				// ���ý�ɫĬ�ϲ���,�����ɫͼ��������,ʹ��Ĭ��ͼ��
										// dwParam1: ��������,ͷ�����塢�ֵ�
										// dwParam2: ����Ĭ��ID						
				
	CrtCmdBorn,							// ��ɫ����(����������Բ���һ�γ�������)
										// dwParam1: ?
										// dwParam2: ?
				
	CrtCmdUpgrade,						// ��ɫ����(����������Բ���һ����������)
										// dwParam1: ?
										// dwParam2: ?
				
	CrtCmdStand,						// ��ɫվ��(������������ǿ������;������������ƶ�����ֹͣ�ƶ�)
										// dwParam1: վ�����µ�TILEλ��(POINT*)
										// dwParam2: ?
				
	CrtCmdAppearance,					// ��������(��Ҫ��һЩ������)
										// dwParam1: �����û���ȡ��(TRUR or FALSE)
										// dwParam2: ���ID (DWORD)
				
	CrtCmdBloodInfo,					// ����ı�Ѫ��(��������������Ѫ����װ����)
										// dwParam1: ���ﵱǰ���ж���Ѫ(int*)
										// dwParam2: ���ﵱǰ�ܹ������ɶ���Ѫ(int*)

	CrtCmdChatInfo,						// ��ɫͷ����������Ϣ
										// dwParam1: ��������(LPSTR)
										// dwParam2: ������ɫ(WPixel*)
				
	CrtCmdMouseAni,						// ����(����ֻ������)����ʱ����·�ϵ��������µĵ������
										// dwParam1: �����TILE����(POINT*)
										// dwParam2: ?
				
	CrtCmdModulePalType,				// ����(����ֻ�н�ɫ)������ĵ�ɫ����������
										// dwParam1: �������(int), ��ϸ��ο����ﲿ���ű�
										// dwParam2: ��ɫ����������(int),1-type1,2-type2,...,0��ʾ��ͼ�����еĵ�ɫ��
				
	CrtCmdModuleAlpha,					// �ı��ɫ͸����
										// dwParam1: ���������(int), ��ϸ��ο����ﲿ���ű�
										// dwParam2: �����͸����, ��ϸ��ο�����༭��
					
	CrtCmdSetting,						// ��������(������)
										// dwParam1: BOOL(��ʶ���»��ǲ�����)
										// dwParam2: ?
				
	CrtCmdAddTitle,						// ��������ӳƺ�
										// dwParam1: LPCSTR(��ʽ����ĳƺ��ַ���)
										// dwParam2: DWORD(�ƺ�Ч����ID)
					
	CrtCmdClearTitle,					// �����������гƺ�
										// dwParam1: ?
										// dwParam2: ?

// ������ص� >>
	CrtCmdPreAttack,					// ����(ת������״̬��������Ч����)
										// dwParam1: ħ��(����)�Ĵ�������(SSkillCreateParam*)
										// dwParam2: ?
				
	CrtCmdStopPreAttack,				// ֹͣ����(ת��վ��״̬��������Чֹͣ)
										// dwParam1: ?
										// dwParam2: ?
				
	CrtCmdAttack,						// ����(ת������״̬������˲��ʩ��������ʩ������ͨ������ʩ����Ч������������������)
										// dwParam1: ħ��(����)�Ĵ�������(SSkillCreateParam*)
										// dwParam2: ?

	CrtCmdStopAttack,					// ֹͣ������ֻ�Գ���ʩ����Ч��ת��վ��״̬��ʩ����Чֹͣ����������ֹͣ��
										// dwParam1: ?
										// dwParam2: ?
						
	CrtCmdChangeLocForSkill,			// ��ɫ�ü���˳�Ƶ�ĳ��Tile��
										// dwParam1: ˳�Ƶ��µ�TILEλ��(POINT*)
										// dwParam2: ?

	CrtCmdSkillSelAni,					// ��Χ���ܵ�ѡ���Ч
										// dwParam1: BOOL(��ʶ���û���ȡ��)
										// dwParam2: ����ID

	CrtCmdSetNpcType,					// ����NPC����Ӧ��ͷ����Ч
										// dwParam1: NPC����Ӧ��ͷ����Ч �ο�enNpcType
										// dwParam2: ?
										
	CrtCmdSetTaskType,					// ����NPCͷ���Ĺ�Ч
										// dwParam1: ����NPCͷ���Ĺ�Ч �ο�enTaskType
										// dwParam2: ?

	CrtCmdSetClanName,					// ������������������
										// dwParam1: LPCSTR(��������)
										// dwParam2: ?

	CrtCmdSetCityName,					// ���������óǳ�����
										// dwParam1: LPCSTR(�ǳ�����)
										// dwParam2: ?

	CrtCmdMaxCommandCount,				// �����ܵ�������
};
//------------------------------------------------------------------------
// �������һЩ������Ķ��壬�Ժ���Ҫ��Ҫ������ȥ
enum enArticleProp
{
	AtcPropNull = 0,					// ������
	AtcPropName,						// ��Ʒ����������					���û򷵻�: char*				
	AtcPropShowName,					// �Ƿ��õ�����Ʒ��ʾ�Լ�������		���û򷵻�: BOOL*
	AtcPropNameColor,					// ��Ʒ��������ʾ��ɫ				���û򷵻�: WPixel*
	AtcPropBkImage,						// ������Ʒ�ı���ͼƬ(���ڰ�̯����������ı���ͼƬ)		���û򷵻�: IBitmap**
	AtcPropShowStar,					// �Ƿ��õ�����Ʒ��ʾ����			���û򷵻�: BOOL*

	AtcPropMaxPropCount,				// ������������Ŀ
};
//------------------------------------------------------------------------
enum enArticleCommand
{
	AtcCmdNull = 0,						// ������

	AtcCmdSetProperty,					// ����ĳЩ���Ե�����
										// dwParam1: ��������
										// dwParam2: ����ֵ(�ο�����������Զ���)

	AtcCmdGetProperty,					// �õ�ĳЩ���Ե�����
										// dwParam1: ��������
										// dwParam2: ����ֵ(�ο�����������Զ���)

	AtcCmdChangePal,					// �ı����ĵ�ɫ��
										// dwParam1: �������µĵ�ɫ��(TRUE)���ǻָ�ԭ���ĵ�ɫ��(FALSE)?
										// dwParam2: ��ɫ������

	AtcCmdThrow,						// Ͷ����Ʒ
										// dwParam1: Tileλ��(POINT*)
										// dwParam2: ��Ʒ��С���(С1,��2,��3)(int*)

	AtcCmdMaxCommandCount,				// �����ܵ�������
};
//------------------------------------------------------------------------
// �������һЩ������Ķ��壬�Ժ���Ҫ��Ҫ������ȥ
enum enAniProp
{
	AniPropNull = 0,					// ������
	AniPropName,						// ��������������		���û򷵻�: char*
	AniPropNameColor,					// ������������ʾ��ɫ	���û򷵻�: WPixel*
					
	AniPropMaxPropCount,				// ������������Ŀ
};				
				
enum enAniCommand				
{
	AniCmdNull = 0,						// ������
						
	AniCmdSetProperty,					// ����ĳЩ���Ե�����
										// dwParam1: ��������
										// dwParam2: ����ֵ(�ο�������������Զ���)
					
	AniCmdGetProperty,					// �õ�ĳЩ���Ե�����
										// dwParam1: ��������
										// dwParam2: ����ֵ(�ο�������������Զ���)
					
	AniCmdMaxCommandCount,				// �����ܵ�������
};
//------------------------------------------------------------------------
enum enTrapCommand
{
	TrpCmdNull = 0,						// ������
				
	TrpCmdTrigger,						// ���崥��
										// dwParam1: ���崥��ʱ�Ĵ�������(SSkillCreateParam*)
										// dwParam2: ?
				
	TrpCmdDestroy,						// ��������
										// dwParam1: ?
										// dwParam2: ?
				
	TrpCmdSetTile,						// ���������tile����
										// dwParam1: POINT*
										// dwParam1: ?

	TrpCmdMaxCommandCount,				// �����ܵ�������
};
//------------------------------------------------------------------------
// ���е���������
enum enTrapType
{
	TrpTypeHanBingXianJing		= 0,	// ��������
	TrpTypeBingKu				= 1,	// ����
	TrpTypeCiRen				= 2,	// ����
	TrpTypeHuoKeng				= 3,	// ���
	TrpTypeYinLei				= 4,	// ����
	TrpTypeLeiGuang				= 5,	// �׹�
	TrpTypeFire					= 6,	// ��(�񰵺�һ�Ż����գ�������˻�����)
	
	TrpTypeMaxCount				,		// ��������
};
//------------------------------------------------------------------------
struct ICreatureSink
{
	virtual void OnCreatureMoveToTileCenter(const POINTS& ptCurTile, const POINTS& ptNextTile) = 0;
};
//------------------------------------------------------------------------
