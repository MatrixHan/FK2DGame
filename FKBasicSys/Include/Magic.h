/**
*	created:		2012-7-2   1:24
*	filename: 		Magic
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ħ������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/MapView/MapItemView.h"
#include "../Include/Ani.h"
#include "../../FKGame/Timer/ITimerAxis.h"
#include "../Include/ResLoader.h"
#include "../Include/AniObj.h"
#include "../Include/PalMgr.h"
#include "../../FKGame/Common/CreatureViewDef.h"
#include "../Include/Color.h"
#include "../Include/Particle.h"
//------------------------------------------------------------------------
#define max_pal_color								8
//------------------------------------------------------------------------
// ����ռλ����
#define CHECK_DISTANCE								3
#define DEFAULT_FLY_SPEED							15
#define PI											3.1416f
#define MAX_LINE_PATH_LENGTH						100
//------------------------------------------------------------------------
// ħ����������ض���
#define TIME_EVENT_ID_MAGIC_MGR_ATTACK_DELAY		1
#define TIME_EVENT_ID_MAGIC_MGR_ATTACK_SPACE		2
#define TIME_EVENT_ID_MAGIC_MGR_ATTACK_LOOP			3
#define TIME_EVENT_ID_MAGIC_MGR_DESTROY				10
//------------------------------------------------------------------------
#define MAGIC_MGR_DEFAULT_ATTACK_DELAY_TIME			2000
#define MAGIC_MGR_DESTROY_TIME						5000
//------------------------------------------------------------------------
int		line(int x1, int y1, int x2, int y2, POINT* pptArray, int& nLen, int nMaxLen=0,
			 bool bTestTile=true, int nBlockTestMode = 0);
int		_test_tile_spread(POINT& ptTile);
int		_test_tile_block(POINT& ptTile, int nBlockTestMode);
//------------------------------------------------------------------------
class CCreatureView;
//------------------------------------------------------------------------
// Magic�浵��ʽ
struct SDamageData
{
	SFlowParam		FlowParam;
	CCreatureView	*pView;
};
//------------------------------------------------------------------------
// Magic�浵��ʽ
struct SMagicArchive
{
	DWORD	dwResID;
	POINT	ptAnchor;
	POINT	ptOffset;
	int		nDelay;
	int		nLayerValue;
	DWORD	dwSoundResID;
	BOOL	bSoundLoop;
};
//------------------------------------------------------------------------
// ����ħ���浵��ʽ
struct SMagicArchive_SingleDir : public SMagicArchive
{
	POINT	ptStartTile;	// ħ����ʼ��(TILE����)
	POINT	ptAttackTile;	// ħ��������(TILE����)
	int		nDir;			// ������
	int		nFramesPerDir;	// ÿ�������ϵ�֡��
	POINT	ptOffset;		// ħ����������ƫ��
	POINT	ptTileOffset;	// ħ������Tile���ĵ��ƫ��
	float	fDir;			// ��ʼʱ�ķ���
	float	fDirInc;		// ����任��
	int		nTimeTile;		// �ɹ�һ��Tile��ʱ��
	int		nLife;			// ��������
	CCreatureView	*pAttackView;
};
//------------------------------------------------------------------------
// �������ڵ�ħ���浵��ʽ
struct SMagicArchive_Life : public SMagicArchive
{
	int	nLifeLine;			// ħ��������
};
//------------------------------------------------------------------------
// ����ħ���浵��ʽ
struct SMagicArchive_Ring : public SMagicArchive_Life
{
	int	nFramesPerDir;		// ħ��ÿ�������ϵ�֡��
};
//------------------------------------------------------------------------
// ���Ͻ�ɫ�Ľ�ӡ�浵��ʽ
struct SMagicArchive_FootPrint : public SMagicArchive_Life
{
	int nDirection;
};
//------------------------------------------------------------------------
// ֻ���Ű˸�������ĳ�������ħ���浵��ʽ
struct SMagicArchive_EightDir : public SMagicArchive_Life
{
	int		nFramesPerDir;	// ÿ���������ж���֡
	POINT	ptTile;
	int		nDir;
};
//------------------------------------------------------------------------
// ����3��״̬��ħ���浵��ʽ
struct SMagicArchive_ThreeState : public  SMagicArchive_Life
{
	int   nLoopStartIndex;	// ����ѭ������Ӧ��֡����,ע�������Ķ����ǽ���������ѭ������������״̬�Ķ�������һ��
	int   nLoopEndIndex;	// ������������Ӧ��֡����
};
//------------------------------------------------------------------------
// ��������һ��б����׹��ħ���浵��ʽ
struct SMagicArchive_Rain : public SMagicArchive_ThreeState
{
	BOOL  bNeedRandomOffset;// �Ƿ���Ҫ�������ƫ��
	POINT ptStepOffset;		// ÿ��ƫ����
};
//------------------------------------------------------------------------
// ͨ��ħ���浵��ʽ
struct SMagicArchive_General : public  SMagicArchive
{
	int   nLoopStartIndex;	// ����ѭ������Ӧ��֡����,ע�������Ķ����ǽ���������ѭ������������״̬�Ķ�������һ��
	int   nLoopEndIndex;	// ������������Ӧ��֡����
};
//------------------------------------------------------------------------
// ��Χ���ܵ�����Ч�浵��ʽ
struct SMagicArchive_Mouse : public  SMagicArchive
{
	int   nLoopStartIndex;	// ����ѭ������Ӧ��֡����,ע�������Ķ����ǽ���������ѭ������������״̬�Ķ�������һ��
	int   nLoopEndIndex;	// ������������Ӧ��֡����
};
//------------------------------------------------------------------------
// �����ļ�
struct SMagicCreateParam 
{
	POINTS	ptAnchor;
	WORD	nDelay;
	BYTE	nLife;
	POINTS	ptOffset;
	BYTE	nLoopStartIndex;
	BYTE	nLoopEndIndex;
	int		nLayerValue;

};
//------------------------------------------------------------------------
// ħ������������
class CMagicMgrBase
{
public:
	CMagicMgrBase();
	virtual ~CMagicMgrBase();

	virtual BOOL	Create(void* buf, int nSize)					{return FALSE;}

	// ��һ��ħ����Ԫ��ը֮���ص��������
	virtual void	OnPlayExplode(void* pThis)						{}
	// ��һ��ħ����Ԫ������֮���ص��������
	virtual void	OnPlayDone(void* pThis, POINT Pt)				{}

	// ����ħ����������ʼ���������ͷ�ħ��
	virtual void	BeginAttack()									{}
	// ����Ϊ�Ѿ��ͷŹ���
	virtual void	SetBeginAttack()								{m_bBeginAttack = TRUE;}
	// �Ƿ��Ѿ��ͷŹ�����
	virtual BOOL	IsBeginAttack()									{return m_bBeginAttack;}
	// ֹͣʩ��
	virtual void	Stop()											{}
	// ħ���Ƿ�������
	virtual BOOL	IsRunning()										{return FALSE;}

	// ��ֵ�ϼ�ID
	virtual void	SetCheckID(int nID)								{m_nCheckID = nID;}
	// ��ȡ�ϼ�ID
	virtual int		GetCheckID()									{return m_nCheckID;}

	// ��ֵ�����ӳ٣���ħ�������ӳ�
	virtual void	SetAttackDelay(int nDelay)						{m_nAttackDelay = nDelay;}
	// ��ȡ�����ӳ�
	virtual int		GetAttackDelay()								{return m_nAttackDelay;}

	// ���ñ���ħ��ID
	virtual void	SetMagicIDBeHit(int nMagicIDBeHit)				{m_nMagicIDBeHit = nMagicIDBeHit;}

	// ���ñ���ħ��ID
	virtual void	SetSoundIDBeHit(int nSoundIDBeHit)				{m_nSoundIDBeHit = nSoundIDBeHit;}

	// ��ӽ�һ���˺���
	virtual void	AddDamage(CCreatureView *pView, SFlowParam FlowParam);
	// Ʈ���ٸ��˺�
	virtual void	FlowDamage(int nNum);
	// Ʈ�˺�
	virtual void	FlowAllDamage();
	// ���ص�ǰƮ�˶��ٸ��˺�
	virtual int		GetFlowDamageNum();
	// ��ʾħ�������˺��ˣ��Ժ���ӵ��˺�������Ʈ�˺�
	virtual void	BeginDamage();
	// ��ȡ��ǰ����˺�����
	virtual SDamageData* GetFrontDamageData();
	// ɾ����ǰ���һ���˺�
	virtual void	DelFrontDamage();

	// ����ħ��������������
	virtual void 	SetMasterView(CCreatureView	*pView);
	// ����ħ��Ҫ�����Ķ���
	virtual void 	SetAttackView(CCreatureView	*pView);

protected:
	CCreatureView	*m_pMasterView;			// ʹ�ø�ħ��������ָ��
	CCreatureView	*m_pAttackView;			// �����ߵ�����ָ��
private:
	BOOL	m_bBeginAttack;					// ħ�������˺��Ƿ�ʼ�ͷ���
	BOOL	m_bBeginDamage;					// ħ���Ƿ�����˺���
	int		m_nMagicIDBeHit;				// ����ʱ�Ĺ�Ч
	int		m_nSoundIDBeHit;				// ����ʱ����Ч
	int		m_nAttackDelay;					// ������ʱ
	int		m_nCheckID;						// ħ���Ľϼ�ID��Ψһȷ��
	int		m_FlowDamNum;					// ��ǰƮ�˶��ٴ��˺���
	list<SDamageData *> m_vDamageData;		// �˺�ջ
};
//------------------------------------------------------------------------
// ħ������
class CMagic  : public CMapItemView, public ITimerSink
{
public:
	// ͼƬ��Դ���
	DWORD			m_dwResID;			// ��ԴID����������Դ����������ͼ����Դ
	CAni*			m_pPoolNodeObj;		// ͼƬ�ڴ棬����ͼƬ�Ƴ��ɼ�����ʱ��������������

	// �������
	SSimpleAniContext	m_AniContext;	// ����������

	// λ�����
	POINT			m_ptOffset;			// ����ڵ������ĵ��ƫ��
	POINT			m_ptAnchor;			// ͼ��ê��
	POINT			m_ptTile;			// �ڵ�ͼ�ϵ�Tile����
	int				m_nLayerValue;		// ��ͼ�ϵĲ��

	// ��ħ��
	CMagicMgrBase*	m_pMgr;				// ħ������������Ҫ���ڻص�

	DWORD			m_dwSoundResID;		// ��ЧID
	BOOL			m_bSoundLoop;		// ��Ч�Ƿ�ѭ��
	int				m_nSoundChunnelID;	// ������Ƶ��ID
public:
	CMagic();
	CMagic(CMagicMgrBase* pMgr);
	virtual ~CMagic();
	virtual void	Close();

	virtual void	OnTimer(DWORD dwEventID);
	
	// CMapItem ,��ʱ��������TILE����
	virtual void	SetItemData(DWORD dwData);
	// CMapItemView
	virtual void	SetTile(POINT& ptTile, IMapView* pMapView);

	virtual DWORD	GetLayerValue();
	virtual DWORD	GetOccupantValue();
	virtual RECT	GetShowRect();
	virtual POINT	GetTile();
	virtual DWORD	GetResID();
	virtual POINT	GetAnchor() const { return m_ptAnchor; }
	virtual DWORD	GetClassID() const{ return miMagicObj; }
	virtual IBitmap	*GetBitmap();

	virtual BOOL	OnEnterViewport(BOOL bVisable);
	virtual void	GetOldDirtyRect(list<RECT> &DirtyRectList);
	virtual void	SetDirty(ICanvas* pCanvas, const POINT& ptViewTop);
	virtual void	OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0);
	virtual void	OnDrawTopmostObject(ICanvas* pCanvas,const POINT& ptTileCenter, DWORD dwFlag);
	virtual BOOL	OnSerialize(BOOL bWrite,IArchive* pArchive);
	virtual BOOL	OnCreateItem(const char* buf, int nLen);

	virtual	BOOL	HitTest(const RECT& rcScreen);
	virtual BOOL	AddToTimerAxis();
	virtual BOOL	AddToMapView(POINT& ptTile);
	virtual BOOL	RemoveFromTimerAxis();
	virtual BOOL	RemoveFromMapView();

	void	UpdatePosInfo(IMapView* pMapView);
	CAni	*RequestRes();
	BOOL	CreateItem(const char* buf, int nLen);
	
	// �ص�
	void	SetMgr(CMagicMgrBase* pMgr);
};
//------------------------------------------------------------------------
// ���������ħ����������
class CMagicSingleDir : public CMagic
{
public:
	int		m_nLen;				// ���鳤��
	int		m_nFramesPerDir;	// ÿ�������֡��(<8)
	int		m_nStartFrameIdx;	// ��ʼ֡����
	POINT	m_ptTileOffset;		// ħ��ƫ��(��Ե�ǰtile���ĵ��)
	float	m_fOldDir;			// ��ʼ����
	float	m_fDirInc;			// ÿ�η���ı����
	int		m_nDirBI;			// �����
	POINT	m_ptTileAttack;		// ������λ��
	CCreatureView	*m_pAttackView;	// ���������ָ��
	int		m_nTimeTile;		// �ɹ�һ��TILE��ʱ��
	int		m_nLife;
	
	CMagicSingleDir();
	virtual ~CMagicSingleDir();

	virtual void Close();
	virtual BOOL AddToMapView(POINT& ptTile);
	virtual BOOL AddToTimerAxis();
	virtual void OnTimer(DWORD dwEventID);
	virtual void OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0);
	virtual RECT GetShowRect();
	virtual BOOL OnCreateItem(const char* buf, int nLen);
};
//------------------------------------------------------------------------
// �������ڵ�ħ��
class CMagicLife : public CMagic
{
public:
	// ���������
	int				m_nCurLife;			// ��ǰ����
	int				m_nLifeLine;		// ������

	CMagicLife() : m_nCurLife(0),m_nLifeLine(-1) {}
	virtual ~CMagicLife() {}

	virtual void OnTimer(DWORD dwEventID);
	virtual BOOL OnCreateItem(const char* buf, int nLen);
};
//------------------------------------------------------------------------
// ����ħ��
class CMagicRing : public CMagicLife
{
public:
	int		m_nFramesPerDir;	// ħ��ÿ�������ϵ�֡��
	int		m_nNoBlockDirs;		// û���ڵ��ķ�����
	int		m_aDirs[32];		// û���ڵ��ķ����б�(���֧��32������)
	int		m_nDirs;
	
	CMagicRing() : m_nFramesPerDir(0),m_nNoBlockDirs(0) {}
	virtual ~CMagicRing() {}

	virtual void	OnTimer(DWORD dwEventID);
	virtual DWORD	GetLayerValue() {return milvLowest;}
	virtual BOOL	OnCreateItem(const char* buf, int nLen);
	virtual void	OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0);

	void SetDirsInfo(int* nDirList, int nDirCount);
};
//------------------------------------------------------------------------
// ֻ���Ű˸�������ĳ�������ħ��
class CMagicEightDir : public CMagicLife
{
public:
	int	m_nFramesPerDir;	// ħ��ÿ�������ϵ�֡��
	int m_nStartFrameIndex;
	int m_nEndFrameIndex;

	CMagicEightDir() : m_nFramesPerDir(0),m_nStartFrameIndex(0),m_nEndFrameIndex(0)	{}
	virtual ~CMagicEightDir() {}

	virtual void	OnTimer(DWORD dwEventID);
	virtual DWORD	GetLayerValue();
	virtual BOOL	OnCreateItem(const char* buf, int nLen);
};
//------------------------------------------------------------------------
// ������Ʒ����˸������
class CMagicStar : public CMagicLife
{
	int		m_nCount;	// ���ڶ���һ���ֻ�֮��ļ��
public:
	enum
	{
		MAX_COUNT = 0
	};

	CMagicStar() { m_nCount = MAX_COUNT - 10 + rand() % 10; }

	virtual void OnTimer(DWORD dwEventID);
	virtual void SetDirty(ICanvas* pCanvas, const POINT& ptViewTop);
	virtual void OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0);
};
//------------------------------------------------------------------------
// ���Ͻ�ɫ�Ľ�ӡ
class CMagicFootprint : public CMagicLife
{
	int		m_nDirection; // ��ӡ�ķ���

public:
	CMagicFootprint() : m_nDirection(0) {}

	virtual BOOL OnCreateItem(const char* buf, int nLen);
	virtual void OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0);
	virtual RECT GetShowRect();
	virtual DWORD GetLayerValue();

	void	SetDirection(int nDir);
};
//------------------------------------------------------------------------
// ���ϲ�����
class CMagicLife_LeftObj :public CMagicLife
{
public:
	virtual void OnTimer(DWORD dwEventID);
};
//------------------------------------------------------------------------
// ����3��״̬��ħ��
class CMagicThreeState : public CMagicLife
{
public:
	int		m_nLoopStartIndex,m_nLoopEndIndex;	// ѭ���Ŀ�ʼ�ͽ�������Ӧ��֡����
	int		m_nState;

	CMagicThreeState() : m_nLoopStartIndex(-1),m_nLoopEndIndex(-1),m_nState(0) {}
	virtual ~CMagicThreeState() {}
	virtual void OnTimer(DWORD dwEventID);
	virtual BOOL OnCreateItem(const char* buf, int nLen);
};
//------------------------------------------------------------------------
// ��������һ��б����׹��ħ��
class CMagicRain : public CMagicThreeState
{
	POINT	m_ptStepOffset; // ÿ��ƫ����
	int		m_nOffX;
	int		m_nOffY;
	BOOL	m_bMove;
	int		m_nMoveTime;
	POINT	m_ptOldOffset;
public:
	CMagicRain() { (__int64&)m_ptStepOffset = 0; (__int64&)m_ptOldOffset = 0; m_bMove = FALSE; m_nMoveTime = 0;}
	virtual ~CMagicRain(){
		g_pTimeAxis->KillTimer(1, static_cast<ITimerSink*>(this));}

	virtual void OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0);
	virtual RECT GetShowRect();
	virtual void OnTimer(DWORD dwEventID);
	virtual BOOL OnCreateItem(const char* buf, int nLen);
	virtual BOOL AddToTimerAxis();
};
//------------------------------------------------------------------------
// ͷ��Ʈ����
class CMagicFlowText :public CMagic
{
public:
	virtual BOOL AddToTimerAxis();
	virtual BOOL AddToMapView(POINT& ptTile);
	virtual BOOL RemoveFromTimerAxis();
	virtual BOOL RemoveFromMapView();
	virtual void OnTimer(DWORD dwEventID);
	virtual BOOL OnCreateItem(const char* buf, int nLen);
	void DrawText(ICanvas* pCanvas, int x, int y, int nFrame, DWORD dwFlag, BYTE nAlpha = 0);
	
	int GetBitmapWidthHalf(int nFrame);
};
//------------------------------------------------------------------------
// �µ�ͨ��ħ��
class CMagicGeneral : public CMagic
{
public:
	CMagicGeneral();
	virtual ~CMagicGeneral();

	virtual void OnTimer(DWORD dwEventID);
	virtual BOOL OnCreateItem(const char* buf, int nLen);
	void	SetLoop(BOOL bLoop);							// �����Ƿ�Ҫѭ��

	int		m_nLoopStartIndex;	// ѭ���Ŀ�ʼ֡����
	int		m_nLoopEndIndex;	// ѭ���Ľ���֡����
	BOOL	m_bLoopPart;		// �Ƿ�ѭ���ֲ�
	BOOL	m_bLoop;			// �Ƿ�ѭ��
};
//------------------------------------------------------------------------
// ��Χ���ܵ�����Ч
class CMagicMouse : public CMagic
{
public:
	CMagicMouse();
	virtual ~CMagicMouse();

	virtual void OnTimer(DWORD dwEventID);
	virtual DWORD GetLayerValue() {return milvLowest-1;}
	virtual RECT GetShowRect();
	virtual BOOL OnCreateItem(const char* buf, int nLen);
	virtual void OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0);

	int		m_nLoopStartIndex;	// ѭ���Ŀ�ʼ֡����
	int		m_nLoopEndIndex;	// ѭ���Ľ���֡����
	BOOL	m_bLoopPart;		// �Ƿ�ѭ���ֲ�
	
	CParticle*	m_pParticle;	// ����Ч��
};
//------------------------------------------------------------------------