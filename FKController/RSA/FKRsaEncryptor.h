/**
*	created:		2012-2-29   4:53
*	filename: 		FKRsaEncryptor
*	author:			FreeKnight
*	Copyright (C): 	TianCong (2012) FreeKnightServerEngine
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "FKBigInt.h"
#include <string>
//------------------------------------------------------------------------
class FKRsaEncryptor
{
public:
	// ������������
	static bool Init( char* p_szSeed );
	// ��������
	static CBigInt GetEncryptResult();
	// �ж������Ƿ�Ϸ�
	static bool	IsValidEncrypt( CBigInt& p_EncryptCode );
	// ��������ַ���
	static void RandStr( char* p_szOutStr, const int p_nLen = BI_MAXLEN );
};