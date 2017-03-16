/**
*	created:		2012-6-21   22:23
*	filename: 		CommonFunc
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../MapView/IMapView.h"
#include "../Controller/IController.h"
//------------------------------------------------------------------------
class CCommonFunc  
{
public:
	// �ر���Ϸ����
	void		CloseGameWorld(void);
	// ��������Ϸ����
	BOOL		CreateNewWorld(int nMapID, POINT ptWorld);
	// �����ͼ
	BOOL		LoadMap(int nMapID, POINT ptWorld);
	// ����һ��ͼƬ
	IBitmap *	LoadBitmap(const char *szFileName);
	// �����ͼ
	char*		LoadFile(const char *szFileName);
	// ����
	BOOL		Create(void);
	// �ر�
	void		Close(void);
	// С��,���������Դ
	void		ClearMemoryResource(void);
public:
	CCommonFunc(void);
	~CCommonFunc(void);
public:
	CMapViewHelper		m_MapViewHelper;
	CControllerHelper	m_ControllerHelper;
	int					m_nMapID;
};
//------------------------------------------------------------------------
extern void			MsgBox(LPCSTR szMsg,LPCSTR szCaption = "FKWarning", DWORD uType = MB_OK | MB_ICONWARNING);
extern BOOL			GetCursorPointTile(POINT& ptTile);
//------------------------------------------------------------------------