/**
*	created:		2012-7-3   23:22
*	filename: 		CStall
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		̯λ
*/
//------------------------------------------------------------------------
#include "../Include/ThingClientCommon.h"
#include "../Include/CStall.h"
#include "../Include/CThingClientFacade.h"
#include "../Include/CreatureViewWrapper.h"
//------------------------------------------------------------------------
IBitmap * CStall::m_pSloganBmpRes = NULL;
//------------------------------------------------------------------------
// ����ͼƬ����
#define STALLSLOGAN_RES_FILENAME "StallSlogan.bmp"
//------------------------------------------------------------------------
CStall::CStall(void)
{
	// ��ͨ���Գ�ʼ��
	m_uid = INVALID_UID;
	m_pItemView = NULL;
	memset(&m_Property, 0, sizeof(m_Property));

	// �߼��������ͳ�ʼ��
	m_ThingClass.SetThingClass(THINGCLASS_STALL);
}
//------------------------------------------------------------------------
CStall::~CStall(void)
{

}
//------------------------------------------------------------------------
void CStall::Release(void)
{
	// ��һ�¼���ȥ,����ɾ��
	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_RELEASE);
	OnMessage(wMsgID, NULL, 0);

	if(g_pThingClientFacade->GetThingTable() == NULL)
	{
		delete this;
		return;
	}

	if(g_pThingClientFacade->GetMapView() == NULL)
	{
		delete this;
		return;
	}
		
	// ��ThingTable�����
	if(!g_pThingClientFacade->GetThingTable()->Remove(m_uid))
	{
		TraceLn("��ThingTable�����ʧ�ܣ�̯λ��");
	}

	// ���MapItem
	if(m_pItemView != NULL)
	{
		POINTS _ptLoc = GetLoc();
		POINT  ptLoc = { _ptLoc.x, _ptLoc.y };
		if(!g_pThingClientFacade->GetMapView()->RemoveItem(ptLoc, m_pItemView))
		{
			TraceLn("���MapItemʧ�ܣ�̯λ��[%d, %d]", ptLoc.x, ptLoc.y);
		}

		if(m_pSloganBmp != NULL)
		{
			CImageLoader::GetInstance()->FreeImage(m_pSloganBmp);
			m_pSloganBmp = NULL;
		}

		// ????��Ϊbasicsysδ��FCL���޷��ͷ�
		SAFE_RELEASE(m_pItemView);
	}

	delete this;
}
//------------------------------------------------------------------------
BOOL CStall::Create(void)
{	
	IItemClassFactory * pItemClassFactory = g_pThingClientFacade->GetItemFactory();
	if(pItemClassFactory == NULL)
	{
		return FALSE;
	}
	
	// ������ͼ����
	m_pItemView = (CMapItemView *)pItemClassFactory->CreateItem(miStaticObj_Booth, (LPCSTR)&m_Property.dwResID, sizeof(DWORD));
	if(m_pItemView == NULL)
	{
		TraceLn("Error! CStall::Create() : Create MapItemView Failed!");
		return FALSE;
	}
	m_pItemView->SetItemData((DWORD)(this));

	// ����
	if(m_pSloganBmpRes == NULL)
	{
		m_pSloganBmpRes = static_cast<IBitmap *>(CImageLoader::GetInstance()->LoadImage(STALLSLOGAN_RES_FILENAME));
		if(m_pSloganBmpRes == NULL)
		{
			return FALSE;
		}
	}
	SetSloganBmp();	
	
	// �����趨
	m_pItemView->OnCommand(AtcCmdSetProperty, AtcPropBkImage, (DWORD)&m_pSloganBmp);
	m_pItemView->OnCommand(AtcCmdSetProperty, AtcPropName, (DWORD)&m_Property.szName[0]);
	WPixel clr = WPixel(0, 255, 0);
	m_pItemView->OnCommand(AtcCmdSetProperty, AtcPropNameColor, (DWORD)&clr);
	m_pItemView->AddFlag(CMapItemView::flagName);	
	m_pItemView->AddFlag(CMapItemView::flagCanbeSelected);	

	// ������ͼ
	POINT ptTile = { m_Property.ptLoc.x, m_Property.ptLoc.y };
	g_pThingClientFacade->GetMapView()->AddItem(ptTile, m_pItemView);	

	return TRUE;
}
//------------------------------------------------------------------------
void CStall::SetUID(UID uid)
{
	m_uid = uid;
}
//------------------------------------------------------------------------
UID	CStall::GetUID(void)
{
	return m_uid;
}
//------------------------------------------------------------------------
DWORD CStall::GetMapID(void)
{
	return GETUIDSERVERID(m_uid);
}
//------------------------------------------------------------------------
void CStall::SetLoc(POINTS ptLoc)
{
	m_Property.ptLoc = ptLoc;
}
//------------------------------------------------------------------------
POINTS CStall::GetLoc(void)
{
	POINTS ptLoc = { 0, 0 };
	if(m_pItemView != NULL)
	{
		POINT _ptLoc = m_pItemView->GetTile();
		ptLoc.x = _ptLoc.x;
		ptLoc.y = _ptLoc.y;
	}

	return ptLoc;
}
//------------------------------------------------------------------------
IThingClass * CStall::GetThingClass(void)
{
	return static_cast<IThingClass *>(&m_ThingClass);
}
//------------------------------------------------------------------------
//// ˽�ж����ֳ�����
//// ˵����
//// ˽�ж����ֳ���ָ�û��Լ��Ľ�ɫ��װ���ȵ���ϸ��Ϣ���������ǿ�������
//// ��dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ���
//// �磺�û���¼ʱ��������ȡ����������ֳ�
//// �ͻ��˸���dwClassID�������󣬵���OnGetPrivateContext()���ö����ֳ�
BOOL CStall::OnSetPrivateContext(LPCSTR buf, int nLen)
{
	if(buf == NULL || nLen != sizeof(SSTALL_PROPERTY_LIST))
	{
		TraceLn("CStall::OnSetPrivateContext Failed! [buf == NULL]");
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SSTALL_PROPERTY_LIST));

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CStall::OnGetPrivateContext(LPSTR buf, int &nLen)
{
	// �ͻ��˲�������������
	return FALSE;
}
//------------------------------------------------------------------------
//// ���󹫿��ֳ�����
//// ˵����
//// �����ֳ�Ϊ����������GameWorld�е���������鿴�Ķ�����Ϣ�����ɫ����HP��װ����״��
//// ������ȡ�ö����ֳ�����dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ��ˣ�
//// �ͻ��˸���dwThingClassID�������󣬵���OnSetContext()���ö����ֳ�
BOOL CStall::OnSetPublicContext(LPCSTR buf, int nLen)
{
	return OnSetPrivateContext(buf, nLen);
}
//------------------------------------------------------------------------
BOOL CStall::OnGetPublicContext(LPSTR buf, int &nLen)
{	
	// �ͻ��˲�������������
	return FALSE;
}
//------------------------------------------------------------------------
//// ����thing�����ݿ��ֳ�
//// ˵����
//// OnGetDBContext() ���ڽ������ֳ����������ݿ�
//// OnSetDBContext() ����ͨ�����ݿⷵ�ص����ݴ�������
BOOL CStall::OnSetDBContext(LPCSTR buf, int nLen)
{	
	return FALSE;
}
//------------------------------------------------------------------------
BOOL CStall::OnGetDBContext(LPSTR buf, int &nLen)
{
	return FALSE;
}
//------------------------------------------------------------------------
int CStall::OnCommand(DWORD dwCommand, LPCSTR buf, int nLen)
{
	return 0;
}
//------------------------------------------------------------------------
//// ��Ϣ֪ͨ, ��ʹ�߼����������ת
void CStall::OnMessage(DWORD dwMsg, LPCSTR buf, int nLen)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return;
	}

	/*
	// ���ж�����Ƿ�ͨ��
	if(!pEventServer->FireVote((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_STALL, (DWORD)this))
	{
		return;
	}

	// Ȼ����Ϊ�¼�
	pEventServer->FireAction((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_STALL, (DWORD)this);
	*/
}
//------------------------------------------------------------------------
//// ��  ����������Ӧ
void CStall::OnResponse(DWORD dwMsg, LPCSTR buf, int nLen)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return;
	}

	/*
	// ������Ӧ
	pEventServer->FireResponse((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_STALL, (DWORD)this);
	*/
}
//------------------------------------------------------------------------
//// ��  ��: ���ı�����ķ���¼�
//// ��  ��: [����] DWORD dwMsg - �¼�ID
//// ��  ��: [����] IEventVoteListener * pListener - �¼������
//// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
//// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CStall::SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}

	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_STALL, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
