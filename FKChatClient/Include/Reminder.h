/**
*	created:		2012-6-28   9:06
*	filename: 		Reminder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ϵͳ��Ϣ��ʾ��
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <list>
#include <string>
#include "../IReminder.h"
//------------------------------------------------------------------------
using std::list;
using std::string;
//------------------------------------------------------------------------
// ϵͳ��Ϣ��ʾ��
class CReminder : public IReminder, 
                  public ITimerSink
{
public:
	CReminder(void);
	virtual ~CReminder(void);
	
public:
	virtual void OnTimer(DWORD dwEventID);
	virtual void OnDraw(ICanvas * pCanvas);	
	virtual void SetType(int nType);
	virtual void SetPos(RECT & rc);
	virtual void AddMessage(LPCSTR lpszText);	
	virtual void ClearMessage(void);

protected:
	string GetLine(LPCSTR * ppText);
	void AddLine(string strText);
	void AutoAdjust(void);

protected:	
	struct SMessage
	{
		string strText;		// ��ʾ�ı�
		WPixel clrText;		// �ı���ɫ
		int nPos;			// ��ʾλ��
	};	
	typedef list<SMessage> MessageList;
	typedef list<SMessage>::iterator MessageListItr;
	typedef list<SMessage>::const_iterator MessageListCItr;
	
	int m_nType;						// ��ʾ��������
	RECT m_rcShow;						// ��ʾ������
	int m_nLineHeight;					// ��Ϣ�еĸ߶�
	WPixel m_clrText;					// �ı��Ŀ�ʼ��ɫ
	ITTFont * m_pFont;					// ��ʾ��ʾ������
	MessageList m_messages;				// ��Ϣ�б�
};
//------------------------------------------------------------------------
