/**
*	created:		2012-6-28   7:10
*	filename: 		ChatItemFactory
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������������ʾԪ��
*/
//------------------------------------------------------------------------
#include "../Include/ChatClient.h"
#include "../Include/ChatItemFactory.h"
#include "../Include/XItemInfo.h"
#include "../Include/XItemPlayer.h"
#include "../Include/XItemThing.h"
#include "../Include/XItemSentence.h"
#include <assert.h>
//------------------------------------------------------------------------
CChatItemFactory::CChatItemFactory(void)
{
}
//------------------------------------------------------------------------
CChatItemFactory::~CChatItemFactory(void)
{
}
//------------------------------------------------------------------------
XItem * CChatItemFactory::CreateTextItem(void * pNode)
{
	assert(pNode != NULL);
	
	ITTFont * pFont = NULL;
	WPixel wpFrontColor;
	WPixel wpBackColor;
	CHtmlNode * pTextNode = static_cast<CHtmlNode *>(pNode);
		
	GetFont(pTextNode, &pFont);
	GetColor(pTextNode, wpFrontColor, "frontcolor");
	if (GetColor(pTextNode, wpBackColor, "backcolor"))
	{
		// �����ı�����
		XItemInfo * pInfoItem = new XItemInfo();	
		if (pInfoItem == NULL)
		{		
			return NULL;
		}
		
		// �����ı�������
		pInfoItem->SetText(pTextNode->GetContent().c_str());
		pInfoItem->SetTextColor(wpFrontColor);
		pInfoItem->SetBackColor(wpBackColor);
		pInfoItem->SetFont(pFont);

		return static_cast<XItem *>(pInfoItem);
	}
	else
	{
		XItemText * pTextItem = new XItemText();
		if (pTextItem == NULL)
		{
			return NULL;
		}

		pTextItem->SetText(pTextNode->GetContent().c_str());
		pTextItem->SetTextColor(wpFrontColor);
		pTextItem->SetFont(pFont);

		return static_cast<XItem *>(pTextItem);
	}
}
//------------------------------------------------------------------------
XItem * CChatItemFactory::CreateLinkItem(void * pNode)
{
	assert(pNode != NULL);
	CHtmlNode * pLinkNode = static_cast<CHtmlNode *>(pNode);

	// ȷ�����ӵ�����
	string strType;
	int nType = -1;
	if (pLinkNode->GetAttribute("type", strType))
	{
		nType = atoi(strType.c_str());
	}	

	// ������Ҫ���͵�����
	switch (nType)
	{
	case LINK_PLAYER:
		return CreatePlayerLink(pLinkNode);
	case LINK_THING:
		return CreateThingLink(pLinkNode);
	case LINK_SENTENCE:
		return CreateSentenceLink(pLinkNode);
	default:
		return NULL;
	}
}
//------------------------------------------------------------------------
XItem * CChatItemFactory::CreateLinkItem2(void * pNode)
{
	assert(pNode != NULL);
	CHtmlNode * pLinkNode = static_cast<CHtmlNode *>(pNode);
	
	// ȷ�����ӵ�����
	string strType;
	int nType = -1;
	if (pLinkNode->GetAttribute("type", strType))
	{
		nType = atoi(strType.c_str());
	}	
	
	// ������Ҫ���͵�����
	switch (nType)
	{
	case LINK_PLAYER:
		return CreatePlayerLink(pLinkNode);
	case LINK_THING:
		return CreateThingLink(pLinkNode);
	case LINK_SENTENCE:
		return CreateSentenceLink2(pLinkNode);
	default:
		return NULL;
	}   
}
//------------------------------------------------------------------------
XItem * CChatItemFactory::CreateInputItem(void * pNode)
{
	return NULL;
}
//------------------------------------------------------------------------
XItem * CChatItemFactory::CreateImageItem(void * pNode)
{
	assert(pNode != NULL);

	CHtmlNode * pImageNode = static_cast<CHtmlNode *>(pNode);
	
	// ��ȡͼƬ·��	
	int nIndex;
	if (!pImageNode->GetAttribute("src", nIndex, 0))
	{
		return NULL;
	}

	// ����ͼƬ
	IBitmap * pImage = CChatClient::Instance()->m_controller.GetFace(nIndex);
	if (pImage == NULL)
	{
		return NULL;
	}

	// ��ȡͼƬ֡��
	int nCount = pImage->GetWidth() / pImage->GetHeight();
	if (nCount <= 0)
	{
		return NULL;
	}

	XItemImage* pImageItem = new XItemImage();
	pImageItem->SetBitmap(pImage);
	pImageItem->SetFrameCount(nCount);
	pImageItem->SetDelay(200);
	
	return static_cast<XItem *>(pImageItem);
}
//------------------------------------------------------------------------
XItem * CChatItemFactory::CreateNewlineItem(void * pNode)
{
	assert(pNode != NULL);
	
	// �����ı�����
	XItemInfo * pTextItem = new XItemInfo();	
	if (pTextItem == NULL)
	{		
		return NULL;
	}
	
	CHtmlNode * pTextNode = static_cast<CHtmlNode *>(pNode);
	
	ITTFont * pFont = NULL;
	WPixel wpColor;
	GetFont(pTextNode, &pFont);
	GetColor(pTextNode, wpColor);	
	
	// �����ı�������
	pTextItem->SetText("\r");
	pTextItem->SetTextColor(wpColor);
	pTextItem->SetFont(pFont);
	
	return static_cast<XItem *>(pTextItem);
}

