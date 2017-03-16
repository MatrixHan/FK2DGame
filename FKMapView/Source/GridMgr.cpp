/**
*	created:		2012-7-1   5:13
*	filename: 		GridMgr
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
/*
 * ���ܣ���Ź��������֮ͬ�����ڹ���ľ�����ܸ���
 * ��;����̬�ر����ء���ͼ�Ͽɼ�������ɨ��
 * ������
		��һ������ֳɺܶ൥Ԫ���û��ɼ��ӿڿ��ܱ�һ����Ԫ���Ҳ���ܱ�һ����Ԫ��С
 * ���ʽ���:
		��飺  ���ӿ��ƶ�ʱ��Ԥ����һ���������ƶ�ʱ��Ҫ����Ŀ飬����˿�Ϊ���
		��ʧ�飺���ӿ��ƶ�ʱ��Ԥ����һ���������ƶ�ʱ���Ѿ�Զ����෴�ķ��������Ϊ��ʧ��
   ʵ�֣�
		���ƶ�ʱ�����µ������ֶ�ʧ�飬֪ͨ�������¼�����
		������һ���ڴ�ʱԤ����ɾ����һ�����ͼƬ(��̬�ر����)��ɨ���·��ֵĴ�����
	˵����
		1�����Ƕ���һЩ�޿飬����Щ�鱻����ʱ��������Ϊ��ͼ���кܶ�հ�����
		2����Ԫ�������ӿ�С�� ̫������̫������ͼ
		3������η��ֲ��Ըĳ�ֻ���˶������϶�Ӧ�Ŀ�����Ϊ�ࣿ�Ժ��ǰ����Ż�ʵ��
 */
