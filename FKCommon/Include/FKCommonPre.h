/********************************************************************
*
*	���ļ���FKMMORPG��һ���֡����ļ���Ȩ����FreeKnightDuzhi(������)���С�
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	�ļ�����	FKCommonPre
*	���ߣ�		FreeKnightDuzhi[ ������ ]
*	�ļ�˵����	
*
*	�޸���:		
*	�޸�����:
*********************************************************************/

#pragma once

//-------------------------------------------------------------------
#include "FKPlatform.h"
#include "FKStdHeader.h"
#include "FKInsideMacro.h"
#include "FKTypedef.h"
#include "FKRecursiveMutex.h"
//-------------------------------------------------------------------
namespace FKCommon
{
	#define FK_COMMON_MAHOR_VERSION 1
	#define FK_COMMON_MINOR_VERSION 0
	#define FK_COMMON_PATCH_VERSION 1
	#define FK_COMMON_VERSION (( FK_COMMON_MAHOR_VERSION << 16 ) | ( FK_COMMON_MINOR_VERSION << 8 )| ( FK_COMMON_PATCH_VERSION ) )
}
//-------------------------------------------------------------------