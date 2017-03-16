/**
*	created:		2012-7-2   3:57
*	filename: 		ActorPreView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��ɫԤ��ʾ��
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "AniGroup.h"
#include "Resloader.h"
#include "../../FKGame/Timer/ITimerAxis.h"
#include "../../FKGame/Common/CreatureViewDef.h"
#include "Pal.h"
//------------------------------------------------------------------------
class CActPreView : public ITimerSink
{
public:
	CActPreView();
	virtual ~CActPreView();
	
	//����ͼ��
	BOOL ChangeModule (int nIndex, DWORD dwResID, BYTE nPal, BYTE nAlpha);
	CAniGroup* RequestRes(int nModuleType);
	void OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag);
	virtual void OnTimer(DWORD dwEventID);
	void CreatePreView (int nType)
	{
		m_AniContext.nType = nType;
		m_nDelay = g_ResLoader.GetFrameDelay(nType, m_AniContext.nState);
		if (m_nDelay > 0)
		{
			g_pTimeAxis->SetTimer(0,
				static_cast<ITimerSink*>(this),
				m_nDelay,
				"BasicSys!!CActPreView::CreatePreView");
		}
	}
	void SetState (int nState);

	//���ﱾ����غ���������״̬��������֡�����
	int  GetCurAni(int nFind);			//���ص�ǰ�ǵڼ����������������ڼ�������

	void ReleasePreView();
public:
	//ͼ�����
	CAniGroup*			m_pModule[CrtPartMax];				//���ͼ��
	DWORD				m_dwModuleID[CrtPartMax];			//���ͼ��ID
	BYTE				m_bytePal[CrtPartMax];				//����ĵ�ɫ��������
	BYTE				m_byteAlpha[CrtPartMax];			//�����͸����������
	CPal				m_Pal[CrtPartMax];					//����ĵ�ɫ��
	DWORD				m_dwDrawFlag;
	SAniContext			m_AniContext;						// ����������
	BYTE*				m_PartStateFlag[CrtPartMax];		//�������������״̬��־
	int					m_nDelay;							//������ʱ
};
//------------------------------------------------------------------------