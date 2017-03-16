/**
*	created:		2012-6-25   0:09
*	filename: 		ThreadRoute
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/Thread/IThreadRoute.h"
#include "Func.h"
#include <list>
using std::list;
//------------------------------------------------------------------------
#define FKTraceColor		g_DefaultTrace.ThreadSafeDebugPrintColor
//------------------------------------------------------------------------
#ifndef	MY_TRACE_COLOR
#define MY_TRACE_COLOR		RGB(146,0,123)
#endif
//------------------------------------------------------------------------
#ifdef _DEBUG
	#define VerifyR(Expression, ReturnValue, Description ) \
		if (Expression) \
	{\
		FKTraceColor(MY_TRACE_COLOR, "���� : %s[%d]\r\n\t%s\r\n",__FILE__,__LINE__,Description);\
		return ReturnValue;\
	}
	#define AssertR(Expression, ReturnValue, Description ) \
		if (!(Expression)) \
	{\
		FKTraceColor(MY_TRACE_COLOR, "ERROR : %s[%d]\r\n\t%s\r\n",__FILE__,__LINE__,Description);\
		return ReturnValue;\
	}
	#define Assert(Expression) \
		if (!(Expression))\
	{\
		FKTraceColor(MY_TRACE_COLOR, "���� : %s[%d]\r\n\t%s\r\n",__FILE__,__LINE__,#Expression);\
		return 0;\
	}
	#define	TraceLn			g_DefaultTrace.DebugPrintLn
#else
	#define VerifyR(Expression, ReturnValue, Description ) \
		if (Expression) \
	{\
		return ReturnValue;\
	}
	#define AssertR(Expression, ReturnValue, Description ) \
		if (!(Expression)) \
	{\
		return ReturnValue;\
	}
	#define Assert(Expression) \
		if (!(Expression))\
	{\
		return 0;\
	}
	#define	TraceLn			__noop
#endif
//------------------------------------------------------------------------
#define SYS_EVENT_COUNT			1
#define MAX_USER_EVENT_COUNT	(64 - SYS_EVENT_COUNT)		//WaitForMultiObject���֧��64���¼�
//------------------------------------------------------------------------
#ifdef _DEBUG
	#define __USE_MALLOC
#endif
//------------------------------------------------------------------------
struct CTimerNode
{
	CTimerNode(ITimerEventSink *pTimerSink,DWORD dwIDEvent,DWORD dwElapse,DWORD dwParam)
	{
		this->pTimerSink = pTimerSink;
		this->dwIDEvent = dwIDEvent;
		this->dwElapse = dwElapse;
		this->dwParam = dwParam;
		dwNextTick = GetTickCount() + dwElapse;
	}
	BOOL operator < (const CTimerNode& t) const
	{
		return dwNextTick < t.dwNextTick;
	}
	BOOL operator == (const CTimerNode& t) const
	{
		return t.pTimerSink == this->pTimerSink && t.dwIDEvent == dwIDEvent;
	}
	ITimerEventSink *pTimerSink;
	DWORD dwIDEvent;
	DWORD dwElapse;
	DWORD dwParam;
	DWORD dwNextTick;
};
//------------------------------------------------------------------------
struct EVENT_INFO
{
	DWORD		dwEventID;
	IEventSink*	pEventSink;
	HANDLE		hLocalCreatedHandle;	// ��IThreadRoute������ά����event
	void Close()
	{
		if(hLocalCreatedHandle)
		{
			CloseHandle(hLocalCreatedHandle);
		}
	}
};
//------------------------------------------------------------------------
enum ThreadCtrlCmd
{
	ctrlNone = 0,
	ctrlExit = 1,			// ֪ͨ�߳��˳�
	ctrlFlipEventBuf = 2,	// ���������ӻ����Eventʱ��֪ͨThreadRoute����Event�б�
	ctrlNewTimer = 4,		// �����һ���µ�timer,���µ�timer��Ҫ�޸�WaitForMultipleObjectsEx()�е�time outֵʱ��������ֵ
	ctrlUnknown = 0			// 
};
//------------------------------------------------------------------------
struct ITrace;
class CThreadRoute : public IThreadRoute  
{
public:
	int			m_nUserEventCount;						//
	HANDLE		m_EventBuf[MAXIMUM_WAIT_OBJECTS];		// m_EventBuf[n]��m_EventInfo[n - 1]��Ӧ
	EVENT_INFO	m_EventInfo[MAX_USER_EVENT_COUNT];		// ǰSYS_EVENT_COUNT������ϵͳ�¼�
	DWORD		m_dwCtrlCmdMask;						// ��������λ���ڵ�

	void ClearCtrEventMask(ThreadCtrlCmd ctrlClearMask);
	void OnTriggerTimer(DWORD &dwNextTimeOut,DWORD& dwTimerNextTick);
	BOOL TriggerCtrlEvent(ThreadCtrlCmd cmd,DWORD dwWaitTime = 0);
	void OnTheadExit();
	int GetTotalEventCount()
	{
		return m_nUserEventCount + SYS_EVENT_COUNT;
	}

	void SetRunning(BOOL bRunning);
	BOOL Create(ITrace* pTrace);
	void Close();
	CThreadRoute();
	~CThreadRoute();

	// �����߳�
	// �ɽ����е����ģ��ӵ�IThreadRoute�Ժ������̣߳�Ҳ�����������Ĺ��������ģ�齫���ѵ��¼���̬��ӽ���
	virtual BOOL Start();
	
	// ֹͣ�߳�,����ָ���ȴ�ʱ��dwWaitTick���Ժ���Ϊ��λ
	virtual BOOL Stop(DWORD dwWaitTime = 3000);
	
	// �߳�����û��?
	virtual BOOL IsRunning();
	
	/*
	 	���ܣ����ģ�齫���ѵ�һ���¼��ҽӵ���ǰ�߳���
		���룺
			hEvent
			pEventSink	�������¼�����ʱ���ص��˽ӿ�
		���أ�
			dwEventID	�����ص����¼�����ʱ���ش����¼�������IEventSink�ı�ʶ
	*/
	virtual BOOL AddEvent(HANDLE hEvent,IEventSink* pEventSink,/*out*/DWORD& dwEventID);

	// ͬ�ϣ���ͬ��������ϵͳ�Զ���æ����һ��Event
	virtual BOOL AddEvent(IEventSink* pEventSink,/*out*/DWORD& dwEventID);

	virtual HANDLE GetEventHandle(DWORD dwEventID);
	
	// ���ģ���������ʱ���������߳�����¼�����
	virtual BOOL RemoveEvent(DWORD dwEventID);
	
	// ��ʱ����
	virtual BOOL SetTimer(ITimerEventSink *pTimerSink,DWORD dwIDEvent,DWORD dwElapse,DWORD dwParam);
	
	// ���ģ���������ʱ���������߳������ʱ����
	virtual BOOL KillTimer(ITimerEventSink *pTimerSink,DWORD dwIDEvent);

	virtual void Release();

	virtual HANDLE GetThreadHandle()
	{
		return 	m_hThread;
	}
	virtual DWORD ResumeThread()
	{
		if(m_bSuspend)
		{
			m_bSuspend = FALSE;
			return ::ResumeThread(m_hThread);
		}
		return 0;
	}
	virtual DWORD SuspendThread()
	{
		if(m_bSuspend)
			return 1;
		m_bSuspend = TRUE;
		return ::SuspendThread(m_hThread);
	}

private:
	DWORD		m_dwThreadID;
	HANDLE		m_hThread;
	BOOL		m_bIsRunning;
	int			m_bSuspend;				// �Ƿ��Ѿ�����
	HANDLE 		m_hCtrlEvent;			// ������һ��ThreadCtrlCmdָ��λ��Ȼ�󴥷�m_hCtrlEvent
	DWORD		m_dwEventIDLoop;		// ÿ����һ��event����ID���Ǽ�1����ʹÿ��event���ǻ��Ψһ��ID
	list<CTimerNode>		m_TimerList;
	CCriticalSectionLock	m_LockTimerList;
	CCriticalSectionLock	m_LockEventBuf;
};
//------------------------------------------------------------------------