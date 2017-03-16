/**
*	created:		2012-6-21   0:25
*	filename: 		ClientGlobal
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "PreInclude.h"
#include "ClientGlobal.h"
//------------------------------------------------------------------------
#define MAX_SOUND_CHANNEL		16

#define COLDTIMERES_CONFIG_FILE "data\\scp\\ColdTime.res"
//------------------------------------------------------------------------
CClientGlobal::CClientGlobal(void)
{
	ZeroMemory(m_szWorkDir, sizeof(m_szWorkDir));
	m_pScreen = NULL;
	m_pMapView = NULL;
	m_pAppNetworkLink = NULL;
	m_pPlayer = NULL;
	m_pWndMgr = NULL;
	m_pGameApp = NULL;
	m_pDefFont = NULL;
	m_pDefFontWithOutline = NULL;
	m_pEventServer = NULL;
	m_pScriptUpdateManager = NULL;
}
//------------------------------------------------------------------------
CClientGlobal::~CClientGlobal(void)
{
	Close();
}
//------------------------------------------------------------------------
BOOL CClientGlobal::Create(HINSTANCE hInstance, LPCSTR szWorkDir, HWND hMainWnd, ICanvas * pScreen, ITTFont * pDefFont, ITTFont * pDefFontWithOutline, ITrace * pTrace)
{
	m_hMainWnd = hMainWnd;
	m_pScreen = pScreen;

	// ����Ŀ¼
	lstrcpyn(m_szWorkDir, szWorkDir, MAX_PATH);
	
	// Trace
	char szTraceFileName[MAX_PATH];
	std::string strTraceFile = std::string(m_szWorkDir) + std::string( TRACE_LOG_FILE );
	wsprintf(szTraceFileName, strTraceFile.c_str() );
	if(pTrace != NULL)
	{
		if(m_TraceHelper.Create(pTrace) == FALSE)
		{
			MessageBox(NULL, "[game]Create Trace failed!", "Trace faile", 0);
			return FALSE;
		}
	}
#ifdef _DEBUG
	else
	{
		if (m_TraceHelper.Create(::GetModuleHandle(NULL), 
									   OS_WINDOW|OS_FILE | OS_OUTPUTDEBUGSTRING, 
									   NULL, NULL, szTraceFileName, FALSE) == FALSE)
		{
			MessageBox(NULL, "[game]First create Trace failed!", "Trace faile", NULL );
			return FALSE;
		}				
	}
#endif
	// ʱ����
	PrintEventLn("Start m_TimerAxisHelper.Create");	
	if(m_TimerAxisHelper.Create() == FALSE)
	{
		MessageBox(0, "Create Axis Failed!", "", 0);
		return FALSE;
	}	
	PrintEventLn("Finish m_TimerAxisHelper.Create");

	// �ļ�ϵͳ����
	IFileSystem * pFileSystem = NULL;
	char szMapDir[MAX_PATH] = { 0 };
	sprintf(szMapDir,    MAP_RES_DIR,			m_szWorkDir);
	m_FileSystemHelper.Create(&pFileSystem, szMapDir, FSS_DIRECTORY, m_TraceHelper.m_pTrace);
	if(pFileSystem == NULL)
	{
		MessageBox(0, "�����ļ�ϵͳʧ��", "", 0);
		return FALSE;
	}

	// ��ͼϵͳ����
	m_pMapView = g_CommonFunc.m_MapViewHelper.m_pMapView;

	// ��Դ������
	PrintEventLn("Start m_ResMgrHelper.Create");
	if(m_ResMgrHelper.Create(m_TraceHelper.GetTraceObject()) == FALSE)
	{
		MessageBox(0, "Create resmgr Failed!", "", 0);
		return FALSE;		
	}
	PrintEventLn("Finish m_ResMgrHelper.Create");

	// CSV�ű�������
	PrintEventLn("Start ScriptUpdateManagerHelper.Create");	
	if(g_ScriptUpdateManagerHelper.Create(m_ResMgrHelper.m_pResMgr, m_TraceHelper.GetTraceObject()) == FALSE)
	{
		MessageBox(0, "Create script Update Failed!", "", 0);
		return FALSE;	
	}
	m_pScriptUpdateManager = g_ScriptUpdateManagerHelper.m_pManager;
	PrintEventLn("Finish ScriptUpdateManagerHelper.Create");	

	// ȫ��Ĭ������
	m_pDefFont = pDefFont;
	if(m_pDefFont == NULL)
	{
		return FALSE;
	}

	// ȫ��Ĭ�ϴ�����������
	m_pDefFontWithOutline = pDefFontWithOutline;
	if(m_pDefFontWithOutline == NULL)
	{
		return FALSE;
	}

	// �¼�������
	char szEventScpFileName[MAX_PATH];
	sprintf(szEventScpFileName, EVENT_RES_FILE, g_ClientGlobal.m_szWorkDir);
	PrintEventLn("Start m_EventServerHelper.Create");	
#ifdef _DEBUG
	if(!g_EventServerHelper.Create(m_ResMgrHelper.m_pResMgr, 
								  m_TraceHelper.m_pTrace,
								  szEventScpFileName,
								  FALSE))
	{
		MessageBox(0, "Create Event Server Failed!", "", 0);
		return FALSE;
	}
#else
	if(!g_EventServerHelper.Create(m_ResMgrHelper.m_pResMgr, 
									m_TraceHelper.m_pTrace,
									szEventScpFileName,
									TRUE))
	{
		MessageBox(0, "Create Event Server Failed!", "", 0);
		return FALSE;
	}
#endif
	m_pEventServer = g_EventServerHelper.m_pEventServer;
	PrintEventLn("Finish EventServerHelper.Create");		
/*	
	// ���ܹ�����
	PrintEventLn("Start m_SkillHelper.Create");	
	if(g_SkillHelper.CreateSkillManager(this) == FALSE)
	{
		MessageBox(0, "Create skill module Failed!", "", 0);
		return FALSE;
	}
	m_pSkillManager = g_SkillHelper.m_pSM;
	PrintEventLn("Finish g_SkillHelper.Create");	
*/

	// ����ű�
	PrintEventLn("Start load all script");
	if(g_ConfigDataCenter.LoadScript() == FALSE)
	{
		MessageBox(0, "load all script failed!", "", 0);
		return FALSE;
	}
	m_pDataCenter = static_cast<IConfigDataCenter *>(&g_ConfigDataCenter);
	PrintEventLn("Finish load all script");

	// ����ItemFactory
	PrintEventLn("Start item factory Create");
	if(g_BasicSysHelper.CreateItemClassFactory(this) == FALSE)
	{
		MessageBox(0, "Create item factory Failed!", "", 0);
		return FALSE;
	}
	m_pItemClassFactory = g_BasicSysHelper.m_pICF;
	PrintEventLn("Finish item factory Create");

	// THING��������
	PrintEventLn("Start thing factory Create");
	if(g_ThingFactoryHelper.Create(this) == FALSE)
	{
		MessageBox(0, "Create thing factory Failed!", "", 0);
		return FALSE;
	}
	m_pThingFactory = g_ThingFactoryHelper.m_pTCF;
	PrintEventLn("Finish thing factory Create");
