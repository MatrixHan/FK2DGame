/**
*	created:		2012-7-2   21:58
*	filename: 		IChunnel
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��ͼͨ��
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/SceneObj/IMapThing.h"
#include "../../FKGame/Common/ChunnelProp.h"
//------------------------------------------------------------------------
struct IChunnel : public IMapThing
{
	// ȡ������
	virtual LPCSTR				GetName(void) = NULL;

	// ȡ�ô��ͷ�ʽ
	virtual DWORD				GetTransMode(void) = NULL;

	// ȡ��Ŀ�����Ϣ
	virtual SCHUNNEL_TARGET *	GetTargetInfor(DWORD &dwCount) = NULL;
};
//------------------------------------------------------------------------