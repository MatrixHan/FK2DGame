/**
*	created:		2012-6-28   7:32
*	filename: 		ChatParser
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ�ֶ�������Ϣ�Ľ���
*/
//------------------------------------------------------------------------
#include "../IChatClient.h"
#include "../Include/ChatClient.h"
#include "../Include/ChatItemFactory.h"
#include "../Include/ChatParser.h"
#include "../../FKGame/SceneObj/IGoods.h"
#include "../../FKGame/KeywordFilter/IKeywordFilter.h"
#include "../../FKGame/MapView/MapItemView.h"
#include "../../FKGame/Common/CreatureViewDef.h"
#include "../../FKGame/WndSys/WndDef.h"
#include <assert.h>
//------------------------------------------------------------------------
CChatParser::CChatParser(void)
: m_pChatNode(NULL)
, m_strColor("-1")
, m_nChannel(-1)
, m_uidSender(INVALID_UID)
, m_nCustomCmd(CHAT_CMD_NONE)
{
}
//------------------------------------------------------------------------
CChatParser::~CChatParser(void)
{
}

//-------------------------------------------------------------------
// ��  ��������������Ϣ 
// ��  ����[����] const char * lpszData - ������Ϣ
// ��  ����[����] int nLen - ������Ϣ����
// ����ֵ��CHtmlNode * - ������ȷ����������Ϣ��HTMLͷ�ڵ㣬���򷵻�NULL
//-------------------------------------------------------------------
CHtmlNode * CChatParser::Parse(const char * lpszData, int nLen)
{
	char buf[1024];

	memset(buf, 0, sizeof(buf));
	m_strColor = "-1";

	if (nLen > sizeof(buf))
	{
		return NULL;
	}

	SChatMsgHeader *pMsgHeader = (SChatMsgHeader *)lpszData;
	if( nLen < sizeof(SChatMsgHeader))
	{
       return NULL;
	}
	// ��������
    if( pMsgHeader->nChannel == CHANNEL_MONSTER)
	{
	   OnMonsterChat(lpszData,nLen);
       return NULL;
	}
	// ����������Ϣ��ͷ�ڵ�
	m_pChatNode = new CHtmlNode();
	if (m_pChatNode == NULL)
	{
		return NULL;
	}
	m_pChatNode->SetName("chat");
	
	// ������Ϣͷ
	if (!ParseHeader(&lpszData, nLen))
	{
		delete m_pChatNode;
		return NULL;
	}

	// �����Զ�������
	if (m_nCustomCmd != CHAT_CMD_NONE)
	{
		OnCustomCmd(lpszData);
		return m_pChatNode;
	}

	// ����˵������
	IKeywordFilter * pFilter = CChatClient::Instance()->m_pGlobal->GetKeywordFilter();
	if (pFilter == NULL)
	{
		delete m_pChatNode;
		return NULL;
	}
	// �Ƚ������������ݵ��ַ���
	string strTemp;
	if( ParseSpecialContent(lpszData,nLen,strTemp) )
	{
		//
		lpszData = strTemp.c_str();
		pFilter->Filter(lpszData, buf, strTemp.length(), enKL_Replace);
	}else{
		pFilter->Filter(lpszData, buf, nLen, enKL_Replace);
		
		// ����˵������
		if (!ParseContent(buf, strlen(buf) + 1))
		{
			delete m_pChatNode;
			return NULL;
		}
	}
	// �Ƿ���Ҫ��ʾ����
	if (m_nChannel == CHANNEL_LOCAL)
	{	
		ShowPaoPao(buf);	
	}else if(m_nChannel == CHANNEL_TEAM)
	{
	   //��������Ƶ��,���������,10��Tile֮��Ҳ
		// if(CChatClient::Instance()->IsDistance(pMsgHeader->uidSender,20))
	   {
		   ShowPaoPao(buf);
	   }
      
	}
	return m_pChatNode;
}

