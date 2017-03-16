/**
*	created:		2012-6-29   21:40
*	filename: 		IEffectClient
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
enum
{
	en_STRENGTH_EFFECT = 1,			//����Ч��
	en_CONSTIT_EFFECT = 2,			//����Ч��
	en_MENTAL_EFFECT = 3,			//����Ч��
	en_WISDOM_EFFECT = 4,			//�ǻ�Ч��
	en_CELERITY_EFFECT = 5,			//����Ч��
	
	en_P_A_EFFECT = 6,				//����������
	en_P_A_MIN_EFFECT = 7,			//������С��������
	en_P_A_MAX_EFFECT = 8,			//������󹥻�����
	en_P_A_PER_EFFECT = 9,			//�����ٷֱ�
	en_P_SUDDENLY_EFFECT = 10,		//������������
	en_M_SUDDENLY_EFFECT = 11,		//��������������
	
	en_F_A_EFFECT = 12,				//����������
	en_I_A_EFFECT = 13,				//������������
	en_E_A_EFFECT = 14,				//�編��������	
	en_N_A_EFFECT = 15,				//������������
	
	en_M_A_EFFECT = 16,				//����������
	en_M_A_PER_EFFECT = 17,			//�����������ٷֱ�
	
	en_CURE_EFFECT = 18,			//����Ч������
	en_CURE_PER_EFFECT = 19,		//����Ч�����Ӱٷֱ�
	
	en_F_D_EFFECT = 20,				//����������
	en_I_D_EFFECT = 21,				//������������
	en_E_D_EFFECT = 22,				//�編��������
	en_N_D_EFFECT = 23,				//������������
	
	en_P_D_EFFECT = 24,				//���������
	en_P_D_PER_EFFECT = 25,			//����������ٷֱ�
	
	en_M_D_EFFECT = 26,				//����������
	en_M_D_PER_EFFECT = 27,			//�����������ٷֱ�
	
	en_P_DAMAGE_EFFECT = 28,		//�����˺����յ���	
	en_M_DAMAGE_EFFECT = 29,		//�����˺����յ���	
	en_ALL_DAMAGE_EFFECT = 30,		//�����˺����յ���	
	
	en_ECHO_DAMAGE_EFFECT = 31,		//�������ӷ����˺�
	
//	en_VERACITY_PER_EFFECT = 32,	//�����ʰٷֱ�
//	en_HEDGE_PER_EFFECT = 33,		//�����ʰٷֱ�
	
	en_PERSIST_HP_EFFECT = 34,		//��������ֵЧ��
	en_PERSIST_MP_EFFECT = 35,		//��������ֵЧ��
	en_PERSIST_HP_PER_EFFECT = 36,	//�����ָ��ٷֱ�HP
	en_PERSIST_MP_PER_EFFECT = 37,	//�����ָ��ٷֱ�MP
	
	en_KILL_HP_EFFECT = 38,			//ÿɱ��һ��������ʻָ�HP
	en_KILL_MP_EFFECT = 39,			//ÿɱ��һ��������ʻָ�MP
	en_DAMAGE_HP_EFFECT = 40,		//ÿ���˺��ָ�HP
	en_DAMAGE_MP_EFFECT = 41,		//ÿ���˺��ָ�MP	
	
	en_HPMAX_EFFECT = 42,			//�������ֵ
	en_MPMAX_EFFECT = 43,			//�����ֵ
	en_HPMAX_PER_EFFECT = 44,		//�������ֵ�ٷֱ�
	en_MPMAX_PER_EFFECT = 45,		//�����ֵ�ٷֱ�
	
	en_MOVESPEEDLEVEL_EFFECT = 52,	//����
	
	//////////////////////////////////////////////////////////////////////////
	//53-63����ʹ��
	en_INVINCIBILITY1_EFFECT = 53,	//�޵�1�������κ��˺���״̬�������ƶ���������Է��𹥻�
	en_INVINCIBILITY2_EFFECT = 54,  //�޵�2�������κ��˺���״̬�������ƶ����������Է��𹥻�
	en_INVINCIBILITY3_EFFECT = 55,	//�޵�3�������κ��˺���״̬���������ƶ����������Է��𹥻�
	
	en_BLACKOUT_EFFECT = 56,		//ѣ��,�޷��ƶ����޷��������޷�ʹ����Ʒ���ܵ�����������
	en_LETHARGY_EFFECT = 57,		//��˯,�޷��ƶ����޷��������޷�ʹ����Ʒ���ܵ���������
	en_FIXATION_EFFECT = 58,		//�����޷��ƶ������Թ���������ʹ����Ʒ���ܵ���������
	en_FROST_EFFECT = 59,			//�������޷��ƶ����޷��������޷�ʹ����Ʒ���ܵ���������
	
	en_AFFRIGHT_EFFECT = 60,		//����,��һ�������ܣ��ܵ��˺�������
	en_SWIVET_EFFECT = 61,			//����,��һ����Χ�����ܣ��ܵ��˺�������
	en_SOMNAMBULATE_EFFECT = 62,	//����,��һ�������ܣ��ܵ��˺�����
	en_PUZZLE_EFFECT = 63,			//�Ի�,��һ����Χ�����ܣ��ܵ��˺�����
	//////////////////////////////////////////////////////////////////////////
	
	
	en_VOTE_MOVE_EFFECT = 100,		//����޷��ƶ�Ч��
	en_VOTE_ATTACK_EFFECT = 101,	//�޷�����Ч��
	en_VOTE_USE_GOODS_EFFECT = 102,	//�޷�ʹ����ƷЧ��
	en_VOTE_DAMAGE_EFFECT = 103,	//�����κ��˺�Ч��
	
	en_P_PERSIST_DAMAGE_EFFECT = 150,		//���������˺�
	en_F_PERSIST_DAMAGE_EFFECT = 151,		//�������˺�
	en_I_PERSIST_DAMAGE_EFFECT = 152,		//�������˺�
	en_E_PERSIST_DAMAGE_EFFECT = 153,		//�������˺�
	en_N_PERSIST_DAMAGE_EFFECT = 154,		//�������˺�
	
	en_FORBID_MAGIC_EFFECT = 200,			//��ħ
	en_CHANGE_SKILL_SHAPE_EFFECT = 201,		//�ı���̬
	en_Add_Enmity_EFFECT = 202,				//���ӳ��ֵ��ֻ�Թ�����Ч
	en_SWIVET2_EFFECT = 203,					//����Ч��������	
	en_SKILL_BREAK_EFFECT = 204,					//���ʩ����Ч��
	
	en_DamageToMP_EFFECT = 220,				//�˺�ת�Ƶ�����Ч��
	en_CRAZYDAMAGE_EFFECT = 221,			//�����Ч��
	en_ALL_DAMAGE_PER_EFFECT = 222,			//�����˺����հٷֱ�
	en_EXP_EFFECT = 223,					//����ϵ��
	en_ACTIVESKILLL_EFFECT = 224,			//�����Ч��
	en_DAMAGETOPAWN_EFFECT = 225,			//�˺�ת�Ƶ�����
	en_CHANGESHAP = 226,					//����Ч��
};
//------------------------------------------------------------------------
struct IEffect
{
	// ��  �������ݲ�������	
	// ��  ����[����] long *pParams  - �����б�
	// ��  ����[����] long lLen  - ����
	// ����ֵ��BOOL - �����ɹ��򷵻�TRUE�����򷵻�FASLE
	virtual BOOL Create(const long *pParams, long lLen) = NULL;
	// ��  ����Ч����ʼ����
	// ��  ����[����] ICreature *pMaster  - ��Ч����������
	// ��  ����[����] UID uidCreator - ������UID
	// ����ֵ��BOOL - �����ɹ��򷵻�TRUE�����򷵻�FASLE
	virtual BOOL Start(ICreature *pMaster, UID uidCreator) = NULL;
	//Ч������
	virtual BOOL End() = NULL;
	//��¡����
	virtual IEffect* Clone() = NULL;
	virtual LPCSTR GetDescription() = NULL;
	virtual void SetDescription(LPCSTR szDes) = NULL;
	virtual void Release() = NULL;
};
//------------------------------------------------------------------------
struct IEffectFactory
{
	virtual void Release() = NULL;	
	// ����Ч��
	virtual BOOL CreateEffect(long lID, IEffect **ppEffect) = NULL;
	// ����ID��ȡ������Ϣ
	virtual LPCSTR GetDescription(long lID) = NULL;
};
//------------------------------------------------------------------------
struct IClientGlobal;
//------------------------------------------------------------------------
class CEffectFactoryHelper
{
private:
	typedef BOOL (*ProcCreateEffectFactory)(IEffectFactory **ppEffectFactory, 
		IClientGlobal *pClientGlobal, const char *pFileName);	
	
public:
	CEffectFactoryHelper()
	{
		m_hDll = NULL;
		m_pEffectFactory = NULL;
		m_szErrMsg[0] = 0;
	}
	~CEffectFactoryHelper()
	{
		Close();
		if (m_hDll != NULL)//ע�⣬�����lib��ʽ������m_hDll����NULL,���������::FreeLibary()���ᱻ����
		{
			::FreeLibrary(m_hDll);
			m_hDll=NULL;
		}
	}	
	
	// ��  ��������Ч��������
	// ����ֵ��BOOL - �����ɹ��򷵻�TRUE,���򷵻�FALSE
	BOOL Create(IClientGlobal *pClientGlobal, const char *pFileName = NULL)
	{
		Close();
		try
		{
			if (m_hDll == NULL)
			{
				//��һ�ε���ʱδ����dll,������м���dll
#ifdef _DEBUG
				m_hDll = ::LoadLibrary("FKEffectClient_D.dll");
				if (m_hDll == NULL)
					throw "�޷����� FKEffectClient_D.dll";
#else
				m_hDll = ::LoadLibrary("FKEffectClient.dll");
				if (m_hDll == NULL)
					throw "�޷����� FKEffectClient.dll";
#endif
			}
			//������dll��������������ӿڶ���Ĵ�������
			ProcCreateEffectFactory proc = NULL;
			proc=(ProcCreateEffectFactory)::GetProcAddress(m_hDll,
				"CreateEffectFactory");
			if (proc == NULL)
				throw "Can't GetProcAddress('CreateEffectFactory')";
			if (!proc(&m_pEffectFactory, pClientGlobal, pFileName))
				throw "CreateEffectFactory() error!";
			
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
		if (m_pEffectFactory != NULL)
		{
			m_pEffectFactory->Release();
			m_pEffectFactory = NULL;			
		}
	}
	//����->,��helper���������ֱ��תΪ�ӿڵ�����
	IEffectFactory* operator ->()
	{
		return m_pEffectFactory;
	}
	IEffectFactory* GetEffectFactory()
	{
		return m_pEffectFactory;
	}
	//�жϽӿ�ָ���Ƿ���Ч
	BOOL IsValid()
	{
		return (m_pEffectFactory != NULL);
	}
	char* GetErrMsg()
	{
		return m_szErrMsg;
	}
private:
	IEffectFactory *m_pEffectFactory;					//�ӿڶ���
	HINSTANCE m_hDll;									//��̬���ӿ��������־
	char m_szErrMsg[128];								//������Ϣ
};
//------------------------------------------------------------------------