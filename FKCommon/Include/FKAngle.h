/********************************************************************
*
*	���ļ���FKMMORPG��һ���֡����ļ���Ȩ����FreeKnightDuzhi(������)���С�
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	�ļ�����	FKAngle
*	���ߣ�		FreeKnightDuzhi[ ������ ]
*	�ļ�˵����	
*
*	�޸���:		
*	�޸�����:
*********************************************************************/

#pragma once

//-------------------------------------------------------------------
#include "FKAngle.h"
#include "FKDegree.h"
//-------------------------------------------------------------------
namespace FKCommon
{
	class CAngel
	{
	private:
		float			m_fAngle;
	public:
		explicit CAngel( float p_fAngle )
			: m_fAngle( p_fAngle )
		{

		}
		inline operator CRadian() const
		{
			return CRadian( CMath::Angle2Radian( m_fAngle ) );
		}
		inline operator CDegree() const
		{
			return CDegree( CMath::Angle2Degree( m_fAngle ) );
		}
	};
}