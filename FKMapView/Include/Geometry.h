/**
*	created:		2012-7-1   2:18
*	filename: 		Geometry
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
// �߶�����ɢ����
class CLineDiffuse
{
public:
	CLineDiffuse()
	{
		m_nArrayWidth = 0;
		m_nArrayHeight = 0;
	}
	BOOL Create(int nWidth,int nHeight)
	{
		m_nArrayWidth = nWidth;
		m_nArrayHeight = nHeight;
		return TRUE;
	}
	void Line(int x1, int y1, int x2, int y2);

	
	enum {retFalse = 0,retTrue = 1,retStop = 2};
	/*
	 *	����retFalse��ʾ�����˳�������retTrue���ֳɹ�������retStop��ʾ�ɹ�����������
	 */
	virtual int OnFoundTile(int x,int y) = NULL;

private:
	int m_nArrayWidth;
	int m_nArrayHeight;
};
//------------------------------------------------------------------------
// ȡ��һ��tile�����ڵ����п�
// ��Ϊ��Ļ��ʾ�������Ӧ��һ�����Σ�������Ҫ������enum��ÿ����
// �����rcTileArea�����{tx,ty,col,row},(tx,ty)��ʾ���Ͻ�tile���꣬(col,row)�ֱ��ʾ��Ļ���������ĸ�����
class CEnumTileArea
{
public:
	CEnumTileArea()
	{
		m_nArrayWidth = 0;
	}
	BOOL Create(int nWidth)
	{
		m_nArrayWidth = nWidth;
		return TRUE;
	}
	BOOL IsValidCo(int x,int y)
	{
		return x >= 0 && x < m_nArrayWidth && x >= 0 && x < m_nArrayWidth;
	}
	void EnumTileArea(const RECT& rcTileArea);
	enum {retFalse = 0,retTrue = 1,retStop = 2};
	/*
	 *	����retFalse��ʾ�����˳�������retTrue���ֳɹ�������retStop��ʾ�ɹ�����������
	 */
	virtual int OnFoundTile(int x,int y) = NULL;

private:
	int m_nArrayWidth;
};
//------------------------------------------------------------------------
/* ��CDiffuseBase
˵��		��
				1������һ�����࣬Ҫ��������Բ����ɢ����Բ����ɢ
				2��������ඨ���˼򵥵Ĵ������л�ȡÿһȦ����㷨
				3���벻Ҫֱ��ʹ�������
*/
class CDiffuseBase
{
	class VEC_POINT ;
	public:
	// ��ʼ�����������������ͼ�ĳߴ�,ע�⣺��ͼ����Ϊ0 based,����X�������ΪnWidth-1
	void Create(int nWidth,int nHeight) ;
	
	CDiffuseBase() ;
	virtual ~CDiffuseBase() ;

protected:

	virtual BOOL IsNeedCheckEdge() = NULL ;						// ��Ҫ�ж��Ƿ���Ҫ�жϱ߽�
	virtual BOOL IsDiffuseComplete( int nCircleIndex ) = NULL ; // �ж��Ƿ������ɢ�����ˣ�Բ�Ļ��Ϳ��Ƿ񵽹涨�İ뾶��

	// �ṩ������������Ӧ���麯��
	enum{ retFalse = 0 , retTrue = 1 , retStop = 2 , retBlock = 3 , retContinue = 4 } ;
	virtual void OnBeginNewCircle() = NULL;						// ֪ͨ���࿪ʼ�µ�һȦ
	virtual int OnFoundTile( int x , int y ) = NULL;			// ֪ͨ���࣬�ҵ�һ���µ�TILE

protected:
	BOOL BeginDiffuse( int nX , int nY ) ;						// ��ʼ��ɢ����Ϊ���ĵ�����
	
	// ���������һ����(x,y)��������������������ڵ�����ӵ���OnFoundtile(...)��
	int TryFindOneTile( int x , int y ,
						int nCurPos , int nCurCirclePointNum ,
						int nCircleIndex ,
						BOOL bIsNeedCheckEdge ,
						int nHeadInArr ,
						BOOL bIsAntiClocekWise) ;

	// ��ʼ����־����m_arrBlockedMark
	void ResetCurMark() ;
	void BlockBetween2Lines(  POINT *arrStartLine , 
								int nStartNumOfLine ,
							  POINT *arrEndLine , 
								int nEndNumOfLine ,
							 int nMinCircleIndex   ,
							BOOL		bIsSmallAngle ,
							BOOL bIsDrawCircleBlocked) ;
	void GetLine( int nX , int nY , POINT *arrLine , int &nNumOfLine  ) ;
protected:
	int m_nWidth;	//������ͼ�Ŀ�
	int m_nHeight;
	int m_nOX ;		// ��ɢԭ���X���꣬ע�⣬�����������ͼ������	
	int m_nOY ;

