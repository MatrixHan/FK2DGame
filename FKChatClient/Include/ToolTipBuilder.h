/**
*	created:		2012-6-28   10:26
*	filename: 		ToolTipBuilder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�������������ToolTip
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../Include/EquipToolTipBuilder.h"
#include "../Include/SkillToolTipBuilder.h"
#include "../Include/GoodsToolTipBuilder.h"
//------------------------------------------------------------------------
class CToolTipBuilder  
{
private:
	//�����Ψһʵ��
	static CToolTipBuilder *pBuilder;   
private:
	IToolTipGenerator * m_pGenerator;           //��ǰʹ�õ�ToolTip������
	CToolTipGenerator m_DefaultGenerator;		//Ĭ�ϵ�ToolTip������
	CEquipToolTipBuilder m_EquipGenerator;		//װ����ToolTip������
	CSkillToolTipBuilder m_SkillGenerator;		//���ܵ�ToolTip������
	CGoodsToolTipBuilder  m_GoodsGenerator;		//��Ʒ��ToolTip������
public:
	//Ϊ��Ʒ��TOOLTIP���һ����
	void AddItem(int nItemID);
	//��һ���¶�������TOOLTIP������
	BOOL SetObject(DWORD dwGoodsID, UID uid);
	//��һ���¶�������TOOLTIP������
	BOOL SetObject(DWORD dwGoodsID, IGoods *pGoods);
	//��һ���¶�������TOOLTIP������
	BOOL SetObject(int nSkillID);
	//ȡTOOLTIP����
	LPCSTR GetToolTip(void);

public:
	CToolTipBuilder();
	virtual ~CToolTipBuilder();
	static BOOL Init(void);
	static void Release(void);
	static CToolTipBuilder * Instance(void);
};
//------------------------------------------------------------------------
