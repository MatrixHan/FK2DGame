/**
*	created:		2012-7-2   1:11
*	filename: 		Pal
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���ȡ��Աȶȡ����ͶȺ���ɫ��ת��
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKCanvas/IFKCanvas.h"
#include "Color.h"
#include "PalMgr.h"
//------------------------------------------------------------------------
#define MAX_PAL_COUNT	256
//------------------------------------------------------------------------
class CPal
{
public:
	WPixel		m_Pal[MAX_PAL_COUNT];		// ��ɫ��
	WPixel		m_CurPal[MAX_PAL_COUNT];	// ��ǰ�ĵ�ɫ��
	int			m_nUseColor;				// ʹ�õ���ɫ��
	
	int			m_nCurPalType;				// ��ǰ��ɫ��
public:
	struct rgb
	{
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE a;
	};
public:
	CPal()
	{
		memset(m_Pal, 0, sizeof(m_Pal));
		m_nUseColor = 0;
		m_nCurPalType = -1;
	}
	virtual ~CPal()
	{
		
	}
	void SetPal(BYTE* pPal, int nUseClr) // ֻ֧��565��ʽ
	{
		ClearPal();
		m_nUseColor = nUseClr;
		memcpy(m_Pal, pPal, nUseClr*2);

		if(m_nCurPalType >= 0)
		{
			ModifyCurPal();
		}
	}
	void ClearPal()
	{
		memset(m_Pal, 0, sizeof(m_Pal));
		m_nUseColor = 0;
	}
	int GetUseColor()
	{
		return m_nUseColor;
	}
	void OnHSLChanged(int nLit, int nHue, int nSat)
	{
		CColor clr;
		int hue, sat, lit;
		BYTE r, g, b;
		for (int i=0; i<m_nUseColor; i++)
		{
			DWORD dwColor = m_Pal[i];
			rgb c = *(rgb*)(&dwColor);
			clr.RGBtoHSL(c.r, c.g, c.b, (UINT&)hue, (UINT&)sat, (UINT&)lit);
			
			hue += nHue;
			if (hue>360) hue-=360;
			if (hue<0) hue+=360;
			sat += nSat;
			if (sat>255) sat=255;
			if (sat<0) sat=0;

			clr.HSLtoRGB(hue, sat, lit, r, g, b);
				
			m_Pal[i] = WPixel(r, g, b);
			
			m_nCurPalType = -1;
		}
	}
	BYTE* GetPal(int nCurPalType = -1)
	{
		if(nCurPalType > 0)
		{
			if(nCurPalType != m_nCurPalType)
			{
				m_nCurPalType = nCurPalType;
				ModifyCurPal();
			}
			return (BYTE*)m_CurPal;
		}
		return (BYTE*)m_Pal;
	}
	void ModifyCurPal()
	{
		switch(m_nCurPalType) 
		{
		case PalType_Frozen:		// ����Ч��
			{
				CColor clr;
				int hue, sat, lit;
				BYTE r, g, b;
				for (int i = 0; i < m_nUseColor; i++)
				{
					DWORD dwColor = m_Pal[i];
					rgb c = *(rgb*)(&dwColor);
					clr.RGBtoHSL(c.r, c.g, c.b, (UINT&)hue, (UINT&)sat, (UINT&)lit);					
					hue = 220;
					sat = 255;
					clr.HSLtoRGB(hue, sat, lit, r, g, b);
						
					m_CurPal[i] = WPixel(r, g, b);
				}
			}
			break;
		case PalType_Toxic:			// �ж�Ч��
			{
				CColor clr;
				int hue, sat, lit;
				BYTE r, g, b;
				for (int i = 0; i < m_nUseColor; i++)
				{
					DWORD dwColor = m_Pal[i];
					rgb c = *(rgb*)(&dwColor);
					clr.RGBtoHSL(c.r, c.g, c.b, (UINT&)hue, (UINT&)sat, (UINT&)lit);					
					hue = 130;
					sat = 255;
					clr.HSLtoRGB(hue, sat, lit, r, g, b);
						
					m_CurPal[i] = WPixel(r, g, b);
				}
			}
			break;
		case PalType_Berserk:		// ��Ч��
			{
				CColor clr;
				int hue, sat, lit;
				BYTE r, g, b;
				for (int i = 0; i < m_nUseColor; i++)
				{
					DWORD dwColor = m_Pal[i];
					rgb c = *(rgb*)(&dwColor);
					clr.RGBtoHSL(c.r, c.g, c.b, (UINT&)hue, (UINT&)sat, (UINT&)lit);					
					hue = 0;
					sat = 255;
					clr.HSLtoRGB(hue, sat, lit, r, g, b);
						
					m_CurPal[i] = WPixel(r, g, b);
				}
			}
			break;
		case PalType_Stone:		// ʯ��
			{
				CColor clr;
				int hue, sat, lit;
				BYTE r, g, b;
				for (int i = 0; i < m_nUseColor; i++)
				{
					DWORD dwColor = m_Pal[i];
					rgb c = *(rgb*)(&dwColor);
					clr.RGBtoHSL(c.r, c.g, c.b, (UINT&)hue, (UINT&)sat, (UINT&)lit);
					sat = 0;
					clr.HSLtoRGB(hue, sat, lit, r, g, b);
						
					m_CurPal[i] = WPixel(r, g, b);
				}
			}
			break;
		default:
			break;
		}
	}
};
//------------------------------------------------------------------------