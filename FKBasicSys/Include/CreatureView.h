/**
*	created:		2012-7-2   2:16
*	filename: 		CreatureView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������ͼ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Magic.h"
#include "AniGroup.h"
#include "AttachObj.h"
#include "Pal.h"
#include "ResLoader.h"
#include "Particle.h"
#include "../../FKChatClient/IPaoPao.h"
#include "../../FKGame/Timer/ITimerAxis.h"
#include "../../FKGame/MapView/MapItemView.h"
#include "../../FKGame/Common/CreatureViewDef.h"
//------------------------------------------------------------------------
class CMagicMgrShadow;
//------------------------------------------------------------------------
#define TIME_EVENT_ID_FRAME			0		// ����ʱ��ID
#define TIME_EVENT_ID_SHORT_STEPS	1		// �鲽ʱ��ID
#define TIME_EVENT_ID_DIE			2		// ����ʬ�屣��ʱ��ID
#define TIME_EVENT_ID_DIE_ANI		3		// ����ʬ����ʧ����ʱ��ID

#define	TIME_SHORT_STEPS			5		// �������ƶ������е��鲽ʱ����
#define TIME_DEFAULT_CORPER			10000	// ����ʬ�屣��ʱ��
#define TIME_DEFAULT_CORPER_ANI		40		// ����ʬ����ʧ����ʱ��

#define	SKILL_MAX_COUNT				98		// ������Ŀ

#define CREATURE_MAX_NAME			36		// ����������ֳ���(�ֽ�)
#define BLOOD_SLOT_WIDTH			40		// Ѫ�����
#define BLOOD_SLOT_HEIGHT			3		// Ѫ���߶�

#define SIT_HEIGHT_OFFSET			20		// ����״̬��ȫ��ƫ��

#define MAX_CHANGE_SHAP_LV			10		// �����������ȼ�
//------------------------------------------------------------------------
struct SItemShowRect
{
	BOOL	bIni;
	RECT	rc;
};
//------------------------------------------------------------------------
// MonsterView�浵��ʽ
struct SMonsterViewArchive
{
	int		nType;
	DWORD	dwResID;
};
//------------------------------------------------------------------------
struct SMonsterViewCreateParam
{
	SMonsterViewCreateParam()
	{
		memset (this, 0, sizeof(SMonsterViewCreateParam));
	}

	int		nHalfHeightOffset;				//�����ߣ���ʾ�˺�ֵ��λ��
	int		nHeightOffset;					//����ȫ�ߣ���ʾ����Ѫ����λ��
	int		nWalkGrade;						//�����ƶ��ȼ���ʵ������֡��ʱ
	int		nRunGrade;						//�����ܶ��ȼ���ʵ������֡��ʱ
	int		nAttackGrade;					//���﹥���ȼ���ʵ������֡��ʱ
	int		nShape;							//�������Σ��о��͹���С�͹������͹���
	int		nType;							//�������ͣ���������������һ��
	BYTE	nStateFlag[CrtPartMax];			//���ﲿ����־�������ù����м�������
	int		nActStateCommonAttack[3];		//��ͨ��������id
	int		nActStateJuqi;					//��������
	int		nActStateSkillAttack;			//���ܹ�������
	int		nSoundIDCommonAttack;			//��ͨ������Ч
	int		nSoundIDFallow;					//������Ч
	int		nSoundIDBeHit;					//������Ч
	int		nSoundIDDie;					//������Ч
};
//------------------------------------------------------------------------
class CCreatureView : public CMapItemView, public ITimerSink
{
private:
	struct SStep
	{
		float x, y;
	};

	struct SDblPoint
	{
		POINT ptTile, ptOff;
	};

public:
	CCreatureView();
	virtual ~CCreatureView();
	void Close();

// ITimerSink
public:
	virtual void OnTimer(DWORD dwEventID)	{}

// CMapItemView
public:
	virtual void	SetFlagColorOnMinmap(DWORD dwColor);						// ������С��ͼ�ϵ���ɫ
	virtual DWORD	GetFlagColorOnMinmap();										// ȡ����С��ͼ�ϵ���ɫ
	
	virtual BOOL	GetVisibleOnMinmap();										// ȡ���Ƿ���С��ͼ����ʾ

	virtual void	SetItemData(DWORD dwData);									// ���ö����˽������
	virtual DWORD	GetItemData() const;										// ��ȡ�����˽������

	virtual void	SetTile(POINT& ptTile, IMapView* pMapView);					// �����ڵ�ͼ�ϵ�λ��
	virtual POINT	GetTile();													// ��ȡ�ڵ�ͼ�ϵ�λ��

	virtual BOOL	SetSelected(BOOL bSelected = TRUE, BOOL bHighLight = TRUE);	// ���ö���ѡ��״̬
	virtual BOOL	SetLocked(int nLockType = 1);								// ���ö�������״̬

	virtual DWORD	GetResID();													// ��ȡ��ԴID
	virtual RECT	GetShowRect();												// ��ȡ������δ�С��Χ
	virtual void	ClearShowRect();											// ���������δ�С��Χ
	virtual POINT	GetAnchor() const;											// ��ȡê��

	virtual BOOL	HitTest(const RECT& rcScreen);								// ���Ե��û�����ĳ��С����ʱ�Ƿ�ѡ��˶���
	virtual BOOL	OnEnterViewport(BOOL bVisable);								// ����������ӿں��뿪�ӿ�ʱ���ص�
	
	virtual BOOL	IsSinking();												// �Ƿ�Ǳˮ���۹���

	virtual int		OnCommand(DWORD dwCmdID, DWORD dwParam1,DWORD dwParam2);	// ͨ�ÿ�������
	
public:
	// ���������Ľ�������(CreatureViewCommond.cpp��ʵ��)
	typedef int (CCreatureView::*ProcCommand)		(DWORD dwParam1, DWORD dwParam2);
	
	virtual int cmdSetProperty(DWORD dwParam1, DWORD dwParam2);				// ��������
	virtual int cmdGetProperty(DWORD dwParam1, DWORD dwParam2);				// ��ȡ����

	virtual int cmdMove(DWORD dwParam1, DWORD dwParam2);					// �ƶ�
	virtual int cmdStand(DWORD dwParam1, DWORD dwParam2);					// վ��
	virtual int cmdSetting(DWORD dwParam1, DWORD dwParam2);					// ��������(������)
	virtual int cmdFallow(DWORD dwParam1, DWORD dwParam2)		{return 0;}	// ����

	virtual int	cmdBorn(DWORD dwParam1, DWORD dwParam2)			{return 0;}	// ��ɫ����
	virtual int	cmdUpgrade(DWORD dwParam1, DWORD dwParam2)		{return 0;}	// ��ɫ����

	virtual int cmdChangeModule(DWORD dwParam1, DWORD dwParam2)	{return 0;}	// ��װ
	virtual int cmdSpecialModule(DWORD dwParam1, DWORD dwParam2){return 0;}	// ���ý�ɫĬ�ϲ���
	virtual int	cmdModulePalType(DWORD dwParam1, DWORD dwParam2){return 0;}	// ��ɫ������ĵ�ɫ����������
	virtual int	cmdModuleAlpha(DWORD dwParam1, DWORD dwParam2)	{return 0;}	// �ı��ɫ͸����

	virtual int	cmdAppearance(DWORD dwParam1, DWORD dwParam2);				// ���
	virtual int	cmdBloodInfo(DWORD dwParam1, DWORD dwParam2);				// ��ʾѪ��
	virtual int cmdAddTitle(DWORD dwParam1, DWORD dwParam2)		{return 0;}	// ��������ӳƺ�
	virtual int cmdClearTitle(DWORD dwParam1, DWORD dwParam2)	{return 0;}	// �����������гƺ�
	virtual int	cmdChatInfo(DWORD dwParam1, DWORD dwParam2);				// ��ɫͷ����������Ϣ
	virtual int	cmdMouseAni(DWORD dwParam1, DWORD dwParam2)		{return 0;}	// ��������ʱ����·�ϵ��������µĵ������

	virtual int	cmdPreAttack(DWORD dwParam1, DWORD dwParam2);				// ����
	virtual int	cmdStopPreAttack(DWORD dwParam1, DWORD dwParam2);			// ֹͣ����
	virtual int	cmdAttack(DWORD dwParam1, DWORD dwParam2);					// ����
	virtual int	cmdStopAttack(DWORD dwParam1, DWORD dwParam2);				// ֹͣ����
	virtual int	cmdChangeLocForSkill(DWORD dwParam1, DWORD dwParam2);		// �ü���˳��
	virtual int	cmdSkillSelAni(DWORD dwParam1, DWORD dwParam2);				// ��Χ���ܵ�ѡ���Ч

	virtual int cmdBeHit(DWORD dwParam1, DWORD dwParam2);					// ���ﱻ��
	virtual int cmdDie(DWORD dwParam1, DWORD dwParam2);						// ��������

	virtual int cmdSetNpcType(DWORD dwParam1, DWORD dwParam2)	{return 0;}	// ����NPC����Ӧ��ͷ����Ч
	virtual int cmdSetTaskType(DWORD dwParam1, DWORD dwParam2)	{return 0;}	// ����NPCͷ���Ĺ�Ч

	virtual int cmdSetClanName(DWORD dwParam1, DWORD dwParam2);				// ������������
	virtual int cmdSetCityName(DWORD dwParam1, DWORD dwParam2);				// ���óǳ�����

	// ���ܵĴ�������(CreatureViewSkill.cpp��ʵ��)
	typedef BOOL (CCreatureView::*ProcCreateSkill)	(SSkillCreateParam* pParam);

	BOOL skill_NNNNNNNNNNNN(SSkillCreateParam* pParam);						// �ռ��ܻ�û��
	BOOL skill_None(SSkillCreateParam* pParam);								// ��ħ��Ч������
	BOOL skill_CommonAttack(SSkillCreateParam* pParam);						// ��ͨ����
	BOOL skill_SingleDir(SSkillCreateParam* pParam);						// ������м���(�����)
	BOOL skill_RainSkill(SSkillCreateParam* pParam);						// �൥Ԫ�����ͼ��ܵĴ�������(�籩��ѩ)
	BOOL skill_WanJianSkill(SSkillCreateParam* pParam);					// �򽣳����༼�ܵĴ�������
	BOOL skill_MultiUnitSingleTileSkill(SSkillCreateParam* pParam);			// ��ͬ��Ŀ��Ķ����Ԫħ�������������ȣ�
	BOOL skill_MultiDirSkillEx(SSkillCreateParam* pParam);					// ������м���(�籩�һ���)
	BOOL skill_SingleCirculDirSkill(SSkillCreateParam* pParam);				// 8���򵥷����ܵĴ�������
	BOOL skill_HalfCirculDirSkill(SSkillCreateParam* pParam);				// 8�������μ��ܵĴ�������
	BOOL skill_CirculDirSkill(SSkillCreateParam* pParam);					// 8����Բ�μ��ܵĴ�������
	BOOL skill_GenericSkill(SSkillCreateParam* pParam);						// ͨ�ü��ܴ���
	BOOL skill_ChangeLoc(POINT pt);											// ˳��ħ��
	
	CMagic* CreateSkillSelMagic(int nSkillID);								// ��������ѡ���Ч


	// ���úͻ�ȡ��������(CreatureViewProperty.cpp��ʵ��)
	typedef void	(CCreatureView::*ProcProperty)		(BOOL bSetProp, DWORD dwPropVal);

	virtual void propMoveStype(BOOL bSetProp, DWORD dwPropVal);				// �ƶ�����
	virtual void propTile(BOOL bSetProp, DWORD dwPropVal);					// �����TILE����
	virtual void propUserName(BOOL bSetProp, DWORD dwPropVal);				// ���������
	virtual void propShowName(BOOL bSetProp, DWORD dwPropVal);				// �Ƿ���������ʾ�Լ�������
	virtual void propNameColor(BOOL bSetProp, DWORD dwPropVal);				// �������ֵ���ɫ
	virtual void propAbsolutePosition(BOOL bSetProp, DWORD dwPropVal);		// ����ľ���λ��
	virtual void propWeaponStyle(BOOL bSetProp, DWORD dwPropVal)		{}	// ��ɫЯ�������ķ�ʽ
	virtual void propWeaponSoundID(BOOL bSetProp, DWORD dwPropVal)		{}	// ��ɫ�����Ĺ�����ЧID
	virtual void propBeHitSoundID(BOOL bSetProp, DWORD dwPropVal)		{}	// ��ɫ�����Ļ���Ŀ����ЧID
	virtual void propIsMoving(BOOL bSetProp, DWORD dwPropVal);				// �����Ƿ������ƶ�
	virtual void propState(BOOL bSetProp, DWORD dwPropVal);					// �����״̬
	virtual void propDirection(BOOL bSetProp, DWORD dwPropVal);				// ����ķ���
	virtual void propMoveSpeedLevel(BOOL bSetProp, DWORD dwPropVal);		// ������ƶ��ٶȵȼ�
	virtual void propAttackSpeedLevel(BOOL bSetProp, DWORD dwPropVal);		// ����Ĺ����ٶȵȼ�
	virtual void propGroupInfoImage(BOOL bSetProp, DWORD dwPropVal)		{};	// �����ϢͼƬ

	// �����ƶ�����(CreatureView.cpp��ʵ��)
	typedef void	(CCreatureView::*ProcMoveStepByDir)	(POINT& ptTileOffsetOld);
	
	void _move_step_north(POINT& ptTileOffsetOld);
	void _move_step_eastnorth(POINT& ptTileOffsetOld);
	void _move_step_east(POINT& ptTileOffsetOld);
	void _move_step_eastsouth(POINT& ptTileOffsetOld);
	void _move_step_south(POINT& ptTileOffsetOld);
	void _move_step_westsouth(POINT& ptTileOffsetOld);
	void _move_step_west(POINT& ptTileOffsetOld);
	void _move_step_westnorth(POINT& ptTileOffsetOld);

public:
	static ProcCommand			m_Command[CrtCmdMaxCommandCount];
	static ProcCreateSkill		m_funcSkill[SKILL_MAX_COUNT + 1];
	static ProcProperty			m_Property[CrtPropMaxPropCount];
	static ProcMoveStepByDir	m_MoveStepByDir[CrtDirMaxDirCount];

public:

	// ״̬���
	virtual void SetState(int nState)							{}		// ���õ�ǰ״̬
	virtual void StopState(int nState)							{}		// ֹͣ��ǰ״̬
	int& State();														// ��ȡ��ǰ״̬
	int  GetState();													// ��ȡ��ǰ״̬
	virtual void RestartCurState()								{}		// ������ǰ״̬

	// �������������
	void IncCurFrame();													// ��һ��
	void DecCurFrame();													// ��һ��
	bool IncAndCheckFrameOverflow();									// ת����һ�����ж��Ƿ�����������
	bool IncAndCheckFrameOverflow(int nFrameIndex);						// ת����һ�����ж��Ƿ���ڲ���
		
	void SetCurFrame(int nFrame);										// ���õ�ǰ��
	int& CurFrame();													// ��ȡ��ǰ��
	int  GetCurFrame();													// ��ȡ��ǰ��
		
	void IniTotalFrame();												// ��ʼ����ǰ״̬�������
	void SetTotalFrame(int nFrames);									// �������
	int  GetTotalFrame();												// ��ȡ�����
	
	int  GetCurAni(int nFind);											// ���ص�ǰ�ǵڼ����������������ڼ�������
	
	int GetFrameDelay(int nGrade = -1);									// ��ȡ���ӳ�

	BOOL GetPartNeedChangePal(int nModuleType);							// ��ȡ�ò����Ƿ���Ҫ�ı��ɫ��
	CAniGroup* RequestRes(int nModuleType);								// ��ȡͼ��
	
	virtual int GetNormalAttackActID()					{return -1;}	// ��ȡĬ�ϵ���ͨ��������ID
	virtual int GetJuqiActID()							{return -1;}	// ��ȡĬ�ϵ���������ID���������Ч��
	virtual int GetSkillAttackActID()					{return -1;}	// ��ȡĬ�ϵļ��ܹ�������ID���������Ч��

	void SetVisible(BOOL bVisible);
	BOOL GetVisible();
	
	virtual void SetShowRect()									{}		// ���ö�����δ�С��Χ

	// ƫ�����
	void SetHalfHeightOffset(int nOffSet);							// ���ð��
	int  GetHalfHeightOffset();										// ��ȡ���
	void SetHeightOffset(int nOffSet);								// ����ȫ��
	int  GetHeightOffset();											// ��ȡȫ��

	// �ƶ����
	virtual void SetStepWalk(float fStep);							// ������������߲���(����ֱ����Ϊ׼)
	virtual void SetStepRun(float fStep);							// ����������ܶ�����(����ֱ����Ϊ׼)
	virtual void SetStepRide(float fStep);							// ��������������ܶ�����(����ֱ����Ϊ׼)

	void GetMoveStep(int nDir, float& x, float& y);					// ��ȡ�ƶ�����
	void GetMoveStepEx(int nDir, float& x, float& y);				// ��ȡ�鲽�ƶ�����

	void SetMoveStyle(int nMoveStyle);								// �����ƶ���ʽ
	int  GetMoveStyle();											// ��ȡ�ƶ���ʽ
	int  GetMoveStateByStyle();										// ͨ���ƶ���ʽ��ȡ�ƶ�״̬

	void SetDirection(int nDir);									// ���õ�ǰ����
	int  GetDirection();											// ��ȡ��ǰ����

	void CalcSpeedMove();											// ������ǰ�ƶ��ٶ�
	SStep*	GetSpeedMove(int nDir);									// ��ȡ��ǰ�ƶ��ٶ�

	void SetCurSpeed(SStep* pSpeed);								// ���õ�ǰ�����µ��ƶ��ٶ�
	void GetCurSpeed(float& x, float& y);							// ��ȡ��ǰ�����µ��ƶ��ٶ�

	void OnMoveStep(DWORD dwPeriod);								// �����ƶ�
	void OnMoveStepEx(DWORD dwPeriod);								// ûͣ��Tile���ĵ���������ƶ�
	void MoveStep(DWORD dwPeriod);									// ���ݵ�ǰ���ƶ���ʽ�ƶ�

	BOOL IsMoving();												// �Ƿ����ƶ���
	void StopMove();												// ֹͣ�ƶ�

	void OnChangeDir();												// ������ı�ʱ����
	BOOL OnChangeTile();											// ��Tile�ı�ʱ����
	
	void AdjustDirection(SSkillCreateParam* pParam);				// У������ķ���

	void ResetMoveInfo();											// �����ƶ�����
	void SetOccupant(BOOL bOccupant);								// ���������Ƿ�ռλ
	BOOL GetOccupant();												// ��ȡ�����Ƿ�ռλ

	virtual void SetAllMoveDelay(int* pDelay)					{}	// �����ƶ�������ʱ

	// �����������
	CAttachStateObj* CreateAttachObj(SAttachStateObjBuffer* pBuf);	// ������������

	virtual void RemoveAllAttachObj()							{}	// ɾ��������������
	virtual void RemoveAllFlowObj();								// ɾ������ƮѪ����

	virtual void OnAttachObjDestroy(IAttachObj* pAttachObj)		{}	// �ص�(�����������ʱ�ص�)
	virtual void OnFlowObjDestroy(IAttachObj* pAttachObj)		{}	// �ص�(Ʈ�ֶ�������ʱ�ص�)

	virtual void StopAttachObj(IAttachObj* pAttachObj);				// ֹͣĳ���������
	void StopAllAttackAttach();										// ֹͣ����ʩ����������

	int ChangePal(DWORD dwPalType);									// �ı��ɫ������
	int UnChangePal(DWORD dwPalType);								// ȡ���ı��ɫ������

	int CreateAppearance(DWORD dwAppearanceID);						// �������ID����������Ч
	int CancelAppearance(DWORD dwAppearanceID);						// �������ID����������Ч

	// �����������
	CMagicMgrBase* FindMagic(int nID);								// Ѱ�ҵ�ǰ���ӵķ���������
	void MagicBeginAttack(int nCheckID);							// �ü��ܿ�ʼ����
	void AddMagic(CMagicMgrBase *pMgr);								// ���뷨������������
	void DelMagic(CMagicMgrBase *pMgr);								// ɾ����������������
	void StopMagic();												// ֹͣ����ħ��
	void AddDmgStack(SBehitParam *pParam);							// �����˺���ջ����

	void FlowHP(SFlowParam FlowParam,
		CCreatureView* pSourceView = NULL,
		long lMagicIDBeHit = 0,
		int nBeHitSoundID = 0);										// ����ƮѪ

	void FlowDamageNum(SFlowParam FlowParam, CCreatureView* pSourceView); // ����ƮѪ
	DWORD GetLastFlowTime();										// ��ȡ�ϴ�ƮѪ��ʱ��
	void SetLastFlowTime(DWORD dwTime);								// �����ϴ�ƮѪ��ʱ��

	virtual int  GetWeaponStyle();					// ��ȡ��������

	virtual void SetStatusMagicCheckID(int nCheckID)			{}	// ����Ŀǰ״̬�¶�Ӧ�ļ���ID
	virtual void SetStatusPlayCommondAttackSound()				{}	// ����Ŀǰ״̬��Ҫ������ͨ��������
	virtual void SetStatusPreAttack()							{}	// ����Ŀǰ״̬�µ���������
	virtual void SetStatusAttackOnce()							{}	// ����Ŀǰ״̬�µ�һ���Թ�������
	virtual void SetStatusAttackCtrl()							{}	// ����Ŀǰ״̬�µĳ�����������

	virtual void OnDrawEx(ICanvas* pCanvas, const POINT& ptTileCenter,
		DWORD dwFlag, SAniContext *pContext, BYTE nAlpha = 0)	{}	// Ϊ�˲�Ӱ����

	// ָ���������
	void AddLinkView(CCreatureView **ppView);						// ����ָ������
	void DelLinkView(CCreatureView **ppView);						// ɾ��ָ������

	// �������
	void ChangeShap(DWORD dwShapID, int nLv);						// ����
	void UnChangeShap(DWORD dwShapID, int nLv);						// ȡ������
	void DoChangeShap();
	BOOL IsChangeShap();											// ��ǰ�Ƿ������

	virtual BOOL ChangeShapEX()						{return FALSE;}

	// ����
	void DrawName(ICanvas* pCanvas, const POINT& ptBottomCenter);			// �������������
	void DrawSelectedName(ICanvas* pCanvas, const POINT& ptBottomCenter);	// ����ѡ���е����������
	void DrawBloodSlot(ICanvas* pCanvas, RECT& rc, int nPercent);			// ��Ѫ��
	void StopAttackSound();													// ֹͣ��������
	BOOL IsDeaded();														// �Ƿ�������
	void AddParticle(DWORD dwPtcID, int nPart);								// �������Ч��
	void UnAddParticle(DWORD dwPtcID, int nPart);							// ȡ������Ч��

public:	
	//ͼ�����
	DWORD			m_dwType;								// ����Type
	DWORD			m_dwCrtID;								// ����ID

	CAniGroup*		m_pModule[CrtPartMax];					// ���ͼ��
	DWORD			m_dwModuleID[CrtPartMax];				// ���ͼ��ID
	DWORD			m_dwDefaultResID[CrtPartMax];			// Ĭ��ͼ��ID
	BYTE			m_bytePal[CrtPartMax];					// ����ĵ�ɫ��������
	BYTE			m_byteAlpha[CrtPartMax];				// �����͸����������
	BYTE*			m_PartStateFlag[CrtPartMax];			// �������������״̬��־

	CPal			m_Pal[CrtPartMax];						// ����ĵ�ɫ��
	int				m_nCurPalList[PalType_MaxType];			// ��ǰҪ��ı�ĵ�ɫ���б�
	int				m_nCurPalType;							// ��ǰ��ɫ������
		
	BYTE			m_byteAlphaAll;							// ��͸���ȣ�����͸��ʱ�õ�

	DWORD			m_dwItemData;							// ����MapItem�ĺ���ʹ�õ�
	DWORD			m_dwResID;								// ��ԴID����������Դ����������ͼ����Դ���������������������ͣ����¼�������ID
	POINT			m_ptAnchor;								// ͼ��ê��
	SAniContext		m_AniContext;							// ����������
	SAniContext		m_AniContextOld;						// �ϴλ�ͼ�Ķ���������

	// �������֡�Ѫ�����
	char			m_szUserName[CREATURE_MAX_NAME];		// ���������
	char			m_szClanName[CREATURE_MAX_NAME + 3];	// ��������
	char			m_szCityName[CREATURE_MAX_NAME + 3];	// ��������

	BOOL			m_bShowClanName;						// �Ƿ���ʾ�������������
	BOOL			m_bShowCityName;						// �Ƿ���ʾ����ĳǳ�����
	BOOL			m_bShowUserName;						// �Ƿ���ʾ���������
	WPixel			m_clrName;								// �����������ֵ���ɫ
	int				m_nBloodVol;							// ��ǰѪ��(���100)
	WPixel			m_wpMinMapFlagColor;					// С��ͼ����ɫ
	IPaoPao*		m_pPaoPao;								// ��ͷ����
	BOOL			m_bVisible;								// �������Ƿ�ɼ�(����)	

	int				m_nHalfHeightOffset;					// ���ê���y����ƫ��(���λ�ÿ���������ʾ����ı���������)
	int				m_nHeightOffset;						// ���ê���y��߶�ƫ��(���λ������ȷ��Ѫ������������������Ϣ��λ��)

	// �ƶ����
	BOOL			m_bNormalMove;							// �Ƿ������ƶ�����ʱ����ʼ�㲻����Tile���ĵ�ľ���Ҫ�����ƶ��������ƶ������ĵ��������ƶ�
	int				m_nMoveSpeedLevel;						// �����ƶ����ٶȵȼ�
	int				m_nAttackSpeedLevel;					// ������·���ٶȵȼ�
	float			m_fStepWalk;							// ��·����
	float			m_fStepRun;								// �ܲ�����
	float			m_fStepRide;							// ������
	SStep			m_StepWalk[CrtDirMaxDirCount];			// ������·8������Ĳ���(ÿ���ƶ���Tile��)
	SStep			m_StepRun[CrtDirMaxDirCount];			// ��ɫ�ܶ�8������Ĳ���(ÿ���ƶ���Tile��)
	SStep			m_StepRide[CrtDirMaxDirCount];			// ��ɫ����8������Ĳ���(ÿ���ƶ���Tile��)
	SStep			m_SpeedMove[CrtDirMaxDirCount];			// �����ƶ�8��������ٶ�(ÿ���ƶ���Tile��)
	SStep			m_fCurSpeed;							// ��ǰ�ٶȣ��ȵ�ǰ�����µ�m_SpeedMove
	POINTS*			m_pPathList;							// ·���б�
	int				m_nPathNodeCount;						// ·���ڵ���
	int				m_nMaxPathNodeCount;					// ���·���ڵ���
	int				m_nCurNodeIndex;						// ��ǰ·���ڵ�����
	BOOL			m_bNeedWait;							// �Ƿ���Ϊǰ�����˶���Ҫ�ȴ�?
	int				m_nWaitTime;							// �ȴ���ʱ��
	int				m_nMoveStyle;							// ��ɫ���ƶ���ʽ(����?�ܲ�?)
	int				m_nLastMoveDir;							// ���һ���ƶ�����
	SDblPoint*		m_pPointList;							// ˫���б�(���ڴӷ����ĵ㿪ʼ�ƶ�ʱ)
	int				m_nMaxPointNodeCount;					// ���˫��ڵ���
	int				m_nPointNodeCount;						// ˫��ڵ���
	int				m_nCurPointNodeIndex;					// ��ǰ˫��ڵ�����
	DWORD			m_dwStartTick;							// ��ʼtick��
	DWORD			m_dwCurTick;							// ��ǰ��tick��
	POINT			m_ptStartPos;							// ��ʼ����λ��


	BOOL			m_bDeaded;								// �Ƿ�������

	ICreatureSink*	m_pCrtSink;								// ����ص��ӿ�

	POINT			m_ptTile;								// �����ڵ�ͼ�ϵ�TILE����
	POINT			m_ptTileOffset;							// ��ɫ��ǰλ����TILE���ĵ�λ�õ�ƫ��

	BOOL			m_bOccupant;							// �������������Ƿ�ռλ

	int				m_nCommonAttackSoundID;					// ��ͨ��������Ч
	int				m_nCommonAttackBeHitSoundID;			// ��ͨ�����Ļ�����Ч
	int				m_nSoundIDFallow;						// ����������Ч
	int				m_nSoundIDBeHit;						// ���ﱻ����Ч
	int				m_nSoundIDDie;							// ����������Ч

	int				m_nAttackSoundChunnelID;				// ����������Ƶ��ID

	int				m_nBeHitLoopBeginFrame;					// ����������ѭ����ʼ֡
	int				m_nBeHitLoopEndFrame;					// ����������ѭ������֡

	SItemShowRect	m_ItemShowRect[CrtStateMax][CrtDirMaxDirCount][30];

	vector<IAttachObj*>			m_vAttachObjs;				// �����������
	vector<IAttachObj*>			m_vFlowObjs;				// ƮѪ��������
	vector<IAttachObj*>			m_vAttackAttachObjs;		// ����ʱ��������������

	CMagicMgrShadow*			m_pMagicShadow;				// ��ӰЧ��

	int				m_nCommonAttackStyleNum;				// Ĭ���˼�����ͨ������ʽ
	int				m_nActStateCommonAttack[3];				// Ĭ�ϵ���ͨ����״̬ID
	int				m_nActStateJuqi;						// Ĭ�ϵ�����״̬ID
	int				m_nActStateSkillAttack;					// Ĭ�ϵļ��ܹ���״̬ID

	// �������	
	BOOL			m_bChangeShap;							// �Ƿ������
	DWORD			m_dwShapID[MAX_CHANGE_SHAP_LV];			// ����������ID
	int				m_nShapLv;								// ��ǰ�����ĸ����ȼ��ı���Ч��

	int				m_nBakCommonAttackSoundID;				// ���ݵ���ͨ��������
	int				m_nBakCommonAttackBeHitSoundID;			// ���ݵ���ͨ������������
	DWORD			m_dwBakModuleID[CrtPartMax];			// �������ͼ��ID
	BYTE			m_byteBakPal[CrtPartMax];				// ��������ĵ�ɫ��������
	BYTE			m_byteBakAlpha[CrtPartMax];				// ���������͸����������

protected:
	vector<CCreatureView**>		m_vppLinkView;				// ����ָ��ö����ָ���б���ֹ�����ͷ������滹������
	vector<CMagicMgrBase*>		m_vMagicMgr;				// ֮ǰʩ�ŵļ��ܹ�����
	IAttachObj*					m_pLockAttachObj;
	DWORD						m_dwLastFlowTime;			// �ϴ�ƮѪ��ʱ��
	CParticle*		m_pParticle[CrtPartMax];				// ����Ч��ID Creature�����в����϶��ɸ�����Ч��
};
//------------------------------------------------------------------------