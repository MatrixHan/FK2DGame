/**
*	created:		2012-7-1   19:47
*	filename: 		ResLoader
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�������нű���װ�����ȡ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#ifdef _DEBUG
#define __USE_MALLOC
#endif
//------------------------------------------------------------------------
#include <map>
using std::map;
//------------------------------------------------------------------------
#include "../../FKGame/Common/CreatureViewDef.h"
#include "ObjPool.h"
//------------------------------------------------------------------------
#define				MAX_CREATURE_TYPE		200		// �������������
#define				MAX_DELAY_TYPE			9		// ���֡��ʱ�ȼ�
#define				MAX_SPEED_GRADE_TYPE	9		// ����ٶȵȼ�����·���ܲ������������ȵĵȼ���
#define				MAX_STEP_TYPE			3		// ����ƶ��������ࣨ��·���ܲ���������)

// ��ģ����ص����нű�����
enum enScriptTypeID
{
	stBmp = 0,
	stAni,
	stMagic,
	stNpc,
	stMonster,
	stActor,
	stSkill
};

// ֡����������ļ��ṹ
struct STypeContext						//��������������
{
	BYTE			m_nX;				//�У�ʵ��״̬����������
	BYTE			m_nY;				//�У���������һ����8
	BYTE			m_nZ;				//���ᣬ23��״̬���������е���״̬���֡����
	BYTE			m_nIndexWide;		//��������������ȣ�Ҳ����ʵ�ʲ���������Ч���⣩
	BYTE			m_nIndexCount;		//�����˼�����������
};

// ������Դ��IDƫ��
enum IDOffset
{
	ioStatic	= 0,
	ioAni		= 200000,
	ioMagic		= 300000,
	ioCreature	= 400000,
};

struct SMonsterViewCreateParam;
struct SMagicCreateParam;
struct SAniObjCreateParam;
struct SStaticObjCreateParam;
struct SAniContext;
class CResLoader  
{	
public:
	CObjectPool<>		m_ObjectPool;
//����Ķ���fsp�ļ�ϵͳ��������ͼ����Դ
	IFileSystem*		m_pfsBmp;
	IFileSystem*		m_pfsAni;
	IFileSystem*		m_pfsMagic;
	IFileSystem*		m_pfsCreature;

#ifdef _USE_BINSCRIPT
	IBinScript*			m_pScpBmp;
	IBinScript*			m_pScpBodyPart;
	IBinScript*			m_pScpCreature;
	IBinScript*			m_pScpAni;
#else
	IResScript*			m_pScpBmp;
	IResScript*			m_pScpBodyPart;
	IResScript*			m_pScpCreature;
	IResScript*			m_pScpAni;
#endif
	//�����վλ�ű�
	IResScript*			m_pScpNpcOcc;
	IResScript*			m_pScpMagic;
	IResScript*			m_pScpSkill;
	IResScript*			m_pScpColorList;

private:
	//֡�����ļ��ṹ�Լ����ݱ���
	BYTE					*m_pFrameSort[MAX_CREATURE_TYPE];	//���������б�
	BYTE					m_nTypes;							//����������������0����1��ʼ����Ϊ�Ǽ�¼�м������ͣ����ǵڼ�������
	STypeContext			*m_tcContext[MAX_CREATURE_TYPE];	//�����ļ�ͷ��Ϣ
	BYTE					*m_pTypeIndex[MAX_CREATURE_TYPE];	//������������
	//����֡��ʱ�ȼ�
	int*					m_FrameDelay[MAX_CREATURE_TYPE];	//����֡��ʱ�ȼ����ݣ�9���ȼ�������ֻ�Ǳ���ָ��
	int*					m_WalkGrade[MAX_CREATURE_TYPE];		//������·�ٶȵȼ����ݣ�9���ȼ�������ֻ�Ǳ���ָ��
	int*					m_RunGrade[MAX_CREATURE_TYPE];		//�����ܲ��ٶȵȼ����ݣ�9���ȼ�������ֻ�Ǳ���ָ��
	int*					m_RideGrade[MAX_CREATURE_TYPE];		//���������ٶȵȼ����ݣ�9���ȼ�������ֻ�Ǳ���ָ��
	int*					m_AttackGrade[MAX_CREATURE_TYPE];	//���湥���ٶȵȼ����ݣ�9���ȼ�������ֻ�Ǳ���ָ��
	float*					m_fStep[MAX_CREATURE_TYPE];			//����Step
	BYTE*					m_DelayIndex[MAX_CREATURE_TYPE];	//����֡��ʱ����
	BYTE*					m_StateIndex[MAX_CREATURE_TYPE];	//����״̬����
	BYTE*					m_AttackIndex[MAX_CREATURE_TYPE];	//����״̬����������
	BYTE*					m_AttackSoundIndex[MAX_CREATURE_TYPE];	//����״̬������Ч����������
	int						m_nHalfHeigh[MAX_CREATURE_TYPE];	//����������
	int						m_nWholeHeight[MAX_CREATURE_TYPE];	//��������ȫ��
	BYTE					m_nFallowStateID[MAX_CREATURE_TYPE];//�������ж���ID
	int						m_nBeHitLoopBegin[MAX_CREATURE_TYPE];//���ж���ѭ����ʼ֡
	int						m_nBeHitLoopEnd[MAX_CREATURE_TYPE];//���ж���ѭ����ʼ֡

	BYTE*					m_bytePartFlag[MAX_CREATURE_TYPE];	//���桰������־������ ֻ������༭���õ�
	POINT					m_ptAnchor[MAX_CREATURE_TYPE];		//����ê��

public:
	CResLoader();
	virtual ~CResLoader();
	void Close();
	BOOL InitFileSystem(IClientGlobal* pGlobal);
	BOOL InitDelayAndStep(IClientGlobal* pGlobal);
	//ר�ö�ȡ֡����ű�
	BOOL LoadFrameSort(IClientGlobal* pGlobal);
	
	// �ж�״̬�Ƿ����
	BOOL GetStateEnable(int nType, int nState);

	//����֡��ʱ�ȼ���ȡ��Ӧ����·����ʱ
	int GetWalkDelayByGrade(int nType, int nGrade);
	//����֡��ʱ�ȼ���ȡ��Ӧ���ܲ�����ʱ
	int GetRunDelayByGrade(int nType, int nGrade);
	//����֡��ʱ�ȼ���ȡ��Ӧ����������ʱ
	int GetRideDelayByGrade(int nType, int nGrade);
	//����֡��ʱ�ȼ���ȡ��Ӧ�Ĺ�������ʱ
	int GetAttackDelayByGrade(int nType, int nGrade);

	//��ȡ״̬������
	int GetAttackFrame(int nType, int nState);

	//��ȡ������Ч������
	int GetAttackSoundFrame(int nType, int nState);

	//����֡��ʱ�ȼ���ȡ��Ӧ�ȼ�������
	int GetDelayByGrade(int nType, int nGrade);
	//��ȡ֡����
	int GetFrameSort(BYTE *pBuf, SAniContext AniContext);
	//��ȡ֡��ʱ
	int GetFrameDelay(int nType, int nState);
	float GetStep(int nType, int nState);
	//��ȡ�����ߣ�ȫ��
	int GetHalfHeigh(int nType)
	{
		return m_nHalfHeigh[nType];
	}
	int GetWholeHeigh(int nType)
	{
		return m_nWholeHeight[nType];
	}
	int GetFallowStateID(int nType)
	{
		return m_nFallowStateID[nType];
	}

	int GetBeHitLoopBegin(int nType)
	{
		return m_nBeHitLoopBegin[nType];
	}

	int GetBeHitLoopEnd(int nType)
	{
		return m_nBeHitLoopEnd[nType];
	}

	//��ȡ���NPC��վλ��ʽ
	BOOL GetNpcOccupantInfo(DWORD dwResID, unsigned char* buf, int nLen);

	//��ȡê��
	void GetAnchor (int nType, POINT& point);
	//���ز���״̬��־
	BYTE* GetStateFlag(int nModuleType, DWORD dwID);
	//��ȡ������ɫ��
	int* GetHSLpal(DWORD dwID, int nPalType);
	//�ݹ����ResObj
	IResObject* SearchObj(IResObject* pParent, DWORD dwID);		//����ID����,�ù��ܺ����õ�
	IResObject* SearchObj(IResObject* pParent, LPCSTR pName);	//�������ֲ���
	//��ȡ�����NPC����������Ӧ��ID,�ϲ��ṩ�ڴ�
	BOOL GetMonsterbuf(DWORD dwID, DWORD *pModuleID, BYTE *pAlpha, BYTE *pPal);
	//��������������ȡ��������
	int GetPartIndexBySortIndex(int nType, int nSort);

	int ReLoadScript(int nScpTypeID);

	void AddSize(DWORD dwSize);
	IPoolNodeObject* RequestPic(DWORD dwResID);
	IPoolNodeObject* RequestAni(DWORD dwResID);
	IPoolNodeObject* RequestMagic(DWORD dwResID);
	IPoolNodeObject* RequestCreature(DWORD dwResID);
	BOOL ReleaseRes(DWORD dwResID);

public: // ������һЩ��ű������ĺ���
	BOOL GetResID(int nIndex, DWORD& dwResID);

	// ��ȡ�����������
	BOOL GetMonsterCreateParam(DWORD dwMstID, SMonsterViewCreateParam& mvCreateParam);
	BOOL GetMagicCreateParam(DWORD dwResID, SMagicCreateParam& mCreateParam);
	BOOL GetAniObjCreateParam(DWORD dwResID, SAniObjCreateParam& aoCreateParam);
	BOOL GetStaticObjCreateParam(DWORD dwResID, SStaticObjCreateParam& soCreateParam);
	
	BOOL GetAniObjOccupantInfo(DWORD dwResID, unsigned char* buf, int nLen);
	BOOL GetStaticObjOccupantInfo(DWORD dwResID, unsigned char* buf, int nLen);
	int  GetStaticObjOccupantOption(DWORD dwResID);
	BOOL GetActorUserPal(DWORD dwResID, int& nCount, BYTE** ppUserPal);
	BOOL GetMonsterUserPal(DWORD dwResID, int& nCount, BYTE** ppUserPal);

	// ��������ħ���ű��Ķ�ȡ����
	int ReadAttachMagic(void* obj, void* buf);
	int ReadBaseMagic(void* obj, void* buf);
	int ReadLifeMagic(void* obj, void* buf);
	int ReadThreeStateMagic(void* obj, void* buf);
	int ReadRainMagic(void* obj, void* buf);
	int ReadRingMagic(void* obj, void* buf);
	int ReadSingleDirMagic(void* obj, void* buf);
	int ReadEightDirMagic(void* obj, void* buf);
	int ReadGeneralMagic(void* obj, void* buf);
	int ReadMouseMagic(void* obj, void* buf);
};
//------------------------------------------------------------------------
extern CResLoader g_ResLoader;
//------------------------------------------------------------------------