/**
*	created:		2012-6-29   12:54
*	filename: 		IContainerClient
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../Common/Macro.h"
//------------------------------------------------------------------------
/////////////////////////////֪ͨ���ڵ���Ϣ����///////////////////////////////////
//֪ͨ���ڸ���
// wParam - int* ��Ҫ���µ�λ���б�
// lParam - int ��Ҫ���µĸ���
//#define CONTAINER_WND_MSG_UPDATE		0	

/////////////////////////////�¼�����������Ϣ///////////////////////////////
//------------------------------------------------------------------------
#define CONTAINER_UPDATE				1	
//------------------------------------------------------------------------
struct SSingleInfo 
{
	int nLocation;	//λ��
	UID uidOldGoods;	//�ɵ���ƷUID
	UID uidNewGoods;	//�µ���ƷUID
	SSingleInfo()
	{
		nLocation = -1;
		uidOldGoods = INVALID_UID;
		uidNewGoods = INVALID_UID;
	}
};
//------------------------------------------------------------------------
//���µ���Ӧ�ֳ�
struct SContainerUpdateResponseContext
{
	SContainerUpdateResponseContext()
	{
		dwContainerID = 0;
		dwUpdateNum = 0;		
	}
	DWORD dwContainerID;
	DWORD dwUpdateNum;	
	//����dwUpdateNum��SSingleInfo
};
//------------------------------------------------------------------------
struct IContainer;
//------------------------------------------------------------------------
struct IContainerGoods
{
	virtual void Release() = NULL;
	virtual UID GetUID() = NULL;
	virtual DWORD GetNumber() = NULL;
	// ȡ����ƷID
	virtual DWORD GetGoodsID() = NULL;	
	// ȡ����Ʒ����
	virtual LPCSTR GetName() = NULL;	
	// ȡ����Ʒ������������
	virtual int	GetPropNum(DWORD dwPropID) = NULL;	
	// ȡ����Ʒ���ַ�������
	virtual LPCSTR GetPropStr(DWORD dwPropID) = NULL;
	// �ж��Ƿ�����
	virtual BOOL IsLock() = NULL;
	// ��������״̬
	virtual void SetLock(BOOL bLock) = NULL;
	// ��ȡ��������ָ��
	virtual IContainer* GetContainer() = NULL;
	// ������������ָ��
	virtual void SetContainer(IContainer* pContainer) = NULL;
	// ��ȡ�������е�λ��
	virtual int GetLocation() = NULL;
	// �����������е�λ��
	virtual void SetLocation(int nLocation) = NULL;
	// ��ȡ��Ʒ����
	virtual DWORD GetType() = NULL;
};
//------------------------------------------------------------------------
struct IContainer 
{
	// �������Ϣ����ID
	virtual void BindWnd(DWORD dwWndID) = NULL;
	// ȡ����
	virtual void UnBindWnd(DWORD dwWndID) = NULL;

	virtual void Release() = NULL;

	// ��ȡ�����ܴ�С
	virtual int GetSize() = NULL;
	// ����uid��ȡ������Ʒ
	virtual IContainerGoods* GetGoods(UID uidGoods) = NULL;
	// ����λ�û�ȡ��Ʒ
	virtual IContainerGoods* GetGoods(int nLocation) = NULL;
	
	// �����Ƿ����������Ʒ
	virtual BOOL CanAdd() = NULL;
	// �ж�Ŀ��λ���Ƿ��������ָ����Ʒ
	virtual BOOL CanAdd(IContainerGoods* pGoods, int nTargetLocation) = NULL;
	// �����Ƿ������Ƴ���Ʒ
	virtual BOOL CanRemove() = NULL;
	// �ж�Դλ���Ƿ������Ƴ�����Ʒ
	virtual BOOL CanRemove(IContainerGoods* pGoods, int nSrcLocation) = NULL;
	// �ύ�������,���������Ʒ,nTargetLocation = -1�����Զ�Ѱ�ҿ�λ
	virtual void AddGoods(IContainerGoods* pGoods, int nTargetLocation, int nNumber) = NULL;	
	// �ύ���������� ��Դ��������Ʒ����ӵ�������
	virtual void AddAllGoodsFromContainer(IContainer *pSrcContainer) = NULL;
	
	//�ύ�������,����������Ʒ
	virtual void DestroyGoods(IContainerGoods *pGoods) = NULL;
	// ��ȡID
	virtual DWORD GetID() = NULL;
	// ��ȡ������Ʒ����
	virtual DWORD GetGoodsNumber() = NULL;

	// ����������Ʒ
	virtual void OnUpdate(LPCSTR buf, int nLen) = NULL;
	// ��ȡ��������
	virtual DWORD GetType() = NULL;
	// ��
	virtual void Open() = NULL;
	// �رգ������ͷ�
	virtual void Close() = NULL;
	// ʹ��ָ��λ�õ���Ʒ
	virtual BOOL UseGoods(int nLocation) = NULL;
};
//------------------------------------------------------------------------
//���������ֳ�
struct SContainerCreateContext
{
	DWORD dwID;
	int	nMaxSize;
	BOOL bCanAdd;
	BOOL bCanRemove;
	DWORD dwType;
	SContainerCreateContext()
	{
		dwID = 0;
		nMaxSize = 0;
		bCanAdd = TRUE;
		bCanRemove = TRUE;
		dwType = 0;
	}
};
//------------------------------------------------------------------------
//������Ʒ�����ֳ�
struct SGoodsCreateContext
{
	DWORD dwType;		//��Ʒ����
	UID uidGoods;		//��ƷUID
	BOOL bLock;			//�Ƿ�����
	DWORD dwNumber;		//��Ʒ��Ŀ
	SGoodsCreateContext()
	{
		dwType = 0;
		uidGoods = INVALID_UID;
		bLock = FALSE;
		dwNumber = 0;
	}
};
//------------------------------------------------------------------------
struct IContainerMgr
{
	virtual void Release() = NULL;
	// �����ֳ���������
	virtual IContainer* CreateContainer(LPCSTR buf, int nLen) = NULL;
	// ��������ID��ȡ����ָ��
	virtual IContainer* GetContainer(DWORD dwContainerID) = NULL;
	// �ͷ���������ID
	virtual void ReleaseContainerID(DWORD dwContainerID) = NULL;
	// �����ֳ�����������Ʒ
	virtual IContainerGoods* CreateGoods(LPCSTR buf, int nLen) = NULL;
};
//------------------------------------------------------------------------
struct IClientGlobal;
//------------------------------------------------------------------------
class CContainerMgrHelper
{
private:
	typedef BOOL (*ProcCreateContainerMgr)(IContainerMgr **ppContainerMgr,
		IClientGlobal *pClientGlobal);	
public:
	CContainerMgrHelper()
	{
		m_hDll = NULL;
		m_pContainerMgr = NULL;
		m_szErrMsg[0] = 0;
	}
	~CContainerMgrHelper()
	{		
		Close();
		if (m_hDll != NULL)
		{
			::FreeLibrary(m_hDll);
			m_hDll=NULL;
		}
	}	
	// ����ֵ��BOOL - �����ɹ��򷵻�TRUE,���򷵻�FALSE
	BOOL Create(IClientGlobal *pClientGlobal)//�����ӿڶ���
	{	
		Close(); 
		try
		{
			if (m_hDll == NULL)
			{
#ifdef _DEBUG
				//��һ�ε���ʱδ����dll,������м���dll
				m_hDll = ::LoadLibrary("FKContainerClient_D.dll");
				if (m_hDll == NULL)
					throw "�޷����� FKContainerClient_D.dll";
#else
				//��һ�ε���ʱδ����dll,������м���dll
				m_hDll = ::LoadLibrary("FKContainerClient.dll");
				if (m_hDll == NULL)
					throw "�޷����� FKContainerClient.dll";
#endif
			}
			
			//������dll��������������ӿڶ���Ĵ�������
			ProcCreateContainerMgr proc = NULL;
			proc=(ProcCreateContainerMgr)::GetProcAddress(m_hDll,
				"CreateContainerMgr");
			if (proc == NULL)
				throw "Can't GetProcAddress('CreateContainerMgr')";
			if (!proc(&m_pContainerMgr, pClientGlobal))
				throw "CreateContainerMgr() error!";
			
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
	void Close()
	{
		if (m_pContainerMgr != NULL)
		{
			m_pContainerMgr->Release();
			m_pContainerMgr = NULL;
		}		
	}
	//����->,��helper���������ֱ��תΪ�ӿڵ�����
	IContainerMgr* operator ->()
	{
		return m_pContainerMgr;
	}
	IContainerMgr* GetContainerMgr()
	{
		return m_pContainerMgr;
	}
	//�жϽӿ�ָ���Ƿ���Ч
	BOOL IsValid()
	{
		return (m_pContainerMgr != NULL);
	}
	char* GetErrMsg()
	{
		return m_szErrMsg;
	}	
private:
	IContainerMgr *m_pContainerMgr;						//�ӿڶ���
	HINSTANCE m_hDll;									//��̬���ӿ��������־
	char m_szErrMsg[128];								//������Ϣ
};
//------------------------------------------------------------------------