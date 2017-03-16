/**
*	created:		2012-7-2   3:08
*	filename: 		MagicMgrMultiDir
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����Ķ൥Ԫ����ħ�������������һ����Ƕ෽��ģ�����Ŀ�ĵ���һ������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Magic.h"
//------------------------------------------------------------------------
#define MAX_DIR_COUNT	20
//------------------------------------------------------------------------
class CMagicMgrMultiDirEx : public CMagicMgrBase, public ITimerSink
{
	CMagicSingleDir		**m_paUnit;				// ��Ԫ����
	CMagicGeneral		**m_paLeftObj;			// ���������

	POINT				m_ptAttackLT;
	int					m_nWidth;
	int					m_nDelay;				// ������ʱ

	int					m_nExplodeNum;
	int					m_nUnitCount;			// ��Ԫ��Ŀ
	int					m_nCurCreateCount;		// Ŀǰ��������
	int					m_nCurDestroyCount;		// Ŀǰ���ٸ���
public:
	struct SMagicMgrArchive_MultiDirEx
	{
		POINT	ptAttackTile;					// ����������TILE
		int		nWidth;							// ����������ı߳�(TILE����)
		int		nDelay;							// ������Ԫħ�����
		SMagicArchive_SingleDir		*pRun;		// ����
		SMagicArchive_General		*pExplode;	// ��ը
	};
public:
	CMagicMgrMultiDirEx();
	virtual ~CMagicMgrMultiDirEx();

	virtual BOOL Create(void* buf, int nSize);
	virtual void OnTimer(DWORD dwEventID);
	virtual void OnPlayDone(void* pThis, POINT Pt);
	virtual void BeginAttack();
};
//------------------------------------------------------------------------