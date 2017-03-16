/**
*	created:		2012-7-1   1:40
*	filename: 		MapCoordinate
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <windows.h>
//------------------------------------------------------------------------
#ifndef PixelCood2GridCood
	#define PixelCood2GridCood(PixelPos, GridLength)	((PixelPos + GridLength - 1) / GridLength)
#endif
#ifndef IN
	#define IN
#endif
#ifndef OUT
	#define OUT
#endif
//------------------------------------------------------------------------
class CMapCoordinate  
{
public:
	// ���������Ͻ���������ϵλ��
	void TileToPixel(IN const POINT& ptTile, OUT POINT& ptTileCenter) const
	{
		ptTileCenter.x = (ptTile.x + ptTile.y - m_yOff) << 5; // * 64 / 2;
		ptTileCenter.y = (ptTile.x - ptTile.y + m_yOff) << 4; // * 32 / 2;
	}
	
	// �����������껻�ɿ�����
	// �˼��㲻�ж��Ƿ���磬���ǳɹ�
	// ��������ԭ����(������x,���¿��y)���ڵ�λ��
	void PixelToTile(IN const POINT& ptWorld, OUT POINT& ptTile) const
	{
		const static int OffsetX[5] = { 0, 1, 1, 1, 2};
		const static int OffsetY[5] = { 0, 1, 0,-1, 0};

		int kx = ptWorld.x >> 6;		// kx = ptWorld.x / 64;
		int ky = ptWorld.y >> 5;		// ky = ptWorld.y / 32
		int off = m_TilePoint[ptWorld.y & 0x1F][ptWorld.x & 0x3F];	// ���ﲻ��Ҫ�Ż������������Ż�
		ptTile.x = kx + ky + OffsetX[off];
		ptTile.y = kx - ky + OffsetY[off] + m_yOff;
	}

	// ����������ϵ�ľ���rcת�ɿ�����rcTileArea = (left,top,col,row)
	// ע��
	//		rcΪ��������ϵ�µľ���,rc���ǵ�K����鲻�ǹ����Ŀ�
	//      �ּٶ�rc���ǵ�K��������ǰ����󶥵����ڵ�K�飬�ұߺ���
	RECT PixelRectToAreaTileRect(RECT rc) const
	{
		IntersectRect(&rc, &m_rcMap, &rc);
		rc.left = (PixelCood2GridCood(rc.left, 64))*64;
		rc.top = (PixelCood2GridCood(rc.top, 32))*32;
		rc.right &= ~63;
		rc.bottom &= ~31;
		RECT rcTileArea;
		PixelToTile((const POINT&)rc, (POINT&)rcTileArea);
		rcTileArea.right = PixelCood2GridCood(rc.right - rc.left, 64);
		rcTileArea.bottom = PixelCood2GridCood(rc.bottom - rc.top, 32);
		if (rcTileArea.top + rcTileArea.right >= m_nMatrixHeight)
			rcTileArea.right = m_nMatrixHeight - rcTileArea.top - 1;
		if (rcTileArea.top < rcTileArea.bottom - 1)
			rcTileArea.bottom = rcTileArea.top + 1;
		return rcTileArea;	
	}

	// ͬ�ϣ���������ϵ��Ч�Լ��
	RECT _PixelRectToAreaTileRect(RECT rc) const
	{
		RECT rcTileArea;
		PixelToTile((const POINT&)rc, (POINT&)rcTileArea);
		rcTileArea.right = PixelCood2GridCood(rc.right - rc.left, 64);
		rcTileArea.bottom = PixelCood2GridCood(rc.bottom - rc.top, 32);
		return rcTileArea;	
	}

	// ���룺nWidth��nHeight�ǵ�ͼ���ؿ�Ⱥ͸߶�
	BOOL Create(int nWidth, int nHeight)
	{
		int w = nWidth, h = nHeight;
		w &= ~63;
		h &= ~31;

		int nRows = PixelCood2GridCood(nHeight, 32);
		int nCols = PixelCood2GridCood(nWidth, 64);

		m_rcMap.right = nWidth;
		m_rcMap.bottom = nHeight;

		m_yOff = nRows - 1;
		m_nMatrixWidth = nRows + nCols;
		m_nMatrixHeight = m_nMatrixWidth - 1;
		return TRUE;
	}
	int GetMatrixWidth()
	{
		return m_nMatrixWidth;
	}
	int GetMatrixHeight()
	{
		return m_nMatrixHeight;
	}
	RECT& GetMapRect()
	{
		return m_rcMap;
	}
	CMapCoordinate()
	{
		m_yOff = 0;
		m_nMatrixWidth = m_nMatrixHeight = 0;
		memset(&m_rcMap, 0, sizeof(m_rcMap));
	}
	~CMapCoordinate()
	{
		
	}
	const static char m_TilePoint[32][64];
private:
	int		m_yOff;				// ���Ͻǵ�һ��TILE��y��ƫ��
	RECT	m_rcMap;
	int		m_nMatrixWidth;
	int		m_nMatrixHeight;
};
//------------------------------------------------------------------------