/**
*	created:		2012-6-23   23:25
*	filename: 		ItemContainer
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
#include "../Include/FKWndSysCommon.h"
//------------------------------------------------------------------------
XItemContainer::XItemContainer()
{
	m_nMargin = 0;
	m_bLineWrap = FALSE;
	m_nMouseHover = MAKELONG(-1, -1);
	m_nMaxWidth = m_nMaxHeight = 0;
	m_ptOffset.x = m_ptOffset.y = 0;
	m_pHScrollBar = m_pVScrollBar = NULL;

	CONN_clicked(this, XItemContainer, OnClick);
}
//------------------------------------------------------------------------
XItemContainer::XItemContainer(XWindow* pParentWnd) : XWindow(pParentWnd)
{
	m_nMargin = 0;
	m_bLineWrap = FALSE;
	m_nMouseHover = MAKELONG(-1, -1);
	m_nMaxWidth = m_nMaxHeight = 0;
	m_ptOffset.x = m_ptOffset.y = 0;
	m_pHScrollBar = m_pVScrollBar = NULL;

	CONN_clicked(this, XItemContainer, OnClick);
}
//------------------------------------------------------------------------
XItemContainer::~XItemContainer()
{
}
//------------------------------------------------------------------------
void XItemContainer::DestroyWindow()
{
	m_pHScrollBar = NULL;
	m_pVScrollBar = NULL;
	Clear();
	XWindow::DestroyWindow();
	
}
//------------------------------------------------------------------------
// �����������
void XItemContainer::Clear()
{
	DeleteLines(GetLineCount());
	m_nMouseHover = MAKELONG(-1, -1);
	SetMaxWidth(GetWidth() - 2*m_nMargin);
	SetMaxHeight(GetHeight() - 2*m_nMargin);
	SetOffset(0, 0);
}
//------------------------------------------------------------------------
// ��ĳ����Ϊ����(nLineΪ����������0��ʼ)
void XItemContainer::BringToTopLine(int nLine)
{
	if (nLine >= GetLineCount()) // ����������������ǿ�����Ƶ����һ��
		nLine = GetLineCount() - 1;
	XLine* pLine = GetLine(nLine);
	int nLineOffset = pLine->GetOffsetY();
	if (nLineOffset + GetHeight() - 2*m_nMargin > GetMaxHeight())
		nLineOffset = GetMaxHeight() - GetHeight() + 2*m_nMargin;
	if (m_pVScrollBar)
		m_pVScrollBar->SetScrollPos(nLineOffset);
	else
		m_ptOffset.y = -nLineOffset;
	AdjustVScroll();
}
//------------------------------------------------------------------------
// �½�һ�У����������Ķ�Ӧ��ָ��
XLine* XItemContainer::NewLine(int nAlign, int nLineSpace)
{
	XLine* pLine = new XLine(nAlign, static_cast<ILineContainer*>(this));
	pLine->SetLineSpace(nLineSpace);
	if (m_vLines.empty())
		pLine->SetOffsetY(0);
	else
	{
		XLine* pLinePrev = GetLine(GetLineCount()-1);
		pLine->SetOffsetY(pLinePrev->GetOffsetY() + pLinePrev->GetHeight() + pLinePrev->GetLineSpace());
	}
	m_vLines.push_back(pLine);
	return pLine;
}
//------------------------------------------------------------------------
int	XItemContainer::GetMaxValidWidth()
{
	return GetMaxWidth()/* - m_nMargin*2*/;
}
//------------------------------------------------------------------------
// ����ƫ�ƾ���õ���Ӧ�е���Ŀ����
// nLine:������
// nOffsetX:��������߼������ƫ��ֵ�������m_ptOffset����λ�ã�������Դ��ھ��ε�ƫ��ֵ
int XItemContainer::GetItemByOffset(int nLine, int nOffsetX)
{
	if (nLine < 0 || nLine >= GetLineCount())
		return -1;
	XLine* pLine = GetLine(nLine);
	ASSERT(pLine);

	nOffsetX -= GetAlignOffset(pLine);
	XItem* pItem = NULL;
	int nItemCount = pLine->GetItemCount();
	for (int i=0; i<nItemCount; i++)
	{
		pItem = pLine->GetItem(i);
		if (nOffsetX >= pItem->GetOffsetX() && 
			nOffsetX < pItem->GetOffsetX() + pItem->GetWidth())
			return i;
	}
	return -1;
}
//------------------------------------------------------------------------
// ��������һ������ɾ������
// nLines: ��������������
void XItemContainer::DeleteLines(int nLines)
{
	if (nLines <= 0)
		return;
	int nLineCount = GetLineCount();
	if (nLines >= nLineCount) // �����������
	{
		// ���������
		for (int i=0; i<nLineCount; i++)
			GetLine(i)->Clear();
		m_vLines.clear();
		
		// �������Ŀ
		int nParentItemCount = m_vSplitItems.size();
		for ( int i=0; i<nParentItemCount; i++)
			m_vSplitItems[i]->Release();
		m_vSplitItems.clear();

		AdjustVScroll();
		InvalidRect();
		return;
	}
	
	// ����Ĺ��̣�����������ɾ����ǰ���nLines��
	// ���ҵ�Ҫɾ�����л�֮ǰ�����е����ĸ���Ŀ�����ţ���֤ɾ����ĵ�һ�еĸ���Ŀ����ɾ��
	int nMaxLineOffset = 0;
	int nParentIndex = -1;
	int nLine = nLines - 1;
	XLine* pLine = GetLine(nLine);
	ASSERT(pLine);
	nMaxLineOffset = pLine->GetOffsetY() + pLine->GetHeight() + pLine->GetLineSpace();
	int nItem = pLine->GetItemCount() - 1;
	XItem* pItem = NULL;
	if (nItem >= 0)
	{
		pItem = pLine->GetItem(nItem);
		nParentIndex = pItem->GetParent();
	}
	if (nParentIndex > XItem::NotParent)
	{
		if (pItem->GetLinkStyle() & XItem::lsLinkNext) // �����ɾ����ĵ�һ�������ӵģ���ɾ�����ĸ�����
			nParentIndex --;
	}
	else
	{
		while (nParentIndex == XItem::NotParent)
		{
			// ��һ��
			if (nItem <= 0) // ת����һ��
			{
				if (--nLine < 0)
					break;
				pLine = GetLine(nLine);
				nItem = pLine->GetItemCount() - 1;
			}
			else
				nItem--;
			if (nItem >= 0)
			{
				pItem = pLine->GetItem(nItem);
				ASSERT(pItem);
				nParentIndex = pItem->GetParent();
			}
		}
	}

	// ���ڵĵ�һ��item�Ƿ���XItem::IsLinkPrev����
	pLine = GetLine(nLines);
	if (pLine)
	{
		pItem = pLine->GetItem(0);
		if (pItem && (pItem->GetLinkStyle() & XItem::lsLinkPrev)) // ȷ����ǰһ�жϿ�����
			pItem->m_nLinkStyle &= ~XItem::lsLinkPrev; // ���ڲ����޸Ľӿڣ�ֻ������ǿ����
	}
	
	// ��nLines��������޸�����
	if (nParentIndex >= 0) // ˵������Ҫɾ������������ɾ���󣬺�����еĸ����������Ҫƫ��
	{
		for (int i=nLines; i<nLineCount; i++)
		{
			pLine = GetLine(i);
			if (pLine)
			{
				pLine->SetOffsetY(pLine->GetOffsetY() - nMaxLineOffset);
				int nItemCount = pLine->GetItemCount();
				for (int j=0; j<nItemCount; j++)
				{
					pItem = pLine->GetItem(j);
					if (pItem && (pItem->GetParent() != XItem::NotParent))
						pItem->SetParent(pItem->GetParent() - nParentIndex - 1);
				}
			}
		}
	}

	// ɾ���������
	for (int m=0; m<nLines; m++)
		GetLine(m)->Clear();
	m_vLines.erase(m_vLines.begin(), m_vLines.begin() + nLines);

	// ɾ������ĸ���Ŀ
	if (nParentIndex >= 0)
	{
		for (int i=0; i<=nParentIndex; i++)
			m_vSplitItems[i]->Release();
		m_vSplitItems.erase(m_vSplitItems.begin(), m_vSplitItems.begin() + nParentIndex + 1);
	}

	int nOldLine = (short)HIWORD(m_nMouseHover);
	int nOldItem = (short)LOWORD(m_nMouseHover);
	nOldLine -= nLines;
	if (nOldLine < -1)
		nOldLine = -1;
	m_nMouseHover = MAKELONG(nOldItem, nOldLine);

	AdjustVScroll();
	InvalidRect();
}
//------------------------------------------------------------------------
// ���һ����Ŀ
BOOL XItemContainer::AddItem(XItem* pItem, BOOL bNewLine, int nAlign, int nLineSpace)
{
	if (pItem == NULL || !pItem->IsValid())
		return FALSE;

	// �����Ҫ���ȹ���һ���нṹ
	XLine* pLine = NULL;
	if (bNewLine)
		pLine = NewLine(nAlign, nLineSpace);
	else
	{
		if (m_vLines.empty())
		{
			bNewLine = TRUE;
			pLine = NewLine(nAlign, nLineSpace);
		}
		else
			pLine = GetLine(GetLineCount() - 1);
	}
	int nRemainWidth = GetMaxValidWidth() - pLine->GetWidth();
	
	// ������Ŀ����
	int itemWidth = pItem->GetWidth();
	int itemHeight = pItem->GetHeight();

	if (m_bLineWrap)
	{
		if (itemWidth > nRemainWidth) // �������ɲ��±���Ŀ
		{
			if (pItem->CanSplit()) // ��Ŀ֧�ַָ�
			{
				int nParent = m_vSplitItems.size();
				XItem* pSrcItem = pItem;
				XItem* pItem1, *pItem2;
				int nSplitCount = 0;
				while (pItem->GetWidth() > nRemainWidth)
				{
					pItem->SplitItem(nRemainWidth, pItem1, pItem2);
					if (pItem1) // ���pItem1==NULL,��˵����һ�е�ʣ����û�������±���Ŀ���κβ��֣�����Ŀֱ�Ӽ�����һ��
					{
						pItem1->SetLinkStyle(XItem::lsLinkNext);
						pItem1->SetParent(nParent);
						pLine->AddItem(pItem1);
						pItem2->SetLinkStyle(XItem::lsLinkPrev);
						pItem2->SetParent(nParent);
						nSplitCount ++;
						if (nSplitCount > 1)
							pItem->Release();
					}
					else // ʵ����pItem2��ͬ��pItem�Ŀ���
					{
						//pItem->Release();
						//pSrcItem = pItem2;
					}
					pLine = NewLine(XLine::LeftAlign);
					nRemainWidth = GetMaxValidWidth() - pLine->GetWidth();
					pItem = pItem2;
				} // while
				pLine->AddItem(pItem);
				if (pItem->GetParent() != XItem::NotParent)
					m_vSplitItems.push_back(pSrcItem);
				else
				{
					if (nSplitCount > 0)
						pSrcItem->Release();
				}
			}
			else // ��Ŀ��֧�ַָ�,��Ҫ�ŵ���һ��
			{
				if (bNewLine) // �½�һ�ж��ݲ��£���ô���"ͼƬ"!!!???
				{
					OutputDebugString("item cannot split and item is too large!\r\n");
					m_vLines.erase(m_vLines.end() - 1);	// ����ɾ������½���һ��
					return FALSE;
				}
				else // ��Ϊ��ǰ���һ�зŲ��£���֮����ͼƬ���ָܷ�����½�һ������
				{
					pLine = NewLine(nAlign, nLineSpace);
					pLine->AddItem(pItem);
				}
			}
		}
		else // �ܹ�������(�Ƿ��½�һ�ж��п���)
		{
			pLine->AddItem(pItem);
		}
	}
	else // û���Զ����еĴ���
	{
		pLine->AddItem(pItem);
		int nLineWidth = pLine->GetWidth();
		if (nLineWidth > GetMaxValidWidth())
			SetMaxWidth(nLineWidth);
	}

	// ������������߼��߶�
	m_nMaxHeight = pLine->GetOffsetY() + pLine->GetHeight() + pLine->GetLineSpace();
	
	AdjustHScroll();
	AdjustVScroll();
	InvalidRect();
	return TRUE;
}
//------------------------------------------------------------------------
// �����ж��룬�õ�ÿ�������߽����Ծ���(>0)
int XItemContainer::GetAlignOffset(XLine* pLine)
{
	int dx = 0;
	if (pLine->GetAlign() == XLine::RightAlign)
		dx = m_nMaxWidth - pLine->GetWidth();
	else if (pLine->GetAlign() == XLine::CenterAlign)
		dx = (m_nMaxWidth - pLine->GetWidth()) / 2;
	return dx;
}
//------------------------------------------------------------------------
void XItemContainer::OnDraw(ICanvas* pCanvas, RECT& rcClip)
{
	XLine* pLine = NULL;
	POINT ptOrigin; // ����ԭ��ľ�������
	ptOrigin.x = m_rcWnd.left + m_ptOffset.x + m_nMargin;
	ptOrigin.y = m_rcWnd.top + m_ptOffset.y + m_nMargin;
	int nLineCount = GetLineCount();
	RECT rc;
	CopyRect(&rc, &rcClip);
	InflateRect(&rc, -m_nMargin, -m_nMargin);
	for (int i=0; i<nLineCount; i++)
	{
		pLine = GetLine(i);
		ASSERT(pLine);
		if (ptOrigin.y + pLine->GetOffsetY() + pLine->GetHeight() < rc.top) // ǰ�治�ɼ�����
			continue;
		
		// �ɼ�����
		try{
		pLine->OnDraw(pCanvas, ptOrigin.x + GetAlignOffset(pLine), 
			ptOrigin.y + pLine->GetOffsetY(), rc, m_nAlpha);}
		catch (...) {
			int j=0;
		}
		
		if (ptOrigin.y + pLine->GetOffsetY()/* + pLine->GetHeight()*/ >= rc.bottom) // ���治�ɼ�����
			break;
	}
}
//------------------------------------------------------------------------
DWORD XItemContainer::OnMouseMove(UINT nButton, UINT nShift, POINT pt)
{
	int nHitLine = -1;
	int nLineCount = GetLineCount();
	XLine* pLine = NULL;
	int yOff = pt.y - m_rcWnd.top - m_ptOffset.y - m_nMargin;
	for (int i=0; i<nLineCount; i++)
	{
		pLine = GetLine(i);
		if (yOff < pLine->GetOffsetY() + pLine->GetHeight())
		{
			nHitLine = i;
			break;
		}
	}

	int xOff = pt.x - m_rcWnd.left - m_ptOffset.x - m_nMargin;
	int nItem = GetItemByOffset(nHitLine, xOff);
	int nHover = MAKELONG(nItem, nHitLine);
	if (m_nMouseHover != nHover)
		OnItemChanged(nHitLine, nItem);
	
	return 0;
}
//------------------------------------------------------------------------
/*
DWORD XItemContainer::OnMouseDown(UINT nButton, UINT nShift, POINT pt)
{
	if (nButton == LeftButton)
	{
		int nLine = (short)HIWORD(m_nMouseHover);
		int nItem = (short)LOWORD(m_nMouseHover);
		if (nLine != -1 && nItem != -1)
		{
			XLine* pLine = GetLine(nLine);
			ASSERT(pLine);
			XItem* pItem = pLine->GetItem(nItem);
			ASSERT(pItem);
			if (pItem->GetParent() != XItem::NotParent) // ����Ǳ��ָ����Ŀ������ø���Ŀ������¼�
				m_vSplitItems[pItem->GetParent()]->OnMouseDown(nButton, nShift, pt);
			if (pItem && (pItem->GetLinkStyle() & XItem::lsNoLink))
				pItem->OnMouseDown(nButton, nShift, pt);
			else
			{
				XLine* pLine1 = pLine;
				XItem* pItem1 = pItem;
				int nLine1 = nLine;
				int nItem1 = nItem;
				while (pItem1->GetLinkStyle() & XItem::lsLinkPrev)
				{
					// ��һ��
					if (nItem1 <= 0) // ת����һ��
					{
						pLine1 = GetLine(--nLine1);
						nItem1 = pLine1->GetItemCount() - 1;
					}
					else
						nItem1--;
					pItem1 = pLine1->GetItem(nItem1);
					pItem1->OnMouseDown(nButton, nShift, pt);
				}
				
				
				pLine1 = pLine;
				pItem1 = pItem;
				nLine1 = nLine;
				nItem1 = nItem;
				while (pItem1->GetLinkStyle() & XItem::lsLinkNext)
				{
					// ��һ��
					if (nItem1 >= pLine1->GetItemCount()-1) // ת����һ��
					{
						pLine1 = GetLine(++nLine1);
						nItem1 = 0;
					}
					else
						nItem1++;
					pItem1 = pLine1->GetItem(nItem1);
					pItem1->OnMouseDown(nButton, nShift, pt);
				}
				pItem->OnMouseDown(nButton, nShift, pt);
			}
		}
	}
	
	return XWindow::OnMouseDown(nButton, nShift, pt);
}*/
//------------------------------------------------------------------------
DWORD XItemContainer::OnMouseUp(UINT nButton, UINT nShift, POINT pt)
{
	if (nButton == LeftButton || nButton == RightButton)
	{
		int nLine = (short)HIWORD(m_nMouseHover);
		int nItem = (short)LOWORD(m_nMouseHover);
		if (nLine >= 0 && nItem >= 0)
		{
			XLine* pLine = GetLine(nLine);
			if (pLine)
			{
				XItem* pItem = pLine->GetItem(nItem);
				if (pItem)
				{
					if (pItem->GetParent() != XItem::NotParent) // ����Ǳ��ָ����Ŀ������ø���Ŀ������¼�
					{
						XLine* pLine1 = pLine;
						XItem* pItem1 = pItem;
						int nLine1 = nLine;
						int nItem1 = nItem;
						int nLineCount = GetLineCount();

						while (pItem1->GetLinkStyle() & XItem::lsLinkPrev)
						{
							// ��һ��
							if (nItem1 <= 0) // ת����һ��
							{
								if (--nLine1 < 0)
									break;
								pLine1 = GetLine(nLine1);
								nItem1 = pLine1->GetItemCount() - 1;
							}
							else
								nItem1--;
							pItem1 = pLine1->GetItem(nItem1);
							pItem1->OnMouseUp(nButton, nShift, pt);
						}
						
						
						pLine1 = pLine;
						pItem1 = pItem;
						nLine1 = nLine;
						nItem1 = nItem;
						while (pItem1->GetLinkStyle() & XItem::lsLinkNext)
						{
							// ��һ��
							if (nItem1 >= pLine1->GetItemCount()-1) // ת����һ��
							{
								if (++nLine1 >= nLineCount)
									break;
								pLine1 = GetLine(nLine1);
								nItem1 = 0;
							}
							else
								nItem1++;
							pItem1 = pLine1->GetItem(nItem1);
							pItem1->OnMouseUp(nButton, nShift, pt);
						}

						m_vSplitItems[pItem->GetParent()]->OnMouseUp(nButton, nShift, pt);
						return XWindow::OnMouseUp(nButton, nShift, pt);
					}
					pItem->OnMouseUp(nButton, nShift, pt);					
				}
			}
		}
	}
	return XWindow::OnMouseUp(nButton, nShift, pt);
}
//------------------------------------------------------------------------
DWORD XItemContainer::OnMouseWheel(UINT nButton, UINT nShift, short zDelta, POINT pt)
{
	if (m_pVScrollBar && m_pVScrollBar->IsEnabled())
	{
		if (zDelta < 0)
		{
			zDelta = -zDelta;
			while (zDelta--)
				m_pVScrollBar->LineRightDown();
		}
		else
		{
			while (zDelta--)
				m_pVScrollBar->LineLeftUp();
		}
		return 0;
	}

	return XWindow::OnMouseWheel(nButton, nShift, zDelta, pt);
}
//------------------------------------------------------------------------
DWORD XItemContainer::OnKeyDown(UINT nKeyCode, UINT nShift)
{
	if (m_pVScrollBar)
		return m_pVScrollBar->OnKeyDown(nKeyCode, nShift);
	return NOTHANDLE;
}
//------------------------------------------------------------------------
void XItemContainer::OnItemChanged(int nLine, int nItem)
{
	int nOldLine = (short)HIWORD(m_nMouseHover);
	int nOldItem = (short)LOWORD(m_nMouseHover);

	int nLineCount = GetLineCount();

	// ��ǰ��item����exit����
	if (nOldLine >= 0 && nOldItem >= 0)
	{
		XLine* pLine = GetLine(nOldLine);
		if (pLine)
		{
			XItem* pItem = pLine->GetItem(nOldItem);
			if (pItem)
			{
				if (pItem->GetLinkStyle() == XItem::lsNoLink)
					pItem->OnMouseExit();
				else
				{
					XLine* pLine1 = pLine;
					XItem* pItem1 = pItem;
					int nLine1 = nOldLine;
					int nItem1 = nOldItem;
					while (pItem1->GetLinkStyle() & XItem::lsLinkPrev)
					{
						// ��һ��
						if (nItem1 <= 0) // ת����һ��
						{
							if (--nLine1 < 0)
								break;
							pLine1 = GetLine(nLine1);
							nItem1 = pLine1->GetItemCount() - 1;
						}
						else
							nItem1--;
						pItem1 = pLine1->GetItem(nItem1);
						pItem1->OnMouseExit();
					}

					pItem->OnMouseExit();
					
					pLine1 = pLine;
					pItem1 = pItem;
					nLine1 = nOldLine;
					nItem1 = nOldItem;
					while (pItem1->GetLinkStyle() & XItem::lsLinkNext)
					{
						// ��һ��
						if (nItem1 >= pLine1->GetItemCount()-1) // ת����һ��
						{
							if (++nLine1 >= nLineCount)
								break;
							pLine1 = GetLine(nLine1);
							nItem1 = 0;
						}
						else
							nItem1++;
						pItem1 = pLine1->GetItem(nItem1);
						pItem1->OnMouseExit();
					}
				}
			}
		}
	}

	if (nLine <= -1 || nItem <= -1)
	{
		SetCursor(CT_ARROW);
		m_nMouseHover = MAKELONG(nItem, nLine);
		return;
	}

	// ���ڵ�item����enter����
	if (nLine >= 0 && nItem >= 0)
	{
		XLine* pLine = GetLine(nLine);
		if (pLine)
		{
			XItem* pItem = pLine->GetItem(nItem);
			if (pItem)
			{
				if (pItem->GetLinkStyle() == XItem::lsNoLink)
				{
					pItem->OnMouseEnter();
					SetCursor(pItem->GetCursorType());
				}
				else
				{
					XLine* pLine1 = pLine;
					XItem* pItem1 = pItem;
					int nLine1 = nLine;
					int nItem1 = nItem;
					while (pItem1->GetLinkStyle() & XItem::lsLinkPrev)
					{
						// ��һ��
						if (nItem1 <= 0) // ת����һ��
						{
							if (--nLine1 < 0)
								break;
							pLine1 = GetLine(nLine1);
							nItem1 = pLine1->GetItemCount() - 1;
						}
						else
							nItem1--;
						pItem1 = pLine1->GetItem(nItem1);
						pItem1->OnMouseEnter();
					}
					
					pItem->OnMouseEnter();
					SetCursor(pItem->GetCursorType());
					
					pLine1 = pLine;
					pItem1 = pItem;
					nLine1 = nLine;
					nItem1 = nItem;
					while (pItem1->GetLinkStyle() & XItem::lsLinkNext)
					{
						// ��һ��
						if (nItem1 >= pLine1->GetItemCount()-1) // ת����һ��
						{
							if (++nLine1 >= nLineCount)
								break;
							pLine1 = GetLine(nLine1);
							nItem1 = 0;
						}
						else
							nItem1++;
						pItem1 = pLine1->GetItem(nItem1);
						pItem1->OnMouseEnter();
					}
				}
			}
		}
	}
	
	m_nMouseHover = MAKELONG(nItem, nLine);
}
//------------------------------------------------------------------------
// ���pItem==NULL����pLine����һ����Ϊ��
void XItemContainer::SetDirty(XLine* pLine, XItem* pItem)
{
	if (!pLine)
		return;

	RECT rc, rcClip;
	if (pItem)
	{
		rc.left = pItem->GetOffsetX();
		rc.right = rc.left + pItem->GetWidth();
		rc.top = pLine->GetOffsetY() + pLine->GetHeight() - pItem->GetHeight();
		rc.bottom = rc.top + pItem->GetHeight();
	}
	else
	{
		rc.left = 0;
		rc.right = rc.left + pLine->GetWidth();
		rc.top = pLine->GetOffsetY();
		rc.bottom = rc.top + pLine->GetHeight();
	}

	int dx = m_rcWnd.left + m_ptOffset.x + m_nMargin + GetAlignOffset(pLine);
	int dy = m_rcWnd.top + m_ptOffset.y + m_nMargin;
	OffsetRect(&rc, dx, dy);
	CopyRect(&rcClip, &m_rcWnd);
	InflateRect(&rcClip, -m_nMargin, -m_nMargin);
	IntersectRect(&rcClip, &rcClip, &rc);
	InvalidRect(&rcClip);
}
//------------------------------------------------------------------------
void XItemContainer::SetHScrollBar(XScrollBar* pBar)
{
	m_pHScrollBar = pBar;
	if (m_pHScrollBar)
	{
		m_pHScrollBar = static_cast<XScrollBar*>(pBar);
		CONN_posChanged(m_pHScrollBar, XItemContainer, OnHScrollPosChanged);
		m_pHScrollBar->Refresh();
	}
}
//------------------------------------------------------------------------
void XItemContainer::SetVScrollBar(XScrollBar* pBar)
{
	m_pVScrollBar = pBar;
	if (m_pVScrollBar)
	{
		m_pVScrollBar = static_cast<XScrollBar*>(pBar);
		CONN_posChanged(m_pVScrollBar, XItemContainer, OnVScrollPosChanged);
		m_pVScrollBar->Refresh();
	}
}
//------------------------------------------------------------------------
void XItemContainer::OnHScrollPosChanged(DWORD dwParam)
{
	m_ptOffset.x = -*(int*)dwParam;
	InvalidRect();
}
//------------------------------------------------------------------------
void XItemContainer::OnVScrollPosChanged(DWORD dwParam)
{
	m_ptOffset.y = -*(int*)dwParam;
	InvalidRect();
}
//------------------------------------------------------------------------
void XItemContainer::AdjustHScroll()
{
	if (m_pHScrollBar)
	{
		int nLimitedWidth = GetWidth() - 2*GetMargin();
		
		// �һ�е�x��ƫ��
		int nNeedScrollValue = GetMaxValidWidth() - nLimitedWidth;
		if (nNeedScrollValue > 0)
		{
			//m_pHScrollBar->SetEnabled(TRUE);
			m_pHScrollBar->SetVisible(TRUE);
			m_pHScrollBar->SetVisibleCount(nLimitedWidth);
			m_pHScrollBar->SetScrollRange(0, nNeedScrollValue, TRUE);
			m_pHScrollBar->Refresh();
		}
		else
		{
			//m_pHScrollBar->SetEnabled(FALSE);
			m_pHScrollBar->SetVisible(FALSE);
			m_pHScrollBar->SetScrollRange(0, 0);
			m_pHScrollBar->SetVisibleCount(0);
			m_pHScrollBar->SetScrollPos(0);
			m_pHScrollBar->Refresh();
		}
	}
}
//------------------------------------------------------------------------
void XItemContainer::AdjustVScroll()
{
	if (GetLineCount() <= 0)
	{
		if (m_pVScrollBar)
		{
			m_pVScrollBar->SetVisible(FALSE);
			m_pVScrollBar->SetScrollRange(0, 0);
			m_pVScrollBar->SetVisibleCount(0);
			m_pVScrollBar->SetScrollPos(0);
			m_pVScrollBar->Refresh();
		}
		m_ptOffset.y = 0;
		return;
	}
	else
	{
		int nLimitedHeight = GetHeight() - 2*GetMargin();
		// ���һ�е�y��ƫ��
		XLine* pLastLine = GetLine(GetLineCount() - 1);
		int nOffy = pLastLine->GetOffsetY() + pLastLine->GetHeight() + pLastLine->GetLineSpace();
		int nNeedScrollValue = nOffy - nLimitedHeight;
		if (m_pVScrollBar)
		{
			if (nNeedScrollValue > 0)
			{
				m_pVScrollBar->SetVisible(TRUE);
				m_pVScrollBar->SetVisibleCount(nLimitedHeight);
				m_pVScrollBar->SetScrollRange(0, nNeedScrollValue, TRUE);
				m_pVScrollBar->Refresh();
			}
			else // �������滹û��������Ч����
			{
				m_pVScrollBar->SetVisible(FALSE);
				m_ptOffset.y = 0;
			}
		}
		if (-m_ptOffset.y > nNeedScrollValue && nNeedScrollValue > 0)
		{
			if (m_pVScrollBar)
				m_pVScrollBar->SetScrollPos(nNeedScrollValue);
			else
				OnVScrollPosChanged((DWORD)&nNeedScrollValue);
		}
	}
}
//------------------------------------------------------------------------
void XItemContainer::SetWindowPos(RECT& rc)
{
	XWindow::SetWindowPos(rc);
	SetMaxWidth(rc.right - rc.left - 2*m_nMargin);
	SetMaxHeight(rc.bottom - rc.top - 2*m_nMargin);
	AdjustHScroll();
	AdjustVScroll();
}
//------------------------------------------------------------------------
void GetColorString(char* color_buf, WPixel color);
BOOL XItemContainer::SaveToHtml(LPCSTR szFileName, LPCSTR szTitle, WPixel backcolor)
{
	char buf_head[] = 
		"<html>\r\n\r\n"
		"<head>\r\n"
		"<meta http-equiv=\"Content-Language\" content=\"zh-cn\">\r\n"
		"<meta name=\"GENERATOR\" content=\"Microsoft FrontPage 5.0\">\r\n<meta name=\"ProgId\" content=\"FrontPage.Editor.Document\">\r\n"
		"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">\r\n";
	char buf_head1[512];
	char color[8];
	GetColorString(color, backcolor);
	wsprintf(buf_head1, "<title>%s</title>\r\n</head>\r\n\r\n<body bgcolor=\"#%s\">\r\n",
		((szTitle)?szTitle:"�����¼"), color);
	char buf_trail[] = "\r\n</body>\r\n\r\n</html>";

	if (!szFileName || *szFileName == '\0')
		return FALSE;
	FILE* hFile = fopen(szFileName, "w");
	if (!hFile)
		return FALSE;
	fwrite(buf_head, sizeof(buf_head), 1, hFile);
	fwrite(buf_head1, strlen(buf_head1), 1, hFile);

	int nLineCount = GetLineCount();
	for (int i=0; i<nLineCount; i++)
	{
		XLine* pLine = GetLine(i);
		if (pLine)
			pLine->SaveToHtml(hFile);
	}
	
	fwrite(buf_trail, sizeof(buf_trail), 1, hFile);
	fclose(hFile);
	return TRUE;
}
//------------------------------------------------------------------------