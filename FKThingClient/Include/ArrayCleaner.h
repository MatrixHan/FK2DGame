/**
*	created:		2012-7-2   19:42
*	filename: 		ArrayCleaner
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ģ�溯��<��������е���Ч����>,�����еĶ������Ϊflat��ʽ������Ҫ���������memcpy����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
template< class TYPE >
struct is_item_invalid
{
	bool operator()( const TYPE& d1 ) const 
	{
		return d1 == 0;
	}
};
//------------------------------------------------------------------------
#define __COMP_FUN	is_item_invalid< TYPE >
//------------------------------------------------------------------------
// ���������е���Ч����
// ����	: �� ���� 0 1 0 3 5 0 8
// ��� :    ���� 1 3 5 8
// pArray	: ���������� ( IN / OUT ) 
// nArrayLen: ���鳤�� ( IN / OUT )   ���ش�����������Ч����
// __is_item_invalid : �ж�ĳ��Ԫ���Ƿ���Ч�ķº���
template< class TYPE >
void CleanArray( TYPE * pArray , int & nArrayLen , __COMP_FUN & __is_item_invalid )
{
	int nMovedIndex = 0;
	int nStartCopy = 0;
	for( nStartCopy = 0 ; nStartCopy< nArrayLen ; nStartCopy ++ )
	{
		if( !__is_item_invalid( pArray[ nStartCopy ] ) )
		{
			int nEndCopy = nStartCopy + 1;
			for( ; nEndCopy < nArrayLen ; nEndCopy ++ )
			{
				if( __is_item_invalid( pArray[ nEndCopy ] ) )
				{
					break;
				}
			}

			if( nMovedIndex != nStartCopy )
			{
				memcpy( &pArray[ nMovedIndex ] , 
					&pArray[ nStartCopy ] , 
					(nEndCopy-nStartCopy)*sizeof( TYPE ) );
			}

			nMovedIndex += nEndCopy-nStartCopy;
			nStartCopy = nEndCopy;
		}
	}

	nArrayLen = nMovedIndex;
}
//------------------------------------------------------------------------