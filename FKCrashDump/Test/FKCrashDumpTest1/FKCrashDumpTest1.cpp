// FKCrashDumpTest1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;
//------------------------------------------------------------------------
// �Ƿ������׷��Զ���DUMP
#define SIMPLE_DUMP
//------------------------------------------------------------------------
#include "../../IFKCrashDumpHelper.h"
//------------------------------------------------------------------------
#ifndef SIMPLE_DUMP
#include "MyCrashReporter.h"
#endif
//------------------------------------------------------------------------
CCrashDumpHelper g_CrashDumpHelper;
//------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	if( g_CrashDumpHelper.IsValid() == FALSE )
	{
		// ��ʼ��ʧ�ܣ��ܿ��ܶ�ȡ��̬���ӿ�ʧ��
		cout << "��ʼ�� CrashDump ʧ�ܣ����鶯̬���ӿ��Ƿ���ڻ�������" << endl;
		getchar();
		return -1;
	}

#ifndef SIMPLE_DUMP
	if( g_CrashDumpHelper.IsValid() )
	{
		g_CrashDumpHelper->AttachCrashDumpSink(&g_CrashReport);
	}
#endif


	int* p = 0;
	*p = 0;


#ifndef SIMPLE_DUMP
	if( g_CrashDumpHelper.IsValid() )
	{
		g_CrashDumpHelper->DetachCrashDumpSink(&g_CrashReport);
	}
#endif
	return 0;
}
//------------------------------------------------------------------------