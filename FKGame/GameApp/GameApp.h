/**
*	created:		2012-6-21   0:44
*	filename: 		GameApp
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "PreInclude.h"
#include "IGameApp.h"
#include "../../FKFileSystem/Include/FKTempl.h"
//------------------------------------------------------------------------
extern BOOL					g_bActive;
extern DWORD				g_dwDxVer;
extern DWORD				TICK_NEED;
extern DWORD				TICK_VAR;
extern BOOL					g_bMouseThreadRun;
extern CCriticalSectionLock	g_DrawMouseLock;
extern void					TraceSysInfo();
//------------------------------------------------------------------------
class CTimerAxisTunner : public ITimerSink
{
public:
	void			Start(void);
private:	
	virtual void	OnTimer(DWORD dwEventID);
private:
	DWORD			m_dwStart;
};
//------------------------------------------------------------------------
struct RenderStruct
{
	DWORD			m_dwTick;					// ��ǰÿ֮֡��ļ��
	DWORD			m_dwTimes;					// ������Ϸ�������--�����ںû�����ģ���ƻ���
	DWORD			m_dwError;					// һ������������
	DWORD			m_dwRenderTime;				// ����ÿ֡����Ҫ��ʱ��
	DWORD			m_dwOnCheckTime;			// OnCheck����Ҫ��ʱ��
	float			m_fUpdateToScreenTime;		// �����ƺõĻ���Update����ʾ����ʱ��
	DWORD			m_dwRenderTimes;			// ����Ϸ���������ڣ��ܹ����Ƶ�֡��
	DWORD			m_dwWholeFps;				// ����Ϸ���������ڵ�ƽ��FPS
	float			m_fFps;						// �չ�ȥ��1���ӵ�FPS
	DWORD			m_dwTime;					// ����Ϸ���������ڣ������˶���ʱ��
	char			m_strState[80];				// ��ҵ�ǰ״̬
};
//------------------------------------------------------------------------
class CCursorDraw
{
public:
	void			Create(ITrace * pTrace);
	void			Release(void);
	void			SaveScreen(ICanvas * pScreen);
	void			DrawCursorThread();
	void			DrawCursorLoop(ICanvas * pCanvas);
	void			UpdateToScreen(ICanvas * pCanvas);
	void			SetUpdateScreen(BOOL bFlag);

	CCursorDraw(void);
	~CCursorDraw(void);
private:
	void			ReleaseCursorBuffer(void);
	void			CreateCursorBuffer(ITrace *pTrace);	
public:
	ICanvas*		m_pScreenSaveCanvas;
	IBitmap*		m_pCursorBackSave;
	RECT			m_rcCursorBackSave;
	POINT			m_ptOldCursor;
	BOOL			m_bReady;
	BOOL			m_bUpdateScreenFlag;	
};
//------------------------------------------------------------------------
class CGameApp : public IGameApp  
{
public:
	virtual BOOL		InitGame(HINSTANCE hInstance = NULL, HWND hWnd = NULL, ICanvas* pScreen = NULL,ITrace* pTrace = NULL, void* pParam1 = NULL, void*pParam2 = NULL);
	virtual BOOL		Loop(void);
	virtual BOOL		PreTranslateMessage(MSG * pMsg);
	virtual LRESULT		OnCommand(DWORD dwCmd, DWORD dwParam1, DWORD dwParam2);
	void				OnMouseMove(DWORD wParam, DWORD lParam);
	BOOL				ScrolltoCenter(void);
	void				Update(BOOL bAddWhole = TRUE);
	float				GetFPS(void);
	void				ReadVersion(void);

	//������Ϣ֧��
	void				RollMsg(void);
	char*				RollMsgPing(void);
	char*				RollMsgVer(void);
	char*				RollMsgServerInfo(void);
	char*				RollMsgTime(void);
	char*				RollMsgCard(void);

	//���ְ���
	void				NewbieHelp(void);

	void				OnCheck(void);
	void				CursorLoop(void);
	void				CursorDrawLoop(void);

	typedef void		(CGameApp::*CalcFunc)();
	void				CalcTime(CalcFunc fn, BOOL bDWORD, DWORD* pdwDword, float* pfFloat, DWORD dwTimes, DWORD dwCallTimes = 1);
	void				ReleaseScreenBuffer(void);
	void				CreateScreenBuffer(ITrace *pTrace);
	void				Close(void);
	void				SetUpdateScreen(BOOL bFlag);
public:
	CGameApp(void);
	virtual ~CGameApp(void);
public:
	RenderStruct		m_rs;
	CCursorDraw			m_cd;
	DWORD				m_dwPing;
	BOOL				m_bQuit;
private:
	int					m_nMoveDelta;
	BOOL				m_bStand;	
	ICanvas*			m_pScreen;
	ICanvas*			m_pScreenBack;
	IBitmap*			m_pOccupantBitmap;
	HINSTANCE			m_hInstance;
	char				m_szVersion[512];
	char				m_szServerInfo[512];
	char				m_szCardInfo[512];
	WPixel				m_colorCardInfo;	
};
//------------------------------------------------------------------------