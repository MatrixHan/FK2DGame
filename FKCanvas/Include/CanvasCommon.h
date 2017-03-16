/**
*	created:		2012-6-19   13:03
*	filename: 		CanvasCommon
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <windows.h>
#include "../../FKTrace/IFKTrace.h"
//------------------------------------------------------------------------
#ifndef _DEBUG
	#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#else
	#define SAFE_RELEASE(p)      { if(p) { try {(p)->Release();}catch(...){}; (p)=NULL; } }
#endif
//------------------------------------------------------------------------
#define SAFE_DELETE(p)		 { if(p) { delete (p); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
//------------------------------------------------------------------------
#ifndef ASSERT
	#include <assert.h>
	#define ASSERT assert
#endif

#ifndef FKTraceLn
	#ifdef _DEBUG
	#define FKTraceLn	g_DefaultTrace.DebugPrintLn
	#else
	#define FKTraceLn	__noop
	#endif
#endif
#ifndef FKTrace
	#ifdef _DEBUG
	#define FKTrace		g_DefaultTrace.DebugPrint
	#else
	#define FKTrace		__noop
	#endif
#endif
//------------------------------------------------------------------------
#ifndef NULL
#define NULL 0
#endif
//------------------------------------------------------------------------
template<class T> inline T MOD (T i, T j) { return (i<0)?(j-1)-((-i-1)%j):i%j; }
template<class T> inline T DIV (T i, T j) { return (i<0)?-((-i-1)/j)-1:i/j; }
template<class T> inline T ABS (T i) { return (i>0)?i:-i; }
template<class T> inline void SWAP(T &x, T &y) { T t; t=x,x=y,y=t; }
//------------------------------------------------------------------------
#define MAX(i, j) ((i < j) ? j : i)
#define MIN(i, j) (( i < j) ? i : j)
//------------------------------------------------------------------------
//���Ծ����Ƿ�����һ��������
#define RectInRect(small, big) (PtInRect(big, ((POINT*)small)[0]) && \
	PtInRect(big, ((POINT*)small)[1]) || *small == *big)
//����һ�������Ƿ�����һ������֮��
#define RectOutRect(small, big) (small.left >= big.right || small.top >= big.bottom || \
	small.right < big.left || small.bottom < big.top)
//�ж����������Ƿ����
inline BOOL operator ==(RECT& left, RECT& right)
{
	return (left.left == right.left &&
		left.top == right.top &&
		left.right == right.right &&
		left.bottom == left.bottom);

}
//------------------------------------------------------------------------