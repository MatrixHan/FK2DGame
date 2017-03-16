/**
*	created:		2012-6-28   4:30
*	filename: 		BulletinWnd
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��������ʾϵͳ������
*/
//------------------------------------------------------------------------
#include "../Include/BulletinWnd.h"
#include "../Include/WindowCreator.h"
#include "../../FKResManager/IFKResManager.h"
//------------------------------------------------------------------------
CBulletinWnd::CBulletinWnd(void)
{
}
//------------------------------------------------------------------------
CBulletinWnd::~CBulletinWnd(void)
{
}

//-------------------------------------------------------------------
// ��  ��������ϵͳ����������
//-------------------------------------------------------------------
BOOL CBulletinWnd::Create(IResObject * pResObject)
{
	if (!CWindowCreator::CreateMainWnd(NULL, this, pResObject))
	{
		return FALSE;
	}

	if (!CWindowCreator::CreateControl(this, &m_container, pResObject->GetSubObjectByIndex(0)))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CBulletinWnd::IsTransparent(POINT& pt)
{
	return FALSE;
}

DWORD CBulletinWnd::OnMouseDown(UINT nButton, UINT nShift, POINT pt)
{
	return m_container.OnMouseDown(nButton, nShift, pt);
}

DWORD CBulletinWnd::OnMouseMove(UINT nButton, UINT nShift, POINT pt)
{
	return m_container.OnMouseMove(nButton, nShift, pt);
}

//-------------------------------------------------------------------
// ��  ������ӹ���
//-------------------------------------------------------------------
void CBulletinWnd::AddBulletin(LPCSTR lpszBulletin)
{
	m_container.AddItem(lpszBulletin);
}
//------------------------------------------------------------------------