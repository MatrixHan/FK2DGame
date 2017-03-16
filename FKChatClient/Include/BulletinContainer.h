/**
*	created:		2012-6-28   2:49
*	filename: 		BulletinContainer
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������Ϣ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <list>
#include "BulletinItem.h"
#include "../../FKGame/WndSys/WndSys.h"
#include "../../FKGame/Timer/ITimerAxis.h"
//------------------------------------------------------------------------
using std::list;
//------------------------------------------------------------------------
// ������Ϣ����
class CBulletinContainer : public XControl,
                           public ITimerSink
{
private:
	typedef list<CBulletinItem> ItemList;
	typedef list<CBulletinItem>::iterator ItemListItr;
	typedef list<CBulletinItem>::const_iterator ItemListCItr;
	
public:
	CBulletinContainer(void);
	virtual ~CBulletinContainer(void);

public:	
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip );	
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);	
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual void OnTimer(DWORD dwEventID);
	
public:	
	void AddItem(LPCSTR lpszBulletin);
	void RemoveItem(ItemListItr itr);
	ItemListItr GetItemByPoint(POINT pt);
	void MouseOver(POINT pt);
	
private:		
	ItemList m_items;
	ITTFont * m_pFont;
	int m_nStartPos;			// ��ʼλ��
	int m_nTotalLen;			// ���й�����ܳ���
	BOOL m_bMouseOver;			// ����Ƿ�������
};
//------------------------------------------------------------------------
