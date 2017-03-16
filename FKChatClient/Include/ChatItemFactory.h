/**
*	created:		2012-6-28   6:47
*	filename: 		ChatItemFactory
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��������ʾԪ�صĶ��󹤳�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../IItemFactory.h"
#include "HtmlNode.h"
#include "../../FKCanvas/IFKCanvas.h"
//------------------------------------------------------------------------
enum 
{
	LINK_PLAYER,	// �������
	LINK_THING,		// װ������
	LINK_SENTENCE	// ��������
};
//------------------------------------------------------------------------
// ��������ʾԪ�صĶ��󹤳�
class CChatItemFactory : public IItemFactory  
{
public:
	CChatItemFactory(void);
	virtual ~CChatItemFactory(void);

public:
	// �����ı�
	virtual XItem * CreateTextItem(void * pNode);
	// ��������
	virtual XItem * CreateLinkItem(void * pNode);
	// ���������
	virtual XItem * CreateInputItem(void * pNode);
	// ����ͼƬ
	virtual XItem * CreateImageItem(void * pNode);
	// ��������
	virtual XItem * CreateNewlineItem(void * pNode);
	// ��������
	virtual XItem * CreateLinkItem2(void * pNode);	

private:
	XItem * CreateSentenceLink2(CHtmlNode * pNode);
	XItem * CreateSentenceLink(CHtmlNode * pNode);
	XItem * CreateThingLink(CHtmlNode * pNode);
	XItem * CreatePlayerLink(CHtmlNode * pNode);
	BOOL GetColor(const CHtmlNode * pNode, WPixel & wpColor, string strAttribute = "color");
	void GetFont(const CHtmlNode * pNode, ITTFont ** pFont);
};
//------------------------------------------------------------------------