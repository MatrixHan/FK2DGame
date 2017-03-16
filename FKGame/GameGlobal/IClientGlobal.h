/**
*	created:		2012-6-21   0:20
*	filename: 		IClientGlobal
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../Timer/TimerAxisHelper.h"
#include "../WndSys/IWndMgr.h"
#include "../SceneObj/IActor.h"
#include "../../FKTrace/IFKTrace.h"
#include "../../FKFileSystem/IFKFileSystem.h"
#include "../../FKResManager/IFKResManager.h"
//------------------------------------------------------------------------
struct ICanvas;
struct IMapView;
struct IAppNetworkLink;
struct IGameApp;
struct IConfigDataCenter;
struct IItemClassFactory;
struct IClientWorld;
struct IThingFactory;
struct IKeywordFilter;
struct ITeamClient;
struct IEffectFactory;
class IEventServer;
struct IController;
struct IChatClient;
struct IFriendClient;
struct ISound;
struct IScriptUpdateManager;
//------------------------------------------------------------------------
// ����
struct IClientGlobal
{
public:
	// ����Ŀ¼
	char							m_szWorkDir[MAX_PATH];
	// ��½ʱ��, ����������
	DWORD							m_dwLoginTimeTick;
public:
	// ʱ���Ḩ����
	CTimerAxisHelper				m_TimerAxisHelper;
	// ��ͼ��Ļ
	ICanvas *						m_pScreen;
	// ��ͼ�ӿ�
	IMapView *						m_pMapView;
	// ����������
	HWND							m_hMainWnd;	
	// ͨѶ��ӿ�
	IAppNetworkLink *				m_pAppNetworkLink;
	// ���ڹ�������
	IWndMgr *						m_pWndMgr;
	// ��CGameApp���ô˱���
	IGameApp *						m_pGameApp;
	// ����
	IActor *						m_pPlayer;
	// ȫ�ֵ�trace����
	CFKTrace						m_TraceHelper;
	// ͼ�ι�����󴴽���
	ICanvasHelper					m_CanvasHelper;
	// ȫ��Ĭ������
	ITTFont *						m_pDefFont;
	// ȫ��Ĭ�ϴ�����������
	ITTFont *						m_pDefFontWithOutline;
	// �ļ�ϵͳ���󴴽���
	CIFileSystemHelper				m_FileSystemHelper;
	// �ű�����Ĵ�����
	CIResMgrHelper					m_ResMgrHelper;
	// ��Ϸ��ֵ����
	IConfigDataCenter  *			m_pDataCenter;
	// CMapItemView������
	IItemClassFactory *				m_pItemClassFactory;
	// �ͻ�������
	IClientWorld *					m_pClientWorld;
	// THING��������
	IThingFactory *					m_pThingFactory;
	// �¼�������
	IEventServer *					m_pEventServer;
	// CSV�ű�������
	IScriptUpdateManager *			m_pScriptUpdateManager;
public:
	// ����ģ��
	virtual IChatClient *			GetChatClient(void) = NULL;
	// �ؼ��ֹ���
	virtual IKeywordFilter *		GetKeywordFilter(void) = NULL;
	// ���
	virtual ITeamClient *			GetTeamClient(void) = NULL;
	// Ч������
	virtual IEffectFactory *		GetEffectFactory(void) = NULL;
	// �ͻ��˿�����
	virtual IController *			GetController(void) = NULL;
	// ����ģ��
	virtual IFriendClient *			GetFriendClient(void) = NULL;
	// ��������
	virtual ISound *				GetSoundEngine(void) = NULL;
};
//------------------------------------------------------------------------