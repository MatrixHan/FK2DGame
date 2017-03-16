/**
*	created:		2012-7-1   5:47
*	filename: 		WeatherParticle
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ϵͳ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKCanvas/IFKCanvas.h"
//------------------------------------------------------------------------
#define P_WHITE_COLOR		WPixel(255,255,255)
//------------------------------------------------------------------------
enum
{
	enWeather_Particle_State_None = 0,				// δ���ɽ׶�
	enWeather_Particle_State_Run,					// ���н׶�
	enWeather_Particle_State_Over,					// �����׶�
};
//------------------------------------------------------------------------
class CWeather;
//------------------------------------------------------------------------
// ������
class CRainParticle
{
public:
	CRainParticle();
	virtual ~CRainParticle();

	void	Create(CWeather *pWeather)	{m_pWeather = pWeather;}
	void	Stop()		{m_bRun = FALSE;}
	void	Start()		{m_bRun = TRUE;}
	void	OnDraw(ICanvas* pCanvas, int nLocX, int nLocY, int nWindPower, int nDownSpeed);

	void	OnTimer();

public:
	CWeather *m_pWeather;

	BOOL	m_bRun;				// �Ƿ�����
	float	m_fSpeedX;			// X���ٶ�
	float	m_fSpeedY;			// Y���ٶ�
	int		m_nLengthX;			// X�᳤��
	int		m_nLengthY;			// Y�᳤��
	int		m_nEndY;			// Y�����λ��
	DWORD	m_dwCreateTime;		// ����ʱ��
	DWORD	m_dwLastTime;		// �ϴ�ʱ��
	DWORD	m_dwCurLife;		// ��ǰ����ʱ��
	POINT	m_ptLoc;			// ���������ɵ�
	POINT	m_ptBeginLoc;		// ��ʼλ��
	POINT	m_ptDrawFrm;		// ����ǰ��
	POINT	m_ptDrawTo;			// ���ߺ��
	int		m_nAlpha;			// ͸��ֵ
	int		m_nState;			// ���ӵ�ǰ�����׶�
	int		m_nOverTime;		// ��������
};
//------------------------------------------------------------------------
// ѩ����
class CSnowParticle
{
public:
	CSnowParticle();
	virtual ~CSnowParticle();

	void	Create(CWeather *pWeather)	{m_pWeather = pWeather;}
	void	Stop()		{m_bRun = FALSE;}
	void	Start()		{m_bRun = TRUE;}
	void	DrawSnow(ICanvas* pCanvas, int x, int y);
	void	OnDraw(ICanvas* pCanvas, int nLocX, int nLocY, int nWindPower, int nDownSpeed);

	void	OnTimer();

public:
	CWeather *m_pWeather;

	BOOL	m_bRun;				// �Ƿ�����
	float	m_fSpeedX;			// X���ٶ�
	float	m_fSpeedY;			// Y���ٶ�
	int		m_nEndY;			// Y�����λ��
	float	m_fFlowX;			// ��ǰƮ���ƫ��
	float	m_fAddFlowX;		// ��ǰƮ���ƫ��
	DWORD	m_dwCreateTime;		// ����ʱ��
	DWORD	m_dwCurLife;		// ��ǰ����ʱ��
	POINT	m_ptLoc;			// ���������ɵ�
	POINT	m_ptBeginLoc;		// ��ʼλ��
	int		m_nAlpha;			// ͸��ֵ
	int		m_nState;			// ���ӵ�ǰ�����׶�
	int		m_nType;			// ��ǰ���������͵�ѩ��
	BOOL	m_bFirst;			// �Ƿ���ʾ��
};
//------------------------------------------------------------------------
// õ�廨��
class CMeiGuiParticle
{
public:
	CMeiGuiParticle();
	virtual ~CMeiGuiParticle();

	void	Create(CWeather *pWeather)	{m_pWeather = pWeather;}
	void	Stop()		{m_bRun = FALSE;}
	void	Start()		{m_bRun = TRUE;}
	void	DrawSnow(ICanvas* pCanvas, int x, int y);
	void	OnDraw(ICanvas* pCanvas, int nLocX, int nLocY, int nWindPower, int nDownSpeed);

	void	OnTimer();

public:
	CWeather *m_pWeather;

	BOOL	m_bRun;				// �Ƿ�����
	float	m_fSpeedX;			// X���ٶ�
	float	m_fSpeedY;			// Y���ٶ�
	int		m_nEndY;			// Y�����λ��
	float	m_fFlowX;			// ��ǰƮ���ƫ��
	float	m_fAddFlowX;		// ��ǰƮ���ƫ��
	DWORD	m_dwCreateTime;		// ����ʱ��
	DWORD	m_dwCurLife;		// ��ǰ����ʱ��
	POINT	m_ptLoc;			// ���������ɵ�
	POINT	m_ptBeginLoc;		// ��ʼλ��
	int		m_nAlpha;			// ͸��ֵ
	int		m_nState;			// ���ӵ�ǰ�����׶�
	int		m_nType;			// ��ǰ���������͵�ѩ��
	BOOL	m_bFirst;			// �Ƿ���ʾ��
	POINT	m_ptOverLoc;		// ���������ɵ�
	int		m_nOverTime;		// ��������
};
//------------------------------------------------------------------------