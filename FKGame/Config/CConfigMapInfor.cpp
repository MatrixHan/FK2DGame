/**
*	created:		2012-6-30   23:37
*	filename: 		CConfigMapInfor
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��ͼ��Ϣ����
*/
//------------------------------------------------------------------------
#include "PreInclude.h"
#include "CConfigMapInfor.h"
#include "../GameGlobal/ClientGlobal.h"
//------------------------------------------------------------------------
BOOL CConfigMapInfor::Create(void)
{	
	IScriptUpdateManager * pScriptUpdateManager = g_ClientGlobal.m_pScriptUpdateManager;
	if(pScriptUpdateManager == NULL)
	{
		return FALSE;
	}
	
	if(!pScriptUpdateManager->LoadScript(MAPINFOR_CONFIG_SCRIPTNAME, static_cast<IScriptUpdateSink *>(this)))
	{
		TraceLn("����%s�ű�ʧ�ܣ�", MAPINFOR_CONFIG_SCRIPTNAME);		
		return FALSE;
	}
	
	return TRUE;	
}
//------------------------------------------------------------------------
// ȡ�����е�ͼ����Ϣ, IN : pInfor �����ɵ�MAP_INFOR����, OUT : ��ǰ�ж����ŵ�ͼ
BOOL CConfigMapInfor::GetMapInfor(MAP_INFOR * pInfor, DWORD &dwCount)
{
	if(pInfor == NULL || dwCount <= 0)
	{
		return FALSE;
	}

	int nMaxCount = dwCount;
	dwCount = 0;
	TVECTOR_MAPINFOR::iterator it = m_vectorMapInfor.begin();
	for( ; it != m_vectorMapInfor.end(); ++it)
	{
		pInfor[dwCount++] = (*it);
		if(dwCount >= nMaxCount)
		{
			return FALSE;
		}
	}

	return TRUE;
}
//------------------------------------------------------------------------
// ȡ��ĳһ�ŵ�ͼ����Ϣ
MAP_INFOR *	CConfigMapInfor::GetMapInfor(DWORD dwMapID)
{
	TVECTOR_MAPINFOR::iterator it = m_vectorMapInfor.begin();
	for( ; it != m_vectorMapInfor.end(); ++it)
	{
		if((*it).nmapid == dwMapID)
		{
			return (&(*it));
		}
	}

	return NULL;
}
//------------------------------------------------------------------------
// ȡ�õ�ǰ��ͼ����
DWORD CConfigMapInfor::GetMapCount(void)
{
	return m_vectorMapInfor.size();
}
//------------------------------------------------------------------------
// �ű�����ɹ�ʱ֪ͨ,ע��it�п���Ϊ��
BOOL CConfigMapInfor::OnScriptLoad(IScriptIterator * it, IResScript * pResScript, LPCSTR szFile)
{
	if(pResScript == NULL)
	{
		return FALSE;
	}

	IResObject * pRootObject = pResScript->GetResObject(1);
	if(pRootObject == NULL)
	{	
		return FALSE;
	}
	
	int nCount = pRootObject->GetSubObjectCount();	
	for(int i = 0; i < nCount; i++)
	{
		IResObject * pObject = pRootObject->GetSubObjectByIndex(i);
		if(pObject == NULL)
		{
			continue;
		}

		MAP_INFOR mapinfor;
		ZeroMemory(&mapinfor, sizeof(MAP_INFOR));
		pObject->GetPropertyLong("applyserver", mapinfor.napplyserver, 1);
		pObject->GetPropertyLong("open", mapinfor.nopen, 1);
		pObject->GetPropertyLong("mapid", mapinfor.nmapid, INVALID_MAPID);
		pObject->GetPropertyLong("usedynamic", mapinfor.lusedynamic, 1);
		pObject->GetPropertyLong("minimaprate", mapinfor.lminimaprate, 20);

		lstrcpyn(mapinfor.szmapname, pObject->GetPropertyString("mapname"), sizeof(mapinfor.szmapname));
		lstrcpyn(mapinfor.szmapfile, pObject->GetPropertyString("mapfile"), sizeof(mapinfor.szmapfile));
		lstrcpyn(mapinfor.szmaifile, pObject->GetPropertyString("maifile"), sizeof(mapinfor.szmaifile));
		lstrcpyn(mapinfor.szminimapfile, pObject->GetPropertyString("minimapfile"), sizeof(mapinfor.szminimapfile));
		lstrcpyn(mapinfor.szworldmapfile, pObject->GetPropertyString("worldmapfile"), sizeof(mapinfor.szworldmapfile));

		pObject->GetArrayLong("staticrelivepoint", mapinfor.lstaticrelivepoint, 3);
		pObject->GetArrayLong("dynamicrelivepoint", mapinfor.ldynamicrelivepoint, 3);
		pObject->GetArrayLong("redstaticrelivepoint", mapinfor.lredstaticrelivepoint, 3);
		pObject->GetArrayLong("reddynamicrelivepoint", mapinfor.lreddynamicrelivepoint, 3);
		pObject->GetArrayLong("landfallpoint", mapinfor.llandfallpoint, 2);

		lstrcpyn(mapinfor.szmonstercreaterfile, pObject->GetPropertyString("monstercreater"), sizeof(mapinfor.szmonstercreaterfile));
		lstrcpyn(mapinfor.szchunnelcreaterfile, pObject->GetPropertyString("chunnelcreater"), sizeof(mapinfor.szchunnelcreaterfile));
		lstrcpyn(mapinfor.szboxcreaterfile,		pObject->GetPropertyString("boxcreater"),	  sizeof(mapinfor.szboxcreaterfile));

		pObject->GetPropertyLong("backgroundmusic", mapinfor.lbackgroundmusic, 0);

		// ��ͼ��С
		long lMapSize[2];
		pObject->GetArrayLong("mapsizetile", lMapSize, 2);
		mapinfor.lmapsizetileX = lMapSize[0];
		mapinfor.lmapsizetileY = lMapSize[1];

		m_vectorMapInfor.push_back(mapinfor);
	}	
	
	return TRUE;
}
//------------------------------------------------------------------------
// �ű���̬����ʱ֪ͨ,ע��it�п���Ϊ��
BOOL CConfigMapInfor::OnScriptUpdate(IScriptIterator * it, IResScript * pResScript, LPCSTR szFile)
{
	return FALSE;	
}
//------------------------------------------------------------------------