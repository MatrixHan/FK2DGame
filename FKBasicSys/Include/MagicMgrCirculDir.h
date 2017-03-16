/**
*	created:		2012-7-2   2:58
*	filename: 		MagicMgrCirculDir
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		8�����Բ�κ����μ��ܹ�����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Magic.h"
//------------------------------------------------------------------------
class CMagicMgrCirculDir : public CMagicMgrBase, public ITimerSink
{
protected:
	CMagicEightDir	**m_paRun;
	int				m_nDirNum;			// �����ķ�������8ΪԲ�Σ�5Ϊ����
	int				m_nCurDestroyCount;	// Ŀǰ���ٸ���
	POINT			m_ptTile;			// ��������TILE
	BOOL			m_bRealTime;		// ���й�Ч�Ƿ��ܹ���֡����
public:
	struct SMagicMgrArchive_CirculDir
	{
		int			nDirNum;		// �����ķ�������8ΪԲ�Σ�5Ϊ����
		POINT		ptTile;			// ��������TILE
		POINT		ptAttackTile;	// ����������TILE
		SMagicArchive_EightDir		*pRun;		// ����
		BOOL		bRealTime;		// ���й�Ч�Ƿ��ܹ���֡����
	};

	CMagicMgrCirculDir();
	virtual ~CMagicMgrCirculDir();

	virtual BOOL Create(void* buf, int nSize);
	virtual void OnTimer(DWORD dwEventID);
	virtual void OnPlayDone(void* pThis, POINT Pt);
	virtual void BeginAttack();
};
//------------------------------------------------------------------------