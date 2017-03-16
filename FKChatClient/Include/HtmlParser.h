/**
*	created:		2012-6-28   8:43
*	filename: 		HtmlParser
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		HTML������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../Include/HtmlNode.h"
//------------------------------------------------------------------------
// HTML������
class CHtmlParser  
{
public:	
	CHtmlNode * Parse(const char * lpszScript);
	CHtmlParser(void);
	virtual ~CHtmlParser(void);
private:
	bool ParseNonregularNode(CHtmlNode *pNode);
	bool ParseRegularNode(string strTagName, CHtmlNode *pNode);
	bool PickValue(string &strValue);
	bool PickName(string & strName);
	bool ParseNode(CHtmlNode * pNode);
	void SkipBlank(void);
private:
	const char * m_lpszScript;
};
//------------------------------------------------------------------------