//// ��  ����ȡ�����ı�����ķ���¼�,����delete pListenerָ��
//// ��  ����[����] DWORD dwMsg  - �¼�ID
//// ��  ����[����] IEventVoteListener * pListener  - �¼������	
//// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CStall::UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}

	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_STALL, (DWORD)this);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
//// ��  ��: ���ı��������Ϊ�¼�
//// ��  ��: [����] DWORD dwMsg - �¼�ID
//// ��  ��: [����] IEventActionListener * pListener - �¼���Ϊִ����
//// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
//// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CStall::SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_STALL, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
//// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
//// ��  ����[����] DWORD dwMsg  - �¼�ID
//// ��  ����[����] IEventActionListener * pListener  - �¼���Ϊִ����
//// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CStall::UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_STALL, (DWORD)this);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
//// ��  ��: ���ı��������Ӧ�¼�
//// ��  ��: [����] DWORD dwMsg - �¼�ID
//// ��  ��: [����] IEventResponseListener * pListener - �¼���Ӧ��
//// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
//// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE	
BOOL CStall::SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_STALL, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
//// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
//// ��  ����[����] DWORD dwMsg  - �¼�ID
//// ��  ����[����] IEventResponseListener * pListener  - �¼���Ӧ��
//// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CStall::UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_STALL, (DWORD)this);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
void CStall::Accept(IMapItemVisitor &visitor)
{
	visitor.VisitStall();
}
//------------------------------------------------------------------------
CMapItemView * CStall::GetMapItem(void) const
{
	return m_pItemView;
}
//------------------------------------------------------------------------
void CStall::SetMapItem(CMapItemView * pMapItemView)
{
	m_pItemView = pMapItemView;
}
//------------------------------------------------------------------------
void CStall::SetSloganBmp(void)
{
	DWORD w = g_pGlobal->m_pDefFontWithOutline->GetLength(m_Property.szName) + 8;
	DWORD h = m_pSloganBmpRes->GetHeight();

	if(!g_pGlobal->m_CanvasHelper.CreateIBitmap(&m_pSloganBmp))
	{
		m_pSloganBmp = NULL;
		return;
	}
	m_pSloganBmp->Create(w, h);
	
	m_pSloganBmp->Draw(0, 0, 4, h, m_pSloganBmpRes, 0, 0);
	int len = 4;
	while(len < w)
	{
		m_pSloganBmp->Draw(len, 0, 13, h, m_pSloganBmpRes, 4, 0);
		len += 13;
	}
	m_pSloganBmp->Draw(w - 4, 0, 4, h, m_pSloganBmpRes, m_pSloganBmpRes->GetWidth()-4, 0);
}
//------------------------------------------------------------------------
UID CStall::GetMaster(void)
{
	return m_Property.uidMaster;
}
//------------------------------------------------------------------------