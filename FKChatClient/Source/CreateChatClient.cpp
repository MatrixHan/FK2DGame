/**
*	created:		2012-6-30   6:02
*	filename: 		CreateChatClient
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ�ִ�������ģ��
*/
//------------------------------------------------------------------------
#include "../IChatClient.h"
#include "../../FKTrace/IFKTrace.h"
#include "../../FKGame/GameGlobal/IClientGlobal.h"
#include "../Include/ChatClient.h"
//------------------------------------------------------------------------
// ��  ������������ģ��
// ��  ����[����] DWORD dwVersion - ����ģ��İ汾��
// ��  ����[����] IClientGlobal * pGlobal - ȫ�ֹ�������ָ��
// ��  ����[���] IChatClient ** ppModule - ����ģ�����ɺ��ַ��ָ��
// ����ֵ��BOOL - ģ�鴴���ɹ�����TRUE,���򷵻�FALSE
//-------------------------------------------------------------------
CHATCLIENT_API BOOL CreateChatClient(DWORD dwVersion, 
									 IClientGlobal * pGlobal, 
									 IChatClient ** ppModule)
{
	// ��鱣��ģ���ַ��ָ���Ƿ���Ч
	if (ppModule == NULL)
	{
		return FALSE;
	}
	*ppModule = NULL;
	
	// ���ȫ�ֹ������ǲ����Ѿ�����
    if (pGlobal == NULL)
	{
		return FALSE;
	}
	g_DefaultTrace.Create(pGlobal->m_TraceHelper.GetTraceObject());
	TraceLn("ChatClient - CreateChatClient");

	// ���汾�ǲ�����ȷ
	if (dwVersion != CHATCLIENT_VERSION)
	{
		PrintEventLn("ChatClient - VERSION NOT MATCH");
		return FALSE;
	}	
	
	// ����ģ�����
	CChatClient * pChatClient = CChatClient::Instance();
	if (pChatClient == NULL)
	{
		PrintEventLn("ChatClient (FAIL) - new CChatClient()");		
		return FALSE;
	}
	if (!pChatClient->Create(pGlobal))
	{
		PrintEventLn("ChatClient (FAIL) - pChatClient->Create(pGlobal)");
		pChatClient->Release();
		return FALSE;
	}	
	
	// �Ѿ��ɹ�������ģ��
	*ppModule = static_cast<CChatClient *>(pChatClient);	
	TraceLn("ChatClient - CreateChatClient OK");
	return TRUE;
}
//------------------------------------------------------------------------