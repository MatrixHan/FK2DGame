/**
*	created:		2012-6-28   9:11
*	filename: 		SysMsgReceiver
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ��ϵͳ��Ϣ������
*/
//------------------------------------------------------------------------
#include "../Include/SysMsgReceiver.h"
#include "../IReminder.h"
#include "../Include/Reminder.h"
#include "../Include/ChatClient.h"
//------------------------------------------------------------------------
CSysMsgReceiver::CSysMsgReceiver(void)
: m_pMiddleReminder(NULL)
, m_pAboveReminder(NULL)
, m_pFloatReminder(NULL)
, m_pUnderReminder(NULL)
, m_bPromptOn(FALSE)
{
}
//------------------------------------------------------------------------
CSysMsgReceiver::~CSysMsgReceiver(void)
{
}

//-------------------------------------------------------------------
// ��  ��������ϵͳ��Ϣ 
//-------------------------------------------------------------------
void CSysMsgReceiver::OnAction(IEvent * pEvent)
{
	if (pEvent == NULL || !m_bPromptOn)
	{
		return;
	}

	// ��ȡ����
	int nLen = 0;
	BYTE * pData = pEvent->GetContext(nLen);
	if (pData == NULL || nLen < sizeof(SSysMessage))
	{
		return;
	}

	const SSysMessage * pMessage = (const SSysMessage *)pData;

	// �����Ϣ�ĳ����ǲ�����ȷ
	if (nLen != (sizeof(SSysMessage) + pMessage->nTextLen))
	{
		return;
	}

	// ��ȡ��ʾ��Ϣ
	char buf[512] = "";
	memset(buf, 0, 512);
	int nTextLen = (pMessage->nTextLen < 512) ? pMessage->nTextLen : 510;
	strncpy(buf, (char *)pData + sizeof(SSysMessage), nTextLen);
	// ����������͵���Ϣ
	ShowMessage(pMessage->nType, buf);
}

//-------------------------------------------------------------------
// ��  ��������ϵͳ��Ϣ 
//-------------------------------------------------------------------
void CSysMsgReceiver::ShowMessage(int nType, const char * pszText)
{
	if (pszText == NULL)
	{
		return;
	}
	
	// ����������͵���Ϣ
	switch (nType)
	{
	case SMT_FLOAT:
		OnFloatMessage(pszText);
		break;

	case SMT_ABOVE:
		OnAboveMessage(pszText);
		break;	

	case SMT_MIDDLE:
		OnMiddleMessage(pszText);
		break;					
		
	case SMT_UNDER:
		OnUnderMessage(pszText);
		break;

	case SMT_HELP:
		CChatClient::Instance()->m_pHelpInfoWnd->AddMessage(pszText);
		break;

	default:
		break;
	}
}

//-------------------------------------------------------------------
// ��  ������ʾ�ڸ�����ʾ��
//-------------------------------------------------------------------
void CSysMsgReceiver::OnFloatMessage(const char * pszText)
{
	if (m_pFloatReminder != NULL)
	{
		m_pFloatReminder->AddMessage(pszText);
	}
}

//-------------------------------------------------------------------
// ��  ������ʾ���Ϸ���ʾ��
//-------------------------------------------------------------------
void CSysMsgReceiver::OnAboveMessage(const char * pszText)
{
	if (m_pAboveReminder != NULL)
	{
		m_pAboveReminder->AddMessage(pszText);
	}
}

//-------------------------------------------------------------------
// ��  ������ʾ���м���ʾ��
//-------------------------------------------------------------------
void CSysMsgReceiver::OnMiddleMessage(const char * pszText)
{
	if (m_pMiddleReminder != NULL)
	{
		m_pMiddleReminder->AddMessage(pszText);
	}
}

//-------------------------------------------------------------------
// ��  ������ʾ���·���ʾ��
//-------------------------------------------------------------------
void CSysMsgReceiver::OnUnderMessage(const char * pszText)
{
	if (m_pUnderReminder != NULL)
	{
		m_pUnderReminder->AddMessage(pszText);
	}
}

