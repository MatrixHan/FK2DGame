/**
*	created:		2012-6-24   22:20
*	filename: 		IThreadRoute
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
/*
	�̹߳���
		����һ���߳̿���ͬʱ��������63���ⲿ�¼�������Խ�ͨѶ�¼��ŵ����߳��У�
		��ͨѶ�¼�����ʱ�����¼�֪ͨת��ͨѶ�㴦��
			��һ����Ҫ��;�Ƕ�ʱ�����񣬲���Ҫ�����ںˣ������ڽ���ʱ�����п���ʵ�ֲ������������Ķ�ʱ��
		��ʱ����������ṩ���õ�״̬����
		
	ʵ�ַ�����
			��ÿ���¼��ŵ�WaitForMultiObject��EventBuf�У���ĳ�¼�����ʱ������ָ���ĵ���

	�ṹͼ��
			������� <=> IThreadRoute <=> ���ģ��

			���������һ��dialog����һ��service
	
	��;��
			1�������ͨѶClientSocket�ҽӵ�һ���߳��У�
			2��һ�����������������Ҷ��������
 */
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
#define VERSION_ThreadRoute	1
//------------------------------------------------------------------------
#define INVALID_EVENT_ID	-1
//------------------------------------------------------------------------
struct ITrace;
struct IEventSink;
struct ITimerEventSink;
//------------------------------------------------------------------------
struct IThreadRoute
{
	//�߳�״̬����

	// �����߳�
	// �ɽ����е����ģ��ӵ�IThreadRoute�Ժ������̣߳�Ҳ�����������Ĺ��������ģ�齫���ѵ��¼���̬��ӽ���
	virtual BOOL Start() = NULL;
	
	// ������ֹͣ�߳�,����ָ���ȴ�ʱ��dwWaitTick���Ժ���Ϊ��λ
	// ע�⣺�˽Ӻ���ֻ�ܹ���������ã�ThreadRoute�̱߳�����ֹͣ���ѣ�
	//		 ���ģ�����ThreadRoute����ͬһ�̣߳����Ե��ô˺�������������
	virtual BOOL Stop(DWORD dwWaitTime = 3000) = NULL;
	
	// �߳�����û��?
	virtual BOOL IsRunning() = NULL;


	//�¼�����
	/*
	 	���ܣ����ģ�齫���ѵ�һ���¼��ҽӵ���ǰ�߳���
		���룺
			hEvent		������hEvent��socket�����
			pEventSink	�������¼�����ʱ���ص��˽ӿ�
		���أ�
			dwEventID	�������¼�����ʱ������IEventSink::OnEvent(dwEventID)
	*/
	virtual BOOL AddEvent(HANDLE hEvent,IEventSink* pEventSink,/*out*/DWORD& dwEventID) = NULL;

	// ͬ�ϣ���֮ͬ������hEvent��IThreadRoute������ɾ�����Ƽ�ʹ������ӿ�
	virtual BOOL AddEvent(IEventSink* pEventSink,/*out*/DWORD& dwEventID) = NULL;

	// ����hEvent����IThreadRoute�Զ������¼�ʱ���û�������Ҫ���Ҵ˾��
	virtual HANDLE GetEventHandle(DWORD dwEventID) = NULL;
	
	// ���һ���¼�����
	virtual BOOL RemoveEvent(DWORD dwEventID) = NULL;
	

	// ��ʱ������
	// ���Ӷ�ʱ��
	// ע�⣺�������ͬһ����ʱ��ʱ��ÿ����ʱ������Ч
	//       dwElapse�����ƣ�dwElapse > 0 && dwElapes < 0x7FFFFFFF
	virtual BOOL SetTimer(ITimerEventSink *pTimerSink,DWORD dwIDEvent,DWORD dwElapse,DWORD dwParam = 0) = NULL;
	
	// ���ģ������IThreadRoute�����ʱ����
	virtual BOOL KillTimer(ITimerEventSink *pTimerSink,DWORD dwIDEvent) = NULL;

