/**
*	created:		2012-6-24   2:17
*	filename: 		HTimer
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�߾��ȵ�ʱ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <windows.h>
//------------------------------------------------------------------------
struct IAxisStrategy;
//------------------------------------------------------------------------
class HTimer : public IAxisStrategy
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
	virtual BOOL    CalcCpuFreq();

	// ����ϵͳAPI���غ��뼶��TickCount��
	DWORD	GetTickCount();

	virtual UINT64	GetCpuFreq()
	{
		return static_cast<UINT64>(m_freq);
	}
public:
	HTimer() : m_hDllInstance( NULL ) , 
		m_uQueryPerformanceCounter( 0 ) , 
		m_uQueryPerformanceFrequency( 0 ) ,
		m_freq( 0.0 ) ,
		m_firstCount( 0 ){
	}

	~HTimer()
	{
		if (m_hDllInstance)
		{
			FreeLibrary(m_hDllInstance);
			m_hDllInstance = NULL;
		}
	}

private:
	HINSTANCE m_hDllInstance;
	typedef BOOL (WINAPI *PQueryPerformanceFrequency)(LARGE_INTEGER *lp);
	typedef BOOL (WINAPI *PQueryPerformanceCounter)(LARGE_INTEGER *lp);
	PQueryPerformanceFrequency m_uQueryPerformanceFrequency;
	PQueryPerformanceCounter   m_uQueryPerformanceCounter;	

	double  m_freq;
	__int64 m_firstCount;
};
//------------------------------------------------------------------------