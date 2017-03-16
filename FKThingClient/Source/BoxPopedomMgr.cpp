/**
*	created:		2012-7-3   0:10
*	filename: 		BoxPopedomMgr
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		boxȨ�޹������й���
*/
//------------------------------------------------------------------------
#include "../Include/BoxPopedomMgr.h"
#include "../Include/CThingClientFacade.h"
#include "../../FKGame/SceneObj/IGoods.h"
#include "../../FKGame/SceneObj/IActor.h"
#include "../Include/ThingClientCommon.h"
#include "../Include/IBox.h"
#include <algorithm>
//------------------------------------------------------------------------
using std::find;
//------------------------------------------------------------------------
CBoxPopedomManager g_BoxPopedomManager;
//------------------------------------------------------------------------
CBoxPopedomManager::CBoxPopedomManager(void) : m_bInit(FALSE)
{

}
//------------------------------------------------------------------------
CBoxPopedomManager::~CBoxPopedomManager(void)
{
	try
	{
		m_mapAllBox.clear();
		m_uidVec.clear();
		Term();
	}
	catch (...) {
	}
}
//------------------------------------------------------------------------
BOOL CBoxPopedomManager::Init()
{
	if (m_bInit)
	{
		return TRUE;
	}
	
	IEventServer* pEventServer = g_pThingClientFacade->GetEventServer();
	if (pEventServer == NULL)
	{
		return FALSE;
	}
	
	// ������ҵĳ�����Ϣ���Խ�����������֮ǰ������
	// �������û�а취�������ӵ����ǵ�����
	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_BORN);
	m_bInit = pEventServer->AddListener(static_cast<IEventActionListener*>(this), wMsgID, SRC_TYPE_ID_ACTOR, 0, "CBoxPopedomManager::Init");
	return m_bInit;
}
//------------------------------------------------------------------------
void CBoxPopedomManager::Term()
{
	if (!m_bInit)
	{
		return;
	}
	
	IEventServer* pEventServer = g_pThingClientFacade->GetEventServer();
	if (pEventServer == NULL)
	{
		return;
	}

	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_BORN);
	pEventServer->RemoveListener(static_cast<IEventActionListener*>(this), wMsgID, SRC_TYPE_ID_ACTOR);
	m_bInit = FALSE;
}
//------------------------------------------------------------------------
void CBoxPopedomManager::OnAction(IEvent* pEvent)
{
	if (pEvent == NULL)
	{
		return;
	}

	IActor* pActor = (IActor*)pEvent->GetSrcPointer();
	if (pActor == NULL
		|| pActor->GetThingClass()->GetThingClass() != THINGCLASS_ACTOR)
	{
		return;
	}

	// �����ӵ�Ȩ��
	long dbid = pActor->GetActorDBID();
	UID uidBox;
	IBox* pBox;
	BOOL bRight;

	for (UID_VEC::iterator it = m_uidVec.begin(); it != m_uidVec.end(); ++it)
	{
		uidBox = *it;
		pBox = (IBox*)g_pThingClientFacade->GetThing(uidBox);
		if (pBox == NULL)
		{
			continue;
		}

		bRight = CanOpen(uidBox, dbid);
		pBox->SetRight(bRight);
	}

	// ������󣬶���������
	m_uidVec.clear();
}
//------------------------------------------------------------------------
void CBoxPopedomManager::AddMaster(UID uidBox, long lMasterDBID)
{
	TMAP_ALLBOX::iterator map_it = m_mapAllBox.find(uidBox);
	if (map_it == m_mapAllBox.end())
	{
		TVECTOR_BOXMASTER vec;
		vec.push_back(lMasterDBID);
		m_mapAllBox[uidBox] = vec;
	}
	else
	{
		TVECTOR_BOXMASTER& vec = (*map_it).second;
		TVECTOR_BOXMASTER::iterator vec_it = find(vec.begin(), vec.end(), lMasterDBID);
		if (vec_it == vec.end())
		{
			// ������
			vec.push_back(lMasterDBID);
		}
	}	
}
//------------------------------------------------------------------------
void CBoxPopedomManager::DelMaster(UID uidBox, long lMasterDBID)
{
	TMAP_ALLBOX::iterator map_it = m_mapAllBox.find(uidBox);
	if (map_it == m_mapAllBox.end())
	{
		return;
	}
	
	TVECTOR_BOXMASTER& vec = (*map_it).second;
	TVECTOR_BOXMASTER::iterator vec_it = find(vec.begin(), vec.end(), lMasterDBID);
	if (vec_it != vec.end())
	{
		vec.erase(vec_it);
	}
}
//------------------------------------------------------------------------
BOOL CBoxPopedomManager::CanOpen(UID uidBox, UID uidMaster)
{
	// ���� uid �õ���ɫ id
	IActor* pMaster = (IActor*)g_pThingClientFacade->GetThing(uidMaster);
	if (pMaster == NULL)
	{
		return FALSE;
	}

	long lMasterDBID = pMaster->GetActorDBID();
	return CanOpen(uidBox, lMasterDBID);
}
//------------------------------------------------------------------------
BOOL CBoxPopedomManager::CanOpen(UID uidBox, long lMasterDBID)
{
	TMAP_ALLBOX::iterator map_it = m_mapAllBox.find(uidBox);
	if (map_it == m_mapAllBox.end())
	{
		// û���ҵ�
		return TRUE;/* �����˶���Ȩ�� */
	}
	
	TVECTOR_BOXMASTER& vec = (*map_it).second;
	if (vec.empty() /* �����˶���Ȩ�� */
		|| find(vec.begin(), vec.end(), lMasterDBID) != vec.end())
	{
		return TRUE;
	}
	
	return FALSE;
}
//------------------------------------------------------------------------
BOOL CBoxPopedomManager::ClearBox(UID uidBox)
{
	TMAP_ALLBOX::iterator map_it = m_mapAllBox.find(uidBox);
	if (map_it == m_mapAllBox.end())
	{
		return FALSE;
	}

	(*map_it).second.clear();
	m_mapAllBox.erase(map_it);
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CBoxPopedomManager::IsAttach(UID uidBox)
{
	return m_mapAllBox.find(uidBox) != m_mapAllBox.end();
}
//------------------------------------------------------------------------
void CBoxPopedomManager::GetMaster(UID uidBox, long* pMasterDBID, int& nMasterCount)
{
	nMasterCount = 0;
	
	TMAP_ALLBOX::iterator map_it = m_mapAllBox.find(uidBox);
	if (map_it == m_mapAllBox.end())
	{
		return;
	}
	
	// �ú��ӵ�Ȩ�ޱ�	
	TVECTOR_BOXMASTER& vec = (*map_it).second;
	for (TVECTOR_BOXMASTER::iterator vec_it = vec.begin(); vec_it != vec.end(); ++vec_it)
	{
		pMasterDBID[nMasterCount++] = *vec_it;
	}
}
//------------------------------------------------------------------------
int CBoxPopedomManager::GetCount(UID uidBox) const
{
	TMAP_ALLBOX::const_iterator map_it = m_mapAllBox.find(uidBox);
	if (map_it == m_mapAllBox.end())
	{
		return 0;
	}

	const TVECTOR_BOXMASTER& vec = (*map_it).second;
	return vec.size();
}
//------------------------------------------------------------------------
void CBoxPopedomManager::CheckBox(UID uidBox)
{
	Init();

	UID_VEC::iterator it = find(m_uidVec.begin(), m_uidVec.end(), uidBox);
	if (it != m_uidVec.end())
	{
		return;
	}

	m_uidVec.push_back(uidBox);
}
//------------------------------------------------------------------------