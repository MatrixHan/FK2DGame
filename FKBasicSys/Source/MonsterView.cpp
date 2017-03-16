/**
*	created:		2012-7-2   3:39
*	filename: 		MonsterView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/BasicCommon.h"
#include "../Include/MonsterView.h"
#include "../Include/ResLoader.h"
#include "../Include/Pal.h"
#include "../Include/PalMgr.h"
#include "../Include/SkillCreate.h"
//------------------------------------------------------------------------
CMonsterView::CMonsterView() : CMstFSM()
{
	ClearAll();
	CMstFSM::Create(this);
}
//------------------------------------------------------------------------
CMonsterView::~CMonsterView()
{
	Close();
	if (m_pCurrentState)
	{
		m_pCurrentState->End();
		m_pCurrentState = NULL;
	}
}
//------------------------------------------------------------------------
void CMonsterView::ClearAll()
{
	m_nFallowStateID = 0;
	m_nCorperShowTime = 0;
	m_nShape = 0;
	
	m_pHeadAttachObj = NULL;
	m_nNpcType = NpcTypeNone;
	m_nTaskType = TaskTypeNone;
}
//------------------------------------------------------------------------
void CMonsterView::Close()
{
	if(m_pHeadAttachObj)
	{
		OnAttachObjDestroy(m_pHeadAttachObj);
		m_pHeadAttachObj = NULL;
	}
	for (int i = 0; i < CrtPartMax; i++)
	{
		if (m_pModule[i])
		{
			g_ResLoader.ReleaseRes(ioCreature + m_dwModuleID[i]);
			m_pModule[i] = NULL;
			m_dwModuleID[i] = 0;
		}
	}
}
//------------------------------------------------------------------------
void CMonsterView::OnTimer(DWORD dwEventID)
{
	switch (dwEventID)
	{
	case TIME_EVENT_ID_FRAME:		// ����ʱ��
	case TIME_EVENT_ID_SHORT_STEPS:	// �鲽
		CMstFSM::OnTimer(dwEventID);
		break;

	case TIME_EVENT_ID_DIE:			// ʬ����ʧʱ��
		g_pTimeAxis->KillTimer(TIME_EVENT_ID_DIE, static_cast<ITimerSink*>(this));

		g_pTimeAxis->SetTimer(TIME_EVENT_ID_DIE_ANI,
			static_cast<ITimerSink*>(this),
			TIME_DEFAULT_CORPER_ANI,
			"BasicSys!!CMonsterView::OnTimer");

		m_nCorperShowTime = 20;
		break;
		
	case TIME_EVENT_ID_DIE_ANI:	// ��ʧ����ʱ��
		if(--m_nCorperShowTime == 0)
		{
			g_pTimeAxis->KillTimer(TIME_EVENT_ID_DIE_ANI, static_cast<ITimerSink*>(this));
			g_pMapView->RemoveItem(m_ptTile, this);
			delete this;
			return;
		}
		break;
	}
}
//------------------------------------------------------------------------
// ��ȡ�������ID
DWORD CMonsterView::GetClassID() const
{
	return miNpcObj;
}
//------------------------------------------------------------------------
// ������ȡ���ݣ����ڴ��ļ��д�������
BOOL CMonsterView::OnSerialize(BOOL bWrite,IArchive* pArchive)
{
	if (!pArchive) return FALSE;
	if(bWrite)
	{
		SMonsterViewArchive a;
		a.dwResID = m_dwResID;
		a.nType = m_AniContext.nType;

		pArchive->Write((char*)&a, sizeof(a));
		return TRUE;
	}
	else
	{
		Close();
		if (!CreateItem((SMonsterViewArchive*)pArchive->GetBuffer()))
		{
			TraceLn("ERROR: CMonsterView::OnSerialize(bRead), create item failed!");
		}
		pArchive->Seek(sizeof(SMonsterViewArchive));
		return TRUE;
	}
}
//------------------------------------------------------------------------
// ���������ֳ���������
BOOL CMonsterView::OnCreateItem(const char* buf, int nLen)
{
	return CreateItem((SMonsterViewArchive*)buf);
}
//------------------------------------------------------------------------
// ����(�磺���ؽű����ݵ�)
void CMonsterView::OnUpdate()
{
	SMonsterViewCreateParam mvcp;
	memset(&mvcp, 0, sizeof(mvcp));
	g_ResLoader.GetMonsterCreateParam(m_dwCrtID, mvcp);
	m_AniContext.nType = mvcp.nType;
	g_ResLoader.GetAnchor(m_AniContext.nType, m_ptAnchor);
	int nDelayBuf[CrtStateMax];
	int nAttackFrameBuf[CrtStateMax];
	int nAttackSoundFrameBuf[CrtStateMax];
	memset (nDelayBuf, 0, sizeof(int) * CrtStateMax);
	memset (nAttackFrameBuf, 0, sizeof(int) * CrtStateMax);
	memset (nAttackSoundFrameBuf, 0, sizeof(int) * CrtStateMax);
	for (int i = 0; i < CrtStateMax; i++)
	{
		nDelayBuf[i] = g_ResLoader.GetFrameDelay(m_AniContext.nType, i);
		nAttackFrameBuf[i] = g_ResLoader.GetAttackFrame(m_AniContext.nType, i);
		nAttackSoundFrameBuf[i] = g_ResLoader.GetAttackSoundFrame(m_AniContext.nType, i);
	}
	CMstFSM::SetAllStateDelay(nDelayBuf);
	CMstFSM::SetAllStateAttackFrame(nAttackFrameBuf);
	CMstFSM::SetAllStateAttackSoundFrame(nAttackSoundFrameBuf);

	int nDelay[3];
	nDelay[0] = g_ResLoader.GetWalkDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
	nDelay[1] = g_ResLoader.GetRunDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
	nDelay[2] = g_ResLoader.GetRideDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
	CMstFSM::SetMoveDelay(nDelay);
	
	m_pCurrentState->Start();
	
	m_fStepWalk = g_ResLoader.GetStep(m_AniContext.nType, CrtStateWalk);
	m_fStepRun = g_ResLoader.GetStep(m_AniContext.nType, CrtStateRun);
	m_fStepRide = g_ResLoader.GetStep(m_AniContext.nType, CrtStateRideRun);

	//���ù������������
	if (!g_ResLoader.GetMonsterbuf(m_dwCrtID, m_dwModuleID, m_byteAlpha, m_bytePal))
	{
		PrintEventLn("ERROR: CMonsterView-OnUpdata-SetModulePro");
	}
	
	// �ٶȵȼ�֧��	
	SetStepWalk(m_fStepWalk);
	SetStepRun(m_fStepRun);	
	SetStepRide(m_fStepRide);
	
	// ƫ��λ�����
	mvcp.nHalfHeightOffset = g_ResLoader.GetHalfHeigh(m_AniContext.nType);
	mvcp.nHeightOffset = g_ResLoader.GetWholeHeigh(m_AniContext.nType);
	SetHalfHeightOffset(mvcp.nHalfHeightOffset);
	SetHeightOffset(mvcp.nHeightOffset);
	
	m_nBeHitLoopBeginFrame = g_ResLoader.GetBeHitLoopBegin(m_AniContext.nType);
	m_nBeHitLoopEndFrame = g_ResLoader.GetBeHitLoopEnd(m_AniContext.nType);

	memcpy (m_nActStateCommonAttack, mvcp.nActStateCommonAttack, sizeof(m_nActStateCommonAttack));
	m_nCommonAttackStyleNum = 0;
	for(int j = 0; j < 3; j++)
	{
		if(m_nActStateCommonAttack[j] >= 0)
			m_nCommonAttackStyleNum++;
	}
	m_nActStateJuqi = mvcp.nActStateJuqi;
	m_nActStateSkillAttack = mvcp.nActStateSkillAttack;
	m_nCommonAttackSoundID = mvcp.nSoundIDCommonAttack;
	
	m_nSoundIDFallow = mvcp.nSoundIDFallow;
	m_nSoundIDBeHit = mvcp.nSoundIDBeHit;
	m_nSoundIDDie = mvcp.nSoundIDDie;
	
	m_nFallowStateID = g_ResLoader.GetFallowStateID(m_AniContext.nType);

	IniTotalFrame();
	ClearShowRect();
}
//------------------------------------------------------------------------
//��ȡ����վλ
DWORD CMonsterView::GetOccupantValue()
{
	if (State() == CrtStateDie || !m_bOccupant)
		return 0x0;
	return CMapItem::GetOccupantValue();
}
//------------------------------------------------------------------------
// ��ȡռλ���б�Ĭ��Ϊ��ռλλ�б�
COccupantTileList* CMonsterView::GetOccupantTileList() const
{
	return m_MstOccInfo.GetOccupantTileList();
}
//------------------------------------------------------------------------
// ��ȡ�����ڵ�ͼ�е���ʾ��
DWORD CMonsterView::GetLayerValue()
{
	if (m_AniContext.IsState(CrtStateDie))
		return milvLowest-1;
	return milvMidle;
}
//------------------------------------------------------------------------
// ����
void CMonsterView::SetDirty(ICanvas* pCanvas, const POINT& ptViewTop)
{
	if(!pCanvas)
		return;

// �ȸ�ɾ����������ʱ���������������
	while(!m_AttachDirtyList.empty())
	{
		RECT rc;
		rc = m_AttachDirtyList.front();
		OffsetRect(&rc,
			-ptViewTop.x,
			-ptViewTop.y);
		pCanvas->AddDirtyRect(rc);
		m_AttachDirtyList.pop_front();
	}

	if(!m_bVisible)
	{
		if(m_bNeedSetOldDirty)
		{
			list<RECT> DirtyRectList;
			GetOldDirtyRect(DirtyRectList);

			RECT rc;
			while(!DirtyRectList.empty())
			{
				rc = DirtyRectList.front();
				OffsetRect(&rc,
					-ptViewTop.x,
					-ptViewTop.y);
				pCanvas->AddDirtyRect(rc);
				DirtyRectList.pop_front();
			}
		}
		return;
	}

	POINT ptNew;
	RECT rcAreaNew;
	Tile_To_World(m_ptTile, ptNew);

// �ȸ���������
	
	POINT ptAttachOffset;
	ptAttachOffset.x = m_ptTileOffset.x + ptNew.x;
	ptAttachOffset.y = m_ptTileOffset.y + ptNew.y;
	vector<IAttachObj*>::iterator it;
	for (it = m_vAttachObjs.begin(); it != m_vAttachObjs.end(); ++it)
	{
		for (int ni = 0; ni < (*it)->GetLoopNum(); ++ni)
		{
			if ((*it)->GetDrawOrder(ni) != IAttachObj::draw_order_topmost)
			{
				(*it)->SetDirty(pCanvas, ptViewTop, ptAttachOffset, ni);
			}
		}
	}
// �ٸ�����
	rcAreaNew = GetShowRect();
	OffsetRect(&rcAreaNew,
		ptNew.x,
		ptNew.y);

	if(m_bNeedSetOldDirty)
	{
		// ������ǻ���ͬһ��
		if(m_AniContextOld.Equal(&m_AniContext))
		{
			if(EqualRect(&rcAreaNew, &m_rcAreaOld))	// ������ϴλ���λ��һ�����ô�����						
				return;
		}
		else
		{
			m_AniContextOld = m_AniContext;
		}

		// ������Ĳ���ͬһ������ϴκ���εľ�������
		// �Ȱ��ϴξɵĻ�ͼ��������
		OffsetRect(&m_rcAreaOld,
			-ptViewTop.x,
			-ptViewTop.y);
		pCanvas->AddDirtyRect(m_rcAreaOld);
	}

	// ������ε�����
	m_rcAreaOld = rcAreaNew;

	// �ٰ��µ�Ҫ��ͼ��������
	OffsetRect(&rcAreaNew,
		-ptViewTop.x,
		-ptViewTop.y);
	pCanvas->AddDirtyRect(rcAreaNew);
// �ٸ�������� end------------------------------------------------------
	
	// ��ʾ�����������ˣ��´�Ҫ�������ĵط�����
	m_bNeedSetOldDirty = TRUE;

}
//------------------------------------------------------------------------
// ��ȡ�ɵ��������Ҫ���ڰѶ���ӵ�ͼ��ɾ��ʱ��ȡ���������
void CMonsterView::GetOldDirtyRect(list<RECT> &DirtyRectList)
{
	if(!m_bNeedSetOldDirty)
		return;

	m_bNeedSetOldDirty = FALSE;

	// �����
	DirtyRectList.push_back(m_rcAreaOld);

	// ��������Ĳ�����
	while(!m_AttachDirtyList.empty())
	{
		DirtyRectList.push_back(m_AttachDirtyList.front());
		m_AttachDirtyList.pop_front();
	}
	vector<IAttachObj*>::iterator it;
	for (it = m_vAttachObjs.begin(); it != m_vAttachObjs.end(); ++it)
	{
		for (int ni = 0; ni < (*it)->GetLoopNum(); ++ni)
		{
			if ((*it)->GetDrawOrder(ni) != IAttachObj::draw_order_topmost)
			{
				(*it)->GetOldDirtyRect(DirtyRectList, ni);
			}
		}
	}
}
//------------------------------------------------------------------------
// ��ͼѭ�����������Ƶ�ָ����ICanvas��
void CMonsterView::OnDraw(ICanvas* pCanvas,const POINT& ptTileCenter, DWORD dwFlag)
{
	if(!pCanvas || !m_bVisible)
		return;
	
	// ����Ч(draw order = draw_order_prev)
	vector<IAttachObj*>::iterator it;
	POINT pt;
	pt.x = m_ptTileOffset.x + ptTileCenter.x;
	pt.y = m_ptTileOffset.y + ptTileCenter.y;
	
	for (it=m_vAttachObjs.begin(); it!=m_vAttachObjs.end(); ++it)
		for (int ni = 0; ni < (*it)->GetLoopNum(); ++ni)
			if ((*it)->GetDrawOrder(ni) == IAttachObj::draw_order_prev)
				(*it)->OnDraw(pCanvas, pt, dwFlag, ni);

	int nOldState = m_AniContext.nState;
	// �������û���ܲ������Ǿ�����·�������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
	if(GetState() == CrtStateRun && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateRun)))
		m_AniContext.nState = CrtStateWalk;

	// �������û����·�����Ǿ���վ���������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
	if(GetState() == CrtStateWalk && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateWalk)))
		m_AniContext.nState = CrtStateStand;

	// ���ж�������ָ����һ����
	if(GetState() == CrtStateFallow)
		m_AniContext.nState = m_nFallowStateID;

	int nCurFrame = m_AniContext.nCurFrame;
	if (m_nCurPalType == PalType_Stone)
	{
		nCurFrame = 0;
	}
	// ����ɫ
	int offx, offy;
	int nModuleType, nCurAni;
	CAniGroup* pCurObj;
	BYTE nDrawOrder[CrtPartMax/2];
	memset (nDrawOrder, 0, sizeof(BYTE) * CrtPartMax/2);
	int nCount = g_ResLoader.GetFrameSort(nDrawOrder, m_AniContext);
	for (int i = 0; i < nCount; i++)
	{
		nModuleType = nDrawOrder[i];
		if(nModuleType < 0 || nModuleType > CrtPartMax - 1)
			continue;
		pCurObj = RequestRes(nModuleType);
	
		if (pCurObj)
		{
			nCurAni = GetCurAni(nModuleType);
			if (nCurAni == -1)
				continue;
			pCurObj->GetOffset(nCurAni, nCurFrame, offx, offy);
			if (m_byteAlpha[nModuleType] == 0 || m_byteAlphaAll != 0)
			{
				if(m_nCorperShowTime > 0)
				{
					pCurObj->Draw(pCanvas, 
						m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
						m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
						nCurAni, nCurFrame, dwFlag,
						m_nCorperShowTime * 12, m_Pal[nModuleType].GetPal(m_nCurPalType), m_pParticle[nModuleType]);
				}
				else
				{
					pCurObj->Draw(pCanvas, 
						m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
						m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
						nCurAni, nCurFrame, dwFlag,
						m_byteAlphaAll, m_Pal[nModuleType].GetPal(m_nCurPalType), m_pParticle[nModuleType]);
				}
			}
			else
			{
				if(m_nCorperShowTime > 0)
				{
					pCurObj->Draw(pCanvas, 
						m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
						m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
						nCurAni, nCurFrame, dwFlag,
						m_nCorperShowTime * 25, m_Pal[nModuleType].GetPal(m_nCurPalType), m_pParticle[nModuleType]);
				}
				else
				{
					pCurObj->Draw(pCanvas, 
						m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
						m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
						nCurAni, nCurFrame, dwFlag,
						m_byteAlpha[nModuleType], m_Pal[nModuleType].GetPal(m_nCurPalType), m_pParticle[nModuleType]);
				}
			}
		}
		// ����Ч
		nModuleType++;
		pCurObj = RequestRes(nModuleType);
		if (pCurObj)
		{
			nCurAni = GetCurAni(nModuleType);
			if (nCurAni == -1)
				continue;
			pCurObj->GetOffset(nCurAni, nCurFrame, offx, offy);
			if (m_byteAlpha[nModuleType] == 0 || m_byteAlphaAll != 0)
			{
				if(m_nCorperShowTime > 0)
				{
					pCurObj->Draw(pCanvas, 
						m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
						m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
						nCurAni, nCurFrame, dwFlag,
						m_nCorperShowTime * 12, m_Pal[nModuleType].GetPal(m_nCurPalType));
				}
				else
				{
					pCurObj->Draw(pCanvas, 
						m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
						m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
						nCurAni, nCurFrame, dwFlag,
						m_byteAlphaAll, m_Pal[nModuleType].GetPal(m_nCurPalType));
				}
			}
			else
			{
				if(m_nCorperShowTime > 0)
				{
					pCurObj->Draw(pCanvas, 
						m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
						m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
						nCurAni, nCurFrame, dwFlag,
						m_nCorperShowTime * 25, m_Pal[nModuleType].GetPal(m_nCurPalType));
				}
				else
				{
					pCurObj->Draw(pCanvas, 
						m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
						m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
						nCurAni, nCurFrame, dwFlag,
						m_byteAlpha[nModuleType], m_Pal[nModuleType].GetPal(m_nCurPalType));
				}
			}
		}
	}

	m_AniContext.nState = nOldState;

	// ����Ч(draw order = draw_order_after)
	for (it=m_vAttachObjs.begin(); it!=m_vAttachObjs.end(); ++it)
		for (int ni = 0; ni < (*it)->GetLoopNum(); ++ni)
			if ((*it)->GetDrawOrder(ni) == IAttachObj::draw_order_after)
				(*it)->OnDraw(pCanvas, pt, dwFlag, ni);
};
//------------------------------------------------------------------------
// ��������һЩ���������Ѫ�ۡ��������ݡ���Ʒ���ȣ���Щ������Զ�������ϲ�
void CMonsterView::OnDrawTopmostObject(ICanvas* pCanvas,const POINT& ptTileCenter, DWORD dwFlag)
{
	vector<IAttachObj*>::iterator it;
	vector<IAttachObj*>::iterator end;
	POINT pt;
	pt.x = m_ptTileOffset.x + ptTileCenter.x;
	pt.y = m_ptTileOffset.y + ptTileCenter.y;

	end = m_vFlowObjs.end();
	for (it=m_vFlowObjs.begin(); it!=end; ++it)
		(*it)->OnDraw(pCanvas, pt, BLIT_MASK);

	if(!m_bVisible)
		return;

	end = m_vAttachObjs.end();
	for (it=m_vAttachObjs.begin(); it!=end; ++it)
		if ((*it)->GetDrawOrder() == IAttachObj::draw_order_topmost)
			(*it)->OnDraw(pCanvas, pt, BLIT_MASK);

	DWORD dwFlags = CMapItemView::GetFlag();
	POINT ptOffset = ptTileCenter;
	ptOffset.x += m_ptTileOffset.x;
	ptOffset.y += m_ptTileOffset.y + GetHeightOffset();
	
#ifndef _USE
	if (m_pPaoPao != NULL && State() != CrtStateDie)
	{
		m_pPaoPao->OnDraw(pCanvas, ptOffset, m_szUserName, m_clrName, g_pFont);
	}
	else
	{
		if (m_AniContext.IsState(CrtStateDie))
			return;
		if (!IsSelected())
		{
			DrawName(pCanvas, ptOffset);
		}
		else
		{
			DrawSelectedName(pCanvas, ptOffset);
		}
	}
#endif
/*
#ifdef _USE
	DrawName(pCanvas, ptOffset);
	if (IsSelected())
	{
		WPixel clr = WPixel(255,0,0);
		WPixel clrText = WPixel(255,255,255);
		char buf[MAX_PATH];
		POINT ptTile;
		g_pMapView->ScreenToTile(ptTileCenter, ptTile);
		
		if (dwFlags & flagInfo)
		{
			IResObject* pObj = g_ResLoader.m_pScpCreature->GetResObject(m_dwResID);
			if (pObj)
			{
				IResObject* pParentObj = pObj->GetParentObject();
				wsprintf(buf, "%s\\%s,(%d,%d),CID=%d,RID=%d", 
					pParentObj->GetObjectName(), pObj->GetObjectName(),
					ptTile.x, ptTile.y, GetClassID(), m_dwResID);
			}
			else
				wsprintf(buf, "(%d,%d),CID=%d,RID=%d", ptTile.x, ptTile.y, GetClassID(), m_dwResID);
			
			int nW = g_pFont->GetLength(buf);
			int nH = g_pFont->GetHeight();
			RECT rcTip;
			int offx, offy;
			if (m_pModule[CrtPartHead])					//ͷ
				m_pModule[CrtPartHead]->GetOffset(GetCurAni(CrtPartHead), GetCurFrame(), offx, offy);
			else if (m_pModule[CrtPartUpBody])			//�ϰ���
				m_pModule[CrtPartUpBody]->GetOffset(GetCurAni(CrtPartUpBody), GetCurFrame(), offx, offy);
			else if (m_pModule[CrtPartDownBody])		//�°���
				m_pModule[CrtPartDownBody]->GetOffset(GetCurAni(CrtPartDownBody), GetCurFrame(), offx, offy);
			else if (m_pModule[CrtPartBody])			//����
				m_pModule[CrtPartBody]->GetOffset(GetCurAni(CrtPartBody), GetCurFrame(), offx, offy);
			else if (m_pModule[CrtPartWholeBody])		//ȫ����
				m_pModule[CrtPartWholeBody]->GetOffset(GetCurAni(CrtPartWholeBody), GetCurFrame(), offx, offy);
			rcTip.left = ptTileCenter.x - m_ptAnchor.x - 1 + offx;
			rcTip.right = rcTip.left + nW + 2;
			rcTip.top = ptTileCenter.y - m_ptAnchor.y - nH - 2 + offy;
			rcTip.bottom = rcTip.top + nH + 1;
			pCanvas->DrawBox(rcTip, clr);
			pCanvas->DrawText(g_pFont, rcTip.left+1, rcTip.top+1, buf, clrText, FONT_NOALPHA);
		}
		
		// draw select rectangle
		if (dwFlags & flagRectangle)
		{
			RECT rc = GetShowRect();
			OffsetRect(&rc, ptTileCenter.x, ptTileCenter.y);
			InflateRect(&rc, 1, 1);
			POINT ptFrom,ptTo;
			ptFrom.x=rc.left, ptFrom.y=rc.top, ptTo.x=rc.right-1, ptTo.y=rc.top;
			pCanvas->DrawLine(ptFrom, ptTo, clr); // �Ϻ�
			ptFrom.x=rc.right-1, ptFrom.y=rc.top, ptTo.x=rc.right-1, ptTo.y=rc.bottom;
			pCanvas->DrawLine(ptFrom, ptTo, clr); // ����
			ptFrom.x=rc.right-1, ptFrom.y=rc.bottom-1, ptTo.x=rc.left, ptTo.y=rc.bottom-1;
			pCanvas->DrawLine(ptFrom, ptTo, clr); // �º�
			ptFrom.x=rc.left, ptFrom.y=rc.bottom-1, ptTo.x=rc.left, ptTo.y=rc.top;
			pCanvas->DrawLine(ptFrom, ptTo, clr); // ����
		}
	}
#else
*/
	if (m_AniContext.IsState(CrtStateDie))
		return;

	//if (dwFlags & flagBloodSlot)
	{
		RECT rc;
		rc.left = ptOffset.x - BLOOD_SLOT_WIDTH/2;
		rc.right = rc.left + BLOOD_SLOT_WIDTH + 1;
		rc.top = ptOffset.y;
		rc.bottom = rc.top + BLOOD_SLOT_HEIGHT + 1;
		DrawBloodSlot(pCanvas, rc, m_nBloodVol);
	}
	/*
#endif
	*/
}
//------------------------------------------------------------------------
// ��������
// dwParam1: 0��ʾ�����������ж�����1��ʾ�Ѿ��������޶���
// dwParam2: ?
int CMonsterView::cmdDie(DWORD dwParam1, DWORD dwParam2)
{
	m_bDeaded = TRUE;
	
	if (IsMoving())
		StopMove();

	g_pMapView->RemoveItem(m_ptTile, this);
	SetState(CrtStateDie);
	g_pMapView->AddItem(m_ptTile, this);

	if (dwParam1 == 1)
	{
		m_AniContext.nCurFrame = - 1;
	}

	// �趨ʬ�屣��ʱ��
	g_pTimeAxis->SetTimer(TIME_EVENT_ID_DIE,
		static_cast<ITimerSink*>(this),
		TIME_DEFAULT_CORPER,
		"BasicSys!!CMonsterView::cmdDie");

	return TRUE;
}
//------------------------------------------------------------------------
// ��ɫ����(����������Բ���һ�γ�������)
// dwParam1: ?
// dwParam2: ?
int CMonsterView::cmdBorn(DWORD dwParam1, DWORD dwParam2)
{
	m_bDeaded = FALSE;
	
	SSkillCreateParam scp;
	scp.nAniType = anitypeGenericAni;
	scp.nIndex = aniBorn;
	scp.ptTile = m_ptTile;
	scp.ptAttack = m_ptTile;

	return CSkillCreate::CreateGenericMagic(&scp);
}
//------------------------------------------------------------------------
// ����NPC����Ӧ��ͷ����Ч
// dwParam1: NPC����Ӧ��ͷ����Ч �ο�enNpcType
// dwParam2: ?
int CMonsterView::cmdSetNpcType(DWORD dwParam1, DWORD dwParam2)
{
	m_nNpcType = dwParam1;
	if(m_nTaskType != TaskTypeNone)
		return TRUE;

	if(m_pHeadAttachObj)
	{
		OnAttachObjDestroy(m_pHeadAttachObj);
		m_pHeadAttachObj = NULL;
	}

	if(m_nNpcType != NpcTypeNone)
	{
		IResObject* pScpSkill = (IResObject*)CSkillCreate::getSkillScpObj(anitypeHeadAni, aniNpcTypeTrans + m_nNpcType - 1);
		if (!pScpSkill)
			return FALSE;
		
		DWORD dwMagicID;
		pScpSkill->GetPropertyLong("MagicID", (long&)dwMagicID, 0);	

		if (dwMagicID > 0)
		{
			SAttachStateObjBuffer st;
			memset(&st, 0, sizeof(st));
			st.dwResID = dwMagicID;

			IResObject* pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(st.dwResID);
			if (pMagicObj)
			{
				g_ResLoader.ReadAttachMagic(pMagicObj, &st);
				
				st.dwAppearanceID = -1;
				
				m_pHeadAttachObj = CreateAttachObj(&st);
			}
		}
	}

	return TRUE;
}
//------------------------------------------------------------------------
// ����NPCͷ���Ĺ�Ч
// dwParam1: ����NPCͷ���Ĺ�Ч �ο�enTaskType
// dwParam2: ?
int CMonsterView::cmdSetTaskType(DWORD dwParam1, DWORD dwParam2)
{
	m_nTaskType = dwParam1;
	
	if(m_pHeadAttachObj)
	{
		OnAttachObjDestroy(m_pHeadAttachObj);
		m_pHeadAttachObj = NULL;
	}

	if(m_nTaskType != TaskTypeNone)
	{
		IResObject* pScpSkill = (IResObject*)CSkillCreate::getSkillScpObj(anitypeHeadAni, aniTaskType_Get_Can + m_nTaskType - 1);
		if (!pScpSkill)
			return FALSE;
		
		DWORD dwMagicID;
		pScpSkill->GetPropertyLong("MagicID", (long&)dwMagicID, 0);	

		if (dwMagicID > 0)
		{
			SAttachStateObjBuffer st;
			memset(&st, 0, sizeof(st));
			st.dwResID = dwMagicID;

			IResObject* pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(st.dwResID);
			if (pMagicObj)
			{
				g_ResLoader.ReadAttachMagic(pMagicObj, &st);
				
				st.dwAppearanceID = -1;
				
				m_pHeadAttachObj = CreateAttachObj(&st);
			}
		}
	}
	else if(m_nNpcType != NpcTypeNone)
	{
		IResObject* pScpSkill = (IResObject*)CSkillCreate::getSkillScpObj(anitypeHeadAni, aniNpcTypeTrans + m_nNpcType - 1);
		if (!pScpSkill)
			return FALSE;
		
		DWORD dwMagicID;
		pScpSkill->GetPropertyLong("MagicID", (long&)dwMagicID, 0);	

		if (dwMagicID > 0)
		{
			SAttachStateObjBuffer st;
			memset(&st, 0, sizeof(st));
			st.dwResID = dwMagicID;

			IResObject* pMagicObj = g_ResLoader.m_pScpMagic->GetResObject(st.dwResID);
			if (pMagicObj)
			{
				g_ResLoader.ReadAttachMagic(pMagicObj, &st);
				
				st.dwAppearanceID = -1;
				
				m_pHeadAttachObj = CreateAttachObj(&st);
			}
		}
	}

	return TRUE;
}
//------------------------------------------------------------------------
// ��������ĵ�ǰ״̬
void CMonsterView::SetState(int nState)
{
	if(nState < 0 || nState >= CrtStateMax)
		return;
	
	if(nState == CrtStateBehit && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateBehit)))
		nState = CrtStateStand;

	if(nState == CrtStateStand && GetMoveStyle() == ActMoveStyleRide)
		nState = CrtStateRideStand;

	if(m_pStates[nState] == NULL)
		return;

	if (CMstFSM::GetCurState() == m_pStates[nState])
	{
		m_pStates[nState]->DelAllAttribute();
		return;
	}

	// ������˲�������������
	if(IsDeaded() && nState != CrtStateDie)
		return;

	m_AniContext.nState = nState;

	// �������û���ܲ������Ǿ�����·�������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
	if(GetState() == CrtStateRun && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateRun)))
		m_AniContext.nState = CrtStateWalk;

	// �������û����·�����Ǿ���վ���������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
	if(GetState() == CrtStateWalk && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateWalk)))
		m_AniContext.nState = CrtStateStand;

	// ���ж�������ָ����һ����
	if(GetState() == CrtStateFallow)
		m_AniContext.nState = m_nFallowStateID;

	if(IsChangeShap())
	{
		// �������û�иö�����վ����
		if(!g_ResLoader.GetStateEnable(m_AniContext.nType, m_AniContext.nState))
		{
			m_AniContext.nState = CrtStateStand;
			nState = CrtStateStand;
		}
	}
	CMstFSM::GotoState(*m_pStates[nState]);	
	
	SetCurFrame(0);
	IniTotalFrame();
}
//------------------------------------------------------------------------
// ֹͣ��ǰ״̬
void CMonsterView::StopState(int nState)
{
	if(nState < 0 || nState >= CrtStateMax)
		return;
	
	if(m_pStates[nState] == NULL)
		return;

	m_pStates[nState]->Stop();
}
//------------------------------------------------------------------------
// ������ǰ״̬
void CMonsterView::RestartCurState()
{
	GetCurState()->End();
	GetCurState()->Start();
}
//------------------------------------------------------------------------
// ��ȡ��ͨ��������
int CMonsterView::GetNormalAttackActID()
{
	if(m_nCommonAttackStyleNum > 0)
	{
		if(m_nCommonAttackStyleNum == 1)
			return m_nActStateCommonAttack[0];
		else
		{
			return m_nActStateCommonAttack[rand() % m_nCommonAttackStyleNum];
		}
	}

	return 0;
}
//------------------------------------------------------------------------
// ��ȡĬ�ϵ���������ID���������Ч��
int CMonsterView::GetJuqiActID()
{
	return m_nActStateJuqi;
}
//------------------------------------------------------------------------
// ��ȡĬ�ϵļ��ܹ�������ID���������Ч��
int CMonsterView::GetSkillAttackActID()
{
	return m_nActStateSkillAttack;
}
//------------------------------------------------------------------------
// ���ö�����δ�С��Χ
void CMonsterView::SetShowRect()
{
	// �����ǰ�Ѿ������˾Ͳ���������
	if(m_ItemShowRect[m_AniContext.nState][m_AniContext.nDir][m_AniContext.nCurFrame].bIni)
		return;
	
	// ��Ϊ�Ѿ�����
	m_ItemShowRect[m_AniContext.nState][m_AniContext.nDir][m_AniContext.nCurFrame].bIni = TRUE;

	// û�и�״̬
	if(m_pStates[m_AniContext.nState] == NULL)
	{
		memset(&m_ItemShowRect[m_AniContext.nState][m_AniContext.nDir][m_AniContext.nCurFrame].rc, 0, sizeof(RECT));
		return;
	}

	int nOldState = m_AniContext.nState;

	// �������û���ܲ������Ǿ�����·�������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
	if(m_AniContext.nState == CrtStateRun && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateRun)))
		m_AniContext.nState = CrtStateWalk;

	// �������û����·�����Ǿ���վ���������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
	if(m_AniContext.nState == CrtStateWalk && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateWalk)))
		m_AniContext.nState = CrtStateStand;

	// ���ж�������ָ����һ����
	if(GetState() == CrtStateFallow)
		m_AniContext.nState = m_nFallowStateID;

	RECT rc;
	RECT rcResult = {800,600,-800,-600};

	BYTE nDrawOrder[CrtPartMax/2];
	int nCount = g_ResLoader.GetFrameSort(nDrawOrder, m_AniContext);

	int offx, offy;
	int nModuleType;
	int nCurAni;
	CAniGroup* pCurObj = NULL;
	for (int i = 0; i < nCount; i++)
	{
		nModuleType = nDrawOrder[i];
		pCurObj = RequestRes(nModuleType);
		if (pCurObj)
		{
			nCurAni = GetCurAni(nModuleType);
			if (nCurAni == -1)
				continue;

			pCurObj->GetOffset(nCurAni, m_AniContext.nCurFrame, offx, offy);
			if(offx == 0 && offy == 0)// ���������Ҫ�Ƿ�ֹ�ű��Ǳ߳��ֿ�Ⱥ͸߶ȶ�Ϊ1ƫ�ƶ�Ϊ0�Ķ��������ȷʵ���ڣ�ԭ��û��ȥ�飩
			{
				if(pCurObj->GetWidth(nCurAni, m_AniContext.nCurFrame) <= 1 && pCurObj->GetHeight(nCurAni, m_AniContext.nCurFrame) <= 1)
					continue;
			}

			rc.left = offx - m_ptAnchor.x;
			rc.top = offy - m_ptAnchor.y;
			rc.right = rc.left + pCurObj->GetWidth(nCurAni, GetCurFrame());
			rc.bottom = rc.top + pCurObj->GetHeight(nCurAni, GetCurFrame());
			if (rcResult.bottom < rc.bottom)
				rcResult.bottom = rc.bottom;
			if (rcResult.left > rc.left)
				rcResult.left = rc.left;
			if (rcResult.right < rc.right)
				rcResult.right = rc.right;
			if (rcResult.top > rc.top)
				rcResult.top = rc.top;
		}

		// ��Ч
		nModuleType++;
		pCurObj = RequestRes(nModuleType);
		if (pCurObj)
		{
			nCurAni = GetCurAni(nModuleType);
			if (nCurAni == -1)
				continue;

			pCurObj->GetOffset(nCurAni, m_AniContext.nCurFrame, offx, offy);
			if(offx == 0 && offy == 0)// ���������Ҫ�Ƿ�ֹ�ű��Ǳ߳��ֿ�Ⱥ͸߶ȶ�Ϊ1ƫ�ƶ�Ϊ0�Ķ��������ȷʵ���ڣ�ԭ��û��ȥ�飩
			{
				if(pCurObj->GetWidth(nCurAni, m_AniContext.nCurFrame) <= 1 && pCurObj->GetHeight(nCurAni, m_AniContext.nCurFrame) <= 1)
					continue;
			}

			rc.left = offx - m_ptAnchor.x;
			rc.top = offy - m_ptAnchor.y;
			rc.right = rc.left + pCurObj->GetWidth(nCurAni, GetCurFrame());
			rc.bottom = rc.top + pCurObj->GetHeight(nCurAni, GetCurFrame());
			if (rcResult.bottom < rc.bottom)
				rcResult.bottom = rc.bottom;
			if (rcResult.left > rc.left)
				rcResult.left = rc.left;
			if (rcResult.right < rc.right)
				rcResult.right = rc.right;
			if (rcResult.top > rc.top)
				rcResult.top = rc.top;
		}
	}
	m_ItemShowRect[nOldState][m_AniContext.nDir][m_AniContext.nCurFrame].rc = rcResult;

	m_AniContext.nState = nOldState;
}
//------------------------------------------------------------------------
// �����ƶ�������ʱ
void CMonsterView::SetAllMoveDelay(int* pDelay)
{
	SetMoveDelay(pDelay);
}
//------------------------------------------------------------------------
// �ص�(�����������ʱ�ص�)
void CMonsterView::OnAttachObjDestroy(IAttachObj* pAttachObj)
{
	vector<IAttachObj*>::iterator it;
	for (it = m_vAttachObjs.begin(); it != m_vAttachObjs.end(); it++)
	{
		if ((*it) == pAttachObj) // ����
		{
			// �Ѳ�������������²ŷ�����
			for (int ni = 0; ni < pAttachObj->GetLoopNum(); ++ni)
			{
				if (pAttachObj->GetDrawOrder(ni) != IAttachObj::draw_order_topmost)
				{
					pAttachObj->GetOldDirtyRect(m_AttachDirtyList, ni);
				}
			}
			
			pAttachObj->Release();
			m_vAttachObjs.erase(it);
			break;
		}
	}
}
//------------------------------------------------------------------------
// �ص�(Ʈ�ֶ�������ʱ�ص�)
void CMonsterView::OnFlowObjDestroy(IAttachObj* pAttachObj)
{
	vector<IAttachObj*>::iterator it;
	for (it = m_vFlowObjs.begin(); it != m_vFlowObjs.end(); it++)
	{
		if ((*it) == pAttachObj) // ����
		{
			pAttachObj->Release();
			m_vFlowObjs.erase(it);
			break;
		}
	}
}
//------------------------------------------------------------------------
// ɾ��������������
void CMonsterView::RemoveAllAttachObj()
{
	vector<IAttachObj*>::iterator it;
	for (it = m_vAttachObjs.begin(); it != m_vAttachObjs.end(); it++)
	{
		// �Ѳ�������������²ŷ�����
		for (int ni = 0; ni < (*it)->GetLoopNum(); ++ni)
		{
			if ((*it)->GetDrawOrder(ni) != IAttachObj::draw_order_topmost)
			{
				(*it)->GetOldDirtyRect(m_AttachDirtyList, ni);
			}
		}
		(*it)->Release();
	}
	m_vAttachObjs.clear();

	m_vAttackAttachObjs.clear();
}
//------------------------------------------------------------------------
// ����Ŀǰ״̬�¶�Ӧ�ļ���ID
void CMonsterView::SetStatusMagicCheckID(int nCheckID)
{
	GetCurState()->SetMagicCheckID(nCheckID);
}
//------------------------------------------------------------------------
// ����Ŀǰ״̬��Ҫ������ͨ��������
void CMonsterView::SetStatusPlayCommondAttackSound()
{
	GetCurState()->SetPlayCommondAttackSound();
}
//------------------------------------------------------------------------
// ����Ŀǰ״̬�µ���������
void CMonsterView::SetStatusPreAttack()
{
	GetCurState()->SetPreAttack();
}
//------------------------------------------------------------------------
// ����Ŀǰ״̬�µ�һ���Թ�������
void CMonsterView::SetStatusAttackOnce()
{
	GetCurState()->SetAttackOnce();
}
//------------------------------------------------------------------------
// ����Ŀǰ״̬�µĳ�����������
void CMonsterView::SetStatusAttackCtrl()
{
	GetCurState()->SetAttackCtrl();
}
//------------------------------------------------------------------------
// Ϊ�˲�Ӱ����
void CMonsterView::OnDrawEx(ICanvas* pCanvas,const POINT& ptTileCenter, DWORD dwFlag, SAniContext *pContext, BYTE nAlpha)
{
	if(!pCanvas || !m_bVisible || m_byteAlphaAll != 0)
		return;
	
	SAniContext OldAniContext;
	OldAniContext.SetData(&m_AniContext);
	m_AniContext.SetData(pContext);

	int nOldState = m_AniContext.nState;
	// �������û���ܲ������Ǿ�����·�������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
	if(GetState() == CrtStateRun && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateRun)))
		m_AniContext.nState = CrtStateWalk;

	// �������û����·�����Ǿ���վ���������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
	if(GetState() == CrtStateWalk && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateWalk)))
		m_AniContext.nState = CrtStateStand;

	// ���ж�������ָ����һ����
	if(GetState() == CrtStateFallow)
		m_AniContext.nState = m_nFallowStateID;

	// ����ɫ
	int offx, offy;
	int nModuleType, nCurAni;
	CAniGroup* pCurObj;
	BYTE nDrawOrder[CrtPartMax/2];
	memset (nDrawOrder, 0, sizeof(BYTE) * CrtPartMax/2);
	int nCount = g_ResLoader.GetFrameSort(nDrawOrder, m_AniContext);
	for (int i = 0; i < nCount; i++)
	{
		nModuleType = nDrawOrder[i];
		if(nModuleType < 0 || nModuleType > CrtPartMax - 1)
			continue;
		pCurObj = RequestRes(nModuleType);
	
		if (pCurObj)
		{
			nCurAni = GetCurAni(nModuleType);
			if (nCurAni == -1)
				continue;
			pCurObj->GetOffset(nCurAni, m_AniContext.nCurFrame, offx, offy);
			pCurObj->Draw(pCanvas, 
				ptTileCenter.x - m_ptAnchor.x + offx,
				ptTileCenter.y - m_ptAnchor.y + offy,
				nCurAni, m_AniContext.nCurFrame, dwFlag, nAlpha, m_Pal[nModuleType].GetPal(m_nCurPalType));
		}

		// ����Ч
		nModuleType++;
		pCurObj = RequestRes(nModuleType);
		if (pCurObj)
		{
			int nCurAni = GetCurAni(nModuleType);
			if (nCurAni == -1)
				continue;
			pCurObj->GetOffset(nCurAni, m_AniContext.nCurFrame, offx, offy);
			if (m_byteAlpha[nModuleType] == 0 || dwFlag == (BLIT_MASK | BLIT_TRANS))
			{
				pCurObj->Draw(pCanvas,
					m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
					m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
					nCurAni, m_AniContext.nCurFrame, dwFlag, 0, m_Pal[nModuleType].GetPal(m_nCurPalType));
			}
			else
			{
				pCurObj->Draw(pCanvas,
					m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
					m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
					nCurAni, m_AniContext.nCurFrame, dwFlag, m_byteAlpha[nModuleType], m_Pal[nModuleType].GetPal(m_nCurPalType));
			}
		}
	}

	m_AniContext.SetData(&OldAniContext);
};
//------------------------------------------------------------------------
BOOL CMonsterView::CreateItem(SMonsterViewArchive* pArchive)
{
	ASSERT(pArchive);
	struct STemp 
	{
		DWORD dwResID;						//����ID��ͨ�����ID���Ի�ȡ����������
		ICreatureSink* pCrtSink;
	};

	m_dwCrtID = ((STemp*)pArchive)->dwResID;//����ID
	m_pCrtSink = ((STemp*)pArchive)->pCrtSink;
	
	if(m_dwCrtID <= 0 || m_pCrtSink == NULL)
	{
		TraceLn("ERROR: CMonsterView::CreateItem FAILE ID = %d", m_dwCrtID);
		return FALSE;
	}

	OnUpdate();
	
	return TRUE;
}
//------------------------------------------------------------------------
/*

//��ȡվλ����Ϣ
BOOL CMonsterView::ReadOccupantInfo()
{
	char buffer[4096];
	unsigned char buf[2048];
	COccupantTileList *pL = (COccupantTileList*)buffer;
	BOOL nLen = g_ResLoader.GetNpcOccupantInfo(m_dwCrtID, buf, 2048);
	if (nLen)
	{
		int w = buf[2], h = buf[3];
		pL->Init(w*h);
		pL->GetPreData().nOffAnchorX = - (char)buf[0];
		pL->GetPreData().nOffAnchorY = - (char)buf[1];
		pL->GetPreData().wWidth = w;
		pL->GetPreData().wHeight = h;
		
		int nRepeatCount = 0;
		int nDirectCount = 0;
		int nPos = 4;
		while (nPos < nLen)
		{
			if (buf[nPos] < 0x80) // ��ֵͬ
			{
				nDirectCount = buf[nPos++] + 1;
				for (int k=0; k<nDirectCount; k++,nPos++)
				{
					pL->Add(buf[nPos]);
				}
			}
			else // ��ֵͬ
			{
				nRepeatCount = buf[nPos++] - 127;
				for (int k=0; k<nRepeatCount; k++)
					pL->Add(buf[nPos]);
				nPos ++;
			}
		}
	}
	else
	{
//		pL->Init(0);
//		pL->GetPreData().nOffAnchorX = 0;
//		pL->GetPreData().nOffAnchorY = 0;
//		pL->GetPreData().wWidth = 0;
//		pL->GetPreData().wHeight = 0;

		return FALSE;
	}
	return m_MstOccInfo.SetOccupantTileList(pL);
}*/