//-------------------------------------------------------------------
// ��  ��������������Ϣ��˵������
//-------------------------------------------------------------------
CHtmlNode * CChatParser::Parse(LPCSTR lpszText)
{
	// ����������Ϣ��ͷ�ڵ�
	m_pChatNode = new CHtmlNode();
	if (m_pChatNode == NULL)
	{
		return NULL;
	}
	m_pChatNode->SetName("chat");

	// ��������
	if (!ParseContent(lpszText, strlen(lpszText) + 1))
	{
		delete m_pChatNode;
		return NULL;
	}
	
	return m_pChatNode;
}

//-------------------------------------------------------------------
// ��  ����������Ϣ������ͷ
// ��  ����[����] const char ** ppData - ��Ϣ��ԭʼ����
// ��  ����[����] int & nLen - ��Ϣ�ĳ���
// ����ֵ��BOOL - ������ȡ����TRUE, ���򷵻�FALSE
//-------------------------------------------------------------------
BOOL CChatParser::ParseHeader(const char ** ppData, int & nLen)
{
	assert (m_pChatNode != NULL);

	if (nLen < sizeof(SChatMsgHeader))
	{
		return FALSE;
	}

	const SChatMsgHeader * pHeader = (const SChatMsgHeader *)(*ppData);
	m_nChannel = pHeader->nChannel;
	m_uidSender = pHeader->uidSender;

	// ���Ƶ���Ƿ���Ч	
	if (!IS_VALID_CHANNEL(pHeader->nChannel))
	{
		return FALSE;
	}
	m_pChatNode->SetAttribute("channel", pHeader->nChannel);
	
	// ���Ƶ��Ϊ��ʾ����
	if (!AddChannel(pHeader->nChannel))
	{
		return FALSE;
	}

	// ��ӷ�����Ϊ��ʾ����
	if (pHeader->nChannel == CHANNEL_PRIVATE)
	{
		if (nLen < sizeof(SPrivateChatMsg))
		{
			return FALSE;
		}
		
		const SPrivateChatMsg * pMsg = (const SPrivateChatMsg *)(*ppData);
		
		if (nLen != sizeof(SPrivateChatMsg) + pMsg->wTextLen - 1)
		{
			return FALSE;
		}
		
		IActor * pActor = CChatClient::Instance()->m_pGlobal->m_pPlayer;
		if (pActor == NULL)
		{
			return FALSE;
		}

		if (strcmp(pMsg->header.szSender, pActor->GetName()) == 0)
		{
			if (!AddText("���", "255,255,255"))
			{
				return FALSE;
			}			
			if (!AddSender(pMsg->szReceiver))
			{
				return FALSE;
			}
			if (!AddText("˵", "255,255,255"))
			{
				return FALSE;
			}
		}
		else
		{
			if (!AddSender(pMsg->header.szSender))
			{
				return FALSE;
			}
			if (!AddText("����˵", "255,255,255"))
			{
				return FALSE;
			}
		}		
	}
	else if (pHeader->nChannel != CHANNEL_SYSTEM)
	{
		if (!AddSender(pHeader->szSender))
		{
			return FALSE;
		}		
	}

	if (!AddText(":", "255,255,255"))
	{
		return FALSE;
	}

	switch (pHeader->nChannel)
	{
	case CHANNEL_SYSTEM:
		return OnSystemChat(ppData, nLen);		
		
	case CHANNEL_LOCAL:
		return OnLocalChat(ppData, nLen);

	case CHANNEL_PRIVATE:
		return OnPrivateChat(ppData, nLen);		

	case CHANNEL_TEAM:
		return OnTeamChat(ppData, nLen);

	case CHANNEL_CLAN:
		return OnClanChat(ppData, nLen);

	case CHANNEL_WORLD:
		return OnWorldChat(ppData, nLen);

	default:
		return FALSE;
	}		
}

