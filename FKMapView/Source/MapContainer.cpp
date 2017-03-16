/**
*	created:		2012-7-1   2:45
*	filename: 		MapContainer
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
/*
 * ���ܣ�
		��ͼ���������ࡣ
		server��clientͨ�ã����ж���֧��CMapItem�ӿ�
 		����ر��赲��Ϣ�������NPC������ȶ������ɾ�飻·����Ч�Ժ�·�����ҹ���
   ʵ�֣�
		 ��Ϸ���󴴽����������ж��󣬽���������CMapContainer����
   �㷨˵����
		һ����ͼ�ϵ�һ�飬�˿�֮�Ͽ��԰������������󣬿��Գ�֮Ϊ��״����
			��������ʾ��ͼ���£�
			  |	  <- CMapItem	   \
			  |	  <- CMapItem		\  ������Ϊ
			  |	  <- CMapItem       /  CLayerList
			  |   <- CMapItem	   /
			 ---  CTile
		��������Ĳ��ͼ�ھ���ʵ��ʱ�����Ż���
			ֻ��һ�����ʱ,CTile::m_pLayerList��ΪCMapItem���ָ�룬ͨ��maskIsMapItem��־λ������
			�������ϵĽ�����Ҫ����CLayerList���ж�����
	    ����  ��Χ����ʱ��Ϊ������ٶȣ��ṩһ�������ڽӿڵĻص����ƣ����ͼѭ��������������ħ��������Χ��
			  ִ�й������£�
				  1���ⲿ����һ�������CMapContainer���������Ρ�Բ���߶�(δʵ��)��
				  2��CMapContainer����ȡ����������ÿ����Ч�㣬
				   �ص��ⲿ����IMapItemEnumSink::OnEnumMapItem()
			  �ŵ��ǣ������˴������������Ч�ԵĴ��룻�����˾�������ĳ˷�����
			  ע�⣺�����ҷ�Χ��Сʱ������ʹ��GetTile()�𲽲���
		�ġ��ṩ������㲥��ʽ�����Χ����
			ͬ(��)�ķ������ƣ�ֱ�ӽ��������ͼ�еĶ���
		����Snapshotϵͳ�㲥������Ϊ�˽�����������������
 */
