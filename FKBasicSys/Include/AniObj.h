/**
*	created:		2012-7-2   1:35
*	filename: 		AniObj
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/MapView/MapItemView.h"
#include "../../FKGame/Timer/ITimerAxis.h"
#include "../../FKGame/Common/CreatureViewDef.h"
#include "../../FKGame/Common/MapItemDef.h"
#include "ObjPool.h"
#include "ResLoader.h"
#include "Ani.h"
//------------------------------------------------------------------------
// �򵥵Ķ���������
struct SSimpleAniContext
{
	int nDelay;				// ֡��ʱ�����ٶ�
	int nDelay1;			// ������������֮��ļ��
	BYTE nFrames;			// ֡��
	BYTE nCurFrame;			// ��ǰ�ǵڼ�֡
	
	SSimpleAniContext()			{Clear();}
	void Clear()			{memset(this,0,sizeof(SSimpleAniContext));}
	void NextFrame()		{if(++nCurFrame>=nFrames)nCurFrame=0;}
	int  CurFrame()			{return (int)nCurFrame;}
	int  Frames()			{return (int)nFrames;}
	void SetCurFrame(int nCurFrame)
	{
		this->nCurFrame = nCurFrame;
	}
};
//------------------------------------------------------------------------
// CAniObj�浵��ʽ
struct SAniObjArchive
{
	DWORD	dwResID;
};
//------------------------------------------------------------------------
struct SAniObjCreateParam
{
	int		nDelay;
	int		nDelay1;
	POINT	ptAnchor;
	POINT	ptSortOff1;
	POINT	ptSortOff2;
};
//------------------------------------------------------------------------
// ��̬����ͼƬ����ռλ��
class CAniObj : public CMapItemView, public ITimerSink
{
	typedef int (CAniObj::*ProcCommand) (DWORD dwParam1, DWORD dwParam2);
	typedef void (CAniObj::*ProcProperty) (BOOL bSetProp, DWORD dwPropVal);
	static ProcCommand	m_Command[AniCmdMaxCommandCount];
	static ProcProperty	m_Property[AniPropMaxPropCount];
protected:
	SSimpleAniContext m_AniContext;	// ����������
	DWORD		m_dwResID;			// ��ԴID����������Դ����������ͼ����Դ
	POINT		m_ptAnchor;			// ͼ���ê��
	CAni*		m_pPoolNodeObj;		// ͼƬ�ڴ棬����ͼƬ�Ƴ��ɼ�����ʱ��������������
	POINT		m_ptTile;
	POINT		m_ptSortOff1;		// ����ƫ�Ƶ�1
	POINT		m_ptSortOff2;		// ����ƫ�Ƶ�2

	DWORD		m_dwData;
	BYTE*		m_pCurPal;

public:
	CAniObj();
	virtual ~CAniObj();
	virtual void Close();

	// ITimerSink
	virtual void OnTimer(DWORD dwEventID);

	DWORD GetOffsetID(){return m_dwResID+ioAni;}
	CAni* RequestRes();
	virtual DWORD GetClassID() const{return miAniObj_NoOcc_RGB;}
	virtual void GetOldDirtyRect(list<RECT> &DirtyRectList);
	virtual void SetDirty(ICanvas* pCanvas, const POINT& ptViewTop);
	virtual void OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0);
	virtual void OnDrawTopmostObject(ICanvas* pCanvas,const POINT& ptTileCenter, DWORD dwFlag=0);
	BOOL OnSerialize(BOOL bWrite,IArchive* pArchive);
	BOOL OnCreateItem(const char* buf, int nLen);
	BOOL CreateItem(SAniObjArchive* pArchive);
	virtual void OnUpdate();
	virtual BOOL OnEnterViewport(BOOL bVisable);
	virtual BOOL HitTest(const RECT& rcScreen);
	virtual RECT GetShowRect();
	virtual POINT GetAnchor() const {return m_ptAnchor;}
	virtual IBitmap* GetBitmap();
    virtual DWORD GetItemData() const {return m_dwData;}
	virtual void SetItemData(DWORD dwData) {m_dwData=dwData;}
	virtual BOOL SetSelected(BOOL bSelected = TRUE, BOOL bHighLight = TRUE);
	virtual void SetTile(POINT& ptTile, IMapView* pMapView);
	virtual POINT GetTile() {return m_ptTile;}
	void UpdatePosInfo(IMapView* pMapView);
	virtual DWORD GetResID() {return m_dwResID;}
	virtual void  Release(){Close();delete this;}

	virtual void AddToTimeAxis();
	virtual void RemoveFromTimeAxis();
	// �������
public:
	virtual int OnCommand(DWORD dwCmdID, DWORD dwParam1,DWORD dwParam2);
	int cmdSetProperty(DWORD dwParam1, DWORD dwParam2);
	int cmdGetProperty(DWORD dwParam1, DWORD dwParam2);
	virtual void propName(BOOL bSetProp, DWORD dwPropVal){}
	virtual void propNameColor(BOOL bSetProp, DWORD dwPropVal){}
};
//------------------------------------------------------------------------
// �������Ķ���(��ռλ)
class CAniObjDiscrete : public CAniObj
{
protected:
	int		m_nTimerObjType;
public:
	CAniObjDiscrete() : m_nTimerObjType(0) {}
	virtual void Close();
	virtual DWORD GetClassID() const	{return miAniObj_NoOcc_RGBA;}
	virtual void OnTimer(DWORD dwEventID);
	virtual void SetDirty(ICanvas* pCanvas, const POINT& ptViewTop);
	virtual void OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0);
};
//------------------------------------------------------------------------
// ��ռλ����
class CAniObjMultiOcc : public CAniObj, public COccupant
{
public:
	virtual DWORD GetClassID() const{return miAniObj_MultiOcc_RGB;}
	virtual DWORD GetOccupantValue(){return 0x0;}
	virtual COccupantTileList* GetOccupantTileList()const {return m_pOccupantTileList;}
	virtual BOOL OnSerialize(BOOL bWrite, IArchive* pArchive)
	{
		BOOL bRtn = CAniObj::OnSerialize(bWrite, pArchive);
		if (bWrite)
			return bRtn;
		return SetOccupantInfo();
	}
	virtual BOOL OnCreateItem(const char* buf, int nLen)
	{
		if (!CAniObj::OnCreateItem(buf, nLen))
			return FALSE;
		return SetOccupantInfo();
	}
	virtual BOOL SetOccupantInfo();
};
//------------------------------------------------------------------------
// �������Ķ���(��ռλ)
class CAniObjMultiOccDiscrete : public CAniObjMultiOcc
{
protected:
	int		m_nTimerObjType;
public:
	CAniObjMultiOccDiscrete() : m_nTimerObjType(0) {}
	virtual void Close();
	virtual DWORD GetClassID() const	{return miAniObj_MultiOcc_RGBA;}
	virtual void OnTimer(DWORD dwEventID);
	virtual void SetDirty(ICanvas* pCanvas, const POINT& ptViewTop);
	virtual void OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0);
};
//------------------------------------------------------------------------
// ������
class CAniObjTransport : public CAniObj
{
protected:
	char	m_szName[MAX_ITEM_NAME_LENGTH];// ����
	WPixel	m_clrName;	// ������ʾ����ɫ
	
	WPixel	m_wpMinMapFlagColor;					// С��ͼ����ɫ
public:
	CAniObjTransport()
	{
		m_szName[0]='\0';
		m_clrName=WPixel(255,255,255);		
		m_wpMinMapFlagColor = WPixel(255,255,255);
	}
	virtual void	SetFlagColorOnMinmap(DWORD dwColor);						// ������С��ͼ�ϵ���ɫ
	virtual DWORD	GetFlagColorOnMinmap();										// ȡ����С��ͼ�ϵ���ɫ

	virtual BOOL GetVisibleOnMinmap();											// ȡ���Ƿ���С��ͼ����ʾ
	virtual DWORD GetClassID() const{return miAniObj_Transprot;}
	virtual void propName(BOOL bSetProp, DWORD dwPropVal);
	virtual void propNameColor(BOOL bSetProp, DWORD dwPropVal);
	virtual void OnDrawTopmostObject(ICanvas* pCanvas,const POINT& ptTileCenter, DWORD dwFlag=0);
};
//------------------------------------------------------------------------