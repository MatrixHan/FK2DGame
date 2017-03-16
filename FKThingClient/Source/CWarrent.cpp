/**
*	created:		2012-7-3   23:11
*	filename: 		CWarrent
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ƾ֤
*/
//------------------------------------------------------------------------
#include "../Include/ThingClientCommon.h"
#include "../Include/CWarrent.h"
#include "../Include/CThingClientFacade.h"
//------------------------------------------------------------------------
CWarrant::CWarrant(void)
{
	// ��ͨ���Գ�ʼ��
	m_uid = INVALID_UID;
	ZeroMemory(&m_Property, sizeof(SWARRAN_PROPERTY_PRIVATE));
	ZeroMemory(m_szName, sizeof(m_szName));
	ZeroMemory(m_Effect, sizeof(m_Effect));

	// �߼��������ͳ�ʼ��
	m_ThingClass.SetThingClass(THINGCLASS_GOODS);
	m_ThingClass.SetWarrant();
}
//------------------------------------------------------------------------
CWarrant::~CWarrant(void)
{

}
//------------------------------------------------------------------------
void CWarrant::Release(void)
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
BOOL CWarrant::Create(void)
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
void CWarrant::SetUID(UID uid)
{
	m_uid = uid;
}
//------------------------------------------------------------------------
UID	CWarrant::GetUID(void)
{
	return m_uid;
}
//------------------------------------------------------------------------
DWORD CWarrant::GetMapID(void)
{
	return GETUIDSERVERID(m_uid);
}
//------------------------------------------------------------------------
void CWarrant::SetLoc(POINTS ptLoc)
{

}
//------------------------------------------------------------------------
POINTS CWarrant::GetLoc(void)
{
	POINTS ptLoc = { 0, 0 };
	return ptLoc;
}
//------------------------------------------------------------------------
IThingClass * CWarrant::GetThingClass(void)
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

BOOL CWarrant::OnSetPrivateContext(LPCSTR buf, int nLen)
{
	if(nLen != sizeof(SWARRAN_PROPERTY_PRIVATE))
	{
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SWARRAN_PROPERTY_PRIVATE));

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CWarrant::OnGetPrivateContext(LPSTR buf, int &nLen)
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
BOOL CWarrant::OnSetPublicContext(LPCSTR buf, int nLen)
{
	if(nLen != sizeof(SWARRAN_PROPERTY_PUBLIC))
	{
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SWARRAN_PROPERTY_PUBLIC));

	// �������Ч��
	ClearEffect();

	// ����Ч��
	IEffectFactory * pEffectFactory = g_pThingClientFacade->GetEffectFactory();
	if(pEffectFactory == NULL)
	{
		return FALSE;
	}
	
	for(int i = 0; i < GOODS_EFFECT_MAXCOUNT; i++)
	{
		if(m_Property.wEffect[i] != 0)
		{
			if(pEffectFactory->CreateEffect(m_Property.wEffect[i], &m_Effect[i]) == FALSE)
			{
				char szTraceBuf[256];
				sprintf(szTraceBuf, "�޷�����%dЧ����%sװ������ʧ�ܣ�", m_Property.wEffect[i], GetName());
				TraceLn(szTraceBuf);				
			}
		}
	}

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CWarrant::OnGetPublicContext(LPSTR buf, int &nLen)
{
	if(nLen < sizeof(SWARRAN_PROPERTY_PUBLIC))
	{
		return FALSE;
	}

	memcpy(buf, &m_Property, sizeof(SWARRAN_PROPERTY_PUBLIC));
	nLen = sizeof(SWARRAN_PROPERTY_PUBLIC);

	return TRUE;
}
//------------------------------------------------------------------------
//// ����thing�����ݿ��ֳ�
//// ˵����
//// OnGetDBContext() ���ڽ������ֳ����������ݿ�
//// OnSetDBContext() ����ͨ�����ݿⷵ�ص����ݴ�������
BOOL CWarrant::OnSetDBContext(LPCSTR buf, int nLen)
{
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CWarrant::OnGetDBContext(LPSTR buf, int &nLen)
{	
	return TRUE;
}
//------------------------------------------------------------------------
//// ��Ϣ֪ͨ, ��ʹ�߼����������ת
void CWarrant::OnMessage(DWORD dwMsg, LPCSTR buf, int nLen)
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
void CWarrant::OnResponse(DWORD dwMsg, LPCSTR buf, int nLen)
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
BOOL CWarrant::SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32])
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
BOOL CWarrant::UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener)
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
BOOL CWarrant::SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32])
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
BOOL CWarrant::UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener)
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
BOOL CWarrant::SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32])
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
BOOL CWarrant::UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener)
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
int CWarrant::OnCommand(DWORD dwCommand, LPCSTR buf, int nLen)
{
	return 0;
}
//------------------------------------------------------------------------
DWORD CWarrant::GetGoodsID(void)
{
	return m_Property.dwGoodsID;
}
//------------------------------------------------------------------------
LPCSTR CWarrant::GetName(void)
{
	return (LPCSTR)m_szName;
}
//------------------------------------------------------------------------
int CWarrant::GetPropNum(DWORD dwPropID)
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
			PrintEventLn("error : CWarrant::GetPropNum : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return 0;	
}
//------------------------------------------------------------------------
LPCSTR CWarrant::GetPropStr(DWORD dwPropID)
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
			PrintEventLn("error : CWarrant::GetStrProp : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return "";
}
//------------------------------------------------------------------------
DWORD CWarrant::GetNumber(void)
{
	return m_Property.dwNumber;
}
//------------------------------------------------------------------------
void CWarrant::Accept(IMapItemVisitor &visitor)
{
	visitor.VisitGoods();
}
//------------------------------------------------------------------------
CMapItemView * CWarrant::GetMapItem(void) const
{
	return NULL;
}
//------------------------------------------------------------------------
void CWarrant::SetMapItem(CMapItemView * pMapItemView)
{

}
//------------------------------------------------------------------------
DWORD CWarrant::GetUseLock(void)
{
	return m_Property.dwUseLock;
}
//------------------------------------------------------------------------
BOOL CWarrant::GetBindFlag(DWORD dwFlag)
{
	return (m_Property.dwUseBind & dwFlag) == dwFlag ? TRUE : FALSE;
}
//------------------------------------------------------------------------
void CWarrant::ClearEffect(void)
{
	for(int i = 0; i < GOODS_EFFECT_MAXCOUNT; i++)
	{
		SAFE_RELEASE(m_Effect[i]);
	}
}
//------------------------------------------------------------------------
IEffect * CWarrant::GetEffect(int nIndex)
{
	if(nIndex < 0 || nIndex >= GOODS_EFFECT_MAXCOUNT)
	{
		return NULL;
	}

	return m_Effect[nIndex];	
}
//------------------------------------------------------------------------