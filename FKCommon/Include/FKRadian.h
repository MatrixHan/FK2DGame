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
class CDegree;
//-------------------------------------------------------------------
namespace FKCommon
{
	/** ����
	*/
	class CRadian
	{
	private:
		float				m_fRad;
	public:
		explicit CRadian( float p_fRad = 0.0f )
			: m_fRad( p_fRad )
		{

		}
		CRadian& operator= ( const float& p_fRad )
		{
			m_fRad = p_fRad;
			return *this;
		}
		CRadian& operator= ( const CRadian& p_fOther )
		{
			m_fRad = p_fOther.GetRadian();
			return *this;
		}
		__forceinline float	GetRadian() const
		{
			return m_fRad;
		}
		const CRadian& operator + () const
		{
			return *this;
		}
		CRadian operator + ( const CRadian& p_Radian ) const
		{
			return CRadian( m_fRad + p_Radian.GetRadian() );
		}
		CRadian& operator += ( const CRadian& p_Radian )
		{
			m_fRad += p_Radian.GetRadian();
			return *this;
		}
		CRadian operator - () const
		{
			return CRadian( -m_fRad );
		}
		CRadian operator - ( const CRadian& p_Radian ) const
		{
			return CRadian( m_fRad - p_Radian.GetRadian() );
		}
		CRadian& operator -= ( const CRadian& p_Radian )
		{
			m_fRad -= p_Radian.GetRadian();
			return *this;
		}
		CRadian operator * ( float p_fValue ) const
		{
			return CRadian( m_fRad * p_fValue ); 
		}
		CRadian operator * ( const CRadian& p_Radian ) const
		{
			return CRadian( m_fRad * p_Radian.GetRadian() );
		}
		CRadian	operator *= ( float p_fValue )
		{
			m_fRad *= p_fValue;
			return *this;
		}
		CRadian operator / ( float p_fValue ) const
		{
			return CRadian( m_fRad / p_fValue );
		}
		CRadian& operator /= ( float p_fValue )
		{
			m_fRad /= p_fValue;
			return *this;
		}
	public:
		float		GetDegree() const;
		float		GetAngle() const;

		CRadian( const CDegree& p_Degree );
		CRadian&	operator = ( const CDegree& p_Degree );
		CRadian		operator + ( const CDegree& p_Degree ) const;
		CRadian&	operator -= ( const CDegree& p_Degree );
		CRadian&	operator += ( const CDegree& p_Degree );
		CRadian		operator - ( const CDegree& p_Degree ) const;
	public:
		bool		operator <	( const CRadian& p_Radian ) const { return (m_fRad < p_Radian.GetRadian()); }
		bool		operator <=	( const CRadian& p_Radian ) const { return (m_fRad <= p_Radian.GetRadian()); }
		bool		operator ==	( const CRadian& p_Radian ) const { return (m_fRad == p_Radian.GetRadian()); }
		bool		operator !=	( const CRadian& p_Radian ) const { return (m_fRad != p_Radian.GetRadian()); }
		bool		operator >=	( const CRadian& p_Radian ) const { return (m_fRad >= p_Radian.GetRadian()); }
		bool		operator >	( const CRadian& p_Radian ) const { return (m_fRad > p_Radian.GetRadian()); }
	};

	CRadian		operator * ( float p_fA, const CRadian& p_Rad );
	CRadian		operator / ( float p_fB, const CRadian& p_Rad );
}
//-------------------------------------------------------------------
#include "FKRadian.inl"
//-------------------------------------------------------------------