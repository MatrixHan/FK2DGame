/**
*	created:		2012-7-4   1:38
*	filename: 		CMedicament
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ҩƷ
*/
//------------------------------------------------------------------------
#include "../Include/ThingClientCommon.h"
#include "../Include/CMedicament.h"
#include "../Include/CThingClientFacade.h"
//------------------------------------------------------------------------
CMedicament::CMedicament(void)
{
	// ��ͨ���Գ�ʼ��
	m_uid = INVALID_UID;
	ZeroMemory(&m_Property, sizeof(SMEDICAMENT_PROPERTY_PRIVATE));
	ZeroMemory(m_szName, sizeof(m_szName));

	// �߼��������ͳ�ʼ��
	m_ThingClass.SetThingClass(THINGCLASS_GOODS);
	m_ThingClass.SetMedicament();	
}
//------------------------------------------------------------------------
CMedicament::~CMedicament(void)
{

}
//------------------------------------------------------------------------
void CMedicament::Release(void)
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
		TraceLn("��ThingTable�����ʧ�ܣ�%s", GetName());
	}

	delete this;
}
//------------------------------------------------------------------------
BOOL CMedicament::Create(void)
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
void CMedicament::SetUID(UID uid)
{
	m_uid = uid;
}
//------------------------------------------------------------------------
UID	CMedicament::GetUID(void)
{
	return m_uid;
}
//------------------------------------------------------------------------
DWORD CMedicament::GetMapID(void)
{
	return GETUIDSERVERID(m_uid);
}
//------------------------------------------------------------------------
void CMedicament::SetLoc(POINTS ptLoc)
{

}
//------------------------------------------------------------------------
POINTS CMedicament::GetLoc(void)
{
	POINTS ptLoc = { 0, 0 };
	return ptLoc;
}
//------------------------------------------------------------------------
IThingClass * CMedicament::GetThingClass(void)
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
BOOL CMedicament::OnSetPrivateContext(LPCSTR buf, int nLen)
{
	if(nLen != sizeof(SMEDICAMENT_PROPERTY_PRIVATE))
	{
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SMEDICAMENT_PROPERTY_PRIVATE));

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMedicament::OnGetPrivateContext(LPSTR buf, int &nLen)
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
BOOL CMedicament::OnSetPublicContext(LPCSTR buf, int nLen)
{
	if(nLen != sizeof(SMEDICAMENT_PROPERTY_PUBLIC))
	{
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SMEDICAMENT_PROPERTY_PUBLIC));

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMedicament::OnGetPublicContext(LPSTR buf, int &nLen)
{
	if(nLen < sizeof(SMEDICAMENT_PROPERTY_PUBLIC))
	{
		return FALSE;
	}

	memcpy(buf, &m_Property, sizeof(SMEDICAMENT_PROPERTY_PUBLIC));
	nLen = sizeof(SMEDICAMENT_PROPERTY_PUBLIC);

	return TRUE;
}
//------------------------------------------------------------------------
//// ����thing�����ݿ��ֳ�
//// ˵����
//// OnGetDBContext() ���ڽ������ֳ����������ݿ�
//// OnSetDBContext() ����ͨ�����ݿⷵ�ص����ݴ�������
BOOL CMedicament::OnSetDBContext(LPCSTR buf, int nLen)
{
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMedicament::OnGetDBContext(LPSTR buf, int &nLen)
{	
	return TRUE;
}
//------------------------------------------------------------------------
//// ��Ϣ֪ͨ, ��ʹ�߼����������ת
void CMedicament::OnMessage(DWORD dwMsg, LPCSTR buf, int nLen)
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
void CMedicament::OnResponse(DWORD dwMsg, LPCSTR buf, int nLen)
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
BOOL CMedicament::SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32])
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
BOOL CMedicament::UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener)
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
BOOL CMedicament::SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32])
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
BOOL CMedicament::UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener)
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
BOOL CMedicament::SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32])
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
BOOL CMedicament::UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener)
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
int CMedicament::OnCommand(DWORD dwCommand, LPCSTR buf, int nLen)
{
	return 0;
}
//------------------------------------------------------------------------
DWORD CMedicament::GetGoodsID(void)
{
	return m_Property.dwGoodsID;
}
//------------------------------------------------------------------------
LPCSTR CMedicament::GetName(void)
{
	return (LPCSTR)m_szName;
}
//------------------------------------------------------------------------
int CMedicament::GetPropNum(DWORD dwPropID)
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
			PrintEventLn("error : CMedicament::GetPropNum : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return 0;	
}
//------------------------------------------------------------------------
LPCSTR CMedicament::GetPropStr(DWORD dwPropID)
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
			PrintEventLn("error : CMedicament::GetStrProp : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return "";
}
//------------------------------------------------------------------------
DWORD CMedicament::GetNumber(void)
{
	return m_Property.dwNumber;
}
//------------------------------------------------------------------------
void CMedicament::Accept(IMapItemVisitor &visitor)
{
	visitor.VisitGoods();
}
//------------------------------------------------------------------------
CMapItemView * CMedicament::GetMapItem(void) const
{
	return NULL;
}
//------------------------------------------------------------------------
void CMedicament::SetMapItem(CMapItemView * pMapItemView)
{

}
//------------------------------------------------------------------------
BYTE * CMedicament::GetExtendBuf(int &nLen)
{
	nLen = sizeof(m_Property.byExtend);
	return m_Property.byExtend;
}
//------------------------------------------------------------------------
DWORD CMedicament::GetUseLock(void)
{
	return m_Property.dwUseLock;
}
//------------------------------------------------------------------------
BOOL CMedicament::GetBindFlag(DWORD dwFlag)
{
	return (m_Property.dwUseBind & dwFlag) == dwFlag ? TRUE : FALSE;
}
//------------------------------------------------------------------------