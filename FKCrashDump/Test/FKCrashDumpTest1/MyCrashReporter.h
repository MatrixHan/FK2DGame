/**
*	created:		2012-6-14   20:14
*	filename: 		MyCrashReporter
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../../IFKCrashDumpHelper.h"
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
extern CCrashReport g_CrashReport;
//------------------------------------------------------------------------