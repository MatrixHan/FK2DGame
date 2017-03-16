/**
*	created:		2012-6-29   12:34
*	filename: 		XResizeButton
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ı䴰���С��ť
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
//------------------------------------------------------------------------
// �ı䴰���С��ť
class XResizeButton : public XButton  
{
public:
	XResizeButton(XWindow * pParentWnd);
	virtual ~XResizeButton(void);

public:
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);

private:
	bool m_bDown;   // �Ƿ��Ѿ������˰�ť
};
//------------------------------------------------------------------------