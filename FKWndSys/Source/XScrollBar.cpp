/**
*	created:		2012-6-24   0:09
*	filename: 		XScrollBar
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
#include "../Include/FKWndSysCommon.h"
//------------------------------------------------------------------------
#define TIMERID_SCROLL			0
#define TIMERID_SCROLLPAGE		1
#define INTERVAL_START_SCROLL	200
#define INTERVAL_SCROLL			5
//------------------------------------------------------------------------
const static int BORDERCOLOR = WPixel(100, 100, 100);      //�߽���ɫ
const static int BORDERWIDTH = 2;                          //�߽���
//------------------------------------------------------------------------
XScrollBar::XScrollBar()
{
	m_nMin = 0;
	m_nPos = -1;
	m_nMax = 100;
	m_nStyle = 0;
	m_nPressFlags = PF_NONE;
	m_pAttach = NULL;
	m_Size.cx = m_Size.cy = 20;
	SetRectEmpty(&m_rcBlock);
	SetRectEmpty(&m_rcLeftTop);
	SetRectEmpty(&m_rcRightBottom);
	m_nVisibleCount = 0;
	m_nTimerTick = 0;
	m_nLineDelta = 1;
	m_ptPageScrollPos.x = m_ptPageScrollPos.y = 0;
}
//------------------------------------------------------------------------
XScrollBar::XScrollBar(XWindow* pParentWnd) : XWindow(pParentWnd)
{
	m_nMin = 0;
	m_nPos = -1;
	m_nMax = 100;
	m_nStyle = 0;
	m_nPressFlags = PF_NONE;
	m_pAttach = NULL;
	m_Size.cx = m_Size.cy = 20;
	SetRectEmpty(&m_rcBlock);
	SetRectEmpty(&m_rcLeftTop);
	SetRectEmpty(&m_rcRightBottom);
	m_nVisibleCount = 0;
	m_nTimerTick = 0;
	m_nLineDelta = 1;
}
//------------------------------------------------------------------------
XScrollBar::~XScrollBar()
{
	GetTimeAxis()->KillTimer(TIMERID_SCROLL, static_cast<ITimerSink*>(this));
	GetTimeAxis()->KillTimer(TIMERID_SCROLLPAGE, static_cast<ITimerSink*>(this));
}
//------------------------------------------------------------------------
void XScrollBar::DestroyWindow()
{
	GetTimeAxis()->KillTimer(TIMERID_SCROLL, static_cast<ITimerSink*>(this));
	GetTimeAxis()->KillTimer(TIMERID_SCROLLPAGE, static_cast<ITimerSink*>(this));
	XWindow::DestroyWindow();
}
//------------------------------------------------------------------------
void XScrollBar::SetScrollRange(int nMin, int nMax, BOOL bRedraw)
{
	ASSERT(nMin<=nMax);
	m_nMin = nMin;
	m_nMax = nMax;
	if (m_nMax < m_nMin)
		m_nMax = m_nMin;
	if (bRedraw)
	{
		Refresh();
		InvalidRect();
	}
}
//------------------------------------------------------------------------
void XScrollBar::OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip)
{	
	pCanvas->DrawBox(m_rcWnd, BORDERCOLOR);
}
//------------------------------------------------------------------------
// ����ĸ���Сͼ��Ļ��ƺ����˲ü����Σ�TNND��̫�鷳�ˣ�Ӱ��Ч�ʣ��ʲ��迼�� :-)
// ����߻�����İ�ť
void XScrollBar::DrawLeftTopButton(ICanvas* pCanvas, int nScrollBarStyle, 
								   RECT& rcLeftTop, RECT& rcClip, DWORD dwBlitStyle)
{
	IBitmap* pBmp = m_pWndBitmap;
	if (!pBmp)
		pBmp = GetDesktop()->GetSysScrollBarBitmap();

	if (nScrollBarStyle == S_HORZ)
	{
		// ����ť
		int nOffset = LeftUp;
		if (m_nPressFlags == PF_LEFTTOP) // ����״̬
			nOffset = LeftDown;
		else if (!IsEnabled())
			nOffset = LeftDisable;
		if (GetWidth() >= 2*m_Size.cx)
			pBmp->Draw(pCanvas, rcLeftTop.left, rcLeftTop.top, m_Size.cx, m_Size.cy, 
				nOffset*m_Size.cx, 0, dwBlitStyle);
	}
	else
	{
		// ���ϰ�ť
		int nOffset = TopUp;
		if (m_nPressFlags == PF_LEFTTOP) // ����״̬
			nOffset = TopDown;
		else if (!IsEnabled())
			nOffset = TopDisable;
		if (GetHeight() >= 2*m_Size.cy)
			pBmp->Draw(pCanvas, rcLeftTop.left, rcLeftTop.top, m_Size.cx, m_Size.cy, 
				nOffset*m_Size.cx, 0, dwBlitStyle);
	}
}
//------------------------------------------------------------------------
// ���ұ߻�����İ�ť
void XScrollBar::DrawRightBottomButton(ICanvas* pCanvas, int nScrollBarStyle, 
									   RECT& rcRightBottom, RECT& rcClip, DWORD dwBlitStyle)
{
	IBitmap* pBmp = m_pWndBitmap;
	if (!pBmp)
		pBmp = GetDesktop()->GetSysScrollBarBitmap();

	if (nScrollBarStyle == S_HORZ)
	{
		// ���Ұ�ť
		int nOffset = RightUp;
		if (m_nPressFlags == PF_RIGHTBOTTOM) // ����״̬
			nOffset = RightDown;
		else if (!IsEnabled())
			nOffset = RightDisable;
		if (GetWidth() >= 2*m_Size.cx)
			pBmp->Draw(pCanvas, rcRightBottom.left, rcRightBottom.top, m_Size.cx, m_Size.cy, 
				nOffset*m_Size.cx, 0, dwBlitStyle);
	}
	else
	{
		// ���°�ť
		int nOffset = BottomUp;
		if (m_nPressFlags == PF_RIGHTBOTTOM) // ����״̬
			nOffset = BottomDown;
		else if (!IsEnabled())
			nOffset = BottomDisable;
		if (GetHeight() >= 2*m_Size.cy)
			pBmp->Draw(pCanvas, rcRightBottom.left, rcRightBottom.top, m_Size.cx, m_Size.cy, 
			nOffset*m_Size.cx, 0, dwBlitStyle);
	}
}
//------------------------------------------------------------------------
// ������������ĳ���
void XScrollBar::DrawTrackBar(ICanvas* pCanvas, int nScrollBarStyle, 
							  RECT& rcTraceBar, RECT& rcClip, DWORD dwBlitStyle)
{	
	IBitmap* pBmp = m_pWndBitmap;
	if (!pBmp)
		pBmp = GetDesktop()->GetSysScrollBarBitmap();

	if (nScrollBarStyle == S_HORZ)
	{
		int n = (rcTraceBar.right - rcTraceBar.left) / m_Size.cx; // ����С����
		int m = (rcTraceBar.right - rcTraceBar.left) % m_Size.cx; // ��ʣ����
		for (int i=0; i<n; i++)
			pBmp->Draw(pCanvas, rcTraceBar.left + i*m_Size.cx, rcTraceBar.top, m_Size.cx, m_Size.cy, 
				ScrollBarBackGroundHorz*m_Size.cx, 0, dwBlitStyle);
		if (m > 0)
			pBmp->Draw(pCanvas, rcTraceBar.right - m, rcTraceBar.top, m, m_Size.cy, 
				ScrollBarBackGroundHorz*m_Size.cx, 0, dwBlitStyle);
	}
	else
	{
		int n = (rcTraceBar.bottom - rcTraceBar.top) / (m_Size.cy ); // ����С����
		int m = (rcTraceBar.bottom - rcTraceBar.top) % (m_Size.cy ); // ��ʣ����
		for (int i=0; i<n; i++)
			pBmp->Draw(pCanvas, rcTraceBar.left, rcTraceBar.top + i*m_Size.cy, m_Size.cx, m_Size.cy, 
				ScrollBarBackGroundVert*m_Size.cx, 0, dwBlitStyle);
		if (m > 0)
			pBmp->Draw(pCanvas, rcTraceBar.left, rcTraceBar.bottom - m, m_Size.cx, m, 
				ScrollBarBackGroundVert*m_Size.cx, 0, dwBlitStyle);
	}
}
//------------------------------------------------------------------------
// ��������
void XScrollBar::DrawSlideBlock(ICanvas* pCanvas, int nScrollBarStyle, 
								RECT& rcSlideBlock, RECT& rcClip, DWORD dwBlitStyle)
{
	if (!IsEnabled()) return;
	IBitmap* pBmp = m_pWndBitmap;
	if (!pBmp)
		pBmp = GetDesktop()->GetSysScrollBarBitmap();
	
	if (nScrollBarStyle == S_HORZ)
	{
		// ��ˮƽ����
		if (IsEnabled() && GetWidth() >= 3*m_Size.cx)
		{
			int nOffset = BlockHorzUp;
			if (m_nPressFlags == PF_BLOCK)
				nOffset = BlockHorzDown;
			
			int nNot3DLength = rcSlideBlock.right - rcSlideBlock.left - 2*DELTA_3D;
			int nNot3DBmpLength = m_Size.cx - 2*DELTA_3D;
			int n = nNot3DLength / nNot3DBmpLength; // ����С����
			int m = nNot3DLength % nNot3DBmpLength; // ��ʣ����
			
			// �Ȼ���ߵ�3D�߽�
			pBmp->Draw(pCanvas, rcSlideBlock.left, rcSlideBlock.top, 
				DELTA_3D, 
				rcSlideBlock.bottom - rcSlideBlock.top, 
				nOffset*m_Size.cx, 0, dwBlitStyle);
			
			// �ٻ�n����3D�߽�ķ���
			for (int i=0; i<n; i++)
				pBmp->Draw(pCanvas, rcSlideBlock.left + DELTA_3D + i*nNot3DBmpLength, 
					rcSlideBlock.top, 
					nNot3DBmpLength, 
					rcSlideBlock.bottom - rcSlideBlock.top,
					nOffset*m_Size.cx + DELTA_3D, 0, dwBlitStyle);
			
			// ��ʣ��ķ���
			if (m > 0)
				pBmp->Draw(pCanvas, rcSlideBlock.right - DELTA_3D - m, 
					rcSlideBlock.top, 
					m, 
					rcSlideBlock.bottom - rcSlideBlock.top, 
					nOffset*m_Size.cx+DELTA_3D, 0, dwBlitStyle);
			
			// ����ұߵ�3D�߽�
			pBmp->Draw(pCanvas, rcSlideBlock.right - DELTA_3D, rcSlideBlock.top, 
				DELTA_3D, 
				rcSlideBlock.bottom - rcSlideBlock.top, 
				(nOffset+1)*m_Size.cx-DELTA_3D, 0, dwBlitStyle);
		}
	}
	else
	{
		// ����ֱ����
		if (IsEnabled() && GetHeight() >= 3*m_Size.cy)
		{
			int nOffset = BlockVertUp;
			if (m_nPressFlags == PF_BLOCK)
				nOffset = BlockVertDown;
			
			int nNot3DLength = rcSlideBlock.bottom - rcSlideBlock.top - 2*DELTA_3D;
			int nNot3DBmpLength = m_Size.cy - 2*DELTA_3D;
			int n = nNot3DLength / nNot3DBmpLength; // ����С����
			int m = nNot3DLength % nNot3DBmpLength; // ��ʣ����

			// �Ȼ������3D�߽�
			pBmp->Draw(pCanvas, rcSlideBlock.left, rcSlideBlock.top, 
				rcSlideBlock.right - rcSlideBlock.left, 
				DELTA_3D, 
				nOffset*m_Size.cx, 0, dwBlitStyle);
			
			// �ٻ�n����3D�߽�ķ���
			for (int i=0; i<n; i++)
				pBmp->Draw(pCanvas, rcSlideBlock.left, 
					rcSlideBlock.top + DELTA_3D + i*nNot3DBmpLength, 
					rcSlideBlock.right - rcSlideBlock.left, 
					nNot3DBmpLength, 
					nOffset*m_Size.cx, DELTA_3D, dwBlitStyle);
			
			// ��ʣ��ķ���
			if (m > 0)
				pBmp->Draw(pCanvas, rcSlideBlock.left, 
					rcSlideBlock.bottom - DELTA_3D - m, 
					rcSlideBlock.right - rcSlideBlock.left, 
					m, 
					nOffset*m_Size.cx, DELTA_3D, dwBlitStyle);
			
			// ��������3D�߽�
			pBmp->Draw(pCanvas, rcSlideBlock.left, rcSlideBlock.bottom - DELTA_3D, 
				rcSlideBlock.right - rcSlideBlock.left, 
				DELTA_3D, 
				nOffset*m_Size.cx, m_Size.cy-DELTA_3D, dwBlitStyle);
		}
	}
}
//------------------------------------------------------------------------
void XScrollBar::OnDraw(ICanvas* pCanvas, RECT& rcClip)
{
	IBitmap* pBmp = m_pWndBitmap;
	if (!pBmp)
		pBmp = GetDesktop()->GetSysScrollBarBitmap();

	if (pBmp)
	{
		DWORD style = BLIT_NORMAL;
		
		if(m_nAlpha < 256)
		{
			pBmp->SetUserData(m_nAlpha);
			style |= BLIT_TRANS;
		}
		
		if(IsMask())
			style |= BLIT_MASK;

		// �����ϰ�ť
		DrawLeftTopButton(pCanvas, m_nStyle, m_rcLeftTop, rcClip, style);

		// ��������
		RECT rc;
		if (m_nStyle == S_HORZ) // ˮƽ
			SetRect(&rc, m_rcWnd.left + m_Size.cx, m_rcWnd.top, m_rcWnd.right - m_Size.cx, m_rcWnd.bottom);
		else
			SetRect(&rc, m_rcWnd.left, m_rcWnd.top + m_Size.cy, m_rcWnd.right, m_rcWnd.bottom - m_Size.cy);
		DrawTrackBar(pCanvas, m_nStyle, rc, rcClip, style);

		// ��������
		DrawSlideBlock(pCanvas, m_nStyle, m_rcBlock, rcClip, style);

		// �����°�ť
		DrawRightBottomButton(pCanvas, m_nStyle, m_rcRightBottom, rcClip, style);
	}

	XWindow::OnDraw(pCanvas, rcClip);
}
//------------------------------------------------------------------------
void XScrollBar::SetScrollPos(int nPos, BOOL bRedraw)
{
	if (m_nMax == m_nMin)
	{
		if (nPos == m_nMax)
			m_nPos = nPos; // TNND�����ﲻ��������Ǹ�if������Ӵ
		return;
	}
	
	if (bRedraw)
	{
		Refresh();
		InvalidRect(&m_rcBlock);
	}
	
	int oldPos = m_nPos;
	m_nPos = nPos;
	if (m_nPos < m_nMin)
		m_nPos = m_nMin;
	if (m_nPos > m_nMax)
		m_nPos = m_nMax;


	s_posChanged((DWORD)&m_nPos);

	if (bRedraw)
	{
		Refresh();
		InvalidRect(&m_rcBlock);
	}
}
//------------------------------------------------------------------------
void XScrollBar::SetSize(SIZE& size) 
{
	m_Size.cx = size.cx; m_Size.cy = size.cy;
}
//------------------------------------------------------------------------
void XScrollBar::MoveWindow(int dx, int dy)
{
	XWindow::MoveWindow(dx, dy);
	OffsetRect(&m_rcBlock, dx, dy);
	OffsetRect(&m_rcLeftTop, dx, dy);
	OffsetRect(&m_rcRightBottom, dx, dy);
}
//------------------------------------------------------------------------
// ������Ϲ���һ��
void XScrollBar::LineLeftUp()
{
	int nPos = GetScrollPos();
	nPos -= m_nLineDelta;
	SetScrollPos(nPos);
}
//------------------------------------------------------------------------
// ���һ��¹���һ��
void XScrollBar::LineRightDown()
{
	int nPos = GetScrollPos();
	nPos += m_nLineDelta;
	SetScrollPos(nPos);
}
//------------------------------------------------------------------------
// ������Ϲ���һҳ
void XScrollBar::PageLeftUp()
{
	int nPos = GetScrollPos();
	int nDelta = m_nVisibleCount >> 3;
	nPos -= (nDelta > 0) ? m_nVisibleCount-nDelta : m_nVisibleCount;
	SetScrollPos(nPos);
}
//------------------------------------------------------------------------
// ���һ��¹���һҳ
void XScrollBar::PageRightDown()
{
	int nPos = GetScrollPos();
	int nDelta = m_nVisibleCount >> 3;
	nPos += (nDelta > 0) ? m_nVisibleCount-nDelta : m_nVisibleCount;
	SetScrollPos(nPos);
}
//------------------------------------------------------------------------
void XScrollBar::CheckPageScroll()
{
	if (m_nPressFlags == PF_LEFTTOPPAGE)
	{
		if (m_nStyle == S_HORZ)
		{
			if (m_ptPageScrollPos.x >= m_rcBlock.left)
				GetTimeAxis()->KillTimer(TIMERID_SCROLLPAGE, static_cast<ITimerSink*>(this));			
		}
		else
		{
			if (m_ptPageScrollPos.y >= m_rcBlock.top)
				GetTimeAxis()->KillTimer(TIMERID_SCROLLPAGE, static_cast<ITimerSink*>(this));			
		}
	}
	else if (m_nPressFlags == PF_RIGHTBOTTOMPAGE)
	{
		if (m_nStyle == S_HORZ)
		{
			if (m_ptPageScrollPos.x < m_rcBlock.right)
				GetTimeAxis()->KillTimer(TIMERID_SCROLLPAGE, static_cast<ITimerSink*>(this));			
		}
		else
		{
			if (m_ptPageScrollPos.y < m_rcBlock.bottom)
				GetTimeAxis()->KillTimer(TIMERID_SCROLLPAGE, static_cast<ITimerSink*>(this));			
		}
	}
}
//------------------------------------------------------------------------
void XScrollBar::OnTimer(DWORD dwEventID)
{
	m_nTimerTick ++;

	if (dwEventID == TIMERID_SCROLL)
	{
		if (m_nTimerTick == 1) // �޸�ʱ�Ӽ��
		{
			GetTimeAxis()->KillTimer(TIMERID_SCROLL, static_cast<ITimerSink*>(this));
			GetTimeAxis()->SetTimer(TIMERID_SCROLL, static_cast<ITimerSink*>(this), INTERVAL_SCROLL);
		}
		else
		{
			if (m_nPressFlags == PF_LEFTTOP)
				LineLeftUp();
			else if (m_nPressFlags == PF_RIGHTBOTTOM)
				LineRightDown();
		}
	}
	else if (dwEventID == TIMERID_SCROLLPAGE)
	{
		if (m_nTimerTick == 1) // �޸�ʱ�Ӽ��
		{
			GetTimeAxis()->KillTimer(TIMERID_SCROLLPAGE, static_cast<ITimerSink*>(this));
			GetTimeAxis()->SetTimer(TIMERID_SCROLLPAGE, static_cast<ITimerSink*>(this), INTERVAL_SCROLL);
		}
		else
		{
			if (m_nPressFlags == PF_LEFTTOPPAGE)
				PageLeftUp();
			else if (m_nPressFlags == PF_RIGHTBOTTOMPAGE)
				PageRightDown();
			CheckPageScroll();
		}
	}
}
//------------------------------------------------------------------------
XWindow* XScrollBar::Attach(XWindow* pWnd)
{
	XWindow* pOldWnd = m_pAttach;
	m_pAttach = pWnd;
	return pOldWnd;
}
//------------------------------------------------------------------------
DWORD XScrollBar::OnMouseDown(UINT nButton, UINT nShift, POINT pt)
{
	if (!IsEnabled())
		return 0;

	m_nTimerTick = 0;
	
	if (PtInRect(&m_rcBlock, pt)) // ���ڻ�����
	{
		m_nPressFlags = PF_BLOCK;
		// ��������໬���󶥵�ƫ����
		POINT ptTemp;
		ptTemp.x = pt.x - m_rcBlock.left;
		ptTemp.y = pt.y - m_rcBlock.top;
		WritePoint(ptTemp);
		InvalidRect(&m_rcBlock);
	}
	else if (PtInRect(&m_rcLeftTop, pt))
	{
		m_nPressFlags = PF_LEFTTOP;
		InvalidRect(&m_rcLeftTop);
		GetTimeAxis()->SetTimer(TIMERID_SCROLL, static_cast<ITimerSink*>(this), INTERVAL_START_SCROLL);
		
		LineLeftUp();
		
	}
	else if (PtInRect(&m_rcRightBottom, pt))
	{
		m_nPressFlags = PF_RIGHTBOTTOM;
		InvalidRect(&m_rcRightBottom);
		GetTimeAxis()->SetTimer(TIMERID_SCROLL, static_cast<ITimerSink*>(this), INTERVAL_START_SCROLL);
		
		LineRightDown();
		
	}
	else
	{
		m_ptPageScrollPos = pt;
		GetTimeAxis()->SetTimer(TIMERID_SCROLLPAGE, static_cast<ITimerSink*>(this), INTERVAL_START_SCROLL);
		if (m_nStyle == S_HORZ)
		{
			if (pt.x < m_rcBlock.left)
			{
				m_nPressFlags = PF_LEFTTOPPAGE;
				PageLeftUp();
			}
			else
			{
				m_nPressFlags = PF_RIGHTBOTTOMPAGE;
				PageRightDown();
			}
		}
		else
		{
			if (pt.y < m_rcBlock.top)
			{
				m_nPressFlags = PF_LEFTTOPPAGE;
				PageLeftUp();
			}
			else
			{
				m_nPressFlags = PF_RIGHTBOTTOMPAGE;
				PageRightDown();
			}
		}
	}
	if (nButton == LeftButton) // �������
	{
		SetCapture();
		if (m_pAttach)
			m_pAttach->SetFocus();
		else
			SetFocus();
		if (IsMovable())
		{
			WritePoint(pt);
			WriteRect(m_rcWnd);
		}
	}
	
	// ͬʱ��������������Ϊ�����
	GetMainWnd()->SetActive();
	return 0;
}
//------------------------------------------------------------------------
DWORD XScrollBar::OnMouseUp(UINT nButton, UINT nShift, POINT pt)
{
	if (!IsEnabled())
		return 0;
	
	if (m_nPressFlags == PF_BLOCK)
	{
		InvalidRect(&m_rcBlock);
	}
	else if (m_nPressFlags == PF_LEFTTOP)
	{
		InvalidRect(&m_rcLeftTop);
		GetTimeAxis()->KillTimer(TIMERID_SCROLL, static_cast<ITimerSink*>(this));
	}
	else if (m_nPressFlags == PF_RIGHTBOTTOM)
	{
		InvalidRect(&m_rcRightBottom);
		GetTimeAxis()->KillTimer(TIMERID_SCROLL, static_cast<ITimerSink*>(this));
	}
	
	m_nPressFlags = PF_NONE;
	
	if (PtInRect(&m_rcBlock, pt) || // ���ڻ�����
		PtInRect(&m_rcLeftTop, pt) || 
		PtInRect(&m_rcRightBottom, pt)) 
		SetCursor(CT_HAND);
	else
		SetCursor(CT_ARROW);

	return XWindow::OnMouseUp(nButton, nShift, pt);
}
//------------------------------------------------------------------------
DWORD XScrollBar::OnKeyDown(UINT nKeyCode, UINT nShift)
{
	switch (nKeyCode)
	{
	case VK_HOME:
		SetScrollPos(0);
		break;
	case VK_END:
		SetScrollPos(GetMaxRange());
		break;
	case VK_UP:
	case VK_LEFT:
		LineLeftUp();
		break;
	case VK_DOWN:
	case VK_RIGHT:
		LineRightDown();
		break;
	case VK_PRIOR:
		PageLeftUp();
		break;
	case VK_NEXT:
		PageRightDown();
		break;
	default:
		{
			return XWindow::OnKeyDown(nKeyCode, nShift);
		}
	}
	InvalidRect();	
	return 0;
}
//------------------------------------------------------------------------
DWORD XScrollBar::OnMouseMove(UINT nButton, UINT nShift, POINT pt)
{
	if (!IsEnabled())
		return 0;

	int nPos;
	if (m_nStyle == S_HORZ)
	{
		if (nButton == LeftButton && m_nPressFlags == PF_BLOCK) // �϶�����
		{
			POINT ptTemp = ReadPoint();
			if (m_nMax > m_nMin)
			{
				int nBlockLength = (GetWidth() - 2*m_Size.cx)*m_nVisibleCount/(m_nVisibleCount + m_nMax - m_nMin);
				nBlockLength = max((int)m_Size.cx, nBlockLength);
				int nPos = 0;
				int nn = GetWidth() - 2*m_Size.cx - nBlockLength;
				if (nn > 0)
					nPos = (pt.x - m_rcWnd.left - m_Size.cx - ptTemp.x)*(m_nMax - m_nMin)/nn;
				SetScrollPos(nPos);
			}
		}
		else
		{
			if (PtInRect(&m_rcBlock, pt) || // ���ڻ�����
				PtInRect(&m_rcLeftTop, pt) || 
				PtInRect(&m_rcRightBottom, pt)) 
				SetCursor(CT_HAND);
			else
				SetCursor(CT_ARROW);
		}
	}
	else if (m_nStyle == S_VERT)
	{
		if (nButton == LeftButton && m_nPressFlags == PF_BLOCK) // �϶�����
		{
			POINT ptTemp = ReadPoint();
			if (m_nMax > m_nMin)
			{
				int nBlockLength = (GetHeight() - 2*m_Size.cy)*m_nVisibleCount/(m_nVisibleCount + m_nMax - m_nMin);
				nBlockLength = max((int)m_Size.cy, nBlockLength);
				nPos = 0;
				int nn = GetHeight() - 2*m_Size.cy - nBlockLength;
				if (nn > 0)
					nPos = (pt.y - m_rcWnd.top - m_Size.cy - ptTemp.y)*(m_nMax - m_nMin)/nn;
				SetScrollPos(nPos);
			}
		}
		else
		{
			if (PtInRect(&m_rcBlock, pt) || // ���ڻ�����
				PtInRect(&m_rcLeftTop, pt) || 
				PtInRect(&m_rcRightBottom, pt)) 
				SetCursor(CT_HAND);
			else
				SetCursor(CT_ARROW);
		}
	}

	return XWindow::OnMouseMove(nButton, nShift, pt);
}
//------------------------------------------------------------------------
DWORD XScrollBar::OnMouseWheel(UINT nButton, UINT nShift, short zDelta, POINT pt)
{
	if (m_pAttach)
		return m_pAttach->OnMouseWheel(nButton, nShift, zDelta, pt);
	return 0;
}
//------------------------------------------------------------------------
void XScrollBar::OnMouseEnter()
{
	if (IsEnabled())
		SetCursor(CT_HAND);
}
//------------------------------------------------------------------------
void XScrollBar::OnMouseExit()
{
	if (IsEnabled())
		SetCursor(CT_ARROW);
}
//------------------------------------------------------------------------
// ������ m_Size, m_nPos, m_rcWnd,m_nVisibleCount, m_nMax, m_nMin �仯ʱ����Ҫˢ��
void XScrollBar::Refresh()
{
	if (m_nMin == m_nMax)
		return;
	if (m_nStyle == S_HORZ)
	{
		SetRect(&m_rcLeftTop, m_rcWnd.left, m_rcWnd.top, m_rcWnd.left + m_Size.cx, m_rcWnd.bottom);
		SetRect(&m_rcRightBottom, m_rcWnd.right - m_Size.cx, m_rcWnd.top, m_rcWnd.right, m_rcWnd.bottom);

		if (m_nMax > m_nMin)
		{
			int nBlockLength = (GetWidth() - 2*m_Size.cx)*m_nVisibleCount/(m_nVisibleCount + m_nMax - m_nMin);
			nBlockLength = max((int)m_Size.cx, nBlockLength);
			
			m_rcBlock.left = m_rcWnd.left + m_Size.cx + 
				(GetWidth() - 2*m_Size.cx - nBlockLength)*(GetScrollPos() - m_nMin)/(m_nMax - m_nMin);
			m_rcBlock.top = m_rcWnd.top;
			m_rcBlock.right = m_rcBlock.left + nBlockLength;
			m_rcBlock.bottom = m_rcWnd.bottom;
		}
	}
	else if (m_nStyle == S_VERT)
	{
		SetRect(&m_rcLeftTop, m_rcWnd.left, m_rcWnd.top, m_rcWnd.right, m_rcWnd.top + m_Size.cy);
		SetRect(&m_rcRightBottom, m_rcWnd.left, m_rcWnd.bottom - m_Size.cy, m_rcWnd.right, m_rcWnd.bottom);
		
		if (m_nMax > m_nMin)
		{
			int nBlockLength = (GetHeight() - 2*m_Size.cy)*m_nVisibleCount/(m_nVisibleCount + m_nMax - m_nMin);
			nBlockLength = max((int)m_Size.cy, nBlockLength);
			
			m_rcBlock.left = m_rcWnd.left;
			m_rcBlock.top = m_rcWnd.top + m_Size.cy + 
				(GetHeight() - 2*m_Size.cy - nBlockLength)*(GetScrollPos() - m_nMin)/(m_nMax - m_nMin);
			m_rcBlock.right = m_rcWnd.right;
			m_rcBlock.bottom = m_rcBlock.top + nBlockLength;
		}
	}
}
//------------------------------------------------------------------------
void XScrollBar::OnPaint(ICanvas* pCanvas)
{
	if (!IsVisible()) 
		return;
	
	RECT rc;
	if (m_pAttach)
	{
		if (m_pAttach->GetParent())
		{
			m_pAttach->GetParent()->GetWindowRect(&rc);
			RECT rcParent;
			m_pAttach->GetWindowRect(&rcParent);
			IntersectRect(&rc, &rcParent, &rc);
		}
	}
	else
	{
		if (m_pParent)
		{
			m_pParent->GetWindowRect(&rc);
			IntersectRect(&rc, &m_rcWnd, &rc);
		}
		else
			CopyRect(&rc, &m_rcWnd);
	}
	if (IsRectEmpty(&rc))
		return;
	
	// ����������࣬���Ʊ�����
	if (pCanvas->IsInDirty(rc))
	{
		OnEraseBkgnd(pCanvas, rc);
		OnDraw(pCanvas, rc);
	}
}
//------------------------------------------------------------------------