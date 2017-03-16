/**
*	created:		2012-6-24   22:45
*	filename: 		CreatureSignal
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ʵ��������Ϣ�Ķ���
*/
//------------------------------------------------------------------------
#pragma once
///------------------------------------------------------------------------
#include <list>
using std::list;
//------------------------------------------------------------------------
#ifndef _BASIC_CONNECTION__
#define _BASIC_CONNECTION__
	//------------------------------------------------------------------------
	template< class ARG1 , class ARG2 , class ARG3 >
	struct __BasicConnection3
	{
		virtual void Emit( ARG1 , ARG2 , ARG3 )
		{
		}
	};

	//------------------------------------------------------------------------
	template< class TYPE , class ARG1 , class ARG2 , class ARG3 >
	struct __SignalConnection3 : public __BasicConnection3< ARG1 , ARG2 , ARG3 >
	{
	public:
		typedef void (TYPE::*CONNECTFUN)( ARG1 , ARG2 , ARG3 );

		void Emit( ARG1 a , ARG2 b , ARG3 c )
		{
			( m_pConnection->*m_ConnectFun )( a , b , c );
		}

	public:
		TYPE * m_pConnection;
		CONNECTFUN m_ConnectFun;
	};

	//------------------------------------------------------------------------
	template< class ARG1 , class ARG2 , class ARG3 >
	class __SignalSender3
	{
	public:
		void AddConnection( __BasicConnection3< ARG1 , ARG2 , ARG3 > * pConnect )
		{
			m_ConnectList.push_back( pConnect );
		}

		void DelConnection( __BasicConnection3< ARG1 , ARG2 , ARG3 > * pConnect )
		{
			m_ConnectList.remove( pConnect );
		}

		void Emit( ARG1 a, ARG2 b, ARG3 c )
		{
			list< __BasicConnection3< ARG1 , ARG2 , ARG3 > * >::iterator it ;
			for( it = m_ConnectList.begin() ; it != m_ConnectList.end() ; /*it ++*/ )
			{
				(*( it ++ ))->Emit( a , b , c );
			}
		}
	private:
		typedef  list< __BasicConnection3< ARG1 , ARG2 , ARG3 > * > ConnectionList;
		ConnectionList m_ConnectList;
	};
	//------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------
// ������Ϣ���ļ���Ϣ���͵���
template< int MAX_MESSAGE = 16 >
class CMessageSender
{
public:
	// ������Ϣ  ��Ϣ��ΪdwMsg , pConnectΪ���ܶ����Ӷ���ָ��
	BOOL Subscibe( DWORD dwMsg , __BasicConnection3< LPCSTR , int , BOOL & > * pConnect )
	{
		if( dwMsg >= 0 && dwMsg < MAX_MESSAGE )
		{
			m_Sender[ dwMsg ].AddConnection( pConnect );
			return TRUE;
		}

		return FALSE;
	}

	// �˶���Ϣ  ��Ϣ��ΪdwMsg , pConnectΪ���ܶ����Ӷ���ָ��
	BOOL UnSubscibe( DWORD dwMsg , __BasicConnection3< LPCSTR , int , BOOL & > * pConnect )
	{
		if( dwMsg >= 0 && dwMsg < MAX_MESSAGE )
		{
			m_Sender[ dwMsg ].DelConnection( pConnect );
			return TRUE;
		}

		return FALSE;		
	}

	// ������Ϣ��ΪdwMsg��������Ϣ�������û�
	void Emit( DWORD dwMsg , LPCSTR buf , int nLen , BOOL & bReturn )
	{
		ASSERT( dwMsg >= 0 && dwMsg < MAX_MESSAGE );
		m_Sender[ dwMsg ].Emit( buf , nLen , bReturn );
	}

protected:
	__SignalSender3< LPCSTR , int , BOOL & >  m_Sender[ MAX_MESSAGE ];
};

//------------------------------------------------------------------------
// ���ܶ����Ӷ���
// TYPE	:	�����������
template< class TYPE >
class CMessageConnection : public __SignalConnection3< TYPE , LPCSTR , int , BOOL & >
{
public:
	CMessageConnection( TYPE * p , void ( TYPE::*member_fun)(LPCSTR , int , BOOL & ) )
	{
		m_pConnection = p;
		m_ConnectFun = member_fun;
	}

	CMessageConnection()
	{
		m_pConnection = 0;
		m_ConnectFun = 0;
	}

	// ���ûص���ʽ pΪ���Ӷ�thisָ�� , member_funΪ������Ϣ�ĳ�Ա����
	// member_fun�Ķ���Ϊ void member_fun(LPCSTR buf, int nLen, BOOL & bReturn )
	// ��: 	m_Connect.SetCallBack( this , &CActor::Accept );
	void SetCallBack( TYPE * p , void ( TYPE::*member_fun)(LPCSTR , int , BOOL & ) )
	{
		m_pConnection = p;
		m_ConnectFun = member_fun;
	}
};
//------------------------------------------------------------------------