/**
*	created:		2012-6-24   22:29
*	filename: 		AttackerCommData
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��Ϸ���߳��빥���߳�ͨ�ŵĽṹ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
class AttackerCommData  
{
protected:
	CRITICAL_SECTION	m_section;
	BOOL				m_bThreadWorking;   //��ʶ�Ƿ����߳��ڹ���
	BOOL				m_bStopQuest;       //�Ƿ���ֹͣ����
public:
	AttackerCommData();
	virtual ~AttackerCommData();
	AttackerCommData(const AttackerCommData& other);
public:
	BOOL IsThreadWorking(void);
	void SetThreadWorking(BOOL bFlag);
	BOOL HasStopQuest(void);
	void SetStopQuest(BOOL bFlag);
};
//------------------------------------------------------------------------