	virtual void Release() = NULL;

	//////////////////// �����̹߳����� //////////////////////
	virtual HANDLE GetThreadHandle() = NULL;
	virtual DWORD ResumeThread() = NULL;
	virtual DWORD SuspendThread () = NULL;
};
//------------------------------------------------------------------------
//�����ģ��
struct IEventSink
{
	// ���¼�������ʱ���ص��˽ӿڣ�����dwEventID����������һ���ӿڸ����Event��
	virtual void OnEvent(DWORD dwEventID) = NULL;
	
	// ���߳��˳�ʱ��hEvent��Чʱ�����ô˽ӿڣ�һ�㲻��ʵ�ִ˽ӿ�
	virtual void OnEventExit(DWORD dwEventID){;}
};
//------------------------------------------------------------------------
// ʱ�������ʵ�ֵĽӿ�
// Ϊ�����mud�е�ITimerSink���ͻ������Ϊ�ĳ�ITimerEventSink
struct ITimerEventSink
{
	// ʱ���¼�����
	// ����ֵΪ�´�ִ�д�timer��ʱ�䣬��msΪ��λ
	// ���ͣ�ô�timer���践��-1��ϵͳ�Զ�KillTimer()
	virtual DWORD OnTimer(DWORD dwTimerID = 0,DWORD dwParam = 0) = NULL;
	// ���߳��˳�ʱ�����ô˽ӿڣ���KillTimer()ʱ���ص��˽ӿڡ�һ�㲻��ʵ�ִ˽ӿ�
	virtual void OnExitTimer(DWORD dwTimerID){;}
};
//------------------------------------------------------------------------
// ����ClientSocket.lib�е�CreateThreadRoute()����һ��IThreadRoute����
extern "C"  BOOL CreateThreadRoute(ITrace* pTrace,IThreadRoute** ppThreadRoute);
//------------------------------------------------------------------------
class CThreadRouteHelper
{
public:
	typedef BOOL (*ProcCreateThreadRoute)(DWORD dwQueryVersion,ITrace* pTrace,IThreadRoute** ppThreadRoute);
	CThreadRouteHelper()
	{
		m_hdll = NULL;
		m_pThreadRoute = NULL;
	}
	~CThreadRouteHelper()
	{
		Close();
	}
	void Close()
	{
		if(m_pThreadRoute)
		{
			m_pThreadRoute->Release();
			m_pThreadRoute = NULL;
		}
		if(m_hdll)
		{
			FreeLibrary(m_hdll);
			m_hdll = NULL;
		}
	}
	BOOL Create(ITrace* pTrace)
	{
		Close();
		try
		{
#if defined(_DEBUG)
			m_hdll = ::LoadLibrary("FKSocket_D.dll");
			if(m_hdll == NULL)
				throw "�޷����� FKSocket_D.dll";
#else
			m_hdll = ::LoadLibrary("FKSocket.dll");
			if(m_hdll == NULL)
				throw "�޷����� FKSocket.dll";
#endif
			ProcCreateThreadRoute proc = (ProcCreateThreadRoute)GetProcAddress(m_hdll, "dllCreateThreadRoute");
			if(proc == NULL)
				throw "Can not GetProcAddress('dllCreateThreadRoute')!";
			if(!proc(VERSION_ThreadRoute,pTrace,&m_pThreadRoute))
				throw "CreateThreadRoute() failed";
			return m_pThreadRoute != NULL;

		}
		catch(LPCSTR szMsg)
		{
			OutputDebugString(szMsg);OutputDebugString("\r\n");
			return FALSE;
		}
		catch(...)
		{
			OutputDebugString("Error : CThreadRouteHelper::Create() : catch error!\r\n");
			return FALSE;
		}
		return TRUE;
	}

public:
	IThreadRoute* m_pThreadRoute;
private:
	HINSTANCE		m_hdll;
};
//------------------------------------------------------------------------