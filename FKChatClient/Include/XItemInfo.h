/**
*	created:		2012-6-28   2:23
*	filename: 		XItemInfo
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���������ʾ��ϵͳ��Ϣ��Ŀ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
//------------------------------------------------------------------------
// ���������ʾ��ϵͳ��Ϣ��Ŀ
class XItemInfo : public XItemText  
{
public:
	virtual void OnDraw(ICanvas* pCanvas, int x, int y, RECT& rcClip, int alpha);
	void SetBackColor(WPixel clrBack);
	XItemInfo();
	virtual ~XItemInfo();

private:
	WPixel m_clrBack; // ������ɫ
};
//------------------------------------------------------------------------
