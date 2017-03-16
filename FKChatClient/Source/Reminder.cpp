/**
*	created:		2012-6-28   9:07
*	filename: 		Reminder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ��ϵͳ��Ϣ��ʾ��
*/
//------------------------------------------------------------------------
#include "../Include/Reminder.h"
#include "../Include/ChatClient.h"
//------------------------------------------------------------------------
CReminder::CReminder(void)
: m_nType(REMINDER_ROLL)
, m_nLineHeight(15)
, m_clrText(WPixel(255, 255, 100))
, m_pFont(NULL)
{
	SetRect(&m_rcShow, 0, 0, 0, 0);

	// ��������
	m_pFont = XFontCreate::GetInstance()->CreateFont("����", 12, FALSE, FALSE, FALSE, TRUE);
}
//------------------------------------------------------------------------
CReminder::~CReminder(void)
{
	// �رն�ʱ��
	ITimeAxis * pTimeAxis = CChatClient::Instance()->GetTimeAxis();
	if (pTimeAxis != NULL)
	{
		pTimeAxis->KillTimer(0, static_cast<ITimerSink*>(this));
	}	

	// ��������
	XFontCreate::GetInstance()->ReleaseFont(m_pFont);
}
//------------------------------------------------------------------------
void CReminder::OnTimer(DWORD dwEventID)
{
	ITimeAxis * pTimeAxis = CChatClient::Instance()->GetTimeAxis();
	
	if (m_nType == REMINDER_ADD)
	{
		if (m_messages.size() > 0)
		{
			m_messages.pop_front();
		}

		if (m_messages.size() == 0 && pTimeAxis != NULL)
		{
			pTimeAxis->KillTimer(dwEventID, static_cast<ITimerSink *>(this));
		}
	}
	else if (m_nType == REMINDER_REPLACE)
	{
		m_messages.clear();

		if (pTimeAxis != NULL)
		{
			pTimeAxis->KillTimer(dwEventID, static_cast<ITimerSink *>(this));
		}
	}
	else
	{
		AutoAdjust();

		if (m_messages.size() == 0 && pTimeAxis != NULL)
		{
			pTimeAxis->KillTimer(dwEventID, static_cast<ITimerSink *>(this));
		}
	}	
}
//------------------------------------------------------------------------
void CReminder::OnDraw(ICanvas * pCanvas)
{
	RECT rc;
	
	if (pCanvas == NULL)
	{
		return;
	}
	
	CopyRect(&rc, &m_rcShow);	
	
	// ��ʾ���е���Ϣ
	for (MessageListItr itr = m_messages.begin(); itr != m_messages.end(); itr++)
	{  
		rc.top = (*itr).nPos;
		rc.bottom = rc.top + m_nLineHeight;
		
		int x = m_rcShow.left + 5;
		if (m_nType == REMINDER_ADD || m_nType == REMINDER_REPLACE)
		{
			int nOffset = (m_rcShow.right - m_rcShow.left - m_pFont->GetLength((*itr).strText.c_str())) / 2;
			x = m_rcShow.left + nOffset;
		}

		pCanvas->DrawClipText(m_pFont, x, (*itr).nPos, 
			(*itr).strText.c_str(), (*itr).clrText, rc, FONT_NOALPHA);
		
	}
}
//------------------------------------------------------------------------
void CReminder::SetPos(RECT & rc)
{
	m_rcShow = rc;
}
//------------------------------------------------------------------------
void CReminder::SetType(int nType)
{
	m_nType = nType;
}
//------------------------------------------------------------------------
void CReminder::AddMessage(LPCSTR lpszText)
{
	if (lpszText == NULL)
	{
		return;
	}

	ITimeAxis * pTimeAxis = CChatClient::Instance()->GetTimeAxis();
	if (pTimeAxis == NULL)
	{
		return;
	}

	if (m_nType == REMINDER_ROLL)
	{
		if (m_messages.size() == 0)
		{
			pTimeAxis->SetTimer(0, static_cast<ITimerSink *>(this), 150, "CReminder::AddMessage");
		}		
	}
	else if (m_nType == REMINDER_REPLACE)
	{
		if (m_messages.size() != 0)
		{
			pTimeAxis->KillTimer(0, static_cast<ITimerSink *>(this));
			m_messages.clear();
		}
		
		pTimeAxis->SetTimer(0, static_cast<ITimerSink *>(this), 6000, "CReminder::AddMessage");
	}
	else
	{
		if (m_messages.size() == 0)
		{
			pTimeAxis->SetTimer(0, static_cast<ITimerSink *>(this), 6000, "CReminder::AddMessage");
		}
	}

	// ����б�Ҫ������Ϣ�ֳɶ���
	while (strlen(lpszText) != 0)
	{
		string strLine = GetLine(&lpszText);
		AddLine(strLine);
	} 
}
//------------------------------------------------------------------------
void CReminder::ClearMessage(void)
{
	m_messages.clear();
}

