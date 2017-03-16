/**
*	created:		2012-6-30   23:57
*	filename: 		ScriptUpdate
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ű�����ģ��ӿ�ʵ��ͷ�ļ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <windows.h>
#include <list>
#include <map>
#include "../../FKGame/Script/IScriptUpdate.h"
#include "../../FKTrace/IFKTrace.h"
#include "../../FKResManager/IFKResManager.h"
//------------------------------------------------------------------------
using std::list;
using std::map;
//------------------------------------------------------------------------
extern CFKTrace				g_DefaultTrace;
#define g_TraceHelper		g_DefaultTrace	
#ifndef TraceLn
	#ifdef _DEBUG
		#define TraceLn g_DefaultTrace.ThreadSafeDebugPrintLn
		#define TraceColorLn g_DefaultTrace.DebugPrintColorLn
	#else
		#define TraceLn __noop
		#define TraceColorLn __noop
	#endif
#endif
//------------------------------------------------------------------------
extern IResMgr*             g_pResMgr;
//------------------------------------------------------------------------
extern _VARIANT             empty_variant;
//------------------------------------------------------------------------
#define SAFE_RELEASE(p) \
	if ((p) != NULL) \
{ \
	(p)->Release(); \
}
//------------------------------------------------------------------------
struct SCRIPT_INFO
{
	char	szFileName[ MAX_PATH ];		
	list< IScriptUpdateSink * >	sinkList;	// һ���ű�����֧�ֶ���ص�����	
};
//------------------------------------------------------------------------
/*
 *	�ű����¹���������ʵ����
 */
class ScriptUpdateManager : public IScriptUpdateManager
{
public:
	// ��  ������ȡ�ű������Զ�ע��
	// ��  ����[����] LPCSTR szFile - �ű��ļ���
	// ��  ����[����] IScriptUpdateSink *pSink - ����ű��Ļص��ӿ�
	// ����ֵ��BOOL - �ɹ�����TRUE������ΪFALSE
	virtual BOOL LoadScript(LPCSTR szFile, IScriptUpdateSink *pSink);
	
	// ��  �������½ű�
	// ��  ����[����] LPCSTR szFile - ���½ű��ļ���
	// ����ֵ��BOOL - �ɹ�����TRUE������ΪFALSE
	virtual BOOL UpdateScript(LPCSTR szFile);	
	
	// ��  ����ע��ű��ص��ӿ�
	// ��  ����[����] LPCSTR szFile - ���½ű��ļ���
	// ��  ����[����] IScriptUpdateSink * pSink - �ű����»ص��ӿ�
	// ����ֵ����
	virtual void RegisterSink(LPCSTR szFile, IScriptUpdateSink *pSink);
	
	// ��  ����ȡ��ע��ű��ص��ӿ�
	// ��  ����[����] LPCSTR szFile - ���½ű��ļ���
	// ��  ����[����] IScriptUpdateSink * pSink - �ű����»ص��ӿ�
	// ����ֵ����
	virtual void UnRegisterSink(LPCSTR szFile, IScriptUpdateSink *pSink);
	
	// �ͷŹ�����
	virtual void Release();
public:
	virtual ~ScriptUpdateManager();
	static ScriptUpdateManager* GetInstance();
private:
	IScriptIterator* GetIteratorFromCSV(LPCSTR szFileName);
	IScriptIterator* GetIteratorFromCSE(LPCSTR szFileName);
	BOOL Load(LPCSTR szFileName, IScriptIterator **ppScriptIterator, IResScript **ppResScript);
private:	
	ScriptUpdateManager();	
	// ��  ������þ���·��
	// ��  ����[����] LPCSTR szFile - ���½ű��ļ���
	// ��  ����[����] char *szPath  - Buf
	// ��  ����[����] int nSize		- Buf��С
	// ����ֵ��Buf
	char* GetAbsolutePath(LPCSTR szFile, char *szPath, int nSize);
private:
	static ScriptUpdateManager* s_pInstance;
	static int s_nRefCount;
	typedef map< int , SCRIPT_INFO >	SCRIPT_TABLE;
	
	SCRIPT_TABLE	  m_ScriptTable;
	int m_nCallNum;
};
//------------------------------------------------------------------------




