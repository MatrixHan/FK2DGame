/**
*	created:		2012-7-1   19:59
*	filename: 		BasicCommon
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//------------------------------------------------------------------------
#ifdef _DEBUG
#define __USE_MALLOC
#endif
//------------------------------------------------------------------------
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <hash_map>
#include <list>
#include <vector>
#include <map>
#include "../../FKTrace/IFKTrace.h"
using std::map;
using std::vector;
using std::list;
using stdext::hash_map;
//------------------------------------------------------------------------
#include "../../FKGame/DataArchive/IDataArchive.h"
#include "../../FKGameModule/Include/AccountSpace.h"
#include "../../FKGame/MapView/IMapView.h"
#include "../../FKGame/MapView/MapItemView.h"
#include "../../FKGame/GameGlobal/IClientGlobal.h"
#include "../../FKGame/Sound/ISound.h"
#include "../../FKGame/Common/WinMsgDef.h"
#include "GlobalFunc.h"
//------------------------------------------------------------------------
#ifndef SAFE_DELETE
#	define SAFE_DELETE(p)		 {if(p){delete (p);(p)=NULL;}}
#endif
//------------------------------------------------------------------------
#ifndef SAFE_DELETE_ARRAY
#	define SAFE_DELETE_ARRAY(p) {if(p){delete [](p);(p)=NULL;}}
#endif
//------------------------------------------------------------------------
#ifndef SAFE_RELEASE
#	ifdef _DEBUG
#		define SAFE_RELEASE(p)      {if(p){(p)->Release();(p)=NULL;}}
#	else
#		define SAFE_RELEASE(p)      {if(p){try{(p)->Release();}catch(...){};(p)=NULL;}}
#	endif
#endif
//------------------------------------------------------------------------

#define _TraceOnce(staticflag, isMsgBox, szErrorInfo) \
	static int staticflag = 0; \
	if (staticflag == 0) \
{ \
	staticflag = 1; \
	if (isMsgBox) \
	::MessageBox(NULL, szErrorInfo, "����", MB_OK); \
	else \
	PrintEventLn(szErrorInfo); \
}

//------------------------------------------------------------------------
typedef POINT	TILE;
//------------------------------------------------------------------------
#define TRANS_VALUE				50 //(96~127,50% alpha)
#define MAX_ITEM_NAME_LENGTH	36 // item ������ֳ���
#define	MAX_PATH_LENGTH			32
//------------------------------------------------------------------------
#ifdef	g_DefaultTrace
#undef	g_DefaultTrace
#endif

#define g_DefaultTrace			gp->m_TraceHelper
#define g_pResMgr				gp->m_ResMgrHelper.m_pResMgr
#define g_fsHelper				gp->m_FileSystemHelper
#define g_CanvasHelper			((IClientGlobal*)gp)->m_CanvasHelper
#define g_pMapView				((IClientGlobal*)gp)->m_pMapView
#define g_pTimeAxis				gp->m_TimerAxisHelper.m_pTimeAxis
#define g_pFont					((IClientGlobal*)gp)->m_pDefFont
#define g_pFontWithOutline		((IClientGlobal*)gp)->m_pDefFontWithOutline
#define g_pSound				((IClientGlobal*)gp)->GetSoundEngine()
#define g_pPlayer				((IClientGlobal*)gp)->m_pPlayer

#define Tile_To_Screen	g_pMapView->TileToScreen
#define Screen_To_Tile	g_pMapView->ScreenToTile
#define Tile_To_World	g_pMapView->TileToWorld
#define World_To_Tile	g_pMapView->WorldToTile
//------------------------------------------------------------------------
extern CCriticalSectionLock	g_AniFileLock;
extern CCriticalSectionLock	g_AniGroupFileLock;
//------------------------------------------------------------------------
// ȫ�ֱ���
extern CAccountSpace<1024, 1024, 1, 1> g_space;
extern IClientGlobal* gp;
//------------------------------------------------------------------------
enum ThreeState
{
	stateCreate = 0,
	stateLoop,
	stateRelease
};
enum AniType 
{
	anitypeSkillAni = 0,			// ���ܶ���
	anitypeGenericAni,				// ͨ�ö���
	anitypeAppearanceAni,			// �������
	anitypeHeadAni,					// ͷ����Ч
};
enum GenericAni 
{
	aniBorn = 0,					// ����			
	aniUpgrade,						// ����
	aniBeLock_Red,					// ��ѡ�У���Ȧ��
	aniBeLock_Green,				// ��ѡ�У���Ȧ��
	aniBeLock_Yellow,				// ��ѡ�У���Ȧ��
	aniBeLock_Blue,					// ��ѡ�У���Ȧ��
	aniMouse,						// ���������

	aniFootprint,
};
enum HeadAni 
{
	aniTaskType_Get_Can = 0,		// С����-��		�������񣬲��������������	
	aniTaskType_Get_NoCanGet,		// С����-��		�������񣬵�δ�������������
	aniTaskType_Give_Complete,		// �պ�С����-��	���пɽ������񣬲�����ɣ�
	aniTaskType_Give_NoComplete,	// �պ�С����-��	���пɽ������񣬵���δ��ɣ�	

	aniNpcTypeTrans,				// ħ����	������Ա��	
	aniNpcTypeSkill,				// ħ����	������ѧϰʦ��
	aniNpcTypeMedicinal,			// ��ҩƿ	��ҩƷ�̣�
	aniNpcTypeWeapon,				// ����		�������̣�
	aniNpcTypeArmor,				// ����		��װ���̣�
	aniNpcTypeRide,					// ��		������̣�
	aniNpcTypeSmith,				// ����		��������
	aniNpcTypeGrocer,				// ����		���ӻ��̣�
};
enum DrawOrder 
{
	doFirstDrawAni = 0,
	doFirstDrawCrt
};
enum enNumType
{
	enNUM_TYPE_YELLOW = 0,		// ��ɫ�� ������Լ���������������˺���
	enNUM_TYPE_ORANGE,			// ��ɫ�� ����������֮������˺���
	enNUM_TYPE_RED,				// ��ɫ�� ���������������Լ�����˺���
	enNUM_TYPE_POWDER,			// ��ɫ�� ������������������˺���
	enNUM_TYPE_GREEN,			// ��ɫ�� ���ָ�������
	
	enNUM_TYPE_MAX_NUM,			// ��������Ŀ
};

enum enLoadState
{
	enLOAD_NONE = 0,	// δ��ȡ
	enLOAD_ING,			// ���ڶ�ȡ
	enLOAD_OVER			// ��ȡ���
};
//------------------------------------------------------------------------