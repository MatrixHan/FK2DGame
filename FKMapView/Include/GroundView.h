/**
*	created:		2012-7-1   2:10
*	filename: 		GroundView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "JpegLoader.h"
#include "ScrollSheet.h"
#include "MapCoordinate.h"
//------------------------------------------------------------------------
#ifdef _DEBUG	
#define GROUND_CACHE_BITMAP_COUNT	60
#else
#define GROUND_CACHE_BITMAP_COUNT	60
#endif
//------------------------------------------------------------------------
class CGroundView : public IScrollSheetSink
{
private:
	int m_nMapWidth;		// ��ͼ��С
	int m_nMapHeight;
	RECT m_rcMapRect;		// ������ͼ���ؾ��Σ������ӿ��Ƿ�Խ����
	int m_nViewWidth;		// �ӿڴ�С
	int m_nViewHeight;
	int m_nGridWidth;		// һ����Ԫ��Ĵ�С
	int m_nGridHeight;		
	int	m_nTotalGridX;		// x,y�����ϸ��������������õ������Լ����
	int	m_nTotalGridY;
	RECT m_rcViewportRect;		// �ӿڵ����ؾ��󣬾�������
	RECT m_rcViewportTileRect;	// �ӿڵ�tile����
	CJpegLoader m_jpgLoader;
	CMapCoordinate	m_MapCo;	// ����ת��
	CScrollSheet	m_ScrollSheet;
	CFIFOCache<IBitmap*,int,GROUND_CACHE_BITMAP_COUNT> m_BitmapRecycle;	

public:
	int GetIndexByPoint(POINT& pt);
	void DrawTileLine(ICanvas* pCanvas);
	void PixelToTile(const POINT &ptScreen, POINT &ptTile) const;
	void PixelToViewTop(const POINT &ptTile, POINT &ptTileCenter) const;
	void TileToPixel(const POINT &ptTile, POINT &ptTileCenter) const;
	void ScrollViewport(int dx, int dy);
	void OnDraw(ICanvas* pCanvas);


	const RECT& GetViewportRect() const
	{
		return m_rcViewportRect;
	}
	const RECT& GetViewportTileRect() const
	{
		return m_rcViewportTileRect;
	}
	const CMapCoordinate& GetMapCoordinary() const
	{
		return m_MapCo;
	}
	int GetMapWidth() const
	{
		return m_nMapWidth;
	}
	int GetMapHeight() const
	{
		return m_nMapHeight;
	}

	const RECT& GetMapRect() const
	{
		return m_rcMapRect;
	}

	int GetViewTopX() const
	{
		return m_ScrollSheet.GetViewTopX();
	}

	int GetViewTopY() const
	{
		return m_ScrollSheet.GetViewTopY();
	}

	int GetViewWidth() const
	{
		return m_nViewWidth;
	}
	int GetViewHeight() const
	{
		return m_nViewHeight;
	}
	void Close();
	BOOL Create(int nMapWidth,int nMapHeight,int nGridWidth,int nGridHeight,
		LPRECT lprcViewport, const char* szJpgFileDir = NULL, IFileObject* pGridIdxArrayFile = NULL, ITrace*pTrace = NULL);
	BOOL MoveViewportTo(int x,int y);
	BOOL SizeViewport(int nWidth,int nHeight);
	CGroundView()
	{
		m_nMapWidth = 0;
		m_nMapHeight = 0;
		m_nViewWidth = 0;
		m_nViewHeight = 0;
		m_nTotalGridX = 0;
		m_nTotalGridY = 0;
		m_nGridHeight = 400;
		m_nGridWidth = 300;
		m_pMapGrid = NULL;

		memset(&m_rcViewportTileRect,0,sizeof(m_rcViewportTileRect));
		memset(&m_rcMapRect,0,sizeof(m_rcMapRect));
	}

	~CGroundView()
	{
		Close();
	}

private:
	void GetBackgroundLoadingBitmap();
	virtual void OnGridChanged(int nMoveDirection, int nDx, int nDy){}
	virtual void OnDirtyGridFound(int nGridX,int nGridY);	// ����һ�����
	virtual void OnDirtyGridLost(int nGridX,int nGridY);	// ����һ������Ѿ�̫Զ����˿��Կ���ɾ�������

	struct MAP_GRID
	{
		enum STATUS{None = 0,Requesting = 1/*���������̨����*/, Loaded = 2/*�Ѿ��������*/,LoadError = 3/*�������*/};
		MAP_GRID()
		{
			Init();
			wBmpIndex = 0xFFFF;
		}
		void Init()
		{
			pBmp = NULL;
			status = None;
		}
		void SetReqesting(){status = Requesting;}
		void SetLoaded(IBitmap* pBmp)
		{
			this->pBmp = pBmp;
			status = Loaded;
		}
		void SetLoadError(){status = LoadError;pBmp = NULL;}

		BOOL IsLoaded(){return pBmp != NULL;}
		BOOL IsNeedLoad(){return status == None;}
		BOOL IsRequesting(){return status == Requesting;}
		BOOL IsLoadError(){return status == LoadError;}
		BOOL IsEmptyBitmap() {return wBmpIndex == 0xFFFF;} // �˿�Ϊ�գ��༭ʱδ��ͼ
	public:
		int GetViewTopY();
		int GetViewTopX();
	public:
		IBitmap*	pBmp;
		WORD		status;
		WORD		wBmpIndex;
	};
	MAP_GRID*		m_pMapGrid;	// ��¼ĳ���bitmap�Ƿ��Ѿ�������

protected:
	void CalcViewTileRect();
};
//------------------------------------------------------------------------
