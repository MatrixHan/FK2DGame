/**
*	created:		2012-6-29   20:35
*	filename: 		IWorkSkillClient
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../SceneObj/IThingPart.h"
//------------------------------------------------------------------------
#define MAX_JINGLIAN_MATERIAL_NUM		6			//�����������Ŀ
#define MAX_JINGLIAN_RESULT				(1+8)		//������������
//������������
#define MAX_MATERIAL_NUM				9			//��������
//------------------------------------------------------------------------
// �����䷽�ؼ���
struct SJingLianKey
{
	DWORD dwID;					//װ��ID
	DWORD dwJingLianLevel;		//�����ȼ�
	
	SJingLianKey()
	{
		memset(this, 0, sizeof(SJingLianKey));
	}
	
	bool operator==(const SJingLianKey &Key) const
	{
		return dwID == Key.dwID && dwJingLianLevel == Key.dwJingLianLevel;
	}
};
//------------------------------------------------------------------------
// ����������Ϣ
struct SJingLianMaterial
{
	DWORD dwID;		//����ID
	DWORD dwNum;	//������Ŀ
	SJingLianMaterial()
	{
		memset(this, 0, sizeof(SJingLianMaterial));
	}
};
//------------------------------------------------------------------------
// �����䷽��Ϣ
struct SJingLianInfo
{
	DWORD dwMinLevel;		//�����ȼ�Ҫ��
	DWORD dwMaterialNum;	//�������ϸ���
	SJingLianMaterial Materials[MAX_JINGLIAN_MATERIAL_NUM];	
	DWORD dwToolsID;
	SJingLianInfo()
	{
		memset(this, 0, sizeof(SJingLianInfo));
	}
};
//------------------------------------------------------------------------
#define	MAX_HE_CHENG_PROBABILITY_NUM	(1+3)		//�ϳɻ�������
struct SHeChengInfo
{
	DWORD dwID;		//װ��ID
	DWORD dwType;	//����
	DWORD dwLevel;	//װ���ȼ�
	DWORD dwHighID;	//��һ��ID
	DWORD dwLowID;	//��һ��ID
	DWORD dwProbabilitys[MAX_HE_CHENG_PROBABILITY_NUM];//����
	DWORD dwMinLevel;		//Ҫ����ͼ��ܵȼ�
	DWORD dwTime;	//������ʱ��
	DWORD dwToolsID;	//����ID
	DWORD dwExp;		//����ֵ 
	SHeChengInfo()
	{
		memset(this, 0, sizeof(SHeChengInfo));
	}
};
//------------------------------------------------------------------------
//������Ϣ
struct SMaterialInfo
{
	DWORD dwMaterialID;		//����ID
	DWORD dwMaterialNum;	//������Ŀ
};
//------------------------------------------------------------------------
//�����䷽��Ϣ
struct SPeiFangInfo
{
	DWORD dwPeiFangID;		//�䷽ID
	DWORD dwLevel;			//��Ҫ���ܵȼ�
	SMaterialInfo MaterialInfos[MAX_MATERIAL_NUM];		//������Ϣ
	DWORD dwMaterialNum;		//���ϸ���
	DWORD dwProductID;		//��ƷID
	DWORD dwProductNum;		//��Ʒ����
	DWORD dwToolsID;		//����ID
	DWORD dwTime;			//������ʱ��
	SPeiFangInfo()
	{
		memset(this, 0, sizeof(SPeiFangInfo));
	}
};
//------------------------------------------------------------------------
// ��������ͨ����Ϣ
struct SWorkSkillBaseInfo 
{
	BYTE bSkillID;			//����ID
	BYTE bSkillType;		//��������
	char szName[16];		//����
	char szDes[256];		//����
	char szTypeDes[256];	//��������
	DWORD dwPicID;			//ͼ��ID
	SWorkSkillBaseInfo()
	{
		memset(this, 0, sizeof(SWorkSkillBaseInfo));
	}
};
//------------------------------------------------------------------------
// �������ܳƺ���Ϣ
struct SWorkSkillTitleInfo
{
	DWORD dwID;				//����ID
	char szName[16];		//��������
	DWORD dwLevel;			//���ܼ���
	char szTitle[16];		//���ܳƺ�
	DWORD dwMaxExp;			//���������	
	long lNeedActorLevel;	//��Ҫ��Ҽ���
	long lNeedActorMoney;	//��Ҫ��ҽ�Ǯ
	SWorkSkillTitleInfo()
	{
		memset(this, 0, sizeof(SWorkSkillTitleInfo));
	}
};
//------------------------------------------------------------------------
// �ϳ���Ϣ�ڶ���
struct SHeChengV2Info{
	enum
	{
		_MAX_MATERIAL_NUM = 18,
		_MAX_EFFECT_NUM = 7,
		_MAX_SLOT_NUM = 3,
	};
	DWORD GoodsID;     //��װ��ID
	char szName[32];   //��װ����
	DWORD MaterialGoodsID[_MAX_MATERIAL_NUM];  //18��������Ʒ��ID
	DWORD MaterialNeed[_MAX_MATERIAL_NUM];     //18��������Ʒ�����������
	
	DWORD SuccessProbability;  //�ɹ���
	DWORD Suit1_GoodsID;      //��װ1 ID
	DWORD Suit1_Probability;  //��װ1 ����
	DWORD Suit2_GoodsID;      //��װ2 ID
	DWORD Suit2_Probability;  //��װ2 ����
	DWORD NeedSkillPoint;     //ʹ������ļ��ܵ� , �����LUA��
	DWORD LuckyStoneProb;     //����ʯ���ӳɹ��ĸ���
	DWORD WhiteStoneProb;     //��ɫʯ���ӳɹ��ĸ���
	DWORD BlackStoneProb;     //��ɫʯ���ӳɹ��ĸ���
	DWORD EffectProbability[_MAX_EFFECT_NUM];  // 7�����ԵĻ���
	DWORD SlotProbability[_MAX_SLOT_NUM];  // 7�����ԵĻ���
	
	SHeChengV2Info()
	{
		memset(this, 0, sizeof(SHeChengV2Info));
	}
};
//------------------------------------------------------------------------
// �����ڶ���
struct SFusionV2Info{
	enum
	{
		MAX_FUSIONV2_MATERIAL_NUM = 18,
	};
	DWORD MainGoodsID;     //����ƷID
	char szName[32];   //����Ʒ��
	DWORD EquipRank;   //��װ������
	DWORD InheritedProbability;   //���Լ̳еĻ���
	
	DWORD MaterialGoodsID[MAX_FUSIONV2_MATERIAL_NUM];  //18��������Ʒ��ID
	DWORD MaterialNeed[MAX_FUSIONV2_MATERIAL_NUM];     //18��������Ʒ�����������
	
	DWORD SuccessProbability;  //�ɹ���
	DWORD ProductGoodsID;      //��Ʒ ID
	DWORD ProductNum;          //��Ʒ����
	
	SFusionV2Info()
	{
		memset(this, 0, sizeof(SFusionV2Info));
	}
};
//------------------------------------------------------------------------
// ��ҩ����
struct SPharmacyV2Info{
	enum
	{
		MAX_PHARMACYV2_PRODUCE_NUM = 4,    //�����4�ֲ�Ʒ
		MAX_PHARMACYV2_MATERIAL_NUM = 18,  //�����18�ֲ���
	};
	DWORD PrescriptionID;     //�䷽ID
	DWORD ProductGoodsID[MAX_PHARMACYV2_PRODUCE_NUM];      //��Ʒ ID
	DWORD ProductNum[MAX_PHARMACYV2_PRODUCE_NUM];          //��Ʒ����
	DWORD ProductZone[MAX_PHARMACYV2_PRODUCE_NUM];         //��Ʒ������������
	DWORD MaterialGoodsID[MAX_PHARMACYV2_MATERIAL_NUM];  //18��������Ʒ��ID
	DWORD MaterialNeed[MAX_PHARMACYV2_MATERIAL_NUM];     //18��������Ʒ�����������
	
	SPharmacyV2Info()
	{
		memset(this, 0, sizeof(SPharmacyV2Info));
	}
};
//------------------------------------------------------------------------
//����2��
typedef SPharmacyV2Info SEpurateV2Info;
//------------------------------------------------------------------------
//��Ƕ2��
struct SInlayInfoV2
{
	static enum
	{	
		MAX_INLAYEFFECT_NUM = 3,    //��ʯЧ��������
			MAX_EQUIP_TYPES = 16,  //���װ����������
	};
	
	DWORD dwStoneID;			//��ʯID
	char  szName[33];           //��ʯ����
	DWORD dwMinLevel;	//Ҫ���ܼ���
	DWORD dwEffects[MAX_INLAYEFFECT_NUM];		//��������
	DWORD dwEffectProbabilitys[MAX_INLAYEFFECT_NUM];//����
	DWORD dwEquipTypes[MAX_EQUIP_TYPES];			//����
	DWORD dwTime;		//������ʱ��
	SInlayInfoV2()
	{
		memset(this, 0, sizeof(SInlayInfoV2));
	}
	
};
//------------------------------------------------------------------------
//����2��
struct SRefineV2Info{
	enum
	{		
		MAX_REFINEV2_MATERIAL_NUM = 6,  //�����18�ֲ���
			RESULT_ZERO = 0,  //����Ϊ0  ������IDX
			RESULT_DECREASE = 1,  //���� -1  ������IDX
			RESULT_NOCHANGE = 2,  //��������  ������IDX
			RESULT_INCREASE = 3,  //����Ϊ +1  ������IDX
	};
	DWORD dwEquipID;     //װ��ID
	char  szName[32 + 1]; //����
	DWORD dwSkillLevel;   //�������ܵĵȼ�
	DWORD dwSmelt;        // �����ľ�����
	DWORD dwMaterialID[MAX_REFINEV2_MATERIAL_NUM];
	DWORD dwMaterialNum[MAX_REFINEV2_MATERIAL_NUM];
	DWORD dwProbability[4]; //�ɹ�������
	DWORD dwTime;   //ʱ��
	SRefineV2Info()
	{
		memset(this, 0, sizeof(SRefineV2Info));
	}
	
	static DWORD MAKEKEY(DWORD dwEquipID, DWORD nSmelt)
	{
		return dwEquipID * 100 + nSmelt;
	}
};
//------------------------------------------------------------------------
struct IActor;
struct IContainer;
struct IWorkSkillRul;
//------------------------------------------------------------------------
struct IWorkSkill
{
	virtual void Release() = NULL;
	// �ж��Ƿ����ʹ�øü���
	virtual BOOL CanUse(LPCSTR pContext, int nLen) = NULL;
	// ʹ�ü���
	virtual void Use(LPCSTR pContext, int nLen) = NULL;
	virtual BYTE GetLevel() = NULL;
	virtual BYTE GetID() = NULL;
	// ��ȡ����ֵ
	virtual WORD GetExp() = NULL;	
	// ��ȡ��ǰ�����ֵ
	virtual DWORD GetMaxExp() = NULL;
	// ��ȡ�ƺ�
	virtual LPCSTR GetTitle() = NULL;
	// ��ȡ����
	virtual IContainer* GetContainer() = NULL;
	// ������
	virtual void DoResult(LPCSTR buf, int nLen) = NULL;
	// ����
	virtual BOOL OnUpdate(LPCSTR buf, int nLen) = NULL;
};
//------------------------------------------------------------------------
struct IWorkSkillMgr : public IThingPart
{
	// ����ID ��ȡ����
	virtual IWorkSkill* GetSkill(BYTE bWorkSkillID) = NULL;
	virtual void RequestShowWorkSkill(UID uidTarget) = NULL;
	virtual void RequestWorkSkillByID(UID uidTarget, BYTE bSkillID) = NULL;
	virtual void RequestOK(UID uidTarget, BYTE bSkillID, DWORD dwMoney, LPCSTR buf = NULL, int nLen = NULL) = NULL;
	virtual void AcceptWorkSkill(UID uidRequestor, BYTE bSkillID, LPCSTR buf = NULL, int nLen = NULL) = NULL;
	virtual void CloseWorkSkill(BYTE bSkillID) = NULL;
	//���������
	virtual void DisrememberSkill(BYTE bSkillID) = NULL;
};
//------------------------------------------------------------------------
struct IWorkSkillSystem
{
	virtual void Release() = NULL;
	// �����������ܹ�����
	virtual BOOL CreateWorkSkillMgr(IWorkSkillMgr **ppWorkSkillMgr) = NULL;
	// ���������ID��ȡ������Ϣ(SWorkSkillBaseInfo)
	virtual SWorkSkillBaseInfo* GetWorkSkillBaseInfo(BYTE bWorkSkillID) = NULL;

	// ��ȡ�����䷽����
	virtual int GetLianDanPeifangNum() = NULL;
	// �����䷽��Ż�ȡ������ƷID (��ƷID)
	virtual DWORD GetLianDanProductID(int nIndex) = NULL;
	// ���ݲ�ƷID ��ȡ�䷽��Ϣ	
	virtual SPeiFangInfo* GetPeiFangInfo(DWORD dwProductID) = NULL;

	// ������ƷID�;����ȼ���ȡ������Ϣ
	virtual SJingLianInfo* GetJingLianInfo(DWORD dwGoodsID, DWORD dwLevel) = NULL;

	virtual SWorkSkillTitleInfo* GetTitleInfo(DWORD dwSkillID, DWORD dwLevel) = NULL;
	virtual IWorkSkillRul* GetRul(BYTE bSkillID) = NULL;
	virtual	SHeChengInfo* GetHeChengInfo(DWORD dwID) = NULL;

	//�ڶ��ڹ�������
	virtual	SHeChengV2Info* GetHeChengV2Info(DWORD dwID) = NULL;
	virtual SFusionV2Info* GetFusionV2Info(DWORD dwMainGoodsID) = NULL;
	virtual SPharmacyV2Info* GetPharmacyInfo(DWORD dwPrescriptionID) = NULL;
	virtual SRefineV2Info* GetRefineV2Info(DWORD dwEquipID, DWORD dwSmelt) = NULL;
	//����һ���������ʹ�С, ����ֵ��ʾ���ж��ٸ���Ʒ,��Ʒ�䷽ID����pBuff;
	virtual DWORD GetPharmacyV2PrescriptionList(DWORD *pBuff, int nLen) = NULL;

	//����һ���������ʹ�С, ����ֵ��ʾ���ж��ٸ���Ʒ,��Ʒ�䷽ID����pBuff;
	virtual DWORD GetEpurateV2PrescriptionList(DWORD *pBuff, int nLen) = NULL;
	virtual SEpurateV2Info* GetEpurateV2Info(DWORD dwPrescriptionID) = NULL;

};
//------------------------------------------------------------------------
struct IClientGlobal;
//------------------------------------------------------------------------
class CWorkSkillSystemHelper
{
private:
	typedef BOOL (*ProcCreateWorkSkillSystemObject)(
		IWorkSkillSystem **ppStatusSys, IClientGlobal *pClientGlobal, const char *pFileName );
	
public:
	CWorkSkillSystemHelper()
	{
		m_hDll = NULL;
		m_pWorkSkillSystem = NULL;
		m_szErrMsg[0] = 0;
	}
	~CWorkSkillSystemHelper()
	{
		Close();
		if (m_hDll != NULL)//ע�⣬�����lib��ʽ������m_hDll����NULL,���������::FreeLibary()���ᱻ����
		{
			::FreeLibrary(m_hDll);
			m_hDll=NULL;
		}
	}	

	BOOL Create(IClientGlobal *pClientGlobal)
	{
		Close();
		try
		{
			if (m_hDll == NULL)
			{
				//��һ�ε���ʱδ����dll,������м���dll
#ifdef _DEBUG
				m_hDll = ::LoadLibrary("FKWorkSkillClient_D.dll");										
				if (m_hDll == NULL)
					throw "�޷����� FKWorkSkillClient_D.dll";
#else
				m_hDll = ::LoadLibrary("FKWorkSkillClient.dll");										
				if (m_hDll == NULL)
					throw "�޷����� FKWorkSkillClient.dll";
#endif
			}
			//������dll��������������ӿڶ���Ĵ�������
			ProcCreateWorkSkillSystemObject proc = NULL;
			proc=(ProcCreateWorkSkillSystemObject)::GetProcAddress(m_hDll,
				"CreateWorkSkillSystemObject");
			if (proc == NULL)
				throw "Can't GetProcAddress('CreateWorkSkillSystemObject')";
			if (!proc(&m_pWorkSkillSystem, pClientGlobal, NULL))
				throw "CreateWorkSkillSystemObject() error!";
			
			return TRUE;//�ӿڶ��󴴽��ɹ�
		}
		catch (LPCSTR szMsg) 
		{
			lstrcpyn(m_szErrMsg, szMsg, sizeof(m_szErrMsg));
			return FALSE;
		}
		catch(...)
		{
			lstrcpyn(m_szErrMsg, "Unknown Error!", sizeof(m_szErrMsg));
			return FALSE;
		}
	}
	void Close()//���ٽӿڶ���
	{
		if (m_pWorkSkillSystem != NULL)
		{
			m_pWorkSkillSystem->Release();
			m_pWorkSkillSystem = NULL;			
		}
	}
	//����->,��helper���������ֱ��תΪ�ӿڵ�����
	IWorkSkillSystem* operator ->()
	{
		return m_pWorkSkillSystem;
	}
	IWorkSkillSystem* GetInterface()
	{
		return m_pWorkSkillSystem;
	}
	//�жϽӿ�ָ���Ƿ���Ч
	BOOL IsValid()
	{
		return (m_pWorkSkillSystem != NULL);
	}
	char* GetErrMsg()
	{
		return m_szErrMsg;
	}
private:
	IWorkSkillSystem *m_pWorkSkillSystem;					//�ӿڶ���
	HINSTANCE m_hDll;									//��̬���ӿ��������־
	char m_szErrMsg[128];								//������Ϣ
};
//------------------------------------------------------------------------