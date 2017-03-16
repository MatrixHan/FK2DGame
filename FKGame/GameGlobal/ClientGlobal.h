/**
*	created:		2012-6-21   0:22
*	filename: 		ClientGlobal
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IClientGlobal.h"
#include "../ClientWorld/CClientWorld.h"
//------------------------------------------------------------------------
class CClientGlobal : public IClientGlobal
{
public:
	CClientGlobal(void);
	~CClientGlobal(void);
public:
	// ����
	BOOL			Create(HINSTANCE hInstance, LPCSTR szWorkDir, HWND hMainWnd, ICanvas * pScreen, ITTFont * pDefFont, ITTFont * pDefFontWithOutline, ITrace * pTrace);
	// �ر�
	void			Close(void);
	// ����ģ��
	virtual IChatClient *			GetChatClient(void);
	// �ؼ��ֹ���
	virtual IKeywordFilter *		GetKeywordFilter(void);
	// ���
	virtual ITeamClient *			GetTeamClient(void);
	// Ч������
	virtual IEffectFactory *		GetEffectFactory(void);
	// �ͻ��˿�����
	virtual IController *			GetController(void);
	// ����ģ��
	virtual IFriendClient *			GetFriendClient(void);
	// ��������
	virtual ISound *				GetSoundEngine(void);
public:
	CClientWorld	m_ClientWorld;
};
//------------------------------------------------------------------------