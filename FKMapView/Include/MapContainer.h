/**
*	created:		2012-7-1   2:38
*	filename: 		MapContainer
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/MapInfo/MapItem.h"
#include "../../FKGame/MapInfo/Tile.h"
#include "../Include/MapCoordinate.h"
#include "../Include/Geometry.h"
#include "../Include/FindPath_R.h"
#include <list>
using std::list;
//------------------------------------------------------------------------
/*
#ifdef _USE_QUICK_SPEED
// ��ͼ���桢����ʱ��������С�趨��(Ϊ�˱���˳����㣬����ѭ������趨��
// ����64(32)�ı���������2����������)
	#ifndef TILES_GRID_SHIFT
		#define TILES_GRID_SHIFT		10	// (>=6)
	#endif
	#define TILES_GRID_WIDTH			(64*(2^(TILES_GRID_SHIFT-6)))	// = 64*2^4 = 64*16 = 1024
	#define TILES_GRID_HEIGHT			(32*(2^(TILES_GRID_SHIFT-6)))	// = 32*2^4 = 32*16 = 512

	#ifndef TILES_PREREAD_SHIFT
		#define TILES_PREREAD_SHIFT		3
	#endif
	#define PREREAD_GRID_ROW			(2^TILES_PREREAD_SHIFT)		// 8X8 GRID BLOCK
	#define PREREAD_GRID_COL			(2^TILES_PREREAD_SHIFT)
#else
	#ifndef TILES_GRID_WIDTH
		#define TILES_GRID_WIDTH		1024//(64*10)
	#endif
	#ifndef TILES_GRID_HEIGHT
		#define TILES_GRID_HEIGHT		512//(32*10)
	#endif
	#ifndef TILES_PREREAD_GRID_ROW
		#define TILES_PREREAD_GRID_ROW	12
	#endif
	#ifndef TILES_PREREAD_GRID_COL
		#define TILES_PREREAD_GRID_COL	12
	#endif
	#ifndef LIST_PREREAD_GRID_ROW
		#define LIST_PREREAD_GRID_ROW	15 // ��֤��TILES_PREREAD_GRID_ROW�Ĳ���ż��������Ϊ2����
	#endif
	#ifndef LIST_PREREAD_GRID_COL
		#define LIST_PREREAD_GRID_COL	15
	#endif
#endif
#define TILES_PREREAD_WIDTH				TILES_GRID_WIDTH*TILES_PREREAD_GRID_COL
#define TILES_PREREAD_HEIGHT			TILES_GRID_HEIGHT*TILES_PREREAD_GRID_ROW
#define LIST_PREREAD_WIDTH				LIST_GRID_WIDTH*LIST_PREREAD_GRID_COL
#define LIST_PREREAD_HEIGHT				LIST_GRID_HEIGHT*LIST_PREREAD_GRID_ROW
#define OBJECT_GRID_WIDTH				TILES_GRID_WIDTH
#define OBJECT_GRID_HEIGHT				TILES_GRID_HEIGHT
#define LIST_GRID_WIDTH					TILES_GRID_WIDTH
#define LIST_GRID_HEIGHT				TILES_GRID_HEIGHT
#ifdef _USE
	#define OBJECT_PREREAD_WIDTH			OBJECT_GRID_WIDTH*4
	#define OBJECT_PREREAD_HEIGHT			OBJECT_GRID_HEIGHT*4
#else
	#define OBJECT_PREREAD_WIDTH			OBJECT_GRID_WIDTH*12
	#define OBJECT_PREREAD_HEIGHT			OBJECT_GRID_HEIGHT*12
#endif*/
//------------------------------------------------------------------------
#define SHIFT_WIDTH			11
#define SHIFT_HEIGHT		10
#define GRID_WIDTH			2048
#define GRID_HEIGHT			1024
#define TILE_GRID_ROW		5
#define TILE_GRID_COL		5
#define OBJECT_GRID_ROW		5
#define OBJECT_GRID_COL		5
#define LIST_GRID_ROW		15
#define LIST_GRID_COL		15
//------------------------------------------------------------------------
#define TILES_PREREAD_WIDTH		GRID_WIDTH*TILE_GRID_COL
#define TILES_PREREAD_HEIGHT	GRID_HEIGHT*TILE_GRID_ROW
//------------------------------------------------------------------------
#ifndef PixelCood2GridCood
#define PixelCood2GridCood(PixelPos, GridLength)	((PixelPos + GridLength - 1) / GridLength)
#endif
//------------------------------------------------------------------------
enum DynamicMapUpdateType{
	mdNorth=0,		// ��̬��ͼ�������ƶ�һ��
	mdEastNorth,
	mdEast,
	mdEastSouth,
	mdSouth,
	mdWestSouth,
	mdWest,
	mdWestNorth,
	mdAll,			// ����̬��ͼ�ƶ��Ĳ�������1(��/��)ʱ�����������

	mdMaxUpdateType
};
//------------------------------------------------------------------------
struct IItemClassFactory;
//------------------------------------------------------------------------
struct SNAP_ITEM
{
	POINT		ptTile;
	CMapItem*	pItem;
};
//------------------------------------------------------------------------
struct SNAP_TILE
{
	POINT		ptTile;
	CTile*		pTile;
};
//------------------------------------------------------------------------
struct BLOCK_INFO
{
	POINT	ptLeftTop;	// ������Ͻ�TILE�����ĵ���������
	int		nIndex;		// ������
};
//------------------------------------------------------------------------
#define MAX_SNAP_ITEM_COUNT		4096
// vcĬ�϶�ջ��С����Ϊ1M������˾ֲ�����������ֵ�����һ������
#define MAX_SNAP_BUF_SIZE		(MAX_SNAP_ITEM_COUNT * sizeof(SNAP_ITEM))
#define MAP_ARCHIVE_VERSION		0x00010102
//------------------------------------------------------------------------
struct MAP_ARCHIVE_HEADER
{
	DWORD		dwSize;				// ��MAP_ARCHIVE_HEADER��ʼ��ַ��ʼ���ݵ��ܳ���
	DWORD 		dwVersion;			// �汾��Ϣ
	int			nMapWidth;			// ���ؿ��
	int			nMapHeight;			// ���ظ߶�
	char		szGroundJpgDir[16];	// �ر�jpg�ļ����ڵ�Ŀ¼������"�ݵ�"
	int			nGridWidth;			// ������
	int			nGridHeight;		// ����߶�
	BOOL		bSaveItemOcc;		// �Ƿ񱣴�����ռλ��Ϣ
	char		szFill[100];		// �����Ϣ