//-------------------------------------------------------------------
// ��  ������ȡһ���ı�
//-------------------------------------------------------------------
string CReminder::GetLine(LPCSTR * ppText)
{	
	WORD wWord;	
	int nLen = 0;
	int nLineLen = m_rcShow.right - m_rcShow.left - 5;
	LPCSTR pText = *ppText;		
	BOOL bChinese = FALSE;
	
	while (*pText != '\0')
	{
		wWord = *pText;

		// ����ǲ��Ǻ���
		if (isascii(wWord))
		{
			bChinese = FALSE;
		}
		else
		{
			if (*(pText + 1) == '\0')
			{
				++pText;
				break;
			}
			
			bChinese = TRUE;
			memcpy(&wWord, pText, 2);			
		}	
		
		// ����ǲ����Ѿ���һ����
		nLen += m_pFont->GetCharLength(wWord);
		if (nLen > nLineLen)
		{
			break;
		}

		// ��һ���ַ�
		if (bChinese)
		{
			pText += 2;
		}
		else
		{
			pText++;
		}
	}
	
	string strLine = string(*ppText, pText - *ppText);
	*ppText = pText;
	
	return strLine;
}

//-------------------------------------------------------------------
// ��  �������һ����ʾ��Ϣ
// ��  ����[����] string strText - �ı�
// ����ֵ��void
//-------------------------------------------------------------------
void CReminder::AddLine(string strText)
{
	// ������Ϣ��ʾ��λ��
	for (MessageListItr itr = m_messages.begin(); itr != m_messages.end();)
	{
		MessageListItr preitr = itr++;

		(*preitr).nPos -= m_nLineHeight;
		if ((*preitr).nPos <= m_rcShow.top)
		{
			m_messages.erase(preitr);
		}
	}
	
	SMessage message;
	message.strText = strText;
	message.clrText = m_clrText;
	message.nPos = m_rcShow.bottom - m_nLineHeight;
	
	m_messages.push_back(message);
}

//-------------------------------------------------------------------
// ��  �����Զ�������Ϣ��λ�á���ɫ��
//-------------------------------------------------------------------
void CReminder::AutoAdjust(void)
{
	for (MessageListItr itr = m_messages.begin(); itr != m_messages.end();)
	{
		// �����Ϣ�Ƶ��˶��ˣ���ɾ��
		if ((*itr).nPos <= m_rcShow.top)
		{
			MessageListItr curitr = itr++;
			m_messages.erase(curitr);
		}
		// ������Ϣ�����ƶ�һ��
		else
		{
			(*itr).nPos--;
			
			if ((*itr).nPos - m_rcShow.top < 70)
			{
				// �ı���ʾ����ɫ
				int nValue = 255 - (70 - ((*itr).nPos - m_rcShow.top)) * 2;
				(*itr).clrText = WPixel(nValue, nValue, 100);
			}			
			
			itr++;
		}
	}	
}
//------------------------------------------------------------------------