//-----------------------------------------------------------------------------
// ��  ������ȡ��������
// ��  ����[����] const CHtmlExNode * pNode - �ڵ�ָ��
// ��  ����[���] ITTFont ** - ��ȡ��������
// ����ֵ��void
//-----------------------------------------------------------------------------
void CChatItemFactory::GetFont(const CHtmlNode *pNode, ITTFont **pFont)
{
	assert(pNode);
	assert(pFont);
	
	// Ĭ������Ϊ����
	string  strFontName = "����";
	if (!pNode->GetAttribute("font", strFontName))
	{
		strFontName = "����";
	}
	
	// Ĭ�������СΪ12
	int nFontSize = 12;
	string strFontSize;
	if (pNode->GetAttribute("size", strFontSize))
	{
		nFontSize = atoi(strFontSize.c_str());
	}
	
	// Ĭ�ϲ��Ǵ���
	BOOL bBold = FALSE;
	string strBold;
	if (pNode->GetAttribute("bold", strBold))
	{
		bBold = (atoi(strBold.c_str()) != 0);
	}
	
	// Ĭ�ϲ���б��
	BOOL bItalic = FALSE;
	string strItalic;
	if (pNode->GetAttribute("italic", strItalic))
	{
		bItalic = (atoi(strItalic.c_str()) != 0);
	}
	
	// Ĭ��û���»���
	BOOL bUnderline = FALSE;
	string strUnderline;
	if (pNode->GetAttribute("underline", strUnderline))
	{
		bUnderline = (atoi(strUnderline.c_str()) != 0);
	}
	
	// ��������
	*pFont = XFontCreate::GetInstance()->CreateFont(strFontName.c_str(), nFontSize, 
		bBold, bItalic, bUnderline, TRUE);
}

