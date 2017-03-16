/**
*	created:		2012-7-2   3:11
*	filename: 		MagicMgrOnePart
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		һ������ħ��������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Magic.h"
//------------------------------------------------------------------------
template <class TMagic, class TMagicArchive>
class CMagicMgrOnePart : public CMagicMgrBase
{
	TMagic*			m_pCreate;		// ����ħ��
public:
	struct SMagicMgrArchive_OnePart
	{
		POINT					ptTile;			// ��������TILE
		POINT					ptAttackTile;	// ����������TILE
		TMagicArchive*			pCreate;		// ����
	};

	CMagicMgrOnePart() : m_pCreate(NULL) {}
	virtual ~CMagicMgrOnePart() {}
	
	virtual BOOL Create(void* buf, int nSize);
	virtual void OnPlayDone(void* pThis, POINT Pt);
	TMagic* GetMagicPtr() {return m_pCreate;}
};
//------------------------------------------------------------------------
template <class TMagic, class TMagicArchive>
BOOL CMagicMgrOnePart<TMagic, TMagicArchive>::Create(void* buf, int nSize)
{
	SMagicMgrArchive_OnePart* pArchive = (SMagicMgrArchive_OnePart*)buf;
	
	// ����ħ��
	if (pArchive->pCreate)
	{
		m_pCreate = new TMagic;
		if (!m_pCreate->OnCreateItem((const char*)pArchive->pCreate, 
			sizeof(TMagicArchive)) || !m_pCreate->AddToMapView(pArchive->ptTile))
		{
			SAFE_RELEASE(m_pCreate);
			return FALSE;
		}
		m_pCreate->m_ptTile = pArchive->ptTile;
		if (!m_pCreate->AddToTimerAxis())
		{
			m_pCreate->RemoveFromMapView();
			SAFE_RELEASE(m_pCreate);
			return FALSE;
		}
		m_pCreate->SetMgr(this);
		return TRUE;
	}
	return FALSE;
}
//------------------------------------------------------------------------
template <class TMagic, class TMagicArchive>
void CMagicMgrOnePart<TMagic, TMagicArchive>::OnPlayDone(void* pThis, POINT Pt)
{
	if (m_pCreate)
	{
		m_pCreate = NULL;
		delete this;
	}
}
//------------------------------------------------------------------------