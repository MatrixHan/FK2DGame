/**
*	created:		2012-6-24   22:34
*	filename: 		AttackerThread
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <windows.h>
//------------------------------------------------------------------------
DWORD WINAPI AttackerThreadProc(LPVOID lpParameter);
//����һ�����е�����, �����Ƿ�ɹ�
BOOL PhraseData(LPCSTR pData, int &id);
//------------------------------------------------------------------------