//------------------------------------------------------------------------
#include "../Include/ScrollSheet.h"
#include "../Include/GridMgr.h"
//------------------------------------------------------------------------
CGridMgr::CGridMgr()
{
	m_pScrollSheetSink = NULL;
	m_ptPlayerGrid.x = m_ptPlayerGrid.y = 0;
	memset(&m_rcMapGrid, 0, sizeof(RECT));
	memset(&m_rcPlayerGrid, 0, sizeof(RECT));
	memset(&m_rcOldPreReadGrid,0,sizeof(RECT));
	memset(&m_rcPreReadGrid, 0, sizeof(RECT));
	memset(&m_rcOldViewGrid, 0, sizeof(RECT));
	memset(&m_rcViewGrid, 0, sizeof(RECT));
	m_bUseDynamicMap = FALSE;
}
//------------------------------------------------------------------------
BOOL CGridMgr::Create(IScrollSheetSink* pSink, int nMapWidth, int nMapHeight, 
					  POINT& ptPlayerPos, BOOL bUseDynamicMap)
{
	ASSERT(pSink);
	m_pScrollSheetSink = pSink;
	m_bUseDynamicMap = bUseDynamicMap;
	
	// ��ͼ�������
	m_rcMapGrid.left	= m_rcMapGrid.top = 0;
	m_rcMapGrid.right	= (nMapWidth + GRID_WIDTH - 1) >> SHIFT_WIDTH;
	m_rcMapGrid.bottom	= (nMapHeight + GRID_HEIGHT - 1) >> SHIFT_HEIGHT;

	// ���(��Ļ���ĵ�)��������
	m_ptPlayerGrid.x = ptPlayerPos.x >> SHIFT_WIDTH;
	m_ptPlayerGrid.y = ptPlayerPos.y >> SHIFT_HEIGHT;

	// Ԥ������
	if (m_bUseDynamicMap)
	{
		m_rcPreReadGrid.left	= m_ptPlayerGrid.x - (TILE_GRID_COL >> 1);
		m_rcPreReadGrid.top		= m_ptPlayerGrid.y - (TILE_GRID_ROW >> 1);
		m_rcPreReadGrid.right	= m_rcPreReadGrid.left + TILE_GRID_COL;
		m_rcPreReadGrid.bottom	= m_rcPreReadGrid.top + TILE_GRID_ROW;
		LimitRect(m_rcPreReadGrid, m_rcMapGrid);
		m_rcOldPreReadGrid = m_rcPreReadGrid;
	}
	else
	{
		m_rcOldPreReadGrid = m_rcPreReadGrid = m_rcMapGrid;
	}

	// ��������
	m_rcViewGrid.left	= m_ptPlayerGrid.x - (OBJECT_GRID_COL >> 1);
	m_rcViewGrid.top	= m_ptPlayerGrid.y - (OBJECT_GRID_ROW >> 1);
	m_rcViewGrid.right	= m_rcViewGrid.left + OBJECT_GRID_COL;
	m_rcViewGrid.bottom	= m_rcViewGrid.top + OBJECT_GRID_ROW;
	LimitRect(m_rcViewGrid, m_rcMapGrid);
	m_rcOldViewGrid = m_rcViewGrid;
	ResetPlayerRect();
	
	if (m_bUseDynamicMap)
		SetPreReadTileRect(m_rcPreReadGrid);
	SetViewTileRect(m_rcViewGrid);

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CGridMgr::SizeViewport(int nWidth, int nHeight)
{
	return TRUE;
}
//------------------------------------------------------------------------
void CGridMgr::Close()
{
	CloseViewTileRect();
	ClosePreReadTileRect();
}
//------------------------------------------------------------------------
// ǿ������Ԥ��ĳ�������и��ǿ鱻����Ϊ��
void CGridMgr::SetPreReadTileRect(RECT rc)
{
	int nDx, nDy;
	int nMoveDir = GetMoveDirection((POINT&)m_rcOldPreReadGrid, (POINT&)m_rcPreReadGrid, nDx, nDy);
	m_pScrollSheetSink->OnGridChanged(nMoveDir, nDx, nDy);
}
//------------------------------------------------------------------------
// ǿ������Ԥ��ĳ�������и��ǿ鱻����Ϊ��
void CGridMgr::ClosePreReadTileRect()
{
}
//------------------------------------------------------------------------
// ǿ�������ӿ�ĳ�������и��ǿ鱻����Ϊ��
void CGridMgr::SetViewTileRect(RECT rc)
{
	for (int y=m_rcViewGrid.top; y<m_rcViewGrid.bottom; y++)
		for (int x=m_rcViewGrid.left; x<m_rcViewGrid.right; x++)
			m_pScrollSheetSink->OnDirtyGridFound(x, y);
}
//------------------------------------------------------------------------
// ǿ�ƹر��ӿ�ĳ�������и��ǿ鱻����Ϊ��ʧ
void CGridMgr::CloseViewTileRect()
{
	for (int y=m_rcViewGrid.top; y<m_rcViewGrid.bottom; y++)
		for (int x=m_rcViewGrid.left; x<m_rcViewGrid.right; x++)
			m_pScrollSheetSink->OnDirtyGridLost(x, y);
}
//------------------------------------------------------------------------
// �����ӿ�
// ��һ������ʱĬ�����������
// bForce : ǿ�Ƹ��£���Ϊһ��û���ƶ����¿�ʱ�����¡�һ���ڵ�һ����ʾʱ��ǿ�Ƹ���
// ���أ�����й�������TRUE�����򷵻�FALSE
BOOL CGridMgr::ScrollViewport(int dx, int dy)
{
	// ���Ԥ������������Ƿ�ı�
	BOOL bPreReadGridChanged = FALSE;
	RECT rcNewPreReadGrid;
	if (m_bUseDynamicMap)
	{
		rcNewPreReadGrid = m_rcPreReadGrid;
		OffsetRect(&rcNewPreReadGrid, dx, dy);
		LimitRect(rcNewPreReadGrid, m_rcMapGrid);
		bPreReadGridChanged = !EqualRect(&rcNewPreReadGrid, &m_rcPreReadGrid);
	}

	// �����ͼ��������Ƿ�ı�
	BOOL bViewGridChanged = FALSE;
	RECT rcNewViewGrid;
	rcNewViewGrid = m_rcViewGrid;
	OffsetRect(&rcNewViewGrid, dx, dy);
	LimitRect(rcNewViewGrid, m_rcMapGrid);
	bViewGridChanged = !EqualRect(&rcNewViewGrid, &m_rcViewGrid);
	
	// �����ɵ����(������Ե�ͼ�ϵĶ���)
	if (bViewGridChanged)
	{
		POINT pt;
		for (int y=m_rcViewGrid.top; y<m_rcViewGrid.bottom; y++)
		{
			for (int x=m_rcViewGrid.left; x<m_rcViewGrid.right; x++)
			{
				pt.x = x, pt.y = y;
				if (!PtInRect(&rcNewViewGrid, pt))
					m_pScrollSheetSink->OnDirtyGridLost(x, y);
			}
		}
	
		m_rcOldViewGrid = m_rcViewGrid;
		m_rcViewGrid = rcNewViewGrid;
		ResetPlayerRect();
	}

	// Ԥ����������иı�
	if (m_bUseDynamicMap && bPreReadGridChanged) // ������Ե�ͼ�ϵĵر���Ϣ
	{
		m_rcOldPreReadGrid = m_rcPreReadGrid;
		m_rcPreReadGrid = rcNewPreReadGrid;
		int nDx, nDy;
		int nMoveDirection = GetMoveDirection((POINT&)m_rcOldPreReadGrid, (POINT&)m_rcPreReadGrid, nDx, nDy);
		m_pScrollSheetSink->OnGridChanged(nMoveDirection, nDx, nDy);
	}

	// �����µ����(������Ե�ͼ�ϵĶ���)
	if (bViewGridChanged)
	{
		POINT pt;
		for (int y=m_rcViewGrid.top; y<m_rcViewGrid.bottom; y++)
		{ 
			for (int x=m_rcViewGrid.left; x<m_rcViewGrid.right; x++)
			{
				pt.x = x, pt.y = y;
				if (!PtInRect(&m_rcOldViewGrid, pt))
					m_pScrollSheetSink->OnDirtyGridFound(x, y);
			}
		}

	}
	return TRUE;
}
//------------------------------------------------------------------------
// �ɼ���ͼ�ƶ���Ļʱ��ʹԤ����ͼ�ѿɼ���ͼ��Χ�����м�
// �㷨�����Ǿ����ܱ�֤��ͼ�ӿ���Ԥ���ӿڵ��е��Ƕ����
// ���أ��й�������TRUE
BOOL CGridMgr::ScrollToCenter(POINT& ptPlayerPos)
{
	// ��Ļ���ĵ���������
	POINT ptPlayerGridNew;
	ptPlayerGridNew.x = ptPlayerPos.x >> SHIFT_WIDTH;
	ptPlayerGridNew.y = ptPlayerPos.y >> SHIFT_HEIGHT;
	if (m_ptPlayerGrid.x == ptPlayerGridNew.x && m_ptPlayerGrid.y == ptPlayerGridNew.y)
		return FALSE;
	
	if (PtInRect(&m_rcPlayerGrid, ptPlayerGridNew))
	{
		m_ptPlayerGrid = ptPlayerGridNew;
		return FALSE;
	}

	int dx, dy;
	if (m_rcPlayerGrid.right - m_rcPlayerGrid.left != 1 ||
		m_rcPlayerGrid.bottom - m_rcPlayerGrid.top != 1) // ˵�����û��վ�ڶ�̬���������������
	{
		dx = ptPlayerGridNew.x - ((m_rcViewGrid.right + m_rcViewGrid.left) >> 1);
		dy = ptPlayerGridNew.y - ((m_rcViewGrid.bottom + m_rcViewGrid.top) >> 1);
	}
	else
	{
		dx = ptPlayerGridNew.x - m_ptPlayerGrid.x;
		dy = ptPlayerGridNew.y - m_ptPlayerGrid.y;
	}
	m_ptPlayerGrid = ptPlayerGridNew;
	return ScrollViewport(dx, dy);
}
//------------------------------------------------------------------------
// �����������ڵ��������
void CGridMgr::GetGridRectBelowViewport(RECT& rcCoveredRect)
{
	rcCoveredRect.left   = m_rcViewGrid.left   << SHIFT_WIDTH;
	rcCoveredRect.top	 = m_rcViewGrid.top    << SHIFT_HEIGHT;
	rcCoveredRect.right  = m_rcViewGrid.right  << SHIFT_WIDTH;
	rcCoveredRect.bottom = m_rcViewGrid.bottom << SHIFT_HEIGHT;
	// ����Ҫ���ͼ�����������ཻ������ΪԤ����������Գ���
}
//------------------------------------------------------------------------
// Ԥ���������ڵ��������
void CGridMgr::GetGridRectBelowPreRead(RECT& rcCoveredRect)
{
	rcCoveredRect.left   = m_rcPreReadGrid.left   << SHIFT_WIDTH;
	rcCoveredRect.top	 = m_rcPreReadGrid.top    << SHIFT_HEIGHT;
	rcCoveredRect.right  = m_rcPreReadGrid.right  << SHIFT_WIDTH;
	rcCoveredRect.bottom = m_rcPreReadGrid.bottom << SHIFT_HEIGHT;
	// ����Ҫ���ͼ�����������ཻ������ΪԤ����������Գ���
}
//------------------------------------------------------------------------
// ����Ԥ����������ϵ����������
int CGridMgr::GetMoveDirection(POINT& ptFrom, POINT& ptTo, int& nDx, int& nDy)
{
	nDx = ptTo.x - ptFrom.x;
	nDy = ptTo.y - ptFrom.y;
	if (nDx > TILE_GRID_COL || nDx < -TILE_GRID_COL || nDy > TILE_GRID_ROW || nDy < -TILE_GRID_ROW)
		return mdAll;// ��ʾԤ������ı�ǰ�������Ԥ�������ཻ
	if (nDx > 0)
	{
		if (nDy > 0)		return mdEastSouth;	// ����
		else if (nDy < 0)	return mdEastNorth;	// ����
		else				return mdEast;		// ��
	}
	else if (nDx < 0)
	{
		if (nDy > 0)		return mdWestSouth;	// ����
		else if (nDy < 0)	return mdWestNorth;	// ����
		else				return mdWest;		// ��
	}
	else
	{
		if (nDy > 0)		return mdSouth;	// ��
		else if (nDy < 0)	return mdNorth;	// ��
	}
	return mdAll;
}
//------------------------------------------------------------------------
void CGridMgr::SnapshotLeftTop(int& nRows, int& nCols, BLOCK_INFO* pBlockInfo, int nMoveDirection)
{
	RECT rcUnion;
	if (nMoveDirection == mdAll)
		rcUnion = m_rcPreReadGrid;
	else
		UnionRect(&rcUnion, &m_rcOldPreReadGrid, &m_rcPreReadGrid);
	POINT ptLeftTop;
	ptLeftTop.x  = rcUnion.left << SHIFT_WIDTH;
	ptLeftTop.y	 = rcUnion.top << SHIFT_HEIGHT;
	nRows = rcUnion.bottom - rcUnion.top;
	nCols = rcUnion.right - rcUnion.left;
	
	int x, y = ptLeftTop.y;
	int nIdx = rcUnion.top * m_rcMapGrid.right + rcUnion.left;
	for (int row=0,i=0; row<nRows; row++,y+=GRID_HEIGHT,nIdx+=m_rcMapGrid.right)
	{
		x = ptLeftTop.x;
		for (int col=0; col<nCols; col++,x+=GRID_WIDTH,i++)
		{
			pBlockInfo[i].ptLeftTop.x = x;
			pBlockInfo[i].ptLeftTop.y = y;
			pBlockInfo[i].nIndex = nIdx + col;
		}
	}
}
//------------------------------------------------------------------------
int CGridMgr::GetBlockIndex(POINT& ptLeftTop)
{
	int nGridX = ptLeftTop.x >> SHIFT_WIDTH;
	int nGridY = ptLeftTop.y >> SHIFT_HEIGHT;
	return nGridY * m_rcMapGrid.right + nGridX;
}
//------------------------------------------------------------------------
// ���뱣֤rcSmall�ĳߴ��rcBig��С��
void CGridMgr::LimitRect(RECT& rcSmall, const RECT& rcBig)
{
	if (rcSmall.left < rcBig.left)
	{
		rcSmall.right += rcBig.left - rcSmall.left;
		rcSmall.left = rcBig.left;
		
		if (rcSmall.right > rcBig.right)
			rcSmall.right = rcBig.right;
	}
	if (rcSmall.top < rcBig.top)
	{
		rcSmall.bottom += rcBig.top - rcSmall.top;
		rcSmall.top = rcBig.top;

		if (rcSmall.bottom > rcBig.bottom)
			rcSmall.bottom = rcBig.bottom;
	}
	if (rcSmall.right > rcBig.right)
	{
		rcSmall.left -= rcSmall.right - rcBig.right;
		rcSmall.right = rcBig.right;
		
		if (rcSmall.left < rcBig.left)
			rcSmall.left = rcBig.left;
	}
	if (rcSmall.bottom > rcBig.bottom)
	{
		rcSmall.top -= rcSmall.bottom - rcBig.bottom;
		rcSmall.bottom = rcBig.bottom;
		
		if (rcSmall.top < rcBig.top)
			rcSmall.top = rcBig.top;
	}
}
//------------------------------------------------------------------------
// �����������κ͵�ͼ����ȷ����һ�ľ��η�Χ
void CGridMgr::ResetPlayerRect()
{
	int dx = 0, dy = 0;
	if (m_rcViewGrid.left <= m_rcMapGrid.left)		// ��߽����
		dx = - ((m_rcViewGrid.left + m_rcViewGrid.right) >> 1);
	if (m_rcViewGrid.top <= m_rcMapGrid.top)		// �ϱ߽����
		dy = - ((m_rcViewGrid.top + m_rcViewGrid.bottom) >> 1);
	if (m_rcViewGrid.right >= m_rcMapGrid.right)	// �ұ߽����
		dx = ((m_rcViewGrid.right - m_rcViewGrid.left) >> 1);
	if (m_rcViewGrid.bottom >= m_rcMapGrid.bottom)	// �±߽����
		dy = ((m_rcViewGrid.bottom - m_rcViewGrid.top) >> 1);

	m_rcPlayerGrid.left = (m_rcViewGrid.left + m_rcViewGrid.right) >> 1;
	m_rcPlayerGrid.top = (m_rcViewGrid.top + m_rcViewGrid.bottom) >> 1;
	m_rcPlayerGrid.right = m_rcPlayerGrid.left + 1;
	m_rcPlayerGrid.bottom = m_rcPlayerGrid.top + 1;
	if (dx < 0)
		m_rcPlayerGrid.left += dx;
	if (dx > 0)
		m_rcPlayerGrid.right += dx;
	if (dy < 0)
		m_rcPlayerGrid.top += dy;
	if (dy > 0)
		m_rcPlayerGrid.bottom += dy;
}
//------------------------------------------------------------------------