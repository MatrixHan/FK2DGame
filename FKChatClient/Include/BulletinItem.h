/**
*	created:		2012-6-28   2:51
*	filename: 		BulletinItem
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKCanvas/IFKCanvas.h"
#include <string>
using std::string;
//------------------------------------------------------------------------
// ����ƶ������ɫ
#define COLOR_MOUSEOVER		WPixel(255,128,128)
// ���������ɫ
#define COLOR_CLICKED		WPixel(128,128,255)
//------------------------------------------------------------------------
// ������
class CBulletinItem  
{
public:
	CBulletinItem(void);
	virtual ~CBulletinItem(void);

public:
	void OnDraw(ICanvas* pCanvas, int x, int y, RECT& rcClip);
	void Click(void);
	void Scrolled(void);
	void MouseOver(BOOL bOver);
	BOOL IsFinished(void);
	int GetLength(void);
	void SetScrollCount(int nCount);
	void SetUrl(string strUrl);
	void SetText(string strText);
	void SetTextColor(WPixel clrText);
	void SetFont(ITTFont * pFont);

private:
	enum {STATE_NORMAL, STATE_MOUSEOVER, STATE_CLICKED};
	static const int ITEM_SPACE;	// ����
	ITTFont * m_pFont;				// ���ı�������
	WPixel m_clrText;				// �ı���ʾ����ɫ
	string m_strText;				// ������ı�
	string m_strUrl;				// ����
	int m_nLength;					// ����ĳ���
	BOOL m_bClicked;				// �ǲ��Ǳ������
	int m_nScrollCount;				// �����������
	int m_nScrolledCount;			// �Ѿ���������
	int m_nState;					// ״̬
};
//------------------------------------------------------------------------