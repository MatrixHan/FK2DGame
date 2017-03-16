/**
*	created:		2012-6-28   2:34
*	filename: 		XItemSentence
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�����������ʾ�Ļ�����Ӷ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
//------------------------------------------------------------------------
// �����������ʾ�Ļ�����Ӷ���
class XItemSentence : public XItemLinkText  
{
public:
	XItemSentence(void);
	virtual ~XItemSentence(void);
	virtual void OnDraw(ICanvas* pCanvas, int x, int y, RECT& rcClip, int alpha);
	void SetBackColor(WPixel clrBack);
public:
	virtual void Emit(void);
private:
	WPixel m_crBackColor;
	BOOL   m_bSetBack;
};
//------------------------------------------------------------------------