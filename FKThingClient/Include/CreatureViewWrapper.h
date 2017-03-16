/**
*	created:		2012-7-2   23:57
*	filename: 		CreatureViewWrapper
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������ʾ��װ��
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/Timer/ITimerAxis.h"
#include "../../FKGame/Common/CreatureViewDef.h"
#include "../../FKGame/MapView/IMapView.h"
#include "../../FKGame/MapView/MapItemView.h"
#include "../../FKCanvas/IFKCanvas.h"
//------------------------------------------------------------------------
// ����
struct stBehitParam;
//------------------------------------------------------------------------
// ����
class CCreatureViewWrapper
{
public:	
	CMapItemView * operator ->(void);

	BOOL				IsValid(void);
	BOOL				AddToMap(POINTS ptTile);

	CMapItemView *		GetItemView(void);

	void				SetData(DWORD dwData);
	void				AddFlag(DWORD dwFlag);
	void				RemoveFlag(DWORD dwFlag);

	// �ƶ�����
	DWORD				GetMoveStyle(void);
	void				SetMoveStyle(DWORD dwStyle);

	// �����TILE����
	POINT				GetTile(void);
	void				SetTile(POINTS ptTile);

	// ���������
	void				GetName(LPSTR szBuf);
	void				SetName(LPCSTR szName);

	// ���ֵ���ɫ
	void				SetNameColor(int r, int g, int b);
	void				SetNameColor(DWORD dwColor);

	// ����״̬
	int					GetState(void);
	void				SetState(int nState);

	// �Ƿ���������ʾ�Լ�������
	BOOL				GetPropShowName(void);
	void				SetPropShowName(BOOL bShow);

	// ����ľ���λ��(ptTileCenter+ptOffset)
	POINT				GetPosition(void);
	void				SetPosition(POINT ptPos);

	// ����ķ���
	int					GetDirection(void);
	void				SetDirection(int nDir);

	// ��ɫЯ�������ķ�ʽ(��������?��������?˫������?)
	DWORD				GetPropWeaponStyle(void);
	void				SetPropWeaponStyle(DWORD dwStyle);

	// �����Ƿ������ƶ�?
	BOOL				IsMoving(void);
	// �ƶ�����,�Ե�ǰ���ƶ���ʽ�ƶ�
	void				Move(POINTS * pPath, int nPathLen);
	// ֹͣ�ƶ�վ����ĳ��Tile��
	void				Stand(POINT ptTile);	
	// �ü���˳�Ƶ�ĳ��Tile��
	void				ChangeLocForSkill(POINT ptTile);	
	// ����
	void				Sit(void);
	// ���ﱻ��
	void				BeHit(SBehitParam * pParam);
	// ��������
	void				Die(DWORD dwParam1, DWORD dwParam2);
	// �������
	void				Born(void);
	// ��������
	void				Upgrade(void);
	// ��������
	void				Fallow(void);
	// ��װ���任���������
	void				ChangeModule(DWORD dwModule, DWORD dwResID);
	// �ı������ĵ�ɫ����������
	void				ChangeModulePal(DWORD dwModule, DWORD dwPalType);
	// ָ���ڲ����Ҳ���ͼ��ʱ�����ض���ͼ������
	void				SpecialModule(DWORD dwModule, DWORD dwResID);
	// ֹͣ����
	void				StopAttack(void);
	// ʹ��ħ��(����)
	void				Attack(DWORD pParam);	
	// ֹͣ����
	void				StopPrepAttack(void);
	// ����
	void				PrepAttack(DWORD pParam);
	// �л���һ��״̬
	void				ChangeToState(int nState, BOOL bFlag);
	// ȡ��ħ��״̬
	void				DestroyMagic(DWORD dwParam);

	// �����ƶ��ٶȵȼ�(0->8)
	void				SetMoveSpeedLevel(int * pSpeed);
	int                 GetMoveSpeedLevel(void);
	// ���ù����ٶ�
	void				SetAttackSpeed(int * pSpeed);
	// ����Ѫ��
	void				SetBloodInfo(int nNowBlood, int nTotalBlood);
	// ������������
	void				SetChatInfo(LPCSTR szMsg, WPixel * pColor);
	// ���ð�����Ϣ
	void				SetTribeTitle(IBitmap * pBitmap);
	// ���óƺ�
	void				SetTitle(LPCSTR szMsg, DWORD dwEffectID);
	// ���������Ĺ�����ЧID
	void				SetWeaponAttackSoundID(int nSoundID);
	// ���������Ļ�����ЧID
	void				SetWeaponBeHitSoundID(int nSoundID);
	// ����С��ͼ��ɫ
	void				SetFlagColorOnMinmap(DWORD dwColor);
	void				SetFlagColorOnMinmap(int r, int g, int b);

	CCreatureViewWrapper(CMapItemView * pItemView);

private:
	CMapItemView *			m_pItemView;
};
//------------------------------------------------------------------------