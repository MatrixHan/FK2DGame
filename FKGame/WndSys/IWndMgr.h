/**
*	created:		2012-6-22   23:40
*	filename: 		IWndMgr
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "WndSys.h"
//------------------------------------------------------------------------
// �ı���Ϸ���
#define WM_CHANGE_GAMECUROSR		WM_APP + 502
// �������(Cursor Type)
#ifndef CT_ARROW
#define CT_ARROW	0	// ��ͷ
#endif

#ifndef CT_HAND
#define CT_HAND		1	// ����
#endif

#ifndef CT_BEAN
#define CT_BEAN		2	// I����
#endif

#ifndef CT_VSIZE
#define CT_VSIZE	3	// ��ֱ�仯�ߴ�
#endif

#ifndef CT_HANDED
#define CT_HANDED   4   // �ս�����
#endif

#ifndef CT_ATTACK
#define CT_ATTACK   5   // ������
#endif

#ifndef CT_TALK
#define CT_TALK     6   // ��̸��
#endif

#ifndef CT_INVALID
#define CT_INVALID  7   // ��Ч��
#endif

#ifndef CT_PICKUP
#define CT_PICKUP   8   // ʰȡ��
#endif

#ifndef CT_MINE
#define CT_MINE   9   // �ɿ�
#endif

#ifndef CT_FIX
#define CT_FIX   10   // ����
#endif

#ifndef CT_HERB
#define CT_HERB   11  // ��ҩ
#endif

#ifndef CT_GOLD
#define CT_GOLD   12  // ���
#endif
//------------------------------------------------------------------------
struct ICursorData;
struct IBitmap;
// �ͻ���Ҫʵ�ֵĻص��ӿ�
struct IWndCallback
{
	virtual BOOL OnEvent(UINT uWndID, UINT uEventID, WPARAM wParam, LPARAM lParam) = 0;
};
//------------------------------------------------------------------------
struct IWndEnableCallEx
{
    virtual BOOL OnEvent(UINT uEventID, WPARAM wParam, LPARAM lParam) = NULL;
};
//------------------------------------------------------------------------
// ��ͨ�ô���
struct IWndEnableCall : public XMainWnd, public IWndEnableCallEx
{
	/*  ����Ժ��ټ�
	DWORD OnKeyDown(UINT nKeyCode, UINT nShift)
		{
			if (nKeyCode == VK_ESCAPE)
			{
				
				return 0;
			}		
			return XMainWnd::OnKeyDown(nKeyCode, UINT nShift);
	}*/
};
//------------------------------------------------------------------------
// ͨ�ô���
struct IWndEnableCall4 : public XGeneralMainWnd, public IWndEnableCallEx
{
	
};
//------------------------------------------------------------------------
template< class ARG1, class ARG2, class ARG3 >
struct __BasicConnection3;
typedef __BasicConnection3< WPARAM, LPARAM, BOOL& > WMMESSAGE_CONNECTION;
//------------------------------------------------------------------------
// ���ڹ������ӿ�
struct IWndMgr
{
	enum    //����ԭ��Ի�����������ԭ��
	{
		POPCOURSE_UNKNOWN = 0,    //����ԭ��		
	};

	// ��  ������������
	// ��  ����[����] UINT uWndID  - ����ID
	// ����ֵ��void*- ���ش���ָ��
	virtual void* CreateWnd(UINT uWndID) = NULL;

	// ��  �����ͷŴ���
	// ��  ����[����] UINT uWndID  - ����ID
	// ����ֵ��BOOL - ����ͷųɹ����򷵻�TRUE,���򷵻�FALSE
	virtual BOOL  CloseWnd(UINT uWndID) = NULL;

	// ��  ������Ϸ��ѭ���������̵���Ϣ�ɷ������ڹ�����
	virtual DWORD OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = NULL;

	// ��  �����߼��������ʾ��ĸ����¼�
	virtual BOOL OnEvent(UINT uWndID, UINT uEventID, WPARAM wParam, LPARAM lParam) = NULL;

	// ��  ������ʾ/���� ����
	virtual BOOL ShowWnd(UINT uWndID, int nCmdShow) = NULL;

	//��  �������ĳ�������Ƿ��Ǵ򿪵�
	virtual BOOL  IsWindowOpened(UINT uWndID) = NULL;

	//��  ����ȡ������ϸ��ӵ�����,һ�������ϷŲ�����
	virtual ICursorData* GetCursorAttachData() = NULL;

	//��  ������������ϸ��ӵ�����
	virtual void SetCursorAttach(IBitmap*pBitmap, ICursorData*pData) = NULL;

	//��  �������ȡ������ϵĸ��ӵ�����
	virtual int GetCursorAttachType() = NULL;

	//��  �����������ϵĸ��ӵ����� 
	virtual void ClearCursorAttach() = NULL;