//------------------------------------------------------------------------
#include "../Include/MapContainer.h"
#include "../../FKGame/SceneObj/IItemClassFactory.h"
#include "../Include/Snapshot.h"
#include "../Include/MapViewCommon.h"
//------------------------------------------------------------------------
CTile CMapContainer::m_EmptyTile;
//------------------------------------------------------------------------
CMapContainer::ProcScrollTilesInfo CMapContainer::m_fnScrollTileInfo[mdMaxUpdateType] =
{
	&CMapContainer::_MovePreReadRect_Top,
	&CMapContainer::_MovePreReadRect_RightTop,
	&CMapContainer::_MovePreReadRect_Right,
	&CMapContainer::_MovePreReadRect_RightDown,
	&CMapContainer::_MovePreReadRect_Down,
	&CMapContainer::_MovePreReadRect_LeftDown,
	&CMapContainer::_MovePreReadRect_Left,
	&CMapContainer::_MovePreReadRect_LeftTop,
	&CMapContainer::_MovePreReadRect_All,
};
//------------------------------------------------------------------------
CMapContainer::CMapContainer()
{
	m_ptTileOrigin.x = m_ptTileOrigin.y = 0;
	m_nMatrixWidth = m_nMatrixHeight = 0;
	memset(&m_rcMapTileRect,0,sizeof(m_rcMapTileRect));
	m_pMultiValueTable = NULL;
	m_szGround[0] = 0;

	m_pTiles = NULL;
	m_pMapTable = NULL;
	m_pArchive = NULL;
	m_pItemCF = NULL;

	m_bUseDynamicMap = FALSE;
	
	m_nMapWidth = 0;
	m_nMapHeight = 0;
	memset(&m_rcMap,0,sizeof(m_rcMap));
}
//------------------------------------------------------------------------
CMapContainer::~CMapContainer()
{
	Close();
}
//------------------------------------------------------------------------
// nMapWidth��nMapHeight�ǵ�ͼ�����ش�С��������64(32)����
// rcPreRead�Ǿ�����ͼ��С�����ƺͽ���64(32)����֮���Ԥ���������
BOOL CMapContainer::Create(int nMapWidth, int nMapHeight, RECT& rcPreRead, ITrace* pTrace)
{
	Close();
	g_DefaultTrace.Create(pTrace);

	m_nMapWidth = nMapWidth;
	m_nMapHeight = nMapHeight;
	SetRect(&m_rcMap, 0, 0, nMapWidth, nMapHeight);

	m_MapCo.Create(nMapWidth, nMapHeight);

	m_LayerListCache.AddCacheList(64);

	// ����ԭ��
	if (IsUseDynamicMap())
	{
		POINT ptWorld, ptTileOrigin;
		ptWorld.x = rcPreRead.left;
		ptWorld.y = rcPreRead.top;
		m_MapCo.PixelToTile(ptWorld, ptTileOrigin);
		ptTileOrigin.y -= (rcPreRead.bottom - rcPreRead.top)/32 - 1;
		SetOriginTile(ptTileOrigin);
	}

	if (!CreateMapArray(rcPreRead))
	{
		return FALSE;
	}

	// ����Ѱ·ģ��
	CPathFinder_R<CMapContainer>::Create(this, m_MapCo.GetMatrixWidth(), m_MapCo.GetMatrixHeight());
	return TRUE;
}
//------------------------------------------------------------------------
// ������ͼ���ξ���
// nWidth��nHeight�ǵ�ͼ�����ش�С
BOOL CMapContainer::CreateMapArray(RECT& rcPreRead)
{
	// ��������TILE���Լ����θ�߳�TILE��
	int nCols = PixelCood2GridCood(rcPreRead.right - rcPreRead.left, 64);
	int nRows = PixelCood2GridCood(rcPreRead.bottom - rcPreRead.top, 32);
	m_nMatrixWidth = nCols + nRows;
	m_nMatrixHeight = m_nMatrixWidth - 1;

	// ���TILE����
	m_rcMapTileRect.left = m_rcMapTileRect.top = 0;
	m_rcMapTileRect.right = m_nMatrixWidth;
	m_rcMapTileRect.bottom = m_nMatrixHeight;

	// TILE����
	m_pTiles = new CTile[m_nMatrixWidth * m_nMatrixHeight]();
	TraceLn("����ͼTile����Ϊ = %d",m_nMatrixWidth * m_nMatrixHeight);
	if(m_pTiles == NULL)
		return FALSE;

	// ����˷�ֵ
	m_pMultiValueTable = new DWORD[m_nMatrixHeight];
	if (m_pMultiValueTable == NULL)
		return FALSE;
	for (int k=0,nVal=0; k<m_nMatrixHeight; k++,nVal+=m_nMatrixWidth)
		m_pMultiValueTable[k] = nVal;

	// ��ʼ����Ч��TILE
	InitBlockInfo((POINT&)rcPreRead, nRows, nCols*2);

	return TRUE;
}
//------------------------------------------------------------------------
void CMapContainer::SetOriginTile(POINT& ptTileOrigin)
{
	m_ptTileOrigin = ptTileOrigin;
	TraceLn("����ͼ���λ��Ϊ = [ %d,%d ]", m_ptTileOrigin.x, m_ptTileOrigin.y);
}
//------------------------------------------------------------------------
void CMapContainer::ScrollTilesInfo(BLOCK_INFO* pBlockInfo, int nRows, int nCols, 
		int nTileRow, int nTileCol, int nDirection, POINT& ptNewTileOrigin, int dx, int dy)