	BOOL IsValid()
	{
		return dwVersion <= MAP_ARCHIVE_VERSION && nMapWidth > 0 && nMapHeight > 0 && dwSize > sizeof(MAP_ARCHIVE_HEADER);
	}
	void Init(int w,int h,LPCSTR szGroundJpgDir)
	{
		memset(this,0,sizeof(MAP_ARCHIVE_HEADER));
		dwVersion = MAP_ARCHIVE_VERSION;
		nMapWidth = w;
		nMapHeight = h;
		lstrcpyn(this->szGroundJpgDir,szGroundJpgDir,sizeof(this->szGroundJpgDir)); // ע�⣺�п���ռ��szFile[0]
		dwSize = sizeof(MAP_ARCHIVE_HEADER);
	}
};
//------------------------------------------------------------------------
// ��ͼ�ϵ�һ��
struct MAP_ARHCIVE_LINE_HEADER
{
	int		nFromX;			// ��ʼ��ƫ��
	int		nTileCount;		// ������
};
//------------------------------------------------------------------------
struct ITrace;
//------------------------------------------------------------------------
typedef list<CMapItem* >  MAP_ITEM_LIST;
//------------------------------------------------------------------------
class CMapContainer : public CPathFinder_R<CMapContainer>
{
private:
	POINT	m_ptTileOrigin;		// ���������ԭ����������ͼ�е�ƫ��
	int		m_nMatrixWidth;		// ��ͼTILE��ȣ�����϶�����ͬ��(������row + col)
	int		m_nMatrixHeight;
	RECT	m_rcMapTileRect;	// map�����鷶Χ
	CTile*	m_pTiles;			// TILE����
	DWORD*	m_pMapTable;		// ��ͼ�ĵ���ӳ���
	int		m_nMapWidth;		// ��ͼ���ؿ��
	int		m_nMapHeight;		// ��ͼ���ظ߶�
	RECT	m_rcMap;			// ����map���ش�С
	CMapCoordinate m_MapCo;		// ���������������֮��ļ���
	BOOL	m_bUseDynamicMap;	// �Ƿ�ʹ�ö�̬��ͼ����������
	IArchive* m_pArchive;
	IItemClassFactory* m_pItemCF;
	CMultLenArrayCache<DWORD,CMapItem*> m_LayerListCache;
	DWORD* m_pMultiValueTable;	// �˷�����GetTileʱ����˷�
	MAP_ITEM_LIST m_MapItemList; // ���ڼ�¼��ͼ��Դ

	typedef void (CMapContainer::*ProcScrollTilesInfo)(BLOCK_INFO* pBlockInfo, int nRows, int nCols, int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy);
	static ProcScrollTilesInfo m_fnScrollTileInfo[mdMaxUpdateType];
public:
	char	m_szGround[256];
	
private:
	static CTile m_EmptyTile;	// ����������Ч������ʱ�����ش�TILE
public:
	BOOL IsBlock(POINTS ptPos) // �ṩ��Ѱ·ģ��
	{
		CTile& tile = GetTile(ptPos.x, ptPos.y);
		return (!tile.IsValid() || tile.IsBlock());
	}
	BOOL IsMovableForMultiOcc(CMapItem* pItem, POINT ptFrom, POINT ptTo);	
	void SetUseDynamicMap(BOOL bUse)	{m_bUseDynamicMap = bUse;}
	BOOL IsUseDynamicMap()				{return m_bUseDynamicMap;}
	BOOL OnReadServerMap(IArchive* pArchive,ITrace* pTrace);
	BOOL OnReadServerMap112(IArchive* pArchive,ITrace* pTrace);
	int GetMapWidth()const		{return m_nMapWidth;}
	int GetMapHeight()const		{return m_nMapHeight;}
	void _MovePreReadRect_Top(BLOCK_INFO* pBlockInfo, int nRows, int nCols, int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy);
	void _MovePreReadRect_RightTop(BLOCK_INFO* pBlockInfo, int nRows, int nCols, int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy);
	void _MovePreReadRect_Right(BLOCK_INFO* pBlockInfo, int nRows, int nCols, int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy);
	void _MovePreReadRect_RightDown(BLOCK_INFO* pBlockInfo, int nRows, int nCols, int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy);
	void _MovePreReadRect_Down(BLOCK_INFO* pBlockInfo, int nRows, int nCols, int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy);
	void _MovePreReadRect_LeftDown(BLOCK_INFO* pBlockInfo, int nRows, int nCols, int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy);
	void _MovePreReadRect_Left(BLOCK_INFO* pBlockInfo, int nRows, int nCols, int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy);
	void _MovePreReadRect_LeftTop(BLOCK_INFO* pBlockInfo, int nRows, int nCols, int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy);
	void _MovePreReadRect_All(BLOCK_INFO* pBlockInfo, int nRows, int nCols, int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy);
	void ScrollTilesInfo(BLOCK_INFO* pBlockInfo, int nRows, int nCols, int nTileRow, int nTileCol, int nDirection, POINT& ptNewTileOrigin, int dx, int dy);
	void InitBlockInfo(POINT& ptLeftTop, int nTileRow, int nTileCol);
	void LoadBlockInfo(POINT& ptLeftTop, int nIndex, int nTileRow, int nTileCol);
	void CopyBlockInfo(POINT& ptSrcLeftTop, POINT& ptDstLeftTop, int nTileRow, int nTileCol);
	void CopyBlockInfoForSurface(POINT& ptSrcLeftTop, POINT& ptDstLeftTop, int nTileRow, int nTileCol);
	void SetOriginTile(POINT& ptTileOrigin);
	POINT GetOriginTile()		{return m_ptTileOrigin;}

