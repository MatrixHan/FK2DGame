/**
*	created:		2012-7-2   1:03
*	filename: 		Particle
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <list>
#include <vector>
#include <string>
#include "Ani.h"
#include "ResLoader.h"
using std::list;
using std::vector;
using std::string;
//------------------------------------------------------------------------
#define MAX_PARTICLE_COUNT 500
//------------------------------------------------------------------------
struct IRleBitmap;
struct IBitmap;
struct ICanvas;
//------------------------------------------------------------------------
struct PARTICLE
{
	float x;				// ��ʼλ��
	float y;				// ��ʼλ��
	float vx;				// ˮƽ�ٶ�
	float vy;				// ��ֱ�ٶ�
	POINT ptTile;			// ��������
	unsigned char life;		// ����
	unsigned char Curlife;	
	float alpha;			// ͸����
	float valpha;			// ͸����˥��
};
//------------------------------------------------------------------------
class CParticleMask
{
public:
	CParticleMask();
	//��ͼƬ�����������Ľ�������������ɵĻ�׼��dx,dy�ֱ��Ӧ�������ܶ�
	//��ˮƽ�ʹ�ֱ����ÿ�����ٸ����ؽ���һ�β�����
	BOOL Create(int dx,int dy,IRleBitmap * pBitmap,BYTE *pPal);

	int Size()const
	{
		return m_MaskPoints.size();
	}

	POINTS GetPoint(int index)const
	{
		return m_MaskPoints[index];
	}

	inline bool isGray565(short r,short g,short b)
	{
		return (abs((r<<1)-(g>>1)-b)+abs(g-r-b)+abs((b<<1)-(g>>1)-r))<6;
	}

private:
	std::vector<POINTS> m_MaskPoints;
};
//------------------------------------------------------------------------
class CParticle
{
public:
	CParticle();
	CParticle(const CParticle & particle);
	virtual ~CParticle();

	void CloseRes();
	CAni *RequestRes();
	void AddNewParticle(CParticleMask * pMask,POINT ptOffset);
	BOOL GenNext();
	void Draw(ICanvas * pCanvas,int x,int y,int alpha);
	void GetSimpling(POINTS & ptSimpling);
	static CParticle* Create(int nParticle);
	BOOL Release();

private:
	float	m_fNowCreateNum;

public:
	DWORD	m_dwResID;			// ��ԴID����������Դ����������ͼ����Դ
	CAni*	m_pPoolNodeObj;		// ͼƬ�ڴ棬����ͼƬ�Ƴ��ɼ�����ʱ��������������

	char m_szName[250];
	int m_nPartAlpha;
	int m_nLife;
	int m_nPerPtcNum;

	int m_nXSpeedMin;
	int m_nXSpeedMax;
	int m_nYSpeedMin;
	int m_nYSpeedMax;
	int m_nXAddSpeed;
	int m_nYAddSpeed;
	int m_nXCaiYang;
	int m_nYCaiYang;
	int m_nXDrawOffset;
	int m_nYDrawOffset;
	int m_nAlphaBegin;
	int m_nAlphaEnd;

	int m_nCurParticleNum;
	PARTICLE m_particles[MAX_PARTICLE_COUNT];
	friend class CParticleSetting;
};
//------------------------------------------------------------------------
class CMultiParticleMask
{
public:
	CMultiParticleMask();
	virtual ~CMultiParticleMask();
	BOOL Create(CParticle * pParticle,IRleBitmap ** ppBitmap,int cBitmap,BYTE * pPal);

	CParticleMask * GetMask(int index);
private:
	CParticleMask * m_pMask;
	int m_nCount;
};
//------------------------------------------------------------------------