	BOOL IsBlocked( int x , int y ) ; // ������Ƿ񱻵�ס�ˣ�ע�⣺�Ǿ����е�����

private:
	// ��һ�����������е���Ϊ�����ڵ���������㷨�������ĸ�ʽ�������
	void MarkAllBlockTile(	int iPosInCircle		,	// ���������һȦ�е�λ�� 
							int iNumOfCirclePoint	,	// ��һȦ�ж��ٸ���
							int iCircleIndex		,	// ���ǵڼ�Ȧ��	void ResetCurMark() ;
							int nHeadInArr			  ,// ��һȦ��һ��Ԫ���������е�λ��
									BOOL	bIsSmallAngle ,
									BOOL bIsDrawCircleBlocked) ;	

	inline void InsertPoint( int nX , int nY , POINT *arrPoint , int &nNumOfLine ) ;
	BOOL IsInLine( POINT *arrLine , int nNumOfLine , int nX , int nY ) ;
	static int m_arrBlockedMark[64][64]; // ���ĳ��Ԫ��ֵ����m_nCurMark���ͱ�ʾ���λ�ñ���ס�ˣ���������ʾ��
	static int m_nCurMark ; // ��ǰʹ�õı�־����
};
//------------------------------------------------------------------------
/*
CCircleDisffuse��

����	��		���ཫ64X64��Χ�ڴ����ĵ�ΪԲ�ĵ�����Χÿһ��Բ������
				�����һ�����������У�������������Ի��ÿһȦ�ϵĵ�����ꡣ

ʹ�÷�����
				1���������Լ������࣬�������麯��OnBeginNewCircl��OnFoundTile
				2��ʹ����������Create�趨������ͼ�ķ�Χ�����Զ���趨��
				3��ʹ��BeginDiffuse��ʼ��ɢ��û��ɢ���µ�һȦ����������OnBeginNewCircle����
				ÿ�����µ�һ�㣨��TILE������������OnFoundTile����

ʾ�����룺

  		CMyEllipsDiffuse myEllips() ; // CMyEllipsDiffuse������CCircleDiffuse
		myEllips.Create( 9999 , 9999 , 35 , 17 ,33) ; // ��ͼ�ߴ�Ϊ9999X9999(ע�⣺�ò�����ζ�ţ���CMyEllipsDiffuse���캯���е�����Create��BeginCircleDiffuse����
*/
class CCircleDiffuse : public CDiffuseBase
{
public:
	// ��ʼԲ����ɢ
	BOOL BeginCircleDiffuse(	int nX , int nY ,	// ����nX,nY��ʾ��ͼ��������ɢ��ԭ�����꣨������ͼ�Ĵ�С�Ѿ���Create�������룩
								int nRadius		) ; // ��Բ��ɢ�����뾶

	CCircleDiffuse(	int nWidth	, int nHeight	) ;
	virtual ~CCircleDiffuse() ;

private:
	virtual BOOL IsNeedCheckEdge() ; // ���ǻ����麯��
	virtual BOOL IsDiffuseComplete( int nCircleIndex ) ;

private:
	int m_nMaxRadius ; // ��ǰ�뾶
};
//------------------------------------------------------------------------
/*
	��CFanShapedDiffuse
	������ɢ���÷��������CCircleDiffuse����
*/
class CFanShapedDiffuse : public CDiffuseBase
{
public:
	// ��ʼ������ɢ��nStarDegree��nEndDegreeȡֵΪ0~360
	// ��ʼ�ǶȰ���ʱ����ת�������Ƕ�Ϊ�����ε�����
	BOOL BeginFanShapedDiffuse(	int nX			, int nY		,
								double dStartDegree , double dEndDegree ,
								int nMaxRadius ) ; 
	
	CFanShapedDiffuse(	int nWidth	, int nHeight );
	virtual ~CFanShapedDiffuse(){} ;

protected:
	virtual BOOL IsNeedCheckEdge() ; // ���ǻ����麯��
	virtual BOOL IsDiffuseComplete( int nCircleIndex ) ;

private:
	void GetEdgePointFromAngle( double dAngle , int &nX , int &nY ) ;
protected:
	int m_nMaxRadius ;
};
//------------------------------------------------------------------------