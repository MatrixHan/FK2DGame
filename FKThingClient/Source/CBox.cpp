/**
*	created:		2012-7-3   0:41
*	filename: 		CBox
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/CBox.h"
#include "../Include/ThingClientCommon.h"
#include "../Include/BoxPopedomMgr.h"
#include "../Include/CreatureViewWrapper.h"
#include "../Include/CThingClientFacade.h"
#include "../../FKGame/Network/IAppNetworkLink.h"
//------------------------------------------------------------------------
CBox::CBox() : m_uid(INVALID_UID), m_pMapItemView(NULL)
{
	m_ThingClass.SetThingClass(THINGCLASS_BOX);	
	ZeroMemory(&m_Property, sizeof(SBOX_PROPERTY_LIST));
}
//------------------------------------------------------------------------
CBox::~CBox()
{

}
//------------------------------------------------------------------------
BOOL CBox::Create()
{
	struct SBoxResID
	{
		DWORD	dwResIDBox;
		DWORD	dwResIDStar;
	};
	
	// ȡ�ô�������	
	IItemClassFactory * pItemClassFactory = g_pThingClientFacade->GetItemFactory();
	if(pItemClassFactory == NULL)
	{
		return FALSE;
	}
	
	// ������ͼ����
	SBoxResID resid;
	resid.dwResIDBox  = m_Property.dwResID1;
	resid.dwResIDStar = m_Property.dwResID2;		
	m_pMapItemView = (CMapItemView*)pItemClassFactory->CreateItem(miStaticObj_Article, (LPCSTR)&resid, sizeof(SBoxResID));
	if(m_pMapItemView == NULL)
	{
		return FALSE;
	}

	m_pMapItemView->SetItemData((DWORD)(this));
	
	// ���������������
	CCreatureViewWrapper viewwrapper(m_pMapItemView);
	viewwrapper.AddToMap(m_Property.ptLoc);
	viewwrapper.SetTile(m_Property.ptLoc);

	// �Ƿ���ʾ����
	if(g_pGlobal == NULL || g_pGlobal->m_pPlayer == NULL)
	{
		// �����ȴ��������ﻹû�д���
		g_BoxPopedomManager.CheckBox(m_uid);
	}
	else
	{
		BOOL bRight = g_BoxPopedomManager.CanOpen(m_uid, g_pGlobal->m_pPlayer->GetUID());
		SetRight(bRight);
	}
	
	// ���䶯��
	if(m_Property.dwState == BOX_CREATE)
	{
		m_pMapItemView->OnCommand(AtcCmdThrow, 0, 0);
	}
	
	return TRUE;
}
//------------------------------------------------------------------------
void CBox::Release(void)
{
	// ��ʼ������...
	// ��һ�¼���ȥ,����ɾ��
	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_RELEASE);
	OnMessage(wMsgID, NULL, 0);

	// �������������
	try
	{
		if (m_Property.dwState == BOX_OPENING
			|| m_Property.dwState == BOX_OPENED)
		{			
			g_pGlobal->m_pWndMgr->OnEvent(WND_TREASUREBOX, WE_TREASURE_CLOSE, 0, (LPARAM)&m_uid);		
			Close();
		}
		ClearBox();
	}
	catch (...) {
	}
	
	// ��ThingTable�����
	IThingTable* pThingTable = g_pThingClientFacade->GetThingTable();
	if (pThingTable == NULL
		|| !pThingTable->Remove(m_uid))
	{
		TraceLn("��ThingTable�����%sʧ�ܣ�", GetName());
	}

	// ���MapItem
	IMapView* pMapView;
	if (m_pMapItemView != NULL
		&& (pMapView = g_pThingClientFacade->GetMapView()) != NULL)
	{
		POINTS ptLoc  = GetLoc();
		POINT  ptTile = { ptLoc.x, ptLoc.y };

		if (!pMapView->RemoveItem(ptTile, m_pMapItemView))
		{
			TraceLn("���MapItemʧ�ܣ�%s[%d, %d]", GetName(), ptLoc.x, ptLoc.y);
		}

		SAFE_RELEASE(m_pMapItemView);
	}
	
	delete this;
}
//------------------------------------------------------------------------
void CBox::SetUID(UID uid)
{
	m_uid = uid;
}
//------------------------------------------------------------------------
UID	CBox::GetUID(void)
{
	return m_uid;
}
//------------------------------------------------------------------------
DWORD CBox::GetMapID(void)
{
	return GETUIDSERVERID(m_uid);
}
//------------------------------------------------------------------------
void CBox::SetLoc(POINTS ptLoc)
{
	m_Property.ptLoc = ptLoc;
}
//------------------------------------------------------------------------
POINTS CBox::GetLoc(void)
{
	return m_Property.ptLoc;
}
//------------------------------------------------------------------------
IThingClass* CBox::GetThingClass(void)
{
	return static_cast<IThingClass*>(&m_ThingClass);
}
//------------------------------------------------------------------------
// ˽�ж����ֳ�����
// ˵����
// ˽�ж����ֳ���ָ�û��Լ��Ľ�ɫ��װ���ȵ���ϸ��Ϣ���������ǿ�������
// ��dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ���
// �磺�û���¼ʱ��������ȡ����������ֳ�
// �ͻ��˸���dwClassID�������󣬵���OnGetPrivateContext()���ö����ֳ�
BOOL CBox::OnSetPrivateContext(LPCSTR buf, int nLen)
{
	if (buf == NULL || nLen != sizeof(SBOX_PROPERTY_LIST))
	{
		TraceLn("CBox::OnSetPrivateContext Failed! [buf == NULL]");
		return FALSE;
	}
	
	memcpy(&m_Property, buf, nLen);	
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CBox::OnGetPrivateContext(LPSTR buf, int &nLen)
{
	// �ͻ��˲�������������
	return FALSE;
}
//------------------------------------------------------------------------
// ���󹫿��ֳ�����
// ˵����
// �����ֳ�Ϊ����������GameWorld�е���������鿴�Ķ�����Ϣ�����ɫ����HP��װ����״��
// ������ȡ�ö����ֳ�����dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ��ˣ�
// �ͻ��˸���dwThingClassID�������󣬵���OnSetContext()���ö����ֳ�
BOOL CBox::OnSetPublicContext(LPCSTR buf, int nLen)
{
	// ��������
	CDataArchive dataarchive((char*)buf, nLen);
	if (!dataarchive.Read(&m_Property, sizeof(SBOX_PROPERTY_LIST)))
	{
		return FALSE;
	}

	// ��λ
	g_BoxPopedomManager.ClearBox(m_uid);

	// �õ��ú��ӵ�����Ȩ����
	// ����
	int nSize = 0;
	if (!dataarchive.Read(&nSize, sizeof(int)))
	{
		return FALSE;
	}

	long ldbid;	
	for (int i = 0; i < nSize; ++i)
	{
		if (!dataarchive.Read(&ldbid, sizeof(long)))
		{
			return FALSE;
		}

		// Ȩ��
		g_BoxPopedomManager.AddMaster(m_uid, ldbid);
	}

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CBox::OnGetPublicContext(LPSTR buf, int &nLen)
{
	// �ͻ��˲�������������
	return FALSE;
}
//------------------------------------------------------------------------
int CBox::OnCommand(DWORD dwCommand, LPCSTR buf, int nLen)
{
	return 0;
}
//------------------------------------------------------------------------
// ��Ϣ֪ͨ, ��ʹ�߼����������ת
void CBox::OnMessage(DWORD dwMsg, LPCSTR buf, int nLen)
{
	// �¼�������
	IEventServer* pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return;
	}

	// ���ж�����Ƿ�ͨ��
	if(pEventServer->FireVote((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_BOX, (DWORD)this) == FALSE)
	{
		return;
	}

	// Ȼ����Ϊ�¼�
	pEventServer->FireAction((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_BOX, (DWORD)this);
}
//------------------------------------------------------------------------
// ��  ����������Ӧ��FireResponse��
void CBox::OnResponse(DWORD dwMsg, LPCSTR buf, int nLen)
{
	// �¼�������	
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return;
	}

	// ������Ӧ
	pEventServer->FireResponse((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_BOX, (DWORD)this);
}
//------------------------------------------------------------------------
// ��  ��: ���ı�����ķ���¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventVoteListener * pListener - �¼������
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CBox::SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}

	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_BOX, (DWORD)this, szDescription);
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ı�����ķ���¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventVoteListener * pListener  - �¼������	
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CBox::UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}

	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_BOX, (DWORD)this);
}
//------------------------------------------------------------------------
// ��  ��: ���ı��������Ϊ�¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventActionListener * pListener - �¼���Ϊִ����
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CBox::SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_BOX, (DWORD)this, szDescription);
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventActionListener * pListener  - �¼���Ϊִ����
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CBox::UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_BOX, (DWORD)this);	
}
//------------------------------------------------------------------------
// ��  ��: ���ı��������Ӧ�¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventResponseListener * pListener - �¼���Ӧ��
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CBox::SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_BOX, (DWORD)this, szDescription);
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventResponseListener * pListener  - �¼���Ӧ��
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CBox::UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_BOX, (DWORD)this);	
}
//------------------------------------------------------------------------
void CBox::Accept(IMapItemVisitor &visitor)
{
	visitor.VisitBox();
}
//------------------------------------------------------------------------
CMapItemView* CBox::GetMapItem() const
{
	return m_pMapItemView;
}
//------------------------------------------------------------------------
void CBox::SetMapItem(CMapItemView * pMapItemView)
{
	m_pMapItemView = pMapItemView;
}
//------------------------------------------------------------------------
LPCSTR CBox::GetName() const
{
	return m_Property.szName;
}
//------------------------------------------------------------------------
BOOL CBox::Open()
{
	// ����������ʹ�����
	char buf[256];
	int nLen = sizeof(buf);
	CDataArchive archive(buf, nLen);
	CS_ACTION_HEAD actionhead;
	
	actionhead.m_wKeyRoot = CMD_ROOT_THING;
	actionhead.m_wKeyMain = ROOTTHING_CS_MAIN_ACTION;
	actionhead.m_wKeySub  = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_OPENBOX);
	
	archive.Write(&actionhead, sizeof(CS_ACTION_HEAD));
	archive.Write(&m_uid, sizeof(UID));

	DWORD dwState = BOX_OPENING;
	archive.Write(&dwState, sizeof(DWORD));	
	
	// ����
	g_pGlobal->m_pAppNetworkLink->Send(buf, archive.GetBufferOffset());
	return TRUE;	
}
//------------------------------------------------------------------------
void CBox::Close()
{
	// ����������ʹ�����
	char buf[256];
	int nLen = sizeof(buf);
	CDataArchive archive(buf, nLen);
	CS_ACTION_HEAD actionhead;
	
	actionhead.m_wKeyRoot = CMD_ROOT_THING;
	actionhead.m_wKeyMain = ROOTTHING_CS_MAIN_ACTION;
	actionhead.m_wKeySub  = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_OPENBOX);
	
	archive.Write(&actionhead, sizeof(CS_ACTION_HEAD));
	archive.Write(&m_uid, sizeof(UID));

	DWORD dwState = BOX_CLOSED;
	archive.Write(&dwState, sizeof(DWORD));
	
	// ����
	g_pGlobal->m_pAppNetworkLink->Send(buf, archive.GetBufferOffset());	
}
//------------------------------------------------------------------------
void CBox::Respond(LPCSTR pBuf, int nLen)
{
	// ���ڴ�״̬+��ʱ��
	// ��״̬+�����ĸ���+����1id+����1��С+����2id+����2��С+...
	// �ر�״̬	
	if (pBuf == NULL || nLen < sizeof(DWORD))
	{
		return;
	}

	// ״̬(��¼�ķ�������״̬)
	DWORD dwState = *(DWORD*)pBuf;
	pBuf += sizeof(DWORD);
	nLen -= sizeof(DWORD);
	
	switch (dwState)
	{
	case BOX_OPENING:
		{
			// ׼����
			m_Property.dwState = BOX_OPENING;
			DWORD dwOpenTime = *(DWORD*)pBuf;
			PrepareOpen(dwOpenTime);
			return;
		}
		
	case BOX_OPENED:
		// ��
		m_Property.dwState = BOX_OPENED;
		OpenBox(pBuf, nLen);
		return;
		
	case BOX_CLOSED:
		// �ر�
		m_Property.dwState = BOX_CLOSED;
		CloseBox();
		return;
		
	case BOX_COMMAND:
		Command(pBuf, nLen);
		return;
		
	default:
		return;
	}
}
//------------------------------------------------------------------------
DWORD CBox::GetType() const
{
	return m_Property.dwTypeId;
}
//------------------------------------------------------------------------
DWORD CBox::GetID() const
{
	return m_Property.dwBoxId;
}
//------------------------------------------------------------------------
void CBox::PrepareOpen(DWORD dwOpenTime)
{
	// ����һ�������Ϣ
	IEventServer* pEventServer = g_pThingClientFacade->GetEventServer();
	if (pEventServer == NULL)
	{
		return;
	}
	
	SBOX_CONTEXT context;
	context.uidBox = m_uid;
	context.dwState = BOX_OPENING;
	
	WORD wEventID = MAKE_MSGID(MAINACTION_CIRCULTYPE_SS, MAINACTION_MSGID_OPENBOX);	
	if (!pEventServer->FireVote((BYTE*)&context, sizeof(SBOX_CONTEXT), wEventID, SRC_TYPE_ID_ACTOR, (DWORD)g_pGlobal->m_pPlayer))
	{
		return;
	}
	
	// ���ý�����ʾ
	g_pGlobal->m_pWndMgr->OnEvent(WND_TREASUREBOX, WE_TREASURE_PROGRESSBAR_SHOW, dwOpenTime, (LPARAM)&m_uid);
}
//------------------------------------------------------------------------
void CBox::OpenBox(LPCSTR pBuf, int nLen)
{
	/*
	if (pBuf == NULL || nLen < sizeof(int))
	{
		return;
	}
	
	// ����һ�������Ϣ
	IEventServer* pEventServer = g_pThingClientFacade->GetEventServer();
	if (pEventServer == NULL)
	{
		return;
	}
	
	SBOX_CONTEXT context;
	context.uidBox = m_uid;
	context.dwState = BOX_OPENED;
	
	WORD wEventID = MAKE_MSGID(MAINACTION_CIRCULTYPE_SS, MAINACTION_MSGID_OPENBOX);	
	if (!pEventServer->FireVote((BYTE*)&context, sizeof(SBOX_CONTEXT), wEventID, SRC_TYPE_ID_ACTOR, (DWORD)g_pGlobal->m_pPlayer))
	{
		return;
	}

	// ��֮ǰ�����
	m_objects.clear();

	// �����ĸ���+����1id+����1��С+����2id+����2��С+...
	int nContainer = *(int*)pBuf;
	if (nContainer <= 0)
	{
		return;
	}

	pBuf += sizeof(int);
	nLen -= sizeof(int);

	// �õ�����������
	IContainerMgr* pContainerMgr = g_pGlobal->GetContainerMgr();
	if (pContainerMgr == NULL)
	{
		return;
	}

	// Ҫ�򿪵�����
	IContainer* pCurContainer = NULL;
	IContainer* pContainer = NULL;

	for (int i = 0; i < nContainer; ++i)
	{
		// ����id
		DWORD dwContainerId = *(int*)pBuf;
		pBuf += sizeof(DWORD);
		nLen -= sizeof(DWORD);

		// ������С
		int nSize = *(int*)pBuf;
		pBuf += sizeof(int);
		nLen -= sizeof(int);
		
		// ��������
		pContainer = pContainerMgr->GetContainer(dwContainerId);
		if (pContainer == NULL)
		{
			SContainerCreateContext context;
			context.dwID = dwContainerId;
			context.nMaxSize = nSize;
			context.dwType = GOODS_TYPE_REALY;
			context.bCanAdd = FALSE;

			pContainer = pContainerMgr->CreateContainer((LPCSTR)&context, sizeof(SContainerCreateContext));
			if (pContainer == NULL)
			{
				continue;
			}
			PrintEventLn("�������� = %d", pContainer->GetID());
		}

		if (pCurContainer == NULL)
		{
			pCurContainer = pContainer;
		}

		// �����������
		m_objects.push_back(dwContainerId);
	}

	// ��
	if (pCurContainer != NULL)
	{
		pCurContainer->Open();

		// ���ý�����ʾ
		g_pGlobal->m_pWndMgr->OnEvent(WND_TREASUREBOX, WE_TREASURE_OPEN, pCurContainer->GetID(), (LPARAM)&m_uid);
		
		// ����һ����Ӧ��Ϣ
		pEventServer->FireResponse((BYTE*)&context, sizeof(SBOX_CONTEXT), wEventID, SRC_TYPE_ID_ACTOR, (DWORD)g_pGlobal->m_pPlayer);
	}

	*/
}
//------------------------------------------------------------------------
void CBox::CloseBox()
{
	g_pGlobal->m_pWndMgr->OnEvent(WND_TREASUREBOX, WE_TREASURE_CLOSE, 0, (LPARAM)&m_uid);

	vector<DWORD>::iterator it = m_objects.begin();
	if (it == m_objects.end())
	{
		return;
	}
/*
	IContainerMgr* pContainerMgr = g_pThingClientFacade->GetContainerMgr();
	if (pContainerMgr == NULL)
	{
		return;
	}

	IContainer* pContainer = pContainerMgr->GetContainer(*it);
	if (pContainer == NULL)
	{
		return;
	}

	if (   pContainer->GetGoodsNumber() == 0
		|| m_Property.dwTypeId == BOX_TYPE_MINE
		|| m_Property.dwTypeId == BOX_TYPE_MEDICINE)
	{
		m_objects.erase(it);
		pContainer->Release();
	}
	*/
}
//------------------------------------------------------------------------
void CBox::ClearBox()
{
	/*
	IContainerMgr* pContainerMgr = g_pThingClientFacade->GetContainerMgr();
	if (pContainerMgr == NULL)
	{
		return;
	}
	
	for (vector<DWORD>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		// ���� id
		DWORD dwContainerId = *it;
		
		// �õ�����
		IContainer* pContainer = pContainerMgr->GetContainer(dwContainerId);
		if (pContainer == NULL)
		{
			continue;
		}
		
		for (int i = 0; i < pContainer->GetSize(); ++i)
		{
			IContainerGoods* pContainerGoods = pContainer->GetGoods(i);
			if (pContainerGoods == NULL)
			{
				continue;
			}
			
			UID uidGoods = pContainerGoods->GetUID();
			IGoods* pGoods = (IGoods*)g_pThingClientFacade->GetThing(uidGoods);
			SAFE_RELEASE(pGoods);
		}
		
		pContainer->Release();
	}

	m_objects.clear();
	*/
}
//------------------------------------------------------------------------
void CBox::SetRight(BOOL bRight)
{
	CCreatureViewWrapper viewwrapper(m_pMapItemView);
	m_pMapItemView->OnCommand(AtcCmdSetProperty, AtcPropShowStar, (DWORD)&bRight);
	if (bRight)
	{
		viewwrapper.AddFlag(CMapItemView::flagCanbeSelected);		
	}
	else
	{
		viewwrapper.RemoveFlag(CMapItemView::flagCanbeSelected);
	}
}
//------------------------------------------------------------------------
BOOL CBox::IsCanOpen(void)
{
	return g_BoxPopedomManager.CanOpen(m_uid, g_pGlobal->m_pPlayer->GetUID());
}
//------------------------------------------------------------------------
void CBox::Command(LPCSTR pBuf, int nLen)
{
	if (nLen < sizeof(DWORD))
	{
		return;
	}
	
	DWORD dwCmd = *(DWORD*)pBuf;
	pBuf += sizeof(DWORD);
	nLen -= sizeof(DWORD);

	switch(dwCmd)
	{
	case BOX_COMMAND_CLEARRIGHT:
		// Ȩ��ʱ�䵽������������Ȩ��
		g_BoxPopedomManager.ClearBox(m_uid);
		SetRight(TRUE);
		break;

	case BOX_COMMAND_ADDRIGHT:
		g_BoxPopedomManager.AddMaster(m_uid, g_pGlobal->m_pPlayer->GetActorDBID());
		SetRight(TRUE);
		break;

	case BOX_COMMAND_DELRIGHT:
		g_BoxPopedomManager.DelMaster(m_uid, g_pGlobal->m_pPlayer->GetActorDBID());
		SetRight(FALSE);
		break;
	}
}
//------------------------------------------------------------------------