/********************************************************************
*
*	���ļ���FKMMORPG��һ���֡����ļ���Ȩ����FreeKnightDuzhi(������)���С�
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	�ļ�����	FKSharedPtr
*	���ߣ�		FreeKnightDuzhi[ ������ ]
*	�ļ�˵����	
*
*	�޸���:		
*	�޸�����:
*********************************************************************/

#pragma once

//-------------------------------------------------------------------
#include "FKCommonPre.h"
//-------------------------------------------------------------------
namespace FKCommon
{
	/** ���ü�������ָ�롣
	* @remarks	����һ����׼�����ü���ָ�룬Ҳ��Щ�ط���֮ΪSmartPtr��
	*			��˼��һ���ģ�������û�ж������ʱ�Լ��ͷš�
	*			FK�����ﲻ������ʹ��������Ϊ��ʽdeleteָ����Ӹ�Ч��
	*			���Ǹ�����������ȷ�޷�֪��һ�����󱻶�����ʹ��ʱ��
	*			���Կ���ʹ������
	* @note		���Ƕ��̰߳�ȫ�ġ�
	*/
	template< class T >
	class TSharedPtr
	{
	public:
		FK_AUTO_SHARED_MUTEX
	public:
		/**
		* Comment: 	���캯��
		* @param:
		* @return:
		* @remarks:	û�н��г�ʼ����������
		* @note:
		*/

		TSharedPtr()
			: m_pUseCount( 0 )
			, m_pDataPtr( NULL )
		{
			FK_SET_AUTO_SHARED_MUTEX_NULL
		}
		explicit TSharedPtr( T* p_pPtr )
			: m_pDataPtr( p_pPtr )
			, m_pUseCount( new unsigned int(1) )
		{
			FK_SET_AUTO_SHARED_MUTEX_NULL
			FK_NEW_AUTO_SHARED_MUTEX
		}
		TSharedPtr( const TSharedPtr& p_Other )
			: m_pDataPtr( 0 )
			, m_pUseCount( 0 )
		{
			FK_SET_AUTO_SHARED_MUTEX_NULL
			FK_MUTEX_CONDITIONAL( p_Other.FK_AUTO_MUTEX_NAME )
			{
				FK_LOCK_MUTEX( *p_Other.FK_AUTO_MUTEX_NAME )
				FK_COPY_AUTO_SHARED_MUTEX( p_Other.FK_AUTO_MUTEX_NAME )
				m_pDataPtr = p_Other.m_pDataPtr;
				m_pUseCount = p_Other.m_pUseCount;
				if( m_pUseCount != NULL )
				{
					++(*m_pUseCount);
				}
			}
		}
		TSharedPtr& operator= ( const TSharedPtr& p_Other )
		{
			if( m_pDataPtr == p_Other.m_pDataPtr )
			{
				return *this;
			}
			Release();
			FK_MUTEX_CONDITIONAL( p_Other.FK_AUTO_MUTEX_NAME )
			{
				FK_LOCK_MUTEX( *p_Other.FK_AUTO_MUTEX_NAME )
					FK_COPY_AUTO_SHARED_MUTEX( p_Other.FK_AUTO_MUTEX_NAME )
					m_pDataPtr = p_Other.m_pDataPtr;
				m_pUseCount = p_Other.m_pUseCount;
				if( m_pUseCount != NULL )
				{
					++(*m_pUseCount);
				}
			}
			return *this;
		}
		virtual ~TSharedPtr()
		{
			Release();
		}
	public:
		__forceinline T& operator*() const
		{
			assert( m_pDataPtr );
			return *m_pDataPtr;
		}
		__forceinline T* operator->() const
		{
			assert( m_pDataPtr );
			return m_pDataPtr;
		}
		__forceinline T* Get() const
		{
			return m_pDataPtr;
		}
		__forceinline bool IsUnique() const
		{
			FK_LOCK_AUTO_SHARED_MUTEX
			assert( m_pUseCount != NULL );
			return ( *m_pUseCount == 1 );
		}
		__forceinline unsigned int GetUseCount() const
		{
			FK_LOCK_AUTO_SHARED_MUTEX
			assert( m_pUseCount != NULL );
			return *m_pUseCount;
		}
		__forceinline unsigned int GetUseCountPtr() const
		{
			return m_pUseCount;
		}
		__forceinline T* GetPtr() const
		{
			return m_pDataPtr;
		}
		__forceinline bool IsNull() const
		{
			return ( m_pDataPtr == NULL );
		}
		__forceinline void Clear()
		{
			if( m_pDataPtr != NULL )
			{
				Release();
				m_pDataPtr = NULL;
				m_pUseCount = NULL;
			}
		}
	public:
		/**
		* Comment: 	������ָ�뵽���ü�������ָ����
		* @param:
		* @return:
		* @remarks:	��������ü�������ָ��δ����ʼ��
		* @note:
		*/
		void Bind( T* p_pDataPtr )
		{
			assert( m_pDataPtr == NULL );
			assert( m_pUseCount == NULL );
			FK_NEW_AUTO_SHARED_MUTEX
			FK_LOCK_AUTO_SHARED_MUTEX
			m_pUseCount = new unsigned int(1);
			m_pDataPtr = p_pDataPtr;
		}
	protected:
		inline void Release()
		{
			bool bDestroyThis = false;

			FK_MUTEX_CONDITIONAL( FK_AUTO_MUTEX_NAME )
			{
				FK_LOCK_AUTO_SHARED_MUTEX
				if( NULL != m_pUseCount )
				{
					if( (--(*m_pUseCount)) == 0 )
					{
						bDestroyThis = true;
					}
				}
			}
			if( bDestroyThis )
			{
				Destroy();
			}
			FK_SET_AUTO_SHARED_MUTEX_NULL
		}

		virtual void Destroy()
		{
			// ����㵱�������˵�������˳�����֮ǰ��ָ�������ͷ�
			// �����˳�ǰʹ��Clear()������
			delete m_pDataPtr;
			delete m_pUseCount;
			FK_DELETE_AUTO_SHARED_MUTEX
		}
	protected:
		///	����������ָ��
		T*				m_pDataPtr;
		/// ���ü���
		unsigned int*	m_pUseCount;
	};

	template< class T, class U >
	inline bool operator==( TSharedPtr< T > const& p_A, TSharedPtr< U > const& p_B )
	{
		return (p_A.Get() == p_B.Get());
	}
	template< class T, class U >
	inline bool operator!=( TSharedPtr< T > const& p_A, TSharedPtr< U > const& p_B )
	{
		return (p_A.Get() != p_B.Get());
	}
}