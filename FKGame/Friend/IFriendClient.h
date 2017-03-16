/**
*	created:		2012-6-30   4:57
*	filename: 		IFriendClient
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ģ��ӿ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#ifdef FRIENDCLIENT_EXPORTS
#define FRIENDCLIENT_API extern "C" __declspec(dllexport)
#else
#define FRIENDCLIENT_API extern "C" __declspec(dllimport)
#endif
//------------------------------------------------------------------------
#define FRIENDCLIENT_VERSION 1
//------------------------------------------------------------------------
// ����ģ��ӿ�
struct IFriendClient
{	
	//-------------------------------------------------------------------
	// ��  �����ͷ�ģ�����Դ 
	// ��  ����void	
	// ����ֵ��void
	//-------------------------------------------------------------------
	virtual void Release(void) = 0;

	//-------------------------------------------------------------------
	// ��  ������ʾ���Ѵ���
	// ��  ����[����] BOOL bShow - �Ƿ���ʾ
	// ��  ����[����] BOOL bTurn - �Ƿ�������ʾ/����
	// ����ֵ��void 
	//-------------------------------------------------------------------	
	virtual void ShowFriendWnd(BOOL bShow, BOOL bTurn = FALSE) = 0;

	//-------------------------------------------------------------------
	// ��  ����С�˵�ʱ��������������
	// ����ֵ��void 
	//-------------------------------------------------------------------	
	virtual void Close() = 0;
	
	//-------------------------------------------------------------------
	// ��  ������Ӻ��ѹ���
	// ��  ����[����] long lFriendId - ����DB ID
	// ����ֵ��void 
	//-------------------------------------------------------------------

	virtual void AddFriend(long lFriendId) = 0;

	//-------------------------------------------------------------------
	// ��  ������Ӻ��ѹ���
	// ��  ����[����] LPCSTR lpszName - ��������
	// ����ֵ��void 
	//-------------------------------------------------------------------	
	virtual void AddFriend(LPCSTR lpszName) = 0;
};
//------------------------------------------------------------------------
// ����ģ���������
class CFriendClientHelper
{
private:
	typedef BOOL (*ProcCreateFriendClient)(DWORD dwVersion, 
										   IClientGlobal * pGlobal, 
										   IFriendClient ** ppModule); 

private:
	HINSTANCE m_hDll;
	IFriendClient * m_pModule; // ����ģ��ָ��

public:
	CFriendClientHelper(void) : m_hDll(NULL), m_pModule(NULL)
	{		
	}

	~CFriendClientHelper(void)
	{
		Close();
	}
	
	// ��������ģ��
	BOOL Create(IClientGlobal * pGlobal)
	{
		if (m_pModule != NULL)
		{
			return TRUE;
		}
		
		try
		{
			// ���붯̬��
#ifdef _DEBUG
			m_hDll = LoadLibrary("FKFriendClient_D.dll");
			if (m_hDll == NULL)
			{
				throw "�޷����� FKFriendClient_D.dll";
			}
#else
			m_hDll = LoadLibrary("FKFriendClient.dll");
			if (m_hDll == NULL)
			{
				throw "�޷����� FKFriendClient.dll";
			}
#endif
			
			// ��ȡ������ַ
			ProcCreateFriendClient createProc;
			createProc = (ProcCreateFriendClient)GetProcAddress(m_hDll, "CreateFriendClient");
			if(createProc == NULL)
			{
				throw "Can not GetProAddress('CreateFriendClient')";
			}
			
			if(!createProc(FRIENDCLIENT_VERSION, pGlobal, &m_pModule))
			{
				throw "CreateFriendClient() catch error!";
			}
			
			return (m_pModule != NULL);
		}
		catch(LPCSTR szMsg)
		{
			OutputDebugString(szMsg);
			OutputDebugString("\r\n");
			return FALSE;
		}
		catch(...)
		{
			OutputDebugString("CreateFriendClient() catch error\r\n");
			return FALSE;
		}
	}

	// ��ȡģ��ָ��
	IFriendClient * GetModule(void) const
	{
		return m_pModule;
	}

	// ��ȡģ��ָ��
	IFriendClient * operator ->(void) const
	{
		return m_pModule;
	}

	// ����ģ��
	void Close(void)
	{
		if (m_pModule != NULL)
		{
			m_pModule->Release();
			m_pModule = NULL;
		}

		if (m_hDll != NULL)
		{
			FreeLibrary(m_hDll);
			m_hDll = NULL;
		}
	}
};
//------------------------------------------------------------------------
// ��  ������������ģ��
// ��  ����[����] DWORD dwVersion - ģ��İ汾��
// ��  ����[����] IClientGlobal * pGlobal - ȫ�ֹ�������ָ��
// ��  ����[���] IFriendClient ** ppModule - ָ��ģ�����ɺ��ַ��ָ��
// ����ֵ��BOOL - ģ�鴴���ɹ�����TRUE,���򷵻�FALSE
//-----------------------------------------------------------------------------
FRIENDCLIENT_API BOOL CreateFriendClient(DWORD dwVersion, 
										 IClientGlobal * pGlobal, 
										 IFriendClient ** ppModule);
//------------------------------------------------------------------------