//-------------------------------------------------------------------
// ��  ������ȡ���� 
//-------------------------------------------------------------------
RECT CSysMsgReceiver::GetRect(IResObject * pResObject)
{
	RECT rc;
	SetRect(&rc, 0, 0, 0, 0);

	if (pResObject == NULL)
	{
		return rc;
	}

	// ��ȡ����ֵ
	LPCSTR pProperty = pResObject->GetPropertyString("rect");
	if (pProperty != NULL)
	{
		int l, t, w, h;
		sscanf(pProperty, "%d,%d,%d,%d", &l, &t, &w, &h);
		SetRect(&rc, l, t, l + w, t + h);
	}

	return rc;
}

//-------------------------------------------------------------------
// ��  ������ʼ�� 
//-------------------------------------------------------------------
BOOL CSysMsgReceiver::Init(IResObject * pResObject)
{
	int i = 0;
	RECT rc;

	if (pResObject == NULL)
	{
		return FALSE;
	}

	// �м���ʾ��
	if ((m_pMiddleReminder = new CReminder()) == NULL)
	{
		return FALSE;
	}
	rc = GetRect(pResObject->GetSubObjectByIndex(i++));
	m_pMiddleReminder->SetPos(rc);	
	m_pMiddleReminder->SetType(REMINDER_REPLACE);

	// �Ϸ���ʾ��
	if ((m_pAboveReminder = new CReminder()) == NULL)
	{
		return FALSE;
	}
	rc = GetRect(pResObject->GetSubObjectByIndex(i++));
	m_pAboveReminder->SetPos(rc);
	m_pAboveReminder->SetType(REMINDER_ADD);

	// ������ʾ��
	if ((m_pFloatReminder = new CReminder()) == NULL)
	{
		return FALSE;
	}
	rc = GetRect(pResObject->GetSubObjectByIndex(i++));
	m_pFloatReminder->SetPos(rc);

	// �·���ʾ��
	if ((m_pUnderReminder = new CReminder()) == NULL)
	{
		return FALSE;
	}
	rc = GetRect(pResObject->GetSubObjectByIndex(i++));
	m_pUnderReminder->SetPos(rc);
	m_pUnderReminder->SetType(REMINDER_REPLACE);

	return TRUE;
}

//-------------------------------------------------------------------
// ��  ��������ϵͳ��Ϣ������
//-------------------------------------------------------------------
void CSysMsgReceiver::Reset(void)
{
	m_pMiddleReminder->ClearMessage();
	m_pAboveReminder->ClearMessage();
	m_pFloatReminder->ClearMessage();
	m_pUnderReminder->ClearMessage();
	m_bPromptOn = FALSE;
}

//-------------------------------------------------------------------
// ��  ����������ʾ��Ϣ 
//-------------------------------------------------------------------
void CSysMsgReceiver::SetPromptOn(void)
{
	m_bPromptOn = TRUE;
}

//-------------------------------------------------------------------
// ��  ������ϵͳ��Ϣ
//-------------------------------------------------------------------
void CSysMsgReceiver::OnSysMsgDraw(ICanvas * pCanvas)
{
	m_pMiddleReminder->OnDraw(pCanvas);
	m_pAboveReminder->OnDraw(pCanvas);
	m_pFloatReminder->OnDraw(pCanvas);
	m_pUnderReminder->OnDraw(pCanvas);
}

//------------------------------------------------------------------------
void CSysMsgReceiver::Clear(int nType)
{
	switch(nType)
	{
	case -1:
		{
           if( m_pFloatReminder )
		   {
		      m_pFloatReminder->ClearMessage();
		   }
           if( m_pAboveReminder )
		   {
			   m_pAboveReminder->ClearMessage();
		   }
           if( m_pMiddleReminder )
		   {
			   m_pMiddleReminder->ClearMessage();
		   }
           if( m_pUnderReminder )
		   {
			   m_pUnderReminder->ClearMessage();
		   }
           break;
		}
	default:
		{
           break;
		}
	}
}
//------------------------------------------------------------------------