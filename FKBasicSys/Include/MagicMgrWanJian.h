/**
*	created:		2012-7-2   3:13
*	filename: 		MagicMgrWanJian
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�򽣳���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Magic.h"
//------------------------------------------------------------------------
class CMagicMgrWanJian : public CMagicMgrBase, public ITimerSink
{
	int				m_nDir;

	int				m_nLoopTime;				// ѭ��ʱ��
	int				m_nDelay;					// ������ʱ
	BOOL			m_bStop;
	POINT			m_ptTile;					// ��������TILE
	
public:
	struct SMagicMgrArchive_WanJian
	{
		POINT		ptTile;						// ��������TILE
		int			nDelay;						// ������Ԫħ�����
		int			nLoopTime;					// ѭ��ʱ��
	};
public:
	CMagicMgrWanJian();
	virtual ~CMagicMgrWanJian();

	virtual BOOL Create(void* buf, int nSize);
	virtual void OnTimer(DWORD dwEventID);
	virtual void OnPlayDone(void* pThis, POINT Pt);
	virtual void Stop();
	virtual BOOL IsRunning();
	virtual void BeginAttack();
};
//------------------------------------------------------------------------