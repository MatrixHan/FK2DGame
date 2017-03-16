/**
*	created:		2012-6-14   23:41
*	filename: 		IFKTrace
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
/*
	ʹ��˵����
	1�� Main�����ļ��а������ļ�ͷ
	2�� ע���������ļ�ͬ��Ŀ¼�·��ö�Ӧ�Ķ�̬���ӿ⡣
	DEBUG�汾����	FKTrace_D.dll 
	RELEASE�汾����	FKTrace.dll
	3�� ��Mainǰ����ȫ�ֱ��� CFKTrace g_Trace;
	4�� ���� Create ��������������Ӧ���� Create �����������潫��ϸ���⡣
	5�� �� Create ʱ��ʾ��ʹ�ô���ģʽ OS_WINDOW������Ҫ WinMain() ��ӵ�����д��룺
		<code>
			MSG msg;
			while (GetMessage (&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		</code>
		�����������������ƹ��ܴ��룬��ɲ�д��
	6����ʹ���� OS_RICHEDIT ģʽ������ Create ֮ǰ���� RichEdit32.dll ,����������
		<code>
			HINSTANCE hDll = LoadLibrary("RICHED32.DLL");
			if( NULL == hDll )
			{
				// DoSth ���� Create ����һ�ֲ���RichEdit��ģʽ
			}
		</code>
	7�������ͨ��������붨���Լ�ϲ����Trace����ɫ
		<code>
			#define MY_TRACE_COLOR		RGB(146,0,123)
		</code>
	8�������ʹ�� g_Trace �ṩ�Ĵ����ӿڲ���ʵ��Log�����
	
	��ɡ�

	ע�⣺
	1��	��ǰ�汾��δ֧��Unicode�ֽڡ��뽫Ӧ�ó����Ϊ���ֽڡ�
	2�� ThreadSafeShowTraceXXXX ϵ�к��� �� ShowTraceXXXX ϵ�к����������̰߳�ȫ������
	3�� ShowTrace			����ͨ�����
		ShowTraceLn			�Դ����з������
		ShowTraceWithTime	��ʱ������
		ShowTraceLnWithTime	��ʱ��������ÿ������Դ����з�
		���� ThreadSafeShowTraceXXXX ϵ�� �Լ� DebugPrintXXX ϵ��ͬ�ϡ�
	4�� ���ļ��ɲ�֧�ֱ�����������Lib��Dll�ڡ�����Ҫ�ù��ܣ�����ϵFreeKnight��ȡ���߰汾
		
	���в��㣬����ϵFreeKnight  
	http://freeknigt.jimdo.com
*/
//------------------------------------------------------------------------
// Create ������ϸ˵��
//------------------------------------------------------------------------
/*
����˵����
@In��
	hInst			:	��������ⲿ�����hInst
	dwOutputStyle	:	�������( �ο������ �������(Output Style)����  )
	hWndMain		:	����Ӧ�ô�����û�� RichEdit �����㲻׼��ʹ�� OS_RICHEDIT ���ͣ�
						��ɴ���NULL��
						��Ӧ�ó��򴰿ھ��,���ô˾���ܰ�Trace����,ͬʱ��ʹ��Ӧ��
						���򴰿��ϵ�RichEdit�ؼ����Trace��Ϣ,��Ȼ���뱣֤hRichEdit
						��dwOutputStyleͬʱ������Чֵ,��hWndMain=NULLʱ,ʧȥ��������
						����.
	hRichEdit		:	����Ӧ�ô�����û�� RichEdit �����㲻׼��ʹ�� OS_RICHEDIT ���ͣ�
						��ɴ���NULL��
						������������� OS_RICHEDIT, �˴��������Ӧ�ó�����������ʾ
						Trace��Ϣ��RichEdit�ؼ��ľ��,��ͬʱ��֤hWndMain��Ч
	szFileName		:	������������� OS_FILE, �˴������ Log ���������ļ���(��·��)
	bExitAll		:	��־Trace���ڹر�ʱ,�Ƿ���ֹ���������Ϣѭ�����˳�������,��ֹ��Щ
						������Trace���ڵ������޴��ڵĳ����ڹر�Trace���ں�û���˳�����
@return��
	BOOL			:	�ɹ�����TRUE�����򷵻�FALSE

@Comment��
	���е�ITrace*��ͨ��g_Trace.GetTraceObject()���
*/
//------------------------------------------------------------------------
// FreeKnight �Ƽ����ִ��뷽ʽ��
//------------------------------------------------------------------------
/*
	1�� ������׺꣬����
	#define FKTRACE(a) g_Trace.ThreadSafeShowTraceLnWithTime(a)

	2�� DEBUG �� RELEASE ģʽ���룬����
	#ifdef _DEBUG
		#define FKTRACE(a) g_Trace.ThreadSafeShowTraceLnWithTime(a)
	#else
		#define FKTRACE(a) void(0)
	#endif

	3�� �ּ���������
	#define TRACE_LEVEL_1		// ��ͨ����
	#define TRACE_LEVEL_2		// �߼�����

	#if defined(TRACE_LEVEL_1) || defined(TRACE_LEVEL_2)
		#define FKTRACE(a)	g_Trace.ThreadSafeShowTraceWithTime(a)
	#else
		#define FKTRACE(a) void(0)
	#endif

	#if defined(TRACE_LEVEL_2)
		#define FKTRACEEX(a) g_Trace.ThreadSafeShowTraceLnWithTime(a)
	#else
		#define FKTRACE(a) void(0)
	#endif
	�����ⷢ��ʱ����ȥ����Ӧ�ĺ궨��

	4����ɫ�ּ���������
	#define FKINFO(a)		g_Trace.ThreadSafeShowTrace(a, RGB(255, 255, 255) )
	#define FKWARNING(a)	g_Trace.ThreadSafeShowTraceWithTime(a)
	#define FKERROR(a)		g_Trace.ThreadSafeShowTraceLnWithTime(a, RGB(255, 0, 0 ), 12, TS_ITALIC )

	5����ϳ��������������Լ���ɫ�Զ��崦������
	#define MY_TRACE_COLOR		RGB(255,0,0)
	#define AssertR(Expression, ReturnValue, Description ) \
		if (!(Expression)) \
		{\
			TraceColor(MY_TRACE_COLOR, "ERROR : %s[%d]\r\n\t%s\r\n",__FILE__,__LINE__,Description);\
			return ReturnValue;\
		}
	#define Assert(Expression) \
		if (!(Expression))\
		{\
			TraceColor(MY_TRACE_COLOR, "ERROR : %s[%d]\r\n\t%s\r\n",__FILE__,__LINE__,#Expression);\
			return 0;\
		}
*/
//------------------------------------------------------------------------
#include <Windows.h>
#include <richedit.h>
#include <stdio.h>
//------------------------------------------------------------------------
// Trace��ǰ�汾��
#define VERSION_QUERY_TRACE			0x0101
//------------------------------------------------------------------------
// һЩ��������
#ifdef _DEBUG
#define MAX_RICHEDIT_MESSAGE_LEN	(60 * 1024)		// RichEdit��������ɳ���
#else
#define MAX_RICHEDIT_MESSAGE_LEN	(32 * 1024)
#endif
#define MAX_TRACE_MSG_LEN			2048			// ÿ��Trace����󳤶�
#define MAX_THREAD_MESSAGE			4000			// ����߳���Ϣ������
#define WM_THREAD_TRACE_EX			(WM_USER+0x127)	// �̰߳�ȫTrace��Ϣ
//------------------------------------------------------------------------
// �������(Output Style)����
#define OS_OUTPUTDEBUGSTRING		0x00000001L		// OutputDebugString���
#define OS_FILE						0x00000002L		// ������ļ�
#define OS_RICHEDIT					0x00000004L		// �����RichEdit
#define OS_WINDOW					0x00000008L		// ���������
//------------------------------------------------------------------------
// ϵͳTrace����ɫ
#define TC_DEFAULT					RGB(0,0,255)	// Ĭ����ɫ
#define TC_WARNING					RGB(106,79,154)	// ������ɫ
#define TC_ERROR					RGB(255,0,0)	// ������ɫ
#define TC_SYSTEMMESSAGE			RGB(155,65,14)	// ϵͳ��Ϣ��ɫ
//------------------------------------------------------------------------
// ϵͳTrace�������С
#define TH_DEFAULT					9				// Ĭ��9����
#define TH_SYSTEMMESSAGE			12				// ϵͳ��Ϣ12����
//------------------------------------------------------------------------
// ϵͳTrace������ʽ
#define TS_DEFAULT					0				// Ĭ����ʽ
#define TS_SYSTEMMESSAGE			TS_BOLD			// ϵͳ��ϢΪ����
//------------------------------------------------------------------------
// RichEdit����
#define TS_BOLD						0x00000001
#define TS_ITALIC					0x00000002
#define TS_UNDERLINE				0x00000004
#define TS_STRIKEOUT				0x00000008
#define TS_PROTECTED				0x00000010
#define TS_LINK						0x00000020		/* Exchange hyperlink extension */
#define TS_SIZE						0x80000000
#define TS_COLOR					0x40000000
#define TS_FACE						0x20000000
#define TS_OFFSET					0x10000000
#define TS_CHARSET					0x08000000
//------------------------------------------------------------------------
// �ӿڶ���
struct ITrace
{
	// ֱ�ӵ�����Ϣ��ʾ
	// ˵�����������лس���("\r\n"), dwStyle��ѡֵ: TS_BOLD,TS_ITALIC ... ������ĺ궨��
	virtual BOOL FKShowTrace(LPCSTR szMsg, COLORREF color = TC_DEFAULT, 
							int nTextHeight = TH_DEFAULT, DWORD dwStyle = TS_DEFAULT, 
							BOOL bLn =	FALSE, BOOL bShowTime = FALSE) = 0;
	// �̰߳�ȫ������Ϣ��ʾ(ͨ����Ϣ����ʵ��)
	// ˵�����������лس���("\r\n"), dwStyle��ѡֵ: TS_BOLD,TS_ITALIC ... ������ĺ궨��
	virtual BOOL FKThreadSafeShowTrace(LPCSTR szMsg, COLORREF color = TC_DEFAULT, 
							int nTextHeight = TH_DEFAULT, DWORD dwStyle = TS_DEFAULT, 
							BOOL bLn = FALSE, BOOL bShowTime = FALSE) = 0;
	// �̰߳�ȫ��ȡ��Ϣ�����е���Ϣ����ʾ����
	virtual BOOL ThreadSafeGetTraceMsg() = 0;
	// ��ȡ�ڲ������Ĵ��ھ��
	virtual HWND GetTraceWindow() = 0;
	// �ͷ�(�����Դ����ڵ�����)
	virtual BOOL Release() = 0;
	// �̰߳�ȫ��ȡ��Ϣ�����е���Ϣ����������ʾ
	// ����void* szBuffer��Ӧtrace�ı���nSizeΪ������ı������С
	// BOOL bRemoveMsg;��ʾȡ����Ϣ���Ƿ�ͬʱɾ����Ϣ�����������Ϣ�������ɾ����Ϣ
	// ��ͬʱ����ThreadSafeGetTraceMsg����
	virtual BOOL ThreadSafeGetTraceMsgContext(char* szBuffer, int nSize, BOOL bRemoveMsg=FALSE) = 0;
	// ��̬�������������Ϣ���͵Ľӿڣ������ڳ������й����д򿪻�ر��ļ�Trace
	virtual BOOL SetOutputFile(bool bOutFile, LPCSTR szFileName = 0) = 0;
};
//------------------------------------------------------------------------
class CFKTrace
{
public:
	typedef BOOL (*ProcCreateTraceObject)(DWORD dwExpectVersion, 
									DWORD dwOutputStyle, HWND hMainWnd, 
									HWND hRichEdit, LPCSTR szFileName, 
									HINSTANCE hInst, BOOL bExitAll, 
									ITrace** ppTrace);
	CFKTrace()
	{
		m_hdll = NULL;
		m_pTrace = NULL;
		m_Flags = FALSE;
	}
	~CFKTrace()
	{
		Close();
	}
	// ��������˵����μ��Ϸ���ϸ����
	BOOL Create(HINSTANCE hInst, DWORD dwOutputStyle = OS_WINDOW | OS_FILE, HWND hWndMain = NULL, 
					HWND hRichEdit = NULL, LPCSTR szFileName = NULL, BOOL bExitAll = FALSE)
	{
		Close();

		try
		{
			if(m_hdll == NULL)
			{
#ifdef _DEBUG
				m_hdll = ::LoadLibrary("FKTrace_D.dll");
				if(m_hdll == NULL)
					throw "���� FKTraceD.dll ʧ�ܣ�";
#else
				m_hdll = ::LoadLibrary("FKTrace.dll");
				if(m_hdll == NULL)
					throw "���� FKTrace.dll ʧ�ܣ�";
#endif
			}
			ProcCreateTraceObject proc;
			proc = (ProcCreateTraceObject)GetProcAddress(m_hdll, "CreateTraceObject");
			if(proc == NULL)
				throw "�޷���ȡCreateTraceObject������ַ��";

			if(!proc(VERSION_QUERY_TRACE, dwOutputStyle, hWndMain, hRichEdit, szFileName, m_hdll, bExitAll, &m_pTrace))
				throw "����Trace����ʧ�ܣ�";

			return m_pTrace != NULL;
		}
		catch(LPCSTR szMsg)
		{
			OutputDebugString(szMsg);OutputDebugString("\r\n");
			return FALSE;
		}
		catch(...)
		{
			OutputDebugString("����Trace��������쳣��\r\n");
			return FALSE;
		}
	}
	//------------------------------------------------------------------------
	BOOL Create(ITrace* pTrace)
	{
		if (m_pTrace == NULL)
			m_pTrace = pTrace;
		m_Flags = TRUE;
		return m_pTrace != NULL;
	}
	BOOL ThreadSafeGetTraceMsg()
	{
		if (m_pTrace == NULL)
			return FALSE;
		return m_pTrace->ThreadSafeGetTraceMsg();
	}
	HWND GetTraceWindow()
	{
		if (m_pTrace == NULL)
			return NULL;
		return m_pTrace->GetTraceWindow();
	}
	ITrace* GetTraceObject()
	{
		return m_pTrace;
	}
	void Close()
	{
		if (m_Flags) return;
		if (m_pTrace)
		{
			m_pTrace->Release();
			m_pTrace = NULL;
		}
		if(m_hdll)
		{
			FreeLibrary(m_hdll);
			m_hdll = NULL;
		}
	}
	BOOL ShowTrace(LPCSTR szMsg, COLORREF color = TC_DEFAULT, int nTextHeight = TH_DEFAULT, 
								DWORD dwStyle = TS_DEFAULT)
	{
		if (m_pTrace == NULL)
		{
			OutputDebugString(szMsg);
			return TRUE;
		}
		return m_pTrace->FKShowTrace(szMsg,color,nTextHeight,dwStyle,FALSE,FALSE);
	}
	BOOL ShowTraceWithTime(LPCSTR szMsg, COLORREF color = TC_DEFAULT, int nTextHeight = TH_DEFAULT, 
								DWORD dwStyle = TS_DEFAULT)
	{
		if (m_pTrace == NULL)
		{
			OutputDebugString(szMsg);
			return TRUE;
		}
		return m_pTrace->FKShowTrace(szMsg,color,nTextHeight,dwStyle,FALSE,TRUE);
	}
	BOOL ShowTraceLn(LPCSTR szMsg, COLORREF color = TC_DEFAULT, int nTextHeight = TH_DEFAULT, 
								DWORD dwStyle = TS_DEFAULT)
	{
		if (m_pTrace == NULL)
		{
			OutputDebugString(szMsg);OutputDebugString("\r\n");
			return TRUE;
		}
		return 	m_pTrace->FKShowTrace(szMsg,color,nTextHeight,dwStyle,TRUE,FALSE);
	}
	BOOL ShowTraceLnWithTime(LPCSTR szMsg, COLORREF color = TC_DEFAULT, int nTextHeight = TH_DEFAULT, 
								DWORD dwStyle = TS_DEFAULT)
	{
		if (m_pTrace == NULL)
		{
			OutputDebugString(szMsg);OutputDebugString("\r\n");
			return TRUE;
		}
		return m_pTrace->FKShowTrace(szMsg,color,nTextHeight,dwStyle,TRUE,TRUE);
	}
	void DebugPrint(LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKShowTrace("���棺�ַ���������\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}

		if (m_pTrace == NULL)
			OutputDebugString(szBuffer);
		else
			m_pTrace->FKShowTrace(szBuffer,TC_DEFAULT,TH_DEFAULT,TS_DEFAULT,FALSE,FALSE);

		va_end(args);
	}
	void DebugPrintWithTime(LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKShowTrace("���棺�ַ���������\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}

		if (m_pTrace == NULL)
			OutputDebugString(szBuffer);
		else
			m_pTrace->FKShowTrace(szBuffer,TC_DEFAULT,TH_DEFAULT,TS_DEFAULT,FALSE,TRUE);

		va_end(args);
	}
	void DebugPrintLn(LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKShowTrace("���棺�ַ���������\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}
		
		if (m_pTrace == NULL){
			OutputDebugString(szBuffer);OutputDebugString("\r\n");}
		else
			m_pTrace->FKShowTrace(szBuffer,TC_DEFAULT,TH_DEFAULT,TS_DEFAULT,TRUE,FALSE);

		va_end(args);
	}
	void DebugPrintLnWithTime(LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKShowTrace("���棺�ַ���������\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}

		if (m_pTrace == NULL){
			OutputDebugString(szBuffer);OutputDebugString("\r\n");}
		else
			m_pTrace->FKShowTrace(szBuffer,TC_DEFAULT,TH_DEFAULT,TS_DEFAULT,TRUE,TRUE);

		va_end(args);
	}
	void DebugPrintColor(COLORREF color, LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);
		
		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];
		
		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
#ifdef _DEBUG
			m_pTrace->FKShowTrace("���棺�ַ���������\r\n", TC_ERROR);
#endif
			// �ַ���̫��,�ж�!
			if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
				szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}
		
		if (m_pTrace == NULL)
			OutputDebugString(szBuffer);
		else
			m_pTrace->FKShowTrace(szBuffer,color,TH_DEFAULT,TS_DEFAULT,FALSE,FALSE);
		
		va_end(args);
	}
	void DebugPrintColorWithTime(COLORREF color, LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);
		
		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];
		
		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
#ifdef _DEBUG
			m_pTrace->FKShowTrace("���棺�ַ���������\r\n", TC_ERROR);
#endif
			// �ַ���̫��,�ж�!
			if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
				szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}
		
		if (m_pTrace == NULL)
			OutputDebugString(szBuffer);
		else
			m_pTrace->FKShowTrace(szBuffer,color,TH_DEFAULT,TS_DEFAULT,FALSE,TRUE);
		
		va_end(args);
	}
	void DebugPrintColorLn(COLORREF color, LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);
		
		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];
		
		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
