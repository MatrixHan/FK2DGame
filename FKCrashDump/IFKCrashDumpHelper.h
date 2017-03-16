/**
*	created:		2012-6-14   18:12
*	filename: 		IFKCrashDumpHelper
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
/*
	ʹ��˵����
	1�� Main�����ļ��а������ļ�ͷ
	2�� ע���������ļ�ͬ��Ŀ¼�·��ö�Ӧ�Ķ�̬���ӿ⡣
		DEBUG�汾����	FKCrashDump_D.dll 
		RELEASE�汾����	FKCrashDump.dll
	3�� ��Mainǰ����ȫ�ֱ��� CCrashDumpHelper g_CrashDumpHelper;
	��ɡ�

	ע�⣺
	1��	��ǰ�汾��δ֧��Unicode�ֽڡ��뽫Ӧ�ó����Ϊ���ֽڡ�
		���в��㣬����ϵFreeKnight  
	http://freeknigt.jimdo.com
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
#include <string>
//------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------
struct ICrashDump;
//------------------------------------------------------------------------
enum ENUM_DumpInfoState
{
	enDumpLog,				//�Ƿ���Log------�������ΪFALSE����һ�¼������MiniDump����Ч
	enDumpBasicInfo,		//������Ϣ
	enDumpModuleList,		//ģ���б�
	enDumpWindowList,		//�����б�
	enDumpProcessList,		//�����б�
	enDumpRegister,			//�Ĵ���
	enDumpCallStack,		//����ջ
	enDumpMiniDump,			//���dmp�ļ�
	enDumpCount
};
//------------------------------------------------------------------------
struct ICrashDumpSink
{
	// ��ʼdump��Ϣ������������----ʵ��������������ô��󱨸�����
	virtual void OnDumpStart(ICrashDump*) = 0;
	// ������ICrashDump��DumpLn Dump�������LogFile����Ϣ
	virtual void OnDump(ICrashDump*,PEXCEPTION_POINTERS) = 0;
	// Dump��������ô˺������˺�������ֵ�ᱻ�����쳣����ķ���ֵ
	virtual LONG OnDumpFinish(ICrashDump*,PEXCEPTION_POINTERS) = 0;
};
//------------------------------------------------------------------------
struct ICrashDump
{
	// �ҽ�/ж�� һ��ICrashDumpSink�Ļص�
	virtual BOOL AttachCrashDumpSink(ICrashDumpSink *) = 0;
	virtual BOOL DetachCrashDumpSink(ICrashDumpSink *) = 0;
	// ����Log�ļ���
	virtual void SetLogFileName(LPCSTR szLogFileName) = 0;
	// ����Dmp�ļ���
	virtual void SetDumpFileName(LPCSTR szDumpFileName) = 0;
	// ����Dump��Ϣ���أ���Ӧ ENUM_DumpInfoState ��
	virtual void SetDumpOn(int iEnum,BOOL bOn) = 0;

	// ����Dump���ͣ�lDumpType����Ϊһ���⼸������
	/*
		ע��:MiniDumpWithIndirectlyReferencedMemory��win98�²�����

		MiniDumpNormal                         = 0x0000,
		MiniDumpWithDataSegs                   = 0x0001,
		MiniDumpWithFullMemory                 = 0x0002,
		MiniDumpWithHandleData                 = 0x0004,
		MiniDumpFilterMemory                   = 0x0008,
		MiniDumpScanMemory                     = 0x0010,
		MiniDumpWithUnloadedModules            = 0x0020,
		MiniDumpWithIndirectlyReferencedMemory = 0x0040,
		MiniDumpFilterModulePaths              = 0x0080,
		MiniDumpWithProcessThreadData          = 0x0100,
		MiniDumpWithPrivateReadWriteMemory     = 0x0200,
		MiniDumpWithoutOptionalData            = 0x0400,
	*/
	virtual void SetDumpType(long lDumpType) = 0;
	// �õ��쳣��
	virtual DWORD GetExceptionCode(PEXCEPTION_POINTERS pExceptionInfo) = 0;
	// �õ��쳣����
	virtual LPCTSTR	GetExceptionDesc(PEXCEPTION_POINTERS pExceptionInfo) = 0;
	// �õ�������ģ���ļ�ƫ��
	virtual DWORD	GetCrashFileOffset(PEXCEPTION_POINTERS pExceptionInfo) = 0;
	// �õ�������ģ���ļ���
	virtual LPCTSTR	GetCrashModuleName(PEXCEPTION_POINTERS pExceptionInfo) = 0;
	// ���һЩDump��Ϣ��Log�ļ�
	virtual void DumpLn(LPCTSTR szMsg) = 0;
	// ȡ�ñ���ģ��Ĳ�Ʒ�汾��
	virtual void GetCrashModuleProductVer(PEXCEPTION_POINTERS pExceptionInfo,WORD pwVer[4]) = 0;
	// ȡ�õ�ǰ����ջ
	virtual string GetCallStack(CONTEXT& Context,HANDLE hThread) = 0;
};
//------------------------------------------------------------------------
class CCrashDumpHelper
{
public:
	CCrashDumpHelper()
	{
		m_pCrashDump = 0;
		m_hDll = 0;

		Create();
	}

	~CCrashDumpHelper()
	{
		Close();
	}
	ICrashDump *operator->()
	{
		return m_pCrashDump;
	}
	void Close()
	{
		if (m_hDll)
		{
			FreeLibrary(m_hDll);
			m_hDll = NULL;
		}
	}
	BOOL IsValid()
	{
		return m_pCrashDump != NULL;
	}
	BOOL Create()
	{
		if(!m_hDll)
			m_hDll = LoadDLL();
		
		if(!m_hDll)return FALSE;
		
		typedef ICrashDump* (*ProcCreateCrashDump)();
		
		ProcCreateCrashDump proc;
		proc = (ProcCreateCrashDump)GetProcAddress(m_hDll, "CreateCrashDump");
		
		if(proc == NULL)return FALSE;
		
		m_pCrashDump = proc();
		
		return m_pCrashDump != NULL;
	}
	
	HINSTANCE LoadDLL()
	{
#ifdef _DEBUG
		return LoadLibrary("FKCrashDump_D.dll");
#else
		return LoadLibrary("FKCrashDump.dll");
#endif
	}
private:
	ICrashDump *m_pCrashDump;
	HINSTANCE	m_hDll;
};
//------------------------------------------------------------------------