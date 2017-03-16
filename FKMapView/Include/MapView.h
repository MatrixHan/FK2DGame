/**
*	created:		2012-7-1   5:53
*	filename: 		MapView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKCanvas/IFKCanvas.h"
#include "../../FKGame/MapView/IMapView.h"
#include "../../FKGame/MapView/MapItemView.h"
#include "ScrollSheet.h"
#include "MapCoordinate.h"
#include "GroundView.h"
#include "MapContainer.h"
#include "MapArchiveMgr.h"
#include "MiniMap.h"
#include "TilesView.h"
#include "Weather.h"
#include <MMSystem.h>
#include <list>
using std::list;
//------------------------------------------------------------------------
#ifdef _DEBUG
#define __USE_MALLOC
#endif
//------------------------------------------------------------------------
struct IItemClassFactory;
//------------------------------------------------------------------------
#define MAX_SHORT_NAME	32
#define CMP_LT	-1
#define CMP_EQ	0
#define CMP_GT	1
//------------------------------------------------------------------------
#ifdef _USE
#define SAVE_CANVAS_W	1280
#define SAVE_CANVAS_H	1024
#else
#define SAVE_CANVAS_W	SCR_W  * 1.5
#define SAVE_CANVAS_H	SCR_H  * 1.5
#endif
//------------------------------------------------------------------------
struct CMapItemViewPtr
{
	CMapItemView* pItem;
	int				nCount;
	CMapItemViewPtr() : pItem(NULL), nCount(0) {}
	BOOL operator == (const CMapItemViewPtr& item)
	{
		return pItem == item.pItem;
	}
	BOOL operator < (const CMapItemViewPtr& item)
	{
		return *pItem < *item.pItem;
	}
	CMapItemView* operator ->()
	{
		return pItem;
	}
	BOOL IsPoint()
	{
		return (pItem->m_ptLeft.x == pItem->m_ptRight.x && pItem->m_ptLeft.y == pItem->m_ptRight.y);
	}
	int Compare(CMapItemViewPtr& item)
	{
		return 0;
	}
	BOOL IsValid()
	{
		return (pItem != NULL);
	}
};
//------------------------------------------------------------------------
class CMapView : public IMapView
{
private:
	// ����ռλ���
	DWORD				m_dwDrawFlag;			// һЩ����������ռλ��ȵĻ��Ʊ�־
	IBitmap*			m_pOccupantBitmap;		// ռλ�����
	// �������
	CMapContainer		m_Map;					// ��ͼ����
	// ����ϵͳ���
	CMapCoordinate		m_MapCo;				// ��ͼ������ϵͳ
	// ��ͼ���
	CGroundView			m_GroundView;			// �ر���ͼ
	CTilesView			m_TilesView;			// �����ͼ�Ķ�̬����
	// ��ͼ�б����
	typedef list<CMapItemViewPtr> DisplayList;
	typedef list<CMapItemViewPtr>::iterator DisplayListPtr;
	DisplayList			m_DisplayList;			// ��ǰ��ͼ��ռλ�������ʾ�б�
	DisplayListPtr		m_iteratorCurItem;	// ȫ����ʾɨ��ʱ�������Ӷ�ռλ������б��еĶԱȽϣ���ס��ǰ��λ�� 
	BOOL				m_bReSortDrawItemList;	// ��ʾʱ�Ƿ���Ҫ�������е����ж�������

	// �������
	CMapItemView*		m_pPlayerItem;			// ���Ƕ���
	POINT				m_ptPlayerTile;			// �������е�λ��
	// ��ͼ�ĵ����
	CMapArchiveMgr		m_MapArchiveMgr;		// ��ͼ�ĵ�������
	char				m_szGrondFileDirName[MAX_SHORT_NAME]; // ��ס������load��ͼʱ�����groundһ���ļ�Ŀ¼������"grass"
	char				m_szMapName[MAX_SHORT_NAME];
	CWeather			m_Weather;				// ����

	// С��ͼ���
	CMiniMap			m_MiniMap;
	BOOL				m_bMouseInMiniMap;
	POINT				m_ptMousePos;
	BOOL				m_bOptimizedScrollScreen;	// �Ż��Ļ�������

	DisplayList			m_VisableListGround;
	DisplayList			m_VisableListObj;
	DisplayList			m_VisableListMultiOcc;

	IBitmap*			m_pSaveBmp;
	ICanvas*			m_pSaveCanvas;

	IBitmap*			m_pMengBmp;
	IBitmap*			m_pWngBmp;

	IBitmap*			m_pMengBmpBig;
	IBitmap*			m_pWngBmpBig;

	IBitmap*			m_pMiniBmp;
	ICanvas*			m_pMiniCanvas;

	IBitmap*			m_pMiniBmpBig;
	ICanvas*			m_pMiniCanvasBig;

	list<RECT>			m_DirtyRectList;

	POINT				m_ptOldSave;
	POINT				m_ptSave;

public:
	// ---------------------------------------------------------------------------------
	// С��ͼ��Ҫ֪����ͼ�ϵ��߼������б�
	// ��������,NPC,���,������LOAD��ͼʱ���ص���ľ������
	typedef list<CMapItemView*> LogicList;
	typedef list<CMapItemView*>::iterator LogicListPtr;
	LogicList			m_LogicItemList;

public:
	BOOL IsItemInViewArea(const POINT &ptTile, CMapItemView *pItemView);
	BOOL IsItemInLogicArea(const POINT &ptTile, CMapItemView *pItemView);
	BOOL IsItemVisable(const POINT& ptTile,CMapItemView *pItemView);
	// �رյ�ǰ��ͼ������һ���յ�ͼ
	BOOL CreateNewMap(int nMapWidth,int nMapHeight,
		LPRECT lprcViewport, LPRECT lprcMiniMapViewport,
		LPCSTR szGroundJpgFileDir, IFileObject* pGridIdxArrayFile, ITrace* pTrace);
	BOOL LoadMapView(IFileObject* pMapFile, IFileObject* pGridIdxArrayFile, IFileObject* pTinyMapFile,
		LPCSTR szGroundJpgFileDir, LPCSTR szMapName, LPRECT lprcViewport, 
		LPRECT lprcMiniMapViewport, LPCSTR szMiniMapJpgFileDir, BOOL bUseDynamicMap, int nMiniMapRate, 
		IItemClassFactory* pItemCF, ITrace* pTrace);
	// ��CreateNewMap()����һ���µ�ͼ֮�󣬻���OnSerialize(sRead)֮�󣬵��ô˺��������ɼ��ر���ͼ��
	// szGroundJpgFileDirΪ�ر�jpg�ļ����ڵ�Ŀ¼
	// szGridIndexArrayΪ�ر��ƴ�������ļ�
	BOOL CreateGroundView(LPRECT lprcViewport, IItemClassFactory* pItemCF, 
		LPCSTR szGroundJpgFileDir, IFileObject* pGridIdxArrayFile=NULL);

	virtual BOOL FindPath(POINTS ptFrom , POINTS ptTo , POINTS** ppBuffer , int& nPathLen);
	virtual BOOL FindSimplePath(POINTS ptFrom , POINTS ptTo , POINTS** ppBuffer , int& nPathLen);

	virtual int GetMapWidth()const		{return m_Map.GetMapWidth();}
	virtual int GetMapHeight()const		{return m_Map.GetMapHeight();}

	// ���Ӵ��ھ��󣬷��ؾ�����������
	virtual const RECT& GetViewportRect()const{return m_GroundView.GetViewportRect();};
	// �ӿڴ�С�ı�
	virtual void OnSizeViewport(int nViewWidth,int nViewHeight);

	// ������Ļ
	virtual BOOL ScrollViewport(int dx, int dy);

	// ���Ƶ�ͼ,rcΪ��ͼ��Ļ�����ݲ���
	virtual void OnDraw(ICanvas* pCanvas, const void* pParam);
	// ����������
	virtual void DrawRectangleGrid(ICanvas* pCanvas, int nGridWidth, int nGridHeight, WPixel clrLine, WPixel clrText);

	// �����Ƿ��ڵر��ϻ�����
	virtual void SetDrawFlag(DWORD dwFlag, DWORD dwClearFlag, void* pParam = 0)
	{
		m_dwDrawFlag |= dwFlag;
		if(dwClearFlag)
			m_dwDrawFlag &= ~dwClearFlag;

		if(dwFlag & maskDrawOccupant)
			m_pOccupantBitmap = (IBitmap*)pParam;
	}

	virtual DWORD GetDrawFlag(DWORD dwFlag)
	{
		return m_dwDrawFlag & dwFlag;
	}

	virtual BOOL AddItem(const POINT& ptTile, CMapItemView* pItem, DWORD dwParam=0);
	virtual BOOL MoveItem(const POINT& ptTileFrom, const POINT& ptTileTo, CMapItemView* pItem, DWORD dwParam=0);
	virtual BOOL RemoveItem(const POINT& ptTile, CMapItemView* pItem, DWORD dwParam=0);

	// �������ռλ��Ϣ
	virtual BOOL AddItemOccupant(const POINT& ptTile, CMapItemView* pItem);

	// ɾ������ռλ��Ϣ
	virtual BOOL RemoveItemOccupant(const POINT& ptTile, CMapItemView* pItem);

	virtual BOOL SetPlayerItem(CMapItemView* pItem)
	{
		m_pPlayerItem = pItem;
		return TRUE;
	}
	virtual CMapItemView* GetPlayerItem()
	{
		return m_pPlayerItem;
	}

	virtual BOOL SnapshotTileByWorldRect(const RECT& rcWorld, IN OUT int& nListCount, OUT SNAP_TILE_INFO* pListBuf)
	{
		return m_Map.SnapshotTileByWorldRect(rcWorld,nListCount, (SNAP_TILE*)pListBuf);
	}

	virtual BOOL SnapshotItemByWorldRect(const RECT& rcWorld, IN OUT int& nListCount, OUT SNAP_ITEM_INFO* pListBuf)
	{
		return m_Map.SnapshotItemByWorldRect(rcWorld,nListCount, (SNAP_ITEM*)pListBuf);
	}

	virtual CMapItemView* HitTest(const RECT& rcScreen, POINT* pPointFound = NULL, DWORD dwHitFlag = 0);

	// ��д��ͼ�浵
	// bServerMap:
	// 		��bServerMap��־�����Ǳ��������ר�õ�ͼ���ǿͻ��˵�ͼ��
	//		���ߵ�����ʱ���������˵�ͼ����Ҫ�����ֻ��ҪCTile���ݼ���
	//		ֻ��ͼ�浵��Ч
	virtual BOOL OnSerialize(BOOL bWrite, IArchive* pArchive, IItemClassFactory *pItemCF, LPRECT lprcViewport, ITrace* pTrace);

	// �������λ�����ڵĿ�����
	virtual void ScreenToTile(const POINT& ptScreen, POINT &ptTile) const
	{
		m_GroundView.PixelToTile(ptScreen,ptTile);
	}
	// ���ؿ�����Ͻǵ�����
	virtual void TileToScreen(const POINT &ptTile, POINT &ptTileCenter) const
	{
		m_GroundView.TileToPixel(ptTile,ptTileCenter);
	}

	// ���ؿ����������
	virtual void TileToWorld(const POINT &ptTile, POINT &ptTileCenter) const
	{
		m_MapCo.TileToPixel(ptTile,ptTileCenter);
	}
	// �������������Ӧ�Ŀ�����
	virtual void WorldToTile(const POINT& ptWorld, POINT &ptTile) const
	{
		m_MapCo.PixelToTile(ptWorld,ptTile);
	}

	// ���ݿ����귵�صر��CTile
	virtual CTile* GetTile(const POINT& ptTile) const
	{
		return &m_Map.GetTile(ptTile);
	}

	// ������Ļ��������ȡ�ÿ�ָ��
	// ���Ƿ�����Ч��ַ��CTile������CTile.IsValid()�жϴ�tile�Ƿ���Ч
	virtual CTile* GetTileFromScreen(POINT ptScreen) const
	{
		POINT ptTile;
		m_GroundView.PixelToTile(ptScreen,ptTile);
		return &m_Map.GetTile(ptTile);
	}

	// ���tile�����Ƿ�Ϊ��Ч����
	virtual BOOL IsValidTile(const POINT& ptTile)
	{
		return m_Map.GetTile(ptTile).IsValid();
	}
	POINT GetPlayerTile()		{return m_ptPlayerTile;}

	virtual void Release()
	{
		Close();
#ifdef _DEBUG
		OutputDebugString("IMapView::Release()");
#endif
		delete this;
	}

	virtual int GetMapTileWidth()
	{
		return m_MapCo.GetMatrixWidth();
	}

	virtual int GetMapTileHeight()
	{
		return m_MapCo.GetMatrixHeight();
	}

	virtual BOOL ScrollToCenter(POINT ptScreenCenter);

	virtual int GetViewTopLeftX() const
	{
		return m_GroundView.GetViewTopX();
	}
	virtual int GetViewTopLeftY() const
	{
		return m_GroundView.GetViewTopY();
	}

	CMapContainer& GetMapContainer()
	{
		return m_Map;
	}

	CMapCoordinate& GetMapCoordinate()
	{
		return m_MapCo;
	}

	CMapView();
	~CMapView();
	void Close();

	virtual int OnDrawOccupant(const POINT& ptTile,CTile* pTile,void* pParam);
	virtual void OnMapItemEnterViewport(const POINT& ptTile,CMapItem* pItem,void* pParam);
	virtual void OnMapItemLeaveViewport(const POINT& ptTile,CMapItem* pItem,void* pParam);

	// С��ͼ���
	virtual void SetMiniMapVisible(BOOL bVisible);
	virtual void SetMiniMapViewport(LPRECT lprcViewport);
	virtual void SetMiniMapTransparent(int nAlpha);
	virtual BOOL GetTinyMapVisible();
	virtual void SetTinyMapVisible(BOOL bVisible);

	// ����С��ͼ��С
	virtual void SetMiniMapBigOrSmall(BOOL bBig);

	// �ı��ͼ����
	virtual void ChangeWeather(SWeatherInfo info);

	virtual BOOL QuickFillOccupant(POINT& ptTileInRect, int nGridWidth, int nGridHeight, int nTileRows, int nTileCols);

	// ����������֧��
	virtual void SetOptimizedScrollScreen(BOOL bOptimized)	{m_bOptimizedScrollScreen = bOptimized;}
	virtual BOOL GetOptimizedScrollScreen()					{return m_bOptimizedScrollScreen;}

	// ���п���ʱ��ʱ���õ�ͼԤ�ȴ���һЩ����
	virtual void OnHandleIdle(DWORD dwParam);

	// Ϊ�˷�ֹ��ͣ�ĸ��½ӿڣ��������һ������ķ���������Ҫ���빦�ܶ���ϣ���޸Ľӿ�ʱ��������ʱ�����������ʵ��
	virtual int OnSomeHandle(DWORD dwParam1, DWORD dwParam2);
private:
	void AddItemToDisplayList(const POINT& ptTile, CMapItemView* pItem);
	void UpdateItemInDisplayList(const POINT& ptTile, CMapItemView* pItem, BOOL bVisible);
	void RemoveItemFromDisplayList(CMapItemView* pItem);
	void AddItemToLogicList(CMapItemView* pItem);
	void RemoveItemFromLogicList(CMapItemView* pItem);
public:
	void DrawObjectInMiniMap(ICanvas *pCanvas, ITTFont* pFont);
	IArchive* GetMapArchive();
	BOOL IsIntersect(CMapItemView& item1, CMapItemView& item2, RECT& rc);
	int CmpPointPoint(CMapItemView& item1, CMapItemView& item2);
	int CmpPointLine(CMapItemView& item1, CMapItemView& item2);
	int CmpLineLine(CMapItemView& item1, CMapItemView& item2);
	int Compare(CMapItemView& item1, CMapItemView& item2);
	void AddSortItem0(CMapItemView* pItem, DisplayList& _DisplayList);
	void UpdateSortItem0(CMapItemView* pItem, DisplayList& _DisplayList);
	void AddSortItem1(CMapItemView* pItem, DisplayList& _DisplayList);
	void UpdateSortItem1(CMapItemView* pItem, DisplayList& _DisplayList);
	void AddSortItem2(CMapItemView* pItem, DisplayList& _DisplayList);
	void UpdateSortItem2(CMapItemView* pItem, DisplayList& _DisplayList);
	void AddSortItemPoint(CMapItemView* pItem, DisplayList& _DisplayList);
	void AddSortItem3(CMapItemView* pItem, DisplayList& _DisplayList);
	void AddSortItem31(CMapItemView* pItem, DisplayList& _DisplayList);
	void UpdateSortItem3(CMapItemView* pItem, DisplayList& _DisplayList);
	void AddPointToList();

private:
	BOOL SaveMapBitmapToFile(LPCSTR szFileName, int nStartRow, int nStartCol, int nRows, int nCols, 
		float fRate, BOOL bDrawGround, BOOL bCombine);
	BOOL StretchBitmap(LPCSTR szFileName, int nNewWidth, int nNewHeight);

#ifdef _DEBUG
private:
	void DrawRect(ICanvas *pCanvas, RECT& rc, WPixel clr);
#endif
};
//------------------------------------------------------------------------
