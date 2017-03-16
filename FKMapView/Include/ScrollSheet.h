/**
*	created:		2012-7-1   1:31
*	filename: 		ScrollSheet
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <windows.h>
//------------------------------------------------------------------------
#ifndef STRUCT_AREA
#define STRUCT_AREA
typedef struct tagAREA
{
	int x,y,w,h;
} AREA, * LPAREA;
#endif
//------------------------------------------------------------------------
#ifndef PixelCood2GridCood
#define PixelCood2GridCood(PixelPos, GridLength)	((PixelPos + GridLength - 1) / GridLength)
#endif
//------------------------------------------------------------------------
// ���ƶ�ʱ�����µ������ֶ�ʧ�飬֪ͨ�������¼�����
struct IScrollSheetSink
{
	virtual void OnGridChanged(int nMoveDirection, int nDx, int nDy) = NULL; // ��������ı�
	virtual void OnDirtyGridFound(int nGridX,int nGridY) = NULL;	// ����һ�����
	virtual void OnDirtyGridLost(int nGridX,int nGridY) = NULL;	// ����һ������Ѿ�̫Զ����˿��Կ���ɾ�������
};
//------------------------------------------------------------------------
class CScrollSheet  
{
public:
	BOOL ScrollToCenter(const RECT& rcView);
	CScrollSheet()
	{
		m_pScrollSheetSink = NULL;

		m_nMapWidth = 0;
		m_nMapHeight = 0;
		m_nViewWidth = 0;
		m_nViewHeight = 0;
		m_nViewTopX = 0;
		m_nViewTopY = 0;


		m_nGridHeight = 0;
		m_nGridWidth = 0;

		memset(&m_rcCurViewGrid,0,sizeof(RECT));
		memset(&m_rcMapGrid,0,sizeof(RECT));
		memset(&m_rcLastDirty,0,sizeof(RECT));
		memset(&m_rcViewportRect,0,sizeof(RECT));
		memset(&m_rcMap,0,sizeof(RECT));
	}
	~CScrollSheet()
	{

	}

	int GetViewTopX() const
	{
		return m_nViewTopX;
	}

	int GetViewTopY() const
	{
		return m_nViewTopY;
	}

	int GetViewWidth() const
	{
		return m_nViewWidth;
	}
	int GetViewHeight() const
	{
		return m_nViewHeight;
	}

	// ��ǰ�ӿ������ǵ����п�����ؾ��󣬻�����������
	void GetViewportCoveredGridRect(RECT& rcCoveredRect)
	{
		rcCoveredRect.left = m_rcCurViewGrid.left * m_nGridWidth;
		rcCoveredRect.right = m_rcCurViewGrid.right * m_nGridWidth;
		rcCoveredRect.top = m_rcCurViewGrid.top * m_nGridHeight;
		rcCoveredRect.bottom = m_rcCurViewGrid.bottom * m_nGridHeight;
	}
	RECT& GetCurViewGrid() {return m_rcCurViewGrid;}

	BOOL Create(IScrollSheetSink* pSink,int nMapWidth,int nMapHeight,
		int nGridWidth,int nGridHeight,LPRECT lprcViewport);
	void Close();

	BOOL MoveViewportTo(int x,int y);
	BOOL ScrollViewport(int dx,int dy);
	BOOL SizeViewport(int nWidth,int nHeight);
	RECT GetViewportRect()
	{
		return m_rcViewportRect;
	}

private:
	void SetViewTileRect(RECT rc);
	IScrollSheetSink *m_pScrollSheetSink;
private:
	int m_nMapWidth;		// ��ͼ��С(����)
	int m_nMapHeight;
	int m_nViewWidth;		// �ӿڴ�С(����)
	int m_nViewHeight;
	int m_nViewTopX;		// �ӿ����Ͻ�����(����)
	int m_nViewTopY;
	RECT m_rcViewportRect;	// �ӿھ���(����)
	RECT m_rcMap;			// ��ͼ����(����)
	int m_nGridWidth;		// һ����Ԫ��Ĵ�С(����)
	int m_nGridHeight;		
	RECT m_rcCurViewGrid;	// ��ǰ�ɼ��ӿ����ڵ����п�(������)
	RECT m_rcMapGrid;		// ȫͼ�����(������)
	RECT m_rcLastDirty;		// ��һ�������Χ������ֱ��ˮƽ�ƶ�ʱ�����Ż�Ϊ����չǰ�������ϵĿ飬����Ͳ��治������Ϊ��
};
//------------------------------------------------------------------------