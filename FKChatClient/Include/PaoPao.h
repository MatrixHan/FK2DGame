/**
*	created:		2012-6-28   9:02
*	filename: 		PaoPao
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ͷ�ϵ�����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "XChatEx.h"
#include "MessageBuilder.h"
#include "../IPaoPao.h"
//------------------------------------------------------------------------
const int PAOPAO_TIMEOUT = 15000;		// ����ͣ����ʱ��
const int PAOPAO_MAX_WIDTH = 100;		// ���ݵ������
const int PAOPAO_MAX_HEIGHT = 10000;	// ���ݵ����߶�
//------------------------------------------------------------------------
#define PAOPAO_BACK_COLOR		WPixel(0,0,0)
#define PAOPAO_BACK_ALPHA		80
//------------------------------------------------------------------------
// ����ͷ�ϵ�����
class CPaoPao : public IPaoPao,
                public ITimerSink
{
public:
	CPaoPao(UID uid);
	virtual ~CPaoPao(void);

public:
	virtual BOOL SetText(LPCSTR lpszText);
	virtual void OnDraw(ICanvas * pCanvas, const POINT & ptBottomCenter, const char * szName, const WPixel & clrName, ITTFont * pNameFont);
	virtual void Release(void);
	virtual void OnTimer(DWORD dwEventID);

private:
	XItemContainer m_container;
	CMessageBuilder m_msgBuilder;
	UID m_uidCreature;
};
//------------------------------------------------------------------------