/**
*	created:		2012-7-2   22:05
*	filename: 		IHorse
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������Ʒ����thing��ֻ��һ�����ݼ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
struct IHorse
{
	// ������ƥID
	virtual void		SetID(int nHorseID) = NULL;

	// ȡ����ƥID
	virtual int			GetID(void) = NULL;
};
//------------------------------------------------------------------------