/*	
	// ������ȴϵͳ
	PrintEventLn("Start Cold Timer system Create");
	if(!g_ColdTimeSystemHelper.Create(m_TraceHelper.m_pTrace, m_pScriptUpdateManager, COLDTIMERES_CONFIG_FILE, static_cast<IClientGlobal *>(this)))
	{
		MessageBox(0, "Create Cold Timer system Failed!", "", 0);
		return FALSE;
	}
	PrintEventLn("Finish Cold Timer system Create");
	
	// ������������
	PrintEventLn("Start sound system Create");
	if(!g_SoundHelper.Create(MAX_SOUND_CHANNEL, static_cast<IClientGlobal *>(this)))
	{
		PrintEventLn("Create sound system Failed!");
	}
	else
	{
		PrintEventLn("Finish sound system Create");
	}

	// ����PKģ��
	PrintEventLn("Start pk system Create");
	if(!g_PKSystemHelper.Create(static_cast<IClientGlobal *>(this)))
	{
		MessageBox(0, "Create pk system Failed!", "", 0);
		return FALSE;
	}
	PrintEventLn("Finish pk system Create");

	// ����״̬ϵͳ
	PrintEventLn("Start state system Create");
	if(!g_StatusClientHelper.Create(static_cast<IClientGlobal *>(this)))
	{
		MessageBox(0, "Create state system Failed!", "", 0);
		return FALSE;
	}
	PrintEventLn("Finish state system Create");

	// ����Ч��ϵͳ 
	PrintEventLn("Start effect factory Create");
	if(!g_EffectFactoryHelper.Create(static_cast<IClientGlobal *>(this)))
	{
		MessageBox(0, "Create effect factory Failed!", "", 0);
		return FALSE;
	}
	PrintEventLn("Finish effect factory Create");

	// ����ҩƷϵͳ 
	PrintEventLn("Start medicine client Create");
	if(!g_MedicineClientHelper.Create(static_cast<IClientGlobal *>(this)))
	{
		MessageBox(0, "Create medicine client Failed!", "", 0);
		return FALSE;
	}
	PrintEventLn("Finish medicine client Create");

	// ������������
	PrintEventLn("Start work skill system Create");
	if(!g_WorkSkillSystemHelper.Create(static_cast<IClientGlobal *>(this)))
	{
		MessageBox(0, "Create work skill system Failed!", "", 0);
		return FALSE;
	}
	PrintEventLn("Finish work skill system Create");

	// �����ؼ��ֹ���ģ��
	PrintEventLn("Start keyword filter Create");
	if(!g_KeywordFilterHelper.Create())
	{
		MessageBox(0, "Create keyword filter Failed!", "", 0);
		return FALSE;
	}
	PrintEventLn("Finish keyword filter Create");	
*/
	// ����
	m_pClientWorld = static_cast<IClientWorld *>(&m_ClientWorld);	

	return TRUE;
}
//------------------------------------------------------------------------
void CClientGlobal::Close(void)
{

	// �ͷ�canvas
	//m_CanvasHelper.Close();

	// �ͷ��ļ�ϵͳ
	//m_FileSystemHelper.Close();
/*
	// �ͷ�npc֧�ֿͻ���
	g_TradeClientHelper.Close();

	// �ͷŹ�������
	g_WorkSkillSystemHelper.Close();

	// �ͷ�ҩƷϵͳ
	g_MedicineClientHelper.Close();

	// �ͷ�Ч��ϵͳ
	g_EffectFactoryHelper.Close();

	// �ͷ�״̬ϵͳ
	g_StatusClientHelper.Close();

	// �ͷ����ģ��
	g_TeamClientHelper.Close();

	// �ͷ�PKģ��
	g_PKSystemHelper.Close();

	// �ͷ���������
	g_SoundHelper.Close();
*/
	// �ͷſͻ���ģ��
	g_GameModuleHelper.Close();
/*
	// �ͷ���ȴϵͳ
	g_ColdTimeSystemHelper.Close();

	// �ͷ�thing����
	g_ThingFactoryHelper.Close();

	// �ͷ�basicsys
	g_BasicSysHelper.Close();

	// �ͷż���ģ��
	g_SkillHelper.Close();
	
	// �ͷ��¼�������
	//g_EventServerHelper

	// �ͷŽű�������
	g_ScriptUpdateManagerHelper.Close();

	// �ͷ�����ģ��
	g_ChatClientHelper.Close();

	// �ͷŽ���ģ��
	g_InteractClientHelper.Close();
*/	
	g_CommonFunc.Close();

	// �ͷ�ʱ����
	m_TimerAxisHelper.Close();
}
//------------------------------------------------------------------------
// ��  �����ؼ��ֹ���
IKeywordFilter * CClientGlobal::GetKeywordFilter(void)
{
	return g_KeywordFilterHelper.m_pKeywordFilter;
}
//------------------------------------------------------------------------
// ��  �������
ITeamClient * CClientGlobal::GetTeamClient(void)
{
	return g_TeamClientHelper.GetModule();
}
//------------------------------------------------------------------------
// ��  ����Ч������
IEffectFactory * CClientGlobal::GetEffectFactory(void)
{
	return g_EffectFactoryHelper.GetEffectFactory();
}
//------------------------------------------------------------------------
// ��  �����ͻ��˿�����
IController * CClientGlobal::GetController(void)
{
	return g_CommonFunc.m_ControllerHelper.m_pInput;
}
//------------------------------------------------------------------------
// ��  ��������ģ��
IChatClient * CClientGlobal::GetChatClient(void)
{
	return g_ChatClientHelper.GetChatClient();
}
//------------------------------------------------------------------------
// ��  ��������ģ��
IFriendClient *	CClientGlobal::GetFriendClient(void)
{
	return g_FriendClientHelper.GetModule();
}
//------------------------------------------------------------------------
// ��  ������������
ISound * CClientGlobal::GetSoundEngine(void)
{
	return g_SoundHelper.m_pSound;
}
//------------------------------------------------------------------------