/**
*	created:		2012-7-2   3:52
*	filename: 		ActorView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�����ͼ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "CreatureView.h"
#include "FrameControl.h"
#include "ActorStates.h"
#include "ActorTitle.h"
//------------------------------------------------------------------------
// ActorView�浵��ʽ
struct SActorViewArchive
{
	DWORD	dwActTypeID;
};
//------------------------------------------------------------------------
class CActorView : public CCreatureView, public CFrameControl, public CActFSM
{
public:
	CActorView();
	virtual ~CActorView();

	void Close();
	virtual void	Release();

public:
	virtual void	OnTimer(DWORD dwEventID);

public:
	virtual DWORD	GetClassID() const;										// ��ȡ�������ID

	virtual BOOL	OnSerialize(BOOL bWrite,IArchive* pArchive);			// ������ȡ���ݣ����ڴ��ļ��д�������
	virtual BOOL	OnCreateItem(const char* buf, int nLen);				// ���������ֳ���������
	virtual void	OnUpdate();												// ����(�磺���ؽű����ݵ�)

	virtual DWORD	GetOccupantValue();										// ��ȡ��ռλ�����ռλֵ
	virtual DWORD	GetLayerValue();										// ��ȡ�����ڵ�ͼ�е���ʾ��
	
	virtual void	SetDirty(ICanvas* pCanvas, const POINT& ptViewTop);		// ����
	virtual void	GetOldDirtyRect(list<RECT> &DirtyRectList);				// ��ȡ�ɵ��������Ҫ���ڰѶ���ӵ�ͼ��ɾ��ʱ��ȡ���������

	virtual IBitmap	*GetBitmap() {return NULL;}								// ��ȡ��ǰ��ʾ��ͼ��
	virtual void	OnDraw(ICanvas* pCanvas,const POINT& ptTileCenter, DWORD dwFlag = 0);				// ��ͼѭ�����������Ƶ�ָ����ICanvas��
	virtual void	OnDrawTopmostObject(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag=0);	// ��������һЩ���������Ѫ�ۡ��������ݡ���Ʒ���ȣ���Щ������Զ�������ϲ�

public:
	// ���������Ľ�������
	virtual int	cmdChangeModule(DWORD dwParam1, DWORD dwParam2);			// ��װ
	virtual int cmdSpecialModule(DWORD dwPam1, DWORD dwParam2);				// ���ý�ɫĬ�ϲ���
	virtual int	cmdModulePalType(DWORD dwParam1, DWORD dwParam2);			// ��ɫ������ĵ�ɫ����������
	virtual int	cmdModuleAlpha(DWORD dwParam1, DWORD dwParam2);				// �ı��ɫ͸����

	virtual int	cmdBorn(DWORD dwParam1, DWORD dwParam2);					// ��ɫ����
	virtual int	cmdUpgrade(DWORD dwParam1, DWORD dwParam2);					// ��ɫ����

	virtual int	cmdMouseAni(DWORD dwParam1, DWORD dwParam2);				// ��������ʱ����·�ϵ��������µĵ������

	virtual int cmdAddTitle(DWORD dwParam1, DWORD dwParam2);				// ��������ӳƺ�
	virtual int cmdClearTitle(DWORD dwParam1, DWORD dwParam2);				// �����������гƺ�

	// ���úͻ�ȡ��������
	virtual void propWeaponStyle(BOOL bSetProp, DWORD dwPropVal);			// ��ɫЯ�������ķ�ʽ
	virtual void propWeaponSoundID(BOOL bSetProp, DWORD dwPropVal);			// ��ɫ�����Ĺ�����ЧID
	virtual void propBeHitSoundID(BOOL bSetProp, DWORD dwPropVal);			// ��ɫ�����Ļ���Ŀ����ЧID
	virtual void propGroupInfoImage(BOOL bSetProp, DWORD dwPropVal);		// �����ϢͼƬ

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
	virtual void SetAllMoveDelay(int* pDelay);								// �����ƶ�������ʱ

	// �����������
	virtual void OnAttachObjDestroy(IAttachObj* pAttachObj);				// �ص�(�����������ʱ�ص�)
	virtual void OnFlowObjDestroy(IAttachObj* pAttachObj);					// �ص�(Ʈ�ֶ�������ʱ�ص�)

	virtual void RemoveAllAttachObj();										// ɾ��������������

	// �����������
	virtual int	 GetWeaponStyle();											// ��ȡ��������

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
	BOOL CreateItem(SActorViewArchive* pArchive);							// �����ֳ���������
	virtual int CreateFootprint();											// ������ӡ

public:
	IBitmap*		m_pGroupInfoImage;				// ��ᣬ������ϢͼƬ(�ⲿ�ͷ�)
	CActorTitle		m_ActorTitle;					// ��ɫ�ƺ�
	int				m_nFootprintLeftRight;			// ���ҽ�ӡ
	int				m_nWeaponStyle;					// ��������
	list<RECT>		m_AttachDirtyList;				// �����������������
};
//------------------------------------------------------------------------