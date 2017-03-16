/********************************************************************
*
*	���ļ���FKMMORPG��һ���֡����ļ���Ȩ����FreeKnightDuzhi(������)���С�
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	�ļ�����	FKRadian
*	���ߣ�		FreeKnightDuzhi[ ������ ]
*	�ļ�˵����	
*
*	�޸���:		
*	�޸�����:
*********************************************************************/

#pragma once

//-------------------------------------------------------------------
#include "FKDegree.h"
//-------------------------------------------------------------------
namespace FKCommon
{
	//-------------------------------------------------------------------
	CRadian::CRadian( const CDegree& p_Degree )
	{
		m_fRad = p_Degree.GetRadian();
	}
	//-------------------------------------------------------------------
	CRadian&	CRadian::operator = ( const CDegree& p_Degree )
	{
		m_fRad = p_Degree.GetRadian();
		return *this;
	}
	//-------------------------------------------------------------------
	CRadian		CRadian::operator + ( const CDegree& p_Degree ) const
	{
		return CRadian( m_fRad + p_Degree.GetRadian() );
	}
	//-------------------------------------------------------------------
	CRadian&	CRadian::operator -= ( const CDegree& p_Degree )
	{
		m_fRad -= p_Degree.GetRadian();
		return *this;
	}
	//-------------------------------------------------------------------
	CRadian&	CRadian::operator += ( const CDegree& p_Degree )
	{
		m_fRad += p_Degree.GetRadian();
		return *this;
	}
	//-------------------------------------------------------------------
	CRadian		CRadian::operator - ( const CDegree& p_Degree ) const
	{
		return CRadian( m_fRad - p_Degree.GetRadian() );
	}
	//-------------------------------------------------------------------
}
