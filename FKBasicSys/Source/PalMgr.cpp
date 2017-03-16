/**
*	created:		2012-7-2   1:13
*	filename: 		PalMgr
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/BasicCommon.h"
#include "../Include/PalMgr.h"
//------------------------------------------------------------------------
CPalMgr	g_PalMgr;
//------------------------------------------------------------------------
void CPalette::SetPalette(BYTE* pPal, int nUseColor)
{
	m_nUseColor = nUseColor;
	memcpy(m_pal, pPal, m_nUseColor<<1);
}
//------------------------------------------------------------------------
void CPalette::Modify(int nPalType)
{
	switch (nPalType) 
	{
	case PalType_Light:	Light(); break;
	case PalType_Frozen:Blue();	 break;
	case PalType_Toxic: Green(); break;
	case PalType_Berserk:Red();	 break;
	}
}
//------------------------------------------------------------------------
// ������ɫ��(�̶������ȣ����ڵ�һ������ѡ��ʱ)
void CPalette::Light()
{
	const int nLight = 4;
	const int nDblLight = nLight<<1;
	WPixel* pPal = (WPixel*)m_pal;
	for (int i=0; i<m_nUseColor; i++)
	{
		// ���ͼ�
		pPal[i].red   = min(31, pPal[i].red   + nLight);
		pPal[i].green = min(63, pPal[i].green + nDblLight);
		pPal[i].blue  = min(31, pPal[i].blue  + nLight);
	}
}
//------------------------------------------------------------------------
// �����ȡ��Աȶ�
void CPalette::Light(int nBrightness, int nContrast)
{
	WPixel* pPal = (WPixel*)m_pal;
	WPixel color;
	float c = nContrast/100.0f;
	int nDblBrightness = nBrightness<<1;
	for(int i=0; i<m_nUseColor; i++)
	{
		color = pPal[i];
		color.red	= max(0, min(31, (int)((color.red   -16)*c + 16 + nBrightness)));
		color.green	= max(0, min(63, (int)((color.green -32)*c + 32 + nDblBrightness)));
		color.blue	= max(0, min(31, (int)((color.blue  -16)*c + 16 + nBrightness)));
		pPal[i] = color;
	}
}
//------------------------------------------------------------------------
// ������ɫ��(���ڱ�����)
void CPalette::Blue()
{
	WPixel* pPal = (WPixel*)m_pal;
	for (int i=0; i<m_nUseColor; i++)
	{
		pPal[i].red   = max(0, pPal[i].red - 12);
		pPal[i].green = max(0, pPal[i].green - 24);
		pPal[i].blue  = max(0, min(31, (int)(pPal[i].blue + 24)));
	}
}
//------------------------------------------------------------------------
// ���̵�ɫ��(�����ж���)
void CPalette::Green()
{
	WPixel* pPal = (WPixel*)m_pal;
	for (int i=0; i<m_nUseColor; i++)
	{
		pPal[i].red   = 0;
		pPal[i].green = max(8, (int)pPal[i].green);
		pPal[i].blue  = 0;
	}
}
//------------------------------------------------------------------------
// ����ɫ��(���ڿ񱩵�)
void CPalette::Red()
{
	WPixel* pPal = (WPixel*)m_pal;
	for (int i=0; i<m_nUseColor; i++)
	{
		pPal[i].red   = max(4, (int)pPal[i].red);
		pPal[i].green = 0;
		pPal[i].blue  = 0;
	}
}
//------------------------------------------------------------------------