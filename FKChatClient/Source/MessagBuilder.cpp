/**
*	created:		2012-6-28   8:57
*	filename: 		MessagBuilder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ��������Ϣ������
*/
//------------------------------------------------------------------------
#include "../Include/MessageBuilder.h"
//------------------------------------------------------------------------
CMessageBuilder::CMessageBuilder(void)
: m_pContainer(NULL)
{
}
//------------------------------------------------------------------------
CMessageBuilder::~CMessageBuilder(void)
{
}

//-------------------------------------------------------------------
// ��  ����������Ϣ���� 
//-------------------------------------------------------------------
void CMessageBuilder::SetContainer(XItemContainer * pContainer)
{
	m_pContainer = pContainer;
}

//-------------------------------------------------------------------
// ��  ���� ������������
//-------------------------------------------------------------------
BOOL CMessageBuilder::Build(LPCSTR lpszText)
{
	// ����Ҫ��ʾ������
	CHtmlNode * pRootNode = m_chatParser.Parse(lpszText);
	if (pRootNode == NULL)
	{
		return FALSE;
	}

	// ��������ӵ���ʾ����
	BOOL bRet = AddToContainer(pRootNode);

	delete pRootNode;	
	return bRet;
}

//-------------------------------------------------------------------
// ��  ��������Ϣ������ӵ���ʾ���� 
//-------------------------------------------------------------------
BOOL CMessageBuilder::AddToContainer(const CHtmlNode * pContent)
{
	string strNodeName;
	XItem * pItem = NULL;
	BOOL bNewline = FALSE;
	
	// �������еĽڵ�
	CHtmlNode::NodeList children = pContent->GetChildren();
	CHtmlNode::NodeList::const_iterator itr = children.begin();
	for (; itr != children.end(); itr++)
	{
		strNodeName = (*itr)->GetName();
		
		if (strNodeName == "text") // �ı�
		{			
			pItem = m_itemFactory.CreateTextItem(static_cast<void *>(*itr));
		}
		else if (strNodeName == "a") // ����
		{
			pItem = m_itemFactory.CreateLinkItem(static_cast<void *>(*itr));			
		}
		else if (strNodeName == "br") // ����
		{
			pItem = m_itemFactory.CreateNewlineItem(static_cast<void *>(*itr));
			bNewline = TRUE;
		}
		else if (strNodeName == "img") // ͼƬ
		{
			pItem = m_itemFactory.CreateImageItem(static_cast<void *>(*itr));			
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

//-------------------------------------------------------------------
// ��  ���������ʾԪ�� 
//-------------------------------------------------------------------
BOOL CMessageBuilder::AddItem(XItem * pItem, BOOL bNewline)
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
//------------------------------------------------------------------------