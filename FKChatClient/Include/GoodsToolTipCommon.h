/**
*	created:		2012-6-28   8:06
*	filename: 		GoodsToolTipCommon
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��Ʒ��Ϊ ��Ʒ, �䷽, ������ƿ3��, ���������ʵ��һЩ��������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IToolTipGenerator.h"
#include "../../FKGame/SceneObj/IGoods.h"
#include "../../FKGame/Common/IConfigDataCenter.h"
//------------------------------------------------------------------------
class CGoodsToolTipCommon : public CToolTipGenerator  
{
protected:
	IGoods*				m_pGoods;
	GOODS_BASEPROPERTY	m_GoodsProp;
protected:
	void AddBind(void);
public:
	CGoodsToolTipCommon();
	virtual ~CGoodsToolTipCommon();

	virtual BOOL SetObject(DWORD dwGoodsID, UID uid);
    virtual BOOL SetObject(DWORD dwGoodsID, IGoods *pGoods);
};
//------------------------------------------------------------------------