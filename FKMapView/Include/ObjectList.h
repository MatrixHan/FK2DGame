/**
*	created:		2012-7-1   5:16
*	filename: 		ObjectList
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�����б����
*/
//------------------------------------------------------------------------
/*
 *	�ͻ��˵�������Ԥ������������õú�С������list�����������Ƚϴ󣬱�֤
 ���������ж���add��remove��moveʱ��������⡣���ǳ�����list�����СΪ16*16��
 ÿ��list�����ϵĶ����������¼����������ͼ��������Ҫ�����߼�����ʱ�����Դ�
 ��Щ�б������ȡ������������

  ����������Ƚ��鷳������ǣ�浽�������ĵ���˳��������ʱ�����б�Ū��һ��
  ���б���������ͼ�����á������ܼӽ����Ķ��󣬷����������9����Ŀ��ơ�
 */
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/MapInfo/MapItem.h"
#include <list>
using std::list;
//------------------------------------------------------------------------
enum ErrType{ERR_INVALID=-1,ERR_FAILED=0,ERR_SUCCESS=1};
//------------------------------------------------------------------------
struct stMapItemInfo
{
	CMapItem*	pItem;
	POINT		ptTile;
	POINT		ptGrid;
	stMapItemInfo(CMapItem* p, const POINT& tile, const POINT& grid)
		: pItem(p), ptTile(tile), ptGrid(grid) {}
	BOOL operator == (const stMapItemInfo& info)
	{
		return (info.pItem == pItem && 
			info.ptTile.x == ptTile.x && info.ptTile.y == ptTile.y);
	}
};
//------------------------------------------------------------------------
typedef list<stMapItemInfo>				MapItemInfoList;
typedef list<stMapItemInfo>::iterator	MapItemInfoListPtr;
//------------------------------------------------------------------------
class CObjectList  
{
protected:
	MapItemInfoList		m_ObjList;
	MapItemInfoListPtr	m_pCurObj;

public:
	CObjectList()
	{
		Clear();
	}
	virtual ~CObjectList()
	{
		Clear();
	}
	bool IsEmpty()
	{
		return m_ObjList.empty();
	}
	void Clear()
	{
		m_ObjList.clear();
	}
	int GetCount()
	{
		return m_ObjList.size();
	}
	MapItemInfoList* GetListPtr()
	{
		return &m_ObjList;
	}
	void Add(CMapItem* pItem, const POINT& ptTile, const POINT& ptGrid)
	{
		if (pItem == NULL) return;
		m_ObjList.push_back(stMapItemInfo(pItem, ptTile, ptGrid));
	}
	int Remove(CMapItem* pItem, const POINT& ptTile)
	{
		if (pItem == NULL) return ERR_INVALID;
		MapItemInfoListPtr first = m_ObjList.begin();
		MapItemInfoListPtr last = m_ObjList.end();
		while (first != last)
		{
			MapItemInfoListPtr next = first;
			++next;
			if ((*first).pItem == pItem)
			{
				m_ObjList.erase(first);
				return ERR_SUCCESS;
			}
			first = next;
		}
		return ERR_FAILED;
	}
	int Move(CMapItem* pItem, const POINT& ptTileFrom, const POINT& ptTileTo, const POINT& ptGridTo)
	{
		if (pItem == NULL) return ERR_INVALID;
		MapItemInfoListPtr first = m_ObjList.begin();
		MapItemInfoListPtr last = m_ObjList.end();
		while (first != last)
		{
			MapItemInfoListPtr next = first;
			++next;
			if ((*first).pItem == pItem)
			{
				(*first).ptTile = ptTileTo;
				(*first).ptGrid = ptGridTo;
				return ERR_SUCCESS;
			}
			first = next;
		}
		Add(pItem, ptTileTo, ptGridTo);
		return ERR_FAILED;
	}

	stMapItemInfo* FindFirstItem(int nGridX, int nGridY)
	{
		m_pCurObj = m_ObjList.begin();
		MapItemInfoListPtr last = m_ObjList.end();
		while (m_pCurObj != last)
		{
			MapItemInfoListPtr next = m_pCurObj;
			++next;
			stMapItemInfo& info = (*m_pCurObj);
			if (info.ptGrid.x == nGridX && info.ptGrid.y == nGridY)
				return &(*m_pCurObj);
			m_pCurObj = next;
		}
		return NULL;
	}
	stMapItemInfo* FindNextItem(int nGridX, int nGridY)
	{
		++m_pCurObj;
		MapItemInfoListPtr last = m_ObjList.end();
		while (m_pCurObj != last)
		{
			MapItemInfoListPtr next = m_pCurObj;
			++next;
			stMapItemInfo& info = (*m_pCurObj);
			if (info.ptGrid.x == nGridX && info.ptGrid.y == nGridY)
				return &(*m_pCurObj);
			m_pCurObj = next;
		}
		return NULL;
	}
};
//------------------------------------------------------------------------