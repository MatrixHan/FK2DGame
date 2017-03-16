/**
*	created:		2012-6-28   9:38
*	filename: 		TitleWnd
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ƺ�ϵͳ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "XListBoxEx.h"
#include "../../FKGame/WndSys/WndSys.h"
#include "../../FKGame/WndSys/IWndMgr.h"
#include "../Include/TitleDef.h"
//------------------------------------------------------------------------
class CTitleWnd:public IWndEnableCall4 
{
public:
	CTitleWnd();
	virtual ~CTitleWnd();
public:
	virtual int OnEvent(UINT uEventID, WPARAM wParam, LPARAM lParam);
public:
	BOOL Create(IResObject *pResObject,int nWndId);
	void OnVScrollPosChanged(DWORD dwParam);
	void ReSetScroll();

public:
	void AddActorTitle(SActorTitle2 *pActorTitle);
	void DeleteActorTitle(long lTitleId);
	void UnUseActorTitle(long lTitleId);
	void StatusChange(long lTitleId,int nStatus);
	void UpdateScroll();
public:
	// ��ť������
	void OnUseTitle();
	void OnDeleteTitle();
	void OnUnUseTitle();
	void OnClose();
	void OnSelChange(DWORD dwParam);
	void OnHide();
	virtual void OnEscape();
	
private:
	XEdit  *m_pBorderEdit1;     // �߿�1
	XEdit  *m_pBorderEdit2;     // �߿�2
	XButton *m_pUserTitleBtn;   // ʹ�óƺ�
	XButton *m_pUnUseTitleBtn;  // ��ʹ�óƺ�
	XButton *m_pDeleteTitleBtn; // ɾ���ƺ�
	XButton *m_pCloseBtn;       // �رհ�ť
	XCheckBox *m_pHideBtn;      // ���سƺ�
	XListBoxEx   *m_pListBox;   // �ƺ��б�
	XScrollBar *m_pScrollBar;   // �����ƺ��б�Ĺ�����
    BOOL    m_bReget;
};
//------------------------------------------------------------------------
