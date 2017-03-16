/**
*	created:		2012-7-1   5:51
*	filename: 		Weather
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ϵͳ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKCanvas/IFKCanvas.h"
#include "../../FKGame/MapView/IMapView.h"
#include "../../FKGame/Thread/IThreadRoute.h"
#include "WeatherParticle.h"
//------------------------------------------------------------------------
#define MAX_WEATHER_PARTICLE_NUM	1000
//------------------------------------------------------------------------
// ����������
class CWeather
{
public:
	CWeather();
	virtual ~CWeather();
public:
	BOOL	Create(LPCSTR szDir);
	void	Release() {delete this;}

	void	ChangeWeather(SWeatherInfo info);
	BOOL	NeedDraw();
	void	OnDraw(ICanvas* pCanvas, int nLocX, int nLocY);
public:
	enWeatherType		m_nWeatherType;								// ��ǰ��������

	BOOL				m_bRandom;									// �Ƿ����
	int					m_nNextParticleNum;							// ��ǰ������
	int					m_nCurParticleNum;							// ��ǰ������
	int					m_nWindPower;								// ����
	int					m_nDownSpeed;								// �½��ٶ�
	CRainParticle		m_RainParticle[MAX_WEATHER_PARTICLE_NUM];	// ��������
	CSnowParticle		m_SnowParticle[MAX_WEATHER_PARTICLE_NUM];	// ѩ����
	CMeiGuiParticle		m_MeiGuiParticle[MAX_WEATHER_PARTICLE_NUM];	// õ�廨��

	IBitmap*			m_pMeiGui[5];
};
//------------------------------------------------------------------------