//-------------------------------------------------------------------
// ��  ��������˵��������, �����������Ϣ�ó������ʾ��HTML��ʽ 
// ��  ����[����] const char * pContent - ˵������
// ��  ����[����] int nLen - ˵�����ݳ���
// ����ֵ��BOOL - ������ȷ����TRUE,���򷵻�FALSE
//-------------------------------------------------------------------
BOOL CChatParser::ParseContent(const char * pContent, int nLen)
{
	assert (m_pChatNode != NULL);

	string strText;
	
	for (int i = 0; i < nLen && *pContent != '\0'; i++)
	{
		// �����ı���ɫ
		if (*pContent == '&' && isdigit(*(pContent + 1)))
		{
			if (!AddSentence(strText, m_strColor))
			{
				return FALSE;
			}
			strText = "";

			if (!ParseColor(&pContent))
			{
				return FALSE;
			}
		}
		// ��������ͼƬ
		else if (*pContent == '#' && isdigit(*(pContent + 1)))
		{
			if (!AddSentence(strText, m_strColor))
			{
				return FALSE;
			}
			strText = "";
			
			if (!ParseFace(&pContent))
			{
				return FALSE;
			}
		}
		// ����װ����Ϣ
		else
		{
			strText += *pContent++;
		}
	}
	
	// �����ϵͳ��Ϣ
	if (!AddSentence(strText, m_strColor))
	{
		return FALSE;
	}
	return TRUE;
}

//-------------------------------------------------------------------
// ��  �������������������ݵ��ַ���
//-------------------------------------------------------------------
BOOL CChatParser::ParseSpecialContent(const char *pContent,int nLen,string& str)
{ 
	if( pContent[0] != SPECIAL_CONTENT)
	{
         return FALSE;
	}
	// ��ʽΪ 
	// Item ����
	// ��һ��Item ����
	// ��һ��Item����
	// ��һ��Item����
    // .......
	if( nLen < (int)sizeof(int) )
	{
       return TRUE;
	}
	pContent += 1;
	int nItemCount = *(int *)pContent;
	pContent+=sizeof(int);
	nLen -=sizeof(int);
	for(int i=0;i<nItemCount;i++)
	{
         if( nLen < sizeof(int) )
		 {
			 return TRUE;
		 }
		 // ����
		 int nItemLen = *(int *)pContent;
		 nLen -=sizeof(int);
		 pContent += sizeof(int);
         // ����
         if( nLen < sizeof(int) )
		 {
			 return TRUE;
		 }		 
		 int nType = *(int *)pContent;
		 nLen -=sizeof(int);
		 pContent += sizeof(int);	
		 //����
		 nItemLen -= 2*sizeof(int);
		 if( nItemLen < 0 )
		 {
			 return TRUE;
		 }
         if(nLen<nItemLen)
		 {
            return TRUE;
		 }
		 char *pBuf = (char *)pContent;
		 pContent += nItemLen;
		 nLen -= nItemLen;
         // ������Ʒ
		 if( nType == eGoods )
		 {
			 IGoods *pGoods = (IGoods *)CChatClient::Instance()->CreateGoods(pBuf,nItemLen,INVALID_UID);
		     if( pGoods!= NULL)
			 {
                 AddGoods((DWORD)pGoods);
			    str += "[";
				str += pGoods->GetName();
				str += "]";
			 }
			 
		 }else{
            ParseContent(pBuf,nItemLen);
			str += pBuf;
		 }
	}
	return TRUE;
}

//-------------------------------------------------------------------
// ��  �������������� 
//-------------------------------------------------------------------
int CChatParser::ParseIndex(char * pBuf, int nLen, const char * pData)
{
	int i = 0;

	for (; i < nLen; i++)
	{
		if (!(((*pData)>='0')&&((*pData)<='9')))
		{
			break;
		}
		*pBuf++ = *pData++;
	}

	return i;
}