#ifdef _DEBUG
			m_pTrace->FKShowTrace("���棺�ַ���������\r\n", TC_ERROR);
#endif
			// �ַ���̫��,�ж�!
			if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
				szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}
		
		if (m_pTrace == NULL){
			OutputDebugString(szBuffer);OutputDebugString("\r\n");}
		else
			m_pTrace->FKShowTrace(szBuffer,color,TH_DEFAULT,TS_DEFAULT,TRUE,FALSE);
		
		va_end(args);
	}
	void DebugPrintColorLnWithTime(COLORREF color, LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);
		
		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];
		
		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
			#ifdef _DEBUG
			m_pTrace->FKShowTrace("���棺�ַ���������\r\n", TC_ERROR);
			#endif
			// �ַ���̫��,�ж�!
			if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
				szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}
		
		if (m_pTrace == NULL){
			OutputDebugString(szBuffer);OutputDebugString("\r\n");}
		else
			m_pTrace->FKShowTrace(szBuffer,color,TH_DEFAULT,TS_DEFAULT,TRUE,TRUE);
		
		va_end(args);
	}
	BOOL ThreadSafeShowTrace(LPCSTR szMsg, COLORREF color = TC_DEFAULT, int nTextHeight = TH_DEFAULT, 
								DWORD dwStyle = TS_DEFAULT)
	{
		if (m_pTrace == NULL)
		{
			OutputDebugString(szMsg);
			return TRUE;
		}
		return m_pTrace->FKThreadSafeShowTrace(szMsg, color, nTextHeight, dwStyle,FALSE,FALSE);
	}
	BOOL ThreadSafeShowTraceWithTime(LPCSTR szMsg, COLORREF color = TC_DEFAULT, int nTextHeight = TH_DEFAULT, 
								DWORD dwStyle = TS_DEFAULT)
	{
		if (m_pTrace == NULL)
		{
			OutputDebugString(szMsg);
			return TRUE;
		}
		return m_pTrace->FKThreadSafeShowTrace(szMsg, color, nTextHeight, dwStyle,FALSE,TRUE);
	}
	BOOL ThreadSafeShowTraceLn(LPCSTR szMsg, COLORREF color = TC_DEFAULT, int nTextHeight = TH_DEFAULT, 
								DWORD dwStyle = TS_DEFAULT)
	{
		if (m_pTrace == NULL)
		{
			OutputDebugString(szMsg);OutputDebugString("\r\n");
			return TRUE;
		}
		return m_pTrace->FKThreadSafeShowTrace(szMsg, color, nTextHeight, dwStyle,TRUE,FALSE);
	}
	BOOL ThreadSafeShowTraceLnWithTime(LPCSTR szMsg, COLORREF color = TC_DEFAULT, int nTextHeight = TH_DEFAULT, 
								DWORD dwStyle = TS_DEFAULT)
	{
		if (m_pTrace == NULL)
		{
			OutputDebugString(szMsg);OutputDebugString("\r\n");
			return TRUE;
		}

		return m_pTrace->FKThreadSafeShowTrace(szMsg, color, nTextHeight, dwStyle,TRUE,TRUE);
	}
	void ThreadSafeDebugPrint(LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKThreadSafeShowTrace("���棺�ַ���������\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}

		if (m_pTrace == NULL)
			OutputDebugString(szBuffer);
		else
			m_pTrace->FKThreadSafeShowTrace(szBuffer,TC_DEFAULT,TH_DEFAULT,TS_DEFAULT,FALSE,FALSE);

		va_end(args);
	}
	void ThreadSafeDebugPrintLn1(LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKThreadSafeShowTrace("���棺�ַ������� !\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}

		if (m_pTrace == NULL){
			OutputDebugString(szBuffer);OutputDebugString("\r\n");}
		else
			m_pTrace->FKThreadSafeShowTrace(szBuffer,TC_DEFAULT,TH_DEFAULT,TS_DEFAULT);

		va_end(args);
	}
	void ThreadSafeDebugPrintWithTime(LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKThreadSafeShowTrace("���棺�ַ���������\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}

		if (m_pTrace == NULL)
			OutputDebugString(szBuffer);
		else
			m_pTrace->FKThreadSafeShowTrace(szBuffer,TC_DEFAULT,TH_DEFAULT,TS_DEFAULT,FALSE,TRUE);

		va_end(args);
	}
	void ThreadSafeDebugPrintLn(LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKThreadSafeShowTrace("���棺�ַ���������\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}

		if (m_pTrace == NULL){
			OutputDebugString(szBuffer);OutputDebugString("\r\n");}
		else
			m_pTrace->FKThreadSafeShowTrace(szBuffer,TC_DEFAULT,TH_DEFAULT,TS_DEFAULT,TRUE,FALSE);

		va_end(args);
	}
	void ThreadSafeDebugPrintLnWithTime(LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKThreadSafeShowTrace("���棺�ַ���������\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}

		if (m_pTrace == NULL){
			OutputDebugString(szBuffer);OutputDebugString("\r\n");}
		else
			m_pTrace->FKThreadSafeShowTrace(szBuffer,TC_DEFAULT,TH_DEFAULT,TS_DEFAULT,TRUE,TRUE);

		va_end(args);
	}
	void ThreadSafeDebugPrintColor(COLORREF color, LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKThreadSafeShowTrace("���棺�ַ���������\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}

		if (m_pTrace == NULL)
			OutputDebugString(szBuffer);
		else
			m_pTrace->FKThreadSafeShowTrace(szBuffer,color,TH_DEFAULT,TS_DEFAULT,FALSE,FALSE);

		va_end(args);
	}
	void ThreadSafeDebugPrintColorWithTime(COLORREF color, LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKThreadSafeShowTrace("���棺�ַ���������\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}

		if (m_pTrace == NULL)
			OutputDebugString(szBuffer);
		else
			m_pTrace->FKThreadSafeShowTrace(szBuffer,color,TH_DEFAULT,TS_DEFAULT,FALSE,TRUE);

		va_end(args);
	}
	void ThreadSafeDebugPrintColorLn(COLORREF color, LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKThreadSafeShowTrace("���棺�ַ���������\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}

		if (m_pTrace == NULL){
			OutputDebugString(szBuffer);OutputDebugString("\r\n");}
		else
			m_pTrace->FKThreadSafeShowTrace(szBuffer,color,TH_DEFAULT,TS_DEFAULT,TRUE,FALSE);

		va_end(args);
	}
	void ThreadSafeDebugPrintColorLnWithTime(COLORREF color, LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;char szBuffer[MAX_TRACE_MSG_LEN];

		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf < 0)
		{
		#ifdef _DEBUG
				m_pTrace->FKThreadSafeShowTrace("���棺�ַ���������\r\n", TC_ERROR);
		#endif
		// �ַ���̫��,�ж�!
		if(strlen(szBuffer) >= MAX_TRACE_MSG_LEN-1)
			szBuffer[MAX_TRACE_MSG_LEN-1] = 0;
		}

		if (m_pTrace == NULL){
			OutputDebugString(szBuffer);OutputDebugString("\r\n");}
		else
			m_pTrace->FKThreadSafeShowTrace(szBuffer,color,TH_DEFAULT,TS_DEFAULT,TRUE,TRUE);

		va_end(args);
	}
public:
	ITrace*			m_pTrace;
private:
	HINSTANCE		m_hdll;
	BOOL			m_Flags;
};
//------------------------------------------------------------------------
// �����ṩ�ı�׼Trace
extern CFKTrace g_DefaultTrace;
//------------------------------------------------------------------------