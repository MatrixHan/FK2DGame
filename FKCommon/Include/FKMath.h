/********************************************************************
*
*	���ļ���FKMMORPG��һ���֡����ļ���Ȩ����FreeKnightDuzhi(������)���С�
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	�ļ�����	FKMath
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
class CDegree;
class CRadian;
class CVector2f;
class CVector3f;
class CVector4f;
class CRay;
class CSphere;
class CMatrix4f;
class CPlane;
class CAxisAlignedBox;
//-------------------------------------------------------------------
namespace FKCommon
{
	/** ���õ���ѧ������
	*/
	class FK_DLL CMath
	{
	public:
		/**
		* Comment: 	��������ö��
		*/
		enum ENUM_AngleStyle
		{
			EAS_Degree,			// �Ƕ�
			EAS_Radian,			// ����
		};
	protected:
		static ENUM_AngleStyle	ms_eAngleStyle;		// ��������float 
		static float*			ms_pSinTable;		// Sin��
		static float*			ms_pTanTable;		// Tan��
		static int				ms_nTableSize;		// ���Ǳ��С
		static float			ms_fTableFactor;	// ���Ǳ����
	public:
		static const float POSTIVE_INFINITY;		// ������
		static const float NEGATIVE_INFINITY;		// ������
		static const float DEGREE_2_RADIAN_PARAM;	// �Ƕ�ת���ȵĲ���
		static const float RADIAN_2_DEGREE_PARAM;	// ����ת�ǶȵĲ���
	public:
		static float SinTable( float p_fValue );
		static float TanTable( float p_fValue );
	public:
		/**
		* Comment: 	ȡ��
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline int GetAbs( int p_nValue );
		static __forceinline float GetAbs( float p_fValue );
		static __forceinline CDegree GetAbs( const CDegree& p_Deg );
		static __forceinline CRadian GetRad( const CRadian& p_Rad );
		/**
		* Comment: 	��ȡ���ڻ���ڲ���ֵ������
		* @param:	p_fValue ����
		* @return:
		* @remarks: ���� GetCeil( 12.64f ) �õ�����ֵ�� 13 �� 13.00f�����ݺ�������ֵ����ͬ��
		* @note:
		*/
		static __forceinline int GetIntCeil( float p_fValue );
		static __forceinline float GetCeil( float p_fValue );
		/**
		* Comment: 	��ȡС�ڻ���ڲ���ֵ������
		* @param:	p_fValue ����
		* @return:
		* @remarks: ���� GetFloor( 12.64f ) �õ��ķ���ֵ�� 12 �� 12.00f�����ݺ�������ֵ����ͬ��
		* @note:
		*/
		static __forceinline int GetIntFloor( float p_fValue );
		static __forceinline float GetFloor( float p_fValue );
		/**
		* Comment: 	�ж�һ�������ķ���
		* @param:
		* @return:	������0�򷵻� 1�� ��С��0�򷵻� -1�� ������0�򷵻�0
		* @remarks:
		* @note:
		*/
		static __forceinline int GetSign( int p_nValue );
		static __forceinline float GetSign( float p_fValue );
		static __forceinline CRadian GetSign( const CRadian& p_Rad );
		static __forceinline CDegree GetSign( const CDegree& p_Deg );
		/**
		* Comment: 	��ĳֵ�ķ�����ֵ
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline CRadian GetArcCos( float p_fValue );
		/**
		* Comment: 	��ĳֵ�ķ�����ֵ
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline CRadian GetArcSin( float p_fValue );
		/**
		* Comment: 	��ĳֵ�ķ�����ֵ
		* @param:
		* @return:	����ֵ�� -pi/2 �� pi/2 ֮�䡣
		* @remarks:
		* @note:
		*/
		static __forceinline CRadian GetArcTan( float p_fValue );
		/**
		* Comment: 	�����X,Y����ķ�����ֵ
		* @param:	p_fY ��ʾ���Y����ֵ��p_fX ��ʾ���X����
		* @return:	����ֵΪ�������ʾ��X����ʱ����ת�ĽǶȡ�
					����ֵΪ�������ʾ��X��˳ʱ����ת�ĽǶȡ�
					����ֵȡֵ��Χ�� -pi �� pi ֮�䣨������-pi��
		* @remarks:
		* @note:
		*/
		static __forceinline CRadian GetArcTan2( float p_fY, float p_fX );
		/**
		* Comment: 	Cos����
		* @param:	p_Rad ����ֵ
		* @param:	p_bUseTable �Ƿ���
		* @return:
		* @remarks:	�����в������Ը���ĵõ�Cosֵ�����ǽ���ʧһ���ľ��ȡ�
		* @note:
		*/
		static __forceinline float GetCos( const CRadian& p_Rad, bool p_bUseTable = false );
		/**
		* Comment: 	Cos����
		* @param:	p_fValue ����ֵ
		* @param:	p_bUseTable �Ƿ���
		* @return:
		* @remarks:	�����в������Ը���ĵõ�Cosֵ�����ǽ���ʧһ���ľ��ȡ�
		* @note:
		*/
		static __forceinline float GetCos( float p_fValue, bool p_bUseTable = false );
		/**
		* Comment: 	Sin����
		* @param:	p_Rad ����ֵ
		* @param:	p_bUseTable �Ƿ���
		* @return:
		* @remarks:	�����в������Ը���ĵõ�Cosֵ�����ǽ���ʧһ���ľ��ȡ�
		* @note:
		*/
		static __forceinline float GetSin( const CRadian& p_Rad, bool p_bUseTable = false );
		/**
		* Comment: 	Sin����
		* @param:	p_fValue ����ֵ
		* @param:	p_bUseTable �Ƿ���
		* @return:
		* @remarks:	�����в������Ը���ĵõ�Cosֵ�����ǽ���ʧһ���ľ��ȡ�
		* @note:
		*/
		static __forceinline float GetSin( float p_fValue, bool p_bUseTable = false );
		/**
		* Comment:	Tan����
		* @param:	p_Rad ����ֵ
		* @param:	p_bUseTable �Ƿ���
		* @return:
		* @remarks:	�����в������Ը���ĵõ�Cosֵ�����ǽ���ʧһ���ľ��ȡ�
		* @note:
		*/
		static __forceinline float GetTan( const CRadian& p_Rad, bool p_bUseTable = false );
		/**
		* Comment:	Tan����
		* @param:	p_fValue ����ֵ
		* @param:	p_bUseTable �Ƿ���
		* @return:
		* @remarks:	�����в������Ը���ĵõ�Cosֵ�����ǽ���ʧһ���ľ��ȡ�
		* @note:
		*/
		static __forceinline float GetTan( float p_fValue, bool p_bUseTable = false );
		/**
		* Comment: 	�� e �� x �η�
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline float GetExp( float p_fValue );
		/**
		* Comment: 	��x�Ķ���
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline float GetLog( float p_fValue );
		/**
		* Comment: 	�� x �� y ����
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline float GetPow( float p_fBase, float p_fExponent );
		/**
		* Comment: 	��ƽ��
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline float GetSqr( float p_fValue );
		/**
		* Comment: 	��ƽ����
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline float GetSqrt( float p_fValue );
		static __forceinline CRadian GetSqrt( const CRadian& p_Rad );
		static __forceinline CDegree GetSqrt( const CDegree& p_Deg );
		/**
		* Comment: 	��ƽ�����ĵ���
		* @param:
		* @return:
		* @remarks: �� 1 / GetSqrt( p_fValue ) ����������׼������Ҫ
		* @note:
		*/
		static __forceinline float InvSqrt( float p_fValue );
		/**
		* Comment: 	��á�0��1���������
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline float UnitRandom();
		/**
		* Comment: 	��á�p_fLow, p_fHigh���������
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline float RangeRandom( float p_fLow, float p_fHeight );
		/**
		* Comment: 	��á�-1��1���������
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline float SymmetricRandom();
		/**
		* Comment: 	���Ƕ�ת��Ϊ����
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline float Degree2Radian( float p_fDegValue );
		/**
		* Comment: 	������תΪ�Ƕ�
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static __forceinline float Radian2Degree( float p_fRadValue );
		/**
		* Comment: 	ת��Math����Ϊ����
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static float Angle2Radian( float p_fAngle );
		/**
		* Comment: 	ת������ΪMath����
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static float Radian2Angle( float p_Rad );
		/**
		* Comment: 	ת��Math����Ϊ�Ƕ�
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static float Angle2Degree( float p_fAngle );
		/**
		* Comment: 	ת���Ƕ�ΪMath����
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static float Degree2Angle( float p_Deg );
		/**
		* Comment: 	����Math��������
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static void SetAngleStyle( ENUM_AngleStyle p_eStyle );
		/**
		* Comment: 	��ȡMath��������
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static ENUM_AngleStyle GetAngleStyle();
	public:
		/**
		* Comment: 	�ж�һ�����Ƿ���ƽ�����������
		* @param:	p_VecPoint ��Ҫ�����ĵ�
		* @param:	p_VecA��p_VecB��p_VecC ��ƽ���ϵ���������������
		* @return:	�����������ڣ��򷵻�true�������������⣬�򷵻�false
		* @remarks:
		* @note:
		*/
		static bool IsPointInTri2D( const CVector2f& p_VecPoint,
			const CVector2f& p_VecA, const CVector2f& p_VecB, const CVector2f& p_VecC );
		/**
		* Comment: 	�ж�һ�����Ƿ���3ά�ռ���������
		* @param:	p_VecPoint ��Ҫ�����ĵ�
		* @param:	p_VecA��p_VecB��p_VecC �ǿռ��ϵ���������������
		* @param:	p_VecNormal �ռ��������εķ���
		* @return:	�����������ڣ��򷵻�true�������������⣬�򷵻�false
		* @remarks:
		* @note:
		*/
		static bool IsPointInTri3D( const CVector3f& p_VecPoint, const CVector3f& p_VecA,
			const CVector3f& p_VecB, const CVector3f& p_vecC, const CVector3f& p_VecNormal );
	public:
		/**
		* Comment: 	�������ߺ�ƽ�潻���ж�
		* @param:	
		* @return:	�����Ƿ��ཻ��ͬʱ�������ߺ�ƽ��ľ��롣�����ཻ���򷵻� std::pair< false, 0 >
		* @remarks:
		* @note:
		*/
		static std::pair< bool, float > Intersect( const CRay& p_Ray, const CPlane& p_Pla );
		/**
		* Comment: 	�ж�һ�����ߺ�����Ľ���
		* @param:	p_bDiscardInside ��������ʼ�������ڣ��Ƿ��������
		* @return:	�����Ƿ��ཻ��ͬʱ�������ߺ�����ľ���,�����ཻ���򷵻� std::pair< false, 0 >
		* @remarks:
		* @note:
		*/
		static std::pair< bool, float > Intersect( const CRay& p_Ray, const CSphere& p_Sphere, bool p_bDiscardInside = true );
		/**
		* Comment: 	�ж�һ�������Ƿ��һ������������н���
		* @param:
		* @return:	�����Ƿ��ཻ��ͬʱ�������ߺ;�������еľ���.�����ཻ���򷵻� std::pair< false, 0 >
		* @remarks:
		* @note:
		*/
		static std::pair< bool, float > Intersect( const CRay& p_Ray, const CAxisAlignedBox& p_Box );
		/**
		* Comment: 	�ж�һ�������Ƿ��һ������������н��㣬ͬʱ���������ཻ�����������ľ��롣
		* @param:	p_fNearDis ���������Ͻ��Ľ��� �� ������� ֮��ľ��롣
		* @param:	p_fFarDis ����������Զ�Ľ��� �� ������� ֮��ľ��롣
		* @return:	���ཻ���򷵻�true��ͬʱ���Ա�֤ p_fFarDis >= p_fNearDis >= 0
		* @remarks:
		* @note:	����������������û�н��㣬�򷵻�false��ͬʱ��p_fNearDis��p_fFarDis ��ֵ�����ı䡣
		*/
		static bool Intersect( const CRay& p_Ray, const CAxisAlignedBox& p_Box, OUT float* p_fNearDis, OUT float* p_fFarDis );
		/**
		* Comment: 	�ж�һ�����ߺ�һ��3D�ռ��������εĹ�ϵ
		* @param:	p_VecNormal �����εķ���
		* @param:	p_bDisposalPositiveSide �Ƿ���������������潻��
		* @param:	p_bDisposalNegativeSide �Ƿ�����������ķ��潻��
		* @return:	���������������ཻ���򷵻� std::pair< true, ��������뽻��֮��ľ��� >
		* @remarks:
		* @note:	��������������û�н��㣬�򷵻�std::pair< false, 0 >
		*/
		static std::pair< bool, float > Intersect( const CRay& p_Ray, const CVector3f& p_VecA, const CVector3f& p_VecB,
			const CVector3f& p_VecC, const CVector3f& p_VecNormal, bool p_bDisposalPositiveSide = true, bool p_bDisposalNegativeSide = true );
		/**
		* Comment: 	�ж�һ�����ߺ�һ��3D�ռ��������εĹ�ϵ
		* @param:	p_bDisposalPositiveSide �Ƿ���������������潻��
		* @param:	p_bDisposalNegativeSide �Ƿ�����������ķ��潻��
		* @return:	���������������ཻ���򷵻� std::pair< true, ��������뽻��֮��ľ��� >
		* @remarks:
		* @note:	��������������û�н��㣬�򷵻�std::pair< false, 0 >��������ķ��������������������ڲ����м����á�
		*/
		static std::pair< bool, float > Intersect( const CRay& p_Ray, const CVector3f& p_VecA, const CVector3f& p_VecB,
			const CVector3f& p_VecC, bool p_bDisposalPositiveSide = true, bool p_bDisposalNegativeSide = true );
		/**
		* Comment: 	���һ�������Ƿ��һ������������ཻ
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static bool Intersect( const CSphere& p_Sph, const CAxisAlignedBox& p_Box );
		/**
		* Comment: 	�ж�һ��ƽ���Ƿ��һ������������ཻ
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static bool Intersect( const CPlane& p_Pla, const CAxisAlignedBox& p_Box );
		/**
		* Comment: 	�ж�һ�������Ƿ��һ��ƽ���ཻ
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static bool Intersect( const CSphere& p_Sph, const CPlane& p_Pla );
		/**
		* Comment: 	�ж�һ�����ߺ�һ��Plane֮����ཻ��ϵ
		* @param:	p_listPlane һ�����͹���������
		* @param:	p_bIsNormalOutside ������ķ����Ƿ���͹�������⡣
		* @return:
		* @remarks:
		* @note:
		*/
		static std::pair< bool, float > Intersect( const CRay& p_Ray, 
			const std::vector<CPlane>& p_listPlane, bool p_bIsNormalOutside = true );
		static std::pair< bool, float > Intersect( const CRay& p_Ray, 
			const std::list<CPlane>& p_listPlane, bool p_bIsNormalOutside = true );
		/**
		* Comment: 	�ж�����float�Ƿ����
		* @param:	p_fTolerance �ݲֵ
		* @return:
		* @remarks:
		* @note:
		*/
		static bool IsEqual( float p_fValueA, float p_fValueB, float p_fTolerance = std::numeric_limits<float>::epsilon() );
		/**
		* Comment: 	����һ������������������������������пռ�
		* @param:	
		* @return:
		* @remarks:	
		* @note:	����3D�ռ���һ��������˵���������пռ�����������ֱ�Ϊ�õ� ����N������T��������B��
					�пռ����� M = �� T�� B, N ��������пռ���� BumpMap �ƹ�ת��������ܴ�
		*/
		static CVector3f CalculateTangentSpaceVector( const CVector3f& p_VecPos1, const CVector3f& p_VecPos2,
			const CVector3f& p_VecPos3, float p_fU1, float p_fV1, float p_fU2, float p_fV2, float p_fU3, float p_fV3 );
		/**
		* Comment:  Ϊһ���洴��һ���������
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static CMatrix4f BuildReflectionMatrix( const CPlane& p_Pla );
		/**
		* Comment: 	��������������ķ��ߣ�������W��Ϣ��
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static CVector3f CalculateBasicFaceNormal( const CVector3f& p_Vec1, const CVector3f& p_Vec2, const CVector3f& p_Vec3 );
		/**
		* Comment: 	��������������ķ��ߣ�������W��Ϣ��
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static CVector4f CalculateFaceNormal( const CVector3f& p_Vec1, const CVector3f& p_Vec2, const CVector3f& p_Vec3 );
		/**
		* Comment: 	��������������ķ��ߣ�������W��Ϣ���������и�ʽ��
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static CVector3f CalculateBasicFaceNormalWithoutNormalize( const CVector3f& p_Vec1, const CVector3f& p_Vec2, const CVector3f& p_Vec3 );
		/**
		* Comment: 	��������������ķ��ߣ�������W��Ϣ���������и�ʽ��
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static CVector4f CalculateFaceNormalWithoutNormalize( const CVector3f& p_Vec1, const CVector3f& p_Vec2, const CVector3f& p_Vec3 );
	public:
		/**
		* Comment: 	��һ��ֵ����һ��ƫ��ֵ�ͱ����������и�˹�����õ�ֵ
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		static float GetGaussianDistribution( float p_fValue, float p_fOffset = 0.0f, float p_fScale = 1.0f );
	protected:
		/**
		* Comment: 	�����������Ǳ�
		* @param:
		* @return:
		* @remarks:
		* @note:
		*/
		void		_BuildTrigTables();
	private:
		float ASM_ARCCOS( float r );
		float ASM_ARCSIN( float r );
		float ASM_ARCTAN( float r );
		float ASM_SIN( float r );
		float ASM_COS( float r );
		float ASM_TAN( float r );
		float ASM_SQRT( float r );			// ���
		float ASM_RSQRT( float r );			// ���� 1/a, ����aΪr�ĸ���
		float CARMAK_INV_SQRT( float r );	// ������������������1/a, ����aΪr�ĸ�
		float ASM_LN( float r );			// ���� log2(r) / log2(e)
	public:
		/**
		* Comment: 	���캯��
		* @param:	p_unTableSize ���Ǳ�Ĵ�С
		* @return:
		* @remarks: ���Ǳ���һ������ʵ�� Sin, Cos, Tan ��ֵ��һ�����ٲ��ұ�
		* @note:
		*/
		CMath( unsigned int p_unTableSize = 4096 );
		~CMath();
		//-------------------------------------------------------------------
		#include "FKMath.inl"
		//-------------------------------------------------------------------
	};
}

