/**
*	created:		2012-7-1   2:56
*	filename: 		MiniMapView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
/*
 * ����		:	����ر���ʾ�����ܰ����ر�������Ź���ʽ�ĵر�ͼ�����Լ�����ر�ͼ��?��
				����ʱ�ر�ĺ�̨���ء�
				�������
   ˵��		:
				1��ʵ�ֵ�Ԫ��IBitmap�Ļ��湦�ܣ�����õ�ͼƬ������������ȡ�򵥵��Ƚ��ȳ�����̭�㷨
				ʵ�֣�
					1)������鶪ʧʱ�������ĸ��ͼƬ��������
					2)������Ҫ�����µ�ͼƬʱ������������ɵ�ͼƬ�ڴ��ύ��CJpegLoader
					3)��CJpegLoaderֻ����û��IBitmap����ʱ�������ڴ洴��IBitmap
				2��jpg�ļ���ȡ�����⣬��һ���Զ�ȡ����Ҫ���ż������š���̨��̬��ȡ��������
 */
//------------------------------------------------------------------------
#include "../Include/MiniMapView.h"
#include "../Include/MapViewCommon.h"
//------------------------------------------------------------------------
#define _NO_USE_THREAD
#ifndef _NO_USE_THREAD
	#if defined(_USE) || defined(_DEBUG) || defined(_TEST)
		#define _NO_USE_THREAD
	#endif
