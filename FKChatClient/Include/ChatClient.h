/**
*	created:		2012-6-28   2:29
*	filename: 		ChatClient
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ģ��
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <string>
#include "../../FKTrace/IFKTrace.h"
#include "../../FKGame/GameGlobal/IClientGlobal.h"
#include "../../FKGame/WndSys/WndSys.h"
#include "../IChatClient.h"
#include "../../FKGame/Controller/IController.h"
#include "../IPaoPao.h"
using std::string;
//------------------------------------------------------------------------
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  {if(p) { delete p;p = NULL; }}
#endif
//------------------------------------------------------------------------
extern CFKTrace					g_DefaultTrace;
extern CWndSysHelper			g_wndHelper;
extern IClientGlobal *			g_pGlobal;
//------------------------------------------------------------------------
#include "ChatController.h"
#include "OutputWindow.h"
#include "InfoWindow.h"
#include "ChatMenu.h"
#include "BulletinWnd.h"
#include "SysMsgReceiver.h"
#include "PromptWnd.h"
#include "InvalidThingMgr.h"
//------------------------------------------------------------------------
#define OUTPUTWINDOW_ID		451		// ������Ϣ��ʾ����
#define INFOWINDOW_ID		452		// ������ʾ����
#define PROMPTWINDOW_ID		453		// ϵͳ��ʾ����
#define BULLETINWINDOW_ID	454		// ϵͳ���洰��
#define HELPINFOWINDOW_ID	455		// ������ʾ��
#define CHAT_MENU_WND_ID    472
#define FRIEND_WND			470
#define PROPERTY_WND		473
//------------------------------------------------------------------------
// �˵���
struct SMenuItem
{
	int	nId;                 // �˵�ID
	const char * pszName;    // �˵�����
};
//------------------------------------------------------------------------
// �˵���ID����
enum CHAT_MENU_ITEM
{
	CHAT_PRIVATE = 100,      // ���Ļ�
	CHAT_TEAM ,              // �������
	CHAT_FRIEND, 			 // ��Ϊ����
	CHAT_INFO				 // ������Ϣ	
};
//------------------------------------------------------------------------
// �ӳ���������ҵĲ˵�
const SMenuItem g_menu[] = 
{
	{CHAT_PRIVATE,	"���Ļ�"	},
	{CHAT_TEAM,		"�������"	},
	{CHAT_FRIEND,	"��Ϊ����"  },
	{CHAT_INFO,		"������Ϣ"  },
	0	
};
//------------------------------------------------------------------------
// ����ģ��
class CChatClient : public IChatClient, 
                    public IDataReceiver, 
					public IEventActionListener,
					public IMenuEventReceiver
{
	friend class CChatController;
	friend class CChatItemFactory;
	friend class CReminder;
	friend class XItemPlayer;
	friend class XItemSentence;
	friend class XItemThing;
	friend class CPromptWnd;
	friend class CReminder;
	friend class CChatParser;
	
protected:
	CChatClient(void);
	virtual ~CChatClient(void);

public:
	static CChatClient * Instance(void);
	BOOL Create(IClientGlobal * pGlobal);
	virtual void Release(void);
	virtual BOOL OnReceived(const char * pData, int nLen);
	virtual void SetWindowTrans(int nLevel);
	virtual void ShowWindow(int nType);
	virtual void ChannelOn(int nChannel, BOOL bOnOff);
	virtual BOOL Speak(int nChannel, const char * pszReceiver, const char * pszText);
	virtual void DisplaySysMessage(const char * pszText);
	virtual void OnAction(IEvent *pEvent);
	virtual IPaoPao * CreatePaoPao(UID uid);
	virtual void OnSysMsgDraw(ICanvas * pCanvas);
	virtual void DisplayFloatMessage(int nType, const char * lpszText);
	virtual void ClearFloatMessage(int nType);
	ITimeAxis * GetTimeAxis(void);
	IActor * GetPlayer(UID uidPlayer);
	ICreature * GetCreature(UID uidCreature);
	void PingStart(void);
	DWORD PingStop(void);
	virtual void Clicked(int nId);
    void SetMenu(const SMenuItem menu[]);
	void ShowMenu(POINT pt,LPCSTR lpszPlayer);
	void ShowTitleWnd(BOOL bTrue,BOOL bTurn = TRUE);					// ��ʾ�ƺŴ���
	int GetDistance(UID uid);											// ��ȡ���֮��ľ���
     
	IThing *GetThing(UID uid);
	IThing *CreateGoods(char *buf,int nLen,UID uid);
public:
	CPromptWnd *			m_pHelpInfoWnd;
private:	
	BOOL m_bCreated;
	IClientGlobal *			m_pGlobal;	
	COutputWindow *			m_pOutputWindow;
	CInfoWindow *			m_pInfoWindow;
	IResScript *			m_pResScript;
	CChatController			m_controller;
	CSysMsgReceiver			m_sysMsgReceiver;
	DWORD					m_dwPingStartTime;
	CBulletinWnd *			m_pBulletinWnd;
	CChatMenu *				m_pMenu;
	CInvalidThingMgr		m_InvlidThingMgr;
	string					m_strMenu;
private:
	static CChatClient *	s_pModule;
};
//------------------------------------------------------------------------