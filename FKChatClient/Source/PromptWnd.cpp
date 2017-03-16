/**
*	created:		2012-6-28   9:04
*	filename: 		PromptWnd
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ��ϵͳ��ʾ��Ϣ����
*/
//------------------------------------------------------------------------
#include "../Include/PromptWnd.h"
#include "../include/WindowCreator.h"
//------------------------------------------------------------------------
CPromptWnd::CPromptWnd(void)
{
	m_nType = REMINDER_ADD;
	m_clrText = WPixel(255, 255, 255);
	
	// ��������
	if (m_pFont != NULL)
	{
		XFontCreate::GetInstance()->ReleaseFont(m_pFont);
	}	

	// ��������
	m_pFont = XFontCreate::GetInstance()->CreateFont("����", 14, TRUE, FALSE, FALSE, TRUE);	

	if (m_pFont != NULL)
	{
		m_nLineHeight = m_pFont->GetHeight();
	}
}
//------------------------------------------------------------------------
CPromptWnd::~CPromptWnd(void)
{
}
//------------------------------------------------------------------------
void CPromptWnd::SetWindowPos(RECT& rc)
{	
	XMainWnd::SetWindowPos(rc);
	CReminder::SetPos(rc);
}

//-------------------------------------------------------------------
// ��  ������ͼ 
//-------------------------------------------------------------------
void CPromptWnd::OnDraw(ICanvas * pCanvas, RECT & rcClip)
{	
	if (pCanvas == NULL)
	{
		return;
	}

	RECT rc = m_rcWnd;
	int nPos = m_rcWnd.top;	
	
	// ��ʾ���е���Ϣ��
	for (MessageListItr itr = m_messages.begin(); itr != m_messages.end(); itr++)
	{  
		rc.top = nPos;
		rc.bottom = rc.top + m_nLineHeight;

		pCanvas->DrawClipText(m_pFont, m_rcWnd.left + 3, nPos, 
			(*itr).strText.c_str(), (*itr).clrText, rc, FONT_NOALPHA);

		nPos += m_nLineHeight;
	}
}

//-------------------------------------------------------------------
// ��  ������������ 
//-------------------------------------------------------------------
DWORD CPromptWnd::OnMouseDown(UINT nButton, UINT nShift, POINT pt)
{
	if (m_messages.size() > 0)
	{
		//ClearMessage();
		return 0;
	}
	else
	{
		return NOTHANDLE;
	}	
}

//-------------------------------------------------------------------
// ��  ���������ͷ� 
//-------------------------------------------------------------------
DWORD CPromptWnd::OnMouseUp(UINT nButton, UINT nShift, POINT pt)
{
	return NOTHANDLE;
}

//-------------------------------------------------------------------
// ��  �����Ƿ�͸��
//-------------------------------------------------------------------
BOOL CPromptWnd::IsTransparent(POINT & pt)
{
	return TRUE;
}

//-------------------------------------------------------------------
// ��  ��������ϵͳ��ʾ��Ϣ���� 
//-------------------------------------------------------------------
BOOL CPromptWnd::Create(IResObject * pResObject)
{
	return CWindowCreator::CreateMainWnd(NULL, this, pResObject);
}

//-------------------------------------------------------------------
// ��  �������һ����ʾ��Ϣ
// ��  ����[����] LPCSTR lpszText - �ı�
// ����ֵ��void
//-------------------------------------------------------------------
void CPromptWnd::AddMessage(LPCSTR lpszText)
{
	if (lpszText == NULL)
	{
		return;
	}

	m_messages.clear();

	// ����б�Ҫ������Ϣ�ֳɶ���
	while (strlen(lpszText) != 0)
	{
		string strLine = GetLine(&lpszText);
		AddLine(strLine);
	} 
	
	if (m_messages.size() > 0)
	{
		m_rcWnd.bottom = m_rcWnd.top + m_nLineHeight * m_messages.size();
		SetAlpha(50);
		SetWindowPos(m_rcWnd);
	}
	else
	{
		SetAlpha(0);
	}

}
//------------------------------------------------------------------------
void CPromptWnd::ClearMessage(void)
{
	m_messages.clear();
	SetAlpha(0);
}

//-------------------------------------------------------------------
// ��  �������һ����ʾ��Ϣ
// ��  ����[����] string strText - �ı�
// ����ֵ��void
//-------------------------------------------------------------------
void CPromptWnd::AddLine(string strText)
{
	// ������Ϣ��ʾ��λ��
	for (MessageListItr itr = m_messages.begin(); itr != m_messages.end(); itr++)
	{
		(*itr).nPos -= m_nLineHeight;
	}
	
	SMessage message;
	message.strText = strText;
	message.clrText = m_clrText;
	message.nPos = m_rcShow.bottom - m_nLineHeight;
	
	m_messages.push_back(message);
}

//-------------------------------------------------------------------
// ��  ������ʾ���� 
//-------------------------------------------------------------------
void CPromptWnd::ShowWindow(BOOL bShow)
{
	SetVisible(bShow);
}
//------------------------------------------------------------------------