//-------------------------------------------------------------------
// ��  ����������ɫ
//-------------------------------------------------------------------
BOOL CChatParser::ParseColor(const char ** ppContent)
{
	char buf[3];
	const char * pData = *ppContent + 1;	// &
	
	// ��ȡ��ɫ����
	memset(buf, 0, sizeof(buf));
	int nCount = ParseIndex(buf, 2, pData);
	if (nCount == 0)
	{
		return FALSE;
	}
	
	m_strColor = buf;
	
	*ppContent += (nCount + 1);
	
	return TRUE;
}

//-------------------------------------------------------------------
// ��  ������������
//-------------------------------------------------------------------
BOOL CChatParser::ParseFace(const char ** ppContent)
{
	char buf[3];
	const char * pData = *ppContent + 1;	// #

	// ��ȡͼƬ����
	memset(buf, 0, sizeof(buf));
	int nCount = ParseIndex(buf, 2, pData);
	if (nCount == 0)
	{
		return FALSE;
	}

	// ��ӱ�ǩͼƬ
	CHtmlNode * pNode = new CHtmlNode();
	if (pNode == NULL)
	{
		return FALSE;
	}
	pNode->SetName("img");
	pNode->AddAttribute("src", buf);
	m_pChatNode->AddChild(pNode);

	*ppContent += (nCount + 1);
	
	return TRUE;
}

//-------------------------------------------------------------------
// ��  ��������װ����Ϣ    ��ʽ��[name,info]
//-------------------------------------------------------------------
BOOL CChatParser::ParseThing(const char ** ppContent)
{
	BOOL bName = TRUE;
	string strName;
	string strInfo;
	const char * pData = *ppContent;	

	// ����װ�����ƺ���Ϣ
	while (*pData != ']')
	{
		if (*pData == '\0')
		{			
			return FALSE;
		}

		if (*pData == ',')
		{
			bName = FALSE;
			pData++;
			continue;
		}

		if (bName)
		{
			strName += *pData++;
		}
		else
		{
			strInfo += *pData++;
		}		
	}

	// ���װ����Ϣ����
	CHtmlNode * pNode = new CHtmlNode();
	if (pNode == NULL)
	{
		return FALSE;
	}
	pNode->SetName("a");
	pNode->SetContent(strName);
	pNode->SetAttribute("href", strInfo);
	pNode->SetAttribute("type", LINK_THING);
	m_pChatNode->AddChild(pNode);

	*ppContent = pData;

	return TRUE;
}

//-------------------------------------------------------------------
// ��  ������������Ƶ����Ϣ 
//-------------------------------------------------------------------
BOOL CChatParser::AddChannel(int nChannel)
{
	CHtmlNode * pNode = new CHtmlNode();
	if (pNode == NULL)
	{
		return FALSE;
	}
	pNode->SetName("text");

	switch (nChannel)
	{
	case CHANNEL_SYSTEM:
		pNode->SetAttribute("frontcolor", "218,212,231");
		pNode->SetAttribute("backcolor", "218,212,231");
		pNode->SetContent("[ϵͳ]");
		break;

	case CHANNEL_LOCAL:
		pNode->SetAttribute("frontcolor", "218,212,231");
		pNode->SetAttribute("backcolor", "0,139,29");
		pNode->SetContent("[����]");		
		break;

	case CHANNEL_PRIVATE:
		pNode->SetAttribute("frontcolor", "252,116,43");
		pNode->SetAttribute("backcolor", "132,7,143");
		pNode->SetContent("[˽��]");		
		break;
		
	case CHANNEL_TEAM:
		pNode->SetAttribute("frontcolor", "81,204,249");
		pNode->SetAttribute("backcolor", "8,31,148");
		pNode->SetContent("[����]");
		break;
		
	case CHANNEL_CLAN:
		pNode->SetAttribute("frontcolor", "21,252,3");
		pNode->SetAttribute("backcolor", "194,105,0");
		pNode->SetContent("[����]");
		break;

	case CHANNEL_WORLD:
		pNode->SetAttribute("frontcolor", "251,242,3");
		pNode->SetAttribute("backcolor", "164,0,0");
		pNode->SetContent("[����]");		
		break;

	default:
		delete pNode;
		return FALSE;
	}		
	
	m_pChatNode->AddChild(pNode);

	return TRUE;
}

