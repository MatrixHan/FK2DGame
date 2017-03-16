/**
*	created:		2012-7-3   23:04
*	filename: 		CValuables
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		Ԫ��
*/
//------------------------------------------------------------------------
#include "../Include/ThingClientCommon.h"
#include "../Include/CValuables.h"
#include "../Include/CThingClientFacade.h"
//------------------------------------------------------------------------
CValuables::CValuables(void)
{
	// ��ͨ���Գ�ʼ��
	m_uid = INVALID_UID;
	ZeroMemory(&m_Property, sizeof(SVALUABLES_PROPERTY_PRIVATE));
	ZeroMemory(m_szName, sizeof(m_szName));

	// �߼��������ͳ�ʼ��
	m_ThingClass.SetThingClass(THINGCLASS_GOODS);
	m_ThingClass.SetValuables();
}
//------------------------------------------------------------------------
CValuables::~CValuables(void)
{

}
//------------------------------------------------------------------------
void CValuables::Release(void)
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

	delete this;
}
//------------------------------------------------------------------------
BOOL CValuables::Create(void)
{
	IConfigDataCenter * pDataCenter = g_pThingClientFacade->GetConfigDataCenter();
	if(pDataCenter == NULL)
	{
		return FALSE;
	}

    GOODS_BASEPROPERTY & goodsbase = pDataCenter->GetGoodsBaseProperty(m_Property.dwGoodsID);
	if(goodsbase.lGoodsID == 0)
	{
		TraceLn("�޷�ȡ����Ʒ��Ϣ��������Ʒʧ�ܣ�%d", m_Property.dwGoodsID);
		return FALSE;
	}
	lstrcpyn(m_szName, goodsbase.szName, sizeof(m_szName));

	return TRUE;	
}
//------------------------------------------------------------------------
void CValuables::SetUID(UID uid)
{
	m_uid = uid;
}
//------------------------------------------------------------------------
UID	CValuables::GetUID(void)
{
	return m_uid;
}
//------------------------------------------------------------------------
DWORD CValuables::GetMapID(void)
{
	return GETUIDSERVERID(m_uid);
}
//------------------------------------------------------------------------
void CValuables::SetLoc(POINTS ptLoc)
{

}
//------------------------------------------------------------------------
POINTS CValuables::GetLoc(void)
{
	POINTS ptLoc = { 0, 0 };
	return ptLoc;
}
//------------------------------------------------------------------------
IThingClass * CValuables::GetThingClass(void)
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
BOOL CValuables::OnSetPrivateContext(LPCSTR buf, int nLen)
{
	if(nLen != sizeof(SWARRAN_PROPERTY_PRIVATE))
	{
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SWARRAN_PROPERTY_PRIVATE));

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CValuables::OnGetPrivateContext(LPSTR buf, int &nLen)
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
BOOL CValuables::OnSetPublicContext(LPCSTR buf, int nLen)
{
	if(nLen != sizeof(SVALUABLES_PROPERTY_PUBLIC))
	{
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SVALUABLES_PROPERTY_PUBLIC));

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CValuables::OnGetPublicContext(LPSTR buf, int &nLen)
{
	if(nLen < sizeof(SVALUABLES_PROPERTY_PUBLIC))
	{
		return FALSE;
	}

	memcpy(buf, &m_Property, sizeof(SVALUABLES_PROPERTY_PUBLIC));
	nLen = sizeof(SVALUABLES_PROPERTY_PUBLIC);

	return TRUE;
}
//------------------------------------------------------------------------
//// ����thing�����ݿ��ֳ�
//// ˵����
//// OnGetDBContext() ���ڽ������ֳ����������ݿ�
//// OnSetDBContext() ����ͨ�����ݿⷵ�ص����ݴ�������
BOOL CValuables::OnSetDBContext(LPCSTR buf, int nLen)
{
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CValuables::OnGetDBContext(LPSTR buf, int &nLen)
{	
	return TRUE;
}
//------------------------------------------------------------------------
//// ��Ϣ֪ͨ, ��ʹ�߼����������ת
void CValuables::OnMessage(DWORD dwMsg, LPCSTR buf, int nLen)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return;
	}

	/*
	// ���ж�����Ƿ�ͨ��
	if(pEventServer->FireVote((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_GOODS, (DWORD)this) == FALSE)
	{
		return;
	}

	// Ȼ����Ϊ�¼�
	pEventServer->FireAction((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_GOODS, (DWORD)this);
	*/
}
//------------------------------------------------------------------------
//// ��  ����������Ӧ��FireResponse��
void CValuables::OnResponse(DWORD dwMsg, LPCSTR buf, int nLen)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return;
	}

	/*
	// ������Ӧ
	pEventServer->FireResponse((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_GOODS, (DWORD)this);
	*/
}
//------------------------------------------------------------------------
//// ��  ��: ���ı�����ķ���¼�
//// ��  ��: [����] DWORD dwMsg - �¼�ID
//// ��  ��: [����] IEventVoteListener * pListener - �¼������
//// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
//// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CValuables::SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}

	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_GOODS, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
