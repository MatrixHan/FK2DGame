/**
*	created:		2012-6-28   7:16
*	filename: 		ChatMenu
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/ChatMenu.h"
#include "../Include/WindowCreator.h"
#include "../../FKResManager/IFKResManager.h"
//------------------------------------------------------------------------
CChatMenuItem::CChatMenuItem(int nId, string strName,BOOL bEnabled)
: m_nId(nId)
, m_strName(strName)
, m_bEnabled(bEnabled)
{

}
//------------------------------------------------------------------------
CChatMenu::CChatMenu(void)
: m_pFrame(NULL)
, m_pEventReceiver(NULL)
, m_nSelected(-1)
{
	m_clrSelected = WPixel(100, 100, 0);
	m_clrText = WPixel(255, 245, 200);
}
//------------------------------------------------------------------------
CChatMenu::~CChatMenu(void)
{
	RemoveAll();
}

//-------------------------------------------------------------------
// ��  ���������˵����� 
//-------------------------------------------------------------------
BOOL CChatMenu::Create(IResObject * pResObject)
{
	int i = 0;

	if (pResObject == NULL)
	{
		return FALSE;
	}

	// �����˵���������
	if (!CWindowCreator::CreateMainWnd(NULL, this, pResObject))
	{
		return FALSE;
	}

	// �������ڵĿ��
	m_pFrame = new XEdit();
	if (!CWindowCreator::CreateControl(this, m_pFrame, pResObject->GetSubObjectByIndex(i++)))
	{
		return FALSE;
	}
	m_pFrame->SetWindowPos(m_rcWnd);

	SetVisible(FALSE);	
	return TRUE;
}
//------------------------------------------------------------------------
void CChatMenu::OnActivate(BOOL bActive)
{
	if (!bActive)
	{
		SetVisible(FALSE);		
	}
	else
	{
		SetVisible(TRUE);		
	}
}

//-------------------------------------------------------------------
// ��  ���������˵� 
//-------------------------------------------------------------------
void CChatMenu::DrawMenu(ICanvas * pCanvas, RECT & rcClip)
{
	RECT rc;
	int x = m_rcWnd.left;
	int y = m_rcWnd.top;

	CopyRect(&rc, &m_rcWnd);
	rc.left += 2;
	rc.right -= 2;

	// ���������˵���
	MenuItemListItr itr = m_items.begin();
	for (int i = 0; itr != m_items.end(); itr++, i++)
	{
		y = m_rcWnd.top + MENU_ITEM_HEIGHT * i;

		rc.top = y + 2;
		rc.bottom = rc.top + MENU_ITEM_HEIGHT;

		// ����ѡ�б�־
		if (i == m_nSelected)
		{
			pCanvas->DrawBox(rc, m_clrSelected);
		}

		// ��ȡ�ַ����ĳ���
		ITTFont * pFont = GetFont();
		if (pFont == NULL)
		{
			return;
		}				
		int nLen = pFont->GetLength((*itr)->m_strName.c_str());

		// �����˵���
		int nOffset = (rc.right - rc.left - nLen) / 2;
		// �������
		if ((*itr)->m_bEnabled)
		{
			pCanvas->DrawClipText(pFont, x + nOffset, y + 4, (*itr)->m_strName.c_str(), m_clrText, rcClip, FONT_NOALPHA);
		}else{
			pCanvas->DrawClipText(pFont, x + nOffset, y + 4, (*itr)->m_strName.c_str(), WPixel(128,128,128), rcClip, FONT_NOALPHA);
		}

	}
}
//------------------------------------------------------------------------
void CChatMenu::OnPaint(ICanvas * pCanvas)
{
	XMainWnd::OnPaint(pCanvas);

	if (IsVisible())
	{
		DrawMenu(pCanvas, m_rcWnd);
	}	
}
//------------------------------------------------------------------------
DWORD CChatMenu::OnMouseMove(UINT nButton, UINT nShift, POINT pt)
{
	// ����ѡ����
	int nIndex = (pt.y - m_rcWnd.top) / MENU_ITEM_HEIGHT;	

	if (nIndex < m_items.size())
	{
		m_nSelected = nIndex;
	}	

	return 0;	
}
//------------------------------------------------------------------------
DWORD CChatMenu::OnMouseDown(UINT nButton, UINT nShift, POINT pt)
{	
	SetVisible(FALSE);

	// ����ѡ����
	int nClicked = (pt.y - m_rcWnd.top) / MENU_ITEM_HEIGHT;	

	if (nClicked < m_items.size())
	{
		m_nSelected = nClicked;		

		if (m_pEventReceiver != NULL)
		{
			MenuItemListItr itr = m_items.begin();
			advance(itr, m_nSelected);
			// �������
			if((*itr)->m_bEnabled)
			{
				m_pEventReceiver->Clicked((*itr)->m_nId);
			}

		}
	}	

	return 0;
}

//-------------------------------------------------------------------
// ��  ���������Ƿ�Ϊ͸��
//-------------------------------------------------------------------
BOOL CChatMenu::IsTransparent(POINT& pt)
{
	return FALSE;
}

//-------------------------------------------------------------------
// ��  ������ʾ�˵�
// ��  ����[����] const POINT * pPos - ��ʾλ��
// ��  ����[���] BOOL bShow - �Ƿ���ʾ
// ����ֵ��void
//-------------------------------------------------------------------
void CChatMenu::Show(const POINT * pPos, BOOL bShow)
{ 
	if (!bShow)
	{
		SetVisible(FALSE);
		return;
	}

	if (pPos != NULL)
	{
		int dx = pPos->x + 30 - m_rcWnd.left;
		int dy = pPos->y - m_rcWnd.top;

		MoveWindow(dx, dy);	
	}

	SetVisible(TRUE);
	SetActive();	
}

//-------------------------------------------------------------------
// ��  �������һ���˵���
// ��  ����[����] int nId - �˵���ID
// ��  ����[����] string strName - �˵�������
// ��  ����[����] BOOL bEnabled - �Ƿ�Enabled
// ����ֵ��void
//-------------------------------------------------------------------
void CChatMenu::AddItem(int nId, string strName)
{
	CChatMenuItem * pItem = new CChatMenuItem(nId, strName,TRUE);
	if (pItem == NULL)
	{
		return;
	}

	m_items.push_back(pItem);

	m_rcWnd.bottom = m_rcWnd.top + m_items.size() * MENU_ITEM_HEIGHT + 4;
	m_pFrame->SetWindowPos(m_rcWnd);
}

//-------------------------------------------------------------------
// ��  �������ò˵��ļ���״̬
//-------------------------------------------------------------------
void CChatMenu::SetMenuItemEnable(int nMenuId,BOOL bEnabled)
{
	MenuItemListItr itr ;
	for(itr = m_items.begin(); itr != m_items.end();itr++)
	{
		if((*itr)->m_nId == nMenuId)
		{
			(*itr)->m_bEnabled = bEnabled;
			return;
		}
	}

}

//-------------------------------------------------------------------
// ��  �������ò˵��ļ���״̬
//-------------------------------------------------------------------
void CChatMenu::SetMenuItemsDisabled(const int nMenuIds[],int nCount)
{
	int i = 0;
	for(i=0 ;i<nCount ;i++)
	{
		SetMenuItemEnable(nMenuIds[i],FALSE);
	}
}

//-------------------------------------------------------------------
// ��  ��������IDɾ���˵��� 
//-------------------------------------------------------------------
void CChatMenu::RemoveItem(int nId)
{
	for (MenuItemListItr itr = m_items.begin(); itr != m_items.end(); itr++)
	{
		if ((*itr)->m_nId == nId)
		{
			delete *itr;
			m_items.erase(itr);

			break;
		}		
	}
}

//-------------------------------------------------------------------
// ��  ����ɾ�����еĲ˵��� 
//-------------------------------------------------------------------
void CChatMenu::RemoveAll(void)
{
	for (MenuItemListItr itr = m_items.begin(); itr != m_items.end(); itr++)
	{
		delete *itr;
	}
	m_items.clear();
}

//-------------------------------------------------------------------
// ��  �������ò˵����¼������� 
//-------------------------------------------------------------------
void CChatMenu::SetEventReceiver(IMenuEventReceiver * pReceiver)
{
	m_pEventReceiver = pReceiver;
}
//------------------------------------------------------------------------