//-------------------------------------------------------------------
// ��  ������ӷ�����
//-------------------------------------------------------------------
BOOL CChatParser::AddSender(string strSender)
{
	CHtmlNode * pNode = NULL;

	// '('
	pNode = new CHtmlNode();
	if (pNode == NULL)
	{
		return FALSE;
	}
	pNode->SetName("text");
	pNode->SetContent("(");
	m_pChatNode->AddChild(pNode);
	
	// (...)���м䲿��
	pNode = new CHtmlNode();
	if (pNode == NULL)
	{
		return FALSE;
	}
	pNode->SetName("a");
	pNode->SetAttribute("type", LINK_PLAYER);
	pNode->SetContent(strSender);
	m_pChatNode->AddChild(pNode);

	// ')'
	pNode = new CHtmlNode();
	if (pNode == NULL)
	{
		return FALSE;
	}
	pNode->SetName("text");
	pNode->SetContent(")");
	m_pChatNode->AddChild(pNode);

	return TRUE;
}

//-------------------------------------------------------------------
// ��  ���� ��ӵ�ǰ��ʱ��
//-------------------------------------------------------------------
BOOL CChatParser::AddTime(void)
{
	SYSTEMTIME now;
	GetLocalTime(&now);

	// ʱ����ʾ�ĸ�ʽ
	char buf[100];
	sprintf(buf, "(%.2d:%.2d)", now.wHour, now.wMinute);
	
	CHtmlNode * pNode = new CHtmlNode();
	if (pNode == NULL)
	{
		return FALSE;
	}
	pNode->SetName("text");
	pNode->SetContent(buf);
	
	m_pChatNode->AddChild(pNode);

	return TRUE;
}

//-------------------------------------------------------------------
// ��  �������һ����� 
//-------------------------------------------------------------------
BOOL CChatParser::AddSentence(string strText, string strColor)
{
	if (!strText.empty())
	{
		CHtmlNode * pNode = new CHtmlNode();
		if (pNode == NULL)
		{
			return FALSE;
		}

		pNode->SetName("a");
		pNode->SetContent(strText);		
		pNode->SetAttribute("type", LINK_SENTENCE);
		if (m_nChannel != CHANNEL_SYSTEM)
		{
			pNode->SetAttribute("color", strColor);
		}		

		m_pChatNode->AddChild(pNode);
	}
	
	return TRUE;
}

//-------------------------------------------------------------------
// ��  �������һ���ı�
//-------------------------------------------------------------------
BOOL CChatParser::AddText(string strText, string strColor)
{
	if (!strText.empty())
	{
		CHtmlNode * pNode = new CHtmlNode();
		if (pNode == NULL)
		{
			return FALSE;
		}
		
		pNode->SetName("text");
		pNode->SetContent(strText);		
		pNode->SetAttribute("frontcolor", strColor);	
		
		m_pChatNode->AddChild(pNode);
	}
	
	return TRUE;
}

//-------------------------------------------------------------------
// ��  �������һ�� ��Ʒ
//-------------------------------------------------------------------
void CChatParser::AddGoods(DWORD dwData)
{
	IGoods *pGoods = (IGoods *)dwData;
	if( pGoods == NULL)
	{
		return ;
	}
	string str = "[";
	AddText(str,"128,128,255");
    CHtmlNode * pNode = new CHtmlNode();
	if (pNode == NULL)
	{
		return ;
	}
	pNode = new CHtmlNode();
	if (pNode == NULL)
	{
		return ;
	}
	pNode->SetName("goods");
    pNode->SetData(dwData);
	pNode->SetContent(pGoods->GetName());
    pNode->SetAttribute("type",LINK_THING);
	m_pChatNode->AddChild(pNode);
	str="]";
	AddText(str,"128,128,255");
}

