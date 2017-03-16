/**
*	created:		2012-7-2   19:43
*	filename: 		FastIDGenerator
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��һ�ֱȽ�Ѹ�ٵĻ��Ʋ���һ�������ID
					��һ������ָ��ΨһID��ʱ�򾭳�Ҫд���ƴ���
					for( int i = 0; i < XXXMax ; i ++ )
						if( i �ǿ���� ) ����i

					����ÿ�δ�0��ʼ����Ч�ʽϵ�Ҳ���鷳��
					�����ṩ���ֽ������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <list>
using std::list;
//------------------------------------------------------------------------
//	1.��һ�����м�¼���п���ID
//	  ������ID�ķ�Χ�ܴ�ʱ������(���������UID�ɴ�0x00-0xffffffff)
template< class __ID_TYPE__ >
class CIDgeneratorPool
{
public:
	// ------------------------------------------------------------------------
	// idMin ~ idMax : �Ϸ�ID��Χ
	// ------------------------------------------------------------------------
	CIDgeneratorPool( __ID_TYPE__ idMin , __ID_TYPE__ idMax )
	{
		for( __ID_TYPE__ i = idMin ; i < idMax ; ++ i )
		{
			m_idList.push_back( i );
		}
	}

	// ------------------------------------------------------------------------
	// ����һ������ID
	// ------------------------------------------------------------------------
	BOOL Allot( __ID_TYPE__ & id )
	{
		if( m_idList.empty() )
			return FALSE;

		id = * (m_idList.begin());
		m_idList.pop_front();
		return TRUE;
	}

	// ------------------------------------------------------------------------
	// �黹һ������ID
	// ------------------------------------------------------------------------
	void Restore( __ID_TYPE__ & id )
	{
		m_idList.push_back( id );
	}

	// ------------------------------------------------------------------------
	// ֱ�Ӱ�ռһ��ָ����ID�����÷���
	// ------------------------------------------------------------------------
	BOOL Forcibl(__ID_TYPE__ id)
	{
		if(m_idList.empty() == TRUE)
		{
			return FALSE;
		}

		list< __ID_TYPE__ >::iterator it = m_idList.begin();
		for( ; it != m_idList.end(); ++it)
		{
			__ID_TYPE__ itid = *it;
			if(itid == id)
			{
				m_idList.erase(it);
				return TRUE;
			}
		}

		return FALSE;
	}	

protected:
	CIDgeneratorPool()
	{
	}

private:
	list< __ID_TYPE__ > m_idList;
};
//------------------------------------------------------------------------
//	2.��һ��ֵ��¼��������һ��ID,ÿ�η����ʱ��Ĭ��
//	  ������һ��ID.
template< class TYPE >
struct is_id_canuse
{
	bool operator()( const TYPE& d1 ) 
	{
		return false;
	}
};
//------------------------------------------------------------------------
template< class __ID_TYPE__ , class __CHECK_IDCAN_USE = is_id_canuse<__ID_TYPE__> >
class CIDgenerator
{
public:
	// ------------------------------------------------------------------------
	// idMin ~ idMax : �Ϸ�ID��Χ
	// __check_fun	 : �ж�ID�Ƿ���Ч�ĺ���
	// ------------------------------------------------------------------------
	CIDgenerator( __ID_TYPE__ idMin , __ID_TYPE__ idMax )
	{
		m_idLastAlloted = idMin;
		m_idMin = idMin;
		m_idMax = idMax;
	}

	// ------------------------------------------------------------------------
	// ����һ������ID
	// ------------------------------------------------------------------------
	BOOL Allot( __ID_TYPE__ & id )
	{
		for ( __ID_TYPE__ i = m_idLastAlloted ; i < m_idMax ; i ++ )
		{
			if( __CHECK_IDCAN_USE( i ) )
			{
				m_idLastAlloted = i + 1;
				id = i;
				return TRUE;
			}
		}

		for ( i = m_idMin ; i < m_idLastAlloted ; i ++ )
		{
			if( __CHECK_IDCAN_USE( i ) )
			{
				m_idLastAlloted = i + 1;
				id = i;
				return TRUE;
			}
		}

		return FALSE;
	}

	// ------------------------------------------------------------------------
	// ����һ������ID
	// ------------------------------------------------------------------------
	BOOL Allot( __ID_TYPE__ & id , __CHECK_IDCAN_USE & __check_fun )
	{
		for ( __ID_TYPE__ i = m_idLastAlloted ; i < m_idMax ; i ++ )
		{
			if( __check_fun( i ) )
			{
				m_idLastAlloted = i + 1;
				id = i;
				return TRUE;
			}
		}

		for ( i = m_idMin ; i < m_idLastAlloted ; i ++ )
		{
			if( __check_fun( i ) )
			{
				m_idLastAlloted = i + 1;
				id = i;
				return TRUE;
			}
		}

		return FALSE;
	}

protected:
	CIDgenerator()
	{
	}

private:
	__ID_TYPE__		m_idLastAlloted;
	__ID_TYPE__		m_idMin;
	__ID_TYPE__		m_idMax;
};
//------------------------------------------------------------------------