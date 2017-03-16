/**
*	created:		2012-7-2   3:12
*	filename: 		MagicMgrSingleDir
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ħ��������(����ħ����3���׶���ɣ����������С���ը)
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Magic.h"
//------------------------------------------------------------------------
class CMagicMgrSingleDir : public CMagicMgrBase, public ITimerSink
{
public:
	CMagicSingleDir		*m_pRun;
	CMagicGeneral		*m_pExplode;

public:
	struct SMagicMgrArchive_SingleDir
	{
		SMagicArchive_SingleDir		*pRun;		// ����
		SMagicArchive_General		*pExplode;	// ��ը
	};
	CMagicMgrSingleDir();
	virtual ~CMagicMgrSingleDir();

	virtual BOOL Create(void* buf, int nSize);
	virtual void OnTimer(DWORD dwEventID);
	virtual void OnPlayDone(void* pThis, POINT Pt);
	virtual void BeginAttack();
};
//------------------------------------------------------------------------