//-----------------------------------------------------------------------------
// ��  ������ȡ��ɫ����
// ��  ����[����] const CHtmlExNode * pNode - �ڵ�ָ��
// ��  ����[����] WPixel & wpColor - ��ȡ������ɫ
// ����ֵ��void
//-----------------------------------------------------------------------------
BOOL CChatItemFactory::GetColor(const CHtmlNode *pNode, WPixel &wpColor, string strAttribute)
{
	// Ĭ����ɫΪ��ɫ
	wpColor = WPixel(255, 255, 255);
	
	string strColor;
	if (pNode->GetAttribute(strAttribute, strColor))
	{
		int r, g, b;
		r = g = b = 255;
		sscanf(strColor.c_str(), "%d,%d,%d", &r, &g, &b );
		wpColor = WPixel(r, g, b);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//-------------------------------------------------------------------
// ��  ��������������� 
// ��  ����[����] CHtmlNode *pNode - ���������Ϣ�Ľڵ�
// ����ֵ��XItem * - �����ɹ��򷵻����Ӷ��󣬷��򷵻�NULL
//-------------------------------------------------------------------
XItem * CChatItemFactory::CreatePlayerLink(CHtmlNode *pNode)
{	
	XItemPlayer * pPlayerItem = new XItemPlayer();
	if (pPlayerItem == NULL)
	{
		return NULL;
	}	
	
	ITTFont * pFont = NULL;
	GetFont(pNode, &pFont);
	pPlayerItem->SetFont(pFont);
	pPlayerItem->SetTextColor(WPixel(0,252,255));
	
	// ��ȡ����·��
	string strTemp;
	if (pNode->GetAttribute("href", strTemp)) 
	{
		pPlayerItem->SetLinkText(strTemp.c_str());
	}
	
	// ������ز���		
	pPlayerItem->SetText(pNode->GetContent().c_str());
	pPlayerItem->SetVisited(FALSE);
	pPlayerItem->SetNotVisitedColor(WPixel(0,252,255));
	pPlayerItem->SetVisitedColor(WPixel(0,252,255));
	pPlayerItem->SetRolloverColor(WPixel(255,0,0));	
	
	return static_cast<XItem *>(pPlayerItem);
}
//------------------------------------------------------------------------
XItem * CChatItemFactory::CreateThingLink(CHtmlNode *pNode)
{
	XItemThing * pThingItem = new XItemThing();
	if (pThingItem == NULL)
	{
		return NULL;
	}	
	
	ITTFont * pFont = NULL;
	GetFont(pNode, &pFont);
	pThingItem->SetFont(pFont);
	// ������ز���		
	pThingItem->SetVisited(FALSE);
	pThingItem->SetGoods((IGoods *)pNode->GetData());	
	return static_cast<XItem *>(pThingItem);
}
//------------------------------------------------------------------------
XItem * CChatItemFactory::CreateSentenceLink2(CHtmlNode * pNode)
{
	WPixel wArray[] = {
		    WPixel(213,0,0),
			WPixel(218,212,231),
			WPixel(252,116,243),
			WPixel(81,204,249),
			WPixel(21,252,3),
			WPixel(251,242,3)
	};
	XItemSentence * pSentenceItem = new XItemSentence();
	if (pSentenceItem == NULL)
	{
		return NULL;
	}	
	
	ITTFont * pFont = NULL;
	GetFont(pNode, &pFont);
	pSentenceItem->SetFont(pFont);
	
	// ��ȡ��ɫ
	int nColor;
	if (pNode->GetAttribute("color", nColor, 0))
	{		
		if(nColor < 0 )
		{
			CHtmlNode *pParent = pNode->GetFirstParent();
			if( pParent == NULL)
			{
				pParent = pNode;
			}
			int nChannel = 1;
			if (pParent->GetAttribute("channel", nChannel, 0))
			{
				if( nChannel >= CHANNEL_SYSTEM && nChannel<= CHANNEL_WORLD)
				{
					pSentenceItem->SetTextColor(wArray[nChannel]);
				}
			}
		}else{
			WPixel clrText = CChatClient::Instance()->m_controller.GetColor(nColor);
			pSentenceItem->SetTextColor(clrText);       
		}
	}else{
		CHtmlNode *pParent = pNode->GetFirstParent();
		if( pParent == NULL)
		{
			pParent = pNode;
		}
		int nChannel = 1;
		if (pParent->GetAttribute("channel", nChannel, 0))
		{
			if( nChannel == CHANNEL_SYSTEM )
			{
				pSentenceItem->SetTextColor(wArray[CHANNEL_SYSTEM]);
				nColor = 10000;
			}
		}
        if( nColor <0 )
		{
			nColor = 1;
			WPixel clrText = CChatClient::Instance()->m_controller.GetColor(nColor);
			pSentenceItem->SetTextColor(clrText);
		}
	}
	// ��ȡ����·��
	string strTemp;
	if (pNode->GetAttribute("href", strTemp)) 
	{
		pSentenceItem->SetLinkText(strTemp.c_str());
	}
	
	// ������ز���		
	pSentenceItem->SetText(pNode->GetContent().c_str());
	pSentenceItem->SetVisited(FALSE);
	pSentenceItem->SetNotVisitedColor(pSentenceItem->GetTextColor());
	pSentenceItem->SetVisitedColor(pSentenceItem->GetTextColor());
	pSentenceItem->SetRolloverColor(WPixel(255,0,0));	
	
	return static_cast<XItem *>(pSentenceItem);	
}
//------------------------------------------------------------------------
XItem * CChatItemFactory::CreateSentenceLink(CHtmlNode *pNode)
{
	XItemSentence * pSentenceItem = new XItemSentence();
	if (pSentenceItem == NULL)
	{
		return NULL;
	}	
	
	ITTFont * pFont = NULL;
	GetFont(pNode, &pFont);
	pSentenceItem->SetFont(pFont);
	// ��ȡ��ɫ
	int nColor;
	if (pNode->GetAttribute("color", nColor, 0))
	{		
		if( nColor<0 )
		{
			nColor = 1;
		}
		WPixel clrText = CChatClient::Instance()->m_controller.GetColor(nColor);
		pSentenceItem->SetTextColor(clrText);
	}

	// ��ȡ����·��
	string strTemp;
	if (pNode->GetAttribute("href", strTemp)) 
	{
		pSentenceItem->SetLinkText(strTemp.c_str());
	}
	
	// ������ز���		
	pSentenceItem->SetText(pNode->GetContent().c_str());
	pSentenceItem->SetVisited(FALSE);
	pSentenceItem->SetNotVisitedColor(pSentenceItem->GetTextColor());
	pSentenceItem->SetVisitedColor(pSentenceItem->GetTextColor());
	pSentenceItem->SetRolloverColor(WPixel(255,0,0));	
	
	return static_cast<XItem *>(pSentenceItem);
}
//------------------------------------------------------------------------