	// �Ƿ��Ѿ���ʼ��
	BOOL IsValid()
	{
		return m_pTiles != NULL;
	}

	// �������������ʹ֮ÿ�鶼����Ч����ϵm_rcMap֮��
	// �����rc�����{tx,ty,col,row},(tx,ty)��ʾ���Ͻ�tile���꣬(col,row)�ֱ��ʾ��Ļ���������ĸ�����
	// ����
	//	FALSE : ���rc��m_rcMap�޽�����ȫ������Ч����
	BOOL AdjustTileRect(RECT &rc);

	// ���Ƿ��ڵ�ͼ������
	BOOL IsTileInMap(int tx,int ty) const
	{
		POINT ptOff = {tx - m_ptTileOrigin.x, ty - m_ptTileOrigin.y};
		return (ptOff.x >= 0 && ptOff.x < m_nMatrixWidth && ptOff.y >= 0 && ptOff.y < m_nMatrixHeight);
	}

	// ���Ƿ��ڵ�ͼ������
	BOOL IsTileInMap(const POINT& ptTile) const
	{
		POINT ptOff = {ptTile.x - m_ptTileOrigin.x, ptTile.y - m_ptTileOrigin.y};
		return (ptOff.x >= 0 && ptOff.x < m_nMatrixWidth && ptOff.y >= 0 && ptOff.y < m_nMatrixHeight);
	}

	// �Ƿ�Ϊ��Ч�飬��������ȷ�ҿ�Ҳ����Ч��
	BOOL IsValidTile(const POINT& ptTile)
	{
		POINT ptOff = {ptTile.x - m_ptTileOrigin.x, ptTile.y - m_ptTileOrigin.y};
		if (ptOff.x >= 0 && ptOff.x < m_nMatrixWidth && ptOff.y >= 0 && ptOff.y < m_nMatrixHeight)
			return m_pTiles[m_pMultiValueTable[ptOff.y] + ptOff.x].IsValid();
		else
			return FALSE;
	}

	// �Ƿ�Ϊ��Ч�飬��������ȷ�ҿ�Ҳ����Ч��
	BOOL IsValidTile(int tx,int ty)
	{
		POINT ptOff = {tx - m_ptTileOrigin.x, ty - m_ptTileOrigin.y};
		if (ptOff.x >= 0 && ptOff.x < m_nMatrixWidth && ptOff.y >= 0 && ptOff.y < m_nMatrixHeight)
			return m_pTiles[m_pMultiValueTable[ptOff.y] + ptOff.x].IsValid();
		else
			return FALSE;
	}

	CTile& GetTile(const POINT& ptTile) const
	{
		POINT ptOff = {ptTile.x - m_ptTileOrigin.x, ptTile.y - m_ptTileOrigin.y};
		if (ptOff.x >= 0 && ptOff.x < m_nMatrixWidth && ptOff.y >= 0 && ptOff.y < m_nMatrixHeight)
			return m_pTiles[m_pMultiValueTable[ptOff.y] + ptOff.x];
		else
			return m_EmptyTile;
	}

