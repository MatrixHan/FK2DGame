/**
*	created:		2012-6-14   18:40
*	filename: 		FKCrashDump
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/FKCrashDump.h"
#include "../Include/MiniVersion.h"
#include <tchar.h>
#include <stdio.h>
#include <tlhelp32.h>
//------------------------------------------------------------------------
#define _WIN32_WINDOWS 0x0500
#pragma comment(linker, "/defaultlib:dbghelp.lib")
//------------------------------------------------------------------------
#ifdef _countof
	#undef _countof
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif
//------------------------------------------------------------------------
#define DEF_LOG_FILE	_T("ErrLog.txt")
#define DEF_DUMP_FILE	_T("dump.dmp")
//------------------------------------------------------------------------
const int		NumCodeBytes = 16;	// ��ǰIP��ָ����ֽ���
ICrashDumpSink* CCrashDump::m_pSink = 0;
string			CCrashDump::m_szRpt = _T("");
FILE*			CCrashDump::m_fpLog = 0;
//------------------------------------------------------------------------
#ifndef VER_PLATFORM_WIN32s
#define VER_PLATFORM_WIN32s             0
#endif
#ifndef VER_PLATFORM_WIN32_WINDOWS
#define VER_PLATFORM_WIN32_WINDOWS      1
#endif
#ifndef VER_PLATFORM_WIN32_NT
#define VER_PLATFORM_WIN32_NT           2
#endif
#ifndef VER_PLATFORM_WIN32_CE
#define VER_PLATFORM_WIN32_CE           3
#endif
//------------------------------------------------------------------------
/*
         dwPlatFormID  dwMajorVersion  dwMinorVersion  dwBuildNumber
95             1              4               0             950
95 SP1         1              4               0        >950 && <=1080
95 OSR2        1              4             <10           >1080
98             1              4              10            1998
98 SP1         1              4              10       >1998 && <2183
98 SE          1              4              10          >=2183
ME             1              4              90            3000

NT 3.51        2              3              51
NT 4           2              4               0            1381
2000           2              5               0            2195
XP             2              5               1            2600
2003 Server    2              5               2            3790

CE             3

*/
//------------------------------------------------------------------------
BOOL CCrashDump::GetWinVer(LPTSTR pszVersion, int *nVersion, LPTSTR pszMajorMinorBuild)
{
	if (!pszVersion || !nVersion || !pszMajorMinorBuild)
		return FALSE;
	lstrcpy(pszVersion, WUNKNOWNSTR);
	*nVersion = WUNKNOWN;

	OSVERSIONINFO osinfo;
	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (!GetVersionEx(&osinfo))
		return FALSE;

	DWORD dwPlatformId   = osinfo.dwPlatformId;
	DWORD dwMinorVersion = osinfo.dwMinorVersion;
	DWORD dwMajorVersion = osinfo.dwMajorVersion;
	DWORD dwBuildNumber  = osinfo.dwBuildNumber & 0xFFFF;	// Win 95 needs this

	wsprintf(pszMajorMinorBuild, _T("%u.%u.%u"), dwMajorVersion, dwMinorVersion, dwBuildNumber);

	if ((dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (dwMajorVersion == 4))
	{
		if ((dwMinorVersion < 10) && (dwBuildNumber == 950))
		{
			lstrcpy(pszVersion, W95STR);
			*nVersion = W95;
		}
		else if ((dwMinorVersion < 10) && 
			((dwBuildNumber > 950) && (dwBuildNumber <= 1080)))
		{
			lstrcpy(pszVersion, W95SP1STR);
			*nVersion = W95SP1;
		}
		else if ((dwMinorVersion < 10) && (dwBuildNumber > 1080))
		{
			lstrcpy(pszVersion, W95OSR2STR);
			*nVersion = W95OSR2;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber == 1998))
		{
			lstrcpy(pszVersion, W98STR);
			*nVersion = W98;
		}
		else if ((dwMinorVersion == 10) && 
			((dwBuildNumber > 1998) && (dwBuildNumber < 2183)))
		{
			lstrcpy(pszVersion, W98SP1STR);
			*nVersion = W98SP1;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber >= 2183))
		{
			lstrcpy(pszVersion, W98SESTR);
			*nVersion = W98SE;
		}
		else if (dwMinorVersion == 90)
		{
			lstrcpy(pszVersion, WMESTR);
			*nVersion = WME;
		}
	}
	else if (dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if ((dwMajorVersion == 3) && (dwMinorVersion == 51))
		{
			lstrcpy(pszVersion, WNT351STR);
			*nVersion = WNT351;
		}
		else if ((dwMajorVersion == 4) && (dwMinorVersion == 0))
		{
			lstrcpy(pszVersion, WNT4STR);
			*nVersion = WNT4;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 0))
		{
			lstrcpy(pszVersion, W2KSTR);
			*nVersion = W2K;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 1))
		{
			lstrcpy(pszVersion, WXPSTR);
			*nVersion = WXP;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 2))
		{
			lstrcpy(pszVersion, W2003SERVERSTR);
			*nVersion = W2003SERVER;
		}
	}
	else if (dwPlatformId == VER_PLATFORM_WIN32_CE)
	{
		lstrcpy(pszVersion, WCESTR);
		*nVersion = WCE;
	}
	return TRUE;
}
//------------------------------------------------------------------------
CCrashDump::CCrashDump()
{
	m_previousFilter = SetUnhandledExceptionFilter(UnhandledExceptionFilter);

	//Ĭ�ϵ�ErrLog�ļ�����
	_tcscpy(m_szLogFileName,DEF_LOG_FILE);
	_tcscpy(m_szDumpFileName,DEF_DUMP_FILE);

	//Ĭ��dump����
	for(int i = 0;i < enDumpCount;i++)
		m_pbOn[i] = TRUE;

	//Ĭ��dump����
	m_lCrashDumpType = IsWindows98() ? MiniDumpNormal : MiniDumpWithIndirectlyReferencedMemory;
}
//------------------------------------------------------------------------
CCrashDump::~CCrashDump()
{
	if(m_previousFilter)
		SetUnhandledExceptionFilter( m_previousFilter );
}
//------------------------------------------------------------------------
void CCrashDump::OnDumpStart()
{
	m_fpLog = fopen("Log\\cd.txt","wb+");
	if(m_pSink)m_pSink->OnDumpStart(Instance());
}
//------------------------------------------------------------------------
LONG CCrashDump::OnDumpFinish(PEXCEPTION_POINTERS pExceptionInfo)
{
	if(m_fpLog)fclose(m_fpLog);
	if(m_pSink)
	{
		return m_pSink->OnDumpFinish(Instance(),pExceptionInfo);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}
//------------------------------------------------------------------------
LONG WINAPI CCrashDump::UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	static BOOL bFirstTime = TRUE;
	if (!bFirstTime)
	{
		Dump("�쳣 : �쳣�������������쳣");
		return EXCEPTION_EXECUTE_HANDLER;
	}

	bFirstTime = FALSE;

	//����
	OnDumpStart();

	if(Instance()->m_pbOn[enDumpLog])
		Instance()->DumpLog(pExceptionInfo);

	if(Instance()->m_pbOn[enDumpMiniDump])
		Instance()->DumpMiniDump(pExceptionInfo);

	return OnDumpFinish(pExceptionInfo);
}
//------------------------------------------------------------------------
void CCrashDump::DumpMiniDump(PEXCEPTION_POINTERS pExceptionInfo)
{
	HANDLE hMiniDumpFile = CreateFile(
		m_szDumpFileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
		NULL);

	if (hMiniDumpFile != INVALID_HANDLE_VALUE)
	{
		DumpMiniDump(pExceptionInfo,hMiniDumpFile);
		CloseHandle(hMiniDumpFile);
	}
}
//------------------------------------------------------------------------
BOOL CCrashDump::IsWindows98()
{
	DWORD dwVersion = GetVersion();
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
	DWORD dwBuild;

	BOOL bBeforeWin2000 = FALSE;

	if (dwVersion < 0x80000000)              // Windows NT/2000, Whistler
	{
		dwBuild = (DWORD)(HIWORD(dwVersion));
	}
	else if (dwWindowsMajorVersion < 4)      // Win32s
	{
		dwBuild = (DWORD)(HIWORD(dwVersion) & ~0x8000);
		bBeforeWin2000 = TRUE;
	}
	else                                     // Windows 95/98/Me
	{
		bBeforeWin2000 = TRUE;
	}

	return bBeforeWin2000;
}
//------------------------------------------------------------------------
BOOL CALLBACK MiniDumpCallback(PVOID pParam,const PMINIDUMP_CALLBACK_INPUT pInput,PMINIDUMP_CALLBACK_OUTPUT pOutput) 
{
	BOOL bRet = FALSE; 

	if( pInput == 0 ) 
		return FALSE; 

	if( pOutput == 0 ) 
		return FALSE; 

	switch( pInput->CallbackType ) 
	{
	case IncludeModuleCallback: 
		{
			_tprintf( _T("IncludeModuleCallback (module: %08I64x) \n"), 
			pInput->IncludeModule.BaseOfImage); 
		}
		break; 
	case IncludeThreadCallback: 
		{
			_tprintf( _T("IncludeThreadCallback (thread: %x) \n"), 
			pInput->IncludeThread.ThreadId);  
		}
		break; 
	case ModuleCallback: 
		{
			wprintf( L"ģ���ջ ( ģ����: %s) \n", pInput->Module.FullPath ); 
		}
		break; 
	case ThreadCallback: 
		{
			_tprintf( _T("�̵߳��� ( �߳�ID: %x) \n"), pInput->Thread.ThreadId ); 
		}
		break; 
	case ThreadExCallback: 
		{
			_tprintf( _T("�̵߳��� ( �߳�ID: %x) \n"), pInput->ThreadEx.ThreadId );  
		}
		break; 
	case MemoryCallback: 
		{
			_tprintf( _T("�ڴ��ջ\n") ); 
		}
		break;
	}

	return TRUE; 
}
//------------------------------------------------------------------------
void CCrashDump::DumpMiniDump(PEXCEPTION_POINTERS pExceptionInfo,HANDLE hFile)
{
	if (pExceptionInfo == NULL) 
	{
		__try 
		{
			OutputDebugString(_T("raising exception\r\n"));
			RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL);
		} 
		__except(DumpMiniDump(GetExceptionInformation()),
			EXCEPTION_CONTINUE_EXECUTION) 
		{
		}
	}
	else
	{
		MINIDUMP_EXCEPTION_INFORMATION eInfo;
		eInfo.ThreadId = GetCurrentThreadId();
		eInfo.ExceptionPointers = pExceptionInfo;
		eInfo.ClientPointers = FALSE;

		MINIDUMP_CALLBACK_INFORMATION mci; 

		mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback; 
		mci.CallbackParam       = 0;

		// ע�� MiniDumpWriteDump ��Win98����Ч��
		MiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			hFile,
			(MINIDUMP_TYPE)m_lCrashDumpType,
			pExceptionInfo ? &eInfo : NULL,
			NULL,
			&mci);
	}
}
//------------------------------------------------------------------------
void CCrashDump::DumpLog(PEXCEPTION_POINTERS pExceptionInfo)
{
	//MessageBox(NULL, "1", NULL, NULL );
	m_szRpt = _T("");

	Dump("---------------------------------------------------\r\n");
	Dump("------------------FK DumpCrash��ʼ-----------------\r\n");
	Dump("---------------------------------------------------\r\n");
	Dump("-----------------------������Ϣ--------------------\r\n");
	Dump("---------------------------------------------------\r\n");
	if(m_pbOn[enDumpBasicInfo])
		DumpBasicInfo(pExceptionInfo);
	Dump("---------------------------------------------------\r\n");
	Dump("-----------------------ģ���б�--------------------\r\n");
	Dump("---------------------------------------------------\r\n");
	if(m_pbOn[enDumpModuleList])
		DumpModuleList(pExceptionInfo);
	Dump("---------------------------------------------------\r\n");
	Dump("-----------------------�����б�--------------------\r\n");
	Dump("---------------------------------------------------\r\n");
	if(m_pbOn[enDumpWindowList])
		DumpWindowList(pExceptionInfo);
	Dump("---------------------------------------------------\r\n");
	Dump("-----------------------�����б�--------------------\r\n");
	Dump("---------------------------------------------------\r\n");
	if(m_pbOn[enDumpProcessList])
		DumpProcessList(pExceptionInfo);
	Dump("---------------------------------------------------\r\n");
	Dump("----------------------�Ĵ�����Ϣ-------------------\r\n");
	Dump("---------------------------------------------------\r\n");
	if(m_pbOn[enDumpRegister])
		DumpRegister(pExceptionInfo);
	Dump("---------------------------------------------------\r\n");
	Dump("---------------------���ö�ջ��Ϣ------------------\r\n");
	Dump("---------------------------------------------------\r\n");
	if(m_pbOn[enDumpCallStack])
	{
		//����ط�������һ��......
		CONTEXT context = *pExceptionInfo->ContextRecord;
		DumpCallStack(&context,GetCurrentThread());
	}

	if(m_pSink)m_pSink->OnDump(Instance(),pExceptionInfo);
	HANDLE hLogFile = CreateFile(
		m_szLogFileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
		NULL);

	if (hLogFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwWrite;
		WriteFile(hLogFile,m_szRpt.c_str(),m_szRpt.size(),&dwWrite,0);
		CloseHandle(hLogFile);
	}

	Dump("---------------------------------------------------\r\n");
	Dump("-------------------FK DumpCrash���----------------\r\n");
	Dump("---------------------------------------------------\r\n");
}
//------------------------------------------------------------------------
void CCrashDump::Dump(LPCTSTR szMsg,...)
{
	TCHAR szBuff[1024];
	va_list argptr;      
	va_start( argptr, szMsg );
	vsprintf( szBuff, szMsg, argptr );
	va_end( argptr );

	m_szRpt += szBuff;

	OutputDebugString(szBuff);

	if(m_fpLog)
	{
		fprintf(m_fpLog,szBuff);
		fflush(m_fpLog);
	}
}
//------------------------------------------------------------------------
//Dumpϵͳ���쳣�Ļ�����Ϣ
void CCrashDump::DumpBasicInfo(PEXCEPTION_POINTERS pExceptionInfo)
{
	PEXCEPTION_RECORD pException = pExceptionInfo->ExceptionRecord;
	PCONTEXT          pContext   = pExceptionInfo->ContextRecord;

	TCHAR *pszCrashModuleFileName = (TCHAR*)GetCrashModuleName(pExceptionInfo);

	Dump(_T("�����ļ���\t%s\r\n"),pszCrashModuleFileName );
	Dump(_T("������Ϣ��\t%s\r\n"), GetExceptionDesc(pExceptionInfo) );
	Dump(_T("�����룺\t0x%08x\r\n"), pException->ExceptionCode );
	Dump(_T("SegCS��\t\t%04x\r\n"), pContext->SegCs );
	Dump(_T("������Eip��\t%08x\r\n"),  pContext->Eip );
	//Dump(_T("Dump���̣�\t%s ����������Ϣ��\t%s �����룺\t\t0x%08x SegCS��\t\t%04x ������Eip��\t%08x.\r\n"),
	//	GetExceptionDesc(pExceptionInfo),
	//	pException->ExceptionCode,
	//	pszCrashModuleFileName, pContext->SegCs, pContext->Eip);

	Dump("�ļ�ƫ�ƣ�\t%08x@%s\t\t\r\n",GetCrashFileOffset(pExceptionInfo),pszCrashModuleFileName);

	Dump("---------------------------------------------------\r\n");
	DumpModuleInfo(GetModuleHandle(pszCrashModuleFileName));
	Dump("---------------------------------------------------\r\n");
	SYSTEMTIME st;
	GetLocalTime(&st);
	TCHAR szTimeBuffer[256];
	sprintf(szTimeBuffer,"崻�ʱ�䣺\t\t%04d��%02d��%02d�� %02d:%02d:%02d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);	
	Dump(_T("%s.\r\n"), szTimeBuffer);

	TCHAR szModuleName[MAX_PATH*2];
	ZeroMemory(szModuleName, sizeof(szModuleName));
	if (GetModuleFileName(0, szModuleName, _countof(szModuleName)-2) <= 0)
		lstrcpy(szModuleName, _T("Unknown"));

	TCHAR szUserName[200];
	ZeroMemory(szUserName, sizeof(szUserName));
	DWORD UserNameSize = _countof(szUserName)-2;
	if (!GetUserName(szUserName, &UserNameSize))
		lstrcpy(szUserName, _T("Unknown"));

	Dump(_T("崻��ļ���\t\t%s, ִ����ϵͳ�û����� %s.\r\n"), szModuleName, szUserName);

	//����ϵͳ���
	TCHAR szWinVer[50], szMajorMinorBuild[50];
	int nWinVer;
	GetWinVer(szWinVer, &nWinVer, szMajorMinorBuild);
	Dump(_T("����ϵͳ:\t\t%s (%s).\r\n"), 
		szWinVer, szMajorMinorBuild);

	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);
	Dump(_T("\t\t\t%d ����CPU, CPU���� %d.\r\n"),
		SystemInfo.dwNumberOfProcessors, SystemInfo.dwProcessorType);

	MEMORYSTATUS SysMemInfo;
	SysMemInfo.dwLength = sizeof(SysMemInfo);
	GlobalMemoryStatus(&SysMemInfo);
#define ONEM (1024 * 1024)
	//�ڴ���Ϣ
	Dump("-----------------------�ڴ�------------------------\r\n");
	Dump(_T("%d%% �ڴ�����ʹ����.\r\n"), SysMemInfo.dwMemoryLoad);
	Dump(_T("%d MBytes physical memory.\r\n"), (SysMemInfo.dwTotalPhys +
		ONEM - 1) / ONEM);
	Dump(_T("%d MBytes physical memory free.\r\n"), 
		(SysMemInfo.dwAvailPhys + ONEM - 1) / ONEM);
	Dump(_T("%d MBytes paging file.\r\n"), (SysMemInfo.dwTotalPageFile +
		ONEM - 1) / ONEM);
	Dump(_T("%d MBytes paging file free.\r\n"), 
		(SysMemInfo.dwAvailPageFile + ONEM - 1) / ONEM);
	Dump(_T("%d MBytes user address space.\r\n"), 
		(SysMemInfo.dwTotalVirtual + ONEM - 1) / ONEM);
	Dump(_T("%d MBytes user address space free.\r\n"), 
		(SysMemInfo.dwAvailVirtual + ONEM - 1) / ONEM);
	Dump("---------------------------------------------------\r\n");
	// ����access violation���͵��쳣
	if (pException->ExceptionCode == STATUS_ACCESS_VIOLATION &&
		pException->NumberParameters >= 2)
	{
		TCHAR szDebugMessage[1000];
		const TCHAR* readwrite = _T("��ȡ");
		if (pException->ExceptionInformation[0])
			readwrite = _T("д��");
		wsprintf(szDebugMessage, _T("%s �ڴ�λ�� %08x ������һ�������ַ��д����\r\n"),
			readwrite, pException->ExceptionInformation[1]);

#ifdef	_DEBUG
		//��AccessViolation�Ƕ�����д���µ�
		OutputDebugString(_T("Exception handler: "));
		OutputDebugString(szDebugMessage);
#endif

		Dump(_T("%s"), szDebugMessage);
	}
}
//------------------------------------------------------------------------
void CCrashDump::DumpModuleList(PEXCEPTION_POINTERS pExceptionInfo)
{
	Dump("\r\n��ǰģ���б�:\r\n");
	HANDLE hModuleShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetCurrentProcessId());
	MODULEENTRY32 me2 = {sizeof(me2)};
	BOOL bOk = Module32First(hModuleShot,&me2);
	for(;bOk;bOk = Module32Next(hModuleShot,&me2))
	{
		Dump(_T("Base = %p\t%s\r\n"),me2.modBaseAddr,me2.szExePath);
		DumpModuleInfo(me2.hModule);
	}
	CloseHandle(hModuleShot);
}
//------------------------------------------------------------------------
void CCrashDump::DumpWindowList(PEXCEPTION_POINTERS pExceptionInfo)
{
	Dump("��ǰ�����б�:\r\n");
	char szWindowText[256];
	HWND hWndTopLevel=GetDesktopWindow();
	hWndTopLevel=GetWindow(hWndTopLevel,GW_CHILD);
	for(;hWndTopLevel;hWndTopLevel=GetWindow(hWndTopLevel,GW_HWNDNEXT))
	{
		GetWindowText(hWndTopLevel,szWindowText,256);
		if(	strcmp(szWindowText,"") != 0 
			&& stricmp(szWindowText,"Default IME") != 0 
			&& stricmp(szWindowText,"DDE Server Window") != 0)
		{
			char szTemp[6];
			if(strlen(szWindowText) >= 5)
			{
				memcpy(szTemp,szWindowText,5);
				szTemp[5] = 0;
				if(stricmp(szTemp,"msime") == 0)continue;
			}
			DWORD dwProcessID,dwThreadID;
			//��ô������ڵĽ��̺��߳�
			dwThreadID=GetWindowThreadProcessId(hWndTopLevel,&dwProcessID);
			Dump(_T("\t���ھ�� = %x\t����ID = %d\t\t%s\r\n"),hWndTopLevel,dwProcessID,szWindowText);
		}
	}
}
//------------------------------------------------------------------------
void CCrashDump::DumpProcessList(PEXCEPTION_POINTERS pExceptionInfo)
{
	Dump("��ǰ�����б�:\r\n");
	PROCESSENTRY32 me={sizeof(me)};
	HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,GetCurrentProcessId());
	BOOL fOk=Process32First(hSnapshot,&me);
	for(;fOk;fOk=Process32Next(hSnapshot,&me))
	{
		Dump(_T("\t����ID = %d\t%s\r\n"),me.th32ProcessID,me.szExeFile);
	}
	CloseHandle(hSnapshot);
}
//------------------------------------------------------------------------
void CCrashDump::DumpRegister(PEXCEPTION_POINTERS pExceptionInfo)
{
	PCONTEXT pContext   = pExceptionInfo->ContextRecord;
	Dump(_T("�Ĵ���������:\r\n"));
	Dump(_T("\tEDI:    0x%08x  ESI: 0x%08x  EAX:   0x%08x\r\n"),
		pContext->Edi, pContext->Esi, pContext->Eax);
	Dump(_T("\tEBX:    0x%08x  ECX: 0x%08x  EDX:   0x%08x\r\n"),
		pContext->Ebx, pContext->Ecx, pContext->Edx);
	Dump(_T("\tEIP:    0x%08x  EBP: 0x%08x  SegCs: 0x%08x\r\n"),
		pContext->Eip, pContext->Ebp, pContext->SegCs);
	Dump(_T("\tEFlags: 0x%08x  ESP: 0x%08x  SegSs: 0x%08x\r\n"),
		pContext->EFlags, pContext->Esp, pContext->SegSs);

	//��ǰIP��ָ��ļ����ֽڵ�ֵ
	//��Ϊ�޷��ж��ڴ��Ƿ�ɶ�����������try-catch
	Dump(_T("\r\nBytes at CS:EIP:\r\n\t"));
	BYTE * code = (BYTE *)pContext->Eip;
	for (int codebyte = 0; codebyte < NumCodeBytes; codebyte++)
	{
		__try
		{
			Dump(_T("%02x "), code[codebyte]);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			Dump(_T("?? "));
		}
	}
	Dump(_T("\r\n"));
}
//------------------------------------------------------------------------
//Dump����ջ - ������WinDbg�Ĵ�ӡCallStack����
void CCrashDump::DumpCallStack(PCONTEXT pContext,HANDLE hThread)
{	
	SymSetOptions(SYMOPT_DEFERRED_LOADS);//SYMOPT_DEBUG

	if(!SymInitialize(GetCurrentProcess(),0,TRUE))
	{
		Dump("\r\n%x\r\n\r\n",GetLastError());
		return;
	}

	WriteStackDetails(pContext,hThread);
	SymCleanup(GetCurrentProcess());
}
//------------------------------------------------------------------------
LPCTSTR CCrashDump::GetExceptionDescription(DWORD ExceptionCode)
{
	struct ExceptionNames
	{
		DWORD	ExceptionCode;
		TCHAR *	ExceptionName;
	};

#if 0  //winnt.h
#define STATUS_WAIT_0                    ((DWORD   )0x00000000L)    
#define STATUS_ABANDONED_WAIT_0          ((DWORD   )0x00000080L)    
#define STATUS_USER_APC                  ((DWORD   )0x000000C0L)    
#define STATUS_TIMEOUT                   ((DWORD   )0x00000102L)    
#define STATUS_PENDING                   ((DWORD   )0x00000103L)    
#define STATUS_SEGMENT_NOTIFICATION      ((DWORD   )0x40000005L)    
#define STATUS_GUARD_PAGE_VIOLATION      ((DWORD   )0x80000001L)    
#define STATUS_DATATYPE_MISALIGNMENT     ((DWORD   )0x80000002L)    
#define STATUS_BREAKPOINT                ((DWORD   )0x80000003L)    
#define STATUS_SINGLE_STEP               ((DWORD   )0x80000004L)    
#define STATUS_ACCESS_VIOLATION          ((DWORD   )0xC0000005L)    
#define STATUS_IN_PAGE_ERROR             ((DWORD   )0xC0000006L)    
#define STATUS_INVALID_HANDLE            ((DWORD   )0xC0000008L)    
#define STATUS_NO_MEMORY                 ((DWORD   )0xC0000017L)    
#define STATUS_ILLEGAL_INSTRUCTION       ((DWORD   )0xC000001DL)    
#define STATUS_NONCONTINUABLE_EXCEPTION  ((DWORD   )0xC0000025L)    
#define STATUS_INVALID_DISPOSITION       ((DWORD   )0xC0000026L)    
#define STATUS_ARRAY_BOUNDS_EXCEEDED     ((DWORD   )0xC000008CL)    
#define STATUS_FLOAT_DENORMAL_OPERAND    ((DWORD   )0xC000008DL)    
#define STATUS_FLOAT_DIVIDE_BY_ZERO      ((DWORD   )0xC000008EL)    
#define STATUS_FLOAT_INEXACT_RESULT      ((DWORD   )0xC000008FL)    
#define STATUS_FLOAT_INVALID_OPERATION   ((DWORD   )0xC0000090L)    
#define STATUS_FLOAT_OVERFLOW            ((DWORD   )0xC0000091L)    
#define STATUS_FLOAT_STACK_CHECK         ((DWORD   )0xC0000092L)    
#define STATUS_FLOAT_UNDERFLOW           ((DWORD   )0xC0000093L)    
#define STATUS_INTEGER_DIVIDE_BY_ZERO    ((DWORD   )0xC0000094L)    
#define STATUS_INTEGER_OVERFLOW          ((DWORD   )0xC0000095L)    
#define STATUS_PRIVILEGED_INSTRUCTION    ((DWORD   )0xC0000096L)    
#define STATUS_STACK_OVERFLOW            ((DWORD   )0xC00000FDL)    
#define STATUS_CONTROL_C_EXIT            ((DWORD   )0xC000013AL)    
#define STATUS_FLOAT_MULTIPLE_FAULTS     ((DWORD   )0xC00002B4L)    
#define STATUS_FLOAT_MULTIPLE_TRAPS      ((DWORD   )0xC00002B5L)    
#define STATUS_ILLEGAL_VLM_REFERENCE     ((DWORD   )0xC00002C0L)     
#endif

	ExceptionNames ExceptionMap[] =
	{
		{0x40010005, _T("Control-C")},
		{0x40010008, _T("Control-Break")},
		{0x80000002, _T("Datatype Misalignment")},
		{0x80000003, _T("Breakpoint �ϵ�")},
		{0xc0000005, _T("Access Violation �����ַ��д����")},
		{0xc0000006, _T("In Page Error RAMҳ����ʴ���")},
		{0xc0000017, _T("No Memory �ڴ治��")},
		{0xc000001d, _T("Illegal Instruction")},
		{0xc0000025, _T("Noncontinuable Exception")},
		{0xc0000026, _T("Invalid Disposition")},
		{0xc000008c, _T("Array Bounds Exceeded")},
		{0xc000008d, _T("Float Denormal Operand")},
		{0xc000008e, _T("Float Divide by Zero ������������")},
		{0xc000008f, _T("Float Inexact Result")},
		{0xc0000090, _T("Float Invalid Operation")},
		{0xc0000091, _T("Float Overflow ������Խ�����")},
		{0xc0000092, _T("Float Stack Check")},
		{0xc0000093, _T("Float Underflow ����������")},
		{0xc0000094, _T("Integer Divide by Zero ���γ�����")},
		{0xc0000095, _T("Integer Overflow ����Խ�����")},
		{0xc0000096, _T("Privileged Instruction")},
		{0xc00000fD, _T("Stack Overflow ��ջ���")},
		{0xc0000142, _T("DLL Initialization Failed ��̬���ӿ��ʼ��ʧ��")},
		{0xe06d7363, _T("Microsoft C++ Exception")},
	};

	for (int i = 0; i < sizeof(ExceptionMap) / sizeof(ExceptionMap[0]); i++)
		if (ExceptionCode == ExceptionMap[i].ExceptionCode)
			return ExceptionMap[i].ExceptionName;

	return _T("Unknown Exception");
}
//------------------------------------------------------------------------
LPTSTR CCrashDump::GetFilePart(LPCTSTR source)
{
	TCHAR *result = lstrrchr(source, _T('\\'));
	if (result)
		result++;
	else
		result = (LPTSTR)source;
	return result;
}
//------------------------------------------------------------------------
LPTSTR CCrashDump::lstrrchr(LPCTSTR string, int ch)
{
	TCHAR *start = (TCHAR *)string;

	while (*string++)                       /* ���ַ���ĩβ */
		;
	/* �Ӻ���ǰ�� */
	while (--string != start && *string != (TCHAR) ch)
		;

	if (*string == (TCHAR) ch)
		return (TCHAR *)string;

	return 0;
}
//------------------------------------------------------------------------
void CCrashDump::DumpModuleInfo(HINSTANCE ModuleHandle)
{
	bool rc = false;
	TCHAR szModName[MAX_PATH*2];
	ZeroMemory(szModName, sizeof(szModName));

	__try
	{
		if (GetModuleFileName(ModuleHandle, szModName, sizeof(szModName)-2) > 0)
		{
			IMAGE_DOS_HEADER *DosHeader = (IMAGE_DOS_HEADER*)ModuleHandle;
			if (IMAGE_DOS_SIGNATURE != DosHeader->e_magic)
				return;

			IMAGE_NT_HEADERS *NTHeader = (IMAGE_NT_HEADERS*)((TCHAR *)DosHeader
				+ DosHeader->e_lfanew);
			if (IMAGE_NT_SIGNATURE != NTHeader->Signature)
				return;

			// ȡ��szModName�ļ����ںʹ�С
			HANDLE ModuleFile = CreateFile(szModName, GENERIC_READ,
				FILE_SHARE_READ, 0, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, 0);

			TCHAR TimeBuffer[100];
			TimeBuffer[0] = _T('\0');

			DWORD FileSize = 0;
			if (ModuleFile != INVALID_HANDLE_VALUE)
			{
				FileSize = GetFileSize(ModuleFile, 0);
				FILETIME LastWriteTime;
				if (GetFileTime(ModuleFile, 0, 0, &LastWriteTime))
				{
					FormatTime(TimeBuffer, LastWriteTime);
				}
				CloseHandle(ModuleFile);
			}

			Dump(_T("Image Base: 0x%08x\t\tImage Size: 0x%08x\r\n"), 
				NTHeader->OptionalHeader.ImageBase, 
				NTHeader->OptionalHeader.SizeOfImage), 

				Dump(_T("ChecksumУ����:  0x%08x\tʱ���: 0x%08x\r\n"), 
				NTHeader->OptionalHeader.CheckSum,
				NTHeader->FileHeader.TimeDateStamp);

			Dump(_T("�ļ���С:  %-10d\t\t�ļ�ʱ��:  %s\r\n"),
				FileSize, TimeBuffer);
			Dump("---------------------------------------------------\r\n");
			Dump(_T("�汾��Ϣ:\r\n"));

			CMiniVersion ver(szModName);
			TCHAR szBuf[200];
			WORD dwBuf[4];

			ver.GetCompanyName(szBuf, sizeof(szBuf)-1);
			Dump(_T("   ��˾��:\t\t%s\r\n"), szBuf);

			ver.GetProductName(szBuf, sizeof(szBuf)-1);
			Dump(_T("   ��Ʒ��:\t\t%s\r\n"), szBuf);

			ver.GetFileDescription(szBuf, sizeof(szBuf)-1);
			Dump(_T("   �ļ�����:\t\t%s\r\n"), szBuf);

			ver.GetFileVersion(dwBuf);
			Dump(_T("   �ļ��汾��:\t\t%d.%d.%d.%d\r\n"), 
				dwBuf[0], dwBuf[1], dwBuf[2], dwBuf[3]);

			ver.GetProductVersion(dwBuf);
			Dump(_T("   ��Ʒ�汾��:\t\t%d.%d.%d.%d\r\n"), 
				dwBuf[0], dwBuf[1], dwBuf[2], dwBuf[3]);

			ver.Release();
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}
}
//------------------------------------------------------------------------
void CCrashDump::FormatTime(LPTSTR output, FILETIME TimeToPrint)
{
	output[0] = _T('\0');
	WORD Date, Time;
	if (FileTimeToLocalFileTime(&TimeToPrint, &TimeToPrint) &&
		FileTimeToDosDateTime(&TimeToPrint, &Date, &Time))
	{
		wsprintf(output, _T("%d/%d/%d %02d:%02d:%02d"),
			(Date / 32) & 15, Date & 31, (Date / 512) + 1980,
			(Time >> 11), (Time >> 5) & 0x3F, (Time & 0x1F) * 2);
	}
}
//------------------------------------------------------------------------
void CCrashDump::WriteStackDetails(PCONTEXT pContext,HANDLE hThread)
{
	Dump( _T("���ö�ջ:\r\n") );

	Dump( _T("\t��ַ\tFrame(EBP)\t����\t\tԴ�ļ�\r\n") );

	DWORD dwMachineType = 0;
	// Could use SymSetOptions here to add the SYMOPT_DEFERRED_LOADS flag

	STACKFRAME sf;
	memset( &sf, 0, sizeof(sf) );

#ifdef _M_IX86
	//���ε�����Ҫ��ʼ��STACKFRAME�ṹ - ����Ҫ��I386������
	sf.AddrPC.Offset       = pContext->Eip;
	sf.AddrPC.Mode         = AddrModeFlat;
	sf.AddrStack.Offset    = pContext->Esp;
	sf.AddrStack.Mode      = AddrModeFlat;
	sf.AddrFrame.Offset    = pContext->Ebp;
	sf.AddrFrame.Mode      = AddrModeFlat;

	dwMachineType = IMAGE_FILE_MACHINE_I386;
#endif

	while ( 1 )
	{
		// �����һ��ջ֡
		if ( ! StackWalk(  dwMachineType,
			GetCurrentProcess(),
			hThread,
			&sf,
			pContext,
			0,
			SymFunctionTableAccess,
			SymGetModuleBase,
			0 ) )
			break;

		if ( 0 == sf.AddrFrame.Offset ) // ���Frame�Ƿ���ȷ
			break;                      // ���ģ���break

		Dump( _T("\t%08X  %08X  "), sf.AddrPC.Offset, sf.AddrFrame.Offset );

		// ��ô�ջ֡�ĺ�����
		BYTE symbolBuffer[ sizeof(SYMBOL_INFO) + 1024 ];
		PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)symbolBuffer;
		pSymbol->SizeOfStruct = sizeof(symbolBuffer);
		pSymbol->MaxNameLen = 1024;

		DWORD64 symDisplacement = 0;

		if ( SymFromAddr(GetCurrentProcess(),sf.AddrPC.Offset,&symDisplacement,pSymbol))
		{
			char str[256];
			if(UnDecorateSymbolName(pSymbol->Name,str,256,UNDNAME_COMPLETE))
			{
				Dump( _T("%hs+%I64X"), str, symDisplacement );
			}            
			else
			{
				Dump( _T("%hs+%I64X"), pSymbol->Name, symDisplacement );
			}
		}
		else    // û�ҵ����ţ����ӡ��ַ
		{
			TCHAR szModule[MAX_PATH] = _T("");
			DWORD section = 0, offset = 0;

			GetLogicalAddress(  (PVOID)sf.AddrPC.Offset,
				szModule, sizeof(szModule), section, offset );

			Dump( _T("%04X:%08X %s"), section, offset, szModule );
		}

		//���Դ�ļ��к� - Stack Frame Entry����Ӧ��
		IMAGEHLP_LINE lineInfo = { sizeof(IMAGEHLP_LINE) };
		DWORD dwLineDisplacement;
		if ( SymGetLineFromAddr( GetCurrentProcess(), sf.AddrPC.Offset,
			&dwLineDisplacement, &lineInfo ) )
		{
			Dump(_T("  %s line %u"),lineInfo.FileName,lineInfo.LineNumber); 
		}

		//4�����ܲ���
		Dump(_T("(%d,%d,%d,%d)"),sf.Params[0],sf.Params[1],sf.Params[2],sf.Params[3]);

		Dump( _T("\r\n") );

		// ��ӡ�ֲ������Ͳ���
		if (1)
		{
			IMAGEHLP_STACK_FRAME imagehlpStackFrame;
			imagehlpStackFrame.InstructionOffset = sf.AddrPC.Offset;
			SymSetContext( GetCurrentProcess(), &imagehlpStackFrame, 0 );

			SymEnumSymbols( GetCurrentProcess(), 0, 0, EnumerateSymbolsCallback, &sf );

			Dump( _T("\r\n") );
		}
	}
}
//------------------------------------------------------------------------
BOOL CALLBACK CCrashDump::EnumerateSymbolsCallback(
	PSYMBOL_INFO  pSymInfo,
	ULONG         SymbolSize,
	PVOID         UserContext )
{

	char szBuffer[2048];

	__try
	{
		if ( FormatSymbolValue( pSymInfo, (STACKFRAME*)UserContext,
			szBuffer, sizeof(szBuffer) ) )  
			Dump( _T("\t%s\r\n"), szBuffer );
	}
	__except( 1 )
	{
		Dump( _T("punting on symbol %s\r\n"), pSymInfo->Name );
	}

	return TRUE;
}
//------------------------------------------------------------------------
//���ݴ�������Ե�ַaddr����ö�Ӧ��ģ��szModule����Section��ƫ����Offset len��szModule�ĳ���
BOOL CCrashDump::GetLogicalAddress(
								   PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset )
{
	MEMORY_BASIC_INFORMATION mbi;

	HMODULE h = GetModuleHandle(0);

	if ( !VirtualQuery( addr, &mbi, sizeof(mbi) ) )
		return FALSE;

	DWORD hMod = (DWORD)mbi.AllocationBase;
	if(hMod == 0)
		hMod += (DWORD)h;

	if ( !GetModuleFileName( (HMODULE)hMod, szModule, len ) )
		return FALSE;

	//DOS_HEADER
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hMod;

	//NT_HEADER
	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)(hMod + pDosHdr->e_lfanew);

	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION( pNtHdr );

	DWORD rva = (DWORD)addr - hMod; // RVA - �ļ�����ƫ����

	//�����нڣ��ҵ�һ������addr���Ե�ַ��
	for (   unsigned i = 0;
		i < pNtHdr->FileHeader.NumberOfSections;
		i++, pSection++ )
	{
		DWORD sectionStart = pSection->VirtualAddress;
		DWORD sectionEnd = sectionStart
			+ max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

		//address�Ƿ���section��
		if ( (rva >= sectionStart) && (rva <= sectionEnd) )
		{
			//�ǵĻ�����section��offset����
			section = i+1;
			offset = rva - sectionStart;

			return TRUE;
		}
	}

	return FALSE;   // Should never get here!
}
//------------------------------------------------------------------------
//����һ��SYMBOL_INFO�ṹ�壬���������
//�����һ���û��������ͣ����ӡ�����ͺ�ֵ
bool CCrashDump::FormatSymbolValue(
								   PSYMBOL_INFO pSym,
								   STACKFRAME * sf,
								   char * pszBuffer,
								   unsigned cbBuffer )
{
	char * pszCurrBuffer = pszBuffer;

	//variable�Ǿֲ��������ǲ���
	if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_PARAMETER )
		pszCurrBuffer += sprintf( pszCurrBuffer, "Parameter " );
	else if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_LOCAL )
		pszCurrBuffer += sprintf( pszCurrBuffer, "Local " );

	// pSym->Tag == 5��ʾ��һ������
	if ( pSym->Tag == 5 )   // SymTagFunction from CVCONST.H from the DIA SDK
		return false;

	// Emit the variable name
	pszCurrBuffer += sprintf( pszCurrBuffer, "\'%s\'", pSym->Name );

	DWORD_PTR pVariable = 0;    //�˵�ַָ��variable���ڴ�

	if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGRELATIVE )
	{
		// if ( pSym->Register == 8 )   // EBP is the value 8 (in DBGHELP 5.1)
		{                               //  This may change!!!
			pVariable = sf->AddrFrame.Offset;
			pVariable += (DWORD_PTR)pSym->Address;
		}
		// else
		//  return false;
	}
	else if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGISTER )
	{
		return false;   // ���ﲻ��ӡ�Ĵ�������
	}
	else
	{
		pVariable = (DWORD_PTR)pSym->Address;   //ȫ�ֱ���
	}

	//��variable�Ƿ�UDT��bHandled
	bool bHandled;
	pszCurrBuffer = DumpTypeIndex(pszCurrBuffer,pSym->ModBase, pSym->TypeIndex,
		0, pVariable, bHandled );

	if ( !bHandled )
	{
		//������Ǹ�UDT������ݴ�Variable�ĳ��ȣ��²������� - Ŀǰֻ֧��char,WORD,DWORD
		BasicType basicType = GetBasicType( pSym->TypeIndex, pSym->ModBase );

		pszCurrBuffer = FormatOutputValue(pszCurrBuffer, basicType, pSym->Size,
			(PVOID)pVariable ); 
	}
	return true;
}
//------------------------------------------------------------------------
//����UDT���ݹ����ĳ�Ա��ֱ����Ա��������C++�������ͣ���ʱ��bHandledΪfalse
//FormatSymbolValue�������Ը�ʽ������������
char * CCrashDump::DumpTypeIndex(
								 char * pszCurrBuffer,
								 DWORD64 modBase,
								 DWORD dwTypeIndex,
								 unsigned nestingLevel,
								 DWORD_PTR offset,
								 bool & bHandled )
{
	bHandled = false;

	//���symbol������ - ����UDT������������
	//������һ����Ա����
	WCHAR * pwszTypeName;
	if ( SymGetTypeInfo( GetCurrentProcess(), modBase, dwTypeIndex, TI_GET_SYMNAME,
		&pwszTypeName ) )
	{
		pszCurrBuffer += sprintf( pszCurrBuffer, " %ls", pwszTypeName );
		LocalFree( pwszTypeName );
	}

	//�ж��ٸ���Ա
	DWORD dwChildrenCount = 0;
	SymGetTypeInfo( GetCurrentProcess(), modBase, dwTypeIndex, TI_GET_CHILDRENCOUNT,
		&dwChildrenCount );

	if ( !dwChildrenCount )     // If no children, we're done
		return pszCurrBuffer;

	// Prepare to get an array of "TypeIds", representing each of the children.
	// SymGetTypeInfo(TI_FINDCHILDREN) expects more memory than just a
	// TI_FINDCHILDREN_PARAMS struct has.  Use derivation to accomplish this.
	struct FINDCHILDREN : TI_FINDCHILDREN_PARAMS
	{
		ULONG   MoreChildIds[1024];
		FINDCHILDREN(){Count = sizeof(MoreChildIds) / sizeof(MoreChildIds[0]);}
	} children;

	children.Count = dwChildrenCount;
	children.Start= 0;

	// Get the array of TypeIds, one for each child type
	if ( !SymGetTypeInfo( GetCurrentProcess(), modBase, dwTypeIndex, TI_FINDCHILDREN,
		&children ) )
	{
		return pszCurrBuffer;
	}

	// Append a line feed
	pszCurrBuffer += sprintf( pszCurrBuffer, "\r\n" );

	// Iterate through each of the children
	for ( unsigned i = 0; i < dwChildrenCount; i++ )
	{
		// Add appropriate indentation level (since this routine is recursive)
		for ( unsigned j = 0; j <= nestingLevel+1; j++ )
			pszCurrBuffer += sprintf( pszCurrBuffer, "\t" );

		// Recurse for each of the child types
		bool bHandled2;
		pszCurrBuffer = DumpTypeIndex( pszCurrBuffer, modBase,
			children.ChildId[i], nestingLevel+1,
			offset, bHandled2 );

		// If the child wasn't a UDT, format it appropriately
		if ( !bHandled2 )
		{
			// Get the offset of the child member, relative to its parent
			DWORD dwMemberOffset;
			SymGetTypeInfo( GetCurrentProcess(), modBase, children.ChildId[i],
				TI_GET_OFFSET, &dwMemberOffset );

			// Get the real "TypeId" of the child.  We need this for the
			// SymGetTypeInfo( TI_GET_TYPEID ) call below.
			DWORD typeId;
			SymGetTypeInfo( GetCurrentProcess(), modBase, children.ChildId[i],
				TI_GET_TYPEID, &typeId );

			// Get the size of the child member
			ULONG64 length;
			SymGetTypeInfo(GetCurrentProcess(), modBase, typeId, TI_GET_LENGTH,&length);

			// Calculate the address of the member
			DWORD_PTR dwFinalOffset = offset + dwMemberOffset;

			BasicType basicType = GetBasicType(children.ChildId[i], modBase );

			pszCurrBuffer = FormatOutputValue( pszCurrBuffer, basicType,
				length, (PVOID)dwFinalOffset ); 

			pszCurrBuffer += sprintf( pszCurrBuffer, "\r\n" );
		}
	}

	bHandled = true;
	return pszCurrBuffer;
}
//------------------------------------------------------------------------
BasicType CCrashDump::GetBasicType( DWORD typeIndex, DWORD64 modBase )
{
	BasicType basicType;
	if ( SymGetTypeInfo( GetCurrentProcess(), modBase, typeIndex,
		TI_GET_BASETYPE, &basicType ) )
	{
		return basicType;
	}

	// Get the real "TypeId" of the child.  We need this for the
	// SymGetTypeInfo( TI_GET_TYPEID ) call below.
	DWORD typeId;
	if (SymGetTypeInfo(GetCurrentProcess(),modBase, typeIndex, TI_GET_TYPEID, &typeId))
	{
		if ( SymGetTypeInfo( GetCurrentProcess(), modBase, typeId, TI_GET_BASETYPE,
			&basicType ) )
		{
			return basicType;
		}
	}
	return btNoType;
}
//------------------------------------------------------------------------
char * CCrashDump::FormatOutputValue(   char * pszCurrBuffer,
									 BasicType basicType,
									 DWORD64 length,
									 PVOID pAddress )
{
	// Format appropriately (assuming it's a 1, 2, or 4 bytes (!!!)
	if ( length == 1 )
		pszCurrBuffer += sprintf( pszCurrBuffer, " = %X", *(PBYTE)pAddress );
	else if ( length == 2 )
		pszCurrBuffer += sprintf( pszCurrBuffer, " = %X", *(PWORD)pAddress );
	else if ( length == 4 )
	{
		if ( basicType == btFloat )
		{
			pszCurrBuffer += sprintf(pszCurrBuffer," = %f", *(PFLOAT)pAddress);
		}
		else if ( basicType == btChar )
		{
			if ( !IsBadStringPtr( *(PSTR*)pAddress, 32) )
			{
				pszCurrBuffer += sprintf( pszCurrBuffer, " = \"%.31s\"",
					*(PDWORD)pAddress );
			}
			else
				pszCurrBuffer += sprintf( pszCurrBuffer, " = %X",
				*(PDWORD)pAddress );
		}
		else
			pszCurrBuffer += sprintf(pszCurrBuffer," = %X", *(PDWORD)pAddress);
	}
	else if ( length == 8 )
	{
		if ( basicType == btFloat )
		{
			pszCurrBuffer += sprintf( pszCurrBuffer, " = %lf",
				*(double *)pAddress );
		}
		else
			pszCurrBuffer += sprintf( pszCurrBuffer, " = %I64X",
			*(DWORD64*)pAddress );
	}

	return pszCurrBuffer;
}
//------------------------------------------------------------------------
BOOL CCrashDump::AttachCrashDumpSink(ICrashDumpSink *pSink)
{
	if(pSink && !m_pSink)
	{
		m_pSink = pSink;
		return TRUE;
	}

	return FALSE;
}
//------------------------------------------------------------------------
BOOL CCrashDump::DetachCrashDumpSink(ICrashDumpSink *pSink)
{
	if(pSink == m_pSink && m_pSink)
	{
		m_pSink = 0;
		return TRUE;
	}

	return FALSE;
}
//------------------------------------------------------------------------
void CCrashDump::SetDumpOn(int iEnum,BOOL bOn)
{
	if(iEnum >= 0 && iEnum < enDumpCount)
	{
		m_pbOn[iEnum] = bOn;
	}
}
//------------------------------------------------------------------------
void CCrashDump::SetLogFileName(LPCSTR szLogFileName)
{
	if(szLogFileName)
		_tcscpy(m_szLogFileName,szLogFileName);
}
//------------------------------------------------------------------------
void CCrashDump::SetDumpFileName(LPCSTR szDumpFileName)
{
	if(szDumpFileName)
		_tcscpy(m_szDumpFileName,szDumpFileName);
}
//------------------------------------------------------------------------
DWORD CCrashDump::GetExceptionCode(PEXCEPTION_POINTERS pExceptionInfo)
{
	if(pExceptionInfo)
	{
		return pExceptionInfo->ExceptionRecord->ExceptionCode;
	}

	return 0;
}
//------------------------------------------------------------------------
LPCTSTR	CCrashDump::GetExceptionDesc(PEXCEPTION_POINTERS pExceptionInfo)
{
	if(pExceptionInfo)
	{
		return GetExceptionDescription(pExceptionInfo->ExceptionRecord->ExceptionCode);
	}

	return _T("Unknown Exception");
}
//------------------------------------------------------------------------
DWORD CCrashDump::GetCrashFileOffset(PEXCEPTION_POINTERS pExceptionInfo)
{
	if(!pExceptionInfo)return 0;

	PEXCEPTION_RECORD pException = pExceptionInfo->ExceptionRecord;
	PCONTEXT          pContext   = pExceptionInfo->ContextRecord;

	HINSTANCE hCrashMod = GetModuleHandle(GetCrashModuleName(pExceptionInfo));
	long lOffset = pContext->Eip - (long)hCrashMod;

	return lOffset;
}
//------------------------------------------------------------------------
LPCTSTR	CCrashDump::GetCrashModuleName(PEXCEPTION_POINTERS pExceptionInfo)
{
	PEXCEPTION_RECORD pException = pExceptionInfo->ExceptionRecord;
	PCONTEXT          pContext   = pExceptionInfo->ContextRecord;

	static TCHAR szCrashModulePathName[MAX_PATH*2];
	_tcscpy(szCrashModulePathName,_T("Unknown"));

	TCHAR *pszCrashModuleFileName = szCrashModulePathName;

	MEMORY_BASIC_INFORMATION MemInfo;

	// From MSDN:
	// VirtualQuery can be used to get the allocation base associated with a
	// code address, which is the same as the ModuleHandle. This can be used
	// to get the filename of the module that the crash happened in.
	if (VirtualQuery((void*)pContext->Eip, &MemInfo, sizeof(MemInfo)) &&
		(GetModuleFileName((HINSTANCE)MemInfo.AllocationBase,
		szCrashModulePathName,
		sizeof(szCrashModulePathName)-2) > 0))
	{
		pszCrashModuleFileName = GetFilePart(szCrashModulePathName);
	}

	return pszCrashModuleFileName;
}
//------------------------------------------------------------------------
void CCrashDump::DumpLn(LPCTSTR szMsg)
{
	Instance()->Dump(szMsg);
	Instance()->Dump(_T("\r\n"));
}
//------------------------------------------------------------------------
void CCrashDump::GetCrashModuleProductVer(PEXCEPTION_POINTERS pExceptionInfo,WORD pwVer[4])
{
	LPCTSTR pszCrashModuleName = GetCrashModuleName(pExceptionInfo);
	CMiniVersion ver(pszCrashModuleName);
	ver.GetProductVersion(pwVer);
}
//------------------------------------------------------------------------
string CCrashDump::GetCallStack(CONTEXT& Context,HANDLE hThread)
{
	DumpCallStack(&Context,hThread);
	return m_szRpt;
	return _T("Unavailable!");
}
//------------------------------------------------------------------------
void CCrashDump::SetDumpType(long lDumpType)
{
	m_lCrashDumpType = lDumpType;
}
//------------------------------------------------------------------------
extern "C" __declspec(dllexport) ICrashDump* CreateCrashDump()
{
	return CCrashDump::Instance();
}
//------------------------------------------------------------------------