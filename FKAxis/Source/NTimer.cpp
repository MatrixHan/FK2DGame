/**
*	created:		2012-6-24   2:54
*	filename: 		NTimer
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/IStrategy.h"
#include "../Include/NTimer.h"
#include "../Include/TimeAxisAux.h"
//------------------------------------------------------------------------
BOOL NTimer::TimerInit()
{
	if(CalcCpuFreq())
	{
		m_ui64Begin = GetRdtscCycleCount();
		return TRUE;
	}

	return FALSE;
}
//------------------------------------------------------------------------
BOOL NTimer::CalcCpuFreq()
{
	char szPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szPath);
	if(szPath[strlen(szPath) - 1] == '\\')
		szPath[strlen(szPath) - 1] = 0;
	strcat(szPath,"\\");

	if(!strchr(g_szWinioFakeDllName,':'))
	{
		strcat(szPath,g_szWinioFakeDllName);
		strcpy((char*)g_szWinioFakeDllName,szPath);
	}

	g_dwProcessID = GetWinIoProcessID();

	CTimeAxisAux::Instance()->Log("ProcessID = %d",g_dwProcessID);

	//�Ƿ�ʹ�ü��������жϣ���Ҫ������
	if(g_dwProcessID != (DWORD)-1)
	{
		if(IsWindows98())
		{
			return TimerInit_9x();
		}
		else
		{
			return TimerInit_NT();
		}
	}
	else
	{
		//98A������������٣��򲻹���
		if(IsWindows98A())
		{
			CTimeAxisAux::Instance()->Log("2");

			return FALSE;
			//���ֵ�ǿ��ŵ�
			UINT64 ui64Freq = ReadFreq();
			if(ui64Freq > 0)
			{
				g_ui64Freq = ui64Freq;

				return TRUE;
			}
			else
			{
				return TimerInit_Here();
			}
		}
		else
		{
			return TimerInit_Here();
		}
	}

	return FALSE;
}
//------------------------------------------------------------------------