/**
*	created:		2012-6-29   14:07
*	filename: 		IClanClient
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
/*
 * 
 * �ͻ�������ϵͳ�ӿ��ļ�,�������ӿ�
 * 1.����ͷ������:
 *     �����������,������ҵĳƺ�,�������������Icon��Ϣ
 * 2.�ͻ�������ϵͳ�ӿ�
 *     ���ڻ�ȡ����ϵͳ����ز���
 *
 * �뽫ÿ�ε��޸ļ�¼˵������:
 */
//------------------------------------------------------------------------
#include <Windows.h>
#include "../../FKGame/SceneObj/IThingPart.h"
//------------------------------------------------------------------------
#define CLAN_CLIENT_VERSION  1  
//�ж��Ƿ�������ID
#define IS_UNIONID(_x)				(((unsigned long)_x) >= 0x100000)
//------------------------------------------------------------------------
struct IActor;
// ����ģ���������
struct IClientGlobal;
//------------------------------------------------------------------------
//����ͷ������,
struct IClanClientPate : public IThingPart
{
	//��  ���������������ƣ����û�����巵�� ""
	virtual const char *GetClanName() = 0;

	//��  �����Ƿ������峤
	virtual bool IsClanShaikh() = 0;

	//��  �������������ϵ
	virtual bool IsHaveClan() = 0;

	//����ս���ж��Ƿ��ǵжԵ�����
	virtual bool IsMyOpposedClan(long lClanID) = 0;

	//���³�ս�еĵж�����
	virtual bool UpdateMyOpposedClan(const BYTE* pData,int nLen) = 0;

	//�Ƿ��ܹ���
	virtual bool IsAttackCamp(UID uid) = 0;

	//ȡ��������ϵĳ�ս����ID,�������0��ʾû������ID
	virtual int  GetSiegeCmapId() = 0;

	//�ж��Ƿ��ǳ�������
	virtual bool IsCastellanCamp() = 0;

	//�Ƿ��ǳ�������
	virtual bool IsCastellanClan(long lCastleId) = 0;
};
//------------------------------------------------------------------------
//�ͻ�������ϵͳ
struct IClanClient
{
	virtual void Release(void)    = 0;

	//�� ��: ������ϵͳ���
	virtual void OpenClanSysPanel(void) = 0;

	//�� ��: ��ȡ����ͷ������ 
	virtual IClanClientPate* GetClanPatePart(void) = 0;

	//�����ͷ�����Ĵ���
	virtual void ReleaseClanWnd(void) = 0;
};
//------------------------------------------------------------------------
class ClanClientHelper
{
	typedef BOOL (*getClanClientInstance)( DWORD dwVersion
										  ,IClientGlobal * pGlobal
										  ,IClanClient ** ppInst);

	HINSTANCE     m_hDll;
	IClanClient * m_pInstance;
public:
	ClanClientHelper()
		:m_hDll(0),m_pInstance(0)
	{}
	~ClanClientHelper()
	{
		Close();
	}

	void Close()
	{
		if(m_pInstance)
		{
			m_pInstance->Release();
			m_pInstance = 0;
		}
		if(m_hDll)
		{
			FreeLibrary(m_hDll);
			m_hDll = 0;
		}
			
	}

	IClanClient *GetModule(void) const
	{
		return m_pInstance;
	}

	IClanClient *operator ->(void) const
	{
		return m_pInstance;
	}

	BOOL Create(IClientGlobal *pGlobal)
	{
		if(m_pInstance)
			return TRUE;

		try
		{
#ifdef _DEBUG
			m_hDll = LoadLibrary("FKClanClient_D.dll");
			if(!m_hDll)
			{
				throw "�޷����� FKClanClient_D.dll!";
			}
#else
			m_hDll = LoadLibrary("FKClanClient.dll");
			if(!m_hDll)
			{
				throw "�޷����� FKClanClient.dll!";
			}
#endif
			
			getClanClientInstance pFun = (getClanClientInstance) GetProcAddress(m_hDll,"getClanClientInstance");
			if(!pFun)
			{
				throw "Not Find getClanClientInstance !";
			}
			
			if(!pFun(CLAN_CLIENT_VERSION,pGlobal,&m_pInstance))
			{
				throw "getClanClientInstance() Failed!";
			}
			return (m_pInstance != NULL);
			
		}
		catch(LPCSTR szMsg)
		{
			OutputDebugString(szMsg);
			OutputDebugString("\r\n");
		}
		catch(...)
		{
			OutputDebugString("getClanClientInstance() catch error\r\n");
		}
		return FALSE;
	}
	
};
//------------------------------------------------------------------------