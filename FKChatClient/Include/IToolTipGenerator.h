/**
*	created:		2012-6-28   8:54
*	filename: 		IToolTipGenerator
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����Ҫ����TOOLTIP������ṩһ��ͳһ�Ľӿ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
#include <string>
//------------------------------------------------------------------------
class IToolTipGenerator  
{
public:
	//Ϊ��Ʒ��TOOLTIP���һ����
	virtual void AddItem(int nItemID) = 0;
	
	//ȡTOOLTIP����
	virtual LPCSTR GetToolTip(void) = 0;
	
	//���TOOLTIP
	virtual void Clear(void) = 0;
};
//------------------------------------------------------------------------
class CToolTipGenerator : public IToolTipGenerator
{
protected:
	std::string strToolTip;

public:	
	//һ��������һ�����ش˷����Ϳ�����
	virtual void AddItem(int nItemID) {};
	
	//ȡTOOLTIP����
	virtual LPCSTR GetToolTip(void) 
	{
		return strToolTip.c_str();
	};
	
	//���TOOLTIP
	virtual void Clear(void) 
	{
		strToolTip.clear();
	}	
};
//------------------------------------------------------------------------