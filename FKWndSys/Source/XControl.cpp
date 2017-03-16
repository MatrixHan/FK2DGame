/**
*	created:		2012-6-23   20:49
*	filename: 		XControl
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
//------------------------------------------------------------------------
XControl::XControl()
{

}
//------------------------------------------------------------------------
XControl::XControl(XWindow* pParentWnd) : XWindow(pParentWnd)
{
}
//------------------------------------------------------------------------
XControl::~XControl()
{

}
//------------------------------------------------------------------------
DWORD XControl::OnMouseDown(UINT nButton, UINT nShift, POINT pt)
{
	XWindow* pWnd = GetMouseMove();
	if (pWnd)
	{
		if (pWnd->IsEnabled() && pWnd != this)
			return pWnd->OnMouseDown(nButton, nShift, pt);
	}
	return XWindow::OnMouseDown(nButton, nShift, pt);
}
//------------------------------------------------------------------------
DWORD XControl::OnMouseUp(UINT nButton, UINT nShift, POINT pt)
{
	XWindow* pWnd = GetMouseMove();
	if (pWnd)
	{
		if (pWnd->IsEnabled() && pWnd != this)
			return pWnd->OnMouseUp(nButton, nShift, pt);
	}
	return XWindow::OnMouseUp(nButton, nShift, pt);
}
//------------------------------------------------------------------------
DWORD XControl::OnMouseMove(UINT nButton, UINT nShift, POINT pt)
{
	XWindow* pWnd = GetMouseMove();
	if (pWnd)
	{
		if (pWnd->IsEnabled() && pWnd != this)
			return pWnd->OnMouseMove(nButton, nShift, pt);
	}
	return XWindow::OnMouseMove(nButton, nShift, pt);
}
//------------------------------------------------------------------------
// ������Ϣ
DWORD XControl::OnKeyDown(UINT nKeyCode, UINT nShift)
{
	XWindow* pWnd = GetFocus();
	if (pWnd) // ������еĽ��㴰�ڴ���
		return pWnd->OnKeyDown(nKeyCode, nShift);
	return XWindow::OnKeyDown(nKeyCode, nShift);
}
//------------------------------------------------------------------------
DWORD XControl::OnKeyUp(UINT nKeyCode, UINT nShift)
{
	XWindow* pWnd = GetFocus();
	if (pWnd) // ������еĽ��㴰�ڴ���
		return pWnd->OnKeyUp(nKeyCode, nShift);
	return XWindow::OnKeyUp(nKeyCode, nShift);
}
//------------------------------------------------------------------------
DWORD XControl::OnChar(UINT nKeyAscii)
{
	XWindow* pWnd = GetFocus();
	if (pWnd) // ������еĽ��㴰�ڴ���
		return pWnd->OnChar(nKeyAscii);
	return XWindow::OnChar(nKeyAscii);
}
//------------------------------------------------------------------------