	CTile& GetTile(int tx, int ty) const
	{
		POINT ptOff = {tx - m_ptTileOrigin.x, ty - m_ptTileOrigin.y};
		if (ptOff.x >= 0 && ptOff.x < m_nMatrixWidth && ptOff.y >= 0 && ptOff.y < m_nMatrixHeight)
			return m_pTiles[m_pMultiValueTable[ptOff.y] + ptOff.x];
		else
			return m_EmptyTile;
	}

	// ����������Ч�Լ�飬����ͬGetTile()
	inline CTile& _GetTile(const POINT& ptTile) const
	{
		//return GetTile(ptTile);
		POINT ptOff = {ptTile.x - m_ptTileOrigin.x, ptTile.y - m_ptTileOrigin.y};
		ASSERT(ptOff.x >= 0 && ptOff.x < m_nMatrixWidth && ptOff.y >= 0 && ptOff.y < m_nMatrixHeight);
		return m_pTiles[m_pMultiValueTable[ptOff.y] + ptOff.x];
	}

	inline CTile& _GetTile(int tx, int ty) const
	{
		//return GetTile(tx, ty);
		POINT ptOff = {tx - m_ptTileOrigin.x, ty - m_ptTileOrigin.y};
		ASSERT(ptOff.x >= 0 && ptOff.x < m_nMatrixWidth && ptOff.y >= 0 && ptOff.y < m_nMatrixHeight);
		return m_pTiles[m_pMultiValueTable[ptOff.y] + ptOff.x];
	}

	CTile* GetTilePtr(const POINT& ptTile) const
	{
		POINT ptOff = {ptTile.x - m_ptTileOrigin.x, ptTile.y - m_ptTileOrigin.y};
		if (ptOff.x >= 0 && ptOff.x < m_nMatrixWidth && ptOff.y >= 0 && ptOff.y < m_nMatrixHeight)
		{
			CTile* pTile = &m_pTiles[m_pMultiValueTable[ptOff.y] + ptOff.x];
			if (pTile->IsValid())
				return pTile;
		}
		return NULL;
	}

	// �ṩһ���ֶ�ֱ�ӷ���map��ά����
	inline CTile* GetMapArray()						{return m_pTiles;}
	int GetMapArrayWidth() const					{return m_nMatrixWidth;}
	int GetMapArrayHeight() const					{return m_nMatrixHeight;}
	const CMapCoordinate& GetMapCoordinary() const	{return m_MapCo;}

	// ����һ���µĿյ�map
	// nMapnWidth��nMapHeightΪ��ͼ���ش�С
	BOOL Create(int nMapWidth, int nMapHeight, RECT& rcPreRead, ITrace* pTrace);
	void Close();

	// ����ǰ��ͼ�����ֳ��������
	// pItemCF���ڴ�������
	BOOL OnSaveMap(IArchive* pArchive,LPCSTR szGroundJpgDir,ITrace* pTrace = NULL);
	BOOL OnLoadMap(IArchive* pArchive,IItemClassFactory* pItemCF, LPRECT lprcViewport, ITrace* pTrace = NULL);

	virtual BOOL AddItem(const POINT& ptTile, CMapItem* pItem, DWORD dwParam=0);
	virtual BOOL MoveItem(const POINT& ptFrom, const POINT& ptTo, CMapItem* pItem, DWORD dwParam=0);
	virtual BOOL RemoveItem(const POINT& ptTile, CMapItem* pItem, DWORD dwParam=0);

	virtual BOOL AddItemOccupant(const POINT& ptTile, CMapItem* pItem);
	virtual BOOL RemoveItemOccupant(const POINT& ptTile, CMapItem* pItem);

	// ����һ����Ҫ��������Ķ���
	BOOL AddLayerItem(const POINT &pt, CMapItem *pItem);

