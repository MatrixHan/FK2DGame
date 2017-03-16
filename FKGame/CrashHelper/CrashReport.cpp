/**
*	created:		2012-6-21   0:06
*	filename: 		CrashReport
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "PreInclude.h"
#include "CrashReport.h"
#include <string>
//------------------------------------------------------------------------
// ��  ������ʼdump��Ϣ������������----ʵ��������������ô��󱨸�����
void CCrashReport::OnDumpStart(ICrashDump* pDump)
{
	if(!pDump)return;


	//char strTemp[256];
	//char strTemp2[256];
	//memset( strTemp, 0, sizeof(strTemp) );
	//memset( strTemp2, 0, sizeof(strTemp2) );
	//SYSTEMTIME st;
	//GetLocalTime(&st);
	//sprintf(strTemp,"%d/%d/%d_%d:%d:%d_%s",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond, std::string(XCRASHREPORT_ERROR_LOG_FILE).c_str());
	//sprintf(strTemp2,"%d/%d/%d_%d:%d:%d_%s",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond, std::string(XCRASHREPORT_MINI_DUMP_FILE).c_str());

	pDump->SetLogFileName(XCRASHREPORT_ERROR_LOG_FILE);
	pDump->SetDumpFileName(XCRASHREPORT_MINI_DUMP_FILE);
}
//------------------------------------------------------------------------
// ��  ����������ICrashDump��DumpLn Dump�������LogFile����Ϣ
void CCrashReport::OnDump(ICrashDump* pDump,PEXCEPTION_POINTERS pExceptionInfo)
{
	if(!g_ClientGlobal.m_pGameApp)return;
	TCHAR pBuffer[256];
	g_ClientGlobal.m_pGameApp->OnCommand(IDM_GET_SERVER_INFO,(long)pBuffer,0);
	pDump->DumpLn(pBuffer);

	if(g_ClientGlobal.m_pPlayer)
	{
		pDump->DumpLn(g_ClientGlobal.m_pPlayer->GetName());
	}
}
//------------------------------------------------------------------------
// ��  ����Dump��������ô˺������˺�������ֵ�ᱻ�����쳣����ķ���ֵ
LONG CCrashReport::OnDumpFinish(ICrashDump* pDump,PEXCEPTION_POINTERS pExceptionInfo)
{
	TCHAR pszFilePart[MAX_PATH];
	lstrcpy(pszFilePart, XCRASHREPORT_CRASH_REPORT_APP);
	
	TCHAR szModuleName[MAX_PATH];
	lstrcpy(szModuleName,pDump->GetCrashModuleName(pExceptionInfo));

	TCHAR szCommandLine[MAX_PATH];
	lstrcpy(szCommandLine,pszFilePart);
	lstrcat(szCommandLine," ");
	
	lstrcat(szCommandLine, " \"");	// surround app name with quotes
	ZeroMemory(szModuleName, sizeof(szModuleName));
	GetModuleFileName(0, szModuleName, _countof(szModuleName)-2);

	char *pModuleName = strrchr(szModuleName,'\\');
	if(pModuleName)
	{
		pModuleName++;
		lstrcat(szCommandLine,pModuleName);
	}
	else
	{
		lstrcat(szCommandLine,"Game.exe");
	}
	lstrcat(szCommandLine, "\"");
	
	//ȡ�ð汾��
	char strVer[256] = {0};
	GetPrivateProfileString("OPTION","Help","",strVer,sizeof(strVer) - 1,".\\update.ini");

	//ȡ�ñ���ģ��Ĳ�Ʒ�汾��
	WORD pwVer[4];
	pDump->GetCrashModuleProductVer(pExceptionInfo,pwVer);

	//���汾�ŵĵ�3λ�ӵ�����
	TCHAR szSubject[256];
	LPSTR szCrashModuleName = (LPSTR)pDump->GetCrashModuleName(pExceptionInfo);

	//����Щ�ĳ�Сд
	for(unsigned int i = 0;i < strlen(szCrashModuleName);i++)
	{
		if(szCrashModuleName[i] >= 'A' && szCrashModuleName[i] <= 'Z')
			szCrashModuleName[i] -= ('A' - 'a');
	}
	strcpy(szSubject,szCrashModuleName);
	sprintf(szSubject,"%s %04d %08x",/*pDump->GetExceptionDesc(pExceptionInfo),*/szSubject,pwVer[2],pDump->GetCrashFileOffset(pExceptionInfo));

	if(g_ClientGlobal.m_pGameApp)
	{
		//��������
		TCHAR pBuffer[256];
		g_ClientGlobal.m_pGameApp->OnCommand(IDM_GET_SERVER_INFO,(long)pBuffer,0);
		sprintf(szSubject,"%s %s",szSubject,pBuffer);
	}

	//��ҽ�ɫ��
	if(g_ClientGlobal.m_pPlayer)
	{
		sprintf(szSubject,"%s %s",szSubject,g_ClientGlobal.m_pPlayer->GetName());
	}

	lstrcat(szCommandLine, " \"");	// surround app name with quotes
	lstrcat(szCommandLine,szSubject);
	lstrcat(szCommandLine, "\"");

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	
	if (CreateProcess(
		NULL,					// name of executable module
		szCommandLine,			// command line string
		NULL,					// process attributes
		NULL,					// thread attributes
		FALSE,					// handle inheritance option
		0,						// creation flags
		NULL,					// new environment block
		NULL,					// current directory name
		&si,					// startup information
		&pi))					// process information
	{
		// XCrashReport.exe was successfully started, so
		// suppress the standard crash dialog
		return EXCEPTION_EXECUTE_HANDLER;
		/*
		//CopyFile("Trace.txt",XCRASHREPORT_TRACE_FILE,FALSE);
		WaitForSingleObject(pi.hProcess,INFINITE);
		//DeleteFile(XCRASHREPORT_TRACE_FILE);
		DWORD dwRetCode = 0;
		if(GetExitCodeProcess(pi.hProcess,&dwRetCode))
		{
		if((long)dwRetCode == -1)
		{
		return EXCEPTION_CONTINUE_SEARCH;
		}
		}
		return EXCEPTION_EXECUTE_HANDLER;//*/
	}
	else
	{
		// XCrashReport.exe was not started - let
		// the standard crash dialog appear
		return EXCEPTION_CONTINUE_SEARCH;
	}
}