/**
*	created:		2012-7-2   3:10
*	filename: 		MagicMgrMultiUnitSingleTile
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�൥Ԫ����ħ��������ֻ�����һ��Tile��
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Magic.h"
#include "../../FKGame/MapInfo/Tile.h"
//------------------------------------------------------------------------
class CMagicMgrMultiUnitSingleTile : public CMagicMgrBase, public ITimerSink
{
	CMagicRain	**m_paUnit;				// ��Ԫ����

	POINT		m_ptAttackTile;			// ����������TILE
	int			m_nUnitCount;			// ��Ԫ��Ŀ
	int			m_nCurCreateCount;		// Ŀǰ��������
	int			m_nCurDestroyCount;		// Ŀǰ���ٸ���
	int			m_nDelay;				// ������ʱ
	int			m_nFlowDamNum;			// ��ǰ��ҪƮ���˺���

public:
	struct SMagicMgrArchive_MultiUnitSingleTile
	{
		POINT		ptAttackTile;		// ����������TILE
		int			nDelay;				// ������Ԫħ�����
		int			nTime;				// �������
		SMagicArchive_Rain	*pUnit;		// ��Ԫ�Ĵ�����
	};
public:
	CMagicMgrMultiUnitSingleTile();
	virtual ~CMagicMgrMultiUnitSingleTile();

public:
	virtual BOOL Create(void* buf, int nSize);
	virtual void OnTimer(DWORD dwEventID);
	virtual void OnPlayExplode(void* pThis);
	virtual void OnPlayDone(void* pThis, POINT Pt);
	virtual void BeginAttack();
};
//------------------------------------------------------------------------