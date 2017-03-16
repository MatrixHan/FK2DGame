/**
*	created:		2012-6-24   2:49
*	filename: 		MSpeed
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/AxisCommon.h"
#include <powrprof.h>
#include "../Include/MSpeed.h"
//------------------------------------------------------------------------
// ƫ����	�ж����������ṹ
// -4        ƫ����{32...16}	|  P DPL 0 D 1 10 000 
// 0         CS�Ĵ���ѡ����     |  ƫ����{15...0}
//------------------------------------------------------------------------
void SetClock9x(int freq)
{
	__asm
	{
		push eax	
		sidt [esp-02h]	// ȡ��IDT��ַ
		pop  ebx
						// ȡ��Ŀ���жϵ�ƫ��
		add  ebx,FREE_INT_NO*8+4
		
		cli 

		push ebp		// �������õ�Լ��Ҫ�ָ�ebp,esi�ȼĴ���
		push esi

		mov  ebp,[ebx]  // ����ɵ��жϵ�ַ
		mov  bp,[ebx-04h]

		lea  esi,RING0

		mov  [ebx-04h],si
		shr  esi,16
		mov  [ebx+02h],si

		int  FREE_INT_NO // �����ж�

		mov  [ebx-04h],bp// �ظ��ɵ�����
		shr  ebp,16
		mov  [ebx+02h],bp

		pop  esi		 // �������õ�Լ��Ҫ�ָ�ebp,esi�ȼĴ���
		pop  ebp

		sti

		//ret  2
	}
		
	return;

	__asm
	{
		//��Windows9x�½���ring0����еĲ���
RING0:		
		cli 
		mov al,36h
		out 43h,al	//д��8253���ƼĴ���������д0�Ŷ�ʱ��
		mov al,0	
		out 40h,al	//д��ʱֵ��λ
		mov al,0
		out 40h,al	//д��ʱֵ��λ
		sti
		iretd
	}
}
//------------------------------------------------------------------------
WORD   GetClock9x()
{
	BYTE byAl = 0;
	BYTE byAh = 0;
	__asm
	{
		push eax	
		sidt [esp-02h]	// ȡ��IDT��ַ
		pop  ebx
						// ȡ��Ŀ���жϵ�ƫ��
		add  ebx,FREE_INT_NO*8+4
		
		cli 

		push ebp		// �������õ�Լ��Ҫ�ָ�ebp,esi�ȼĴ���
		push esi

		mov  ebp,[ebx]  // ����ɵ��жϵ�ַ
		mov  bp,[ebx-04h]

		lea  esi,RING0

		mov  [ebx-04h],si
		shr  esi,16
		mov  [ebx+02h],si

		int  FREE_INT_NO // �����ж�

		mov  [ebx-04h],bp// �ظ��ɵ�����
		shr  ebp,16
		mov  [ebx+02h],bp

		pop  esi		 // �������õ�Լ��Ҫ�ָ�ebp,esi�ȼĴ���
		pop  ebp

		sti

		//ret  2
	}
		
	return	(WORD)byAh << 8 + byAl;

	__asm
	{
		//��Windows9x�½���ring0����еĲ���
RING0:		
		cli 
		mov al,36h
		out 43h,al	//д��8253���ƼĴ���������д0�Ŷ�ʱ��
		in  al,40h  //д��ʱֵ��λ
		mov byAl,al
		in  al,40h	//д��ʱֵ��λ
		mov byAh,al
		sti
		iretd
	}
}
//------------------------------------------------------------------------
__int64	GetCycleCountPerSecond()
{
	__int64 last_cycle = 0,now_cycle=0,cycle_count = 0;
	WORD    wLastTick = 0 ,wNowTick=0;
	DWORD   dwTickCount = 0;
	DWORD    wCycleLow=0,wCycleHight=0;

	__asm
	{
		push eax	
		sidt [esp-02h]	// ȡ��IDT��ַ
		pop  ebx
						// ȡ��Ŀ���жϵ�ƫ��
		add  ebx,FREE_INT_NO*8+4
		
		cli 

		push ebp		// �������õ�Լ��Ҫ�ָ�ebp,esi�ȼĴ���
		push esi

		mov  ebp,[ebx]  // ����ɵ��жϵ�ַ
		mov  bp,[ebx-04h]

		lea  esi,RING0

		mov  [ebx-04h],si
		shr  esi,16
		mov  [ebx+02h],si

		int  FREE_INT_NO // �����ж�

		mov  [ebx-04h],bp// �ظ��ɵ�����
		shr  ebp,16
		mov  [ebx+02h],bp

		pop  esi		 // �������õ�Լ��Ҫ�ָ�ebp,esi�ȼĴ���
		pop  ebp

		sti

		//ret  2
	}
		
	return (cycle_count/dwTickCount)*(1193181/1000);

	__asm
	{
		//��Windows9x�½���ring0����еĲ���
RING0:		
		cli 
		mov  al, 6		// ��ȡ��ǰ��TICK��			
		out  43h,al
        in   al, 40h
        mov  ah, al
        in   al, 40h
        xchg ah, al
        mov  wLastTick, ax // ���浱ǰTICK��

		rdtsc
		mov wCycleLow,eax
		mov wCycleHight,edx
	}
	last_cycle = ((__int64)wCycleHight << 32) | (__int64)wCycleLow;

	while(1)
	{
		__asm
		{
		mov  al, 6		   // ѭ������			
		out  43h,al
        in   al, 40h
        mov  ah, al
        in   al, 40h
        xchg ah, al
		mov  wNowTick, ax

		rdtsc
		mov wCycleLow,eax
		mov wCycleHight,edx
		}
		now_cycle = ((__int64)wCycleHight << 32) | (__int64)wCycleLow;

        if( wNowTick > wLastTick )
		{
			dwTickCount += wNowTick - wLastTick;
			cycle_count += now_cycle - last_cycle;
		}

		last_cycle = now_cycle;
		wLastTick = wNowTick;
		if( dwTickCount > 65535 )
		{
			__asm
			{
			sti
			iretd
			}
		}
	}
}
//------------------------------------------------------------------------
#define ALWAYS_ON	0x0
//------------------------------------------------------------------------
UINT uiDynThrottleAC = ALWAYS_ON;
UINT uiDynThrottleDC = ALWAYS_ON;
//------------------------------------------------------------------------
typedef BOOLEAN (WINAPI *LPFReadProcessorPwrScheme)(UINT, PMACHINE_PROCESSOR_POWER_POLICY);
typedef BOOLEAN (WINAPI *LPFWriteProcessorPwrScheme)(UINT, PMACHINE_PROCESSOR_POWER_POLICY);
typedef BOOLEAN (WINAPI *LPFGetActivePwrScheme)(PUINT);
typedef BOOLEAN (WINAPI *LPFSetActivePwrScheme)(UINT, PGLOBAL_POWER_POLICY, PPOWER_POLICY);
//------------------------------------------------------------------------
HINSTANCE hPwrfInstance = NULL;
//------------------------------------------------------------------------
LPFReadProcessorPwrScheme	lpfReadProcessorPwrScheme = NULL;
LPFWriteProcessorPwrScheme	lpfWriteProcessorPwrScheme = NULL;
LPFGetActivePwrScheme	lpfGetActivePwrScheme = NULL;
LPFSetActivePwrScheme	lpfSetActivePwrScheme = NULL;
//------------------------------------------------------------------------
/*
// �ر�SPEEDSTEP����
BOOL DisableSpeedSteping()
{
		if( hPwrfInstance == NULL )
		{
			hPwrfInstance = LoadLibrary("powrprof.dll");
			lpfReadProcessorPwrScheme = (LPFReadProcessorPwrScheme)GetProcAddress(hPwrfInstance,"ReadProcessorPwrScheme");
			lpfWriteProcessorPwrScheme = (LPFWriteProcessorPwrScheme)GetProcAddress(hPwrfInstance,"WriteProcessorPwrScheme");
			lpfGetActivePwrScheme = (LPFGetActivePwrScheme)GetProcAddress(hPwrfInstance,"GetActivePwrScheme");
			lpfSetActivePwrScheme = (LPFSetActivePwrScheme)GetProcAddress(hPwrfInstance,"SetActivePwrScheme");
		}

		if( lpfReadProcessorPwrScheme == NULL )
			return FALSE;
		if( lpfWriteProcessorPwrScheme == NULL )
			return FALSE;
		if( lpfGetActivePwrScheme == NULL )
			return FALSE;
		if( lpfSetActivePwrScheme == NULL )
			return FALSE;

		UINT		psIdx = 0;
		MACHINE_PROCESSOR_POWER_POLICY mppp;
		if( !lpfGetActivePwrScheme(&psIdx) )
			return FALSE;
		if( !lpfReadProcessorPwrScheme(psIdx, &mppp) ) 
			return FALSE;

		uiDynThrottleAC = mppp.ProcessorPolicyAc.DynamicThrottle;
		uiDynThrottleDC = mppp.ProcessorPolicyDc.DynamicThrottle;
   
		#define ALWAYS_ON   0x0
		mppp.ProcessorPolicyAc.DynamicThrottle = ALWAYS_ON;
		mppp.ProcessorPolicyDc.DynamicThrottle = ALWAYS_ON;
    
		if( !lpfWriteProcessorPwrScheme(psIdx, &mppp) )
			return FALSE;

		if( !lpfSetActivePwrScheme(psIdx, NULL, NULL) )
			return FALSE;

		return TRUE;
}

// ��ԭ��Դ������
bool ReturnOriginalPowerScheme( void )
{
   	UINT psIdx = 0;
	bool bRet = false;

    if ( hPwrfInstance )
    {
	    if ( lpfGetActivePwrScheme(&psIdx) ) 
        {
			MACHINE_PROCESSOR_POWER_POLICY mppp;
		    if ( lpfReadProcessorPwrScheme(psIdx, &mppp) ) 
            {
			    mppp.ProcessorPolicyAc.DynamicThrottle = uiDynThrottleAC;
			    mppp.ProcessorPolicyDc.DynamicThrottle = uiDynThrottleDC;

	    	    if ( lpfWriteProcessorPwrScheme(psIdx, &mppp) ) 
                {
				    if ( lpfSetActivePwrScheme(psIdx, NULL, NULL) ) 
                    {
					    bRet = true;
				    }
			    }
		    }
	    }
    }
	return bRet;
}
*/
//------------------------------------------------------------------------
MACHINE_PROCESSOR_POWER_POLICY g_mppp;
//------------------------------------------------------------------------
// �ر�SPEEDSTEP����
BOOL DisableSpeedSteping()
{
		if( hPwrfInstance == NULL )
		{
			hPwrfInstance = LoadLibrary("powrprof.dll");
			lpfReadProcessorPwrScheme = (LPFReadProcessorPwrScheme)GetProcAddress(hPwrfInstance,"ReadProcessorPwrScheme");
			lpfWriteProcessorPwrScheme = (LPFWriteProcessorPwrScheme)GetProcAddress(hPwrfInstance,"WriteProcessorPwrScheme");
			lpfGetActivePwrScheme = (LPFGetActivePwrScheme)GetProcAddress(hPwrfInstance,"GetActivePwrScheme");
			lpfSetActivePwrScheme = (LPFSetActivePwrScheme)GetProcAddress(hPwrfInstance,"SetActivePwrScheme");
		}

		if( lpfReadProcessorPwrScheme == NULL )
			return FALSE;
		if( lpfWriteProcessorPwrScheme == NULL )
			return FALSE;
		if( lpfGetActivePwrScheme == NULL )
			return FALSE;
		if( lpfSetActivePwrScheme == NULL )
			return FALSE;

		UINT		psIdx = 0;
		MACHINE_PROCESSOR_POWER_POLICY mppp;
		if( !lpfGetActivePwrScheme(&psIdx) )
			return FALSE;
		if( !lpfReadProcessorPwrScheme(psIdx, &g_mppp) ) 
			return FALSE;

		mppp = g_mppp;

		//AC
		mppp.ProcessorPolicyAc.DynamicThrottle = PO_THROTTLE_NONE;
		for(int i = 0;i < mppp.ProcessorPolicyAc.PolicyCount;i++)
		{
			mppp.ProcessorPolicyAc.Policy[i].AllowDemotion = 0;
			mppp.ProcessorPolicyAc.Policy[i].AllowPromotion = 0;
			mppp.ProcessorPolicyAc.Policy[i].DemotePercent = 0;
			mppp.ProcessorPolicyAc.Policy[i].PromotePercent = 0;
		}

		//DC
		mppp.ProcessorPolicyDc.DynamicThrottle = PO_THROTTLE_NONE;
		for( int i = 0;i < mppp.ProcessorPolicyDc.PolicyCount;i++)
		{
			mppp.ProcessorPolicyDc.Policy[i].AllowDemotion = 0;
			mppp.ProcessorPolicyDc.Policy[i].AllowPromotion = 0;
			mppp.ProcessorPolicyDc.Policy[i].DemotePercent = 0;
			mppp.ProcessorPolicyDc.Policy[i].PromotePercent = 0;
		}

		if( !lpfWriteProcessorPwrScheme(psIdx, &mppp) )
			return FALSE;

		if( !lpfSetActivePwrScheme(psIdx, NULL, NULL) )
			return FALSE;

		return TRUE;
}
//------------------------------------------------------------------------
// ��ԭ��Դ������
bool ReturnOriginalPowerScheme( void )
{
   	UINT psIdx = 0;
	bool bRet = false;

    if ( hPwrfInstance && lpfWriteProcessorPwrScheme && lpfGetActivePwrScheme && lpfSetActivePwrScheme)
    {
	    if ( lpfGetActivePwrScheme(&psIdx) ) 
        {
			if ( lpfWriteProcessorPwrScheme(psIdx, &g_mppp) ) 
			{
				if ( lpfSetActivePwrScheme(psIdx, NULL, NULL) ) 
				{
					bRet = true;
				}
			}
	    }
    }
	return bRet;
}
//------------------------------------------------------------------------