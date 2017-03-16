/**
*	created:		2012-6-28   9:03
*	filename: 		PaoPao
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ������ͷ�ϵ�����
*/
//------------------------------------------------------------------------
#include "../Include/PaoPao.h"
#include "../Include/ChatClient.h"
#include "../../FKGame/Common/CreatureViewDef.h"
#include "../../FKGame/MapView/MapItemView.h"
//------------------------------------------------------------------------
CPaoPao::CPaoPao(UID uid)
: m_uidCreature(uid)
{
	RECT rc = {0, 0, PAOPAO_MAX_WIDTH, 100};
	m_container.SetWindowPos(rc);
	m_container.SetAutoWrap(TRUE);
	m_container.SetMaxWidth(PAOPAO_MAX_WIDTH);
	m_container.SetMaxHeight(PAOPAO_MAX_HEIGHT);	
	m_container.SetVisible(TRUE);

	m_msgBuilder.SetContainer(&m_container);
}
//------------------------------------------------------------------------
CPaoPao::~CPaoPao(void)
{
	CChatClient::Instance()->GetTimeAxis()->KillTimer(0, static_cast<ITimerSink*>(this));
}

//-------------------------------------------------------------------
// ��  ������������Ҫ��ʾ���ı�
//-------------------------------------------------------------------
BOOL CPaoPao::SetText(LPCSTR lpszText)
{	
	CChatClient::Instance()->GetTimeAxis()->KillTimer(0, static_cast<ITimerSink*>(this));	

	// ����Ҫ��ʾ������
	m_container.Clear();
	m_msgBuilder.Build(lpszText);
	
	CChatClient::Instance()->GetTimeAxis()->SetTimer(0, static_cast<ITimerSink*>(this), PAOPAO_TIMEOUT, "CPaoPao::SetText"); 

	return TRUE;
}

void CPaoPao::OnDraw(ICanvas * pCanvas, const POINT & ptBottomCenter, const char * szName, const WPixel & clrName, ITTFont * pNameFont)
{
	RECT rcContainer;
	rcContainer.left = ptBottomCenter.x - PAOPAO_MAX_WIDTH / 2;
	rcContainer.right = rcContainer.left + PAOPAO_MAX_WIDTH;
	rcContainer.bottom = ptBottomCenter.y;
	rcContainer.top = rcContainer.bottom - m_container.GetMaxHeight();

	RECT rcBox = rcContainer;
	rcBox.top = rcContainer.top - pNameFont->GetHeight();

	// ���Ʊ���
	pCanvas->DrawBox(rcBox, PAOPAO_BACK_COLOR, BLIT_TRANS, PAOPAO_BACK_ALPHA);

	// ��������
	pCanvas->DrawText(pNameFont, rcBox.left, rcBox.top, szName, clrName, FONT_NOALPHA);
	pCanvas->DrawText(pNameFont, rcBox.left + pNameFont->GetLength(szName), rcBox.top, "��", 
		              clrName, FONT_NOALPHA);	

	// ��������
	m_container.SetWindowPos(rcContainer);
	m_container.OnDraw(pCanvas, rcContainer);
}

//-------------------------------------------------------------------
// ��  ������������ 
//-------------------------------------------------------------------
void CPaoPao::Release(void)
{
	delete this;
}

void CPaoPao::OnTimer(DWORD dwEventID)
{
	CChatClient::Instance()->GetTimeAxis()->KillTimer(dwEventID, static_cast<ITimerSink*>(this));
	m_container.Clear();

	ICreature * pCreature = CChatClient::Instance()->GetCreature(m_uidCreature);
	if (pCreature != NULL && pCreature->GetMapItem() != NULL)
	{		
		CMapItemView * pMapItemView = pCreature->GetMapItem();
		
		// �������
		IPaoPao ** ppPaoPao = NULL;
		pMapItemView->OnCommand(CrtCmdChatInfo, (DWORD)&ppPaoPao, 0);
		if (ppPaoPao != NULL)
		{
			*ppPaoPao = NULL;
			Release();
		}
	}				
}
//------------------------------------------------------------------------