/**
*	created:		2012-6-28   10:29
*	filename: 		WindowCreator
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ�ִ��ڴ�����
*/
//------------------------------------------------------------------------
#include "../Include/WindowCreator.h"
#include "../Include/ChatClient.h"
//------------------------------------------------------------------------
CWindowCreator::CWindowCreator(void)
{
}
//------------------------------------------------------------------------
CWindowCreator::~CWindowCreator(void)
{
}

//-------------------------------------------------------------------
// ��  ��������ͨ�ô��� 
// ��  ����[����] XWindow *pParent - ������
// ��  ����[����] XWindow *pMe - �Լ�
// ��  ����[����] IResObject *pResObject - �ű���Դ
// ����ֵ��XWindow * - �����ɹ�����TRUE,���򷵻�FALSE
//-------------------------------------------------------------------
BOOL CWindowCreator::CreateGeneralWnd(XWindow *pParent, XWindow *pMe, IResObject *pResObject)
{
	if (pParent == NULL)
	{
		pParent = g_wndHelper.GetGameDesktop();
	}
	
	if (pParent == NULL || pMe == NULL || pResObject == NULL)
	{
		return FALSE;
	}

	return (g_wndHelper.CreateXWindow(pParent, pResObject, pResObject->GetID(), pMe) != NULL);
}

//-------------------------------------------------------------------
// ��  �����������ڿؼ�
// ��  ����[����] XWindow *pParent - ������
// ��  ����[����] XWindow *pMe - �Լ�
// ��  ����[����] IResObject *pResObject - �ű���Դ
// ����ֵ��XWindow * - �����ɹ�����TRUE,���򷵻�FALSE
//-------------------------------------------------------------------
BOOL CWindowCreator::CreateControl(XWindow *pParent, XWindow *pMe, IResObject *pResObject)
{
	if (pParent == NULL || pResObject == NULL)
	{
		return FALSE;
	}
	
	return (g_wndHelper.CreateXWindow(pParent, pResObject, pResObject->GetID(), pMe) != NULL);
}

//-------------------------------------------------------------------
// ��  �������������� 
// ��  ����[����] XWindow *pParent - ������
// ��  ����[����] XWindow *pMe - �Լ�
// ��  ����[����] IResObject *pResObject - �ű���Դ
// ����ֵ��XWindow * - �����ɹ�����TRUE,���򷵻�FALSE
//-------------------------------------------------------------------
BOOL CWindowCreator::CreateMainWnd(XWindow *pParent, XWindow *pMe, IResObject *pResObject)
{
	if (pParent == NULL)
	{
		pParent = g_wndHelper.GetGameDesktop();
	}
	
	if (pParent == NULL || pMe == NULL || pResObject == NULL)
	{
		return FALSE;
	}
	
	pMe->SetParent(pParent);
	g_wndHelper.CreateStockProperty(pMe, pResObject, pResObject->GetID());
	pParent->AddWindow(pMe);	

	return TRUE;
}
//------------------------------------------------------------------------