/**
*	created:		2012-6-28   2:08
*	filename: 		IItemFactory
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		HTML����ʾԪ�ض��󹤳��ӿ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
class XItem;
//------------------------------------------------------------------------
// HTML����ʾԪ�ض��󹤳��ӿ�(ʹ������Բ���ʵ��)
struct IItemFactory  
{
	// �����ı�
	virtual XItem * CreateTextItem(void * pNode)
	{
		return NULL;
	}

	// ��������
	virtual XItem * CreateLinkItem(void * pNode)
	{
		return NULL;
	}
	
	virtual XItem * CreateLinkItem2(void * pNode)
	{
		return NULL;
	}

	// ���������
	virtual XItem * CreateInputItem(void * pNode)
	{
		return NULL;
	}

	// ����ͼƬ
	virtual XItem * CreateImageItem(void * pNode)
	{
		return NULL;
	}
	
	// ��������
	virtual XItem * CreateNewlineItem(void * pNode)
	{
		return NULL;
	}	

};
//------------------------------------------------------------------------