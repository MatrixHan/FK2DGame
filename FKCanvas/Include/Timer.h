/**
*	created:		2012-6-19   21:03
*	filename: 		Timer
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
#ifndef _MSVC
	#define _MSVC
#endif
//------------------------------------------------------------------------
class WTimer  
{
private:
	union uINT64 {
		__int64 time;
		struct {
			DWORD low;
			DWORD high;
		};
	};

	int m_iPriority;
	HANDLE m_hThread;
	uINT64 m_i64Timer;
	DWORD m_StartTime;
	static DWORD s_CP10K;		// ���֮һ���������
	static DWORD s_CyclePerSec;	// һ�� CPU ��������
public:
	WTimer();

	static int CPU();			//���� CPU Ƶ��
	void Reset(int t=0);		//��ʼ����ʱ��
	void Start();				//����ʱ����
	DWORD Stop();				//�õ�ʱ����ʱ��(��С��λ�� 16 ʱ������, ������� P200 �� 5 ��)
	DWORD GetTime();			//���ص�ǰ����ֵ (��С��λΪ 1/10000 ��)
};
//------------------------------------------------------------------------