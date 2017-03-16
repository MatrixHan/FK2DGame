/**
*	created:		2012-7-2   1:55
*	filename: 		SkillCreate
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ͨ�ô�������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/Common/CreatureViewDef.h"
#include "Magic.h"
//------------------------------------------------------------------------
class CSkillCreate  
{
public:
	CSkillCreate();
	virtual ~CSkillCreate();
	
public:
	// ����ͨ�ü���
	static BOOL CreateGenericSkill(SSkillCreateParam* pParam, CCreatureView *pMaster = NULL);

	// ��ͨ�����Ĵ�������
	static BOOL CreateCommonAttackSkill(SSkillCreateParam* pParam, CCreatureView *pMaster = NULL);

	// ����Զ�̹������ͼ��ܵĴ�������
	static BOOL CreateSingleDirSkill(SSkillCreateParam* pParam, CCreatureView *pMaster = NULL);

	// �������Զ�̹������ͼ��ܵĴ�������
	static BOOL CreateMultiDirSkillEx(SSkillCreateParam* pParam, CCreatureView *pMaster = NULL);

	// �򽣳����༼�ܵĴ�������
	static BOOL CreateWanJianSkill(SSkillCreateParam* pParam, CCreatureView *pMaster = NULL);

	// �൥Ԫ�����ͼ��ܵĴ�������
	static BOOL CreateRainSkill(SSkillCreateParam* pParam, CCreatureView *pMaster = NULL);

	// ��ͬ��Ŀ��Ķ����Ԫħ�������������ȣ�
	static BOOL CreateMultiUnitSingleTileSkill(SSkillCreateParam* pParam, CCreatureView *pMaster = NULL);

	// 8�����ܵĴ�������
	static BOOL CreateCirculDirSkill(SSkillCreateParam* pParam, CCreatureView *pMaster = NULL, int nDir = 8);
	
	// ˳��
	static BOOL CreateChangeLoc(POINT pt, CCreatureView *pMaster = NULL);

public:
	// ������ӡ����
	static int CreateFootprintAnimate(SSkillCreateParam* pParam, int nDirection, int nLeftRight);

	// ����ͨ�ö���
	static int CreateGenericMagic(SSkillCreateParam* pParam);

	// ��ӰЧ��
	static BOOL CreateShadow(CCreatureView *pMaster = NULL);

public:
	static void* getSkillScpObj(int nAniType, int nIndex);
	static void* getSkillLevelScpObj(int nAniType, int nIndex, int nLevel);
};
//------------------------------------------------------------------------