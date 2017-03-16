/**
*	created:		2012-7-2   1:14
*	filename: 		AniGroup
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "Ani.h"
#include <map>
#include "../../FKGame/Common/CreatureViewDef.h"
#include "AniGroupLoader.h"
//------------------------------------------------------------------------
//���������ļ��ṹͼ
/*--------------------------------------------------------------------------
�ļ�ͷ			�����ļ��汾��
��Ϣ			�����ļ����ж��ٶ�����
���ж���������	�����ļ������еĶ���������
����֡������	�����ļ������е�֡��ָ���ж�����֡������һ������
����			������
--------------------------------------------------------------------------*/
enum PalType
{
	ptFrozen=0, // ����
	ptToxic,	// �ж�
	ptBerserk,	// ��
	ptType1,	// ����1
	ptType2,
	ptType3,
	ptType4,
	ptType5,

	ptMaxPalTypeCount,
};
//------------------------------------------------------------------------
class CMultiParticleMask;
class CParticle;
class CParticleMask;
//------------------------------------------------------------------------
class CAniGroup : public IPoolNodeObject // 40 bytes
{
public:
	typedef std::map<int,CMultiParticleMask*> MaskSet;
	typedef MaskSet::iterator mask_iterator;
	
public:
	enLoadState					m_LoadState[CrtStateMax];
	DWORD						m_dwResID;

	IBitmap**					m_ppaState;			// ָ��ÿ��״̬��ͼ������
	IBitmap**					m_ppaIBitmap;		// ֡ͼ������
	BYTE*						m_pPal;				// ���õ�ɫ��
	stGroupInfoHeader			m_GroupInfoHdr;		// ����Ϣͷ
	stAniIndex*					m_paAniIndex;		// ��������
	stFrameIndex*				m_paFrameIndex;		// ����֡����
	IFileObject*				m_pFile;			// �ļ�����
	DWORD						m_nDataOffset;		// ����ƫ��
	BYTE*						m_pUserPal[ptMaxPalTypeCount]; // ���ﲻnew��ֱ��ָ������ƽű�����ĵ�ַ
	int							m_nMaxUserPalCount;
	MaskSet						m_setMask;
	DWORD						m_dwSize;
public:
	CAniGroup();
	virtual ~CAniGroup();
	void Draw(ICanvas *pCanvas, int nX, int nY, int nAni, int nFrame, int nFlag, BYTE nAlpha = 0, BYTE* pPal=NULL, CParticle * pParticle=NULL);
	bool Open(IFileSystem* pFileSystem, LPCSTR szName, DWORD dwResID);

	// ����������
	int GetAniCount(){return m_GroupInfoHdr.total_ani;}
	int GetTotalFrames(){return m_GroupInfoHdr.total_frame;}

	// ��������
	int	GetTotalFrames(int nAni);

	// ֡����
	void GetOffset(int nAni, int nFrame, int& nOffsetX, int& nOffsetY);
	int GetWidth(int nAni, int nFrame);
	int GetHeight(int nAni, int nFrame);
	IBitmap* GetBitmap(int nAni, int nFrame);

	// ��ɫ�����
	BYTE* GetPal()							{return m_pPal;}
	int GetUseColor()						{return m_GroupInfoHdr.use_color;}
	BYTE* GetUserPal(int nIndex)			{return m_pUserPal[nIndex];}
	BYTE** GetUserPal()						{return m_pUserPal;}
	void SetMaxUserPalCount(int nCount)		{m_nMaxUserPalCount=nCount;}
	int  GetMaxUserPalCount()				{return m_nMaxUserPalCount;}

	virtual DWORD GetSize(){return m_dwSize;}
	virtual void Release(){delete this;}
	
	BOOL GetStateData(int nState, BYTE* pBuffer, DWORD dwBufferSize);
	DWORD GetStateSize(int nState);
	IBitmap* GetStateBitmap(int nState);
	CMultiParticleMask * GetParticleMask(CParticle * pParticle,int cFrame,BYTE * pPal);
};
//------------------------------------------------------------------------
// ����������
struct SAniContext
{
	//BYTE nDelay;		// ֡��ʱ�����ٶ�
	int nType;			// �������ͣ�����2����
	int nFrames;		// ֡��,������255֡
	int nCurFrame;		// ��ǰ�ǵڼ�֡
	int nState;			// ��ǰ״̬
	int nDir;			// ��ǰ����

	SAniContext()			{Clear();}
	void Clear()			{memset(this,0,sizeof(SAniContext));}
	void NextFrame()		{if(++nCurFrame>=nFrames)nCurFrame=0;}
	bool IsPlayOver()		{return nCurFrame > nFrames;}
	int  State()			{return (int)nState;}
	int  Dir()				{return (int)nDir;}
	int  CurFrame()			{return (int)nCurFrame;}
	int  Frames()			{return (int)nFrames;}
	bool IsState(int state) {return (int)nState==state;}
	bool IsDir(int dir)		{return (int)nDir==dir;}
	bool Equal(SAniContext *pAniContext)
	{
		if(nCurFrame != pAniContext->nCurFrame
		|| nDir != pAniContext->nDir
		|| nState != pAniContext->nState
		|| nType != pAniContext->nType)
			return FALSE;
		return TRUE;
	}
	void SetData(SAniContext *pAniContext)
	{
		nType		= pAniContext->nType;
		nFrames		= pAniContext->nFrames;
		nCurFrame	= pAniContext->nCurFrame;
		nState		= pAniContext->nState;
		nDir		= pAniContext->nDir;
	}
};
//------------------------------------------------------------------------
#define cur_ani ((m_AniContext.nState<<3)+m_AniContext.nDir)
//------------------------------------------------------------------------	