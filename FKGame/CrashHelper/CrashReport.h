/**
*	created:		2012-6-21   0:05
*	filename: 		CrashReport
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../PreInclude.h"
//------------------------------------------------------------------------
class CCrashReport : public ICrashDumpSink
{
public:
	// ��ʼdump��Ϣ������������----ʵ��������������ô��󱨸�����
	virtual void	OnDumpStart(ICrashDump *);
	// ������ICrashDump��DumpLn Dump�������LogFile����Ϣ
	virtual void	OnDump(ICrashDump*,PEXCEPTION_POINTERS);
	// Dump��������ô˺������˺�������ֵ�ᱻ�����쳣����ķ���ֵ
	virtual LONG	OnDumpFinish(ICrashDump*,PEXCEPTION_POINTERS);
};
//------------------------------------------------------------------------