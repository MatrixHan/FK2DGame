/**
*	created:		2012-7-2   2:48
*	filename: 		CreatureViewSkill
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�����＼���йص�
*/
//------------------------------------------------------------------------
#include "../Include/BasicCommon.h"
#include "../Include/CreatureView.h"
#include "../Include/ResLoader.h"
#include "../Include/SkillCreate.h"
//------------------------------------------------------------------------
CCreatureView::ProcCreateSkill CCreatureView::m_funcSkill[SKILL_MAX_COUNT + 1] =
{
	&CCreatureView::skill_CommonAttack,					// 0	��ͨ����

// սʿ
	&CCreatureView::skill_SingleCirculDirSkill,			// 1	�Ʒ�ն
	&CCreatureView::skill_SingleCirculDirSkill,			// 2	����
	&CCreatureView::skill_SingleCirculDirSkill,			// 3	�����͵�
	&CCreatureView::skill_GenericSkill,					// 4	ŭ��
	
	&CCreatureView::skill_GenericSkill,					// 5	����
	&CCreatureView::skill_HalfCirculDirSkill,			// 6	��ɨ�˷�
	&CCreatureView::skill_None,							// 7	���
	&CCreatureView::skill_SingleDir,					// 8	�ػ�
	
	&CCreatureView::skill_SingleCirculDirSkill,			// 9	����
	&CCreatureView::skill_SingleCirculDirSkill,			// 10	��ɱ��
	&CCreatureView::skill_GenericSkill,					// 11	ŭ�����
	&CCreatureView::skill_GenericSkill,					// 12	�ܱ�
	
	&CCreatureView::skill_GenericSkill,					// 13	����
	&CCreatureView::skill_GenericSkill,					// 14	��������
	&CCreatureView::skill_SingleCirculDirSkill,			// 15	��ɱ
	&CCreatureView::skill_SingleCirculDirSkill,			// 16	��Ѫ��ɱ
	
	&CCreatureView::skill_GenericSkill,					// 17	սʿ��̬�л�
	
// ��ʦ
	&CCreatureView::skill_SingleDir,					// 18	������
	&CCreatureView::skill_GenericSkill,					// 19	�������
	&CCreatureView::skill_SingleDir,					// 20	������
	&CCreatureView::skill_GenericSkill,					// 21	�ƻ�����
	
	&CCreatureView::skill_None,							// 22	��Ӱ����
	&CCreatureView::skill_MultiUnitSingleTileSkill,		// 23	��������
	&CCreatureView::skill_GenericSkill,					// 24	���
	&CCreatureView::skill_SingleDir,					// 25	���

	&CCreatureView::skill_GenericSkill,					// 26	��ӡ
	&CCreatureView::skill_GenericSkill,					// 27	����
	&CCreatureView::skill_GenericSkill,					// 28	����
	&CCreatureView::skill_GenericSkill,					// 29	����쳾
	
	&CCreatureView::skill_RainSkill,					// 30	��ѩ����
	&CCreatureView::skill_SingleDir,					// 31	����
	&CCreatureView::skill_GenericSkill,					// 32	�ػ���
	&CCreatureView::skill_GenericSkill,					// 33	����
	
	&CCreatureView::skill_GenericSkill,					// 34	��ʦ��̬�л�
	
// ��ʦ
	&CCreatureView::skill_SingleDir,					// 35	�ϵ羪��
	&CCreatureView::skill_GenericSkill,					// 36	��Ȫ����
	&CCreatureView::skill_GenericSkill,					// 37	����Ԫ��
	&CCreatureView::skill_GenericSkill,					// 38	��������
	
	&CCreatureView::skill_GenericSkill,					// 39	����
	&CCreatureView::skill_GenericSkill,					// 40	����ħ
	&CCreatureView::skill_GenericSkill,					// 41	������
	&CCreatureView::skill_GenericSkill,					// 42	���ֻش�
	
	&CCreatureView::skill_GenericSkill,					// 43	��������
	&CCreatureView::skill_GenericSkill,					// 44	��������
	&CCreatureView::skill_GenericSkill,					// 45	�������
	&CCreatureView::skill_SingleCirculDirSkill,			// 46	������צ
	
	&CCreatureView::skill_SingleCirculDirSkill,			// 47	����
	&CCreatureView::skill_SingleCirculDirSkill,			// 48	����
	&CCreatureView::skill_GenericSkill,					// 49	Ѱ��
	&CCreatureView::skill_SingleCirculDirSkill,			// 50	���ƶϻ�
	
	&CCreatureView::skill_GenericSkill,					// 51	��ʦ��̬�л�

// ����
	&CCreatureView::skill_GenericSkill,					// 52	ħ�޷���
	&CCreatureView::skill_SingleDir,					// 53	��Ӱ
	&CCreatureView::skill_GenericSkill,					// 54	ڤ��ʴ��
	&CCreatureView::skill_GenericSkill,					// 55	��Ѫ
	
	&CCreatureView::skill_GenericSkill,					// 56	�ε�����
	&CCreatureView::skill_GenericSkill,					// 57	����
	&CCreatureView::skill_GenericSkill,					// 58	��Ӱ����
	&CCreatureView::skill_GenericSkill,					// 59	����
	
	&CCreatureView::skill_GenericSkill,					// 60	����
	&CCreatureView::skill_GenericSkill,					// 61	������״
	&CCreatureView::skill_GenericSkill,					// 62	���ҿ�ħ
	&CCreatureView::skill_GenericSkill,					// 63	�����׶�

	&CCreatureView::skill_GenericSkill,					// 64	ʴƤ����
	&CCreatureView::skill_GenericSkill,					// 65	����
	&CCreatureView::skill_GenericSkill,					// 66	�߻�����
	&CCreatureView::skill_GenericSkill,					// 67	������

	&CCreatureView::skill_GenericSkill,					// 68	������̬�л�

// ѵ������
	&CCreatureView::skill_NNNNNNNNNNNN,					// 69	������������
	&CCreatureView::skill_NNNNNNNNNNNN,					// 70	��������������
	&CCreatureView::skill_NNNNNNNNNNNN,					// 71	�������������
	&CCreatureView::skill_NNNNNNNNNNNN,					// 72	��������������
	&CCreatureView::skill_NNNNNNNNNNNN,					// 73	����ֵ��������

// ���＼��
	&CCreatureView::skill_GenericSkill,					// 74	�����޵Ļ�������
	&CCreatureView::skill_SingleDir,					// 75	��Ӱ�Ļ�������

// ���＼��
	&CCreatureView::skill_GenericSkill,					// 76 	���弼�ܣ���ͨ���ܣ�
	&CCreatureView::skill_SingleDir,					// 77 	���弼�ܣ�����ϵ��
	&CCreatureView::skill_MultiUnitSingleTileSkill,		// 78	���弼�ܣ���ͬ��Ŀ��Ķ൥Ԫ������ħ����
	&CCreatureView::skill_SingleCirculDirSkill,			// 79	���弼�ܣ����Լ�Ϊ����8���򵥷����ܣ�

	&CCreatureView::skill_GenericSkill,					// 80 	��Χ���ܣ�һ���Եġ���Χ���Σ�����ͨ���ܣ�
	&CCreatureView::skill_SingleDir,					// 81 	��Χ���ܣ�һ���Եġ���Χ���Σ�������ϵ��
	&CCreatureView::skill_HalfCirculDirSkill,			// 82	��Χ���ܣ�һ���Եġ���Χ���Σ������Լ�Ϊ����8�������μ��ܣ�
	&CCreatureView::skill_CirculDirSkill,				// 83	��Χ���ܣ�һ���Եġ���Χ���Σ������Լ�Ϊ����8����Բ�μ��ܣ�

	&CCreatureView::skill_GenericSkill,					// 84 	��Χ���ܣ������Եģ���Χ���Σ�����ͨ���ܣ�
	&CCreatureView::skill_RainSkill,					// 85 	��Χ���ܣ������Եģ���Χ���Σ����Ե��淶Χ�Ķ൥Ԫ���������ͣ�
	&CCreatureView::skill_HalfCirculDirSkill,			// 86	��Χ���ܣ������Եģ���Χ���Σ������Լ�Ϊ����8�������μ��ܣ�
	&CCreatureView::skill_CirculDirSkill,				// 87	��Χ���ܣ������Եģ���Χ���Σ������Լ�Ϊ����8����Բ�μ��ܣ�

	&CCreatureView::skill_GenericSkill,					// 88	�����ٻ�����
	
	&CCreatureView::skill_SingleCirculDirSkill,			// 89	����ǰ��9��9��Բ�η�Χ����
	&CCreatureView::skill_SingleCirculDirSkill,			// 90	����ǰ��5��6��Բ�η�Χ����

// ����Ƭ��Ҽ���
	&CCreatureView::skill_GenericSkill,					// 91 	սʿ��Ⱥ��ѣ��
	&CCreatureView::skill_WanJianSkill,					// 92 	սʿ���򽣳���
	
	&CCreatureView::skill_GenericSkill,					// 93 	��ʦ����˪����
	&CCreatureView::skill_MultiDirSkillEx,				// 94 	��ʦ���������
	
	&CCreatureView::skill_GenericSkill,					// 95 	��ʦ������
	&CCreatureView::skill_RainSkill,					// 96 	��ʦ����������
	
	&CCreatureView::skill_GenericSkill,					// 97 	���ף�Ⱥ�循��
	&CCreatureView::skill_GenericSkill,					// 98 	���ף��Ա�����


//	skill_MultiDirSkillEx,				// 4	���һ���
//	skill_GenericSkill,					// 6	Ⱥ������
};

