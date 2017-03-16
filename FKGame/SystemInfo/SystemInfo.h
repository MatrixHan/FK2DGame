/**
*	created:		2012-6-20   23:07
*	filename: 		SystemInfo
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN
//------------------------------------------------------------------------
// �汾��
#define FK_SYSTEMINFO_VERSION		1.01
//------------------------------------------------------------------------
#define SI_BYTES		1
#define SI_KILOBYTES	1024
#define SI_MEGABYTES	(1024 * 1024)
//------------------------------------------------------------------------
// OS���ƶ���
#define SI_OS_NT		0
#define SI_OS_2K		1
#define SI_OS_XP		2
#define SI_OS_DOTNET	3
#define SI_OS_95		4
#define SI_OS_98		5
#define SI_OS_ME		6
#define SI_OS_LINUX		7
#define SI_OS_FREEBSD	8
#define SI_OS_UNKNOWN	9
//------------------------------------------------------------------------
// �ڲ�����
typedef char* siString;
typedef int siSize;
typedef unsigned int siInt;
//------------------------------------------------------------------------
// OS�汾��Ϣ
typedef struct
{
	int major;
	int minor;
	int revision;
}siVersion;
//------------------------------------------------------------------------
// CPU������Ϣ
typedef struct
{
	char hasMMX;
	char hasSSE;
	char hasSSE2;
	char has3dNow;
	char idString[16];
}siCpu;
//------------------------------------------------------------------------
// OS���Ʊ�
extern siString siOsNames[];
//------------------------------------------------------------------------
siInt		siGetCpuSpeed(void);
siCpu		siGetCpuType(void);
siSize		siGetPhysicalMem(int units);
siSize		siGetAvailableMem(int units);
siVersion	siGetOsVersion(void);
siString	siGetOsName(void);
siInt		siGetOsType(void);
const char* siGetLastError(void);
//------------------------------------------------------------------------