{
	(this->*m_fnScrollTileInfo[nDirection])(pBlockInfo, nRows, nCols, nTileRow, nTileCol, ptNewTileOrigin, dx, dy);
}
//------------------------------------------------------------------------
void CMapContainer::InitBlockInfo(POINT& ptLeftTop, int nTileRow, int nTileCol)
{
	POINT ptTileLeftTop;
	m_MapCo.PixelToTile(ptLeftTop, ptTileLeftTop);
	POINT ptCurTile;
	for (int row=0; row<nTileRow; row++) // һ����֮������
	{
		ptCurTile.x = ptTileLeftTop.x + row;
		ptCurTile.y = ptTileLeftTop.y - row;
		for (int col=0; col<nTileCol; col++) // ����
		{
			CTile& tile = _GetTile(ptCurTile);
			tile.Create();
			
			ptCurTile.x += !(col&1); // odd
			ptCurTile.y += col&1;
		}
	}
}
//------------------------------------------------------------------------
void CMapContainer::LoadBlockInfo(POINT& ptLeftTop, int nIndex, int nTileRow, int nTileCol)
{
	if (ptLeftTop.x + GRID_WIDTH > m_nMapWidth)
		nTileCol = PixelCood2GridCood(m_nMapWidth - ptLeftTop.x, 32);
	if (ptLeftTop.y + GRID_HEIGHT > m_nMapHeight)
		nTileRow = PixelCood2GridCood(m_nMapHeight - ptLeftTop.y, 32);
	InitBlockInfo(ptLeftTop, nTileRow, nTileCol);
	m_pArchive->SeekTo(m_pMapTable[nIndex]);
	POINT ptTileLeftTop;
	m_MapCo.PixelToTile(ptLeftTop, ptTileLeftTop);
	_LoadBlock(m_pArchive, ptTileLeftTop, nTileRow, nTileCol, m_pItemCF);
}
//------------------------------------------------------------------------
void CMapContainer::CopyBlockInfo(POINT& ptSrcLeftTop, POINT& ptDstLeftTop, int nTileRow, int nTileCol)
{
	POINT ptSrcTileLeftTop, ptDstTileLeftTop;
	m_MapCo.PixelToTile(ptSrcLeftTop, ptSrcTileLeftTop);
	m_MapCo.PixelToTile(ptDstLeftTop, ptDstTileLeftTop);
	int dx = ptDstTileLeftTop.x - ptSrcTileLeftTop.x;
	int dy = ptDstTileLeftTop.y - ptSrcTileLeftTop.y;
	for (int row=0; row<nTileRow; row++) // һ����֮������
	{
		POINT ptCurTile;
		ptCurTile.x = ptSrcTileLeftTop.x + row;
		ptCurTile.y = ptSrcTileLeftTop.y - row;
		for (int col=0; col<nTileCol; col++) // ����
		{
			GetTile(ptCurTile.x + dx, ptCurTile.y + dy) = GetTile(ptCurTile);
			
			ptCurTile.x += !(col&1); // odd
			ptCurTile.y += col&1;
		}
	}
}
//------------------------------------------------------------------------
void CMapContainer::CopyBlockInfoForSurface(POINT& ptSrcLeftTop, POINT& ptDstLeftTop, int nTileRow, int nTileCol)
{
	POINT ptSrcTileLeftTop, ptDstTileLeftTop;
	m_MapCo.PixelToTile(ptSrcLeftTop, ptSrcTileLeftTop);
	m_MapCo.PixelToTile(ptDstLeftTop, ptDstTileLeftTop);
	int dx = ptDstTileLeftTop.x - ptSrcTileLeftTop.x;
	int dy = ptDstTileLeftTop.y - ptSrcTileLeftTop.y;
	if (dx == 0 && dy == 0)
		return;
	for (int row=0; row<nTileRow; row++) // һ����֮������
	{
		POINT ptCurTile;
		ptCurTile.x = ptSrcTileLeftTop.x + row;
		ptCurTile.y = ptSrcTileLeftTop.y - row;
		for (int col=0; col<nTileCol; col++) // ����
		{
			// ֻ��ԴTILE�ĵر���Ϣ��ӵ�Ŀ��TILE
			CTile& srcTile = GetTile(ptCurTile);
			DWORD dwFlag = srcTile.GetFlag();
			dwFlag = srcTile.IsOccupant_Surface();

			CTile& dstTile = GetTile(ptCurTile.x + dx, ptCurTile.y + dy);
			DWORD dwFlagDstItem = dstTile.GetFlag();
			dwFlagDstItem &= ~ CTile::maskOccupant_Surface;
			dstTile.GetLayerCount();
			dwFlagDstItem |= dwFlag;
			dstTile.SetFlag(dwFlagDstItem);
			
			ptCurTile.x += !(col&1); // odd
			ptCurTile.y += col&1;
		}
	}
}
//------------------------------------------------------------------------
void CMapContainer::Close()
{
	if (m_pMultiValueTable)
	{
		delete[] m_pMultiValueTable;
		m_pMultiValueTable = NULL;
	}
	if(m_pTiles)
	{
		delete[] m_pTiles;
		m_pTiles = NULL;
		m_nMatrixWidth = 0;
	}

	//���ٵ�ͼ��Դ
	MAP_ITEM_LIST::iterator it   =  m_MapItemList.begin();
	MAP_ITEM_LIST::iterator end  =  m_MapItemList.end();
	for(;it != end; ++it)
	{
		SAFE_RELEASE((*it));
	}
	m_MapItemList.clear();
	m_LayerListCache.Close();
}
//------------------------------------------------------------------------
BOOL CMapContainer::MoveItem(const POINT& ptFrom, const POINT& ptTo, CMapItem* pItem, DWORD dwParam)
{
	if (!RemoveItem(ptFrom, pItem))
		return FALSE;
	return AddItem(ptTo, pItem);
}
//------------------------------------------------------------------------
// �ݲ�ʵ������
BOOL CMapContainer::AddLayerItem(const POINT &pt, CMapItem *pItem)
{
	return AddItem(pt,pItem);
}
//------------------------------------------------------------------------
BOOL CMapContainer::AddItem(const POINT& ptTile, CMapItem* pItem, DWORD dwParam)
{
	if (pItem == NULL)
		return NULL;
	CTile& tile = GetTile(ptTile);
	if (!tile.IsValid()) // tile��Ч
	{
		POINT pt;
		m_MapCo.TileToPixel(ptTile, pt);
		int nGridX = pt.x >> SHIFT_WIDTH;
		int nGridY = pt.y >> SHIFT_HEIGHT;
		return FALSE;	// ����ĳЩ������Դ�ĳ����̬��ͼ��Ե�ߵ���Ч�����������������Ч����Ȼ����TRUE���Ա�֤������߼�������ȷ
	}

	// ����ռλ���Ƿ��ص�
#ifdef _USE
	if (dwParam == 1)
	{
		if (pItem->GetOccupantTileList() == NULL)
		{
			CTile& tile = GetTile(ptTile);
			if ( tile.IsValid() &&tile.IsOccupant_Item())
				return FALSE;
		}
		else
		{
			if (!TestIntersect(ptTile, pItem))
				return FALSE;
		}
	}
#endif
	try
	{
		
	// ��Ӷ���
	if (tile.GetMapItem() == NULL) // tile��ԭ��û�ж���
		tile.SetOneLayer(pItem);
	else // tile��ԭ���ж���
	{ 
		// ��ǰֻ��һ�����ʱ������һ����㣬��ʱ����һ�����Ǩ��m_pLayerList��
		if (tile.IsMapItemValid())
		{
			CMapItem *pFirst = tile.GetMapItem();
			tile.SetNoLayer();	// �����ǰm_pMapItem
			//....... ����pItemռλ����ı�ر�flag
			if (!m_LayerListCache.AddData(tile.GetLayerListPointer(), pFirst))
				return FALSE;
		}
		// ��Ӷ����б���
		if (!m_LayerListCache.AddData(tile.GetLayerListPointer(), pItem))
		{
			return FALSE;
		}
	}

	// ����ռλ��
	if (pItem->GetOccupantTileList() == NULL)
		tile.Set_Occupant_Item(pItem->GetOccupantValue());
	else
		AddMultiOccupantValue(ptTile, pItem);
	}
	catch (...)
	{
	}
	return TRUE;
}
//------------------------------------------------------------------------
// ���һ��CTile���кܶ����ͬ�Ķ���Ӧ����ɾ�����ϵģ�
BOOL CMapContainer::RemoveItem(const POINT& ptTile, CMapItem* pItem, DWORD dwParam)
{
	CTile& tile = GetTile(ptTile);
	if (!tile.IsValid()) // tile��Ч
	{
		POINT ptOff = {ptTile.x - m_ptTileOrigin.x, ptTile.y - m_ptTileOrigin.y};
		if (ptOff.x >= 0 && ptOff.x < m_nMatrixWidth && ptOff.y >= 0 && ptOff.y < m_nMatrixHeight)
		{
			POINT pt, ptLeftTop, ptTileLeftTop = m_ptTileOrigin;
			m_MapCo.TileToPixel(ptTile, pt);
			ptTileLeftTop.y -= TILES_PREREAD_HEIGHT / 32 - 1;
			m_MapCo.TileToPixel(ptTileLeftTop, ptLeftTop);
			RECT rcPreRead;
			rcPreRead.left = ptLeftTop.x;
			rcPreRead.top = ptLeftTop.y;
			rcPreRead.right = rcPreRead.left + TILES_PREREAD_WIDTH;
			rcPreRead.bottom = rcPreRead.top + TILES_PREREAD_HEIGHT;
			if (PtInRect(&rcPreRead, pt)) // ��Ԥ��������
			{
				int nGridX = pt.x >> SHIFT_WIDTH;
				int nGridY = pt.y >> SHIFT_HEIGHT;
				return FALSE;
			}
			return TRUE; // ��Ԥ�����θ����Ч����!
		}
		return TRUE; // �������Ԥ�����������Թ�!
	}

	try{

	// �Ƴ�����
	if (tile.GetMapItem() == NULL) // tile��û�ж���
	{
		POINT pt;
		m_MapCo.TileToPixel(ptTile, pt);
		int nGridX = pt.x >> SHIFT_WIDTH;
		int nGridY = pt.y >> SHIFT_HEIGHT;
		return FALSE;
	}
	else if (tile.IsMapItemValid()) // tile����һ������
	{
		if (tile.GetMapItem() != pItem) // ������ָ���Ķ���
		{
			return FALSE;
		}
		tile.SetNoLayer();
	}
	else if (!m_LayerListCache.RemoveData(tile.GetLayerListPointer(), pItem)) // tile���ж������
	{
		return FALSE;
	}
	
	// ����ռλ��
	if (pItem->GetOccupantTileList() == NULL)
		tile.RemoveItemOccupant(pItem->GetOccupantValue());
	else
		RemoveMultiOccupantValue(ptTile, pItem);
	}
	catch (...)
	{
	}
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMapContainer::AddItemOccupant(const POINT& ptTile, CMapItem* pItem)
{
	if(pItem == NULL)
		return FALSE;

	CTile& tile = GetTile(ptTile);
	if (!tile.IsValid()) // tile��Ч
		return FALSE;

	// ����ռλ��
	if (pItem->GetOccupantTileList() == NULL)
		tile.Set_Occupant_Item(pItem->GetOccupantValue());
	else
		AddMultiOccupantValue(ptTile, pItem);

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMapContainer::RemoveItemOccupant(const POINT& ptTile, CMapItem* pItem)
{
	if(pItem == NULL)
		return FALSE;

	CTile& tile = GetTile(ptTile);
	if (!tile.IsValid()) // tile��Ч
		return FALSE;

	// ����ռλ��
	if (pItem->GetOccupantTileList() == NULL)
		tile.RemoveItemOccupant(pItem->GetOccupantValue());
	else
		RemoveMultiOccupantValue(ptTile, pItem);
	
	return TRUE;
}
//------------------------------------------------------------------------
void CMapContainer::AddMultiOccupantValue(POINT pt, CMapItem *pItem)
{
	COccupantTileList* pList = pItem->GetOccupantTileList();
	pt.x += pList->GetPreData().nOffAnchorX; // pt�ɷ��õ�(ê��)ƫ�Ƶ�ռλ����εĻ���(�������½�)
	pt.y += pList->GetPreData().nOffAnchorY;
	int sw = pList->GetPreData().wWidth;
	int sh = pList->GetPreData().wHeight;
	if(pList->GetCount() == 0)
	{// ����������ռλ��
		int xFrom = pt.x;
		for(int y = sh; y > 0; y--,pt.y++)
		{
			for(int x = sw; x > 0 ;x--,pt.x++)
			{
				CTile& tile = GetTile(pt);
				if(tile.IsValid()) // �˿����п��ܳ����
					tile.Set_Occupant_Item();
			}
			pt.x = xFrom;
		}
	}
	else
	{ // ����־�ֽ�����ʶռλ���������һЩ����ռλ��
		char* pL = pList->GetDataBuffer();;
		int xFrom = pt.x;
		for(int y = sh; y > 0; y--,pt.y++)
		{
			char*p = pL;
			for(int x = sw; x > 0 ;x--,p++,pt.x++)
				if(*p)
				{
					CTile& tile = GetTile(pt);
					if(tile.IsValid()) // �˿����п��ܳ����
						tile.Set_Occupant_Item_Byte(*p);
				}
			pt.x = xFrom;
			pL += sw;
		}
	}
}
//------------------------------------------------------------------------
void CMapContainer::RemoveMultiOccupantValue(POINT pt, CMapItem *pItem)
{
	COccupantTileList* pList = pItem->GetOccupantTileList();
	pt.x += pList->GetPreData().nOffAnchorX; // pt�ɷ��õ�(ê��)ƫ�Ƶ�ռλ����εĻ���(�������½�)
	pt.y += pList->GetPreData().nOffAnchorY;
	int sw = pList->GetPreData().wWidth;
	int sh = pList->GetPreData().wHeight;
	if(pList->GetCount() == 0)
	{// ����������ռλ��
		int xFrom = pt.x;
		for(int y = sh; y > 0; y--,pt.y++)
		{
			for(int x = sw; x > 0 ;x--,pt.x++)
			{
				CTile& tile = GetTile(pt);
				if(tile.IsValid()) // �˿����п��ܳ����
				{
					tile.Clear_Occupant_Item_Byte();
					tile.OnUpdateItems();
				}
			}
			pt.x = xFrom;
		}
	}
	else
	{ // ����־�ֽ�����ʶռλ���������һЩ����ռλ��
		char* pL = pList->GetDataBuffer();;
		int xFrom = pt.x;
		for(int y = sh; y > 0; y--,pt.y++)
		{
			char*p = pL;
			for(int x = sw; x > 0 ;x--,p++,pt.x++)
				if(*p)
				{
					CTile& tile = GetTile(pt);
					if(tile.IsValid()) // �˿����п��ܳ����
					{
						tile.Clear_Occupant_Item_Byte(*p);
						tile.OnUpdateItems();
					}
				}
			pt.x = xFrom;
			pL += sw;
		}
	}
}
//------------------------------------------------------------------------
// ���Խ�Ҫ��ӵĶ����ռλ���Ƿ���������е�ռλ�����ظ�
BOOL CMapContainer::TestIntersect(POINT pt, CMapItem* pItem)
{
	COccupantTileList* pList = pItem->GetOccupantTileList();
	pt.x += pList->GetPreData().nOffAnchorX; 
	pt.y += pList->GetPreData().nOffAnchorY;
	int sw = pList->GetPreData().wWidth;
	int sh = pList->GetPreData().wHeight;
	if(pList->GetCount() == 0)
	{// ����������ռλ��
		int xFrom = pt.x;
		for(int y = sh; y > 0; y--,pt.y++)
		{
			for(int x = sw; x > 0 ;x--,pt.x++)
			{
				CTile& tile = GetTile(pt);
				if(tile.IsValid() &&tile.IsBlock())
					return FALSE;
			}
			pt.x = xFrom;
		}
	}
	else
	{ // ����־�ֽ�����ʶռλ���������һЩ����ռλ��
		char* pL = pList->GetDataBuffer();;
		int xFrom = pt.x;
		for(int y = sh; y > 0; y--,pt.y++)
		{
			char*p = pL;
			for(int x = sw; x > 0 ;x--,p++,pt.x++)
				if(*p)
				{
					CTile& tile = GetTile(pt);
					if(tile.IsValid() &&tile.IsBlock())
						return FALSE;
				}
				pt.x = xFrom;
				pL += sw;
		}
	}
	return TRUE;
}
//------------------------------------------------------------------------
// ���Զ�ռλ��������ƶ���
BOOL CMapContainer::IsMovableForMultiOcc(CMapItem* pItem, POINT ptFrom, POINT ptTo)
{
	COccupantTileList* pList = pItem->GetOccupantTileList();
	POINT pt = ptFrom;
	pt.x += pList->GetPreData().nOffAnchorX; 
	pt.y += pList->GetPreData().nOffAnchorY;
	int sw = pList->GetPreData().wWidth;
	int sh = pList->GetPreData().wHeight;
	int dx = ptTo.x - ptFrom.x;
	int dy = ptTo.y - ptFrom.y;

	char* pL = pList->GetDataBuffer();
	int nOffset = sw*dy + dx;
	for (; pt.y<ptTo.y; pt.y++)
	{
		char* p = pL;
		for (; pt.x<ptTo.x; pt.x++,p++)
		{
			if (*p)
			{
				CTile& tile = GetTile(pt.x + dx, pt.y + dy);
				if (tile.IsValid() && tile.IsBlock() && *(pL+nOffset) == 0)
					return FALSE;
			}
		}
		pL += sw;
	}
	return TRUE;
}
//------------------------------------------------------------------------
// �������������ʹ֮ÿ�鶼����Ч����ϵm_rcMapTileRect֮��
// �����rc�����{tx,ty,col,row},(tx,ty)��ʾ���Ͻ�tile���꣬(col,row)�ֱ��ʾ��Ļ���������ĸ�����
// ����
//	FALSE : ���rc��m_rcMapTileRect�޽�����ȫ������Ч����
BOOL CMapContainer::AdjustTileRect(RECT &rc)
{
	if(rc.right < 0 || rc.left >= m_nMatrixWidth || rc.bottom < 0 || rc.top < 0 || rc.top >= m_nMatrixWidth)
	{
		TraceLn("Error : CMapContainer::AdjustTileRect() : invalid TileRect(%d,%d),(%d,%d)",rc.left,rc.top,rc.top,rc.bottom);
		return FALSE;
	}
	if(rc.left < 0)
	{
		rc.right +=  rc.left / 2;	// �۳�
		rc.bottom +=  rc.left / 2;	// �۳� 
		rc.left = 0;
	}
	if(rc.left + rc.right > m_nMatrixWidth)
		rc.right = 	m_nMatrixWidth - rc.left;

	if(rc.top + rc.right > m_nMatrixWidth)
		rc.right = m_nMatrixWidth - rc.top;
	if(rc.top - rc.bottom + 1 < 0)
		rc.bottom = rc.top + 1;

	// ���½�
	if(rc.left + rc.right + rc.bottom - 1 > m_nMatrixWidth)
		rc.bottom = m_nMatrixWidth - rc.left - rc.right + 1;
	
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMapContainer::SnapshotRectItem(const RECT& rc,/*in out*/int& nListCount,/*out*/SNAP_ITEM* pListBuf)
{
	if(pListBuf == NULL || nListCount < 1)
	{
		TraceLn("Error : CMapContainer::SnapshotRectItem() : invalid param!");
		return FALSE;
	}
	RECT rcArea;
	if(!IntersectRect(&rcArea,&rc,&m_rcMapTileRect))
		return FALSE;
	int nFoundCount = 0;

	int txEnd = rcArea.right;
	int tyEnd = rcArea.bottom;
	CTile* pTileLeft = &m_pTiles[rcArea.top * m_nMatrixWidth + rcArea.left];
	CTile* pTile = pTileLeft;
	for(int ty = rcArea.top; ty < tyEnd; ty++)
	{
		for(int tx = rcArea.left; tx < txEnd; tx++,pTile++)
		{
			int nCount = pTile->GetLayerCount();
			if(nCount)
			{
				for(int i = 0; i < nCount; i++)
				{
					if(nFoundCount >= nListCount)
					{
						TraceLn("Error : CMapContainer::SnapshotRectItem() : to %d items found,but %d max",nFoundCount,nListCount);
						goto END_SnapshotRectItem;
					}
					pListBuf[nFoundCount].ptTile.x = tx;
					pListBuf[nFoundCount].ptTile.y = ty;
					pListBuf[nFoundCount].pItem = pTile->GetLayer(i);
					nFoundCount++;
				}
			}
		}
		pTileLeft += m_nMatrixWidth;
		pTile = pTileLeft;
	}

END_SnapshotRectItem:
	nListCount = nFoundCount;
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMapContainer::SnapshotItemByWorldRect(const RECT &rcWorld, IN OUT int& nListCount, OUT SNAP_ITEM* pListBuf)
{
	if (pListBuf == NULL || nListCount < 1)
	{
		TraceLn("Error : CMapContainer::SnapshotItemByWorldRect() : invalid param!");
		return FALSE;
	}
	CSnapshotItemByWorldRect snap(this, rcWorld, nListCount, pListBuf);
	nListCount = snap.m_nFoundCount;
	return nListCount > 0;
}
//------------------------------------------------------------------------
BOOL CMapContainer::SnapshotTileByWorldRect(const RECT &rcWorld, IN OUT int& nListCount, OUT SNAP_TILE* pListBuf)
{
	if (pListBuf == NULL || nListCount < 1)
	{
		TraceLn("Error : CMapContainer::SnapshotTileByWorldRect() : invalid param!");
		return FALSE;
	}
	CSnapshotTileByWorldRect snap(this, rcWorld, nListCount, pListBuf);
	nListCount = snap.m_nFoundCount;
	return nListCount > 0;
}
//------------------------------------------------------------------------