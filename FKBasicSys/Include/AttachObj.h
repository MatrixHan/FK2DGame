/**
*	created:		2012-7-2   1:59
*	filename: 		AttachObj
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��������(�ܹ���������һ���ƶ���������������������һ��)
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/Timer/ITimerAxis.h"
#include "../../FKGame/Common/CreatureViewDef.h"
#include "Ani.h"
//------------------------------------------------------------------------
#define MAX_DRAW_FRAMES 5
//------------------------------------------------------------------------
// ��������ӿ�
struct IAttachObj
{
	// ��ͼ˳���Ȼ����󻭡��ö���
	enum {draw_order_prev=0, draw_order_after, draw_order_topmost};
	enum {stateCreate=0, stateLoop, stateRelease};
	virtual int  GetDrawOrder(int nLoopNumIndex = 0) = NULL;
	virtual RECT GetShowRect(int nLoopNumIndex = 0)	{RECT rc = {0,0,0,0};	return rc;};
	virtual void SetState(int nNewState) = NULL;
	virtual DWORD GetID() = NULL;
	virtual void GetOldDirtyRect(list<RECT> &DirtyRectList, int nLoopNumIndex = 0)	{}
	virtual void SetDirty(ICanvas* pCanvas, const POINT& ptViewTop, const POINT& ptOffset, int nLoopNumIndex = 0) {}
	virtual void OnDraw(ICanvas* pCanvas,
		const POINT& ptTileCenter, DWORD dwFlag = 0, int nLoopNumIndex = 0) = NULL;
	virtual void Release() = NULL;
	virtual DWORD GetAppearanceID() = NULL;
	virtual int GetState() = NULL;
	virtual int GetLoopNum()	{return 1;}
};
//------------------------------------------------------------------------
// ��������
struct  SAttachObjBuffer
{
	DWORD	dwResID;		// ��ԴID
	POINT	ptAnchor;		// ê��
	int		nDelay;			// ������ʱ
	POINT	ptOffset;		// ����������λ�õ�ƫ��(����Ϊ(0,0))
	BOOL	bAddCrtHeight;	// �Ƿ��������ȫ��
	int		nDrawOrder;		// ��ͼ˳��(0-������֮ǰ���ƣ�1��������֮�����)
	DWORD	dwAppearanceID;	// ���ID

	DWORD	dwSoundResID;	// ��ЧID
	BOOL	bSoundLoop;		// ��Ч�Ƿ�ѭ��
};
//------------------------------------------------------------------------
class CCreatureView;
//------------------------------------------------------------------------
class CAttachObj : public IAttachObj, public ITimerSink
{
public:
	DWORD			m_dwResID;			// ��ԴID
	DWORD			m_dwAppearanceID;	// ���ID
	int				m_nState;			// ��ǰ״̬����������ѭ�������ٵ�
	int				m_nFrame;			// ��ǰ֡
	int				m_nFrameCount;		// ��֡��
	POINT			m_ptOffset;			// ƫ�ƣ����ڽ���ʱ�߶�Ϊ(0,0)
	int				m_nDrawOrder[MAX_DRAW_FRAMES];	// ����˳�����������ԣ�
	int				m_nDrawOrderSave;
	POINT			m_ptAnchor;			// ê��
	CAni*			m_pPoolNodeObj;		// ������
	CCreatureView*	m_pParent;			// ������������
	BOOL			m_bNeedSetOldDirty[MAX_DRAW_FRAMES];// �Ƿ���Ҫ�Ѿɵľ�������
	int				m_nFrameOld[MAX_DRAW_FRAMES];		// ����
	RECT			m_rcAreaOld[MAX_DRAW_FRAMES];		// �ϴλ�ͼ�ľ���λ��
	
	DWORD			m_dwSoundResID;		// ��ЧID
	BOOL			m_bSoundLoop;		// ��Ч�Ƿ�ѭ��
	int				m_nSoundChunnelID;	// ������Ƶ��ID
public:
	CAttachObj(CCreatureView* pParent);
	virtual ~CAttachObj();
	void Close();

	// ITimerSink
	virtual void	OnTimer(DWORD dwEventID)  = NULL;

	virtual void	Release()							{delete this;}
	virtual DWORD	GetAppearanceID()					{return m_dwAppearanceID;}
	virtual void	SetState(int nNewState)				{m_nState = nNewState;}
	virtual int		GetState()							{return m_nState;}
	virtual int		GetDrawOrder(int nLoopNumIndex)		{return m_nDrawOrder[nLoopNumIndex];}
	virtual RECT	GetShowRect(int nLoopNumIndex = 0);
	virtual void	GetOldDirtyRect(list<RECT> &DirtyRectList, int nLoopNumIndex = 0);
	virtual void	SetDirty(ICanvas* pCanvas, const POINT& ptViewTop, const POINT& ptOffset, int nLoopNumIndex = 0);
	virtual void	OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0, int nLoopNumIndex = 0);

	CAni*	RequestRes();
	BOOL	Create(const char* buf, int nLen);
	void	SetDrawOrder(int nOrder)					{m_nDrawOrder[0] = nOrder;}
	DWORD	GetID()										{return m_dwResID;}
};
//------------------------------------------------------------------------
// ��������(3״̬��������)
struct SAttachStateObjBuffer : public SAttachObjBuffer
{
	SAttachStateObjBuffer()
	{
		m_nNoShowBegin = m_nNoShowEnd = -1;
		m_nLoopNum = 1;
	}
	int		nLoopTime;			// ѭ������
	int		nLoopStartIndex;	// ����ѭ������Ӧ��֡����,ע�������Ķ����ǽ���������ѭ������������״̬�Ķ�������һ��
	int		nLoopEndIndex;		// ������������Ӧ��֡����

	int		m_nLoopNum;			// Ҫѭ��������(����ͨ��3����)Ĭ��1����
	int		m_nNoShowBegin, m_nNoShowEnd;		// ������ʾ�Ŀ�ʼ������֡(������ת��������Ϊ���ɼ�)
};
//------------------------------------------------------------------------
// ��3��״̬����������(������ѭ�������٣�3��״̬��ͼƬλ��ͬһ������)
class CAttachStateObj : public CAttachObj  
{
public:
	int		m_nLoopTime;						// ѭ������
	int		m_nLoopStartIndex,m_nLoopEndIndex;	// ѭ���Ŀ�ʼ�ͽ�������Ӧ��֡����
	int		m_nLoopNum;							// Ҫѭ��������(����ͨ��3����)
												// Ĭ��1����

	int		m_nNoShowBegin, m_nNoShowEnd;		// ������ʾ�Ŀ�ʼ������֡(������ת��������Ϊ���ɼ�)
	int		m_nLoopNumIndex[MAX_DRAW_FRAMES];

public:
	CAttachStateObj(CCreatureView* pParent);
	virtual ~CAttachStateObj();
	
	virtual void OnTimer(DWORD dwEventID);
	BOOL Create(const char* buf, int nLen);

	virtual int  GetDrawOrder(int nLoopNumIndex = 0) {
		return m_nDrawOrder[nLoopNumIndex];}
	virtual RECT GetShowRect(int nLoopNumIndex = 0);
	virtual void SetDirty(ICanvas* pCanvas, const POINT& ptViewTop, const POINT& ptOffset, int nLoopNumIndex = 0);
	virtual void OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0, int nLoopNumIndex = 0);

	virtual int GetLoopNum()	{return m_nLoopNum;}

private:
	int m_nLoopNumSave;
};
//------------------------------------------------------------------------
class CFlowText : public IAttachObj, public ITimerSink
{
private:
	POINT			m_ptBeginPos;		// ��ʼλ��
	POINT			m_ptPos;			// ��ǰλ��
	POINT			m_ptOffset;			// ÿ�ε�ƫ����
	enFlowType		m_nFlowType;		// Ʈ������
	int				m_nFlowHeight;		// Ʈ���߶�
	CCreatureView*	m_pParent;			// ������������
	float			m_a;
	int				m_t;
	BYTE			m_nAlpha;
	int				m_nCurFrame;
	enNumType		m_nNumType;
	int				m_nSign;
	int				m_nDelay;
	BOOL			m_bBeginDraw;

	int				m_nNumSize;			// ���ֵ�λ��
	int				m_Num[32];			// ҪƮ������
public:
	// ��������
	struct  SFlowTextBuffer
	{
		POINT		ptPos;				// ��ʼ����λ��
		int			nDelay;				// ������ʱ
		POINT		ptOffset;			// ����ÿ�ε�ƫ����
		int			nNum;				// ��������
		enFlowType	nFlowType;			// Ʈ������
		enNumType	nNumType;			// Ʈ������
	};
	
	CFlowText(CCreatureView* pParent);
	virtual ~CFlowText();
	void Close();

	BOOL Create(const char* buf, int nLen);
	// ITimerSink
	virtual void OnTimer(DWORD dwEventID);
	virtual int  GetDrawOrder(int nLoopNumIndex = 0)	{return IAttachObj::draw_order_topmost;}
	virtual void SetState(int nNewState){};
	virtual DWORD GetID()				{return 0;}
	virtual DWORD GetAppearanceID()		{return -1;}
	virtual int GetState()				{return -1;}
	virtual void OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0, int nLoopNumIndex = 0);
	virtual void Release()				{delete this;}
};
//------------------------------------------------------------------------
class CBehitMagic : public CAttachObj
{
public:
	CBehitMagic();
	CBehitMagic(CCreatureView* pParent);
	
	BOOL Create(const char* buf, int nLen);
	virtual void OnTimer(DWORD dwEventID);
};
//------------------------------------------------------------------------