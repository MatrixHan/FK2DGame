/**
*	created:		2012-6-28   8:03
*	filename: 		GoodsToolTipBuilder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���������������Ʒ��ToolTip
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/SceneObj/IGoods.h"
#include "IToolTipGenerator.h"
#include "GoodsToolTipCommon.h"
//------------------------------------------------------------------------
class CGoodsToolTipBuilder : public CGoodsToolTipCommon  
{
public:
	enum
	{
		enSoldPrice,      //�ۼ�
		enName,           //����
		enDescription,        //����
		enNeedLevel,          //ʹ�õȼ�
		enGoodsType,          //��Ʒ���� (��ʯ , ��ҩ)
		enRank,               //��Ʒ����
		enBlank,              //����
		enBind,           //��
		enUnknown,            //δ֪����
	};

private:
	
	void AddSoldPrice(void);    
	void AddName(void);
	void AddDescription(void);      
	void AddNeedLevel(void);    
	void AddGoodsType(void);
	void AddRank(void);
	void AddBlank(void);
	

public:
	CGoodsToolTipBuilder();
	virtual ~CGoodsToolTipBuilder();

	//��һ���¶�������TOOLTIP������
	BOOL SetObject(DWORD dwGoodsID, UID uid);
	//��һ���¶�������TOOLTIP������
	BOOL SetObject(DWORD dwGoodsID, IGoods *pGoods);	
	//Ϊ��Ʒ��TOOLTIP���һ����
	void AddItem(int nItemID);
};
//------------------------------------------------------------------------