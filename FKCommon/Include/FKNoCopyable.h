/********************************************************************
*
*	���ļ���FKMMORPG��һ���֡����ļ���Ȩ����FreeKnightDuzhi(������)���С�
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	�ļ�����	FKNoCopyable
*	���ߣ�		FreeKnightDuzhi[ ������ ]
*	�ļ�˵����	
*
*	�޸���:		
*	�޸�����:
*********************************************************************/

#pragma once

//-------------------------------------------------------------------

namespace FKCommon
{
	/** ˽�п��������˽�����ز������࣬��֤�̳��ڸ�������಻���ܱ�������
	*/
	class FK_DLL IFKNoCopyable
	{
	protected:
		IFKNoCopyable(){}
		~IFKNoCopyable(){}
	private:
		IFKNoCopyable( const IFKNoCopyable& );
		const IFKNoCopyable& operator= ( const IFKNoCopyable& );
	};
}