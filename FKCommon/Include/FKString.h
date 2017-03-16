/********************************************************************
*
*	���ļ���FKMMORPG��һ���֡����ļ���Ȩ����FreeKnightDuzhi(������)���С�
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	�ļ�����	FKString
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
	/** �ַ��������� */
	class FK_DLL CStringUtil
	{
	public:
		/**
		* Comment: 	ɾ���ַ����Ŀհ��ֽں�TAB�Ʊ��
		* @param:
		* @return:
		* @remarks:	��������ָ����ɾ���ַ���ǰ���Ŀհס���Ĭ�������߶�ɾ����
		* @note:
		*/
		static void Trim( OUT String& p_str, bool p_bLeft = true, bool p_bRight = true );
		/**
		* Comment: 	��һ���ַ������ݷָ����ָ�Ϊһ���ַ�������
		* @param:	p_Delims	�ָ���б�
		* @param:	p_unMaxSplits	���ָ����������ֵΪ0�����ʾ�����Ʒָ
					��>0�����ʾ�ָ�ָ���������ٽ��зָ�
		* @return:
		* @remarks:
		* @note:
		*/
		static std::vector< String > Split( const String& p_str, const String& p_Delims = "\t\n ", unsigned int p_unMaxSplits = 0 );
		/**
		* Comment: 	��һ���ַ����������ַ�ת��ΪСд�ַ�
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static void ToLower( OUT String& p_Str );
		/**
		* Comment: 	��һ���ַ��������ַ�ת��Ϊ��д�ַ�
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static void ToUpper( OUT String& p_Str );
		/**
		* Comment: 	�ж�һ���ַ����Ƿ�����ָ���ο��ַ���Ϊ��ͷ��
		* @param:	p_Pattern	�ο��ַ���
		* @param:	p_bLowerCase	���ò���Ϊtrue������ζ����Ҫ���бȽϵ��ַ����ڱȽ�ǰ����ת��ΪСд�ַ���
					��Ȼ���ο��ַ���Ҳ����Сд��
		* @return:
		* @remarks:
		* @note:
		*/
		static bool	IsStartWith( const String& p_Str, const String& p_Pattern, bool p_bLowerCase = true );
		/**
		* Comment: 	�ж�һ���ַ����Ƿ�����ָ���ο��ַ���Ϊ��β��
		* @param:	p_Pattern	�ο��ַ���
		* @param:	p_bLowerCase	���ò���Ϊtrue������ζ����Ҫ���бȽϵ��ַ����ڱȽ�ǰ����ת��ΪСд�ַ���
					��Ȼ���ο��ַ���Ҳ����Сд��
		* @return:
		* @remarks:
		* @note:
		*/
		static bool IsEndWith( const String& p_Str, const String& p_Pattern, bool p_bLowerCase = true );
		/**
		* Comment: 	��׼��·��������׼�����·������ǰ��б�ߣ�Ҳ����ǰб����Ϊ��β��
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static String StandardPath( const String& p_Str );
		/**
		* Comment: 	��һ���������ļ���·���ָ�Ϊ�����ļ�����·�������֡�
		* @param:
		* @return:
		* @remarks:	ע�⣺·��������׼������
		* @note:
		*/
		static void SplitFileName( const String& p_WholeName, OUT String& p_BaseName, OUT String& p_Path );
		/**
		* Comment: 	�ȶ��ļ����Ƿ�����ĳ�������ļ���������׺��
		* @param:	p_Pattern	�ȶԲ����ַ����������԰�����*��ͨ���
		* @param:	p_bCaseSensitive	�Ƿ�Դ�Сд����
		* @return:
		* @remarks:
		* @note:
		*/
		static bool MatchFile( const String& p_Str, const String& p_Pattern, bool p_bCaseSensitive = false );
	private:
		///	StringUtil�����հ��ַ���
		static const String			ms_StringUtilTemp;
	};
}