	/* ˵������Χ�㲥(���ڿ��յĹ㲥)
			����ʽ�㲥���Խ�����㲥�����У���Χ�ڵ��ֳ��Ѿ������ı䣬��ʱ���ܵ������鳤�ȱ仯��
			���շ�ʽ�㲥�ʺ��ڹ㲥��Щ���ܵ��¶���״̬�仯����Ϣ
			����ȡ�����б��Ժ������㲥��Ϣʱ����Щ���������ĳЩ��Ϣ����ʧ��
			�����Ҫ���˶����Ƿ���Ч����鷽����pItem�Ƿ��ڴ�tile��
	*/
	/*
	  ���ܣ����߼������ڹ㲥���������Ҵ��ϵ��µ�˳�򡣹㷺���ڷ�Χ�㲥���繥��������㲥��
	  ������
		���룺
			rc			: tile����ϵ�µľ���
			nListCount	: pListBuf�����ά��
		�����
			pListBuf	: �������ж���
			nListCount	: ���� pListBuf ����Ч�Ķ�������
	 */
	BOOL SnapshotRectItem(const RECT& rc,/*in out*/int& nListCount,/*out*/SNAP_ITEM* pListBuf);

	/*
	  ���ܣ�ȡ����������ϵ�µ�һ�����η�Χ�����ж���Ŀ��գ��������Ҵ��ϵ��µ�˳��
	  ������
		���룺
			rcWorld		: ��Ϸ������������
			nListCount	: pListBuf�����ά��
		�����
			pListBuf	: �������ж���
			nListCount	: ���� pListBuf ����Ч�Ķ�������
	 */

	// ����ͬEnumItemByWorldRect()
	BOOL SnapshotItemByWorldRect(const RECT &rcWorld, /*in out*/int& nListCount,/*out*/SNAP_ITEM* pListBuf);

	/*
	  ���ܣ�ȡ����������ϵ�µ�һ�����η�Χ������tile�Ŀ��� 
	  ������
		���룺
			rcWorld		: ��Ϸ������������
			nListCount	: pListBuf�����ά��
		�����
			pListBuf	: ��������tile
			nListCount	: ���� pListBuf ����Ч�Ķ�������
	 */
	// ����ͬEnumTileByWorldRect()
	BOOL SnapshotTileByWorldRect(const RECT &rcWorld, /*in out*/int& nListCount,/*out*/SNAP_TILE* pListBuf);

	CMapContainer();
	virtual ~CMapContainer();

	BOOL OnLoadMap112(IArchive* pArchive,IItemClassFactory* pItemCF,LPRECT lprcViewport, ITrace* pTrace=NULL);
	BOOL OnAddMap112(IArchive* pArchive,IItemClassFactory* pItemCF,LPRECT lprcViewport, ITrace* pTrace=NULL);
	BOOL OnSaveMap112(IArchive* pArchive,LPCSTR szGroundJpgDir, int nGridWidth, int nGridHeight, ITrace* pTrace=NULL);
	void ViewRectToPreReadRect(RECT& rcView, RECT& rcPreRead, int nMapWidth, int nMapHeight);
	
private:
	BOOL _LoadBlock(IArchive* pArchive, POINT ptTileLeftTop, int nTileRow, int nTileCol, IItemClassFactory* pItemCF);
	BOOL _LoadTileInfo(IArchive* pArchive, CTile* pTile, POINT& ptTile, IItemClassFactory* pItemCF);
	BOOL _SaveBlock(IArchive* pArchive, POINT ptTileLeftTop, int nTileRow, int nTileCol);
	BOOL _SaveMultiEmptyTileInfo(IArchive* pArchive, int nCount);
	BOOL _SaveMultiTileInfo(IArchive* pArchive, POINT* ptTileList, int nCount);
	BOOL _SaveTileInfo(IArchive* pArchive, CTile* pTile);
	void AddMultiOccupantValue(POINT pt, CMapItem *pItem);
	void RemoveMultiOccupantValue(POINT pt, CMapItem *pItem);
	BOOL TestIntersect(POINT pt, CMapItem* pItem);
	
protected:
	BOOL CreateMapArray(RECT& rcPreRead);
};
//------------------------------------------------------------------------