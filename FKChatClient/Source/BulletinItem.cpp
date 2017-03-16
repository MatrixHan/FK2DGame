/**
*	created:		2012-6-28   2:51
*	filename: 		BulletinItem
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ά��������
*/
//------------------------------------------------------------------------
#include "../Include/BulletinItem.h"
//------------------------------------------------------------------------
// ��������֮��ļ��
const int CBulletinItem::ITEM_SPACE = 20;
//------------------------------------------------------------------------
CBulletinItem::CBulletinItem(void)
: m_pFont(NULL)
, m_clrText(WPixel(255,255,0))
, m_nLength(0)
, m_bClicked(FALSE)
, m_nScrollCount(1)
, m_nScrolledCount(0)
, m_nState(STATE_NORMAL)
{
}
//------------------------------------------------------------------------
CBulletinItem::~CBulletinItem(void)
{
}
//-------------------------------------------------------------------
// ��  �������û���ʱʹ�õ����� 
//-------------------------------------------------------------------
void CBulletinItem::SetFont(ITTFont * pFont)
{
	m_pFont = pFont;
}

//-------------------------------------------------------------------
// ��  �������ù�����ı� 
//-------------------------------------------------------------------
void CBulletinItem::SetText(string strText)
{
	m_strText = strText;

	if (m_pFont != NULL)
	{
		m_nLength = m_pFont->GetLength(strText.c_str()) + ITEM_SPACE;
	}
}

void CBulletinItem::SetTextColor(WPixel clrText)
{
	m_clrText = clrText;
}

//-------------------------------------------------------------------
// ��  ������������ 
//-------------------------------------------------------------------
void CBulletinItem::SetUrl(string strUrl)
{
	m_strUrl = strUrl;
}

//-------------------------------------------------------------------
// ��  �������ù����Ĵ��� 
//-------------------------------------------------------------------
void CBulletinItem::SetScrollCount(int nCount)
{
	m_nScrollCount = nCount;
}

//-------------------------------------------------------------------
// ��  ������ȡ����ĳ��� 
//-------------------------------------------------------------------
int CBulletinItem::GetLength(void)
{
	return m_nLength;
}

//-------------------------------------------------------------------
// ��  �����Ƿ������ָ�������Ĺ���
//-------------------------------------------------------------------
BOOL CBulletinItem::IsFinished(void)
{
	return (m_nScrolledCount >= m_nScrollCount);
}

//-------------------------------------------------------------------
// ��  ��������Ƶ����� 
//-------------------------------------------------------------------
void CBulletinItem::MouseOver(BOOL bOver)
{
	if (bOver)
	{
		m_nState = STATE_MOUSEOVER;
	}
	else
	{
		if (m_bClicked)
		{
			m_nState = STATE_CLICKED;
		}
		else
		{
			m_nState = STATE_NORMAL;
		}
	}
}

//-------------------------------------------------------------------
// ��  ������������Ŀ 
//-------------------------------------------------------------------
void CBulletinItem::Click(void)
{
	m_bClicked = TRUE;
	m_nState = STATE_CLICKED;

	if (!(m_strUrl.empty()))
	{
		::ShellExecute(NULL, "open", m_strUrl.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
}

//-------------------------------------------------------------------
// ��  �������һ�ι���
//-------------------------------------------------------------------
void CBulletinItem::Scrolled(void)
{
	m_nScrolledCount++;
}

//-------------------------------------------------------------------
// ��  ���������Լ�
//-------------------------------------------------------------------
void CBulletinItem::OnDraw(ICanvas* pCanvas, int x, int y, RECT& rcClip)
{
	if (pCanvas == NULL)
	{
		return;
	}

	switch (m_nState)
	{
	case STATE_NORMAL:
		pCanvas->DrawClipText(m_pFont, x, y, m_strText.c_str(), m_clrText, rcClip, FONT_NOALPHA);
		break;

	case STATE_MOUSEOVER:
		pCanvas->DrawClipText(m_pFont, x, y, m_strText.c_str(), COLOR_MOUSEOVER, rcClip, FONT_NOALPHA);
		break;

	case STATE_CLICKED:
		pCanvas->DrawClipText(m_pFont, x, y, m_strText.c_str(), COLOR_CLICKED, rcClip, FONT_NOALPHA);
		break;

	default:
		break;
	}
}
//------------------------------------------------------------------------