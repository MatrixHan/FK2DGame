/**
*	created:		2012-7-3   0:02
*	filename: 		BoxPopedomMgr
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		boxȨ�޹������й���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <vector>
#include <map>
#include "../../FKGame/Event/IEvent.h"
using std::vector;
using std::map;
//------------------------------------------------------------------------
typedef __int64 UID;
//------------------------------------------------------------------------
class CBoxPopedomManager : public IEventActionListener
{
public:
	virtual void OnAction(IEvent* pEvent);

	// ����һȨ����,
	void AddMaster(UID uidBox, long lMasterDBID);
	void DelMaster(UID uidBox, long lMasterDBID);

	// ����ܷ�ʰ����Ʒ
	BOOL CanOpen(UID uidBox, UID uidMaster);
	BOOL CanOpen(UID uidBox, long lMasterDBID);

	// �����box�Ĺ���
	BOOL ClearBox(UID uidBox);

	// ȡ�ô�box�Ƿ����й���
	BOOL IsAttach(UID uidBox);

	// ȡ����Ȩ����
	void GetMaster(UID uidBox, long* pMasterDBID, int &nMasterCount);

	// ȡ����Ȩ�޵�����
	int GetCount(UID uidBox) const;

	// �����������ﴴ���Ľ��
	void CheckBox(UID uidBox);

	CBoxPopedomManager(void);
	~CBoxPopedomManager(void);
private:
	BOOL Init();
	void Term();

private:
	typedef vector<UID>					UID_VEC;
	typedef vector<long>			    TVECTOR_BOXMASTER;
	typedef map<UID, TVECTOR_BOXMASTER>	TMAP_ALLBOX;
	TMAP_ALLBOX		m_mapAllBox;
	UID_VEC         m_uidVec;
	BOOL            m_bInit;	
};
//------------------------------------------------------------------------
extern CBoxPopedomManager g_BoxPopedomManager;
//------------------------------------------------------------------------