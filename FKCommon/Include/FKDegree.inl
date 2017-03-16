/********************************************************************
*
*	���ļ���FKMMORPG��һ���֡����ļ���Ȩ����FreeKnightDuzhi(������)���С�
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	�ļ�����	FKDegree
*	���ߣ�		FreeKnightDuzhi[ ������ ]
*	�ļ�˵����	
*
*	�޸���:		
*	�޸�����:
*********************************************************************/

#pragma once

//-------------------------------------------------------------------
#include "FKRadian.h"
//-------------------------------------------------------------------
namespace FKCommon
{
	//-------------------------------------------------------------------
	CDegree::CDegree( const CRadian& p_Red )
	{
		m_fDeg = p_Red.GetDegree();
	}
	//-------------------------------------------------------------------
	CDegree& CDegree::operator = ( const CRadian& p_Rad )
	{
		m_fDeg = p_Rad.GetDegree();
		return *this;
	}
	//-------------------------------------------------------------------
	CDegree CDegree::operator + ( const CRadian& p_Rad ) const
	{
		return CDegree( m_fDeg + p_Rad.GetDegree() );
	}
	//-------------------------------------------------------------------
	CDegree& CDegree::operator += ( const CRadian& p_Rad ) 
	{
		m_fDeg += p_Rad.GetDegree();
		return *this;
	}
	//-------------------------------------------------------------------
	CDegree CDegree::operator - ( const CRadian& p_Rad ) const
	{
		return CDegree( m_fDeg - p_Rad.GetDegree() );
	}
	//-------------------------------------------------------------------
	CDegree& CDegree::operator -= ( const CRadian& p_Rad )
	{
		m_fDeg -= p_Rad.GetDegree();
		return *this;
	}
	//-------------------------------------------------------------------
	float CDegree::GetRadian() const
	{
		return CMath::Degree2Radian( m_fDeg );
	}
	//-------------------------------------------------------------------
	float CDegree::GetAngle() const
	{
		return CMath::Degree2Angle( m_fDeg );
	}
	//-------------------------------------------------------------------
	CDegree operator * ( float p_fA, const CDegree& p_Deg )
	{
		return CDegree( p_fA * p_Deg.GetDegree() );
	}
	//-------------------------------------------------------------------
	CDegree operator / ( float p_fA, const CDegree& p_Deg )
	{
		return CDegree( p_fA / p_Deg.GetDegree() );
	}
	//-------------------------------------------------------------------
}