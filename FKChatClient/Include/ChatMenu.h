/**
*	created:		2012-6-28   7:15
*	filename: 		ChatMenu
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��Ӳ˵�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
#include <string>
#include "../../FKGame/WndSys/WndSys.h"
using std::string;
//------------------------------------------------------------------------
const int MENU_ITEM_HEIGHT = 15;
//------------------------------------------------------------------------
// �˵��¼�������
struct IMenuEventReceiver
{
	// �����¼�
	virtual void Clicked(int nId) = 0;
};
//------------------------------------------------------------------------
// ��Ӳ˵���
class CChatMenuItem
{
public:
	int		m_nId;					// �˵����ID
	string	m_strName;				// �˵��������
	BOOL    m_bEnabled;				// �Ƿ�Enabled
public:
	CChatMenuItem(int nId, string strName,BOOL bEnabled = TRUE);
};
//------------------------------------------------------------------------
// ��Ӳ˵�
class CChatMenu : public XMainWnd
{
public:
	typedef list<CChatMenuItem *> MenuItemList;
	typedef list<CChatMenuItem *>::iterator MenuItemListItr;

public:
	CChatMenu(void);
	virtual ~CChatMenu(void);
	
Signals:
	Signal1 s_selChanged;
	
public:
	BOOL Create(IResObject * pResObject);
	void DrawMenu(ICanvas * pCanvas, RECT & rcClip);
	virtual void OnPaint(ICanvas * pCanvas);
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual void OnActivate(BOOL bActive);
	virtual BOOL IsTransparent(POINT& pt);
	void Show(const POINT * pPos, BOOL bShow); 	
	void AddItem(int nId, string strName);
	void RemoveItem(int nId);
	void RemoveAll(void);
	void SetEventReceiver(IMenuEventReceiver * pReceiver);
    void SetMenuItemEnable(int nMenuId,BOOL bEnabled = TRUE);  
    void SetMenuItemsDisabled(const int nMenuIds[],int nCount);

private:
	XEdit * m_pFrame;						// ���
	IMenuEventReceiver * m_pEventReceiver;	// �¼�������
	int m_nSelected;						// ѡ�е���
	WPixel m_clrSelected;					// ѡ�������ɫ
	WPixel m_clrText;						// �˵�����ı���ɫ
	MenuItemList m_items;					// �˵����б�	
};
//------------------------------------------------------------------------