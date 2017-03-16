/**
*	created:		2012-6-20   23:29
*	filename: 		ITimerAxis
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <windows.h>
//------------------------------------------------------------------------
struct ITimerSink
{
	// ��ʱ���ʱ����
	virtual void	OnTimer(DWORD dwEventID) = NULL; 	
};
//------------------------------------------------------------------------
struct ITimeAxis
{
	// ��ʱ���ʱ����
	virtual void	OnCheck(void) = NULL; 
	// ���ö�ʱ�������ڵ��Լ���һ�������ַ�
	virtual BOOL	SetTimer(DWORD dwEventID, ITimerSink * pTimerSink, DWORD dwElapse, LPCSTR szTimerDesc = NULL) = NULL;
	// ���ٶ�ʱ��
	virtual void	KillTimer(DWORD dwEventID, ITimerSink * pTimerSink)	= NULL;
	// ���Է��ؾ��������ٴ���ĵ�ǰʱ��Tick��
	virtual DWORD	GetTimeTick(void) = NULL;
	// ����ʱ����
	virtual void	Release(void) = NULL;
};
//------------------------------------------------------------------------
// �����ӿ� - �ж�����Ƿ�ʹ�ü��٣����¼���Ƶ��(��ƵCPU)
struct ITimeAxisAux
{
	virtual BOOL	UsingGear(void) = 0;
	virtual void	RecalcCpuFreq(void) = 0;
	virtual UINT64	GetCpuFreq(void) = 0;
};
//------------------------------------------------------------------------