/**
*	created:		2012-6-30   1:07
*	filename: 		IConjurePart
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ٻ�����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <list>
using std::list;
//------------------------------------------------------------------------
// ����
struct IThingPart;

// ����
struct IConjurePart : public IThingPart
{
	// ȡ������
	virtual UID					GetChief(void) = NULL;

	// �ܷ��ҵ�
	virtual BOOL				FindPawn(UID uidPawm) = NULL;

	// ȡ��צ���б�
	virtual list< UID >			GetPawn(void) = NULL;
};
//------------------------------------------------------------------------