/**
*	created:		2012-7-2   3:34
*	filename: 		MonsterView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������ͼ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "CreatureView.h"
#include "MonsterStates.h"
#include "FrameControl.h"
#include "../../FKGame/Common/MapItemDef.h"
//------------------------------------------------------------------------
class CMonsterView : public CCreatureView, public CMstFSM, public CFrameControl
{
public:
	CMonsterView();
	virtual ~CMonsterView();

	void Close();
	void ClearAll();
public:
	virtual void	OnTimer(DWORD dwEventID);

public:
	virtual DWORD	GetClassID() const;										// ��ȡ�������ID

	virtual BOOL	OnSerialize(BOOL bWrite,IArchive* pArchive);			// ������ȡ���ݣ����ڴ��ļ��д�������
	virtual BOOL	OnCreateItem(const char* buf, int nLen);				// ���������ֳ���������
	virtual void	OnUpdate();												// ����(�磺���ؽű����ݵ�)

	virtual DWORD	GetOccupantValue();										// ��ȡ��ռλ�����ռλֵ
	virtual COccupantTileList * GetOccupantTileList(void) const;			// ��ȡռλ���б�Ĭ��Ϊ��ռλλ�б�
	virtual DWORD	GetLayerValue();										// ��ȡ�����ڵ�ͼ�е���ʾ��
	
	virtual void	SetDirty(ICanvas* pCanvas, const POINT& ptViewTop);		// ����
	virtual void	GetOldDirtyRect(list<RECT> &DirtyRectList);				// ��ȡ�ɵ��������Ҫ���ڰѶ���ӵ�ͼ��ɾ��ʱ��ȡ���������

	virtual IBitmap	*GetBitmap() {return NULL;}								// ��ȡ��ǰ��ʾ��ͼ��
	virtual void	OnDraw(ICanvas* pCanvas,const POINT& ptTileCenter, DWORD dwFlag = 0);				// ��ͼѭ�����������Ƶ�ָ����ICanvas��
	virtual void	OnDrawTopmostObject(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag=0);	// ��������һЩ���������Ѫ�ۡ��������ݡ���Ʒ���ȣ���Щ������Զ�������ϲ�

public:
	// ���������Ľ�������
	virtual int cmdDie(DWORD dwParam1, DWORD dwParam2);						// ��������
	virtual int	cmdBorn(DWORD dwParam1, DWORD dwParam2);					// ��ɫ����

	virtual int cmdSetNpcType(DWORD dwParam1, DWORD dwParam2);				// ����NPC����Ӧ��ͷ����Ч
	virtual int cmdSetTaskType(DWORD dwParam1, DWORD dwParam2);				// ����NPCͷ���Ĺ�Ч
public:
	// ״̬���
	virtual void SetState(int nState);										// ���õ�ǰ״̬
	virtual void StopState(int nState);										// ֹͣ��ǰ״̬
	virtual void RestartCurState();											// ������ǰ״̬

	// �������������
	virtual int  GetNormalAttackActID();									// ��ȡ��ͨ��������ID
	virtual int	 GetJuqiActID();											// ��ȡĬ�ϵ���������ID���������Ч��
	virtual int	 GetSkillAttackActID();										// ��ȡĬ�ϵļ��ܹ�������ID���������Ч��

	virtual void SetShowRect();												// ���ö�����δ�С��Χ

	// �ƶ����
	virtual void SetStepWalk(float fStep);									// ������������߲���(����ֱ����Ϊ׼)
	virtual void SetStepRun(float fStep);									// ����������ܶ�����(����ֱ����Ϊ׼)
	virtual void SetStepRide(float fStep);									// ��������������ܶ�����(����ֱ����Ϊ׼)
	virtual void SetAllMoveDelay(int* pDelay);								// �����ƶ�������ʱ

	// �����������
	virtual void OnAttachObjDestroy(IAttachObj* pAttachObj);				// �ص�(�����������ʱ�ص�)
	virtual void OnFlowObjDestroy(IAttachObj* pAttachObj);					// �ص�(Ʈ�ֶ�������ʱ�ص�)

	virtual void RemoveAllAttachObj();										// ɾ��������������							// ɾ��������������

	// �����������
	virtual void SetStatusMagicCheckID(int nCheckID);						// ����Ŀǰ״̬�¶�Ӧ�ļ���ID
	virtual void SetStatusPlayCommondAttackSound();							// ����Ŀǰ״̬��Ҫ������ͨ��������
	virtual void SetStatusPreAttack();										// ����Ŀǰ״̬�µ���������
	virtual void SetStatusAttackOnce();										// ����Ŀǰ״̬�µ�һ���Թ�������
	virtual void SetStatusAttackCtrl();										// ����Ŀǰ״̬�µĳ�����������

	virtual void OnDrawEx(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag,
				SAniContext *pContext, BYTE nAlpha = 0);					// Ϊ�˲�Ӱ����


	// �������
	virtual BOOL ChangeShapEX();											// ����

	// ����
	BOOL CreateItem(SMonsterViewArchive* pArchive);							// �����ֳ���������

private:
	int			m_nShape;						// ��������
	COccupant	m_MstOccInfo;					// �����վλ����
	list<RECT>	m_AttachDirtyList;				// �����������������
	int			m_nCorperShowTime;				// ����ʬ����ʧ��������
	int			m_nFallowStateID;				// ��������ж���ID
	IAttachObj*	m_pHeadAttachObj;				// ͷ����Ч
	int			m_nNpcType;						// NPC����Ӧ��ͷ����Ч
	int			m_nTaskType;					// ����NPCͷ���Ĺ�Ч
};
//------------------------------------------------------------------------