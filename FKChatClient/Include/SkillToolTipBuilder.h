/**
*	created:		2012-6-30   4:47
*	filename: 		SkillToolTipBuilder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����������������ܵ�TOOLTIP
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IToolTipGenerator.h"
//------------------------------------------------------------------------
class CSkillToolTipBuilder : public CToolTipGenerator  
{
public:
	CSkillToolTipBuilder();
	virtual ~CSkillToolTipBuilder();
	
	//��һ���¶�������TOOLTIP������
	BOOL SetObject(DWORD dwSkillID);
	
	//Ϊ��Ʒ��TOOLTIP���һ����
	void AddItem(int nItemID);	
};
//------------------------------------------------------------------------