/**
*	created:		2012-6-28   8:45
*	filename: 		InfoWindow
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		װ��������ʾ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
// װ��������ʾ����
class CInfoWindow : public XMainWnd  
{
public:
	CInfoWindow();
	virtual ~CInfoWindow();

public:
	bool SetInfo(string strInfo);
	void ClearInfo(void);
	bool Create(IResObject * pResObject);
	virtual void SetWindowPos(RECT & rc);
	virtual void OnActivate(BOOL bActive);
	virtual void SetVisible(BOOL bVisible = TRUE);

private:
	XHtml * m_pHtml;
};
//------------------------------------------------------------------------