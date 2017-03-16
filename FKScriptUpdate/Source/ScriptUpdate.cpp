/**
*	created:		2012-6-30   23:59
*	filename: 		ScriptUpdate
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/ScriptUpdate.h"
#include "../Include/ScriptAdapter_csv.h"
#include "../Include/ScriptAdapter_cse.h"
#include "../Include/HashFunction.h"
//------------------------------------------------------------------------
ScriptUpdateManager*		ScriptUpdateManager::s_pInstance=NULL;
int							ScriptUpdateManager::s_nRefCount=0;
CFKTrace					g_DefaultTrace;	
IResMgr*                    g_pResMgr = NULL;
_VARIANT                    empty_variant = { _VARIANT::enType_EMPTY, 0 };
//------------------------------------------------------------------------
ScriptUpdateManager::ScriptUpdateManager()
{
	m_nCallNum = 0;
}
//------------------------------------------------------------------------
ScriptUpdateManager::~ScriptUpdateManager()
{

}
//------------------------------------------------------------------------
//�ӿں���
// ��  ������ȡ�ű������Զ�ע��
// ��  ����[����] LPCSTR szFile - �ű��ļ���
// ��  ����[����] IScriptUpdateSink *pSink - ����ű��Ļص��ӿ�
// ����ֵ��BOOL - �ɹ�����TRUE������ΪFALSE
BOOL ScriptUpdateManager::LoadScript(LPCSTR szFile, IScriptUpdateSink *pSink)
{
	// ע��Ϊ�����Ч�ʺͱ���̫���鷳�����ﲻ�ܱ�Ƕ�׵��ã�Ҳ�����ڻص�����װ��
	// �ű������Ҫ������ο��¼�ϵͳ
	if (m_nCallNum  > 0)
	{
		memcpy(NULL, NULL, -1);
	}

	if (g_pResMgr == NULL || szFile == NULL || pSink == NULL)
	{
		return FALSE;
	}

	//ע��
	RegisterSink(szFile, pSink);	

	// ת���ɾ���·��
	char szPath[MAX_PATH] = {0};
	GetAbsolutePath(szFile, szPath, MAX_PATH);

	unsigned int unHashKey = SDBMHash( szPath );
	SCRIPT_TABLE::iterator it = m_ScriptTable.find( unHashKey );
	if (it == m_ScriptTable.end())
	{
		return FALSE;
	}

	IScriptIterator *pSriptIterator = NULL;
	IResScript *pResSript = NULL;
	if (!Load(szFile, &pSriptIterator, &pResSript))
	{
		return FALSE;
	}

	m_nCallNum ++;

	//֪ͨ�ű�LOAD
	SCRIPT_INFO &info = (*it).second;
	BOOL bSucceed = TRUE;
	list< IScriptUpdateSink* >::iterator itList = info.sinkList.begin();
	for(; itList != info.sinkList.end(); ++itList)
	{
		IScriptUpdateSink *pSink = *itList;
		if (pSink != NULL)
		{
			if (!pSink->OnScriptLoad(pSriptIterator, pResSript, szFile))
			{				
				bSucceed = FALSE;
				TraceLn("�ű��ļ�%s����ʧ��", szPath);
				break;
			}			
		}
	}	

	if (pResSript != NULL)
	{
		pResSript->Release();
		pResSript = NULL;
	}

	if (pSriptIterator != NULL)
	{
		pSriptIterator->Release();
		pSriptIterator = NULL;
	}	

	m_nCallNum --;

	return bSucceed;
}
//------------------------------------------------------------------------
// ��  �������½ű�
// ��  ����[����] LPCSTR szFile - ���½ű��ļ���
// ����ֵ��BOOL - �ɹ�����TRUE������ΪFALSE
BOOL ScriptUpdateManager::UpdateScript(LPCSTR szFile) 
{
	if (szFile == NULL)
	{
		return FALSE;
	}

	// ת���ɾ���·��
	char szPath[MAX_PATH] = {0};
	GetAbsolutePath(szFile, szPath, MAX_PATH);

	unsigned int unHashKey = SDBMHash( szPath );
	SCRIPT_TABLE::iterator it = m_ScriptTable.find(unHashKey);
	if (it == m_ScriptTable.end())
	{
		return FALSE;
	}

	IScriptIterator *pSriptIterator = NULL;
	IResScript *pResSript = NULL;
	if (!Load(szFile, &pSriptIterator, &pResSript))
	{
		return FALSE;
	}

	SCRIPT_INFO &info = (*it).second;	
	// ��ʼ���½ű�
	BOOL  bSucceed = TRUE;
	list< IScriptUpdateSink* >::iterator itList = info.sinkList.begin();
	for(; itList != info.sinkList.end(); ++itList)
	{
		IScriptUpdateSink *pSink = *itList;
		if (pSink != NULL)
		{
			if (!pSink->OnScriptUpdate(pSriptIterator, pResSript, szFile))
			{				
				bSucceed = FALSE;
				TraceLn("�ű��ļ�%s����ʧ��", szPath);
				break;
			}			
		}
	}	
	if (pResSript != NULL)
	{
		pResSript->Release();
		pResSript = NULL;
	}

	if (pSriptIterator != NULL)
	{
		pSriptIterator->Release();
		pSriptIterator = NULL;
	}	

	return bSucceed;
}
//------------------------------------------------------------------------
// ��  ����ע��ű��ص��ӿ�
// ��  ����[����] LPCSTR szFile - ���½ű��ļ���
// ��  ����[����] IScriptUpdateSink * pSink - �ű����»ص��ӿ�
// ����ֵ����
void ScriptUpdateManager::RegisterSink(LPCSTR szFile, IScriptUpdateSink* pSink)
{
	// ת���ɾ���·��
	char szPath[MAX_PATH]={0};
	GetAbsolutePath(szFile, szPath, MAX_PATH);

	unsigned int unHashKey = SDBMHash( szPath );
	SCRIPT_TABLE::iterator it = m_ScriptTable.find(unHashKey);
	if (it != m_ScriptTable.end())
	{
		//�Ѿ����ڸ��ļ�
		SCRIPT_INFO &info = (*it).second;
		info.sinkList.push_back(pSink);//����µĽ�����
		info.sinkList.unique();//����Ψһ		
	}
	else
	{
		//�����ڸ��ļ���������µ�
		SCRIPT_INFO info;
		lstrcpyn(info.szFileName, szPath, MAX_PATH);		
		info.sinkList.push_back(pSink);
		m_ScriptTable.insert( std::make_pair( unHashKey, info ) );
	}	
}
//------------------------------------------------------------------------
// ��  ����ȡ��ע��ű��ص��ӿ�
// ��  ����[����] LPCSTR szFile - ���½ű��ļ���
// ��  ����[����] IScriptUpdateSink * pSink - �ű����»ص��ӿ�
// ����ֵ����
void ScriptUpdateManager::UnRegisterSink(LPCSTR szFile, IScriptUpdateSink * pSink)
{
	// ת���ɾ���·��
	char szPath[MAX_PATH] = {0};	
	GetAbsolutePath(szFile, szPath, MAX_PATH);

	unsigned int unHashKey = SDBMHash( szPath );
	SCRIPT_TABLE::iterator it = m_ScriptTable.find(unHashKey);
	if (it != m_ScriptTable.end())
	{
		SCRIPT_INFO & info = (*it).second;
		info.sinkList.remove(pSink);
	}
}
//------------------------------------------------------------------------
// �ͷŹ�����
void ScriptUpdateManager::Release() 
{
	s_nRefCount--;
	if (s_nRefCount <= 0)
	{
		m_ScriptTable.empty();
		delete this;
	}
}
//------------------------------------------------------------------------
ScriptUpdateManager* ScriptUpdateManager::GetInstance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new ScriptUpdateManager;
	}

	s_nRefCount++;
	return s_pInstance;
}
//------------------------------------------------------------------------
IScriptIterator* ScriptUpdateManager::GetIteratorFromCSV(LPCSTR szFileName)
{	
	CScriptAdapter_csv *pScriptAdapter = new CScriptAdapter_csv();
	if(!pScriptAdapter->LoadScript(szFileName))
	{
		return NULL;
	}
	return pScriptAdapter->GetScriptIterator();	
}
//------------------------------------------------------------------------
IScriptIterator* ScriptUpdateManager::GetIteratorFromCSE(LPCSTR szFileName)
{	
	CScriptAdapter_cse* pScriptAdapter = new CScriptAdapter_cse();
	if(!pScriptAdapter->LoadScript(szFileName))
	{
		return NULL;
	}

	return pScriptAdapter->GetScriptIterator();	
}
//------------------------------------------------------------------------
BOOL ScriptUpdateManager::Load(LPCSTR szFileName, 
							   IScriptIterator **ppScriptIterator, IResScript **ppResScript)
{
	int nLen = strlen(szFileName);
	if( nLen < 3 )
	{
		return FALSE;
	}

	LPCSTR pExternName = szFileName + nLen - 3;
	if(stricmp( pExternName , "SCP" ) == 0)
	{		
		*ppResScript = g_pResMgr->LoadResScript(szFileName);
		return (*ppResScript != NULL) ? TRUE : FALSE;
	}
	else if (stricmp( pExternName , "RES" ) == 0)
	{
		BYTE Key[] = "abcd";
		*ppResScript = g_pResMgr->LoadResScript(szFileName, FALSE, Key);
		return (*ppResScript != NULL) ? TRUE : FALSE;
	}
	else if (stricmp( pExternName , "CSV" ) == 0)
	{		
		*ppScriptIterator = GetIteratorFromCSV(szFileName);
		return (*ppScriptIterator != NULL) ? TRUE : FALSE;
	}
	else if (stricmp(pExternName, "CSE") == 0)
	{
		// add by wangzq
		*ppScriptIterator = GetIteratorFromCSE(szFileName);
		return (*ppScriptIterator != NULL) ? TRUE : FALSE;
	}
	return FALSE;
}
//------------------------------------------------------------------------
// ת���ɾ���·��	
char* ScriptUpdateManager::GetAbsolutePath(LPCSTR szFile, char *szPath, int nSize)
{
	if (szFile[1] != ':')
	{
		//���·��
		GetCurrentDirectory(nSize, szPath);
		strcat(szPath, "\\");
		strcat(szPath, szFile);		
	}
	else
	{
		//����·��
		lstrcpyn(szPath, szFile, nSize);
	}

	strupr(szPath);//�����е�Сд��ĸת���ɴ�д

	return szPath;
}
//------------------------------------------------------------------------
extern "C" __declspec(dllexport) BOOL CreateScriptUpdateManager(
	IScriptUpdateManager **ppManager, IResMgr *pResMgr, ITrace *pTrace)
{	
	if (ppManager == NULL || pResMgr == NULL)
	{
		return FALSE;
	}

	g_pResMgr = pResMgr;

	if (!g_DefaultTrace.Create(pTrace))
	{
		return FALSE;
	}	

	*ppManager = ScriptUpdateManager::GetInstance();
	return *ppManager != NULL;
}
//------------------------------------------------------------------------