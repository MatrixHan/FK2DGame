/**
*	created:		2012-6-28   2:41
*	filename: 		XItemThing
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�����������ʾ����Ʒ��Ŀ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
#include "../../FKGame/SceneObj/IGoods.h"
//------------------------------------------------------------------------
// �����������ʾ����Ʒ��Ŀ
class XItemThing : public XItemLinkText  
{
public:
	XItemThing(void);
	virtual ~XItemThing(void);
public:
	void SetGoods(IGoods *pGoods);
public:
	virtual void Emit(void);
private:
	IGoods *m_pGoods;
};
//------------------------------------------------------------------------