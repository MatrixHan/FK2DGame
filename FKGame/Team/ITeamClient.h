/**
*	created:		2012-6-29   14:09
*	filename: 		ITeamClient
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���ģ��ӿ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#ifdef FKTEAMCLIENT_EXPORTS
#define TEAMCLIENT_API extern "C" __declspec(dllexport)
#else
#define TEAMCLIENT_API extern "C" __declspec(dllimport)
#endif
//------------------------------------------------------------------------
#define TEAMCLIENT_VERSION 1
//------------------------------------------------------------------------
// ���ģ��ӿ�
struct ITeamClient
{	
	//-------------------------------------------------------------------
	// ��  �����ͷ�ģ�����Դ 
	//-------------------------------------------------------------------
	virtual void Release(void) = 0;	

	//-------------------------------------------------------------------
	// ��  ������ʾ��Ӳ˵�
	// ��  ����[����] IActor * pActor - ���ָ��
	// ��  ����[����] const POINT * pPos - ��ʾλ��
	// ��  ����[����] BOOL bShow - �Ƿ���ʾ
	// ����ֵ��void
	//-------------------------------------------------------------------	
	virtual void ShowTeamMenu(IActor * pActor, const POINT * pPos, BOOL bShow) = 0;
	
	//-------------------------------------------------------------------
	// ��  �������һ������ǲ��Ƕ���
	// ��  ����[����] DWORD dwPlayerid - ���ID
	// ����ֵ��BOOL - ����Ƕ��ѷ���TRUE�����򷵻�FALSE
	//-------------------------------------------------------------------
	virtual BOOL IsTeammate(DWORD dwPlayerid) = 0;
	
	//-------------------------------------------------------------------
	// ��  ���������뿪���� 
	//-------------------------------------------------------------------
	virtual void LeaveTeam(void) = 0;
	
	//-------------------------------------------------------------------
	// ��  ������ȡ��������ж�Ա��ID
	// ��  ����[���] int & nCount - ��Ա����
	// ����ֵ��const DWORD * - ����ID������ָ��
	//-------------------------------------------------------------------
	virtual const DWORD * GetTeamIds(int & nCount) const = 0;

	//-------------------------------------------------------------------
	// ��  ������ȡ�ӳ�ID
	// ��  ����void
	// ����ֵ�����ضӳ���DB ID
	//-------------------------------------------------------------------
	virtual DWORD GetCaptainId(void)  = 0;

	//-------------------------------------------------------------------
	// ��  ������ȡ���ѡ��
	// ��  ����void
	// ����ֵ��TRUE��ͬ����ӣ�FALSE �ܾ����
	//-------------------------------------------------------------------
	virtual BOOL  GetTeamOption(void)  = 0;

	//-------------------------------------------------------------------
	// ��  �����������ѡ��
	// ��  ����BOOL bNotRefuse ,TRUE��ͬ����ӣ�FALSE �ܾ����
	// ����ֵ��void
	//-------------------------------------------------------------------
	virtual void  SetTeamOption(BOOL bNotRefuse)  = 0;

	//-------------------------------------------------------------------
	// ��  ��������������
	// ��  ����DWORD dwInviteeID  �������˵�DB ID
	// ����ֵ��void
	//-------------------------------------------------------------------
	virtual void  InviteJionTeam(DWORD dwInviteeID)  = 0;

	
	//-------------------------------------------------------------------
	// ��  ��������������
	// ��  ����LPCSTR lpszName  �������˵�����
	// ����ֵ��void
	//-------------------------------------------------------------------
	virtual void  InviteJionTeam(LPCSTR lpszName)  = 0;	

	//-------------------------------------------------------------------
	// ��  ������ö���ID
	// ��  ����
	// ����ֵ��û�ж����ʱ�򷵻� 0
	//-------------------------------------------------------------------	
	virtual DWORD GetTeamID() = 0;
	
};
//------------------------------------------------------------------------
// ���ģ���������
class CTeamClientHelper
{
private:
	typedef BOOL (*ProcCreateTeamClient)(DWORD dwVersion, 
										 IClientGlobal * pGlobal, 
										 ITeamClient ** ppModule); 

private:
	HINSTANCE m_hDll;
	ITeamClient * m_pModule; // ���ģ��ָ��

public:
	CTeamClientHelper(void) : m_hDll(NULL), m_pModule(NULL)
	{		
	}

	~CTeamClientHelper(void)
	{
		Close();
	}
	
	// �������ģ��
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
			m_hDll = LoadLibrary("FKTeamClient_D.dll");
			if (m_hDll == NULL)
			{
				throw "�޷����� FKTeamClient_D.dll";
			}
#else
			m_hDll = LoadLibrary("FKTeamClient.dll");
			if (m_hDll == NULL)
			{
				throw "�޷����� FKTeamClient.dll";
			}
#endif
			// ��ȡ������ַ
			ProcCreateTeamClient createProc;
			createProc = (ProcCreateTeamClient)GetProcAddress(m_hDll, "CreateTeamClient");
			if(createProc == NULL)
			{
				throw "Can not GetProAddress('CreateTeamClient')";
			}
			
			if(!createProc(TEAMCLIENT_VERSION, pGlobal, &m_pModule))
			{
				throw "CreateTeamClient() catch error!";
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
			OutputDebugString("CreateTeamClient() catch error\r\n");
			return FALSE;
		}
	}

	// ��ȡģ��ָ��
	ITeamClient * GetModule(void) const
	{
		return m_pModule;
	}

	// ��ȡģ��ָ��
	ITeamClient * operator ->(void) const
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
//-----------------------------------------------------------------------------
// ��  �����������ģ��
// ��  ����[����] DWORD dwVersion - ģ��İ汾��
// ��  ����[����] IClientGlobal * pGlobal - ȫ�ֹ�������ָ��
// ��  ����[���] ITeamClient ** ppModule - ָ��ģ�����ɺ��ַ��ָ��
// ����ֵ��BOOL - ģ�鴴���ɹ�����TRUE,���򷵻�FALSE
//-----------------------------------------------------------------------------
TEAMCLIENT_API BOOL CreateTeamClient(DWORD dwVersion, 
									 IClientGlobal * pGlobal, 
									 ITeamClient ** ppModule);
//------------------------------------------------------------------------