#endif
//------------------------------------------------------------------------
// szGridIndexArrayΪ�ر��ƴ�������ļ�
BOOL CMiniMapView::Create(int nMapWidth, int nMapHeight, int nGridWidth, int nGridHeight,
						 LPRECT lprcViewport, 
						 const char* szJpgFileDir, ITrace*pTrace)
{
	m_nMapWidth = nMapWidth;
	m_nMapHeight = nMapHeight;
	m_rcMapRect.right = nMapWidth;
	m_rcMapRect.bottom = nMapHeight;

	m_nGridWidth = nGridWidth;
	m_nGridHeight = nGridHeight;

	m_nViewWidth = lprcViewport->right - lprcViewport->left;
	m_nViewHeight = lprcViewport->bottom - lprcViewport->top;
	
	m_nTotalGridX = PixelCood2GridCood(m_nMapWidth, nGridWidth);
	m_nTotalGridY = PixelCood2GridCood(m_nMapHeight, nGridHeight);

	if(!m_jpgLoader.Create(szJpgFileDir,pTrace))
		return FALSE;

	int nTotalGrid = m_nTotalGridX * m_nTotalGridY;
	TraceLn("����ͼС��ͼGrid���� = %d", nTotalGrid);
	m_pMapGrid = new MAP_GRID[nTotalGrid];
	if(m_pMapGrid == NULL)
		return FALSE;

	for(int i = 0; i < nTotalGrid; i++)
		m_pMapGrid[i].wBmpIndex = (WORD)i;

	if (!m_ScrollSheet.Create(static_cast<IScrollSheetSink*>(this),
		nMapWidth, nMapHeight, nGridWidth, nGridHeight, lprcViewport))
		return FALSE;

	return TRUE;
}
//------------------------------------------------------------------------
void CMiniMapView::Close()
{
	m_jpgLoader.Close();
	if(m_pMapGrid)
	{
		MAP_GRID *p = m_pMapGrid;
		for(int y = 0; y < m_nTotalGridY; y++)
			for(int x = 0; x < m_nTotalGridX; x++)
			{
				if(p->pBmp != NULL)
					p->pBmp->Release();
				p++;
			}
		delete m_pMapGrid;
		m_pMapGrid = NULL;
	}
}
//------------------------------------------------------------------------
BOOL CMiniMapView::SizeViewport(int nWidth, int nHeight)
{
	if(nWidth == 0 || nHeight == 0)
		return FALSE;
	m_nViewWidth = nWidth;
	m_nViewHeight = nHeight;
	m_ScrollSheet.SizeViewport(nWidth, nHeight);
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMiniMapView::MoveViewportTo(int x, int y)
{
	m_ScrollSheet.MoveViewportTo(x,y);
	return TRUE;
}
//------------------------------------------------------------------------
// �ѿɼ�����ʾ���������û������ؿ��IBitmap������ɺ�ɫ
void CMiniMapView::OnDraw(ICanvas* pCanvas)
{
	int nViewTopX = m_ScrollSheet.GetViewTopX();
	int nViewTopY = m_ScrollSheet.GetViewTopY();
	int col = (nViewTopX + m_nViewWidth - 1) / m_nGridWidth - nViewTopX / m_nGridWidth + 1;
	int row = (nViewTopY + m_nViewHeight - 1) / m_nGridHeight - nViewTopY / m_nGridHeight + 1;
	if(col == 0 || row == 0)
		return;

	int nGridTopX = nViewTopX / m_nGridWidth;						// ���ӿ����ϽǸ������
	int nGridTopY = nViewTopY / m_nGridHeight;

	RECT rcView = {nViewTopX,nViewTopY,nViewTopX + m_nViewWidth,nViewTopY + m_nViewHeight};
	
	RECT rcDirtyGrid;	// ������Ҫ��һ������ʾ����飬��������ÿ�����
	rcDirtyGrid.left = nGridTopX * m_nGridWidth;
	rcDirtyGrid.top = nGridTopY * m_nGridHeight;
	rcDirtyGrid.right = rcDirtyGrid.left + m_nGridWidth;
	rcDirtyGrid.bottom = rcDirtyGrid.top + m_nGridHeight;

	ASSERT(nGridTopY * m_nTotalGridX + nGridTopX >= 0);
	MAP_GRID* pLine = &m_pMapGrid[ nGridTopY * m_nTotalGridX + nGridTopX ];
	int nDirtyRectX0 = rcDirtyGrid.left;
	for(int y = 0; y < row; y++)
	{
		MAP_GRID* px = pLine;
		for(int x = 0; x < col; x++,px++)
		{
			RECT rcGridInScreen; // һ��grid����Ļ�Ͽɼ��Ĳ���
			BOOL b = IntersectRect(&rcGridInScreen,&rcDirtyGrid,&rcView);
			
			// �������һ��IBitmap������Ƿ����µ�IBitmap�������
			if(px->pBmp == NULL)
			{
#ifdef _NO_USE_THREAD
				// ����������վ�����ҿ�
				int index = (nGridTopY + y) * m_nTotalGridX + nGridTopX + x;
				IBitmap* pBmp = m_BitmapRecycle.GetObj(m_pMapGrid[index].wBmpIndex);
				if (pBmp)
					m_pMapGrid[index].SetLoaded(pBmp);
				else
				{
					// �Ҳ���ֻ��ȥ������
					if (m_BitmapRecycle.IsFull())
						m_BitmapRecycle.PopHead();
					pBmp = m_jpgLoader.LoadJpgNoUseThread(m_pMapGrid[index].wBmpIndex);
					if (pBmp)
						m_pMapGrid[index].SetLoaded(pBmp);
				}
#else
				if (px->IsNeedLoad())
					OnDirtyGridFound(nGridTopX + x,nGridTopY + y);
				if (px->IsRequesting())
				{
					// �ȴ��ܶ�ʱ�䣬�Ա�ͼƬ��ʱ�����ܼ��ؽ���
					DWORD dwLast = GetTickCount();
					do
					{
						GetBackgroundLoadingBitmap();
						if(!px->IsRequesting())
							break;
						else
							Sleep(1);
					#ifdef _DEBUG
					}while(GetTickCount() - dwLast < 100);
					#else
					}while(GetTickCount() - dwLast < 80);
					#endif
				}
#endif // _NO_USE_THREAD
#ifdef _DEBUG
				if(px->IsLoadError())
				{
					char str[1024];
					wsprintf(str,"Warn : CMiniMapView::OnDraw() : can not load grid(%d,%d),load error\r\n",nGridTopX + x,nGridTopY + y);
					OutputDebugString(str);
				}
				else if(px->IsEmptyBitmap())
				{
					char str[1024];
					wsprintf(str,"Warn : CMiniMapView::OnDraw() : can not load grid(%d,%d),wait time out\r\n",nGridTopX + x,nGridTopY + y);
					OutputDebugString(str);
				}
#endif
			}
			if(px->pBmp)
			{
				DWORD dwStyle = BLIT_NORMAL;
				if (m_nAlpha >0 && m_nAlpha < 256)
				{
					dwStyle |= BLIT_TRANS;
					px->pBmp->SetUserData(m_nAlpha);
				}
				px->pBmp->Draw(pCanvas, 
					rcGridInScreen.left - nViewTopX, 
					rcGridInScreen.top - nViewTopY,
					rcGridInScreen.right - rcGridInScreen.left, // width
					rcGridInScreen.bottom - rcGridInScreen.top, // height
					rcGridInScreen.left - rcDirtyGrid.left,
					rcGridInScreen.top  - rcDirtyGrid.top, dwStyle);
			}

			rcDirtyGrid.left += m_nGridWidth;
			rcDirtyGrid.right = rcDirtyGrid.left + m_nGridWidth; 
		}
		rcDirtyGrid.left = nDirtyRectX0;
		rcDirtyGrid.right = rcDirtyGrid.left + m_nGridWidth;
		rcDirtyGrid.top += m_nGridHeight;
		rcDirtyGrid.bottom += m_nGridHeight;

		pLine += m_nTotalGridX;
	}
}
//------------------------------------------------------------------------
// ����Ƿ����µ�IBitmap�������
void CMiniMapView::GetBackgroundLoadingBitmap()
{
	int nXY;IBitmap* pBitmap;
	while(m_jpgLoader.GetBmp(nXY,&pBitmap))
	{
		if(m_pMapGrid[nXY].pBmp == NULL)
		{
			if(pBitmap)
				m_pMapGrid[nXY].SetLoaded(pBitmap);
			else
				m_pMapGrid[nXY].SetLoadError();
		}
		else
		{
			if(pBitmap)
				pBitmap->Release();
		}
	}
}
//------------------------------------------------------------------------
void CMiniMapView::OnDirtyGridFound(int nGridX,int nGridY)
{
#ifndef _NO_USE_THREAD
	int index = nGridY * m_nTotalGridX + nGridX;
	if (m_pMapGrid[index].IsEmptyBitmap()) return;
	if(m_pMapGrid[index].IsNeedLoad())
	{
		// ����������վ�����ҿ�
		IBitmap* pBmp = m_BitmapRecycle.GetObj(m_pMapGrid[index].wBmpIndex);
		if(pBmp)
		{
			m_pMapGrid[index].SetLoaded(pBmp);
			return;
		}

		// �Ҳ���ֻ��ȥ������
		m_pMapGrid[index].SetReqesting();
		m_jpgLoader.AddJpg(m_pMapGrid[index].wBmpIndex,index,0,NULL,m_BitmapRecycle.IsFull() ? m_BitmapRecycle.PopHead() : NULL);
	}
#endif
}
//------------------------------------------------------------------------
void CMiniMapView::OnDirtyGridLost(int nGridX,int nGridY)
{
#ifndef _NO_USE_THREAD
	int index = nGridY * m_nTotalGridX + nGridX;
	if (m_pMapGrid[index].IsEmptyBitmap()) return;
	if(m_pMapGrid[index].IsLoaded())
		m_BitmapRecycle.AddTail(m_pMapGrid[index].wBmpIndex,m_pMapGrid[index].pBmp);
	else if(m_pMapGrid[index].IsRequesting())
	{
		IBitmap* pBmp = NULL;
		m_jpgLoader.RemoveOldJpg(m_pMapGrid[index].wBmpIndex,&pBmp); // ����������������jpg�о������ž�Ҫ����̭��
		if(pBmp)
			m_BitmapRecycle.AddGarbageToHead(-1,pBmp);
	}
	m_pMapGrid[index].Init();
#endif
}
//------------------------------------------------------------------------
void CMiniMapView::ScrollViewport(int dx, int dy)
{
	if (!m_ScrollSheet.ScrollViewport(dx, dy))
		return;
}
//------------------------------------------------------------------------