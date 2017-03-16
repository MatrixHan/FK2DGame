/**
*	created:		2012-6-28   8:56
*	filename: 		MessageBuilder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������Ϣ������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "ChatParser.h"
#include "ChatItemFactory.h"
#include "../../FKGame/WndSys/WndSys.h"
//------------------------------------------------------------------------
// ������Ϣ������
class CMessageBuilder  
{
public:
	CMessageBuilder(void);
	virtual ~CMessageBuilder(void);

public:
	void SetContainer(XItemContainer * pContainer);
	BOOL Build(LPCSTR lpszText);

private:	
	BOOL AddToContainer(const CHtmlNode * pContent);
	BOOL AddItem(XItem * pItem, BOOL bNewline);

private:
	CChatParser m_chatParser;
	CChatItemFactory m_itemFactory;
	XItemContainer * m_pContainer;
};
//------------------------------------------------------------------------