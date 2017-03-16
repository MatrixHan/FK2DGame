/**
*	created:		2012-6-24   3:03
*	filename: 		TimeMan
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <winnt.h>
#include "NTimer.h"
#include "MTimer.h"
#include "HTimer.h"
#include "MSpeed.h"
//------------------------------------------------------------------------
extern bool CheckRDTSCTechnology(void);
//------------------------------------------------------------------------
class CTimeMan
{
public:
	// ��ʼ��ʱ��
	void TimerInit()
	{
		try
		{
			if( ! CheckRDTSCTechnology() )
			{
				m_HTimer.TimerInit();
				m_bInitSucceed = TRUE;
				return;
			}

		}
		catch(...)
		{
			//
		}
		m_bInitSucceed = m_NTimer.TimerInit();
		if(!m_bInitSucceed)
			m_bInitSucceed = m_MTimer.TimerInit();
	}
	// ����ϵͳAPI���غ��뼶��TickCount��
	DWORD	GetTickCount()
	{
		if(m_NTimer.IsInitSucceed()) return m_NTimer.GetTickCount();
		if(m_MTimer.IsInitSucceed()) return m_MTimer.GetTickCount();
		if(m_HTimer.IsInitSucceed()) return m_HTimer.GetTickCount();

		return ::GetTickCount();
	}
private:
	BOOL	m_bInitSucceed;
	MTimer	m_MTimer;
	NTimer	m_NTimer;
	HTimer	m_HTimer;
public:
	CTimeMan() : m_bInitSucceed(FALSE)
	{
		try{
			// �ȷ�ֹ�޸�8253������

			if( IsWindows98() )
			{
				//				if( GetClock9x()!= 0 )
				//				{
				//				MessageBox( NULL , "��ʼ��ʧ��,ϵͳ���ֿ��ɵ����׳���!" , "ERROR" , MB_OK );
				//				PostQuitMessage(0);
				//				return FALSE;
				//				}
			}else
			{
				//����׼����ֹ��Ƶ������Ƶ���޷���ֹ��(?)
				//DisableSpeedSteping();
			}
		}
		catch(...)
		{}
	}
	~CTimeMan()
	{
		try{ 
			if( !IsWindows98() )
			{
				//����׼����ԭ��Դ����
				//ReturnOriginalPowerScheme();
			}
		}catch(...)
		{
			//
		}	
	}
};
//------------------------------------------------------------------------