/**
*	created:		2012-7-2   3:07
*	filename: 		MagicMgrGeneral
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ͨ�õ�ħ��������(������̬�ͱ�ը̬���)
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Magic.h"
//------------------------------------------------------------------------
class CMagicMgrGeneral : public CMagicMgrBase, public ITimerSink
{
	CMagicGeneral	*m_pRun;
	CMagicGeneral	*m_pExplode;
	int				m_nLoopTime;
	BOOL			m_bIsRunning;
	BOOL			m_bBegin;
	BOOL			m_bCanCtrl;
	BOOL			m_bOnceAttack;		// �Ƿ�һ�����˺�
public:
	struct SMagicMgrArchive_General
	{
		POINT					ptTile;				// ��������TILE
		POINT					ptAttackTile;		// ����������TILE
		BOOL					bNeedAttackDelay;	// �Ƿ���Ҫ�����ӳ�
		int						nLoopTime;			// ѭ����ʱ��
		BOOL					bCanCtrl;			// �Ƿ��ܿ���
		BOOL					bOnceAttack;		// �Ƿ�һ�����˺�
		
		SMagicArchive_General	*pRun;				// ����̬ħ���Ĵ�������
		SMagicArchive_General	*pExplode;			// ��ը̬ħ���Ĵ�������
	};
	CMagicMgrGeneral();
	virtual ~CMagicMgrGeneral();
	
public:
	virtual BOOL Create(void* buf, int nSize);
	virtual void OnPlayDone(void* pThis, POINT Pt);
	virtual void OnTimer(DWORD dwEventID);
	virtual void BeginAttack();
	virtual void Stop();
	virtual BOOL IsRunning();
	virtual void AddDamage(CCreatureView *pView, SFlowParam FlowParam);
};
//------------------------------------------------------------------------