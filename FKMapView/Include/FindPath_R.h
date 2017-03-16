/**
*	created:		2012-7-1   2:40
*	filename: 		FindPath_R
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		˫������Ѱ·
*/
//------------------------------------------------------------------------
//	�㷨����
//
//	1. ·������ʱ�����Ƿ����ײֱ����Ŀ���ƽ� ( �ҳ����� )
//  3. �������н���������������������ת��
//	4. ��������ǰ�������������������������ǰ��
//	5. ���ջᵽ��Ŀ���	, ��������
//
//   ���ܣ��������㷨Ѱ��·��
// 
//   Լ����TileContainer�ǰ�����ͼ��Ԫ�������,��ʵ��
//		   IsBlock( POINTS ) ����
//
//   ˵������ǰ�汾�� 8 ������Ѱ· б���ж�����סʱ������
//		   ��Ҫ���岻ͬ�Ĺ�����Ҫ���� FindPath
//
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "MapViewCommon.h"
//------------------------------------------------------------------------
struct   RIVERTAG;
//------------------------------------------------------------------------
// �������¶���MAX_SEARCH_AREA�Ը��ĳ�����Ҫ�����������Χ
#ifndef  MAX_SEARCH_AREA
#define  MAX_SEARCH_AREA	128
#endif
//------------------------------------------------------------------------
template < class TileContainer , int MaxPathLen = 1024 >
class CPathFinder_R{
private:
	TileContainer	*  m_pTiles;			// �ж�ĳ���Ƿ��赲�ĺ���ָ��
	RIVERTAG  *  m_pNearest;				// ��ӽ��ĵ�
	int	         m_nWidth;					// ��ͼ���
	int		     m_nHeight;					// ��ͼ�߶�

	POINTS		 m_ptPath[ MaxPathLen ];	// ·��������
	int			 m_nPathLen;				// ·������

	DWORD		 m_cFindID;					//������ʾ���ǵڼ���Ѱ·

	// ��ʱ����
	RIVERTAG     m_TempTags[ MAX_SEARCH_AREA ][ MAX_SEARCH_AREA ];
	POINTS       m_ptStart;					// ����Ѱ·����ʼ��
public:
	int          m_nSearchNodes;			// ���һ��Ѱ·�����ĵ���
	DWORD        m_dwElapseTime;			// ���һ��Ѱ·���ĵ�ʱ��

public:
	CPathFinder_R();
	virtual ~CPathFinder_R();

	// ��ʼ����ͼ���������������׳����� ����LPCTSTR
	//
	// pMap		:	��ͼͼ��ָ��
	// nWidth   :   ��ͼ���    
	// nHeight  :   ��ͼ�߶�
	BOOL Create( TileContainer * pContainer , int nWidth , int nHeight  );

	// Ѱ· , �����������׳����� ����LPCTSTR
	// ptFrom	:   ·������ʼ��
	// ptTo		:   ·����Ŀ���
	// ...		
	BOOL FindPath( POINTS ptFrom , POINTS ptTo , POINTS ** ppBuffer , int & nPathLen );

	// Ѱ��һ����·��
	// ��ʱ����·����Ҫ�ܾ�ȷֻ��Ҫ����һ��ģ��·���Ϳ�
	BOOL FindSimplePath( POINTS ptFrom , POINTS ptTo , POINTS ** ppBuffer , int & nPathLen );

	// ���� FindPath ��ȡ��·��
	// pBuffer	:   ȡ����·���ϸ���Ļ�����
	// nMaxStep	:	�������Ĵ�С , ��ֹԽ��
	// ����  	:   -1	------------ ��������
	//				0 - nMaxStep ·����ĸ���
	//				> nMaxStep	 ������̫С
	BOOL GetPath( POINTS ** ppBuffer , int & nPathLen );

	// ����ڴ�, ������´�Ҫ�������µ��� InitializeData
	void Close();

private:
	//   �ж�һЩ�����ߵ����� , ����б�����ж�����סʱ������
	BOOL CheckInValidMoving( POINTS ptFrom , POINTS ptTo );

	//	 �ж�һ�����Ƿ��ڵ�ͼ������
	bool PointInMap( POINTS pos );

	// ����ָ��ȡ������λ��
	POINTS  AddressToPos( RIVERTAG * pAddr );

	BOOL   FindPathAux( POINTS ptFrom , POINTS ptTo );
	BOOL   GetPathAux( POINTS * pBuffer , int & nMaxStep );

	// ��һ��ָ���㿪ʼ�ƹ��ϰ�
	// pos		:  ������ʼ��
	// ptTo		:  ��·�յ�
	// nRunDir	:  �������з���
	// nEchoDir	:  ���Ƶķ���	1	-----------  ��ʱ��
	//							-1	-----------  ˳ʱ��
	//							����ֵ������
	bool Echo2( POINTS & pos , POINTS ptTo , int nRunDir , int nEchoDir );
	// ��������ͬʱ�����İ汾
	bool Echo( POINTS & pos , POINTS ptTo , int nRunDir , int nEchoDir , int nRunDir2 , int nEchoDir2 );

	// ȡ����ʱ��Ϣ
	RIVERTAG * GetInfo( POINTS pos );

	// ·���Ż�
	void PathOptimize();

