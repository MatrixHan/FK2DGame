/**
*	created:		2012-6-28   0:46
*	filename: 		IChatClient
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ģ��Ľӿ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
struct IPaoPao;
struct IClientGlobal;
struct ICanvas;
//------------------------------------------------------------------------
typedef __int64 UID;
//------------------------------------------------------------------------
// ����ģ��ӿ�
struct IChatClient
{
	//-------------------------------------------------------------------
	// ��  �����û����� 
	// ��  ����[����] int nChannel - ����Ƶ��
	// ��  ����[����] const char * pszReceiver - ��Ϣ�Ľ�����
	// ��  ����[����] const char * pszText - ���Ե��ı�
	// ����ֵ��BOOL - ��Ҫ˵�Ļ������Ч�ҿ��Է��ͷ���TRUE�����򷵻�FALSE
	//-------------------------------------------------------------------
	virtual	BOOL Speak(int nChannel, const char * pszReceiver, const char * pszText) = 0;

	//-------------------------------------------------------------------
	// ��  ���� ��������Ƶ���Ŀ���
	// ��  ����[����] int nChannel - ����Ƶ��
	// ��  ����[����] BOOL bOnOff - �Ƿ��
	// ����ֵ��void
	//-------------------------------------------------------------------
	virtual void ChannelOn(int nChannel, BOOL bOnOff) = 0;

	//-------------------------------------------------------------------
	// ��  �����趨�Ƿ���ʾ������Ϣ���� 
	// ��  ����[����] int nType - ��ʾ��ʽ
	// ����ֵ��void
	//-------------------------------------------------------------------
	virtual void ShowWindow(int nType) = 0;

	//-------------------------------------------------------------------
	// ��  ����������Ϣ��ʾ���ڵ�͸���� 
	// ��  ����[����] int nLevel - ͸����
	// ����ֵ��void
	//-------------------------------------------------------------------
	virtual void SetWindowTrans(int nLevel) = 0;	

	//-------------------------------------------------------------------
	// ��  �����ͷ�����ģ��ʹ�õ���Դ
	// ��  ����void
	// ����ֵ��void
	//-------------------------------------------------------------------
	virtual void Release(void) = 0;
	
	//-------------------------------------------------------------------
	// ��  ������ʾϵͳƵ����Ϣ
	// ��  ����const char * pszText - ��ʾ�ı�
	// ����ֵ��void
	//-------------------------------------------------------------------
	virtual void DisplaySysMessage(const char * pszText) = 0;
	
	//-------------------------------------------------------------------
	// ��  ��������һ������ 
	// ��  ����UID uid - �������ݵ�����UID
	// ����ֵ��void
	//-------------------------------------------------------------------
	virtual IPaoPao * CreatePaoPao(UID uid) = 0;

	//-------------------------------------------------------------------
	// ��  ������ʾ����ͼ
	//-------------------------------------------------------------------
	virtual void OnSysMsgDraw(ICanvas * pCanvas) = 0;

	//-------------------------------------------------------------------
	// ��  ������ʾ������ʾ��Ϣ
	// ��  ����int nType - ��ʾ�����ͣ�ȷ����ʾ��λ�ã�(SysMessageDef.h)
	// ��  ����const char * lpszText - ��ʾ���ı�
	// ����ֵ��void
	//-------------------------------------------------------------------
	virtual void DisplayFloatMessage(int nType, const char * lpszText) = 0;

	//-------------------------------------------------------------------
	// ��  ������ո�����ʾ��Ϣ
	// ��  ����int nType - ��ʾ�����ͣ�ȷ����ʾ��λ�ã�(SysMessageDef.h),-1Ϊ���еĵ�
	// ����ֵ��void
	//-------------------------------------------------------------------
	virtual void ClearFloatMessage(int nType) = 0;	

	//-------------------------------------------------------------------
	// ��ʾ�ƺŴ���
	//-------------------------------------------------------------------	
	virtual void ShowTitleWnd(BOOL bTrue,BOOL bTurn = TRUE) = 0;
};
//------------------------------------------------------------------------
#ifdef FKCHATCLIENT_EXPORTS
#define CHATCLIENT_API extern "C" __declspec(dllexport)
#else
#define CHATCLIENT_API extern "C" __declspec(dllimport)
#endif
//------------------------------------------------------------------------
#define CHATCLIENT_VERSION 1	// ����ģ��İ汾��
//------------------------------------------------------------------------
// ��  ������������ģ��
// ��  ����[����] DWORD dwVersion - ����ģ��İ汾��
// ��  ����[����] IClientGlobal * pGlobal - ȫ�ֹ�������ָ��
// ��  ����[���] IChatClient ** ppModule - ����ģ�����ɺ��ַ��ָ��
// ����ֵ��BOOL - ģ�鴴���ɹ�����TRUE,���򷵻�FALSE
//-------------------------------------------------------------------
CHATCLIENT_API BOOL CreateChatClient(DWORD dwVersion, IClientGlobal * pGlobal, IChatClient ** ppModule);
//------------------------------------------------------------------------
// ����ģ�������
class CChatClientHelper  
{
public:
	typedef BOOL (*ProcCreateChatClient)(DWORD dwVersion, 
		IClientGlobal * pGlobal, IChatClient ** ppModule); 
	
public:
	CChatClientHelper(void)
	{
	}
	virtual ~CChatClientHelper(void)
	{
		Close();
	}
	
public:
	// ����ģ��
	BOOL Create(IClientGlobal * pGlobal)
	{
		try
		{	
			Close();
			
			// ���붯̬��
#ifdef _DEBUG
			m_hDll = LoadLibrary("FKChatClient_D.dll");
			if (m_hDll == NULL)
			{
				throw "�޷����� FKChatClient_D.dll";
			}
#else
			m_hDll = LoadLibrary("FKChatClient.dll");
			if (m_hDll == NULL)
			{
				throw "�޷����� FKChatClient.dll";
			}
#endif
			
			// ��ȡ������ַ
			ProcCreateChatClient createProc;
			createProc = (ProcCreateChatClient)GetProcAddress(m_hDll, "CreateChatClient");
			if(createProc == NULL)
			{
				throw "Can not GetProAddress('CreateChatClient')";
			}
			
			// ����ģ��
			if(!createProc(CHATCLIENT_VERSION, pGlobal, &m_pModule))
			{
				throw "CreateChatClient() catch error!";
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
			OutputDebugString("CreateChatClient() catch error\r\n");
			return FALSE;
		}
	}
	
	// ��ȡģ��
	IChatClient * operator ->(void) const
	{
		return m_pModule;
	}

	IChatClient * GetChatClient(void) const
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
	
private:
	HINSTANCE m_hDll;
	IChatClient * m_pModule; // ����ģ��ָ��
};
//------------------------------------------------------------------------
