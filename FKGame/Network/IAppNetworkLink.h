/**
*	created:		2012-6-22   0:25
*	filename: 		IAppNetworkLink
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IClientSocket.h"
#include "../Common/Macro.h"
//------------------------------------------------------------------------
enum 
{
	NETWORKLINKTYPE_LOGIN = 0,
	NETWORKLINKTYPE_MAPCHANGING, 
	NETWORKLINKTYPE_SELACTOR, 
	NETWORKLINKTYPE_SCENE, 	
	NETWORKLINKTYPE_RUNNING, 
	NETWORKLINKTYPE_MAX, 
};
//------------------------------------------------------------------------
struct IAppNetworkLink : public IClientLinkSink
{
public:
	// �������ݰ�
	virtual int				Send(LPCSTR buf, int nLen) = NULL;
	// ȡ��IClientLinkָ��, ʹ�÷������ḻ
	virtual IClientLink *	GetClientLink(void) = NULL;
	// ���ص�ǰ���ӵ�����һ��״̬
	virtual int GetType(void) = NULL;
};
//------------------------------------------------------------------------