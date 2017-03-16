/**
*	created:		2012-7-2   3:49
*	filename: 		ActorTitle
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��ҳƺŹ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#define COLORSIGN	'#'
#define MAX_TITLE_LENGTH	36
//------------------------------------------------------------------------
// �ַ�������
struct SCharConext
{
	char wChar[3]; // �ַ�
	WPixel color;
	int	nWidth; // �ַ������ؿ��
};
//------------------------------------------------------------------------
typedef vector<SCharConext>	NodeList;
typedef vector<WPixel>		ColorList;
//------------------------------------------------------------------------
// �ƺ�Ч��
// ����
class CTitleEffect
{
public:
	virtual BOOL	Create(NodeList& CharList)	{return FALSE;}
	virtual void	Close()		{}
	virtual void	OnDraw(ICanvas* pCanvas, POINT& ptBottomCenter, NodeList& CharList)	{}
	virtual int		GetWidth()	{return 0;}
	virtual int		GetHeight() {return 0;}
	virtual void	Release()	{Close();delete this;}

protected:
	void DrawRect(ICanvas* pCanvas, RECT& rc, WPixel clr);
	void DrawBox(ICanvas* pCanvas, RECT& rc, WPixel clr, int alpha);
};
//------------------------------------------------------------------------
// ��������Ч���ƺ�
class CTitleEffect0 : public CTitleEffect
{
public:
	CTitleEffect0();

	virtual BOOL	Create(NodeList& CharList);
	virtual void	OnDraw(ICanvas* pCanvas, POINT& ptBottomCenter, NodeList& CharList);
	virtual int		GetWidth()	{return m_nWidth;}
	virtual int		GetHeight() {return m_nHeight;}

protected:
	int m_nWidth;
	int m_nHeight;
	int m_nCharCount;
	ITTFont* m_pFont;
};
//------------------------------------------------------------------------
// ����Ч��
class CTitleEffect1 : public CTitleEffect0
{
public:
	CTitleEffect1();

	virtual void OnDraw(ICanvas* pCanvas, POINT& ptBottomCenter, NodeList& CharList);
};
//------------------------------------------------------------------------
// ����½������Ч��
class CTitleEffect2 : public CTitleEffect0, public ITimerSink
{
public:
	CTitleEffect2();
	virtual ~CTitleEffect2();
	
	virtual BOOL Create(NodeList& CharList);
	virtual void Close();
	virtual void OnDraw(ICanvas* pCanvas, POINT& ptBottomCenter, NodeList& CharList);
	virtual void OnTimer(DWORD dwEventID);

protected:
	int m_nTimerCount;
	int m_nState;
};
//------------------------------------------------------------------------
// ����½����ɫЧ��
class CTitleEffect3 : public CTitleEffect0, public ITimerSink
{
public:
	CTitleEffect3();
	virtual ~CTitleEffect3();

	virtual BOOL Create(NodeList& CharList);
	virtual void Close();
	virtual void OnDraw(ICanvas* pCanvas, POINT& ptBottomCenter, NodeList& CharList);
	virtual void OnTimer(DWORD dwEventID);

protected:
	int m_nTimerCount;
};
//------------------------------------------------------------------------
// �ƺŶ���
class CTitleObj
{
public:
	CTitleObj() : pTitleEffect(NULL) {}

	void Close();
	void OnDraw(ICanvas* pCanvas, POINT& ptBottomCenter);

public:
	NodeList		CharList;
	CTitleEffect*	pTitleEffect;
};
//------------------------------------------------------------------------
// ��ɫ�ƺ�
class CActorTitle  
{
public:
	CActorTitle();
	virtual ~CActorTitle();

	int		AddTitle(LPCSTR szFormatStr, DWORD dwEffectID);
	void	OnDraw(ICanvas* pCanvas, POINT& ptBottomCenter);
	void	ClearTitle();
	int		ParseFormatString(LPCSTR szFormatStr, CTitleObj& to);

public:
	vector<CTitleObj>	m_TitleList;
	static ColorList	m_ColorList;

};
//------------------------------------------------------------------------
// Ч������
class CTitleEffectFactory
{
public:
	CTitleEffect* CreateTitleEffect(DWORD dwEffectID);
	
	CTitleEffect* CreateEffect0();
	CTitleEffect* CreateEffect1();
	CTitleEffect* CreateEffect2();
	CTitleEffect* CreateEffect3();

public:
	typedef CTitleEffect* (CTitleEffectFactory::*ProcCreator)();
	static ProcCreator m_fnCreateEffect[];
};
//------------------------------------------------------------------------