/**
*	created:		2012-6-24   20:56
*	filename: 		MapChanger
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "MapChanger.h"
//------------------------------------------------------------------------
CMapChanger  g_MapChanger;
//------------------------------------------------------------------------
CMapChanger::CMapChanger()
{

}
//------------------------------------------------------------------------
CMapChanger::~CMapChanger()
{

}
//------------------------------------------------------------------------
void CMapChanger::OnTimer(DWORD dwEventID)
{
	g_ClientGlobal->m_TimerAxisHelper.m_pTimeAxis->KillTimer(0, this);	
}
//------------------------------------------------------------------------
BOOL CMapChanger::OnReceived(const char* pData, int nLen)
{	
	TraceLn("�յ����л���ͼ��Ϣ , CMapChanger::OnReceived");
/*
	struct __SC_THING_CHANAGEMAP_HEAD
	{//���ݴ�����ʱ�Ѿ�ȥ��������Ϣͷ��, ԭSC_THING_CHANAGEMAP_HEAD ������,�����Լ�дһ��
		DWORD	m_dwChangeMode;		// �л�ģʽ.1 : �������л�, 2 ��������л�
		int		m_nMapID;			// ��ͼID
		POINT	m_ptWorld;			// ��������
	};

	//ͣ������
	g_pGlobal->GetSoundEngine()->ClearAllSound();
	g_pGlobal->GetSoundEngine()->ResetCurrentEnvironmentSoundConfig();

	//�ص������
	g_pGlobal->m_pWndMgr->OnEvent(WND_INPUTWND, WE_CW_INPUT_CLEAR, 0, 0);


	//׼������ͼ��,֪ͨ������ȡ��ע���ݼ�, ��������ڻ�ͼʱͨ����ݼ���������
	g_pGlobal->m_pWndMgr->OnEvent(WND_MAIN, WE_AW_ALLWND_UNREGACCELE, 0, 0);
	g_pGlobal->m_pWndMgr->OnEvent(WND_CHAT, WE_AW_ALLWND_UNREGACCELE, 0, 0);
	g_pGlobal->m_pWndMgr->OnEvent(WND_TARGETRIM, WE_AW_ALLWND_UNREGACCELE, 0, 0);
	g_pGlobal->m_pWndMgr->OnEvent(WND_MINIMAP, WE_AW_ALLWND_UNREGACCELE, 0, 0);

	//�յ�����ͼ��Ϣ������һЩ����, ��Щ����ͨ���������ĳЩ�¼�����,����ʱ��������
	g_pGlobal->m_pWndMgr->ShowWnd(WND_MAIN, SW_HIDE);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_PLAYERINFO, SW_HIDE);

	//���ܴ���
	g_pGlobal->m_pWndMgr->CreateWnd(WND_PRACTICESKILL);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_PRACTICESKILL, SW_HIDE);						
	g_pGlobal->m_pWndMgr->CreateWnd(WND_VOCATIONSKILL);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_VOCATIONSKILL, SW_HIDE);									
	g_pGlobal->m_pWndMgr->CreateWnd(WND_WORKSKILL);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_WORKSKILL, SW_HIDE);

	//�������ܴ���
	g_pGlobal->m_pWndMgr->CreateWnd(WND_REFINE);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_REFINE, SW_HIDE);						
	g_pGlobal->m_pWndMgr->CreateWnd(WND_COMPOSE);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_COMPOSE, SW_HIDE);		
	g_pGlobal->m_pWndMgr->CreateWnd(WND_INLAY);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_INLAY, SW_HIDE);	
	g_pGlobal->m_pWndMgr->CreateWnd(WND_EPURATE);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_EPURATE, SW_HIDE);	
	g_pGlobal->m_pWndMgr->CreateWnd(WND_PHARMACY);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_PHARMACY, SW_HIDE);	

	g_pGlobal->m_pWndMgr->CreateWnd(WND_REQUEST_WORKSKILL);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_REQUEST_WORKSKILL, SW_HIDE);						
	g_pGlobal->m_pWndMgr->CreateWnd(WND_REFINE_EX);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_REFINE_EX, SW_HIDE);						
	g_pGlobal->m_pWndMgr->CreateWnd(WND_COMPOSE_EX);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_COMPOSE_EX, SW_HIDE);		
	g_pGlobal->m_pWndMgr->CreateWnd(WND_INLAY_EX);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_INLAY_EX, SW_HIDE);	
	g_pGlobal->m_pWndMgr->CreateWnd(WND_EPURATE_EX);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_EPURATE_EX, SW_HIDE);	



	//�رյ�¼ϵ�д���, �򿪵����촰�ڵ�
	g_pGlobal->m_pWndMgr->ShowWnd(WND_ACTORSELECT, SW_HIDE);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_ACTORSELECT_BAK, SW_HIDE);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_LOGIN_BAK, SW_HIDE);

	g_pGlobal->m_pWndMgr->ShowWnd(WND_LOGIN, SW_HIDE);
	g_pGlobal->m_pWndMgr->CreateWnd(WND_MINIMAP);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_MINIMAP, SW_HIDE);	
	g_pGlobal->m_pWndMgr->ShowWnd(WND_MINIMAP, SW_SHOW);	
	g_pGlobal->m_pWndMgr->CreateWnd(WND_CHAT);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_CHAT, SW_SHOW);	

	g_pGlobal->m_pWndMgr->CreateWnd(WND_TARGETRIM);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_TARGETRIM, SW_HIDE);	
	g_pGlobal->m_pWndMgr->CreateWnd(WND_EQUIPMENT);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_EQUIPMENT, SW_HIDE);		
	g_pGlobal->m_pWndMgr->CreateWnd(WND_PACKAGE);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_PACKAGE, SW_HIDE);

	//�Ʊ�����
	g_pGlobal->m_pWndMgr->CreateWnd(WND_TREASUREBOX);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_TREASUREBOX, SW_HIDE);	

	//֪ͨ�������Դ���, ��ʼ�������ﴴ������Ϣ
	g_pGlobal->m_pWndMgr->CreateWnd(WND_PLAYERINFO);	
	g_pGlobal->m_pWndMgr->ShowWnd(WND_PLAYERINFO, SW_HIDE);	
	g_pGlobal->m_pWndMgr->OnEvent(WND_PLAYERINFO, WE_CW_PLAYERINFO_STARTLISTENBORN, 0, 0);	


	//ϵͳ�˵�����
	g_pGlobal->m_pWndMgr->CreateWnd(WND_SYSTEMMENU);	
	g_pGlobal->m_pWndMgr->ShowWnd(WND_SYSTEMMENU, SW_HIDE);	

	//�����ͼ����
	g_pGlobal->m_pWndMgr->CreateWnd(WND_WORLDMAP);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_WORLDMAP, SW_HIDE);

	//���˵�װ������
	g_pGlobal->m_pWndMgr->CreateWnd(WND_EQUIPMENT_OTHER);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_EQUIPMENT_OTHER, SW_HIDE);

	//ͼ�����ô���
	g_pGlobal->m_pWndMgr->CreateWnd(WND_GRAPHICSSETTING);	
	g_pGlobal->m_pWndMgr->ShowWnd(WND_GRAPHICSSETTING, SW_HIDE);

	//�Զ��ظ�����
	g_pGlobal->m_pWndMgr->CreateWnd(WND_AUTOREVIVE);	
	g_pGlobal->m_pWndMgr->ShowWnd(WND_AUTOREVIVE, SW_HIDE);

	//�ڶ������
	g_pGlobal->m_pWndMgr->CreateWnd(WND_EXTRASHORTCUT);	
	g_pGlobal->m_pWndMgr->ShowWnd(WND_EXTRASHORTCUT, SW_HIDE);

	//��������
	g_pGlobal->m_pWndMgr->CreateWnd(WND_PETCOMMAND);	
	g_pGlobal->m_pWndMgr->ShowWnd(WND_PETCOMMAND, SW_HIDE);

	//������ᴰ��
	g_pGlobal->m_pWndMgr->CreateWnd(WND_DROPSCROLL);	
	g_pGlobal->m_pWndMgr->ShowWnd(WND_DROPSCROLL, SW_SHOW);

	//У���봰��
	g_pGlobal->m_pWndMgr->CreateWnd(WND_VALIDATIONCODE);	
	g_pGlobal->m_pWndMgr->ShowWnd(WND_VALIDATIONCODE, SW_HIDE);

	//����������
	g_pGlobal->m_pWndMgr->CreateWnd(WND_PROCESSBAR);	
	g_pGlobal->m_pWndMgr->ShowWnd(WND_PROCESSBAR, SW_HIDE);

	//LUA����������
	g_pGlobal->m_pWndMgr->CreateWnd(WND_LUAPROCESSBAR);	
	g_pGlobal->m_pWndMgr->ShowWnd(WND_LUAPROCESSBAR, SW_HIDE);

	//����ͨ�����봰��
	g_pGlobal->m_pWndMgr->CreateWnd(WND_GENERALPASSWORD);	
	g_pGlobal->m_pWndMgr->ShowWnd(WND_GENERALPASSWORD, SW_HIDE);

	//Loading����
	g_pGlobal->m_pWndMgr->CreateWnd(WND_LOADING);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_LOADING, SW_HIDE);

	//������Ҫ�ڼ��ܴ��ڴ�������ܴ���,��Ϊ��Ҫ���ܴ���ȡ����ͼƬ
	g_pGlobal->m_pWndMgr->CreateWnd(WND_MAIN);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_MAIN, SW_SHOW);	

	//�����԰����
	g_pGlobal->m_pWndMgr->CreateWnd(WND_CLAN_HOMESTEAD);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_CLAN_HOMESTEAD, SW_HIDE);	

	//���弼�ܴ���
	g_pGlobal->m_pWndMgr->CreateWnd(WND_CLAN_SKILL);
	g_pGlobal->m_pWndMgr->ShowWnd(WND_CLAN_SKILL, SW_HIDE);	

	//�ºϳɴ���
	g_pGlobal->m_pWndMgr->CreateWnd(WND_COMPOSEV2);		
	g_pGlobal->m_pWndMgr->ShowWnd(WND_COMPOSEV2, SW_HIDE);	

	//����������
	g_pGlobal->m_pWndMgr->CreateWnd(WND_FUSIONV2);		
	g_pGlobal->m_pWndMgr->ShowWnd(WND_FUSIONV2, SW_HIDE);	

	//����������
	g_pGlobal->m_pWndMgr->CreateWnd(WND_PHARMACYV2);		
	g_pGlobal->m_pWndMgr->ShowWnd(WND_PHARMACYV2, SW_HIDE);

	//����Ƕ����
	g_pGlobal->m_pWndMgr->CreateWnd(WND_INLAYV2);		
	g_pGlobal->m_pWndMgr->ShowWnd(WND_INLAYV2, SW_HIDE);

	//�¾�������
	g_pGlobal->m_pWndMgr->CreateWnd(WND_REFINEV2);		
	g_pGlobal->m_pWndMgr->ShowWnd(WND_REFINEV2, SW_HIDE);

	//����������
	g_pGlobal->m_pWndMgr->CreateWnd(WND_EPURATEV2);		
	g_pGlobal->m_pWndMgr->ShowWnd(WND_EPURATEV2, SW_HIDE);

	//������������Чλ��Ϊ��Ч, ����ᵯ��"δѧ����̬ת��, ������ʾ�ڶ������"
	g_pGlobal->m_pWndMgr->OnEvent(WND_GRAPHICSSETTING, WE_GRAPHICSSETTING_SETSKILLDATAFLAG, 0, 0);

	__SC_THING_CHANAGEMAP_HEAD *pHead = (__SC_THING_CHANAGEMAP_HEAD *)pData;

	if (pHead->m_dwChangeMode == 2)
	{		
		g_MapChangingLinkSink.SetServer(pData + sizeof(__SC_THING_CHANAGEMAP_HEAD), sizeof(SServerKey));

		//�л�����ͼת��̬ȥ		
		g_MapChangingLinkSink.RegisterLogonMsg();  //ע��,��ʼ�ȴ����������Ϣ

		g_pGlobal->m_pWndMgr->CreateWnd(WND_LOADING);
		g_pGlobal->m_pWndMgr->ShowWnd(WND_LOADING, SW_SHOW);

		//������ģʽ��ΪĬ������, ��������ѡ���滹�����������������
		g_pGlobal->GetController()->changeControlMode(0);

		g_pGlobal->m_pWndMgr->ShowWnd(WND_MAIN, SW_HIDE);		
	}	

	//������¼��Ϣ ,֪ͨ����ģ��, ��ҿ�ʼ������Ϸ
	BoardCast_Client_Play();

	//ͣ��������Ч, �ٸ�λ������Ч, ����(��ͬ���͵�ͼ����ʱ,����������(�򶳽�ʱ��)
	g_pGlobal->GetSoundEngine()->ClearAllSound();
	g_pGlobal->GetSoundEngine()->ResetCurrentEnvironmentSoundConfig();
*/
	return TRUE;	
}
//------------------------------------------------------------------------
void CMapChanger::BoardCast_Client_Play(void)
{
	//IEventServer *pEventServer = g_pGlobal->m_pEventServer;
	//if (pEventServer)
	//{
	//	pEventServer->FireAction(NULL, 0, MSG_SYSTEM_CLIENT_PLAY, SRC_TYPE_ID_SYSTEM, (DWORD)this);
	//}
}
//------------------------------------------------------------------------
void CMapChanger::__DisConnectNetwork(void)
{
	if (g_ClientGlobal->m_pAppNetworkLink == NULL)
	{
		return;
	}
	IClientLink *pLink = g_ClientGlobal->m_pAppNetworkLink->GetClientLink();

	if (pLink && pLink->IsConnected())
	{
		pLink->CloseConnect();
	}		
}
//------------------------------------------------------------------------