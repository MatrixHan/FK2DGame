/**
*	created:		2012-7-1   2:49
*	filename: 		SnapShot
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Geometry.h"
#include "MapContainer.h"
//------------------------------------------------------------------------
/*
  ���ܣ�ȡ��һ���߶�(�������߶εĵ�һ��)�ϸ���Ŀ���
  ������
	���룺
		(x1,y1),(x2,y2)	: ��ʼ����յ㣬��Ϸ������������ϵ
		nListCount	: pListBuf�����ά��
	�����
		m_nListCount	: ���� m_pListBuf ����Ч�Ķ�������
		m_pListBuf		: �������ж��󣬲�����(x1,y1)�п��ܴ��ڵĶ���
  ʹ�÷�����
		CSnapshotLine<4096> line(&m_MapContainer,x1,y1,x2,y2);
		// line.m_nFoundCount��line.m_ListBuf�а����˷��ؽ��
*/
//------------------------------------------------------------------------
template<int nMaxItemCount>
class CSnapshotLine : public CLineDiffuse
{
public:
	CSnapshotLine(CMapContainer* pMap,int x1, int y1, int x2, int y2)
	{
		Create(pMap->GetMapArrayWidth(),pMap->GetMapArrayWidth());

		m_pMap = pMap;
		m_nFoundCount = 0;
		Line(x1,y1,x2,y2);
	}
	virtual int OnFoundTile(int x,int y)
	{
		if(m_nFoundCount >= nMaxItemCount)
		{
			TraceLn("Error : CSnapshotLine::OnFoundTile() : to %d items found,but %d max",m_nFoundCount,nMaxItemCount);
			return retStop;
		}
		CTile& tile = m_pMap->GetTile(x,y);
		if(!tile.IsValid())	// �˿�Ϊ������������õ���Ч�飬����
			return retTrue;
		m_ListBuf[m_nFoundCount].x = x;
		m_ListBuf[m_nFoundCount].y = y;
		m_nFoundCount++;
		return retTrue;
	}
public:
	int		m_nFoundCount;					// �����ҵ���item����
	POINT	m_ListBuf[nMaxItemCount];		// ����item����
private:
	CMapContainer*	m_pMap;
};
//------------------------------------------------------------------------
/*
	���ܣ� �������Ŵ��߶�(�������߶εĵ�һ��)�����߻����ʱ������һ������Ҫ���tile��item
	˵����
		dwStopFlagΪtile��item��Ҫ����ı�־λ���μ�CTile��־λ���壬һ���ȡ��maskBlock��maskFly��maskOccupant
		������ֿ��赲����û����item��*ppItem ֵ�ᱻ����Ϊ NULL����ʱһ������Ϊ�ر��赲�����ر���û�ж���
	���룺
		dwStopFlag : �������ϴ˱�־�Ŀ�����ֹ����
	�����
		m_bFoundTile : ����ҵ��赲�飬����TRUE�����򷵻�FALSE
		m_ptStopTile : ���ؽ�����
		m_pStopItem	 : �����ҵ���item����Ҫû�ҵ�item����m_pStopItem������ΪNULL
	ʹ�÷�����
		CSnapshotShoot line(&m_MapContainer,CTile::maskFly,x1,y1,x2,y2)
		line.Line(x1,y1,x2,y2);
		if(line.m_bFoundTile)
		{
			//line.m_ptStopTile��line.m_pStopItem�����˷��ؽ��
		}
*/
class CSnapshotShoot : public CLineDiffuse
{
public:
	CSnapshotShoot(CMapContainer* pMap,DWORD dwStopFlag,int x1, int y1, int x2, int y2)
	{
		Create(pMap->GetMapArrayWidth(),pMap->GetMapArrayWidth());

		m_pMap = pMap;
		m_dwStopFlag = dwStopFlag;

		m_bFoundTile = FALSE;
		m_pStopItem = NULL;

		Line(x1,y1,x2,y2);
	}
	virtual int OnFoundTile(int x,int y)
	{
		CTile& tile = m_pMap->GetTile(x,y);
		if(!tile.IsValid())	// �˿�Ϊ������������õ���Ч�飬����
			return retTrue;
		if(tile.GetFlag() & m_dwStopFlag)
		{
			m_bFoundTile = TRUE;
			m_ptStopTile.x = x; m_ptStopTile.y = y;
			int nCount = tile.GetLayerCount();				
			for(int i = 0; i < nCount; i++)					
			{												
				CMapItem* pItem = tile.GetLayer(i);			
				if(pItem->GetOccupantValue() & m_dwStopFlag)	
					m_pStopItem = pItem;					
			}												
			return retStop;										
		}
		return retTrue;
	}

public:
	BOOL		m_bFoundTile;	// �����Ƿ��ҵ�tile
	POINT		m_ptStopTile;	// �����ҵ���tile
	CMapItem*	m_pStopItem;	// �����ҵ���item

private:
	CMapContainer*	m_pMap;
	DWORD			m_dwStopFlag;
};
//------------------------------------------------------------------------
/* ���ܣ����������ڻ��湥��ʱ�������������ϰ�ǰ�����ǵ����ж���(�������߶εĵ�һ��)
   ˵����
		dwTileFlagΪtile��item��Ҫ����ı�־λ���μ�CTile��־λ���壬һ���ȡ��maskBlock��maskFly��maskOccupant
		������ֿ��赲����û����item��*ppItem ֵ�ᱻ����Ϊ NULL����ʱһ������Ϊ�ر��赲�����ر���û�ж���
	���룺
		dwStopFlag : �������ϴ˱�־�Ŀ�����ֹ����
		nListCount : pListBuf�����ά��
	�����
		����ҵ����󣬷���TRUE�����򷵻�FALSE
		����ɹ���nListCount��pListBuf���ض����б�
	ʹ�÷�����
		CSnapshotLine<4096> line(&m_MapContainer,x1,y1,x2,y2);
		// line.m_nFoundCount��line.m_ListBuf�а����˷��ؽ��
			
*/
template<int nMaxItemCount>
class CSnapshotShootItemCovered : public CLineDiffuse
{
public:
	CSnapshotShootItemCovered(CMapContainer* pMap,int x1, int y1, int x2, int y2)
	{
		Create(pMap->GetMapArrayWidth(),pMap->GetMapArrayWidth());
		m_pMap = pMap;
		m_nFoundCount = 0;

		Line(x1,y1,x2,y2);
	}
	virtual int OnFoundTile(int x,int y)
	{
		if(m_nFoundCount >= nMaxItemCount)
		{
			TraceLn("Error : CSnapshotShootItemCovered::OnFoundTile() : to %d items found,but %d max",m_nFoundCount,nMaxItemCount);
			return retStop;
		}
		CTile& tile = m_pMap->GetTile(x,y);
		if(!tile.IsValid())	// �˿�Ϊ������������õ���Ч�飬����
			return retTrue;
		m_ListBuf[m_nFoundCount].x = x;
		m_ListBuf[m_nFoundCount].y = y;
		m_nFoundCount++;
		return retTrue;
	}
public:
	int		m_nFoundCount;					// �����ҵ���item����
	POINT	m_ListBuf[nMaxItemCount];		// ����item����
private:
	CMapContainer*	m_pMap;
};
//------------------------------------------------------------------------
/*
  ���ܣ�ȡ��һ���߶�(�������߶εĵ�һ��)�����ж���Ŀ���
  ������
	���룺
		(x1,y1),(x2,y2)	: ��ʼ����յ㣬��Ϸ������������ϵ
		nListCount	: pListBuf�����ά��
	�����
		m_nListCount	: ���� m_pListBuf ����Ч�Ķ�������
		m_pListBuf		: �������ж��󣬲�����(x1,y1)�п��ܴ��ڵĶ���
  ʹ�÷�����
		CSnapshotLine<4096> line(&m_MapContainer,x1,y1,x2,y2);
		// line.m_nFoundCount��line.m_ListBuf�а����˷��ؽ��
*/
template<int nMaxItemCount>
class CSnapshotItemOnLine : public CLineDiffuse
{
public:
	CSnapshotItemOnLine(CMapContainer* pMap,int x1, int y1, int x2, int y2)
	{
		Create(pMap->GetMapArrayWidth(),pMap->GetMapArrayWidth());
		m_pMap = pMap;
		m_nFoundCount = 0;
		Line(x1,y1,x2,y2);
	}
	virtual int OnFoundTile(int x,int y)
	{
		CTile& tile = m_pMap->GetTile(x,y);
		if(!tile.IsValid())	// �˿�Ϊ������������õ���Ч�飬����
			return retTrue;
		int nItemCount = tile.GetLayerCount();
		for(int tt = 0; tt < nItemCount; tt++)
		{
			if(m_nFoundCount >= nMaxItemCount)
			{
				TraceLn("Error : CSnapshotItemOnLine::OnFoundTile() : to %d items found,but %d max",m_nFoundCount,nMaxItemCount);
				return retStop;
			}
			m_ListBuf[m_nFoundCount].ptTile.x = x;
			m_ListBuf[m_nFoundCount].ptTile.y = y;
			m_ListBuf[m_nFoundCount].pItem = tile.GetLayer(tt);
			m_nFoundCount++;
		}
		return retTrue;
	}
public:
	int			m_nFoundCount;					// �����ҵ���item����
	SNAP_ITEM	m_ListBuf[nMaxItemCount];		// ����item����
private:
	CMapContainer*	m_pMap;
};
//------------------------------------------------------------------------
class CSnapshotItemByWorldRect : public CEnumTileArea
{
public:
	CSnapshotItemByWorldRect(CMapContainer* pMap, const RECT &rcWorld, 
		int nMaxItemCount, SNAP_ITEM* pListBuf)
	{
		Create(pMap->GetMapArrayWidth());
		m_pMap = pMap;
		m_nMaxItemCount = nMaxItemCount;
		m_pListBuf = pListBuf;
		m_nFoundCount = 0;
		EnumTileArea(pMap->GetMapCoordinary().PixelRectToAreaTileRect(rcWorld));
	}
	virtual int OnFoundTile(int x, int y)
	{
		try{
		CTile& tile = m_pMap->_GetTile(x,y);
		int nItemCount = tile.GetLayerCount();
		for (int tt=0; tt<nItemCount; tt++)
		{
			if (m_nFoundCount >= m_nMaxItemCount)
			{
				return retStop;
			}
			m_pListBuf[m_nFoundCount].ptTile.x = x;
			m_pListBuf[m_nFoundCount].ptTile.y = y;
			m_pListBuf[m_nFoundCount].pItem = tile.GetLayer(tt);
			m_nFoundCount++;
		}
		}catch (...) {
		}
		return retTrue;
	}
public:
	int			m_nMaxItemCount;
	int			m_nFoundCount;	// �����ҵ���item����
	SNAP_ITEM*	m_pListBuf;		// ����item����
private:
	CMapContainer*	m_pMap;
};
//------------------------------------------------------------------------
class CSnapshotTileByWorldRect : public CEnumTileArea
{
public:
	CSnapshotTileByWorldRect(CMapContainer* pMap, const RECT &rcWorld,
		int nMaxTileCount, SNAP_TILE* pListBuf)
	{
		Create(pMap->GetMapArrayWidth());
		m_pMap = pMap;
		m_nMaxTileCount = nMaxTileCount;
		m_pListBuf = pListBuf;
		m_nFoundCount = 0;
		EnumTileArea(pMap->GetMapCoordinary().PixelRectToAreaTileRect(rcWorld));
	}
	virtual int OnFoundTile(int x, int y)
	{
		CTile& tile = m_pMap->_GetTile(x, y);
		ASSERT(tile.IsValid());
		if (m_nFoundCount >= m_nMaxTileCount)
		{
			return retStop;
		}
		m_pListBuf[m_nFoundCount].ptTile.x = x;
		m_pListBuf[m_nFoundCount].ptTile.y = y;
		m_pListBuf[m_nFoundCount].pTile = &tile;
		m_nFoundCount++;
		
		return retTrue;
	}
public:
	int			m_nMaxTileCount;
	int			m_nFoundCount;	// �����ҵ���TILE����
	SNAP_TILE*	m_pListBuf;		// ����TILE����
private:
	CMapContainer*	m_pMap;
};
//------------------------------------------------------------------------