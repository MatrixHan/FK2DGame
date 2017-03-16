/**
*	created:		2012-6-25   0:50
*	filename: 		BaseControlMode
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		IController����յ��ĸ��ֲ���ί�й���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
class CMapItemView;
//------------------------------------------------------------------------
#include "IControlModel.h"
#include "../Finder/MonsterFinder.h"
#include "../ValidationCode/WndMgrSignal.h"
//------------------------------------------------------------------------
class CBaseControlMode : public IControlMode 
{
private:
	BOOL m_bAutoAttack;                             //�Ƿ������Զ����
	CMonsterFinder          m_MonsterFinder;        //����������
	BOOL                    m_bPressCtrl;			//��¼�Ƿ�����Ctrl
public:
	void __SaveScreen(void);

	/*
	�����������ϵĹ�������, 
	������ϸ����ǹ�������,�������ȡ��,������TRUE
	������ϸ�������������,�����ȡ��,������FALSE
	*/
	BOOL clearAttackCmd();
	/*
	  ���û�ѡ����(����F1,��)��,���������ͷż���,����ѡ����
	  ����ɹ�,����TRUE   ,�ɹ�������, 
								1  �ü��ܲ���ѡ����  
								2  �ϴε�ѡ�Ķ���û��

	  ���򷵻�FALSE
	*/
	BOOL attack_Immediate(int SkillID); 	
	/*
	  �û�����1, ��Ȱ�ťʱִ��
	*/
	void UseSkill(int SkillID);
	/*
	  �û����� Alt + ���� �Ȱ�ťʱִ��
	*/
	void UseSkill_Self(int SkillID);
	/*
	  ��������ϸ��ŵĹ�������
	  ���� ,  ���е�CMapItemView, �����е�Tile
	  ��� ,  ����������ϵ�����,����TRUE, ���򷵻�FALSE
	*/
	BOOL ProcessCursorAttackCmd(CMapItemView* pItem, POINT ptTileClicked);
	/*
	  ��������ϸ��ŵ�LUAѡ������
	  ���� ,  ���е�CMapItemView, �����е�Tile
	  ��� ,  ����������ϵ�����,����TRUE, ���򷵻�FALSE
	 */
	BOOL ProcessLUASelectActorCmd(CMapItemView* pItem, POINT ptTileClicked);
	/*
	  ���ĳ������, �Ƿ�����ȴ״̬
	  ���� ,  ����ID
	  ��� ,  �ü����Ƿ�����ȴ 
	*/
	BOOL __SkillInColding(int nSkillID);
	/*
	  ʰȡ���һȦ�ı���
	*/
	void __PickUpTreasure_Round(void);

	//����ʰȡĳһ�����ϵ�����(��__PickUpTreasure_Round() �е��� )
	//�ɹ����˺���, ����TRUE, ����FALSE
	BOOL __TryPickup(int Tile_X, int Tile_Y);

	//����ʰȡĳһ��ITEM  (��__TryPickup(int Tile_X, Tile_Y) �е��� )
	//�ɹ����˺���, ����TRUE, ����FALSE
	BOOL __TryPickupBox(CMapItemView* pItem);
	/*
	  ת��һ���û����µļ�
	*/
	void __TranslateKeyCode(int *pKeyCode);
	/*
	  ���һ��, ���е��Ƿ���
	*/
	BOOL __isTreasureBox(CMapItemView *pItem);

	CWMConnection<CBaseControlMode> m_SkillConnect;				// ʹ�ü���ʱ,������յ���Ϣ
	CWMConnection<CBaseControlMode> m_SkillSelfConnect;			// ����������������
	CWMConnection<CBaseControlMode> m_GoodsConnect;  
	CWMConnection<CBaseControlMode> m_CommandQueueConnect;		// ����������е�����
	CWMConnection<CBaseControlMode> m_CommandTraceConnect;		// ����������������
	CWMConnection<CBaseControlMode> m_CommandSelectTeamate;		// ����������������
	CWMConnection<CBaseControlMode> m_CommandAttackTeamate;		// �����ʩ��������
	CWMConnection<CBaseControlMode> m_CommandPet;				// ִ�г���ָ������
	CWMConnection<CBaseControlMode> m_CommandSetLockFire;		// �����Զ���������
	CWMConnection<CBaseControlMode> m_CommandCancelTarget;		// ȡ����ǰĿ������
	CWMConnection<CBaseControlMode> m_CommandLongMove;			// �������ƶ�
	CWMConnection<CBaseControlMode> m_CommandLoadMapBlock;		// ��ȡ��ͼռλ��

public:
	CBaseControlMode();
	virtual ~CBaseControlMode();
	
	virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnRButtonUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnRButtonDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam);

	//����ģ�鷢����,Ҫ��ʹ�ü���
	void OnSkillClickedConnect( WPARAM wParam, LPARAM lParam,BOOL &bContinue);
	void OnGoodsClickedConnect( WPARAM wParam, LPARAM lParam,BOOL &bContinue);

	//����ģ�鷢����,Ҫ��������������� 
	void OnClearCommandQueueConnect( WPARAM wParam, LPARAM lParam,BOOL &bContinue);

	//����ģ�鷢����,Ҫ�����ĳ�����
	void OnTraceCommandConnect( WPARAM wParam, LPARAM lParam,BOOL &bContinue);

	//����ģ�鷢����,Ҫ����Լ�ʹ�ü���
	void OnSelfSkillConnect( WPARAM wParam, LPARAM lParam,BOOL &bContinue);
	
	//��֪�����﷢����, ˵ѡ����һĿ��
	void OnSelectNewTargetCommandConnect( WPARAM wParam, LPARAM lParam,BOOL &bContinue);

	//��֪�����﷢����, ˵�����ʩ��
	void OnAttackTeamateCommandConnect( WPARAM wParam, LPARAM lParam,BOOL &bContinue);

	//��֪�����﷢����, ȡ����ǰĿ�껷
	void OnCancelTargetCommandConnect( WPARAM wParam, LPARAM lParam,BOOL &bContinue);

	//���ڷ�����, ִ�г���ָ��
	void OnExecutePetCommandConnect( WPARAM wParam, LPARAM lParam,BOOL &bContinue);

	//���ڷ�����, ����"�Զ�����"�Ƿ���
	void OnSetLockFireState( WPARAM wParam, LPARAM lParam,BOOL &bContinue);

	//���ڷ�����, ִ�г������ƶ�ָ��
	void OnLongmoveCommandConnect( WPARAM wParam, LPARAM lParam,BOOL &bContinue);

	//���ڷ�����, ��ȡ��ͼռλ����Ϣ
	void OnLoadMapBlockCommandConnect( WPARAM wParam, LPARAM lParam,BOOL &bContinue);

	void ConnectSingal(void);
	void DisConnectSingal(void);
};
//------------------------------------------------------------------------