/**
*	created:		2012-6-29   12:35
*	filename: 		XResizeButton
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ�ֿ��Ըı䴰���С�İ�ť
*/
//------------------------------------------------------------------------
#include "../Include/XResizeButton.h"
#include "../Include/OutputWindow.h"
//------------------------------------------------------------------------
XResizeButton::XResizeButton(XWindow * pParentWnd)
: XButton(pParentWnd)
, m_bDown(false)
{
}
//------------------------------------------------------------------------
XResizeButton::~XResizeButton(void)
{
}
//------------------------------------------------------------------------
DWORD XResizeButton::OnMouseDown(UINT nButton, UINT nShift, POINT pt)
{
	m_bDown = true;
	SetCapture();
	
	return XButton::OnMouseDown(nButton, nShift, pt);
}
//------------------------------------------------------------------------
DWORD XResizeButton::OnMouseUp(UINT nButton, UINT nShift, POINT pt)
{
	m_bDown = false;
	ReleaseCapture();
	
	return XButton::OnMouseUp(nButton, nShift, pt);
}
//------------------------------------------------------------------------
DWORD XResizeButton::OnMouseMove(UINT nButton, UINT nShift, POINT pt)
{	
	if (m_bDown)
	{
		RECT rcWindow;
		m_pParent->GetWindowRect(&rcWindow);

		if (rcWindow.bottom - pt.y > WINDOW_MIN_HEIGHT 
			&& rcWindow.bottom - pt.y < WINDOW_MAX_HEIGHT)
		{
			rcWindow.top = pt.y;

			// �ı丸�����С
			m_pParent->SetWindowPos(rcWindow);
		}				
	}
	
	return 0;
}
//------------------------------------------------------------------------