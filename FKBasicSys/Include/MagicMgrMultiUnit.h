/**
*	created:		2012-7-2   3:09
*	filename: 		MagicMgrMultiUnit
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�൥Ԫ����ħ��������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Magic.h"
#include "../../FKGame/MapInfo/Tile.h"
//------------------------------------------------------------------------
#define MAX_ATTACK_TIME		10000
//------------------------------------------------------------------------
class CMagicMgrMultiUnit : public CMagicMgrBase, public ITimerSink
{
	SMagicArchive_Rain	m_pUnit;

	POINT			m_ptAttackLT;
	int				m_nWidth;
	int				m_nLoopTime;		// ѭ��ʱ��
	int				m_nDelay;			// ������ʱ
	BOOL			m_bStop;


public:
	struct SMagicMgrArchive_MultiUnit
	{
		POINT		ptAttackTile;		// ����������TILE
		int			nWidth;				// ����������ı߳�(TILE����)
		int			nDelay;				// ������Ԫħ�����
		int			nLoopTime;			// ѭ��ʱ��
		SMagicArchive_Rain	*pUnit;		// ��Ԫ�Ĵ�����
	};
public:
	CMagicMgrMultiUnit();
	virtual ~CMagicMgrMultiUnit();

public:
	virtual BOOL Create(void* buf, int nSize);
	virtual void OnTimer(DWORD dwEventID);
	virtual void OnPlayDone(void* pThis, POINT Pt);
	virtual void Stop();
	virtual BOOL IsRunning();
	virtual void BeginAttack();
};
//------------------------------------------------------------------------