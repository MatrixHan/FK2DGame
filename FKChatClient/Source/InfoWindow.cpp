/**
*	created:		2012-6-28   8:46
*	filename: 		InfoWindow
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ��װ��������ʾ����
*/
//------------------------------------------------------------------------
#include "../Include/ChatClient.h"
#include "../Include/InfoWindow.h"
#include <assert.h>
//------------------------------------------------------------------------
CInfoWindow::CInfoWindow(void)
: m_pHtml(NULL)
{
}
//------------------------------------------------------------------------
CInfoWindow::~CInfoWindow(void)
{
}
//------------------------------------------------------------------------
bool CInfoWindow::Create(IResObject * pResObject)
{
	CWndSysHelper wndHelper;
	IResObject * pSubResObject = NULL;
	XWindow * pDesktop = wndHelper.GetGameDesktop();
	assert(pDesktop != NULL);

	if (pResObject == NULL)
	{
		PrintEventLn("ChatClient (FAIL) - pResObject == NULL");
		return false;
	}	

	// ����������ʾ����
	SetParent(pDesktop);
	wndHelper.CreateStockProperty(this, pResObject, INFOWINDOW_ID);
	pDesktop->AddWindow(this);	
	
	// ����HTML�ؼ�	
	m_pHtml = new XHtml(this);
	if (m_pHtml == NULL)
	{
		PrintEventLn("ChatClient (FAIL) - new XHtml(this)");
		return false;
	}
	pSubResObject = pResObject->GetSubObjectByIndex(0);
	if (pSubResObject == NULL)
	{
		PrintEventLn("ChatClient (FAIL) - GetSubObjectByIndex(0)");
		return false;
	}
	if(wndHelper.CreateXWindow(this, pSubResObject, 0, m_pHtml) == NULL)
	{
		PrintEventLn("ChatClient (FAIL) - CreateXWindow(m_pHtml)");
		return false;
	}

	m_pHtml->m_pHScrollBar->SetVisible(FALSE);
	m_pHtml->m_pVScrollBar->SetVisible(FALSE);
	
	SetVisible(FALSE);
	
	return true;
}
//------------------------------------------------------------------------
void CInfoWindow::SetWindowPos(RECT & rc)
{
	XMainWnd::SetWindowPos(rc);

	if (m_pHtml != NULL)
	{
		m_pHtml->m_pContainer->SetWindowPos(rc);
	}
}

//-------------------------------------------------------------------
// ��  �������������Ϣ 
// ��  ����void
// ����ֵ��void
//-------------------------------------------------------------------
void CInfoWindow::ClearInfo(void)
{
	m_pHtml->m_pContainer->Clear();
	m_pHtml->Clear();
}

//-------------------------------------------------------------------
// ��  ��������Ҫ��ʾ��������Ϣ 
// ��  ����[����] string strInfo - ������Ϣ
// ����ֵ��bool - ������ʾ����true,���򷵻�false
//-------------------------------------------------------------------
bool CInfoWindow::SetInfo(string strInfo)
{
	ClearInfo();

	if (m_pHtml->m_LineParser.ParseLine(strInfo.c_str()))
	{
		m_pHtml->m_pHScrollBar->SetVisible(FALSE);
		m_pHtml->m_pVScrollBar->SetVisible(FALSE);
		return true;
	}
	else
	{
		return false;
	}
}
//------------------------------------------------------------------------
void CInfoWindow::OnActivate(BOOL bActive)
{
	SetVisible(bActive);
}
//------------------------------------------------------------------------
void CInfoWindow::SetVisible(BOOL bVisible)
{
	if (bVisible)
	{
		XMainWnd::SetActive();
		XMainWnd::SetVisible(bVisible);
	}
	else
	{
		XMainWnd::SetVisible(bVisible);
	}
}
//------------------------------------------------------------------------