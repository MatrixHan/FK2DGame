/**
*	created:		2012-7-2   3:54
*	filename: 		ActorView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/BasicCommon.h"
#include "../Include/ActorView.h"
#include "../Include/ResLoader.h"
#include "../Include/PalMgr.h"
#include "../Include/SkillCreate.h"
//------------------------------------------------------------------------
CActorView::CActorView() : CActFSM()
{
	memset(m_pModule, 0, sizeof(m_pModule));
	memset(m_dwModuleID, 0, sizeof(m_dwModuleID));
	m_pGroupInfoImage = NULL;
	AddFlag(flagInfo|flagName|flagPaoPao|flagGroup|flagBloodSlot);
	m_nFootprintLeftRight = 0;
	m_nWeaponStyle = ActWeaponStyleSingle;
	CActFSM::Create(this);
}
//------------------------------------------------------------------------
CActorView::~CActorView()
{
}
//------------------------------------------------------------------------
void CActorView::Release()
{
	Close();
	if (m_pCurrentState)
	{
		m_pCurrentState->End();
		m_pCurrentState = NULL;
	}
	delete this;
}
//------------------------------------------------------------------------
void CActorView::Close()
{
	m_pGroupInfoImage = NULL;
}
//------------------------------------------------------------------------
void CActorView::OnTimer(DWORD dwEventID)
{
	CActFSM::OnTimer(dwEventID);
}
//------------------------------------------------------------------------
// ��ȡ�������ID
DWORD CActorView::GetClassID() const
{
	return miActorObj;
}
//------------------------------------------------------------------------
// ������ȡ���ݣ����ڴ��ļ��д�������
BOOL CActorView::OnSerialize(BOOL bWrite,IArchive* pArchive)
{
	if (!pArchive) return FALSE;
	if (bWrite)
	{
		SActorViewArchive a = 
		{
			m_dwResID,
		};
			
		pArchive->Write((char*)&a, sizeof(a));
		return TRUE;
	}
	else
	{
		Close();
		if (!CreateItem((SActorViewArchive*)pArchive->GetBuffer()))
		{
			TraceLn("ERROR: CActorView::OnSerialize(bRead), create item failed!");
		}
		pArchive->Seek(sizeof(SActorViewArchive));
		return TRUE;
	}
}
//------------------------------------------------------------------------
// ���������ֳ���������
BOOL CActorView::OnCreateItem(const char* buf, int nLen)
{
	Close();
	return CreateItem((SActorViewArchive*)(buf));
}
//------------------------------------------------------------------------
// ����(�磺���ؽű����ݵ�)
void CActorView::OnUpdate()
{
	//����֡��ʱ
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
	CActFSM::SetAllStateDelay(nDelayBuf);
	CActFSM::SetAllStateAttackFrame(nAttackFrameBuf);
	CActFSM::SetAllStateAttackSoundFrame(nAttackSoundFrameBuf);

	int nDelay[3];
	nDelay[0] = g_ResLoader.GetWalkDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
	nDelay[1] = g_ResLoader.GetRunDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
	nDelay[2] = g_ResLoader.GetRideDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
	CActFSM::SetMoveDelay(nDelay);

	m_pCurrentState->Start();
	m_fStepWalk = g_ResLoader.GetStep(m_AniContext.nType, CrtStateWalk);
	m_fStepRun = g_ResLoader.GetStep(m_AniContext.nType, CrtStateRun);
	m_fStepRide = g_ResLoader.GetStep(m_AniContext.nType, CrtStateRideRun);
	g_ResLoader.GetAnchor(m_AniContext.nType, m_ptAnchor);


	SetStepWalk(m_fStepWalk);
	SetStepRun(m_fStepRun);
	SetStepRide(m_fStepRide);

	// ƫ��λ�����
	SetHalfHeightOffset(g_ResLoader.GetHalfHeigh(m_AniContext.nType));
	SetHeightOffset(g_ResLoader.GetWholeHeigh(m_AniContext.nType));

	m_nBeHitLoopBeginFrame = g_ResLoader.GetBeHitLoopBegin(m_AniContext.nType);
	m_nBeHitLoopEndFrame = g_ResLoader.GetBeHitLoopEnd(m_AniContext.nType);

	ClearShowRect();
	
	IniTotalFrame();

}
//------------------------------------------------------------------------
// ��ȡ��ռλ�����ռλֵ
DWORD CActorView::GetOccupantValue()
{
	if (State() == CrtStateDie || !m_bOccupant)
		return 0x0;
	return CMapItem::GetOccupantValue();
}
//------------------------------------------------------------------------
// ��ȡ�����ڵ�ͼ�е���ʾ��
DWORD CActorView::GetLayerValue()
{
	if (State() == CrtStateDie)
		return milvLowest;
	return milvMidle;
}
//------------------------------------------------------------------------
// ����
void CActorView::SetDirty(ICanvas* pCanvas, const POINT& ptViewTop)
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
void CActorView::GetOldDirtyRect(list<RECT> &DirtyRectList)
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
void CActorView::OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag)
{
	if(!pCanvas || !m_bVisible)
		return;

	// ����Ч(draw order = draw_order_prev)
	vector<IAttachObj*>::iterator it;
	POINT pt;
	pt.x = m_ptTileOffset.x + ptTileCenter.x;
	pt.y = m_ptTileOffset.y + ptTileCenter.y;
	
	for (it = m_vAttachObjs.begin(); it != m_vAttachObjs.end(); ++it)
		for (int ni = 0; ni < (*it)->GetLoopNum(); ++ni)
			if ((*it)->GetDrawOrder(ni) == IAttachObj::draw_order_prev)
				(*it)->OnDraw(pCanvas, pt, dwFlag, ni);

	int nOldState = m_AniContext.nState;
	if(IsChangeShap())
	{
		// �������û���ܲ������Ǿ�����·�������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
		if(GetState() == CrtStateRun && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateRun)))
			m_AniContext.nState = CrtStateWalk;

		// �������û����·�����Ǿ���վ���������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
		if(GetState() == CrtStateWalk && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateWalk)))
			m_AniContext.nState = CrtStateStand;
		
		// �������û�����¶����Ǿ���վ���������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
		if(GetState() == CrtStateSit && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateSit)))
			m_AniContext.nState = CrtStateStand;
	}

	// ����ɫ
	int offx, offy;
	int nModuleType, nCurAni;
	CAniGroup* pCurObj;
	BYTE nDrawOrder[CrtPartMax/2];
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
			if (m_byteAlpha[nModuleType] == 0 || m_byteAlphaAll != 0)
			{
				pCurObj->Draw(pCanvas, 
					m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
					m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
					nCurAni, m_AniContext.nCurFrame, dwFlag,
					m_byteAlphaAll, m_Pal[nModuleType].GetPal(m_nCurPalType), m_pParticle[nModuleType]);
			}
			else
			{
				pCurObj->Draw(pCanvas, 
					m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
					m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
					nCurAni, m_AniContext.nCurFrame, dwFlag,
					m_byteAlpha[nModuleType], m_Pal[nModuleType].GetPal(m_nCurPalType), m_pParticle[nModuleType]);
			}
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
			if (m_byteAlpha[nModuleType] == 0 || m_byteAlphaAll != 0)
			{
				pCurObj->Draw(pCanvas,
					m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
					m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
					nCurAni, m_AniContext.nCurFrame, dwFlag,
					m_byteAlphaAll, m_Pal[nModuleType].GetPal(m_nCurPalType));
			}
			else
			{
				pCurObj->Draw(pCanvas,
					m_ptTileOffset.x + ptTileCenter.x - m_ptAnchor.x + offx,
					m_ptTileOffset.y + ptTileCenter.y - m_ptAnchor.y + offy,
					nCurAni, m_AniContext.nCurFrame, dwFlag,
					m_byteAlpha[nModuleType], m_Pal[nModuleType].GetPal(m_nCurPalType));
			}
		}
	}

	if(IsChangeShap())
	{
		m_AniContext.nState = nOldState;
	}
	
	// ����Ч(draw order = draw_order_after)
	for (it=m_vAttachObjs.begin(); it!=m_vAttachObjs.end(); ++it)
		for (int ni = 0; ni < (*it)->GetLoopNum(); ++ni)
			if ((*it)->GetDrawOrder(ni) == IAttachObj::draw_order_after)
				(*it)->OnDraw(pCanvas, pt, dwFlag, ni);
};
//------------------------------------------------------------------------
// ��������һЩ���������Ѫ�ۡ��������ݡ���Ʒ���ȣ���Щ������Զ�������ϲ�
void CActorView::OnDrawTopmostObject(ICanvas* pCanvas,const POINT& ptTileCenter, DWORD dwFlag)
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

	if(GetState() == CrtStateSit)
		ptOffset.y += SIT_HEIGHT_OFFSET;

#ifndef _USE
	// draw chat info
	BOOL bHadDrawGroupInfo = FALSE;
	if (m_pPaoPao != NULL)
	{
		if ((dwFlags & flagGroup) && m_pGroupInfoImage)
		{
			ptOffset.y -= m_pGroupInfoImage->GetHeight();
			m_pGroupInfoImage->Draw(pCanvas, 
				ptOffset.x-m_pGroupInfoImage->GetWidth()/2, 
				ptOffset.y, BLIT_MASK);
			bHadDrawGroupInfo = TRUE;
		}
		// �ƺ�
//		m_ActorTitle.OnDraw(pCanvas, ptOffset);
		// ����
		if (dwFlags & flagPaoPao)
		{
			m_pPaoPao->OnDraw(pCanvas, ptOffset, m_szUserName, m_clrName, g_pFont);
		}
	}
	else // draw user name
	{
		if (dwFlags & flagName)
		{
			DrawName(pCanvas, ptOffset);
			ptOffset.y -= g_pFontWithOutline->GetHeight() + 3;
		}

		if ((dwFlags & flagGroup) && m_pGroupInfoImage && !bHadDrawGroupInfo)
		{
			ptOffset.y -= m_pGroupInfoImage->GetHeight();
			m_pGroupInfoImage->Draw(pCanvas, 
				ptOffset.x-m_pGroupInfoImage->GetWidth()/2, 
				ptOffset.y, BLIT_MASK);
		}
		
		// �ƺ�
		m_ActorTitle.OnDraw(pCanvas, ptOffset);
	}
#endif
/*
#ifdef _USE
	DrawName(pCanvas, ptOffset);
	if (IsSelected())
	{
		WPixel clr = WPixel(255,255,0);
		WPixel clrText = WPixel(0,0,255);
		char buf[MAX_PATH];
		POINT ptTile;
		g_pMapView->ScreenToTile(ptTileCenter, ptTile);
		
		if (dwFlags & flagInfo)
		{
			IResObject* pObj = g_ResLoader.m_pScpBodyPart->GetResObject(m_dwResID);
			if (pObj)
			{
				IResObject* pParentObj = pObj->GetParentObject()->GetParentObject();
				wsprintf(buf, "%s\\%s,(%d,%d),CID=%d,RID=%d,SORT=%d", 
					pParentObj->GetObjectName(), pObj->GetObjectName(),
					ptTile.x, ptTile.y, GetClassID(), m_dwResID, dwFlag);
			}
			else
				wsprintf(buf, "(%d,%d),CID=%d,RID=%d,SORT=%d", ptTile.x, ptTile.y, GetClassID(), m_dwResID, dwFlag);
			
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
	if (State() == CrtStateDie)
		return;

	if (dwFlags & flagBloodSlot)
	{
		RECT rc;
		rc.left = ptTileCenter.x + m_ptTileOffset.x - BLOOD_SLOT_WIDTH/2;
		rc.right = rc.left + BLOOD_SLOT_WIDTH + 1;
		rc.top = ptTileCenter.y + m_ptTileOffset.y + GetHeightOffset();

		if(GetState() == CrtStateSit)
			rc.top += SIT_HEIGHT_OFFSET;

		rc.bottom = rc.top + BLOOD_SLOT_HEIGHT + 1;		
		DrawBloodSlot(pCanvas, rc, m_nBloodVol);
	}
/*
	//m_pGroupInfoImage->Draw(pCanvas)
#endif	
	*/
}
//------------------------------------------------------------------------
//��װ
//dwParam1:���ﲿ������
//dwParam2:����ID
int CActorView::cmdChangeModule(DWORD dwParam1, DWORD dwParam2)
{
	if(IsChangeShap())
	{
		if(m_dwBakModuleID[dwParam1] == dwParam2)
			return TRUE;
		
		m_dwBakModuleID[dwParam1] = dwParam2;
		return TRUE;
	}
	else
	{
		if(m_dwModuleID[dwParam1] != dwParam2)
			ClearShowRect();

		// ������ԴID
		DWORD dwID = m_dwModuleID[dwParam1];

		// �ͷ���ǰ����Դ
		if (m_pModule[dwParam1])
		{
			g_ResLoader.ReleaseRes(ioCreature + dwID);
			m_pModule[dwParam1] = NULL;
		}
		
		// �����µ���Դ
		m_dwModuleID[dwParam1] = dwParam2;
		if (!RequestRes(dwParam1))
			return FALSE;

		return TRUE;
	}
}
//------------------------------------------------------------------------
//ʹ��Ĭ��ͼ��
//dwParam1:���ﲿ��
//dwParam2:����Ĭ��ͼ��
int CActorView::cmdSpecialModule(DWORD dwParam1, DWORD dwParam2)
{
	if (dwParam1 < 0 || dwParam1 >= CrtPartMax || dwParam2 < 0)
		return 0;
	m_dwDefaultResID[dwParam1] = dwParam2;
	return 1;
}
//------------------------------------------------------------------------
//��ɫ������ɫ��
//dwParam1:��������
//dwParam2:��ɫ������
int CActorView::cmdModulePalType(DWORD dwParam1, DWORD dwParam2)
{
	if(IsChangeShap())
	{
		if (dwParam1 >= 0 && dwParam1 < CrtPartMax && dwParam2 >=0 && dwParam2 < MAX_PAL_TYPES)
		{
			m_byteBakPal[dwParam1] = dwParam2;
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		if (dwParam1 >= 0 && dwParam1 < CrtPartMax && dwParam2 >=0 && dwParam2 < MAX_PAL_TYPES)
		{
			m_bytePal[dwParam1] = dwParam2;
			return TRUE;
		}
		return FALSE;
	}
}
//------------------------------------------------------------------------
//��ɫ����͸����
//dwParam1:��������
//dwParam2:��͸��ֵ
int CActorView::cmdModuleAlpha(DWORD dwParam1, DWORD dwParam2)
{
	if(IsChangeShap())
	{
		if (dwParam1 >= 0 && dwParam1 < CrtPartMax && dwParam2 >=0 && dwParam2 < 256)
		{
			m_byteBakAlpha[dwParam1] = dwParam2;
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		if (dwParam1 >= 0 && dwParam1 < CrtPartMax && dwParam2 >=0 && dwParam2 < 256)
		{
			m_byteAlpha[dwParam1] = dwParam2;
			return TRUE;
		}
		return FALSE;
	}
}
//------------------------------------------------------------------------
// ��ɫ����(����������Բ���һ�γ�������)
// dwParam1: ?
// dwParam2: ?
int CActorView::cmdBorn(DWORD dwParam1, DWORD dwParam2)
{
	m_bDeaded =	FALSE;
	SetState(CrtStateStand);

	SSkillCreateParam scp;
	scp.nAniType = anitypeGenericAni;
	scp.nIndex = aniBorn;
	scp.ptTile = m_ptTile;
	scp.ptAttack = m_ptTile;

	return CSkillCreate::CreateGenericMagic(&scp);
}
//------------------------------------------------------------------------
// ��ɫ����(����������Բ���һ����������)
// dwParam1: ?
// dwParam2: ?
int CActorView::cmdUpgrade(DWORD dwParam1, DWORD dwParam2)
{
	IResObject* pScpSkill = (IResObject*)CSkillCreate::getSkillScpObj(anitypeGenericAni, aniUpgrade);
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
			CreateAttachObj(&st);
		}
	}
	Fun_PlaySound(m_ptTile, 567);
	return TRUE;
}
//------------------------------------------------------------------------
// ����(����ֻ������)����ʱ����·�ϵ��������µĵ������
// dwParam1: ?
// dwParam2: ?
int CActorView::cmdMouseAni(DWORD dwParam1, DWORD dwParam2)
{
	SSkillCreateParam scp;
	scp.nAniType = anitypeGenericAni;
	scp.nIndex = aniMouse;
	scp.ptTile = m_ptTile;
	scp.ptAttack = *(POINT*)dwParam1;
	
	return CSkillCreate::CreateGenericMagic(&scp);
}
//------------------------------------------------------------------------
// ��������ӳƺ�
// dwParam1: LPCSTR(��ʽ����ĳƺ��ַ���)
// dwParam2: DWORD(�ƺ�Ч����ID)
int CActorView::cmdAddTitle(DWORD dwParam1, DWORD dwParam2)
{
	return m_ActorTitle.AddTitle((LPCSTR)dwParam1, dwParam2);
}
//------------------------------------------------------------------------
// �����������гƺ�
// dwParam1: ?
// dwParam2: ?
int CActorView::cmdClearTitle(DWORD dwParam1, DWORD dwParam2)
{
	m_ActorTitle.ClearTitle();
	return TRUE;
}
//------------------------------------------------------------------------
// ��ɫЯ�������ķ�ʽ(��������?˫������?����������?)
// ���û򷵻�: int* enActorWeaponStyle����
void CActorView::propWeaponStyle(BOOL bSetProp, DWORD dwPropVal)
{
	if (!bSetProp)
		*(int*)dwPropVal = m_nWeaponStyle;
	else
	{
		m_nWeaponStyle = *(int*)dwPropVal;
	}
}
//------------------------------------------------------------------------
// ��ɫ�����Ĺ�����ЧID
// ���û򷵻�: int*
void CActorView::propWeaponSoundID(BOOL bSetProp, DWORD dwPropVal)
{
	if(IsChangeShap())
	{
		if (!bSetProp)
		{
			*(int*)dwPropVal = m_nBakCommonAttackSoundID;
		}
		else
		{
			m_nBakCommonAttackSoundID = *(int*)dwPropVal;
		}
	}
	else
	{
		if (!bSetProp)
		{
			*(int*)dwPropVal = m_nCommonAttackSoundID;
		}
		else
		{
			m_nCommonAttackSoundID = *(int*)dwPropVal;
		}
	}
}
//------------------------------------------------------------------------
// ��ɫ�����Ļ���Ŀ����ЧID
// ���û򷵻�: int*
void CActorView::propBeHitSoundID(BOOL bSetProp, DWORD dwPropVal)
{
	if(IsChangeShap())
	{
		if (!bSetProp)
		{
			*(int*)dwPropVal = m_nBakCommonAttackBeHitSoundID;
		}
		else
		{
			m_nBakCommonAttackBeHitSoundID = *(int*)dwPropVal;
		}
	}
	else
	{
		if (!bSetProp)
		{
			*(int*)dwPropVal = m_nCommonAttackBeHitSoundID;
		}
		else
		{
			m_nCommonAttackBeHitSoundID = *(int*)dwPropVal;
		}
	}
}
//------------------------------------------------------------------------
// �����ϢͼƬ
// ���û򷵻�: IBitmap*
void CActorView::propGroupInfoImage(BOOL bSetProp, DWORD dwPropVal)
{
	if (bSetProp)
	{
		m_pGroupInfoImage = *(IBitmap**)dwPropVal;
	}
	else
		*(IBitmap**)dwPropVal = m_pGroupInfoImage;
}
//------------------------------------------------------------------------
// ��������ĵ�ǰ״̬
void CActorView::SetState(int nState)
{
	if(nState < 0 || nState >= CrtStateMax)
		return;
	
	if(nState == CrtStateBehit && GetMoveStyle() == ActMoveStyleRide)
		return;

	if(nState == CrtStateStand && GetMoveStyle() == ActMoveStyleRide)
		nState = CrtStateRideStand;

	if(m_pStates[nState] == NULL)
		return;

	if (CActFSM::GetCurState() == m_pStates[nState])
	{
		m_pStates[nState]->DelAllAttribute();
		return;
	}

	// ������˲�������������
	if(IsDeaded() && nState != CrtStateDie)
		return;

	m_AniContext.nState = nState;

	if(IsChangeShap())
	{
		// �������û���ܲ������Ǿ�����·�������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
		if(GetState() == CrtStateRun && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateRun)))
			m_AniContext.nState = CrtStateWalk;

		// �������û����·�����Ǿ���վ���������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
		if(GetState() == CrtStateWalk && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateWalk)))
			m_AniContext.nState = CrtStateStand;

		// �������û�иö�����վ����
		if(!g_ResLoader.GetStateEnable(m_AniContext.nType, m_AniContext.nState))
		{
			m_AniContext.nState = CrtStateStand;
			nState = CrtStateStand;
		}
	}

	CActFSM::GotoState(*m_pStates[nState]);

	SetCurFrame(0);

	IniTotalFrame();
}
//------------------------------------------------------------------------
// ֹͣ��ǰ״̬
void CActorView::StopState(int nState)
{
	if(nState < 0 || nState >= CrtStateMax)
		return;
	
	if(m_pStates[nState] == NULL)
		return;

	m_pStates[nState]->Stop();
}
//------------------------------------------------------------------------
// ������ǰ״̬
void CActorView::RestartCurState()
{
	GetCurState()->End();
	GetCurState()->Start();
}
//------------------------------------------------------------------------
// ��ȡ��ͨ��������
int CActorView::GetNormalAttackActID()
{
	if(IsChangeShap())
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
	else
	{
		switch(m_nWeaponStyle) 
		{
		case ActWeaponStyleSingle:
			return CrtStateSingleAttack1;
		case ActWeaponStyleDouble:
			return CrtStateDoubleAttack1;
		case ActWeaponStyleBow:
			return CrtStateArrowAttack;
		default:
			break;
		}
	}
	return CrtStateSingleAttack1;
}
//------------------------------------------------------------------------
// ��ȡĬ�ϵ���������ID���������Ч��
int CActorView::GetJuqiActID()
{
	if(IsChangeShap() && m_AniContext.nType != 0 && m_AniContext.nType != 1)
	{
		return m_nActStateJuqi;
	}
	else
		return -1;
}
//------------------------------------------------------------------------
// ��ȡĬ�ϵļ��ܹ�������ID���������Ч��
int CActorView::GetSkillAttackActID()
{
	if(IsChangeShap() && m_AniContext.nType != 0 && m_AniContext.nType != 1)
	{
		return m_nActStateSkillAttack;
	}
	else
		return -1;
}
//------------------------------------------------------------------------
// ���ö�����δ�С��Χ
void CActorView::SetShowRect()
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

	if(IsChangeShap())
	{
		// �������û���ܲ������Ǿ�����·�������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
		if(m_AniContext.nState == CrtStateRun && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateRun)))
			m_AniContext.nState = CrtStateWalk;

		// �������û����·�����Ǿ���վ���������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
		if(m_AniContext.nState == CrtStateWalk && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateWalk)))
			m_AniContext.nState = CrtStateStand;
		
		// �������û�����¶����Ǿ���վ���������棬�߻��Ǳ�����ģ����Ǹо�������ܱ�Ť
		if(m_AniContext.nState == CrtStateSit && !(g_ResLoader.GetStateEnable(m_AniContext.nType, CrtStateSit)))
			m_AniContext.nState = CrtStateStand;
	}

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
void CActorView::SetAllMoveDelay(int* pDelay)
{
	SetMoveDelay(pDelay);
}
//------------------------------------------------------------------------
// �ص�(�����������ʱ�ص�)
void CActorView::OnAttachObjDestroy(IAttachObj* pAttachObj)
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
void CActorView::OnFlowObjDestroy(IAttachObj* pAttachObj)
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
void CActorView::RemoveAllAttachObj()
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
// ��ȡ��������
int CActorView::GetWeaponStyle()
{
	return m_nWeaponStyle;
}
//------------------------------------------------------------------------
// ����Ŀǰ״̬�¶�Ӧ�ļ���ID
void CActorView::SetStatusMagicCheckID(int nCheckID)
{
	GetCurState()->SetMagicCheckID(nCheckID);
}
//------------------------------------------------------------------------
// ����Ŀǰ״̬��Ҫ������ͨ��������
void CActorView::SetStatusPlayCommondAttackSound()
{
	GetCurState()->SetPlayCommondAttackSound();
}
//------------------------------------------------------------------------
// ����Ŀǰ״̬�µ���������
void CActorView::SetStatusPreAttack()
{
	GetCurState()->SetPreAttack();
}
//------------------------------------------------------------------------
// ����Ŀǰ״̬�µ�һ���Թ�������
void CActorView::SetStatusAttackOnce()
{
	GetCurState()->SetAttackOnce();
}
//------------------------------------------------------------------------
// ����Ŀǰ״̬�µĳ�����������
void CActorView::SetStatusAttackCtrl()
{
	GetCurState()->SetAttackCtrl();
}
//------------------------------------------------------------------------
// Ϊ�˲�Ӱ����
void CActorView::OnDrawEx(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag, SAniContext *pContext, BYTE nAlpha)		// Ϊ�˲�Ӱ����
{
	if(!pCanvas || !m_bVisible || m_byteAlphaAll != 0)
		return;
	
	SAniContext OldAniContext;
	OldAniContext.SetData(&m_AniContext);
	m_AniContext.SetData(pContext);

	if(IsChangeShap())
	{
		if(!(g_ResLoader.GetStateEnable(m_AniContext.nType, GetState())))
			m_AniContext.nState = CrtStateStand;
	}

	// ����ɫ
	int offx, offy;
	int nModuleType, nCurAni;
	CAniGroup* pCurObj;
	BYTE nDrawOrder[CrtPartMax/2];
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
			nCurAni = GetCurAni(nModuleType);
			if (nCurAni == -1)
				continue;
			pCurObj->GetOffset(nCurAni, m_AniContext.nCurFrame, offx, offy);
			pCurObj->Draw(pCanvas, 
				ptTileCenter.x - m_ptAnchor.x + offx,
				ptTileCenter.y - m_ptAnchor.y + offy,
				nCurAni, m_AniContext.nCurFrame, dwFlag, nAlpha, m_Pal[nModuleType].GetPal(m_nCurPalType));
		}
	}
	m_AniContext.SetData(&OldAniContext);
}
//------------------------------------------------------------------------
BOOL CActorView::CreateItem(SActorViewArchive* pArchive)
{
	ASSERT(pArchive);
	struct STemp
	{
		DWORD dwActorType;
		ICreatureSink* pCrtSink;
	};
	m_pCrtSink = ((STemp*)pArchive)->pCrtSink;
	m_AniContext.nType = ((STemp*)pArchive)->dwActorType;
	m_dwType = m_AniContext.nType;

	OnUpdate();

	return TRUE;
}
//------------------------------------------------------------------------
// ������ӡ
int CActorView::CreateFootprint()
{
	SSkillCreateParam scp;
	scp.nAniType = anitypeGenericAni;
	scp.nIndex = aniFootprint;
	scp.ptTile = m_ptTile;
	scp.ptAttack = m_ptTile;
	m_nFootprintLeftRight = (m_nFootprintLeftRight == 0) ? 1 : 0;
	
	return CSkillCreate::CreateFootprintAnimate(&scp, GetDirection(), m_nFootprintLeftRight);	
}
//------------------------------------------------------------------------
// ����
BOOL CActorView::ChangeShapEX()
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
	CActFSM::SetAllStateDelay(nDelayBuf);
	CActFSM::SetAllStateAttackFrame(nAttackFrameBuf);
	CActFSM::SetAllStateAttackSoundFrame(nAttackSoundFrameBuf);

	int nDelay[3];
	nDelay[0] = g_ResLoader.GetWalkDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
	nDelay[1] = g_ResLoader.GetRunDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
	nDelay[2] = g_ResLoader.GetRideDelayByGrade(m_AniContext.nType, m_nMoveSpeedLevel);
	CActFSM::SetMoveDelay(nDelay);

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
	m_nCommonAttackBeHitSoundID = 0;
	
	m_nSoundIDFallow = mvcp.nSoundIDFallow;
	m_nSoundIDBeHit = mvcp.nSoundIDBeHit;
	m_nSoundIDDie = mvcp.nSoundIDDie;
	
	IniTotalFrame();
	ClearShowRect();
	return TRUE;
}
//------------------------------------------------------------------------