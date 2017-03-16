/**
*	created:		2012-7-3   0:57
*	filename: 		MsgFormat
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�������Ϳͻ���ͨ�ŵ���Ϣ�ṹ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "MsgKeyMainSelectActor.h"
#include "../FKGame/Common/GoodsPropertyDef.h"
#include "MsgKeyRoot.h"
#include "MsgKeyMainThing.h"
#include "../FKGame/Common/CreaturePropertyDef.h"
//------------------------------------------------------------------------
#pragma pack(1)	
//------------------------------------------------------------------------
#define SURECODE_DB_MAXLEN		40				// û��MD5��ʶ����
#define SURECODE_MD5_MAXLEN		33				// MD5���ʶ���룬ͨ�����紫��
//------------------------------------------------------------------------
// ��  ���������������ͻ��˵ĵ�½̬��Ϣ��
	struct SC_LOGIN_HEAD
	{
		WORD    m_wKeyRoot;
		WORD    m_wKeyMain;

		SC_LOGIN_HEAD(WORD wKeyMain)
		{
			m_wKeyRoot = CMD_ROOT_LOGIN;
			m_wKeyMain = wKeyMain;
		}

		SC_LOGIN_HEAD(void)
		{
			m_wKeyRoot = 0;
			m_wKeyMain = 0;
		}
	};	
//------------------------------------------------------------------------
// ��  �����ͻ��˷����������ĵ�½̬��Ϣ��
	struct CS_LOGIN_HEAD
	{
		WORD    m_wKeyRoot;
		WORD    m_wKeyMain;

		CS_LOGIN_HEAD(WORD wKeyMain)
		{
			m_wKeyRoot = CMD_ROOT_LOGIN;
			m_wKeyMain = wKeyMain;
		}

		CS_LOGIN_HEAD(void)
		{
			m_wKeyRoot = 0;
			m_wKeyMain = 0;
		}
	};
//------------------------------------------------------------------------
// ��  ���������������ͻ��˵�ѡ������̬��Ϣ��
	struct SC_SELECTACTOR_HEAD
	{
		WORD    m_wKeyRoot;
		WORD    m_wKeyMain;

		SC_SELECTACTOR_HEAD(void)
		{
			m_wKeyRoot = CMD_ROOT_SELECTACTOR;
		}

		SC_SELECTACTOR_HEAD(WORD wKeyMain)
		{
			m_wKeyRoot = CMD_ROOT_SELECTACTOR;
			m_wKeyMain = wKeyMain;
		}
	};	

	struct SC_SELECTACTOR_ENTERCODE : public SC_SELECTACTOR_HEAD
	{
		DWORD	m_dwEnterCode;

		SC_SELECTACTOR_ENTERCODE(DWORD dwEnterCode)
		{
			m_wKeyRoot = CMD_ROOT_SELECTACTOR;
			m_wKeyMain = ROOTSELECTACTOR_SC_MAIN_ENTERCODE;
			m_dwEnterCode = dwEnterCode;
		}
	};
//------------------------------------------------------------------------
// ��  �����ͻ��˷�����������ѡ������̬��Ϣ��
	struct CS_SELECTACTOR_HEAD
	{
		WORD    m_wKeyRoot;
		WORD    m_wKeyMain;

		CS_SELECTACTOR_HEAD(void)
		{
			m_wKeyRoot = CMD_ROOT_SELECTACTOR;
			m_wKeyMain = 0;

		}
	};

	struct CS_SELECTACTOR_SELECT_HEAD : public CS_SELECTACTOR_HEAD
	{
		// �ʺ�
		char	m_szUserName[32 + 1];
		// ����
		char	m_szPassword[32 + 1];

		// ѡ�������ActorID
		DWORD	m_dwSelActorID;	
		
		// �������к�
		char	szSerialNO[20];	

		// ��¼����������Ҫ���ڸ��л����������֣�
		long	lFirstLogin;

		// ʶ����
		char	m_szSureCode[SURECODE_MD5_MAXLEN];

		CS_SELECTACTOR_SELECT_HEAD(void)
		{
			memset(this, 0, sizeof(CS_SELECTACTOR_SELECT_HEAD));
			m_wKeyRoot = CMD_ROOT_SELECTACTOR;
			m_wKeyMain = ROOTSELECTACTOR_CS_MAIN_SELECT;
			m_dwSelActorID = 0;
		}

		CS_SELECTACTOR_SELECT_HEAD(DWORD dwActorID)
		{
			memset(this, 0, sizeof(CS_SELECTACTOR_SELECT_HEAD));
			m_wKeyRoot = CMD_ROOT_SELECTACTOR;
			m_wKeyMain = ROOTSELECTACTOR_CS_MAIN_SELECT;
			m_dwSelActorID = dwActorID;
		}
	};

	struct CS_SELECTACTOR_MAC_HEAD : public CS_SELECTACTOR_HEAD
	{
		char	m_szMAC[512];

		CS_SELECTACTOR_MAC_HEAD(char szMac[512])
		{
			memset(this, 0, sizeof(CS_SELECTACTOR_MAC_HEAD));
			m_wKeyRoot = CMD_ROOT_SELECTACTOR;
			m_wKeyMain = ROOTSELECTACTOR_CS_MAIN_MAC;
			memcpy(m_szMAC, szMac, sizeof(m_szMAC));
		}
	};

	struct CS_SELECTACTOR_SELECT_HEAD_QQ : public CS_SELECTACTOR_HEAD
	{
		// QQ��
		char	m_szQQ[32 + 1];

		// QQData����
		int		m_nQQDataLen;

		// QQData
		char	m_szQQData[256];

		// QQData MD5
		char	m_szQQDataMD5[32 + 1];

		// ѡ�������ActorID
		DWORD	m_dwSelActorID;	
		
		// �������к�
		char	szSerialNO[20];	

		// ��¼����������Ҫ���ڸ��л����������֣�
		long	lFirstLogin;

		// ʶ����
		char	m_szSureCode[SURECODE_MD5_MAXLEN];

		CS_SELECTACTOR_SELECT_HEAD_QQ(void)
		{
			memset(this, 0, sizeof(CS_SELECTACTOR_SELECT_HEAD_QQ));
			m_wKeyRoot = CMD_ROOT_SELECTACTOR;
			m_wKeyMain = ROOTSELECTACTOR_CS_MAIN_SELECT_QQ;
			m_dwSelActorID = 0;
		}

		CS_SELECTACTOR_SELECT_HEAD_QQ(DWORD dwActorID)
		{
			memset(this, 0, sizeof(CS_SELECTACTOR_SELECT_HEAD_QQ));
			m_wKeyRoot = CMD_ROOT_SELECTACTOR;
			m_wKeyMain = ROOTSELECTACTOR_CS_MAIN_SELECT_QQ;
			m_dwSelActorID = dwActorID;
		}
	};
//------------------------------------------------------------------------
// ��  ���������������ͻ��˵�����̬��Ϣ��
	// �����������ͻ��˵���Ϣͷ
	struct SC_THING_HEAD
	{
		WORD	m_wKeyRoot;
		WORD	m_wKeyMain;

		SC_THING_HEAD(void)
		{
			m_wKeyRoot = CMD_ROOT_THING;
			m_wKeyMain = 0;
		}
	};

	// ������ͨ��ROOTTHING_SC_MAIN_PRIVATECONTEXT�����ͻ��˵���Ϣͷ
	struct SC_THING_PRIVATE_HEAD : public SC_THING_HEAD
	{	
		UID		m_uidThing;
		WORD	m_wThingClassID;
		WORD	m_wReserved;

		SC_THING_PRIVATE_HEAD(void)
		{
			m_uidThing = INVALID_UID;
			m_wKeyMain = ROOTTHING_SC_MAIN_PRIVATECONTEXT;
			m_wThingClassID = 0;
			m_wReserved = 0;
		}
	};

	// ������ͨ��ROOTTHING_SC_MAIN_PUBLICCONTEXT�����ͻ��˵���Ϣͷ
	struct SC_THING_PUBLIC_HEAD : public SC_THING_HEAD
	{
		UID		m_uidThing;
		WORD	m_wThingClassID;
		WORD	m_wReserved;

		SC_THING_PUBLIC_HEAD(void)
		{
			m_uidThing = INVALID_UID;
			m_wKeyMain = ROOTTHING_SC_MAIN_PUBLICCONTEXT;
			m_wThingClassID = 0;
			m_wReserved = 0;
		}
	};

	// ������ͨ��ROOTTHING_SC_MAIN_ACTION�����ͻ��˵���Ϣͷ
	struct SC_THING_ACTION_HEAD : public SC_THING_HEAD
	{
		UID		m_uidThing;
		WORD	m_wKeySub;

		SC_THING_ACTION_HEAD(void)
		{
			m_uidThing = INVALID_UID;
			m_wKeyMain = ROOTTHING_SC_MAIN_ACTION;
			m_wKeySub = 0;
		}
	};

	// ������ͨ��ROOTTHING_SC_MAIN_CHANGEMAP�����ͻ��˵���Ϣͷ
	struct SC_THING_CHANAGEMAP_HEAD : public SC_THING_HEAD
	{
		DWORD	m_dwChangeMode;		// �л�ģʽ.1 : �������л�, 2 ��������л�
		int		m_nMapID;			// ��ͼID
		POINT	m_ptWorld;			// ��������			
	};

	// ������ͨ��ROOTTHING_SC_MAIN_COUNTFEE�����ͻ��˵���Ϣͷ
	struct SC_THING_COUNTFEE_HEAD : public SC_THING_HEAD
	{
		DWORD	m_dwCountMode;		// �Ʒ�ģʽ
		int		m_nQdot;			// Q��
		int		m_nSecond;			// ʣ��ʱ��
		char	m_szDesc[128];		// ����
	};

	// ������ͨ��ROOTTHING_SC_MAIN_GAMEWORLD�����ͻ��˵���Ϣͷ
	struct SC_THING_GAMEWORLD_HEAD : public SC_THING_HEAD
	{
		WORD	m_wKeySub;			// ����Ϣ��

		SC_THING_GAMEWORLD_HEAD(WORD wKeySub)
		{
			m_wKeyRoot = CMD_ROOT_THING;
			m_wKeyMain = ROOTTHING_SC_MAIN_GAMEWORLD;
			m_wKeySub = wKeySub;
		}		
	};
//------------------------------------------------------------------------
// ��  �����ͻ��˷���������������̬��Ϣ��
	// �ͻ��˷�����������ͨ����Ϣͷ	
	struct CS_THING_HEAD
	{
		WORD	m_wKeyRoot;
		WORD	m_wKeyMain;

		CS_THING_HEAD(void)
		{
			m_wKeyRoot = CMD_ROOT_THING;
			m_wKeyMain = 0;
		}
	};

	// ROOTTHING_CS_MAIN_ACTIONͨ����Ϣͷ
	struct CS_ACTION_HEAD : public CS_THING_HEAD
	{
		WORD    m_wKeySub;

		CS_ACTION_HEAD(WORD wKeySub)
		{
			m_wKeyRoot = CMD_ROOT_THING;
			m_wKeyMain = ROOTTHING_CS_MAIN_ACTION;
			m_wKeySub  = wKeySub;
		}

		CS_ACTION_HEAD(void)
		{
			m_wKeyRoot = CMD_ROOT_THING;
			m_wKeyMain = ROOTTHING_CS_MAIN_ACTION;
			m_wKeySub  = 0;
		}
	};

	// ROOTTHING_CS_MAIN_ACTIONͨ����Ϣͷ, ��Ҫ���ʱ��������ݰ����������
	struct CS_ACTION_HEAD_EX : public CS_ACTION_HEAD
	{
		DWORD   m_dwTimeTick;

		CS_ACTION_HEAD_EX(WORD wKeySub, DWORD dwTimeTick)
		{
			m_wKeyRoot	= CMD_ROOT_THING;
			m_wKeyMain	= ROOTTHING_CS_MAIN_ACTION;
			m_wKeySub	= wKeySub;
			m_dwTimeTick= dwTimeTick;
		}

		CS_ACTION_HEAD_EX(void)
		{
			m_wKeyRoot	= CMD_ROOT_THING;
			m_wKeyMain	= ROOTTHING_CS_MAIN_ACTION;
			m_wKeySub	= 0;
			m_dwTimeTick= 0;
		}
	};
//------------------------------------------------------------------------
// ��  ���������������ͻ��˵��˺���Ϣ��
	struct SC_DAMAGE_REHEARSAL_HEAD
	{
		UID		uidSource;		// ������ID
		WORD	wCheckId;		// ���ܵĽϼ�ID��Ψһȷ��
		BOOL	bDoAct;			// �Ƿ�Ҫ����������
		WORD	wFlowType;		// Ʈ������,0 Ʈ��������,1 Ʈ����,2 Ʈ�ֿ�,3 Ʈ����
		int		nDamHP;			// ����HPֵ
	};
//------------------------------------------------------------------------
// ��  ���������������ͻ��˵�������Ϣ��
	struct SC_CURE_REHEARSAL_HEAD
	{
		UID		uidSource;		// ������ID
		WORD	wCheckId;		// ���ܵĽϼ�ID��Ψһȷ��
		BOOL	bDoAct;			// �Ƿ�Ҫ�����ƶ���
		int		nCureHP;		// �ӵ�HPֵ
	};
//------------------------------------------------------------------------
// ��  ����ʹ��ҩƷ
	struct CS_USEMEDICAMENT_CONTEXT
	{
		UID		uidMedicament;	// ҩƷ
		UID		uidTarget;		// Ŀ�����
		POINTS	ptTarget;		// Ŀ��tile
	};
//------------------------------------------------------------------------
// ��  ����������Ʒ
	struct CS_THROWGOODS_CONTEXT
	{
		UID		uidGoods;		// ��Ʒid		
		POINTS	ptTile;			// ���ڵ�ͼ��λ��
		long	lContainerID;	// ����id
	};
//------------------------------------------------------------------------
// ��  ����ħ������
	struct SC_MAGICRELIVE_CONTEXT
	{
		UID		uidMagicer;		// ʩ����
	};

	struct CS_MAGICRELIVE_CONTEXT
	{
		UID		uidMagicer;		// ʩ����
		BOOL	bAgree;			// �Ƿ�ͬ��
	};
//------------------------------------------------------------------------
// ��  ������������۲�
	struct SC_HOLDPRIZE_OBSERVE_CONTEXT
	{
		UID		uidMonster;		// �۲�θ���
		UID		uidCurAward;	// ��ǰ����
		DWORD	dwHoldTeamID;	// �����θ���ID
	};

	struct CS_HOLDPRIZE_OBSERVE_CONTEXT
	{
		BOOL	bObserveFlag;	// �Ƿ�۲�
		UID		uidMonster;		// �۲�θ���
	};
//------------------------------------------------------------------------
// ��  �������¸���ʱ��
	struct SC_RELIVE_TIME_CONTEXT
	{
		int		nReliveTimeLeft;// ʣ�ิ��ʱ��
		int		nStaticMapID;	// ������̬��ͼID
		POINT	ptReliveLoc;	// ����λ��
	};
//------------------------------------------------------------------------
// ��  ������Ϣ����
	struct SC_MSG_CENTER_CONTEXT
	{
		DWORD	dwKey;			// 
		DWORD	dwType;			// ����; MSG_CENTER_TYPE_TRADE....
		DWORD	dwDataLen;		// ���ݳ���
								// ����
	};
//------------------------------------------------------------------------
// ��  �����۲��ɫλ��
	struct SC_MSG_LOOK_ACTOR_DATA_CONTEXT
	{
		UID		uidActor;		// UID
		DWORD	dwStaticMapID;	// ������̬��ͼID
		DWORD	dwDynamicMapID;	// ������̬��ͼID
		SACTOR_PROPERTY_PUBLIC stPublicProp;	// ����
	};

	struct CS_MSG_LOOK_ACTOR_DATA_CONTEXT
	{
		DWORD	dwActorDBID[64];// �۲��ActorDBID

		CS_MSG_LOOK_ACTOR_DATA_CONTEXT()
		{
			ZeroMemory(this, sizeof(CS_MSG_LOOK_ACTOR_DATA_CONTEXT));
		}
	};
//------------------------------------------------------------------------
// ��  �����������
	struct SC_MSG_PLAYERLOGOUT_CONTEXT
	{
		DWORD	dwStateFlag;	// 1 = ׼�����ߣ�2 = ���ڴ��̣�3 = �������
		DWORD	dwTimeLeft;		// ʣ��׼������ʱ��
	};
//------------------------------------------------------------------------
// ��  �����ٻ�
	struct SC_MSG_CONJURE_CONTEXT
	{
		UID		uidMaster;		// ����
		UID		uidChief;		// ����
		DWORD	dwPawnCount;	// צ������
		//.......				// צ��UID����
	};

	struct CS_MSG_CONJURE_CONTEXT
	{
		UID		uidMaster;		// �θ�����
	};
//------------------------------------------------------------------------
// ��  ��������
	struct SC_MSG_MOUNT_CONTEXT
	{
		int		nIndex;			// index
		int		nHorseID;		// ��ƥid
	};
//------------------------------------------------------------------------
// ��  ����ȡ���������
	struct SC_MSG_GETRIDEDATA_CONTEXT
	{
		int		nCurMountIndex;	// ��ǰ��ĺ�ƥ
		int		nHorseCount;	// ӵ����ƥ����

		struct HORSEDATA
		{
			int	nIndex;			// index
			int nID;			// ID
		};
		//.....					// ��ƥ����
	};

	struct CS_MSG_GETRIDEDATA_CONTEXT
	{
		UID		uidMaster;		// �θ�����
	};
//------------------------------------------------------------------------
// ��  ������ݼ�
	struct CS_MSG_SHORTCUT_CONTEXT
	{
		char	szShortcutBuf[CREATURE_SHORTCUT_BUF_LEN];// ��ݼ�buf
	};
//------------------------------------------------------------------------
// ��  ����ˢ�����
	struct SC_MSG_CLEAN_SHOW_CONTEXT
	{
		struct  SHOWRES
		{
			DWORD	dwGoodsID;	// ��ƷID
			char	nSmeltLevel;// �����ȼ�
		};

		SHOWRES	stShowRes[EQUIPPOS_INDEX_MAX];
	};
//------------------------------------------------------------------------
// ��  �����۲����װ��
	struct CS_MSG_LOOK_ACTOR_EQUIP_CONTEXT
	{
		UID		uidMaster;		// �۲����
	};

	struct SC_MSG_LOOK_ACTOR_EQUIP_CONTEXT
	{
		UID		uidMaster;		// ���۲����
		UID		uidEquip[EQUIPPOS_INDEX_MAX];
	};
//------------------------------------------------------------------------
#pragma pack()	
//------------------------------------------------------------------------