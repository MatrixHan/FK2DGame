/**
*	created:		2012-7-2   22:06
*	filename: 		IRidePart
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��˲���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
// ����
struct IThingPart;
struct IHorse;
//------------------------------------------------------------------------
// ����
struct IRidePart : public IThingPart
{
	// ȡ����ƥ
	virtual IHorse *		GetHorse(int nIndex) = NULL;

	// �Ƿ���������
	virtual BOOL			IsRiding(void) = NULL;
};
//------------------------------------------------------------------------