/**
*	created:		2012-6-28   2:27
*	filename: 		XItemPlayer
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ��������ϵ������ʾ
*/
//------------------------------------------------------------------------
#include "../Include/ChatClient.h"
#include "../Include/XItemPlayer.h"
//------------------------------------------------------------------------
XItemPlayer::XItemPlayer(void)
{
}
//------------------------------------------------------------------------
XItemPlayer::~XItemPlayer(void)
{
}
//------------------------------------------------------------------------
void XItemPlayer::Emit(void)
{
	// �Ѹ������Ϊ˽�Ķ���
	CChatClient::Instance()->m_controller.SetChatPlayer(m_strText);
	m_bVisited = FALSE;
}
//------------------------------------------------------------------------
DWORD XItemPlayer::OnMouseUp(UINT nButton, UINT nShift, POINT pt)
{
	// ���������ҽ�
	if( nButton == RightButton )
	{
		if( m_strText == "ϵͳ" )
		{
			return XItemLinkText::OnMouseUp(nButton, nShift, pt);
		}
		CChatClient::Instance()->ShowMenu(pt,m_strText.c_str());  
		return 0;
	}
	return XItemLinkText::OnMouseUp(nButton, nShift, pt);
}
//------------------------------------------------------------------------