	// ��  �������Ĵ��ڷ�������Ϣ
	// ��  ����[����] UINT uWndID  - ����ID
	// ��  ����[����] UINT uEventID  - ��ϢID
	// ��  ����[����] WMMESSAGE_CONNECTION*pConnect  - ���Ӷ���
	// ����ֵ��BOOL
	virtual BOOL SubscibeMessage(UINT uWndID, UINT uEventID, 
		WMMESSAGE_CONNECTION *pConnect) = NULL;
	
	// ��  �����˶���Ϣ
	// ��  ����[����] UINT uWndID  - ����ID
	// ��  ����[����] UINT uEventID  - ��ϢID
	// ��  ����[����] WMMESSAGE_CONNECTION*pConnect  - ���Ӷ���
	// ����ֵ��BOOL
	virtual BOOL UnSubscibeMessage(UINT uWndID, UINT uEventID,
		WMMESSAGE_CONNECTION *pConnect) = NULL;
	
	// ��  �������ڹ㲥һ����Ϣ����ǰSubscibeMessage���Ķ�������յ�
	virtual void PostWindowMessage(UINT uWndID, UINT uEventID, 
		WPARAM wParam, LPARAM lParam) = NULL;

	// ��  �����ͷŴ��ڹ�����
	virtual void Release() = NULL;
	
	// ��  ����ע��ĳ�����ڣ��ڴ򿪻�ر�ʱ��Ҫ�յ�WE_CW_ALLWND_SHOW��Ϣ��
	//         ���Ҵ˴���Ҫ�̳�IWndEnableCall	
	virtual void RegTellShowWnd  (UINT uWndID) = NULL;

	// ��  ����ȡ��ע��ĳ�����ڣ��ڴ򿪻�ر�ʱ��Ҫ�յ�WE_CW_ALLWND_SHOW��Ϣ��
	virtual void UnRegTellShowWnd(UINT uWndID) = NULL;

	// �����Զ������Ϣ��
	virtual void  CustomMsgBox(LPCSTR pMsg, DWORD BtnNO1 = 1, 
		DWORD BtnNO2 = 0, 
		DWORD BtnNO3 = 0,
		IWndCallback * pCallBack = NULL,  
		RECT * rcWnd = NULL,
		WPARAM wParam = 0,
		LPARAM lParam = 0,
		int    nCourse = POPCOURSE_UNKNOWN) = NULL;

	// �����Զ���������
	virtual void  CustomInputBox(LPCSTR pMsg, 
		IWndCallback * pCallBack = NULL,
		DWORD  dwMaxLen = 6,
		BOOL   bDigitOnly = TRUE,
		BOOL   bNullFlag = FALSE,
		BOOL   bPasswordFlag = FALSE,
		int    nCourse = POPCOURSE_UNKNOWN) = NULL;

	// �ڴ򿪵���������, �ر�����һ��,���û�д��ŵ�������, �򷵻�FALSE
	virtual BOOL HideOnePrimacyWnd(void) = NULL;
};
//------------------------------------------------------------------------
struct IClientGlobal;
//------------------------------------------------------------------------
// ���ڹ�����������
class CIWndMgrHelper
{
public:
	typedef BOOL (*ProcCreateWndMgr)(IClientGlobal *pGlobal, IWndMgr **ppWndMgr, IWndCallback *pCallBack);
	
	CIWndMgrHelper()
	{
		m_hdll = NULL;
		m_pWndMgr = NULL;
	}
	~CIWndMgrHelper()
	{
		Close();
	}
	IWndMgr* operator ->()
	{
		return m_pWndMgr;
	}
	
	BOOL Create(IClientGlobal *pGlobal, IWndCallback *pCallBack)
	{
		Close();
		try
		{
			if (m_hdll == NULL)
			{
#ifdef _DEBUG
				m_hdll = ::LoadLibrary("FKWndMgr_D.dll");
				if (m_hdll == NULL)
					throw "�޷����� FKWndMgr_D.dll";
#else				
				m_hdll = ::LoadLibrary("FKWndMgr.dll");
				if (m_hdll == NULL)
					throw "�޷����� FKWndMgr.dll";
#endif
			}
			ProcCreateWndMgr proc;
			proc = (ProcCreateWndMgr)GetProcAddress(m_hdll, "CreateWndMgrDLL");
			if (proc == NULL)
				throw "Can not GetProcAddress('CreateWndMgrDll')";
			
			if (!proc(pGlobal, &m_pWndMgr, pCallBack))
				throw "CreateWndMgr() error";

			return m_pWndMgr != NULL;
		}
		catch(LPCSTR szMsg)
		{
			OutputDebugString(szMsg);
			OutputDebugString("\r\n");
			return FALSE;
		}
		catch(...)
		{
			OutputDebugString("CIWndMgrHelper::Create() : catch error\r\n");
			return FALSE;
		}
	}

	void Close()
	{
		if (m_pWndMgr)
		{
			m_pWndMgr->Release();
			m_pWndMgr = NULL;
		}
		if (m_hdll)
		{
			FreeLibrary(m_hdll);
			m_hdll = NULL;
		}
	}

public:
	HINSTANCE	m_hdll;
	IWndMgr		*m_pWndMgr;
};
//------------------------------------------------------------------------