//------------------------------------------------------------------------
// �ռ��ܻ�û��
BOOL CCreatureView::skill_NNNNNNNNNNNN(SSkillCreateParam* pParam)
{
	return FALSE;
}
//------------------------------------------------------------------------
// ��ħ��Ч������
BOOL CCreatureView::skill_None(SSkillCreateParam* pParam)
{
	return TRUE;
}
//------------------------------------------------------------------------
// ��ͨ����
BOOL CCreatureView::skill_CommonAttack(SSkillCreateParam* pParam)
{
	return CSkillCreate::CreateCommonAttackSkill(pParam, this);
}
//------------------------------------------------------------------------
// ������м���(�����)
BOOL CCreatureView::skill_SingleDir(SSkillCreateParam* pParam)
{
	return CSkillCreate::CreateSingleDirSkill(pParam, this);
}
//------------------------------------------------------------------------
// �൥Ԫ�����ͼ��ܵĴ�������(�籩��ѩ)
BOOL CCreatureView::skill_RainSkill(SSkillCreateParam* pParam)
{
	return CSkillCreate::CreateRainSkill(pParam, this);
}
//------------------------------------------------------------------------
// �򽣳����༼�ܵĴ�������
BOOL CCreatureView::skill_WanJianSkill(SSkillCreateParam* pParam)
{
	return CSkillCreate::CreateWanJianSkill(pParam, this);
}
//------------------------------------------------------------------------
// ��ͬ��Ŀ��Ķ����Ԫħ�������������ȣ�
BOOL CCreatureView::skill_MultiUnitSingleTileSkill(SSkillCreateParam* pParam)
{
	return CSkillCreate::CreateMultiUnitSingleTileSkill(pParam, this);
}
//------------------------------------------------------------------------
// ������м���(�籩�һ���)
BOOL CCreatureView::skill_MultiDirSkillEx(SSkillCreateParam* pParam)
{
	return CSkillCreate::CreateMultiDirSkillEx(pParam, this);
}
//------------------------------------------------------------------------
// 8�����Բ�κ����μ��ܵĴ�������(�����湥��)
BOOL CCreatureView::skill_SingleCirculDirSkill(SSkillCreateParam* pParam)
{
	return CSkillCreate::CreateCirculDirSkill(pParam, this, 1);
}
//------------------------------------------------------------------------
// 8�����Բ�κ����μ��ܵĴ�������(�����湥��)
BOOL CCreatureView::skill_HalfCirculDirSkill(SSkillCreateParam* pParam)
{
	return CSkillCreate::CreateCirculDirSkill(pParam, this, 5);
}
//------------------------------------------------------------------------
// 8�����Բ�κ����μ��ܵĴ�������(�����湥��)
BOOL CCreatureView::skill_CirculDirSkill(SSkillCreateParam* pParam)
{
	return CSkillCreate::CreateCirculDirSkill(pParam, this, 8);
}
//------------------------------------------------------------------------
// ͨ�ü��ܴ���
BOOL CCreatureView::skill_GenericSkill(SSkillCreateParam* pParam)
{
	return CSkillCreate::CreateGenericSkill(pParam, this);
}
//------------------------------------------------------------------------
// ˳��ħ��
BOOL CCreatureView::skill_ChangeLoc(POINT pt)
{
	return CSkillCreate::CreateChangeLoc(pt, this);
}
//------------------------------------------------------------------------
// ��������ѡ���Ч
CMagic* CCreatureView::CreateSkillSelMagic(int nSkillID)
{
	return NULL;

	IResObject* pMagicObj = NULL;
	SMagicArchive_Mouse stRun;
	pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(32);
	if (!pMagicObj)
		return NULL;

	memset(&stRun, 0, sizeof(stRun));
	g_ResLoader.ReadMouseMagic(pMagicObj, &stRun);
	stRun.dwResID = 32;

	CMagicMouse *pMagic = new CMagicMouse;
	if(!pMagic->OnCreateItem((const char*)&stRun, sizeof(SMagicArchive_Mouse)))
	{
		SAFE_RELEASE(pMagic);
		return NULL;
	}
	pMagic->AddToMapView(m_ptTile);

	return pMagic;
}
//------------------------------------------------------------------------
/*
�Ʒ�ն
����
�����͵�
ŭ��
����
��ɨ�˷�
���
�ػ�
����
��ɱ��
ŭ�����
�ܱ�
����
��������
��ɱ
��Ѫ��ɱ
սʿ��̬�л�


������
�������
������
�ƻ�����
��Ӱ����
��������
���
���
��ӡ
����
����
����쳾
��ѩ����
����
�ػ���
����
��ʦ��̬�л�

�ϵ羪��
��Ȫ����
����Ԫ��
��������
����
����ħ
������???
���ֻش�
��������
��������
�������
������צ
����
����
Ѱ��
���ƶϻ�
��ʦ��̬�л�


ħ�޷���???
��Ӱ
ڤ��ʴ��
��Ѫ
�ε�����
����
��Ӱ����???
����???
����???
������״???
���ҿ�ħ???
�����׶�
ʴƤ����
����
�߻�����
������
������̬�л�
*/
//------------------------------------------------------------------------