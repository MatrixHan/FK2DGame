/**
*	created:		2012-6-24   2:27
*	filename: 		TimeAxis
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <set>
#include <list>
#include <functional>
#include "../../FKGame/Timer/ITimerAxis.h"
using std::list;
//------------------------------------------------------------------------
#pragma comment( lib,"winmm.lib" )
//------------------------------------------------------------------------
#define		TIMER_LONG_ELAPSE			120
#define		TIMER_GIID					1000
#define     TIMER_UNIT					5
#define		TIMER_DESC_LEN				64
//------------------------------------------------------------------------
struct TIMERKEY 
{
	ITimerSink *pTimerSink;
	DWORD dwEventID;
//	Timer����,�����ñ��ڵ��Դ���
#ifdef _DEBUG
	char szTimerDesc[TIMER_DESC_LEN];

	TIMERKEY()
	{
		szTimerDesc[0]=0;
	}
#endif
};
//------------------------------------------------------------------------
struct TIMERDATA
{
	TIMERKEY TimerKey;
	DWORD dwLastTickCount;
	DWORD dwElapseTime;
	BOOL IsOverTime(DWORD dwTickCount)
	{
		return dwTickCount>=dwLastTickCount+dwElapseTime;
	}
};
//------------------------------------------------------------------------
struct TIMERPOS
{
	BOOL bIsShort;
	//char PosBuf[sizeof(_Rb_tree_iterator<TIMERDATA,TIMERDATA const &,TIMERDATA const*>)];
	char PosBuf[sizeof(int)];
	DWORD dwShortTimerIndex;
	TIMERKEY TimerKey;
};
//------------------------------------------------------------------------
template<>
struct std::tr1::hash<TIMERKEY>
{
	size_t operator()(TIMERKEY TimerKey) const 
	{
		return size_t(LOWORD((DWORD)(TimerKey.pTimerSink))+TimerKey.dwEventID);
	}
};
//------------------------------------------------------------------------
//template<>
//struct std::identity<TIMERPOS>
//{
//	const TIMERKEY& operator()(const TIMERPOS& TimerPos) const 
//	{
//		return TimerPos.TimerKey;
//	}
//};
//------------------------------------------------------------------------
template<>
struct std::equal_to<TIMERKEY>
{
	bool operator()(const TIMERKEY& K1, const TIMERKEY& K2 ) const 
	{
		return (K1.dwEventID==K2.dwEventID&&K1.pTimerSink==K2.pTimerSink);		
	}
};
//------------------------------------------------------------------------
extern DWORD g_dwTimeLine;
//------------------------------------------------------------------------
template<>
struct std::less<TIMERDATA>
{
	bool operator()(const TIMERDATA& d1, const TIMERDATA& d2 ) const 
	{
		return ((d1.dwLastTickCount+d1.dwElapseTime)-g_dwTimeLine)<((d2.dwLastTickCount+d2.dwElapseTime)-g_dwTimeLine);
	}
};
//------------------------------------------------------------------------
// Ĭ�ϵ�insert�������ظ�,�ػ�һ��
//typedef std::set<TIMERDATA,std::less<TIMERDATA>,alloc >  LONG_TIMER_LIST;
//pair<LONG_TIMER_LIST::iterator,bool> LONG_TIMER_LIST::insert(const TIMERDATA & __x )
//{
//	LONG_TIMER_LIST::iterator it = _M_t.insert_equal( __x );
//	return pair<LONG_TIMER_LIST::iterator, bool>(it,true);
//}
//------------------------------------------------------------------------
class CTimeAxis  : public ITimeAxis
{
public:
	// ��ʱ���ʱ����
	virtual void	OnCheck(void); 
	// ���ö�ʱ�������ڵ��Լ���һ�������ַ�
	virtual BOOL	SetTimer(DWORD dwEventID, ITimerSink * pTimerSink, DWORD dwElapse, LPCSTR szTimerDesc = NULL);
	// ���ٶ�ʱ��
	virtual void	KillTimer(DWORD dwEventID, ITimerSink * pTimerSink);
	// ���Է��ؾ��������ٴ���ĵ�ǰʱ��Tick��
	virtual DWORD	GetTimeTick(void);
	// ����ʱ����
	virtual void	Release(void);

	BOOL			_SetTimer(DWORD dwEventID, ITimerSink *pTimerSink, DWORD dwElapse);
	void			InitTimerList(void);

	CTimeAxis();
	virtual ~CTimeAxis();

private:
	void OnCheckAux(DWORD dwTickCount);

private:
	int m_nCurShortTimer;
	DWORD m_dwShortArrayLimit[TIMER_LONG_ELAPSE];
	DWORD m_dwShortTimerCount[TIMER_LONG_ELAPSE];
	DWORD m_dwShortTimerVaildCount[TIMER_LONG_ELAPSE];
	BOOL m_bInitTimerList;
	BOOL m_dwLastCheckTime;

	//hashtable<TIMERPOS,TIMERKEY,hash<TIMERKEY>,identity<TIMERPOS>,equal_to<TIMERKEY>,alloc> m_TimerPosTable;
	//typedef hashtable<TIMERPOS,TIMERKEY,hash<TIMERKEY>,identity<TIMERPOS>,equal_to<TIMERKEY>,alloc>::iterator HASHTABLE_ITERATOR;
	//set<TIMERDATA,less<TIMERDATA>,alloc> m_LongElapseTimerList;
	//typedef set<TIMERDATA,less<TIMERDATA>,alloc>::iterator LONG_TIMER_ITERATOR;
	list<TIMERDATA> m_ShortElapseTimerArray[TIMER_LONG_ELAPSE];
	typedef list<TIMERDATA>::iterator SHORT_TIMER_ITERATOR;
};
//------------------------------------------------------------------------
