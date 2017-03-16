/**
*	created:		2012-7-1   1:32
*	filename: 		ScrollSheet
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
//------------------------------------------------------------------------
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------
BOOL CScrollSheet::Create(IScrollSheetSink* pSink,
						  int nMapWidth,int nMapHeight,
						  int nGridWidth,int nGridHeight,
						  LPRECT lprcViewport)
{
	m_pScrollSheetSink = pSink,
	m_nMapWidth = nMapWidth;
	m_nMapHeight = nMapHeight;
	SetRect(&m_rcMap,0,0,nMapWidth,nMapHeight);

	m_nViewWidth = lprcViewport->right - lprcViewport->left;
	m_nViewHeight = lprcViewport->bottom - lprcViewport->top;
	m_nViewTopX = lprcViewport->left;
	m_nViewTopY = lprcViewport->top;
	SetRect(&m_rcViewportRect,m_nViewTopX,m_nViewTopY,m_nViewTopX + m_nViewWidth,m_nViewTopY + m_nViewHeight);

	m_nGridWidth = nGridWidth;
	m_nGridHeight = nGridHeight;
	SetRect(&m_rcMapGrid,0,0,(m_nMapWidth + nGridWidth - 1) / nGridWidth,(m_nMapHeight + nGridHeight - 1) / nGridHeight);

	RECT rc;
	SetRect(&rc, m_nViewTopX / nGridWidth, m_nViewTopY / nGridHeight,
		(m_nViewTopX + m_nViewWidth - 1) / nGridWidth + 1, (m_nViewTopY + m_nViewHeight - 1) / nGridHeight + 1);
	SetViewTileRect(rc);
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CScrollSheet::SizeViewport(int nWidth, int nHeight)
{
	if(nWidth == 0 || nHeight == 0)
		return FALSE;
	m_nViewWidth = nWidth;
	m_nViewHeight = nHeight;

	SetRect(&m_rcCurViewGrid,m_nViewTopX / m_nGridWidth,m_nViewTopY / m_nGridHeight,
		(m_nViewTopX + m_nViewWidth - 1) / m_nGridWidth + 1,
		(m_nViewTopY + m_nViewHeight - 1) / m_nGridHeight + 1);

	// �����µ����?
	//.....
	return TRUE;
}
//------------------------------------------------------------------------
void CScrollSheet::Close()
{

}
//------------------------------------------------------------------------
BOOL CScrollSheet::MoveViewportTo(int x, int y)
{
	ScrollViewport(x - m_nViewTopX,y - m_nViewTopY);
	return TRUE;
}
//------------------------------------------------------------------------
// ǿ�������ӿ�ĳ�������и��ǿ鱻����Ϊ��
void CScrollSheet::SetViewTileRect(RECT rc)
{
	IntersectRect(&rc,&rc,&m_rcMapGrid);
	m_rcCurViewGrid = rc;
	InflateRect(&rc,1,1);
	IntersectRect(&rc,&rc,&m_rcMapGrid);
	for(int y = rc.top; y < rc.bottom; y++)
		for(int x = rc.left; x < rc.right; x++)
			m_pScrollSheetSink->OnDirtyGridFound(x,y);

	m_rcLastDirty = rc;
}
//------------------------------------------------------------------------
// �����ӿ�
// ��һ������ʱĬ�����������
// bForce : ǿ�Ƹ��£���Ϊһ��û���ƶ����¿�ʱ�����¡�һ���ڵ�һ����ʾʱ��ǿ�Ƹ���
// ���أ�����й�������TRUE�����򷵻�FALSE
BOOL CScrollSheet::ScrollViewport(int dx, int dy)
{
	if(m_nViewTopX + dx < 0)
		dx = -m_nViewTopX;
	if(m_nViewTopX + m_nViewWidth + dx > m_nMapWidth)
		dx = m_nMapWidth - m_nViewTopX - m_nViewWidth;
	if(m_nViewTopY + dy < 0)
		dy = - m_nViewTopY;
	if(m_nViewTopY + m_nViewHeight + dy > m_nMapHeight)
		dy = m_nMapHeight - m_nViewTopY - m_nViewHeight;
	if(dx == 0 && dy == 0)
		return FALSE;

	RECT rcNew,rcOffset,rcNewDirty;
	SetRect(&rcNew,(m_nViewTopX + dx) / m_nGridWidth,(m_nViewTopY + dy) / m_nGridHeight,
		(m_nViewTopX + m_nViewWidth - 1 + dx) / m_nGridWidth + 1,
		(m_nViewTopY + m_nViewHeight - 1 + dy) / m_nGridHeight + 1);
	if(EqualRect(&rcNew,&m_rcCurViewGrid))
	{
		m_nViewTopX += dx;
		m_nViewTopY += dy;
		SetRect(&m_rcViewportRect,m_nViewTopX,m_nViewTopY,m_nViewTopX + m_nViewWidth,m_nViewTopY + m_nViewHeight);
		return TRUE;
	}

	rcOffset = rcNew;
	OffsetRect(&rcOffset,dx > 0 ? 1 : dx < 0 ? -1 : 0,dy > 0 ? 1 : dy < 0 ? -1 : 0);
	UnionRect(&rcNewDirty,&rcNew,&rcOffset);
	IntersectRect(&rcNewDirty,&rcNewDirty,&m_rcMapGrid);

	m_nViewTopX += dx;
	m_nViewTopY += dy;

	SetRect(&m_rcViewportRect,m_nViewTopX,m_nViewTopY,m_nViewTopX + m_nViewWidth,m_nViewTopY + m_nViewHeight);
	
	// �����ɵ���飬�Ա�����ڴ�
	for(int y = m_rcLastDirty.top; y < m_rcLastDirty.bottom; y++)
		for(int x = m_rcLastDirty.left; x < m_rcLastDirty.right; x++)
		{
			POINT pt;
			pt.x = x; pt.y = y;
			if(!PtInRect(&rcNewDirty,pt) && !PtInRect(&rcNew,pt))
				m_pScrollSheetSink->OnDirtyGridLost(x,y);
		}

	// �����µ����
	for(int y = rcNewDirty.top; y < rcNewDirty.bottom; y++)
		for(int x = rcNewDirty.left; x < rcNewDirty.right; x++)
		{
			POINT pt;
			pt.x = x; pt.y = y;
			if(!PtInRect(&rcNewDirty/*m_rcLastDirty*/,pt))
				m_pScrollSheetSink->OnDirtyGridFound(x,y);
		}

	m_rcCurViewGrid = rcNew;
	m_rcLastDirty = rcNewDirty;
	return TRUE;
}
//------------------------------------------------------------------------
// �ɼ���ͼ�ƶ���Ļʱ��ʹԤ����ͼ�ѿɼ���ͼ��Χ�����м�
// �㷨�����Ǿ����ܱ�֤��ͼ�ӿ���Ԥ���ӿڵ��е��Ƕ����
// ���أ��й�������TRUE
BOOL CScrollSheet::ScrollToCenter(const RECT& rcView)
{
	int x = (rcView.left + rcView.right) / 2  - m_nViewWidth / 2;
	int y = (rcView.top + rcView.bottom) / 2 - m_nViewHeight / 2;

	return ScrollViewport(x - m_nViewTopX,y - m_nViewTopY);
}
//------------------------------------------------------------------------