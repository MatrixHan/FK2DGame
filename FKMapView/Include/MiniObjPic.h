/**
*	created:		2012-7-1   5:01
*	filename: 		MiniObjPic
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/MapInfo/MapItem.h"
#include "../../FKCanvas/IFKCanvas.h"
//------------------------------------------------------------------------
class CMiniObjPic  
{
private:
	BOOL m_bOK;
	IBitmap *m_pPIC[PIC_MAX];
	WPixel  m_Color[PIC_MAX];
	int m_offsetX[PIC_MAX];
	int m_offsetY[PIC_MAX];

public:
	CMiniObjPic();
	virtual ~CMiniObjPic();

	BOOL Init();
	BOOL IsOK();

	IBitmap * GetPIC(DWORD dwID);
	WPixel GetColor(DWORD dwID);
	//����ֵ�����Ƿ񻭳ɹ�, �����ɹ��Ļ�, ������ǰ�Ĵ���
	BOOL DrawPIC(ICanvas* pCanvas, int xPos, int yPos, DWORD dwPICID);
};
//------------------------------------------------------------------------
extern CMiniObjPic g_MiniObjPic;
//------------------------------------------------------------------------