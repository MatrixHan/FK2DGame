/**
*	created:		2012-6-21   0:41
*	filename: 		IGameApp
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
enum
{
	gcmdFullScreen	= 50000,		// ����ȫ��
	gcPlayerMovie	= 50001,		// ֪ͨĿǰ���ڲ�����
	gcmdChangMap	= 50002,		// ���������ͼ,dwParam1ָ���µĵ�ͼ��
	gcmdExit		= 50003			// �˳���Ϸ
};
//------------------------------------------------------------------------
// ����
struct ITrace;
struct ICanvas;
//------------------------------------------------------------------------
// ����
struct IGameApp
{
	// һ����WM_INITDIALOG��������Ϣ��������ô˺�������ʾ�����ڴ����ɹ�
	// ��Ϸ�����ڴ˴�����Ϸ����
	// pParam1��pParam2Ϊ���Ӳ�����Ϊδ������������������Ҫ����
	virtual BOOL		InitGame(HINSTANCE hInstance = NULL, HWND hWnd = NULL, ICanvas* pScreen = NULL,ITrace* pTrace = NULL, void* pParam1 = NULL, void*pParam2 = NULL) { return FALSE; }

	// ��Ϸ��ѭ����һ����OnIdle���������
	// ע�⣺
	//		MFC��ATLĬ����Ϊ�ǣ�������Ϣ����ʱֻ����OnIdleһ��
	//		���Ҫ����OnIdle��ʵ�֣�ʹ֮�ڿ���ʱ�����޴εص���OnIdle()
	// ���أ�������FALSEʱ��Ҫ���������˳�
	virtual BOOL		Loop(void) { return 0; }

	// ��Ϣ���ˣ��ӹ���ꡢ���̵ȿ�����Ϣ
	// ���أ�����TRUE��ʾ����Ϣ�Ѿ����ӹܣ������ڲ����ٴ������Ϣ
	virtual BOOL		PreTranslateMessage(MSG * pMsg) { return FALSE; }

	// �Զ���������
	virtual LRESULT		OnCommand(DWORD dwCmd,DWORD dwParam1,DWORD dwParam2) { return -1; }
};
//------------------------------------------------------------------------