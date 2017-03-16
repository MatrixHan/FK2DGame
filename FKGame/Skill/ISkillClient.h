/**
*	created:		2012-6-30   1:12
*	filename: 		ISkillClient
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���ܹ���ģ��ӿ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#define INVALID_SKILLID				0xFFFF	// ��Ч����ID
#define INVALID_SKILLLV 			0xFFFF	// ��Ч�ȼ�ID
//------------------------------------------------------------------------
#define MAX_SKILL_DESC_LEN			512		// ����˵����󳤶�
//------------------------------------------------------------------------
#include "../SceneObj/IThingPart.h"
//------------------------------------------------------------------------
// �����б�������ݽṹ
struct SSkillData
{
	WORD	wSkillID;			// ����ID
	WORD	wLevel;				// ���ܵȼ�
};
//------------------------------------------------------------------------
// ʹ�ü��ܵ����ݰ�
struct SUseSkillContext
{
	SSkillData	SkillData;		// ����ID��ȼ�
	UID			uidTarget;		// �ͷż���ʱ���㵽��λ���ϵĶ���
	POINTS		ptTargetTile;	// �ͷż���ʱ���㵽��λ��
	WORD		wCheckId;		// ���ܵĽϼ�ID��Ψһȷ��
};
//------------------------------------------------------------------------
// ��ֹʹ�ü��ܵ���������
enum enForbidSkillType
{
	enFORBID_SKILL_TYPE_ALL = 0,	// ��ֹ���м��ܰ�����ͨ����ʩ��
	enFORBID_SKILL_TYPE_MAGIC,		// ��ֹ���м���ʩ�Ų�������ͨ����

	enFORBID_SKILL_TYPE_MAX_COUN,	// ��������Ŀ
};
//------------------------------------------------------------------------
// ������Ϣ
enum enSkillInfo
{
	// ������ֵ
	enSKILL_INFO_GUNSHOT = 0,			// ��������(��λ��Tile,Ĭ��Ϊ0�޾�������)
	enSKILL_INFO_TESTBLOCK,				// �ж��赲��ʽ�����С����ߡ������赲��
	enSKILL_INFO_CONSUMEMP,				// �˼������ĵ�MP(Ĭ��Ϊ0)
	
	// ѧϰ�������
	enSKILL_INFO_STUDYNEED_LEVEL,		// ѧϰ���ܣ��ȼ�����
	enSKILL_INFO_STUDYNEED_EXP,			// ѧϰ���ܣ���������
	enSKILL_INFO_STUDYNEED_MONEY,		// ѧϰ���ܣ���Ǯ����

	// ʱ�����
	enSKILL_INFO_COLDTIMEID,			// ��ȴʱ��ID
	enSKILL_INFO_TIMEPREP,				// ����ʱ��
	enSKILL_INFO_TIMEATTACK,			// ����ʱ�䣨��Ĭ��ֵ���������������Ϊ��ֱʱ�䣬��������������������ʱ���޷�������������
	enSKILL_INFO_TIMECTRL,				// ��������ʱ�䣬�ڸ�ʱ������ұ���������������ֹͣʩ����Ĭ��Ϊ0��
	
	// ��Ʒ����
	enSKILL_INFO_NEEDGOODS,				// �Ƿ���Ҫ��Ʒ��0��ʾ����Ҫ��1��ʾҪ���ĵ���2��ʾֻҪ���ھ�����
	enSKILL_INFO_GOODSID,				// ��ƷID
	enSKILL_INFO_GOODSNUM,				// ��Ʒ��Ŀ
	
	// ���Ч�����
	enSKILL_INFO_HAVEDAM,				// �Ƿ����˺�
	enSKILL_INFO_DAMTYPE,				// �˺���������
	
	// �������
	enSKILL_INFO_VIEWLEVEL,				// ���ܶ�Ӧ�Ķ����ȼ�

	enSKILL_INFO_MAX_COUNT,				// ��������Ϣ��Ŀ
};
//------------------------------------------------------------------------
// �жϼ����ܷ�ʹ�õķ�ʽ
enum enUseSkillType
{
	enATTACK_BEGIN = 0,			// �յ���������
	enATTACK_PRE_OVER,			// ��������ʱ�ļ����ܷ�ʹ���ж�
};
//------------------------------------------------------------------------
// ��������
enum enSkillType
{
	enSKILL_TYPE_INITIATIVE = 0,// ��������
	enSKILL_TYPE_PASSIVE,		// ��������

	enSKILL_TYPE_ERROR,			// ��������
};
//------------------------------------------------------------------------
// ���ܹ�������
enum enSkillAttackType
{
	enSKILL_ATTACK_TYPE_TARGER_ENEMY = 0,	// Ҫѡ�����ֻ��ѡ��(������)����
	enSKILL_ATTACK_TYPE_TARGER_FRIEND,		// Ҫѡ�����ֻ��ѡ��(������)�ѷ�
	enSKILL_ATTACK_TYPE_TARGER_CORPSE,		// Ҫѡ�����ֻ��ѡ��ʬ��
	enSKILL_ATTACK_TYPE_TARGER_SELFBABY,	// Ҫѡ�����ֻ��ѡ���Լ��ĳ���
	enSKILL_ATTACK_TYPE_TARGER_ALL,			// Ҫѡ���������(������)���ﶼ����

	enSKILL_ATTACK_TYPE_GROUND,				// Ҫѡ�����

	enSKILL_ATTACK_TYPE_NO_SELECT,			// ����ѡ�����ֻҪ���˾��Զ�ʩ��

	enSKILL_ATTACK_TYPE_ERROR,				// ��������
};
//------------------------------------------------------------------------
// ����ʹ��ʧ�ܵķ���ֵ
enum enCanUseSkillInfo
{
	enUSE_SKILL_SUCCEED = 0,				// ����ʹ�øü���

	enUSE_SKILL_FAIL_TOO_FAR,				// ����̫Զ
	enUSE_SKILL_FAIL_NO_MAGIC,				// MP����
	enUSE_SKILL_FAIL_NO_TARGER,				// û��Ŀ���Ŀ����ʧ
	enUSE_SKILL_FAIL_WEAPEN_ERROR,			// ��������
	enUSE_SKILL_FAIL_GOODS_ERROR,			// ��Ʒ���󲻹�
	enUSE_SKILL_FAIL_FORBID,				// ����ħ�Ƚ�ֹʩ����
	enUSE_SKILL_FAIL_STATUS_NEED,			// ״̬���󲻶�
	enUSE_SKILL_FAIL_SKILL_SHAPE,			// ��ǰ��̬����
	enUSE_SKILL_FAIL_HAVE_BLOCK,			// �м����ϰ�
	enUSE_SKILL_FAIL_IN_COLD_TIME,			// ���ܻ�δ��ȴ���
	enUSE_SKILL_FAIL_LEVEL_NO_ENOUGH,		// ���ܵȼ����������ѧ��ļ��ܵȼ�

	enUSE_SKILL_FAIL_STATE_IN_PREPARE,		// �������У�����ʩ����������
	enUSE_SKILL_FAIL_STATE_IN_ATTACK,		// �ڹ���״̬�У�����ʩ����������
	enUSE_SKILL_FAIL_STATE_IN_CTRL,			// �ڳ���ʩ��״̬�У�����ʩ����������

	enUSE_SKILL_FAIL_SKILL_TYPE_ERROR,		// ����ʹ�ô��󣨱���ʹ���˱������ܣ�
	enUSE_SKILL_FAIL_ATTACK_TYPE_ERROR,		// ����ʹ�ô��󣨱���Եз�ʹ�����漼�ܣ�����Ժ�Ҫ��ϸ�֣�

	enUSE_SKILL_FAIL_OTHER,					// �����쳣,�����쳣�ǲ��÷�����ȴ�����ˣ����缼�ܵȼ�������Чֵ
};
//------------------------------------------------------------------------
// �۲��߹��ĵļ���ʹ����Ϣ���
enum enObserverUseSkillInfo
{
	enOBSERVER_USE_SKILL_SUCCEED = 0,				// ����ʹ�øü���

	enOBSERVER_USE_SKILL_FAIL_NO_MAGIC,				// MP����
	enOBSERVER_USE_SKILL_FAIL_WEAPEN_ERROR,			// ��������
	enOBSERVER_USE_SKILL_FAIL_FORBID,				// ����ħ�Ƚ�ֹʩ����
	enOBSERVER_USE_SKILL_FAIL_STATUS_NEED,			// ״̬���󲻶�
	enOBSERVER_USE_SKILL_FAIL_SKILL_SHAPE,			// ��ǰ��̬����

	enOBSERVER_USE_SKILL_FAIL_OTHER,				// �����쳣
};
//------------------------------------------------------------------------
// ��������ļ��ܽ׶�״̬
enum enSkillStatus
{
	enSKILL_STATUS_NORMAL,					// ��ʩ�ż���״̬
	enSKILL_STATUS_PREPARE,					// ����״̬
	enSKILL_STATUS_ATTACK_ONCE,				// ʩ�ż���״̬
	enSKILL_STATUS_ATTACK_CTRL,				// ����ʩ�ż���״̬
};
//------------------------------------------------------------------------
// �ж��ܷ�ѧϰ���ܷ��صĽ��
enum enStudySkillResult
{
	enSTUDY_SKILL_RESULT_SUCCEED,			// ����ѧϰ�����ü���
		
	enSTUDY_SKILL_RESULT_FAIL_MONEY,		// ���������ü��ܡ���Ʊ����
	enSTUDY_SKILL_RESULT_FAIL_EXP,			// ���������ü��ܡ����鲻��
	enSTUDY_SKILL_RESULT_FAIL_OTHER,		// ���������ü��ܡ�����ԭ�򣬴���
};
//------------------------------------------------------------------------
// ������б�
enum enSkillActive
{
	enSKILL_ACTIVE_NONE = 0,				// ���輤��
	enSKILL_ACTIVE_FUCHOU,					// �������Ѫ��ɱ������
	
	enSKILL_ACTIVE_MAX,						// ��󼤻�����
};
//------------------------------------------------------------------------
// ���ܹ۲���
class ISkillObserver
{
public:
	// �����ܷ����ı�ʱ����
	// nInfo	����ǰ�ü��ܿɷ�ʹ�õ���Ϣ
	// IconID	���ü�����ʾ��ͼ��
	// UpIconID ��������ƽ�����ʱ����ʾ��ͼ��
	virtual void UpdateSkillInfo(enObserverUseSkillInfo nInfo, int IconID, int UpIconID)	= NULL;
};
//------------------------------------------------------------------------
// ���ܿ�����,���ǰ�
struct ISkillControl : public IThingPart
{
	// ȡ�øü��ܵĵȼ�(0Ϊδѧ��)
	virtual WORD GetSkillLevel(WORD wSkillID)									= NULL;

	// ȡ����������ļ��ܽ׶�״̬
	virtual enSkillStatus GetSkillStatus()										= NULL;


	// ��ֹʩ�ż���
	virtual void ForbidSkill(enForbidSkillType nType)							= NULL;
	
	// ȡ����ֹʩ�ż���
	virtual void UnForbidSkill(enForbidSkillType nType)							= NULL;
	

	// ����ĳ����
	virtual void ActiveSkill(enSkillActive nType)								= NULL;
	
	// ȡ������ĳ����
	virtual void UnActiveSkill(enSkillActive nType)								= NULL;

	// ��ȡ��ǰĳ�����ܵļ������
	virtual BOOL GetCurForbidSkill(enSkillActive nType)							= NULL;
	

	// �ı似��ʹ����̬
	virtual void ChangeSkillShape()												= NULL;
	
	// ȡ���ı似��ʹ����̬
	virtual void UnChangeSkillShape()											= NULL;
	
	// ��ȡ��ǰ����ʹ����̬ FALSEΪδ�ı� TUREΪ�ı���
	virtual BOOL GetCurSkillShape()												= NULL;


	// ��ʼ����ĳ�����ܵı仯
	virtual void SubscibeSkill(WORD	wSkillID, ISkillObserver *pSkillObserver)	= NULL;

	// ȡ������ĳ�����ܵı仯
	virtual void UnSubscibeSkill(WORD wSkillID, ISkillObserver *pSkillObserver)	= NULL;
};
//------------------------------------------------------------------------
// ���ܹ�����,���������ܼ����ܿ�����
struct ISkillManager
{
	// �������ܿ�����
	virtual ISkillControl *CreateSkillControl()									= NULL;

	// ȡ�øü��ܵ����ȼ�
	virtual int GetSkillMaxLevel(WORD wSkillID)									= NULL;

	// ȡ��ĳ�ȼ��ļ��ܵ�����
	virtual void GetSkillName(SSkillData SkillData, LPSTR buf)					= NULL;

	// ȡ��ĳ�ȼ��ļ��ܵ�˵��
	virtual void GetSkillDesc(SSkillData SkillData, LPSTR buf)					= NULL;

	// ȡ��ĳ�ȼ��ļ��ܵ������Ϣ ���󷵻�-1
	virtual long GetSkillInfo(SSkillData SkillData, enSkillInfo nInfo)			= NULL;

	// ȡ�øü����Ƿ�����
	virtual BOOL GetSkillBeLock(WORD wSkillID)									= NULL;

	// ȡ�øü������ͣ��������ܣ��������ܣ�������
	virtual enSkillType GetSkillType(WORD wSkillID)								= NULL;

	// ȡ�øü��ܵĹ�����ʽ��Ҫѡ����󣬶Եأ�������
	virtual enSkillAttackType GetSkillAttackType(WORD wSkillID)					= NULL;

	// ȡ�ü���ͼ�꣬����Ĭ�ϵ�ͼ��
	virtual int	GetDefaultSkillIcon(WORD wSkillID)								= NULL;

	// �ж��ܷ�ʹ�øõȼ�����
	virtual enCanUseSkillInfo CanUseSkill(SUseSkillContext * pContext,
		enUseSkillType nType = enATTACK_BEGIN, BOOL bNotJudgeDistant = FALSE)	= NULL;

	// ȡ�����ĳְҵ����ѧ�ļ����б�nSize��������Ĵ�С��INVALID_SKILLID��ʾ��Ч�ļ���ID
	virtual WORD* GetOccupationSkillList(int nOccupation, int &nSize)			= NULL;

	virtual void Release()														= NULL;
};
//------------------------------------------------------------------------
struct IClientGlobal;					// �����ȫ��Global
#define VERSION_QUERY_SKILLCLIENT	1	// �汾��
//------------------------------------------------------------------------
extern "C" __declspec(dllexport) BOOL ProcCreateSkillManager(DWORD dwVersion , ISkillManager** ppManager, IClientGlobal* pGlobal);

// ������
class CSkillHelper
{
public:
	ISkillManager	*m_pSM;
private:
	HINSTANCE		m_hdll;
public:
	// �����๤��
	typedef BOOL (*ProcCreateSkillManager)(DWORD dwVersion , ISkillManager** ppManager, IClientGlobal* pGlobal);	
	
	CSkillHelper()
	{
		m_hdll = NULL;
		m_pSM = NULL;
	}
	
	~CSkillHelper()
	{
		Close();
		if (m_hdll)
		{
			FreeLibrary(m_hdll);
			m_hdll = NULL;
		}	
	}

	void Close()
	{
		if (m_pSM)
		{
			m_pSM->Release();
			m_pSM = NULL;
		}		
	}

	ISkillManager* operator ->()
	{
		return m_pSM;
	}
	
	BOOL CreateSkillManager(IClientGlobal* pGlobal)
	{
		try
		{
			LoadDLL();
			
			ProcCreateSkillManager proc;
			proc = (ProcCreateSkillManager)GetProcAddress(m_hdll, "CreateSkillManager");
			
			if(proc == NULL)
				throw "Can not GetProAddress('CreateSkillManager')";
			
			if(!proc(VERSION_QUERY_SKILLCLIENT, &m_pSM, pGlobal))
				throw "CreateSkillManager() catch error!";
			return m_pSM != NULL;
		}
		catch(LPCSTR szMsg)
		{
			OutputDebugString(szMsg);OutputDebugString("\r\n");
			return NULL;
		}
		catch(...)
		{
			OutputDebugString("CreateSkillManager() catch error\r\n");
			return NULL;
		}
	}
	
	void LoadDLL()
	{
		if(m_hdll == NULL)
		{
#ifdef _DEBUG
			m_hdll = LoadLibrary("FKSkillClient_D.dll");
			if(m_hdll == NULL)
				throw "�޷����� FKSkillClient_D.dll";
#else
			m_hdll = LoadLibrary("FKSkillClient.dll");
			if(m_hdll == NULL)
				throw "�޷����� FKSkillClient.dll";
#endif
		}
	}
};
//------------------------------------------------------------------------