//-------------------------------------------------------------------
// ��  ����ϵͳ������Ϣ 
//-------------------------------------------------------------------
BOOL CChatParser::OnSystemChat(const char **ppData, int & nLen)
{
	if (nLen < sizeof(SSystemChatMsg))
	{
		return FALSE;
	}
	
	const SSystemChatMsg * pMsg = (const SSystemChatMsg *)(*ppData);
	
	if (nLen != sizeof(SSystemChatMsg) + pMsg->wTextLen - 1)
	{
		return FALSE;
	}

	m_nCustomCmd = pMsg->nCustomCmd;
	*ppData = pMsg->szText;
	nLen = pMsg->wTextLen;
		
	return TRUE;
}

//-------------------------------------------------------------------
// ��  ��������������Ϣ 
//-------------------------------------------------------------------
BOOL CChatParser::OnLocalChat(const char **ppData, int &nLen)
{
	if (nLen < sizeof(SLocalChatMsg))
	{
		return FALSE;
	}
	
	const SLocalChatMsg * pMsg = (const SLocalChatMsg *)(*ppData);
	
	if (nLen != sizeof(SLocalChatMsg) + pMsg->wTextLen - 1)
	{
		return FALSE;
	}
	
	*ppData = pMsg->szText;
	nLen = pMsg->wTextLen;
	
	return TRUE;
}

//-------------------------------------------------------------------
// ��  ����˽��������Ϣ 
//-------------------------------------------------------------------
BOOL CChatParser::OnPrivateChat(const char **ppData, int &nLen)
{
	if (nLen < sizeof(SPrivateChatMsg))
	{
		return FALSE;
	}
	
	const SPrivateChatMsg * pMsg = (const SPrivateChatMsg *)(*ppData);
	
	if (nLen != sizeof(SPrivateChatMsg) + pMsg->wTextLen - 1)
	{
		return FALSE;
	}
	
	*ppData = pMsg->szText;
	nLen = pMsg->wTextLen;
	
	return TRUE;
}

//-------------------------------------------------------------------
// ��  ��������������Ϣ 
//-------------------------------------------------------------------
BOOL CChatParser::OnTeamChat(const char **ppData, int &nLen)
{
	if (nLen < sizeof(STeamChatMsg))
	{
		return FALSE;
	}
	
	const STeamChatMsg * pMsg = (const STeamChatMsg *)(*ppData);
	
	if (nLen != sizeof(STeamChatMsg) + pMsg->wTextLen - 1)
	{
		return FALSE;
	}
	
	*ppData = pMsg->szText;
	nLen = pMsg->wTextLen;
	
	return TRUE;
}

//-------------------------------------------------------------------
// ��  ��������������Ϣ 
//-------------------------------------------------------------------
BOOL CChatParser::OnClanChat(const char **ppData, int &nLen)
{
	if (nLen < sizeof(SClanChatMsg))
	{
		return FALSE;
	}
	
	const SClanChatMsg * pMsg = (const SClanChatMsg *)(*ppData);
	
	if (nLen != sizeof(SClanChatMsg) + pMsg->wTextLen - 1)
	{
		return FALSE;
	}
	
	*ppData = pMsg->szText;
	nLen = pMsg->wTextLen;
	
	return TRUE;
}

//-------------------------------------------------------------------
// ��  ��������������Ϣ 
//-------------------------------------------------------------------
BOOL CChatParser::OnWorldChat(const char **ppData, int &nLen)
{
	if (nLen < sizeof(SWorldChatMsg))
	{
		return FALSE;
	}
	
	const SWorldChatMsg * pMsg = (const SWorldChatMsg *)(*ppData);
	
	if (nLen != sizeof(SWorldChatMsg) + pMsg->wTextLen - 1)
	{
		return FALSE;
	}
	
	*ppData = pMsg->szText;
	nLen = pMsg->wTextLen;
	
	return TRUE;
}

