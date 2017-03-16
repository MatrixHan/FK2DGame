/**
*	created:		2012-6-24   2:19
*	filename: 		MTimer
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��ȷʱ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "AxisCommon.h"
#include <stdlib.h>
#include <time.h>
#include "HTimer.h"
//------------------------------------------------------------------------
struct IAxisStrategy;
//------------------------------------------------------------------------
class MTimer : public IAxisStrategy
{
public:
	virtual void	Start(LPCSTR pBuf,long lLen)
	{
	}
	virtual void	Stop()
	{
	}
	// ��ʼ��ʱ��
	virtual BOOL    TimerInit();
	virtual BOOL	CalcCpuFreq();
	// ����ϵͳAPI���غ��뼶��TickCount��
	virtual DWORD	GetTickCount();
	virtual UINT64	GetCpuFreq()
	{
		return m_RdtscPerSecond;
	}
public:
	MTimer();
	virtual ~MTimer();
private:
	UINT64  CalculateRdtscPerSecond();
	UINT64  CalculateRdtscPerSecond2();
	UINT64  CalculateRdtscPerSecond3();
	void	sleep( clock_t wait );
	// ȡ�õ�ǰCPUʱ������
	inline UINT64  GetRdtscCycleCount();
	// ȡ�ó�ʼCPUʱ������
	inline UINT64  GetStartCycleCount();
	// ȡ��ÿ����ʱ��������
	inline UINT64  GetRdtscPerSecond();

	enum
	{
		__ticks_per_second  = 1000 ,	// ÿ��1000TICKS
		__sleep_time_forcheck=256  ,	// sleep��ʱ��
		__test_flush_times  = 4  ,		// ����flushʱ�����
		__sleep_times		= 2	   ,	// ����sleep�Ĵ���
	};
private:
	UINT64  m_StartCycleCount;
	UINT64	m_RdtscPerSecond;
	DWORD   m_dwLastTickCount;
};
//------------------------------------------------------------------------