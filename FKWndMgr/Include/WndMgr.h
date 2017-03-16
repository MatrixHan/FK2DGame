/**
*	created:		2012-6-25   1:37
*	filename: 		WndMgr
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���ڹ�����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/IWndMgr.h"
#include "../../FKResManager/IFKResManager.h"
#include "../../FKFileSystem/IFKFileSystem.h"
#include "../../FKController/ValidationCode/WndMgrSignal.h"
#include "../../FKGame/WndSys/WndSys.h"
#include "../../FKTrace/IFKTrace.h"
//------------------------------------------------------------------------
#define  MAX_WINDOW_NUM	512
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
	#ifdef FKTraceLn
	#undef FKTraceLn
	#endif
	#define	FKTraceLn			g_DefaultTrace.DebugPrintLn
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
	#ifdef FKTraceLn
	#undef FKTraceLn
	#endif
	#define	FKTraceLn			__noop
#endif
//------------------------------------------------------------------------
struct IClientGlobal;
//------------------------------------------------------------------------
class CWndMgr : public IWndMgr
{
public:
	CWndMgr();
	virtual ~CWndMgr();
public:
	virtual void* CreateWnd(UINT uWndID);
	virtual BOOL  CloseWnd(UINT uWndID);
	virtual DWORD OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnEvent(UINT uWndID, UINT uEventID, WPARAM wParam, LPARAM lParam);
	virtual BOOL  ShowWnd(UINT uWndID, int nCmdShow);
	virtual BOOL  IsWindowOpened(UINT uWndID);
	virtual ICursorData *GetCursorAttachData();
	virtual void  SetCursorAttach(IBitmap *pBitmap, ICursorData *pData);	
	virtual int  GetCursorAttachType() ;
	virtual void ClearCursorAttach();

	virtual BOOL  SubscibeMessage(UINT uWndID, UINT uEventID, 
		WMMESSAGE_CONNECTION *pConnect);

	virtual BOOL  UnSubscibeMessage(UINT uWndID, UINT uEventID, 
		WMMESSAGE_CONNECTION *pConnect);

	virtual void  PostWindowMessage(UINT uWndID, UINT uEventID, 
		WPARAM wParam, LPARAM lParam);

	// ע��ĳ�����ڣ��ڴ򿪻�ر�ʱ��Ҫ�յ�WE_CW_ALLWND_SHOW��Ϣ�����Ҵ˴���Ҫ�̳�IWndEnableCall	
	virtual void  RegTellShowWnd (UINT uWndID);

	// ȡ��ע��ĳ�����ڣ��ڴ򿪻�ر�ʱ��Ҫ�յ�WE_CW_ALLWND_SHOW��Ϣ��
	virtual void  UnRegTellShowWnd(UINT uWndID);

	virtual void  Release();

	//ȡ�����Խ���Ĵ���ID
	UINT GetWindowBelowCursor(POINT pt, int nFlags)
	{
		XWindow *pWndDesktop = m_WndSysHelper.GetGameDesktop();
		Assert(pWndDesktop);
		XWindow *pWnd = pWndDesktop->GetWindowBelowCursor(pt, nFlags);
		return pWnd == NULL ? 0 : pWnd->GetID();
	}

	//�ر����й����Դ���
	BOOL CloseAllFunctionWnd();

	BOOL Init(IClientGlobal *pGlobal);

	//�Զ������Ϣ��
	virtual void  CustomMsgBox(LPCSTR pMsg, DWORD BtnNO1 = 1, 
		DWORD BtnNO2 = 0, 
		DWORD BtnNO3 = 0,
		IWndCallback * pCallBack = NULL,
		RECT * rcWnd = NULL,
		WPARAM wParam = 0,
		LPARAM lParam = 0,
		int    nCourse = POPCOURSE_UNKNOWN);

	//�Զ���������
	virtual void  CustomInputBox(LPCSTR pMsg, 
		IWndCallback * pCallBack = NULL,
		DWORD  dwMaxLen = 6,
		BOOL   bDigitOnly = TRUE,
		BOOL   bNullFlag = FALSE,
		BOOL   bPasswordFlag = FALSE,
		int    nCourse = POPCOURSE_UNKNOWN);

	// �ڴ򿪵���������, �ر�����һ��,���û�д��ŵ�������, �򷵻�FALSE
	virtual BOOL HideOnePrimacyWnd(void);

private:
	BOOL ShowWndAux(UINT uWndID, int nCmdShow);
	void CloseAllWnd(); //�ر����д���
	//��ȡ����WNDSYS�����TOOLTIP
	void __LoadWndToolTip(IResScript *pRescript);

	//������Щ�����ǰ���Ҫ��(��ESC�ɹر�) 
	void __DeclarePrimaryWnd(void);

public:
	IFileSystem		*m_pFileSystem;	//����ϵͳ���ļ�ϵͳ
	IFileSystem		*m_pFileSystemEx;
	IResScript		*m_pResScript;	//�������ʹ��ڵĽű�
	CWndSysHelper	m_WndSysHelper;	//����ϵͳ������

private:
	CIResMgrHelper m_ResMgrHelper;
	CIFileSystemHelper m_FileSystemHelper;
	CWMMessageSender<256> m_MessageSender[MAX_WINDOW_NUM];
	//���ĳ����������Ҫ�򿪻�ر�ʱ��
	//Ҫ�յ�WE_CW_ALLWND_SHOW����Ϣ���ͱ���ע��
	DWORD m_WndTellShow[MAX_WINDOW_NUM+1];

	//��Ҫ�Ĵ����б�, ÿ������ESCʱ, ��������һ�����ر�
	std::list<int> m_lsPrimaryWnd;
};
//------------------------------------------------------------------------