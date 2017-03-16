/**
*	created:		2012-6-20   23:55
*	filename: 		Macro
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
#define FK_TEST
//------------------------------------------------------------------------
#ifdef FK_TEST
#define _USE_BINSCRIPT
// #define _USE_FSP
#endif
//------------------------------------------------------------------------
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) if(p) { (p)->Release(); (p) = NULL; }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) if(p) { delete (p); (p) = NULL; }
#endif
//------------------------------------------------------------------------
#ifndef ASSERT
#define ASSERT(x)	{if(!(x))__asm int 3}
#endif
//------------------------------------------------------------------------
#define _D3D_ON_
//------------------------------------------------------------------------
// ��������
#define WIN_CLASS_NAME	"FreeKnightGameClass"
// ������
#define WIN_TITLE_NAME	"��ʧ֮�� Ver1.0"
// ���ڿ��
#define GAME_WINDOW_WIDTH	1024
// ���ڸ߶�
#define GAME_WINDOW_HEIGHT	768
#define	VIEW_WIDTH			1024
#define	VIEW_HEIGHT			768
//------------------------------------------------------------------------
#ifndef PrintEventLn
	#ifdef _DEBUG
	#define PrintEventLn g_DefaultTrace.ThreadSafeDebugPrintLn
	#else
	#define PrintEventLn g_DefaultTrace.ThreadSafeDebugPrintLn
	#endif
#endif
#ifdef TraceLn
#undef TraceLn
#endif
#define TraceLn PrintEventLn
#define SimpleTraceLn g_DefaultTrace.DebugPrintLnWithTime
//------------------------------------------------------------------------
#define XCRASHREPORT_MINI_DUMP_FILE		"Report.dmp"
#define XCRASHREPORT_ERROR_LOG_FILE		"Errorlog.txt"
#define XCRASHREPORT_TRACE_FILE			"Trace.txt"
//------------------------------------------------------------------------
#ifdef _DEBUG
	#define XCRASHREPORT_CRASH_REPORT_APP	"ReportD.dll"
	#else
	#define XCRASHREPORT_CRASH_REPORT_APP	"Report.dll"
#endif
//------------------------------------------------------------------------
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif
//------------------------------------------------------------------------
#ifndef IN
	#define IN
#endif
#ifndef OUT
	#define OUT
#endif
//------------------------------------------------------------------------
#define MAX(i, j) ((i < j) ? j : i)
#define MIN(i, j) (( i < j) ? i : j)
//------------------------------------------------------------------------
#define STARTPROFILE( CHECKPOINT )	CProfileManager::Start_Profile(CHECKPOINT);{
#define STOPPROFILE					}CProfileManager::Stop_Profile();
//------------------------------------------------------------------------
#define INVALID_SOCKET_ID			(-1)
#define MAX_SOCKET_DATA_LEN			(16 * 1024)						// �û��������ߴ磬�����ٴ��ˣ��Ǻ�
#define MAX_SOCKET_RAW_DATA_LEN		(MAX_SOCKET_DATA_LEN + 256)		// �ײ�ͨѶ����󳤶ȣ����ಿ�����������ͷ��
#define MAX_SOCKET_BUF_LEN			MAX_SOCKET_RAW_DATA_LEN	
#define MAX_CLIENT_SOCKET			63								// IThreadRoute������ͬʱ����63��ͨѶ�����¼����������˳�����ܻ���Ҫ�����¼�������ͬʱ֧�ֵ����ӻ����
#define MAX_NAME_STRING_LEN			32								// �û��������롢��������������õ������
//------------------------------------------------------------------------
// UID����
typedef  __int64 UID;
struct _UID
{
	DWORD	dwRotatoryID;
	WORD	wTypeID;
	WORD	wServerID;
};
//------------------------------------------------------------------------
// ��ЧUID
#define	INVALID_UID					((UID)0xFFFFFFFFFFFFFFFF)
// ��Ч�ĵ�ͼID
#define INVALID_MAPID				0
// UID����
#define GETUIDROTATORYID(uid)		(((_UID&)uid).dwRotatoryID)
#define GETUIDTYPE(uid)				(((_UID&)uid).wTypeID)
#define GETUIDSERVERID(uid)			(((_UID&)uid).wServerID)
// ��߼��������ֳ���
#define MAX_NAME_LEN				32
// ���½���볤��
#define MAX_LOGINPASSWORD_LEN		16
// ��Ч���߼���������ID�����������߼�����
#define INVALID_PROPID				-1
// ���·������
#define MAX_PATH_LEN				256
// �㲥��Χ�Ŀ��
#define BROADCAST_AREA_WIDTH		20
// �㲥��Χ�ĸ߶�
#define BROADCAST_AREA_HEIGHT		20
// һ����Ļ��TILE��
#define SCREEN_TILE					16
// ������NPC�Ի������TILE��
#define MEETNPC_SPACE_TILE			8
// �ƶ����ٶȵȼ�
#define MAX_MOVESPEED_LEVEL			9
// ��ͨ�����������ȼ�
#define MAX_ATTACKSPEED_LEVEL		9
//------------------------------------------------------------------------
#define	 MOVE_DELTA				40
//------------------------------------------------------------------------
#define	 TICK_BOTTOM			16	// ���ˢ����
#define  TICK_INACTIVE			100 // ��ʧ������ˢ����
#define  TICK_TOP				30	// ���ˢ����
#define  TICK_WAIT_LOGIC		49
#define  TICK_TIMES				16
//------------------------------------------------------------------------
#define USER_NAME_MAX_LEN		32
#define PASSWORD_MAX_LEN		32
//------------------------------------------------------------------------
#define LOG_PATH				"%s\\Log"
#define TRACE_LOG_FILE			"Log\\trace.txt"
#define GAME_CONFIG_FILE		"Data\\Config\\FKGameConfig.ini"
#define DEBUG_RES_LIST_FILE		"Data\\scp\\Window.scp"
#define RELEASE_RES_LIST_FILE	"Data\\scp\\Window.res"
#define DEBUG_FILESYSTEM_DIR	"Data\\pic"
#define RELEASE_FILESYSTEM_FILE "Data\\pic\\pic.fsp"
#define	OCCUPANT_BITMAP_FILE	"%s\\Data\\Res\\flags.bmp"
#define CLIENT_VERSION_FILE		"ClientVersion.FKP"
#define EVENT_RES_FILE			"%s\\Data\\Scp\\Event.scp"
#define MAP_RES_DIR				"%s/Data/map"
//------------------------------------------------------------------------