//-------------------------------------------------------------------
// ��  ��������������Ϣ 
//-------------------------------------------------------------------
BOOL CChatParser::OnMonsterChat(const char *pData,int nLen)
{
	SMonsterChatMsg *pMonsterChatMsg = (SMonsterChatMsg *)pData;
	if( nLen < sizeof(SMonsterChatMsg))
	{
		return FALSE;
	}
	m_uidSender = pMonsterChatMsg->header.uidSender;
    ShowPaoPao(pMonsterChatMsg->szText);
	return TRUE;
}

//-------------------------------------------------------------------
// ��  ������ʾ�������� 
//-------------------------------------------------------------------
void CChatParser::ShowPaoPao(const char * pszText)
{
	IPaoPao ** ppPaoPao = NULL;
	ICreature *pCreature = CChatClient::Instance()->GetCreature(m_uidSender);
	if (pCreature == NULL || pCreature->GetMapItem() == NULL)
	{
		return;
	}
   
	// ��ȡ����
	CMapItemView * pMapItemView = pCreature->GetMapItem();		
	pMapItemView->OnCommand(CrtCmdChatInfo, (DWORD)&ppPaoPao, 0);
	if (ppPaoPao == NULL)
	{
		return;
	}

	// �����û�д������ݣ��򴴽�һ��
	if (*ppPaoPao == NULL)
	{
		*ppPaoPao = CChatClient::Instance()->CreatePaoPao(m_uidSender);
		if (*ppPaoPao == NULL)
		{ 
			return;
		}
	}

	// ��������Ҫ��ʾ������
	(*ppPaoPao)->SetText(pszText);
}

//-------------------------------------------------------------------
// ��  ���������Զ�������� 
//-------------------------------------------------------------------
void CChatParser::OnCustomCmd(LPCSTR lpszText)
{
	switch(m_nCustomCmd) 
	{
	case CHAT_CMD_PING:
		OnPingCmd();
		break;
		
	case CHAT_CMD_BULLETIN:
		OnBulletinCmd(lpszText);
		break;
		
	case CHAT_CMD_FAIL:
		OnFailCmd(lpszText);
		break;

	default:
		break;
	}

	m_nCustomCmd = CHAT_CMD_NONE;
}

//-------------------------------------------------------------------
// ��  ����ping�����
//-------------------------------------------------------------------
void CChatParser::OnPingCmd(void)
{
	char buf[100];
	
	sprintf(buf, "PING: %d", CChatClient::Instance()->PingStop());

	if (!ParseContent(buf, strlen(buf) + 1))
	{
		delete m_pChatNode;
		m_pChatNode = NULL;
	}
}

//-------------------------------------------------------------------
// ��  �������洦�� 
//-------------------------------------------------------------------
void CChatParser::OnBulletinCmd(LPCSTR lpszText)
{
	CChatClient::Instance()->m_pBulletinWnd->AddBulletin(lpszText);
	delete m_pChatNode;
	m_pChatNode = NULL;
}

//-------------------------------------------------------------------
// ��  ��������˽����Ϣʧ�ܴ��� 
//-------------------------------------------------------------------
void CChatParser::OnFailCmd(LPCSTR lpszText)
{
	IWndMgr * pWndMgr = CChatClient::Instance()->m_pGlobal->m_pWndMgr;
	if (pWndMgr != NULL)
	{
		pWndMgr->OnEvent(WND_CHAT_CHANNEL, WE_WW_CHANNEL_SETCHANNEL, CHANNEL_LOCAL, 0);
	}	

	if (!ParseContent(lpszText, strlen(lpszText) + 1))
	{
		delete m_pChatNode;
		m_pChatNode = NULL;
	}
}
//------------------------------------------------------------------------