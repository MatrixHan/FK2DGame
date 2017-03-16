/**
*	created:		2012-6-29   12:33
*	filename: 		XMoveButton
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ƶ����尴ť
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
//------------------------------------------------------------------------
// �ƶ����尴ť
class XMoveButton : public XButton  
{
public:
	XMoveButton(XWindow * pParentWnd);
	virtual ~XMoveButton(void);

public:
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);

private:
	POINT m_ptDown; // ���°�ťʱ����������
	bool m_bDown;   // �Ƿ��Ѿ������˰�ť
};
//------------------------------------------------------------------------