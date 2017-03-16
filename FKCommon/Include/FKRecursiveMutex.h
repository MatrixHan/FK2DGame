/********************************************************************
*
*	���ļ���FKMMORPG��һ���֡����ļ���Ȩ����FreeKnightDuzhi(������)���С�
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	�ļ�����	FKRecursiveMutex
*	���ߣ�		FreeKnightDuzhi[ ������ ]
*	�ļ�˵����	
*
*	�޸���:		
*	�޸�����:
*********************************************************************/

#pragma once

//-------------------------------------------------------------------
#include <windows.h>
#include "FKNoCopyable.h"
//-------------------------------------------------------------------

namespace FKCommon
{
	class FK_DLL CFKRecursiveMutex : public IFKNoCopyable
	{
		//-------------------------------------------------------------------
		/**	һ���������������Է�ֹ�����ͷ�����
		*/
		class CFKScopedLock : public IFKNoCopyable
		{
			friend class CFKRecursiveMutex;
		public:
			CFKScopedLock()
				: m_pMutex( NULL )
			{

			}
			CFKScopedLock( CFKRecursiveMutex& p_Mutex )
			{
				Acquire( p_Mutex );
			}
			~CFKScopedLock()
			{
				if( NULL != m_pMutex )
				{
					Release();
				}
			}
			void Acquire( CFKRecursiveMutex& p_Mutex )
			{
				m_pMutex = &p_Mutex;
				p_Mutex.Lock();
			}
			bool TryAcquire( CFKRecursiveMutex& p_Mutex )
			{
				bool bResult = p_Mutex.TryLock();
				if( bResult )
				{
					m_pMutex = &p_Mutex;
				}
				return bResult;
			}
			void Release()
			{
				m_pMutex->Unlock();
				m_pMutex = NULL;
			}
		private:
			CFKRecursiveMutex*	m_pMutex;
		};
		//-------------------------------------------------------------------
		friend class CFKScopedLock;
	public:
		CFKRecursiveMutex()
		{
			InitializeCriticalSection( &m_Impl );
		}
		~CFKRecursiveMutex()
		{
			DeleteCriticalSection( &m_Impl );
		}
		void Lock()
		{
			EnterCriticalSection( &m_Impl );
		}
		bool TryLock()
		{
			return ( TryEnterCriticalSection( &m_Impl ) != 0 );
		}
		void Unlock()
		{
			LeaveCriticalSection( &m_Impl );
		}
	private:
		CRITICAL_SECTION		m_Impl;
	};
}