//// ��  ����ȡ�����ı�����ķ���¼�,����delete pListenerָ��
//// ��  ����[����] DWORD dwMsg  - �¼�ID
//// ��  ����[����] IEventVoteListener * pListener  - �¼������	
//// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CValuables::UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}

	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_GOODS, (DWORD)this);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
//// ��  ��: ���ı��������Ϊ�¼�
//// ��  ��: [����] DWORD dwMsg - �¼�ID
//// ��  ��: [����] IEventActionListener * pListener - �¼���Ϊִ����
//// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
//// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
/////////////////////////////////////////////////////////	
BOOL CValuables::SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_GOODS, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
//// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
//// ��  ����[����] DWORD dwMsg  - �¼�ID
//// ��  ����[����] IEventActionListener * pListener  - �¼���Ϊִ����
//// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CValuables::UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}

	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_GOODS, (DWORD)this);	
	*/
	return TRUE;
}
//------------------------------------------------------------------------
//// ��  ��: ���ı��������Ӧ�¼�
//// ��  ��: [����] DWORD dwMsg - �¼�ID
//// ��  ��: [����] IEventResponseListener * pListener - �¼���Ӧ��
//// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
//// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CValuables::SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_GOODS, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
//// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
//// ��  ����[����] DWORD dwMsg  - �¼�ID
//// ��  ����[����] IEventResponseListener * pListener  - �¼���Ӧ��
//// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CValuables::UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_GOODS, (DWORD)this);	
	*/
	return TRUE;
}
//------------------------------------------------------------------------
int CValuables::OnCommand(DWORD dwCommand, LPCSTR buf, int nLen)
{
	return 0;
}
//------------------------------------------------------------------------
DWORD CValuables::GetGoodsID(void)
{
	return m_Property.dwGoodsID;
}
//------------------------------------------------------------------------
LPCSTR CValuables::GetName(void)
{
	return (LPCSTR)m_szName;
}
//------------------------------------------------------------------------
int CValuables::GetPropNum(DWORD dwPropID)
{
	switch(dwPropID)
	{
	case GOODS_PROPID_NUMBER:
		{
			return m_Property.dwNumber;
		}
		break;
	default:
		{
			PrintEventLn("error : CValuables::GetPropNum : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return 0;	
}
//------------------------------------------------------------------------
LPCSTR CValuables::GetPropStr(DWORD dwPropID)
{
	switch(dwPropID)
	{
	case GOODS_PROPID_NAME:
		{
			return m_szName;
		}
		break;
	default:
		{
			PrintEventLn("error : CValuables::GetStrProp : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return "";
}
//------------------------------------------------------------------------
DWORD CValuables::GetNumber(void)
{
	return m_Property.dwNumber;
}
//------------------------------------------------------------------------
void CValuables::Accept(IMapItemVisitor &visitor)
{
	visitor.VisitGoods();
}
//------------------------------------------------------------------------
CMapItemView * CValuables::GetMapItem(void) const
{
	return NULL;
}
//------------------------------------------------------------------------
void CValuables::SetMapItem(CMapItemView * pMapItemView)
{

}
//------------------------------------------------------------------------
DWORD CValuables::GetUseLock(void)
{
	return m_Property.dwUseLock;
}
//------------------------------------------------------------------------
BOOL CValuables::GetBindFlag(DWORD dwFlag)
{
	return (m_Property.dwUseBind & dwFlag) == dwFlag ? TRUE : FALSE;
}
//------------------------------------------------------------------------