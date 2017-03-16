/**
*	created:		2012-7-2   21:48
*	filename: 		TMemory
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��Ƶ��new , delete���ڴ�Ӧ��stl���ڴ��Ż�����
					˵��: ʵ����С����Ҳ�ǿ����� __NEW_POOL_ALLOC ��
						  ����__NEW��Ψһ����� ����Ϊÿ����ά��һ��
						  ����صģ�����Class A �� Class B���� 5 bytes
						  ��ôʹ��__NEW��STL���ƵĻ� A��B��ͨ��һ������
						  �صģ�B�ͷŵ��ڴ���Լ�����A��,��__NEW_POOL_
						  ALLOC����ͨ��

						  __NEW_POOL_ALLOC �����̰߳�ȫ��
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <list>
using std::list;
//------------------------------------------------------------------------
// ���ڴ�����࣬��ʵֻ�Ǹ�����ڴ���ͷ�
class CChunkManager
{
public:
	void AddChunk( void * pChunk )
	{
		m_ChunkList.push_back( pChunk );
	}

	~CChunkManager()
	{
		list< void * >::iterator it = m_ChunkList.begin();
		for( ; it != m_ChunkList.end() ; it ++ )
		{
			delete ( * it );
		}

		m_ChunkList.clear();
	}
private:
	list< void * >	m_ChunkList;
};
//------------------------------------------------------------------------
extern CChunkManager g_ChunkManager;
//------------------------------------------------------------------------
// ����Ǵ���128�ӽڶ���Ļ���汾
template< class __T , int PoolSize = 32 >
class CMemPoolObject
{
public:	
	void * operator new(size_t size)
	{
		__poolNode * & pPoolList =  GetPoolFirst();
		if( pPoolList ) 
		{
			__poolNode * pTemp = pPoolList;
			pPoolList = pPoolList->pNext;
			return pTemp;
		}else
		{
			union __poolNodeAux
			{
				char data[ sizeof( __T ) ];
				__poolNodeAux * pNext;
			};

			__poolNodeAux * pChunk =  ( __poolNodeAux * )
				malloc( sizeof( __poolNodeAux ) * PoolSize );
			g_ChunkManager.AddChunk( pChunk );

			if( pChunk == NULL ) return NULL;

			for( int i = 1 ; i < PoolSize - 1 ; i ++ )
			{
				pChunk[ i ].pNext = &pChunk[ i + 1 ];
			}

			pChunk[ PoolSize - 1 ].pNext = NULL;
			pPoolList = ( __poolNode *)(&pChunk[ 1 ]);
			return &pChunk[0];
		}		
	}

	void operator delete(void *p)
	{
		__poolNode * & pPoolList =  GetPoolFirst();
		__poolNode * pTemp = pPoolList;
		pPoolList = (__poolNode *)p;
		pPoolList->pNext = pTemp;
	}

private:
	union __poolNode{
		__poolNode * pNext;
	};

	static __poolNode * & GetPoolFirst()
	{
		static __poolNode * s_PoolList = NULL;
		return s_PoolList;
	}
};
//------------------------------------------------------------------------