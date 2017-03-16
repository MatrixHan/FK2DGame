/**
*	created:		2012-6-29   15:41
*	filename: 		CoolTimeMgr
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��ȴʱ����������ⲿ�ӿڣ��ṩ��ȴʱ��Ļ�������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../SceneObj/IThingPart.h"
//------------------------------------------------------------------------
//��ȴʱ��۲��ߣ��������Ĳ�����ʼ��ȴʱ�յ�֪ͨ��
//�ͻ��˿���ʵ������ӿ�����ɶ����Ŀ�ʼ���š�
class IColdTimeObserver
{
public:
	// ��  ������ȴ��ʼ
	// ��  ����[����] long lColdTime - ��ȴʱ�䳤��(����)
	// ����ֵ��void
	// ע  �⣺�ڱ������в���ȡ��������ȴ��ʼ�¼�
	virtual void OnStart(long lColdTime) = NULL;
};
//------------------------------------------------------------------------
class IColdTimeMgr : public IThingPart
{
public:
	// ��  ������ѯ�Ƿ���Խ���ĳ������
	// ��  ����[����] long lType  - ��������
	// ��  ����[����] long lValue - ����ֵ,���������е�ΨһID 
	// ����ֵ��BOOL - ������Բ���,��û������ȴ,�򷵻�true,���򷵻�false
	virtual BOOL CanOperate(long lType, long lValue) = NULL;

	// ��  ��������ĳ������,�ڲ���������ȴ��ʼ
	// ��  ����[����] long lType  - ��������
	// ��  ����[����] long lValue - ����ֵ,���������е�ΨһID 
	// ����ֵ��BOOL - ����ɹ��򷵻�true,���򷵻�false
	virtual BOOL Operate(long lType, long lValue) = NULL;

	// ��  ������ȡĳ����������ȴʱ�䳤��(����)
	// ��  ����[����] long lType  - ��������
	// ��  ����[����] long lValue - ����ֵ,���������е�ΨһID 
	// ����ֵ��long - ���ظò�������ȴʱ�䳤��(����)
	virtual long GetColdTime(long lType, long lValue) = NULL;

	// ��  ��������ĳ����������ȴʱ�䳤��(����)
	// ��  ����[����] long lType  - ��������
	// ��  ����[����] long lValue - ����ֵ,���������е�ΨһID 
	// ��  ����[����] long lCodeTime - ��ȴ�¼�����(����)
	// ����ֵ��void
	virtual void SetColdTime(long lType, long lValue, long lCodeTime) = NULL;

	// ��  ������ȡĳ�������Ŀ�ʼ��ȴʱ��
	// ��  ����[����] long lType  - ��������
	// ��  ����[����] long lValue - ����ֵ,���������е�ΨһID 
	// ����ֵ��long - ���ض�Ӧ�����Ŀ�ʼ��ȴʱ��
	virtual long GetStartTime(long lType, long lValue) = NULL;

	// ��  ����������ȴ��ʼ�¼�
	// ��  ����[����] long lType  - ��������
	// ��  ����[����] long lValue - ����ֵ,���������е�ΨһID 
	// ��  ����[����] IColdTimeObserver *pColdTimeObserver - ��ȴ��ʼ�¼������� 
	// ����ֵ��void
	virtual void Subscibe(long lType, long lValue,
		IColdTimeObserver *pColdTimeObserver) = NULL;

	// ��  ����ȡ��������ȴ��ʼ�¼�
	// ��  ����[����] long lType  - ��������
	// ��  ����[����] long lValue - ����ֵ,���������е�ΨһID 
	// ��  ����[����] IColdTimeObserver *pColdTimeObserver - ��ȴ��ʼ�¼������� 
	// ����ֵ��void
	virtual void Unsubscibe(long lType, long lValue,
		IColdTimeObserver *pColdTimeObserver) = NULL;

	virtual void Release() = NULL;
	
	virtual void SetData(LPCSTR buf, int nLen) = NULL;
	virtual BOOL GetData(LPSTR buf, int &nLen) = NULL;
};
//------------------------------------------------------------------------
struct IColdTimeSystem
{
	virtual void Release() = NULL;
	
	// ����: ����һ����ȴ������
	virtual IColdTimeMgr* CreateColdTimeMgr() = NULL;
	// ��  ������ȡĳ����������ȴʱ�䳤��(����)
	// ��  ����[����] long lType  - ��������
	// ��  ����[����] long lValue - ����ֵ,���������е�ΨһID 
	// ����ֵ��long - ���ظò�������ȴʱ�䳤��(����)
	virtual long GetColdTime(long lType, long lValue) = NULL;
};
//------------------------------------------------------------------------
struct ITrace;
struct IClientGlobal;
struct IScriptUpdateManager;
//------------------------------------------------------------------------
class CColdTimeSystemHelper
{
private:
	typedef BOOL (*ProcCreateColdTimeSysObject)(
		IColdTimeSystem **ppColdTimeSys, 
		ITrace *pTrace, IScriptUpdateManager *pSrciptUpdateManager,
		LPCSTR szFile, IClientGlobal *pClientGlobal);
	
public:
	CColdTimeSystemHelper()
	{
		m_hDll = NULL;
		m_pColdTimeSys = NULL;
		m_szErrMsg[0] = 0;
	}
	~CColdTimeSystemHelper()
	{
		Close();
		if (m_hDll != NULL)//ע�⣬�����lib��ʽ������m_hDll����NULL,���������::FreeLibary()���ᱻ����
		{
			::FreeLibrary(m_hDll);
			m_hDll=NULL;
		}
	}	
	//�ļ������Ϊ�գ���ȡĬ���ļ���
	BOOL Create(ITrace *pTrace, IScriptUpdateManager *pSrciptUpdateManager,
		LPCSTR szFile = NULL, IClientGlobal *pClientGlobal = NULL)
	{
		Close();
		try
		{
			if (m_hDll == NULL)
			{
				//��һ�ε���ʱδ����dll,������м���dll
#ifdef _DEBUG
				m_hDll = ::LoadLibrary("FKColdTime_D.dll");
				if (m_hDll == NULL)
					throw "�޷����� FKColdTime_D.dll";
#else
				m_hDll = ::LoadLibrary("FKColdTime.dll");
				if (m_hDll == NULL)
					throw "�޷����� FKColdTime.dll";
#endif
			}
			//������dll��������������ӿڶ���Ĵ�������
			ProcCreateColdTimeSysObject proc = NULL;
			proc=(ProcCreateColdTimeSysObject)::GetProcAddress(m_hDll,
				"CreateColdTimeSysObject");
			if (proc == NULL)
				throw "Can't GetProcAddress('CreateColdTimeSysObject')";
			if (!proc(&m_pColdTimeSys, pTrace, pSrciptUpdateManager, szFile, pClientGlobal))
				throw "CreateColdTimeSysObject() error!";
			
			return TRUE;//�ӿڶ��󴴽��ɹ�
		}
		catch (LPCSTR szMsg) 
		{
			lstrcpyn(m_szErrMsg, szMsg, sizeof(m_szErrMsg));
			return FALSE;
		}
		catch(...)
		{
			lstrcpyn(m_szErrMsg, "Unknown Error!", sizeof(m_szErrMsg));
			return FALSE;
		}
	}
	void Close()//���ٽӿڶ���
	{
		if (m_pColdTimeSys != NULL)
		{
			m_pColdTimeSys->Release();
			m_pColdTimeSys = NULL;			
		}
	}
	//����->,��helper���������ֱ��תΪ�ӿڵ�����
	IColdTimeSystem* operator ->()
	{
		return m_pColdTimeSys;
	}
	//�жϽӿ�ָ���Ƿ���Ч
	BOOL IsValid()
	{
		return (m_pColdTimeSys != NULL);
	}
	char* GetErrMsg()
	{
		return m_szErrMsg;
	}
	IColdTimeSystem* GetInterface()
	{
		return m_pColdTimeSys;
	}
private:
	IColdTimeSystem *m_pColdTimeSys;					//�ӿڶ���
	HINSTANCE m_hDll;									//��̬���ӿ��������־
	char m_szErrMsg[128];								//������Ϣ
};
//------------------------------------------------------------------------
