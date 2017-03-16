/**
*	created:		2012-6-24   20:47
*	filename: 		MACSender
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../FKControllerInclude.h"
//------------------------------------------------------------------------
namespace NETTOOLS
{
	BOOL GetMACAddress(char * MACAddress);
}
//------------------------------------------------------------------------
class CMACSender  
{
protected:
	int m_nMacPosition;   //������������, Ҫ����MAC��λ��
	char m_Mac[512];
public:
	CMACSender();
	virtual ~CMACSender();
public:
	void SetMacPosition(DWORD dwPos);
	int GetMacPosition(void);
	//��MAC���͵�������
	LPCSTR GetMacPackage(void);
};
//------------------------------------------------------------------------