//------------------------------------------------------------------------
// ������������߲���(����ֱ����Ϊ׼)
void CMonsterView::SetStepWalk(float fStep)
{
	const float fRateX = 1.0f;
	const float fRateY = 0.5f;
	if (fStep > 32.0f) fStep = 32.0f;
	if (fStep <= 0.0f) fStep = 1.0f;
	
	m_StepWalk[CrtDirNorth].x = 0;
	m_StepWalk[CrtDirNorth].y = -fStep;
	
	m_StepWalk[CrtDirSouth].x = 0;
	m_StepWalk[CrtDirSouth].y = fStep;
	
	m_StepWalk[CrtDirEast].x = fStep*2;
	m_StepWalk[CrtDirEast].y = 0;
	
	m_StepWalk[CrtDirWest].x = -fStep*2;
	m_StepWalk[CrtDirWest].y = 0;
	
	m_StepWalk[CrtDirEastNorth].x = fRateX*fStep;
	m_StepWalk[CrtDirEastNorth].y = -fRateY*fStep;
	
	m_StepWalk[CrtDirEastSouth].x = fRateX*fStep;
	m_StepWalk[CrtDirEastSouth].y = fRateY*fStep;
	
	m_StepWalk[CrtDirWestSouth].x = -fRateX*fStep;
	m_StepWalk[CrtDirWestSouth].y = fRateY*fStep;
	
	m_StepWalk[CrtDirWestNorth].x = -fRateX*fStep;
	m_StepWalk[CrtDirWestNorth].y = -fRateY*fStep;
}
//------------------------------------------------------------------------
// ����������ܶ�����(����ֱ����Ϊ׼)
void CMonsterView::SetStepRun(float fStep)
{
	const float fRateX = 1.0f;
	const float fRateY = 0.5f;
	if (fStep > 32.0f) fStep = 32.0f;
	if (fStep <= 0.0f) fStep = 1.0f;
	
	m_StepRun[CrtDirNorth].x = 0;
	m_StepRun[CrtDirNorth].y = -fStep;
	
	m_StepRun[CrtDirSouth].x = 0;
	m_StepRun[CrtDirSouth].y = fStep;
	
	m_StepRun[CrtDirEast].x = fStep*2;
	m_StepRun[CrtDirEast].y = 0;
	
	m_StepRun[CrtDirWest].x = -fStep*2;
	m_StepRun[CrtDirWest].y = 0;
	
	m_StepRun[CrtDirEastNorth].x = fRateX*fStep;
	m_StepRun[CrtDirEastNorth].y = -fRateY*fStep;
	
	m_StepRun[CrtDirEastSouth].x = fRateX*fStep;
	m_StepRun[CrtDirEastSouth].y = fRateY*fStep;
	
	m_StepRun[CrtDirWestSouth].x = -fRateX*fStep;
	m_StepRun[CrtDirWestSouth].y = fRateY*fStep;
	
	m_StepRun[CrtDirWestNorth].x = -fRateX*fStep;
	m_StepRun[CrtDirWestNorth].y = -fRateY*fStep;
}
//------------------------------------------------------------------------
// ��������������ܶ�����(����ֱ����Ϊ׼)
void CMonsterView::SetStepRide(float fStep)
{
	const float fRateX = 1.0f;
	const float fRateY = 0.5f;
	if (fStep > 32.0f) fStep = 32.0f;
	if (fStep <= 0.0f) fStep = 1.0f;
	
	m_StepRide[CrtDirNorth].x = 0;
	m_StepRide[CrtDirNorth].y = -fStep;
	
	m_StepRide[CrtDirSouth].x = 0;
	m_StepRide[CrtDirSouth].y = fStep;
	
	m_StepRide[CrtDirEast].x = fStep*2;
	m_StepRide[CrtDirEast].y = 0;
	
	m_StepRide[CrtDirWest].x = -fStep*2;
	m_StepRide[CrtDirWest].y = 0;
	
	m_StepRide[CrtDirEastNorth].x = fRateX*fStep;
	m_StepRide[CrtDirEastNorth].y = -fRateY*fStep;
	
	m_StepRide[CrtDirEastSouth].x = fRateX*fStep;
	m_StepRide[CrtDirEastSouth].y = fRateY*fStep;
	
	m_StepRide[CrtDirWestSouth].x = -fRateX*fStep;
	m_StepRide[CrtDirWestSouth].y = fRateY*fStep;
	
	m_StepRide[CrtDirWestNorth].x = -fRateX*fStep;
	m_StepRide[CrtDirWestNorth].y = -fRateY*fStep;
}
//------------------------------------------------------------------------
// ����
BOOL CMonsterView::ChangeShapEX()
{
	SMonsterViewCreateParam mvcp;
	memset(&mvcp, 0, sizeof(mvcp));

	if(!g_ResLoader.GetMonsterCreateParam(m_dwShapID[m_nShapLv], mvcp))
		return FALSE;

	m_AniContext.nType = mvcp.nType;
	g_ResLoader.GetAnchor(m_AniContext.nType, m_ptAnchor);
	int nDelayBuf[CrtStateMax];
	int nAttackFrameBuf[CrtStateMax];
	int nAttackSoundFrameBuf[CrtStateMax];
	memset (nDelayBuf, 0, sizeof(int) * CrtStateMax);
	memset (nAttackFrameBuf, 0, sizeof(int) * CrtStateMax);
	memset (nAttackSoundFrameBuf, 0, sizeof(int) * CrtStateMax);
	for (int i = 0; i < CrtStateMax; i++)
	{
		nDelayBuf[i] = g_ResLoader.GetFrameDelay(m_AniContext.nType, i);
		nAttackFrameBuf[i] = g_ResLoader.GetAttackFrame(m_AniContext.nType, i);
		nAttackSoundFrameBuf[i] = g_ResLoader.GetAttackSoundFrame(m_AniContext.nType, i);
	}
	CMstFSM::SetAllStateDelay(nDelayBuf);
	CMstFSM::SetAllStateAttackFrame(nAttackFrameBuf);
	CMstFSM::SetAllStateAttackSoundFrame(nAttackSoundFrameBuf);

	int nDelay[3];
	nDelay[0] = g_ResLoader.GetWalkDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
	nDelay[1] = g_ResLoader.GetRunDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
	nDelay[2] = g_ResLoader.GetRideDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
	CMstFSM::SetMoveDelay(nDelay);

	m_pCurrentState->Start();

	m_fStepWalk = g_ResLoader.GetStep(m_AniContext.nType, CrtStateWalk);
	m_fStepRun = g_ResLoader.GetStep(m_AniContext.nType, CrtStateRun);
	m_fStepRide = g_ResLoader.GetStep(m_AniContext.nType, CrtStateRideRun);

	//���ù������������
	if (!g_ResLoader.GetMonsterbuf(m_dwShapID[m_nShapLv], m_dwModuleID, m_byteAlpha, m_bytePal))
	{
		return FALSE;
	}

	// �ٶȵȼ�֧��	
	SetStepWalk(m_fStepWalk);
	SetStepRun(m_fStepRun);
	SetStepRide(m_fStepRide);

	// ƫ��λ�����
	mvcp.nHalfHeightOffset = g_ResLoader.GetHalfHeigh(m_AniContext.nType);
	mvcp.nHeightOffset = g_ResLoader.GetWholeHeigh(m_AniContext.nType);
	SetHalfHeightOffset(mvcp.nHalfHeightOffset);
	SetHeightOffset(mvcp.nHeightOffset);
	
	m_nBeHitLoopBeginFrame = g_ResLoader.GetBeHitLoopBegin(m_AniContext.nType);
	m_nBeHitLoopEndFrame = g_ResLoader.GetBeHitLoopEnd(m_AniContext.nType);

	memcpy (m_nActStateCommonAttack, mvcp.nActStateCommonAttack, sizeof(m_nActStateCommonAttack));
	m_nCommonAttackStyleNum = 0;
	for(int j = 0; j < 3; j++)
	{
		if(m_nActStateCommonAttack[j] >= 0)
			m_nCommonAttackStyleNum++;
	}
	m_nActStateJuqi = mvcp.nActStateJuqi;
	m_nActStateSkillAttack = mvcp.nActStateSkillAttack;
	m_nCommonAttackSoundID = mvcp.nSoundIDCommonAttack;
	
	m_nSoundIDFallow = mvcp.nSoundIDFallow;
	m_nSoundIDBeHit = mvcp.nSoundIDBeHit;
	m_nSoundIDDie = mvcp.nSoundIDDie;
	
	m_nFallowStateID = g_ResLoader.GetFallowStateID(m_AniContext.nType);

	IniTotalFrame();
	ClearShowRect();
	return TRUE;
}
//------------------------------------------------------------------------