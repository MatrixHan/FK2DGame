/**
*	created:		2012-7-3   1:10
*	filename: 		CChunnel
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��ͼͨ��
*/
//------------------------------------------------------------------------
#include "../Include/ThingClientCommon.h"
#include "../Include/CChunnel.h"
#include "../Include/CThingClientFacade.h"
#include "../Include/CreatureViewWrapper.h"
//------------------------------------------------------------------------
CChunnel::CChunnel(void)
{
	// ��ͨ���Գ�ʼ��
	m_uid = INVALID_UID;
	m_pItemView = NULL;
	memset(&m_Property, 0, sizeof(m_Property));

	// �߼��������ͳ�ʼ��
	m_ThingClass.SetThingClass(THINGCLASS_CHUNNEL);
}
//------------------------------------------------------------------------
CChunnel::~CChunnel(void)
{

}
//------------------------------------------------------------------------
void CChunnel::Release(void)
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
	if(g_pThingClientFacade->GetThingTable()->Remove(m_uid) == FALSE)
	{
		TraceLn("��ThingTable�����ʧ�ܣ�%s", GetName());
	}

	// ���MapItem
	if(m_pItemView != NULL)
	{
		POINTS _ptLoc = GetLoc();
		POINT  ptLoc = { _ptLoc.x, _ptLoc.y };
		if(g_pThingClientFacade->GetMapView()->RemoveItem(ptLoc, m_pItemView) == FALSE)
		{
			TraceLn("���MapItemʧ�ܣ�%s[%d, %d]", GetName(), ptLoc.x, ptLoc.y);
		}

		// ????��Ϊbasicsysδ��FCL���޷��ͷ�
		SAFE_RELEASE(m_pItemView);
	}

	delete this;
}
//------------------------------------------------------------------------
BOOL CChunnel::Create(void)
{
	IItemClassFactory * pItemClassFactory = g_pThingClientFacade->GetItemFactory();
	if(pItemClassFactory == NULL)
	{
		return FALSE;
	}
	
	// ������ͼ����
	m_pItemView = (CMapItemView *)pItemClassFactory->CreateItem(miAniObj_Transprot, (LPCSTR)&m_Property.dwResID, sizeof(DWORD));
	if(m_pItemView == NULL)
	{
		return FALSE;
	}
	m_pItemView->SetItemData((DWORD)(this));

	// ���������������
	CCreatureViewWrapper viewwrapper(m_pItemView);
	viewwrapper.AddToMap(m_Property.ptLoc);
	viewwrapper.SetTile(m_Property.ptLoc);
	viewwrapper.AddFlag(CMapItemView::flagCanbeSelected);
	viewwrapper.AddFlag(CMapItemView::flagPaoPao);
	viewwrapper.SetFlagColorOnMinmap(255, 255, 0);

	return TRUE;
}
//------------------------------------------------------------------------
void CChunnel::SetUID(UID uid)
{
	m_uid = uid;
}
//------------------------------------------------------------------------
UID	CChunnel::GetUID(void)
{
	return m_uid;
}
//------------------------------------------------------------------------
DWORD CChunnel::GetMapID(void)
{
	return GETUIDSERVERID(m_uid);
}
//------------------------------------------------------------------------
void CChunnel::SetLoc(POINTS ptLoc)
{
	m_Property.ptLoc = ptLoc;
}
//------------------------------------------------------------------------
POINTS CChunnel::GetLoc(void)
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
IThingClass * CChunnel::GetThingClass(void)
{
	return static_cast<IThingClass *>(&m_ThingClass);
}
//------------------------------------------------------------------------
// ˽�ж����ֳ�����
// ˵����
// ˽�ж����ֳ���ָ�û��Լ��Ľ�ɫ��װ���ȵ���ϸ��Ϣ���������ǿ�������
// ��dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ���
// �磺�û���¼ʱ��������ȡ����������ֳ�
// �ͻ��˸���dwClassID�������󣬵���OnGetPrivateContext()���ö����ֳ�
BOOL CChunnel::OnSetPrivateContext(LPCSTR buf, int nLen)
{
	if(buf == NULL || nLen != sizeof(SCHUNNEL_PROPERTY_LIST))
	{
		TraceLn("CChunnel::OnSetPrivateContext Failed! [buf == NULL]");
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SCHUNNEL_PROPERTY_LIST));

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CChunnel::OnGetPrivateContext(LPSTR buf, int &nLen)
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
BOOL CChunnel::OnSetPublicContext(LPCSTR buf, int nLen)
{
	return OnSetPrivateContext(buf, nLen);
}
//------------------------------------------------------------------------
BOOL CChunnel::OnGetPublicContext(LPSTR buf, int &nLen)
{	
	// �ͻ��˲�������������
	return FALSE;
}
//------------------------------------------------------------------------
// ����thing�����ݿ��ֳ�
// ˵����
// OnGetDBContext() ���ڽ������ֳ����������ݿ�
// OnSetDBContext() ����ͨ�����ݿⷵ�ص����ݴ�������
BOOL CChunnel::OnSetDBContext(LPCSTR buf, int nLen)
{	
	return FALSE;
}
//------------------------------------------------------------------------
BOOL CChunnel::OnGetDBContext(LPSTR buf, int &nLen)
{
	return FALSE;
}
//------------------------------------------------------------------------
int CChunnel::OnCommand(DWORD dwCommand, LPCSTR buf, int nLen)
{
	return 0;
}
//------------------------------------------------------------------------
void CChunnel::OnMessage(DWORD dwMsg, LPCSTR buf, int nLen)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return;
	}

	/*
	// ���ж�����Ƿ�ͨ��
	if(pEventServer->FireVote((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_CHUNNEL, (DWORD)this) == FALSE)
	{
		return;
	}

	// Ȼ����Ϊ�¼�
	pEventServer->FireAction((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_CHUNNEL, (DWORD)this);
	*/
}
//------------------------------------------------------------------------
// ��  ����������Ӧ
void CChunnel::OnResponse(DWORD dwMsg, LPCSTR buf, int nLen)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return;
	}

	/*
	// ������Ӧ
	pEventServer->FireResponse((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_CHUNNEL, (DWORD)this);
	*/
}
//------------------------------------------------------------------------
// ��  ��: ���ı�����ķ���¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventVoteListener * pListener - �¼������
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CChunnel::SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}

	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_CHUNNEL, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ı�����ķ���¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventVoteListener * pListener  - �¼������	
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CChunnel::UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}

	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_CHUNNEL, (DWORD)this);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ��: ���ı��������Ϊ�¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventActionListener * pListener - �¼���Ϊִ����
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CChunnel::SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_CHUNNEL, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventActionListener * pListener  - �¼���Ϊִ����
BOOL CChunnel::UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_CHUNNEL, (DWORD)this);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ��: ���ı��������Ӧ�¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventResponseListener * pListener - �¼���Ӧ��
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CChunnel::SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_CHUNNEL, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventResponseListener * pListener  - �¼���Ӧ��
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CChunnel::UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_CHUNNEL, (DWORD)this);	
	*/
	return TRUE;
}
//------------------------------------------------------------------------
LPCSTR CChunnel::GetName(void)
{
	return m_Property.szName;
}
//------------------------------------------------------------------------
void CChunnel::Accept(IMapItemVisitor &visitor)
{
	visitor.VisitChunnel();
}
//------------------------------------------------------------------------
CMapItemView * CChunnel::GetMapItem(void) const
{
	return m_pItemView;
}
//------------------------------------------------------------------------
void CChunnel::SetMapItem(CMapItemView * pMapItemView)
{
	m_pItemView = pMapItemView;
}
//------------------------------------------------------------------------
DWORD CChunnel::GetTransMode(void)
{
	return m_Property.dwTransMode;
}
//------------------------------------------------------------------------
SCHUNNEL_TARGET * CChunnel::GetTargetInfor(DWORD &dwCount)
{
	dwCount = m_Property.dwTargetCount;
	return m_Property.TargetInfor;
}
//------------------------------------------------------------------------