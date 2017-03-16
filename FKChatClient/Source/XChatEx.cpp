/**
*	created:		2012-6-28   12:23
*	filename: 		XChatEx
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ��������Ϣ����չ������λ�øı�ȵ�����
*/
//------------------------------------------------------------------------
#include "../Include/XChatEx.h"
//------------------------------------------------------------------------
XChatEx::XChatEx(XWindow * pParent)
: XChat(pParent)
, m_pItemFactory(NULL)
{
	m_nAlpha = 0;
	m_bAutoSroll = TRUE;

	if (m_pVScrollBar != NULL)
	{
		//CONN_posChanged(m_pVScrollBar, XChatEx, OnVScrollBarChanged);
	}	
}
//------------------------------------------------------------------------
XChatEx::~XChatEx(void)
{
}
//------------------------------------------------------------------------
void XChatEx::SetWindowPos(RECT& rc)
{
	XWindow::SetWindowPos(rc);

	if (m_pVScrollBar != NULL)
	{	
		m_pVScrollBar->SetWindowPos(m_rcScrollBar);
		m_pVScrollBar->SetScrollPos(m_pVScrollBar->GetMaxRange(), FALSE);
		m_pVScrollBar->Refresh();
	}
	
	if (m_pContainer != NULL)
	{
		m_pContainer->SetWindowPos(m_rcContainer);
	}
}
//------------------------------------------------------------------------
void XChatEx::SetWindowPos(RECT & rcScrollBar, RECT & rcContainer)
{
	m_rcScrollBar = rcScrollBar;
	m_rcContainer = rcContainer;

	// ȷ�������ͻ��˵�����
	RECT rcClient;
	CopyRect(&rcClient, &rcContainer);
	rcClient.left = rcScrollBar.left;

	SetWindowPos(rcClient);
}

//-------------------------------------------------------------------
// ��  ���������ʾԪ�� 
// ��  ����[����] XItem * pItem - Ҫ��ʾ����ĿԪ��
// ��  ����[����] BOOL bNewline - �Ƿ��к������
// ����ֵ��BOOL - ������ӷ���TRUE�����򷵻�FALSE
//-------------------------------------------------------------------
BOOL XChatEx::AddItem(XItem * pItem, BOOL bNewline)
{
	if (pItem == NULL)
	{
		return FALSE;
	}

	if (!m_pContainer->AddItem(pItem, bNewline))
	{
		delete pItem;
		return FALSE;
	}

	return TRUE;
}

//-------------------------------------------------------------------
// ��  ������ʾ������Ϣ 
// ��  ����[����] const CHtmlNode * pContent - ��Ϣ����
// ����ֵ��BOOL - ������ʾ����TRUE,���򷵻�FALSE
//-------------------------------------------------------------------
BOOL XChatEx::DisplayMessage(const CHtmlNode * pContent)
{
	string strNodeName;
	XItem * pItem = NULL;
	BOOL bNewline = TRUE;

	if (m_pItemFactory == NULL)
	{
		return FALSE;
	}
	
	// ȷ���Ƿ�Ҫ�Զ�����
	int nPos = m_pVScrollBar->GetScrollPos();
	if (nPos == m_pVScrollBar->GetMaxRange())
	{
		m_bAutoSroll = TRUE;
	}
	else
	{
		m_bAutoSroll = FALSE;
	}

	CHtmlNode::NodeList children = pContent->GetChildren();
	CHtmlNode::NodeList::const_iterator itr = children.begin();
	for (; itr != children.end(); itr++)
	{
		strNodeName = (*itr)->GetName();
		
		if (strNodeName == "text") // �ı�
		{			
			pItem = m_pItemFactory->CreateTextItem(static_cast<void *>(*itr));
		}
		else if (strNodeName == "a") // ����
		{
			pItem = m_pItemFactory->CreateLinkItem2(static_cast<void *>(*itr));			
		}
		else if (strNodeName == "br") // ����
		{
			pItem = m_pItemFactory->CreateNewlineItem(static_cast<void *>(*itr));
			bNewline = TRUE;
		}
		else if (strNodeName == "img") // ͼƬ
		{
			pItem = m_pItemFactory->CreateImageItem(static_cast<void *>(*itr));			
		}else if(strNodeName == "goods") // װ��
		{
            pItem =  m_pItemFactory->CreateLinkItem2(static_cast<void *>(*itr));	
		}
		else
		{
			return FALSE;
		}
		
		if (AddItem(pItem, bNewline))
		{
			bNewline = FALSE;
		}
		else
		{
			return FALSE;
		}
	} 
	return TRUE;
}
//------------------------------------------------------------------------
void XChatEx::SetItemFactory(IItemFactory * pFactory)
{
	m_pItemFactory = pFactory;
}

//-------------------------------------------------------------------
// ��  ��������ǲ������������������й���
//-------------------------------------------------------------------
void XChatEx::AutoScroll(void)
{
	if (m_bAutoSroll)
	{	
		m_pVScrollBar->SetScrollPos(m_pVScrollBar->GetMaxRange());
		m_pParent->OnMessage(0, FALSE, 0);
	}
	else
	{
		m_pParent->OnMessage(0, TRUE, 0);
	}
}
//------------------------------------------------------------------------
RECT XChatEx::GetContainerRect(void) const
{
	return m_rcContainer;
}
//------------------------------------------------------------------------
RECT XChatEx::GetScrollBarRect(void) const
{
	return m_rcScrollBar;
}
//------------------------------------------------------------------------
void XChatEx::OnVScrollBarChanged(DWORD dwParam)
{
	int nPos = *((int *)dwParam);
	if (nPos == m_pVScrollBar->GetMaxRange())
	{
		// ֹͣ��ʾ
		m_pParent->OnMessage(0, FALSE, 0);
	}
}
//------------------------------------------------------------------------