	// ��ͨ������
	BOOL CennectPoint( POINTS pos1 , POINTS pos2 );

	// �����������Ƿ�����ͨ
	BOOL TestConnectPoint( POINTS pos1 , POINTS pos2 );

	// �ж�һ�����Ƿ����赲��
	BOOL IsBlock( POINTS pos );

	// ����һ�������ڶ��������˷����������һ��
	int  GetNearestDir( int nDeltaX , int nDeltaY );
};
//------------------------------------------------------------------------
#define		BLUE_FLAG	  ( 0x01 << 1 )		// ���߱�־λ
#define     VISIT_FLAG	  ( 0x01 << 2 )		// ���ʱ�־λ
#define     OPEN_FLAG     ( 0x01 << 3 )     // �Ѽ���OPEN
//------------------------------------------------------------------------
#define     DIR_HIT( d )      ( (char)(0x01 << (4+d)) )
#define     DIR_CW( d )		  ( (char)(0x01 << (d)) )		//	˳ʱ��Ǻ�
#define     DIR_CCW( d )	  ( (char)(0x01 << (4+d)) )		//  ��ʱ��Ǻ�
//------------------------------------------------------------------------
struct RIVERTAG				// ������¼�����ߺ�����һЩ��ʱ��Ϣ
{
	RIVERTAG  * pNext;		// ������ĵ�����
	RIVERTAG  * pParent;	// ����ָ����������·��ʱ��˷
	DWORD       cFindID;	// �ڼ���Ѱ·
	char		cFlag;		// ��Ϣ��־�ֽ�	����λ��ʾ��ײʱ����Ĳ��Է����cDir��һ��
	char        cDir;		// ����ʱ�ķ���
	char        cEchoDir;	// ����ʱ��ת�ķ���
	char        cReserse;	// ����ֵ
};
//------------------------------------------------------------------------
template < class TileContainer , int MaxPathLen>
CPathFinder_R< TileContainer , MaxPathLen >::CPathFinder_R()
{
	m_pTiles	=	NULL;	
	m_pNearest  =   NULL;
	m_nWidth	=	0;	
	m_nHeight	=	0;
	m_cFindID   =   0;
}
//------------------------------------------------------------------------
template < class TileContainer , int MaxPathLen>
CPathFinder_R< TileContainer , MaxPathLen >::~CPathFinder_R()
{
	Close();
}
//------------------------------------------------------------------------
template < class TileContainer , int MaxPathLen>
void CPathFinder_R< TileContainer , MaxPathLen >::Close()
{
	m_pTiles	=	NULL;	
	m_nWidth	=	0;	
	m_nHeight	=	0;	
	m_pNearest  =   NULL;
}
//------------------------------------------------------------------------
template < class TileContainer , int MaxPathLen>
BOOL CPathFinder_R< TileContainer , MaxPathLen >::Create( TileContainer * pContainer , int nWidth , int nHeight  )
{
	if( pContainer == NULL ) {
		throw( "Parameter error! CPathFinder_R< TileContainer , MaxPathLen >::InitializeData" );
		return FALSE;
	}
	if( nWidth <= 0 || nHeight <= 0 )
	{
		throw( "Parameter error! CPathFinder_R< TileContainer , MaxPathLen >::InitializeData\r\n" );
		return FALSE;
	}

	Close();

	m_nWidth	= nWidth;
	m_nHeight	= nHeight;

	m_pTiles   = pContainer;

	return TRUE;		// return OK!
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >	
BOOL CPathFinder_R< TileContainer , MaxPathLen >::FindPathAux( POINTS ptFrom , POINTS ptTo )
{
	if( !PointInMap( ptFrom ) || !PointInMap( ptTo ) )
	{
		return FALSE;
	}

	//�����һ��Ѱ·�ı���
	m_pNearest = NULL;
	m_nSearchNodes =0;

	m_ptStart = ptFrom;

	// ͬһ����
	if( ptFrom.x == ptTo.x && ptTo.y == ptFrom.y ) return FALSE;

	POINTS       ptRun	   = ptFrom;

	RIVERTAG  * pOldNode   = NULL;
	RIVERTAG  * pNode	   = NULL;

	// ���ܵ��ĵ��ֱ���߹�ȥ�������ϰ���ͣ
	if( IsBlock( ptTo ) )
	{
		while( 1 )
		{
			m_nSearchNodes ++;

#ifdef	_DEBUG
			ASSERT( PointInMap( ptRun ) );
#endif
			pNode = GetInfo( ptRun );

			if( pNode == NULL ) return FALSE;

			pNode->pParent = pOldNode;

			pOldNode = pNode;


			if( IsBlock( ptRun ) && m_pNearest != NULL ) return TRUE;

			if( ptTo.y == ptRun.y && ptTo.x == ptRun.x ) return TRUE;

			m_pNearest = pNode;

			if( ptTo.y > ptRun.y ) ptRun.y += 1;
			if( ptTo.y < ptRun.y ) ptRun.y -= 1;
			if( ptTo.x > ptRun.x ) ptRun.x += 1;
			if( ptTo.x < ptRun.x ) ptRun.x -= 1;
		}
	}

	//  ��һ���߳�����
	while( 1 )
	{
		m_nSearchNodes ++;

#ifdef	_DEBUG
		ASSERT( PointInMap( ptRun ) );
#endif
		pNode = GetInfo( ptRun );

		if( pNode == NULL ) return FALSE; 

		pNode->cFlag |= BLUE_FLAG;

		pNode->pParent = pOldNode;

		pOldNode = pNode;

		if( ptTo.y == ptRun.y && ptTo.x == ptRun.x )
			break;

		if( ptTo.y > ptRun.y ) ptRun.y += 1;
		if( ptTo.y < ptRun.y ) ptRun.y -= 1;
		if( ptTo.x > ptRun.x ) ptRun.x += 1;
		if( ptTo.x < ptRun.x ) ptRun.x -= 1;
	}

	ptRun	   = ptFrom;
	POINTS oldpos = ptFrom;
	//  �ڶ����������������ϰ��ƹ�ֱ���ص�����Ϊֹ
	while( 1 )
	{

#ifdef	_DEBUG
		ASSERT( PointInMap( ptRun ) );
#endif

		pNode = GetInfo( ptRun );

		if( pNode == NULL ) return FALSE;

		pNode->cFlag |= VISIT_FLAG;

		//�����ϰ��Ͱ�����ǰһ������뵽���������
		if( IsBlock( ptRun ) )// || CheckInValidMoving( oldpos , ptRun ) )
		{
			if( ptRun.y > oldpos.y && ptRun.x == oldpos.x)
			{
				ptRun = oldpos;
				if( !Echo( ptRun , ptTo , 1 , -1 , 1 , 1 ) ) return TRUE;
			}
			if( ptRun.y < oldpos.y  && ptRun.x == oldpos.x)
			{
				ptRun = oldpos;
				if( !Echo( ptRun , ptTo , 3 , -1 , 3 , 1) ) return TRUE;
			}
			if( ptRun.x > oldpos.x && ptRun.y == oldpos.y)
			{
				ptRun = oldpos;
				if( !Echo( ptRun , ptTo , 0 , -1 , 0 , 1 ) ) return TRUE;
			}
			if( ptRun.x < oldpos.x && ptRun.y == oldpos.y)
			{
				ptRun = oldpos;
				if( !Echo( ptRun , ptTo , 2 , -1 , 2 , 1 ) ) return TRUE;
			}
			if( ptRun.x < oldpos.x && ptRun.y < oldpos.y)
			{
				ptRun = oldpos;
				if( !Echo( ptRun , ptTo , 3 , -1 , 2 , 1 ) ) return TRUE;
			}
			if( ptRun.x > oldpos.x && ptRun.y < oldpos.y)
			{
				ptRun = oldpos;
				if( !Echo( ptRun , ptTo , 0 , -1 , 3 , 1 ) ) return TRUE;
			}
			if( ptRun.x > oldpos.x && ptRun.y > oldpos.y)
			{
				ptRun = oldpos;
				if( !Echo( ptRun , ptTo , 1 , -1 , 0 , 1 ) ) return TRUE;
			}
			if( ptRun.x < oldpos.x && ptRun.y > oldpos.y)
			{
				ptRun = oldpos;
				if( !Echo( ptRun , ptTo , 2 , -1 , 1 , 1 ) ) return TRUE;
			}
		}

		pOldNode = pNode;
		oldpos = ptRun;

		m_pNearest = GetInfo( ptRun );
		if( m_pNearest == NULL ) return FALSE;
		if( ptTo.y == ptRun.y && ptTo.x == ptRun.x ) return TRUE;

		if( ptTo.y > ptRun.y ) ptRun.y += 1;
		if( ptTo.y < ptRun.y ) ptRun.y -= 1;
		if( ptTo.x > ptRun.x ) ptRun.x += 1;
		if( ptTo.x < ptRun.x ) ptRun.x -= 1;
	}

	return FALSE;
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >
BOOL  CPathFinder_R< TileContainer , MaxPathLen >::GetPathAux( POINTS * pBuffer , int & nMaxStep )
{
	if( pBuffer == NULL ) return FALSE;

	int nCou = 0;

	RIVERTAG * pNode = m_pNearest;
	while( pNode != NULL )
	{
		nCou ++;
		pNode = pNode->pParent;
		if( nCou > MaxPathLen ) 
		{
			nCou = MaxPathLen - 1;
			TraceLn(" the path is to long ! CPathFinder_R< TileContainer , MaxPathLen >::GetPathAux" );
			nMaxStep = 0;
			return FALSE;
		}
	}

	// OVER STACK
	if( nCou <= 0 || nCou > nMaxStep ) 
	{
		nMaxStep = nCou;
		return FALSE;
	}

	nMaxStep = nCou;

	int retValue = nCou;
	pNode = m_pNearest;
	while( pNode != NULL )
	{
		memcpy( &pBuffer[ --nCou ] , &(AddressToPos( pNode )) , sizeof( POINTS ) );

#ifdef _DEBUG
		ASSERT( nCou >= 0 );
		if( nCou == 0 ) break;
#endif
		pNode = pNode->pParent;
	}

	return TRUE;
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >
inline BOOL CPathFinder_R< TileContainer , MaxPathLen >::CheckInValidMoving( POINTS ptFrom , POINTS ptTo )
{

#ifdef _DEBUG
	ASSERT( PointInMap( ptFrom ) );
	ASSERT( PointInMap( ptTo )   );
#endif

	return abs( ptFrom.x - ptTo.x ) == 1 && abs( ptFrom.y - ptTo.y ) == 1 
		&& ( m_pTileContainers[ ptFrom.y*m_nWidth + ptTo.x ].IsBlock()
		|| m_pTileContainers[ ptTo.y*m_nWidth + ptFrom.x ].IsBlock() );
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >
inline bool CPathFinder_R< TileContainer , MaxPathLen >::PointInMap( POINTS pos )
{
	return pos.x >= 0 && pos.x < m_nWidth && pos.y >= 0 && pos.y < m_nHeight;
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >
BOOL CPathFinder_R< TileContainer , MaxPathLen >::
FindPath( POINTS ptFrom , POINTS ptTo , POINTS ** ppBuffer , int & nPathLen )
{
	if( m_cFindID >= 0xffffffff ) 
	{
		// ����ɼ�¼
		ZeroMemory( m_TempTags , sizeof( m_TempTags ) );
		m_cFindID = 0;
	}
	m_cFindID ++;

	if( !FindPathAux( ptFrom , ptTo ) ) return FALSE;

	m_nPathLen = MaxPathLen;
	if( !GetPathAux( m_ptPath , m_nPathLen ) ) return FALSE;
	if( m_nPathLen <= 1 ) return FALSE;

	// �����������·��̫��ͽ����Ż�
	int nLen = abs( ptFrom.x - ptTo.x ) + abs( ptFrom.y - ptTo.y );
	if( m_nPathLen > nLen  &&  m_nPathLen > 10 ) PathOptimize();

	if (m_nPathLen <= 0
		|| m_nPathLen > MaxPathLen
		)
		return FALSE;

	if( !GetPath( ppBuffer , nPathLen ) ) return FALSE;

	return TRUE;
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >
BOOL CPathFinder_R< TileContainer , MaxPathLen >::
FindSimplePath( POINTS ptFrom , POINTS ptTo , POINTS ** ppBuffer , int & nPathLen )
{
	static int s_nDirMove[][2] = { 
		{ 0  , -1 }	,	// ��
		{ 1  , -1 } ,	// ����
		{ 1	 , 0  } ,	// ��
		{ 1	 , 1  }	,	// ����
		{ 0	 , 1  }	,	// ��
		{ -1 , 1  }	,	// ����
		{ -1 , 0  } ,	// ��
		{ -1 , -1 }	,	// ����
	};	// �˸������ϵ��ƶ�


	if( m_cFindID >= 0xffffffff ) 
	{
		// ����ɼ�¼
		ZeroMemory( m_TempTags , sizeof( m_TempTags ) );
		m_cFindID = 0;
	}
	m_cFindID ++;

	if( !PointInMap( ptFrom ) || !PointInMap( ptTo ) )
	{
		return FALSE;
	}

	//�����һ��Ѱ·�ı���
	m_pNearest = NULL;
	m_nSearchNodes =0;

	m_ptStart = ptFrom;

	POINTS      ptRun	   = ptFrom;
	POINTS		oldpos	   = ptFrom;
	RIVERTAG  * pOldNode   = NULL;
	RIVERTAG  * pNode	   = NULL;

	// ͬһ����
	if( ptFrom.x == ptTo.x && ptTo.y == ptFrom.y ) return FALSE;

	int nMoveDir = GetNearestDir( ptTo.x - ptFrom.x , ptTo.y - ptFrom.y );;
	while( 1 )
	{

#ifdef	_DEBUG
		ASSERT( PointInMap( ptRun ) );
#endif

		pNode = GetInfo( ptRun );

		if( pNode == NULL ) return FALSE;

		pNode->pParent = pOldNode;

		pOldNode = pNode;
		oldpos = ptRun;

		m_pNearest = GetInfo( ptRun );
		if( m_pNearest == NULL ) return FALSE;
		if( ptTo.y == ptRun.y && ptTo.x == ptRun.x ) break;

		int dx = ptTo.x - ptRun.x;
		int dy = ptTo.y - ptRun.y;

		if( dx == 0 || dy == 0 || abs(dx) == abs(dy) )
		{
			if( nMoveDir != GetNearestDir( dx , dy ) ) break;
		}

		ptRun.y +=	s_nDirMove[ nMoveDir ][ 1 ];
		ptRun.x +=	s_nDirMove[ nMoveDir ][ 0 ];

		if( IsBlock( ptRun ) )
		{
			break;
		}
	}

	m_nPathLen = MaxPathLen;
	if( !GetPathAux( m_ptPath , m_nPathLen ) ) return FALSE;
	if( m_nPathLen <= 1 ) return FALSE;

	if( !GetPath( ppBuffer , nPathLen ) ) return FALSE;

	return TRUE;
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >
BOOL CPathFinder_R< TileContainer , MaxPathLen >::
GetPath( POINTS ** ppBuffer , int & nPathLen )
{
	if( ppBuffer == NULL ) return FALSE;

	* ppBuffer = m_ptPath;
	nPathLen = m_nPathLen;

	return TRUE;
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >
bool CPathFinder_R< TileContainer , MaxPathLen >::
Echo2( POINTS & pos , POINTS ptTo , int nRunDir , int nEchoDir )
{
	static const POINTS	ptDir[ 4 ] = { { 1 , 0 } , { 0 , 1 } , { -1 , 0} , {0 , -1} };

	RIVERTAG * pNode;
	POINTS	   ptNode = pos;
	POINTS	   temppos;
	POINTS     ptOldPos = pos;
	int        nTestDir;
	int        nCount = 0;
	bool       bHit = true;

	while( 1 )
	{
		nCount ++;
		m_nSearchNodes ++;

		pNode = GetInfo( ptNode );
		if( pNode == NULL ) return FALSE;

		if( ptNode.x != ptOldPos.x || ptNode.y != ptOldPos.y )
		{
			//�ҵ�Ŀ�������ҵ�����
			if( ( pNode->cFlag & VISIT_FLAG )== 0 )
			{

				if( pNode->pParent == NULL || pNode->cFlag & BLUE_FLAG )
					pNode->pParent = GetInfo( ptOldPos );

				if( ptTo.x == ptNode.x && ptTo.y == ptNode.y || pNode->cFlag & BLUE_FLAG )
				{
					pos = ptNode;
					pNode->cDir  |= DIR_HIT( nRunDir );
					pNode->cFlag |= VISIT_FLAG;
					return true;
				}
			}else{
				//�����һ���Ѿ�·���˲��ҷ���Ҳ��ͬ���˳�
				if( pNode->cDir & DIR_HIT( nRunDir ) ) return false;
			}
			pNode->cDir  |= DIR_HIT( nRunDir );
			pNode->cFlag |= VISIT_FLAG;
		}
		ptOldPos = ptNode;

		//���Դ�ֱ�����ܷ��ƶ�
		nTestDir = nRunDir + nEchoDir;
		if( nTestDir < 0 ) nTestDir += 4;
		if( nTestDir >= 4 ) nTestDir -= 4;

		temppos.x = ptNode.x + ptDir[ nTestDir ].x;
		temppos.y = ptNode.y + ptDir[ nTestDir ].y;

		//������ͼ�ر߽���,�˳�
		if( !PointInMap( temppos ) ) return false;

		//��ֱ��������
		if( !IsBlock( temppos ) )
		{
			//ת��
			nRunDir = nTestDir;
			ptNode = temppos;
			//�Ƶķ���Ҳת��
			if( bHit ) nEchoDir *= -1;
			bHit = false;
			continue;
		}

		bHit = false;
		//��һ�β�����ת���ɹ�ת����
		if( nCount == 1 ) 
		{
			nRunDir += 2;
			nRunDir %= 4;
			bHit = false;
			nEchoDir *= -1;
			continue;
		}

		//��������߾�ǰ��һ��
		temppos.x = ptNode.x + ptDir[ nRunDir ].x;
		temppos.y = ptNode.y + ptDir[ nRunDir ].y;

		//������ͼ�߳��߽�,�˳�
		if( !PointInMap( temppos ) ) return false;

		//������赲��ת������ǰ��һ��
		if( IsBlock( temppos ) )
		{
			nEchoDir *= -1;
			bHit = true;

			// �������������һ���ٵ�
			if( GetInfo( temppos ) == NULL ) return FALSE;
			if( GetInfo( temppos )->cFlag & DIR_HIT( nRunDir ) )
			{
				nRunDir += 2;
				nRunDir %= 4;
			}

			GetInfo( temppos )->cFlag |= DIR_HIT( nRunDir );

		}else{
			ptNode = temppos;
		}
	}
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >
bool  CPathFinder_R< TileContainer , MaxPathLen >::Echo( POINTS & pos , POINTS ptTo , int nRunDir , int nEchoDir , int nRunDir2 , int nEchoDir2 )
{
	static const POINTS	ptDir[ 4 ] = { { 1 , 0 } , { 0 , 1 } , { -1 , 0} , {0 , -1} };

	RIVERTAG * pNode = NULL;
	POINTS	   ptNode = pos;
	POINTS	   temppos;
	POINTS     ptOldPos = pos;
	int        nTestDir;
	int        nCount = 0;
	bool       bHit = true;

	RIVERTAG * pNode2 = NULL;
	POINTS	   ptNode2 = pos;
	POINTS	   temppos2;
	POINTS     ptOldPos2 = pos;
	int        nTestDir2;
	int        nCount2 = 0;
	bool       bHit2 = true;

	//����ͬʱ�н�
	bool   bRunning1 = true;
	bool   bRunning2 = true;

	void * lastpos = NULL;
	int lastcount = 0;
	void * lastpos2 = NULL;
	int lastcount2 = 0;

	while( bRunning1 || bRunning2 )
	{

		if( bRunning1 )
		{
			nCount ++;
			m_nSearchNodes ++;

			pNode = GetInfo( ptNode );
			if( pNode == NULL ) return FALSE;

			// ����Ƿ��ҵ�Ŀ���·
			if( ptNode.x != ptOldPos.x || ptNode.y != ptOldPos.y )
			{
				// ������û��·��
				if( ( pNode->cFlag & VISIT_FLAG ) == 0 )
				{

					if( pNode->pParent == NULL || pNode->cFlag & BLUE_FLAG )
						pNode->pParent = GetInfo( ptOldPos );

					//�ҵ�Ŀ�������ҵ�����	
					if( ptTo.x == ptNode.x && ptTo.y == ptNode.y || pNode->cFlag & BLUE_FLAG )
					{
						pos = ptNode;
						pNode->cDir  |= DIR_HIT( nRunDir );
						pNode->cFlag |= VISIT_FLAG;
						return true;
					}
				}
			}

			// �������ͬ�ķ�ʽ����ͬһ����,���˳�
			if( ( pNode->cFlag & VISIT_FLAG ) && ( pNode->cDir & DIR_HIT( nRunDir ) ) )
			{
				if( nEchoDir == 1 && ( pNode->cEchoDir & DIR_CW( nRunDir ) )  ) 
				{
					bRunning1 = false;
					continue;
				}
				if( nEchoDir == -1 && ( pNode->cEchoDir & DIR_CCW( nRunDir ) )  ) 
				{
					bRunning1 = false;
					continue;
				}
			}
			pNode->cDir  |= DIR_HIT( nRunDir );
			pNode->cFlag |= VISIT_FLAG;
			if( nEchoDir == 1 ) pNode->cEchoDir |= DIR_CW( nRunDir );
			if( nEchoDir == -1 ) pNode->cEchoDir |= DIR_CCW( nRunDir );

			ptOldPos = ptNode;

			//���Դ�ֱ�����ܷ��ƶ�
			nTestDir = nRunDir + nEchoDir;
			if( nTestDir < 0 ) nTestDir += 4;
			if( nTestDir >= 4 ) nTestDir -= 4;

			temppos.x = ptNode.x + ptDir[ nTestDir ].x;
			temppos.y = ptNode.y + ptDir[ nTestDir ].y;

			//������ͼ�ر߽���,�˳�
			if( !PointInMap( temppos ) ) 
			{
				bRunning1 = false;
				continue;
			}
			//��ֱ��������
			if( !IsBlock( temppos ) )
			{
				//ת��
				nRunDir = nTestDir;
				ptNode = temppos;
				//�Ƶķ���Ҳת��
				if( bHit ) nEchoDir *= -1;
				bHit = false;
				continue;
			}

			bHit = false;
			//��һ�β�����ת���ɹ�ת����
			if( nCount == 1 ) 
			{
				nRunDir += 2;
				nRunDir %= 4;
				bHit = false;
				nEchoDir *= -1;
				continue;
			}

			//��������߾�ǰ��һ��
			temppos.x = ptNode.x + ptDir[ nRunDir ].x;
			temppos.y = ptNode.y + ptDir[ nRunDir ].y;

			//������ͼ�߳��߽�,�˳�
			if( !PointInMap( temppos ) ) 
			{
				bRunning1 = false;
				continue;
			}

			//������赲��ת������ǰ��һ��
			if( IsBlock( temppos ) )
			{
				nEchoDir *= -1;
				bHit = true;

				// �������������һ���ٵ�
				if( GetInfo( temppos ) == NULL ) 
				{
					bRunning1 = false;
					continue;
				}
				if( GetInfo( temppos )->cFlag & DIR_HIT( nRunDir ) )
				{
					nRunDir += 2;
					nRunDir %= 4;
				}

				GetInfo( temppos )->cFlag |= DIR_HIT( nRunDir );

			}else{
				ptNode = temppos;
			}
		}

		//������
		if( bRunning2 )
		{
			nCount2 ++;
			m_nSearchNodes ++;

			pNode2 = GetInfo( ptNode2 );
			if( pNode2 == NULL ) return FALSE;

			if( ptNode2.x != ptOldPos2.x || ptNode2.y != ptOldPos2.y )
			{
				// ������û��·��
				if( ( pNode2->cFlag & VISIT_FLAG )== 0 )
				{

					if( pNode2->pParent == NULL || pNode2->cFlag & BLUE_FLAG )
						pNode2->pParent = GetInfo( ptOldPos2 );

					//�ҵ�Ŀ�������ҵ�����
					if( ptTo.x == ptNode2.x && ptTo.y == ptNode2.y || pNode2->cFlag & BLUE_FLAG )
					{
						pos = ptNode2;
						pNode2->cDir  |= DIR_HIT( nRunDir2 );
						pNode2->cFlag |= VISIT_FLAG;
						return true;
					}
				}else{
					//�����һ���Ѿ�·���˲��ҷ���Ҳ��ͬ���˳�
					if( pNode2->cDir & DIR_HIT( nRunDir2 ) ) 
					{
						bRunning2 = false;
						continue;
					}

				}
				pNode2->cDir  |= DIR_HIT( nRunDir2 );
				pNode2->cFlag |= VISIT_FLAG;
			}

			// �������ͬ�ķ�ʽ����ͬһ����,���˳�
			if( ( pNode2->cFlag & VISIT_FLAG ) && ( pNode2->cDir & DIR_HIT( nRunDir2 ) ) )
			{
				if( nEchoDir2 == 1 && ( pNode2->cEchoDir & DIR_CW( nRunDir2 ) )  ) 
				{
					bRunning2 = false;
					continue;
				}
				if( nEchoDir2 == -1 && ( pNode2->cEchoDir & DIR_CCW( nRunDir2 ) )  ) 
				{
					bRunning2 = false;
					continue;
				}
			}
			pNode2->cDir  |= DIR_HIT( nRunDir2 );
			pNode2->cFlag |= VISIT_FLAG;
			if( nEchoDir2 == 1 ) pNode2->cEchoDir |= DIR_CW( nRunDir2 );
			if( nEchoDir2 == -1 ) pNode2->cEchoDir |= DIR_CCW( nRunDir2 );

			ptOldPos2 = ptNode2;

			//���Դ�ֱ�����ܷ��ƶ�
			nTestDir2 = nRunDir2 + nEchoDir2;
			if( nTestDir2 < 0 ) nTestDir2 += 4;
			if( nTestDir2 >= 4 ) nTestDir2 -= 4;

			temppos2.x = ptNode2.x + ptDir[ nTestDir2 ].x;
			temppos2.y = ptNode2.y + ptDir[ nTestDir2 ].y;

			//������ͼ�ر߽���,�˳�
			if( !PointInMap( temppos2 ) )
			{
				bRunning2 = false;
				continue;
			}

			//��ֱ��������
			if( !IsBlock( temppos2 ) )
			{
				//ת��
				nRunDir2 = nTestDir2;
				ptNode2 = temppos2;
				//�Ƶķ���Ҳת��
				if( bHit2 ) nEchoDir2 *= -1;
				bHit2 = false;
				continue;
			}

			bHit2 = false;
			//��һ�β�����ת���ɹ�ת����
			if( nCount2 == 1 ) 
			{
				nRunDir2 += 2;
				nRunDir2 %= 4;
				bHit2 = false;
				nEchoDir2 *= -1;
				continue;
			}

			//��������߾�ǰ��һ��
			temppos2.x = ptNode2.x + ptDir[ nRunDir2 ].x;
			temppos2.y = ptNode2.y + ptDir[ nRunDir2 ].y;

			//������ͼ�߳��߽�,�˳�
			if( !PointInMap( temppos2 ) )
			{
				bRunning2 = false;
				continue;
			}

			//������赲��ת������ǰ��һ��
			if( IsBlock( temppos2 ) )
			{
				nEchoDir2 *= -1;
				bHit2 = true;

				// �������������һ���ٵ�
				if( GetInfo( temppos2 ) == NULL ) 
				{
					bRunning2 = false;
					continue;
				}
				if( GetInfo( temppos2 )->cFlag & DIR_HIT( nRunDir2 ) )
				{
					nRunDir2 += 2;
					nRunDir2 %= 4;
				}

				GetInfo( temppos2 )->cFlag |= DIR_HIT( nRunDir2 );

			}else{
				ptNode2 = temppos2;
			}
		}

	}

	return false;
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >
inline RIVERTAG * CPathFinder_R< TileContainer , MaxPathLen >::GetInfo( POINTS pos )
{
	// Ѱ·��ʼ����64,64��λ��
	int x = pos.x - m_ptStart.x + ( MAX_SEARCH_AREA >> 1 );
	int y = pos.y - m_ptStart.y + ( MAX_SEARCH_AREA >> 1 );
	// ����
	if( x < 0 || x >= 128 || y < 0 || y >= 128 ) 
	{
		return FALSE;
	}

	RIVERTAG * pReturn = &(m_TempTags[y][x]);
	if( pReturn->cFindID != m_cFindID )
	{
		memset( pReturn , 0 , sizeof( RIVERTAG ) );
		pReturn->cFindID = m_cFindID;
	}

	return pReturn;
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >
POINTS  CPathFinder_R< TileContainer , MaxPathLen >::AddressToPos( RIVERTAG * pAddr )
{
	int index = pAddr - &(m_TempTags[0][0]);

#ifdef _DEBUG
	ASSERT( index >= 0 );
#endif


	POINTS pos;
	pos.x = m_ptStart.x - ( MAX_SEARCH_AREA >> 1 ) + index % MAX_SEARCH_AREA;
	pos.y = m_ptStart.y - ( MAX_SEARCH_AREA >> 1 ) + index / MAX_SEARCH_AREA;

	return pos;
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >
void  CPathFinder_R< TileContainer , MaxPathLen >::PathOptimize()
{
	// �ҵ����йյ�
	int			 nCornerIndex[ MaxPathLen ];
	int			 nCornerNum;
	nCornerNum = 0;

	RIVERTAG * pInfo = GetInfo( m_ptPath[ 0 ] );
	if( pInfo == NULL ) return;
	char   nLastRunDir = pInfo->cDir;
	nCornerIndex[ nCornerNum ++ ] = 0;
	for( int i = 1 ; i < m_nPathLen ; i ++ )
	{
		RIVERTAG * pInfo = GetInfo( m_ptPath[ i ] );
		if( pInfo == NULL ) continue;
		if( pInfo->cFlag & BLUE_FLAG ) continue;

		char nRunDir = pInfo->cDir;
		if( nRunDir != nLastRunDir ) 
		{
			nCornerIndex[ nCornerNum ++ ] = i;
			nLastRunDir = nRunDir;
		}
	}
	nCornerIndex[ nCornerNum ++ ] = m_nPathLen - 1;

	// �����ҵ��Ĺյ�
	bool bConnected = false;
	int nNowIndex = -1;
	POINTS ptNow , ptTo;

	while( nNowIndex++ < nCornerNum - 1 )		// �Ӻ���ǰ��������
	{
		if( nCornerIndex[ nNowIndex ] < 0 ) continue;
		ptNow =  m_ptPath[ nCornerIndex[ nNowIndex ] ];

		for( int i = nCornerNum - 1 ; i > nNowIndex; i -- )
		{
			ptTo =  m_ptPath[ nCornerIndex[ i ] ];

			// ����������Ӿ�������м��
			ASSERT( !(ptNow.x == ptTo.x && ptNow.y == ptTo.y) );

			if( TestConnectPoint( ptNow , ptTo ) )
			{
				if( !CennectPoint( ptNow , ptTo ) )
				{
				}
				else
					bConnected = true;
				// ����м��
				for ( int j = nNowIndex+1 ; j < i ; j ++ )
					nCornerIndex[ j ] = -1;

				// ������һ����
				break;
			}
		}
	}

	m_nPathLen = MaxPathLen;
	if( bConnected == true ) GetPathAux( m_ptPath , m_nPathLen );
}
//------------------------------------------------------------------------
template< class TileContainer , int MaxPathLen >
BOOL CPathFinder_R< TileContainer , MaxPathLen >::CennectPoint( POINTS pos1 , POINTS pos2 )
{
#ifdef	_DEBUG
	ASSERT( pos1.x != pos2.x || pos1.y != pos2.y );
#endif

	RIVERTAG * pNode = NULL;
	RIVERTAG * pOldNode = NULL;
	RIVERTAG * pEndNode = GetInfo( pos2 );

	if( pEndNode == NULL ) return FALSE;

	while( 1 )
	{
#ifdef	_DEBUG
		ASSERT( PointInMap( pos1 ) );
#endif
		pNode = GetInfo( pos1 );

		if( pNode == NULL ) return FALSE;

		if( pOldNode ) 
		{
			pNode->pParent = pOldNode;
		}

		pOldNode = pNode;

		if( pEndNode == pNode )
			return TRUE;

		if( pos2.y > pos1.y ) pos1.y += 1;
		if( pos2.y < pos1.y ) pos1.y -= 1;
		if( pos2.x > pos1.x ) pos1.x += 1;
		if( pos2.x < pos1.x ) pos1.x -= 1;

		if( IsBlock( pos1 ) ) return FALSE;
	}

	return FALSE;
}
//------------------------------------------------------------------------
// �����������Ƿ�����ͨ
template< class TileContainer , int MaxPathLen >
BOOL CPathFinder_R< TileContainer , MaxPathLen >::TestConnectPoint( POINTS pos1 , POINTS pos2 )
{
#ifdef	_DEBUG
	ASSERT( pos1.x != pos2.x || pos1.y != pos2.y );
#endif

	while( 1 )
	{
#ifdef	_DEBUG
		ASSERT( PointInMap( pos1 ) );
#endif

		if( pos2.y == pos1.y && pos2.x == pos1.x )
			return TRUE;

		if( pos2.y > pos1.y ) pos1.y += 1;
		if( pos2.y < pos1.y ) pos1.y -= 1;
		if( pos2.x > pos1.x ) pos1.x += 1;
		if( pos2.x < pos1.x ) pos1.x -= 1;

		if( IsBlock( pos1 ) ) return FALSE;
	}

	return FALSE;
}
//------------------------------------------------------------------------
// �ж�һ�����Ƿ����赲��
template< class TileContainer , int MaxPathLen >
inline BOOL CPathFinder_R< TileContainer , MaxPathLen >::IsBlock( POINTS pos )
{
	ASSERT( m_pTiles );
	return m_pTiles->IsBlock( pos );
}
//------------------------------------------------------------------------
// ����һ�������ڶ��������˷����������һ��
template< class TileContainer , int MaxPathLen >
int  CPathFinder_R< TileContainer , MaxPathLen >::GetNearestDir( int nDeltaX , int nDeltaY )
{
	float fSlope; // б��
	if (nDeltaX == 0)
		return (nDeltaY >= 0) ? 4 : 0;
	else
		fSlope = (float)nDeltaY/nDeltaX;

	if (fSlope >= 0.0f) // 1,3����
	{
		if (nDeltaX >= 0) // 1
		{
			if (fSlope >= 2.414f)	return  4; // 90	(67.5��90.0) < tan(67.5) = 2.414 >
			if (fSlope >= 0.414f)	return  3; // 45	(22.5��67.5) < tan(22.5) = 0.414 >
			if (fSlope >=   0.0f)	return  2; // 0		(00.0��22.5)
		}
		else // 3
		{
			if (fSlope >= 2.414f)	return 0;
			if (fSlope >= 0.414f)	return 7;
			if (fSlope >=   0.0f)	return 6;
		}
	}
	else // 2,4
	{
		fSlope = -fSlope;
		if (nDeltaX >= 0) // 4
		{
			if (fSlope >= 2.414f)	return  0;
			if (fSlope >= 0.414f)	return  1;
			if (fSlope >=   0.0f)	return  2;
		}
		else // 2
		{
			if (fSlope >= 2.414f)	return 4;
			if (fSlope >= 0.414f)	return 5;
			if (fSlope >=   0.0f)	return 6;		
		}
	}
	return 0;
}
//------------------------------------------------------------------------