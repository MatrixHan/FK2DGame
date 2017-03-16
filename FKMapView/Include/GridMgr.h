/**
*	created:		2012-7-1   5:12
*	filename: 		GridMgr
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�����������������Ź���Ĺ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../Include/MapContainer.h"
//------------------------------------------------------------------------
#ifndef STRUCT_AREA
#define STRUCT_AREA
typedef struct tagAREA
{
	int x,y,w,h;
} AREA, * LPAREA;
#endif
//------------------------------------------------------------------------
struct IScrollSheetSink;
//------------------------------------------------------------------------
class CGridMgr  
{
	RECT				m_rcMapGrid;		// ������ͼ���������(��������)
	RECT				m_rcPlayerGrid;		// ��һ������(���������Χ�ڲ��ᵼ�µ�ͼ����Ĺ���)
	POINT				m_ptPlayerGrid;		// �����������
	RECT				m_rcOldPreReadGrid;	// ��ǰ��Ԥ���������ڵ��������(��������)
	RECT				m_rcPreReadGrid;	// Ԥ������(��������)
	RECT				m_rcOldViewGrid;	// ��ǰ�Ŀ����������ڵ��������(��������)
	RECT				m_rcViewGrid;		// �����������ڵ��������(��������)
	BOOL				m_bUseDynamicMap;	// �Ƿ�ʹ�ö�̬��ͼ
	IScrollSheetSink*	m_pScrollSheetSink; // ���ڶ�����(����)���������д���
public:
	CGridMgr();
	~CGridMgr() {}

	BOOL Create(IScrollSheetSink* pSink, int nMapWidth, int nMapHeight, 
		POINT& ptPlayerPos, BOOL bUseDynamicMap = FALSE);
	void Close();

	BOOL ScrollViewport(int dx, int dy);
	BOOL SizeViewport(int nWidth, int nHeight);
	BOOL ScrollToCenter(POINT& ptPlayerPos);

	void GetGridRectBelowViewport(RECT& rcCoveredRect);
	void GetGridRectBelowPreRead(RECT& rcCoveredRect);
	RECT& GetPreReadGrid()			{return m_rcPreReadGrid;}
	RECT& GetViewGrid()				{return m_rcViewGrid;}
	RECT& GetMapGrid()				{return m_rcMapGrid;}
	POINT GetPlayerGrid()			{return m_ptPlayerGrid;}
	BOOL  IsUseDynamicMap()			{return m_bUseDynamicMap;}
	
	void SnapshotLeftTop(int& nRows, int& nCols, BLOCK_INFO* pBlockInfo, int nMoveDirection);
	int GetBlockIndex(POINT& ptLeftTop);
private:
	void SetPreReadTileRect(RECT rc);
	void ClosePreReadTileRect();
	void SetViewTileRect(RECT rc);
	void CloseViewTileRect();
	int  GetMoveDirection(POINT& ptFrom, POINT& ptTo, int& nDx, int& nDy);
	void LimitRect(RECT& rcSmall, const RECT& rcBig);
	void ResetPlayerRect();
};
//------------------------------------------------------------------------