/**
*	created:		2012-7-2   19:38
*	filename: 		ArrayAlloter
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���������
					����ȡ��һ�������п��õ�Ԫ�ط����ͳ���ѷ����Ԫ�ظ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <list>
using std::list;
//------------------------------------------------------------------------
template< int nArraySize , class Element >
class CArrayAlloter
{
public:
	// ------------------------------------------------------------------------
	Element & operator[]( int nIndex )
	{
		ASSERT( nIndex >= 0 && nIndex < nArraySize );
		return m_element[nIndex];
	}

	// ------------------------------------------------------------------------
	// ����һ�����õ�Ԫ��
	// ------------------------------------------------------------------------
	int  Allot()
	{
		if( m_UsableList.empty() )
			return -1;

		int nIndex = m_UsableList.front();
		m_UsableList.pop_front();

		m_nAllotedCount ++;
		return nIndex;
	}

	// ------------------------------------------------------------------------
	// �黹һ��Ԫ��
	// ------------------------------------------------------------------------
	void Restore( int nIndex )
	{
		m_UsableList.push_back( nIndex );
		m_nAllotedCount --;
	}

	// ------------------------------------------------------------------------
	// �����ѷ���ĸ���
	// ------------------------------------------------------------------------
	int  CountAlloted()
	{
		return m_nAllotedCount;
	}

	// ------------------------------------------------------------------------
	CArrayAlloter() : m_nAllotedCount( 0 )
	{
		for( int i = 0 ; i < nArraySize ; i ++ )
		{
			m_UsableList.push_back(i);
		}
	}

private:
	Element		m_element[ nArraySize ];
	list< int > m_UsableList;
	int			m_nAllotedCount;
};
//------------------------------------------------------------------------
// �������ģ�治��Ҫ��ʾ�ĵ���Restore
// ��Ҫ�ṩһ���º����ж�ĳ��Ԫ���Ƿ����
template< class TYPE >
struct is_element_canuse
{
	bool operator()( const TYPE& d1 ) const
	{
		return false;
	}
};
//------------------------------------------------------------------------
template< int nArraySize , class Element , class __CHECK_CAN_USE2 >
class CArrayAlloter2
{
public:
	// ------------------------------------------------------------------------
	Element & operator[]( int nIndex )
	{
		ASSERT( nIndex >= 0 && nIndex<nArraySize );
		return m_element[nIndex];
	}

	// ------------------------------------------------------------------------
	// ����һ�����õ�Ԫ��
	// ------------------------------------------------------------------------
	int  Allot()
	{
		for ( int i = m_nLastAllotIndex ; i < nArraySize ; i ++ )
		{
			if( __CHECK_CAN_USE2()( m_element[i] ) )
			{
				m_nLastAllotIndex = i + 1;
				return i;
			}
		}

		for ( i = 0 ; i < m_nLastAllotIndex ; i ++ )
		{
			if( __CHECK_CAN_USE2()( m_element[i] ) )
			{
				m_nLastAllotIndex = i + 1;
				return i;
			}
		}

		return -1;
	}

	// ------------------------------------------------------------------------
	CArrayAlloter2() : m_nLastAllotIndex( 0 )
	{
	}

private:
	Element		m_element[ nArraySize ];
	int			m_nLastAllotIndex;
};
//------------------------------------------------------------------------