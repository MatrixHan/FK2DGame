/**
*	created:		2012-7-3   23:17
*	filename: 		CHorse
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������Ʒ����thing��ֻ��һ�����ݼ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IHorse.h"
//------------------------------------------------------------------------
class CHorse : public IHorse
{
public:
	// ������ƥID
	virtual void		SetID(int nHorseID);
	// ȡ����ƥID
	virtual int			GetID(void);

	CHorse(void);
	virtual ~CHorse(void);	
private:
	int					m_nID;
};
//------------------------------------------------------------------------