/**
*	created:		2012-7-3   22:54
*	filename: 		CEquipment
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		װ��
*/
//------------------------------------------------------------------------
#include "../Include/ThingClientCommon.h"
#include "../Include/CEquipment.h"
#include "../Include/CThingClientFacade.h"
#include "../../FKGame/Effect/IEffectClient.h"
//------------------------------------------------------------------------
CEquipment::CEquipment(void)
{
	// ��ͨ���Գ�ʼ��
	m_uid = INVALID_UID;
	ZeroMemory(&m_Property, sizeof(SEQUIPMENT_PROPERTY_PRIVATE));
	ZeroMemory(m_Effect, sizeof(m_Effect));
	ZeroMemory(m_szName, sizeof(m_szName));

	// �߼��������ͳ�ʼ��
	m_ThingClass.SetThingClass(THINGCLASS_GOODS);
	m_ThingClass.SetEquipment();	
}
//------------------------------------------------------------------------
CEquipment::~CEquipment(void)
{

}
//------------------------------------------------------------------------
void CEquipment::Release(void)
{
	// ��һ�¼���ȥ,����ɾ��
	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_RELEASE);
	OnMessage(wMsgID, NULL, 0);

	// �������Ч��
	ClearEffect();

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
BOOL CEquipment::Create(void)
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
void CEquipment::SetUID(UID uid)
{
	m_uid = uid;
}
//------------------------------------------------------------------------
UID	CEquipment::GetUID(void)
{
	return m_uid;
}
//------------------------------------------------------------------------
DWORD CEquipment::GetMapID(void)
{
	return GETUIDSERVERID(m_uid);
}
//------------------------------------------------------------------------
void CEquipment::SetLoc(POINTS ptLoc)
{

}
//------------------------------------------------------------------------
POINTS CEquipment::GetLoc(void)
{
	POINTS ptLoc = { 0, 0 };
	return ptLoc;
}
//------------------------------------------------------------------------
IThingClass * CEquipment::GetThingClass(void)
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
BOOL CEquipment::OnSetPrivateContext(LPCSTR buf, int nLen)
{
	if(nLen != sizeof(SEQUIPMENT_PROPERTY_PRIVATE))
	{
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SEQUIPMENT_PROPERTY_PRIVATE));

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CEquipment::OnGetPrivateContext(LPSTR buf, int &nLen)
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
BOOL CEquipment::OnSetPublicContext(LPCSTR buf, int nLen)
{
	if(nLen != sizeof(SEQUIPMENT_PROPERTY_PUBLIC))
	{
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SEQUIPMENT_PROPERTY_PUBLIC));

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
				TraceLn( szTraceBuf);				
			}
		}
	}

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CEquipment::OnGetPublicContext(LPSTR buf, int &nLen)
{
	if(nLen < sizeof(SEQUIPMENT_PROPERTY_PUBLIC))
	{
		return FALSE;
	}

	memcpy(buf, &m_Property, sizeof(SEQUIPMENT_PROPERTY_PUBLIC));
	nLen = sizeof(SEQUIPMENT_PROPERTY_PUBLIC);

	return TRUE;
}
//------------------------------------------------------------------------
// ����thing�����ݿ��ֳ�
// ˵����
// OnGetDBContext() ���ڽ������ֳ����������ݿ�
// OnSetDBContext() ����ͨ�����ݿⷵ�ص����ݴ�������
BOOL CEquipment::OnSetDBContext(LPCSTR buf, int nLen)
{
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CEquipment::OnGetDBContext(LPSTR buf, int &nLen)
{	
	return TRUE;
}
//------------------------------------------------------------------------
// ��Ϣ֪ͨ, ��ʹ�߼����������ת
void CEquipment::OnMessage(DWORD dwMsg, LPCSTR buf, int nLen)
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
// ��  ����������Ӧ��FireResponse��
void CEquipment::OnResponse(DWORD dwMsg, LPCSTR buf, int nLen)
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
// ��  ��: ���ı�����ķ���¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventVoteListener * pListener - �¼������
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CEquipment::SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32])
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
// ��  ����ȡ�����ı�����ķ���¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventVoteListener * pListener  - �¼������	
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CEquipment::UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener)
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
// ��  ��: ���ı��������Ϊ�¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventActionListener * pListener - �¼���Ϊִ����
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CEquipment::SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32])
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
// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventActionListener * pListener  - �¼���Ϊִ����
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CEquipment::UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener)
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
// ��  ��: ���ı��������Ӧ�¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventResponseListener * pListener - �¼���Ӧ��
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CEquipment::SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32])
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
// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventResponseListener * pListener  - �¼���Ӧ��
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CEquipment::UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener)
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
// ��  ����ִ������
int CEquipment::OnCommand(DWORD dwCommand, LPCSTR buf, int nLen)
{
	return 0;
}
//------------------------------------------------------------------------
// ��  ����ȡ����ƷID
DWORD CEquipment::GetGoodsID(void)
{
	return m_Property.dwGoodsID;
}
//------------------------------------------------------------------------
// ȡ����������
LPCSTR CEquipment::GetName(void)
{
	return (LPCSTR)m_szName;
}
//------------------------------------------------------------------------
// ȡ�����������������
int CEquipment::GetPropNum(DWORD dwPropID)
{
	switch(dwPropID)
	{
	case GOODS_PROPID_NUMBER:
		{
			return m_Property.dwNumber;
		}
		break;
		// ��ǰ�;�
	case GOODS_PROPID_ENDURE_CUR:
		{
			return m_Property.nEndureCur;
		}
		break;
		// ��ǰ����;�
	case GOODS_PROPID_ENDURE_CURMAX:
		{
			return m_Property.nEndureCurMax;
		}
		break;
		// �����ȼ�
	case GOODS_PROPID_SMELTLEVEL:
		{
			return m_Property.nSmeltLevel;
		}
		break;
	default:
		{
			PrintEventLn("error : CEquipment::GetPropNum : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return 0;	
}
//------------------------------------------------------------------------
// ȡ��������ַ�������
LPCSTR CEquipment::GetPropStr(DWORD dwPropID)
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
			PrintEventLn("error : CEquipment::GetStrProp : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return "";
}
//------------------------------------------------------------------------
// ��  ����ȡ����Ʒ����
DWORD CEquipment::GetNumber(void)
{
	return m_Property.dwNumber;
}
//------------------------------------------------------------------------
// ��  ����ȡ��Ч��
IEffect * CEquipment::GetEffect(int nIndex)
{
	if(nIndex < 0 || nIndex >= GOODS_EFFECT_MAXCOUNT)
	{
		return NULL;
	}

	return m_Effect[nIndex];	
}
//------------------------------------------------------------------------
// ��  ����ȡ�þ����ȼ�
int	CEquipment::GetSmeltLevel(void)
{
	return m_Property.nSmeltLevel;
}
//------------------------------------------------------------------------
// ��  ����ȡ��װ���Ĳ����
DWORD CEquipment::GetSlotLeft(void)
{
	return m_Property.nSlotLeft;
}
//------------------------------------------------------------------------
// ��  ��������Ƿ���װ������װ��
BOOL CEquipment::CanEquip(ICreature * pCreature, int nEquipPos)
{
	if(pCreature == NULL)
	{
		return FALSE;
	}

	IConfigDataCenter * pConfigDataCenter = g_pThingClientFacade->GetConfigDataCenter();
	if(pConfigDataCenter == NULL)
	{
		return FALSE;
	}

	EQUIPMENT_BASEPROPERTY baseproperty = pConfigDataCenter->GetEquipmentBaseProperty(m_Property.dwGoodsID, m_Property.nSmeltLevel);
	if(baseproperty.lGoodsID == 0)
	{
		return FALSE;
	}

	// ����
	if(pCreature->GetThingClass()->GetThingClass() == THINGCLASS_ACTOR)
	{
		IActor * pActor = (IActor *)(pCreature);

		char szSystemMsg[256];
		// �ܷ�װ��
		BOOL bEquipFlag = GetBindFlag(GOODS_BINDFLAG_EQUIP);	
		if(!bEquipFlag)
		{
			sprintf(szSystemMsg, "%s�޷�װ����", baseproperty.szName);

			WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_SYSTEMMESSAGE);
			pActor->OnMessage(wMsgID, szSystemMsg, sizeof(szSystemMsg));
			return FALSE;
		}

		// ƥ��ȼ�
		int nLevel = pActor->GetPropNum(CREATURE_PROPID_LEVEL);
		if(nLevel < baseproperty.lAllowLevel)
		{
			sprintf(szSystemMsg, "���ȼ�������%sװ��ʧ�ܣ�", baseproperty.szName);

			WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_SYSTEMMESSAGE);
			pActor->OnMessage(wMsgID, szSystemMsg, sizeof(szSystemMsg));
			return FALSE;
		}

		// ƥ��ְҵ
		if(baseproperty.lAllowOccupation != 0)
		{
			int nOccupation = pActor->GetPropNum(CREATURE_PROPID_OCCUPATION);
			if(nOccupation != baseproperty.lAllowOccupation)
			{
				sprintf(szSystemMsg, "�޷�ƥ��ְҵ��%sװ��ʧ�ܣ�", baseproperty.szName);

				WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_SYSTEMMESSAGE);
				pActor->OnMessage(wMsgID, szSystemMsg, sizeof(szSystemMsg));
				return FALSE;
			}			
		}

		// ƥ��λ��
		if(baseproperty.lEquipPos1 != nEquipPos && baseproperty.lEquipPos2 != nEquipPos && baseproperty.lEquipPos3 != nEquipPos)
		{
			sprintf(szSystemMsg, "װ��λ�ô���%sװ��ʧ�ܣ�", baseproperty.szName);

			WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_SYSTEMMESSAGE);
			pActor->OnMessage(wMsgID, szSystemMsg, sizeof(szSystemMsg));
			return FALSE;
		}
	}

	return TRUE;
}
//------------------------------------------------------------------------
// ��  ����visitor
void CEquipment::Accept(IMapItemVisitor &visitor)
{
	visitor.VisitGoods();
}
//------------------------------------------------------------------------
// ȡ�õ�ͼ���ָ��
CMapItemView * CEquipment::GetMapItem(void) const
{
	return NULL;
}
//------------------------------------------------------------------------
// ��  ����������ͼ�ӿ�
void CEquipment::SetMapItem(CMapItemView * pMapItemView)
{

}
//------------------------------------------------------------------------
// ��  ����ȡ���;������ֵ
void CEquipment::GetEndure(int &nCur, int &nCurMax)
{
	nCur = m_Property.nEndureCur;
	nCurMax = m_Property.nEndureCurMax;
}
//------------------------------------------------------------------------
// ��  �����������Ч��
void CEquipment::ClearEffect(void)
{
	for(int i = 0; i < GOODS_EFFECT_MAXCOUNT; i++)
	{
		SAFE_RELEASE(m_Effect[i]);
	}
}
//------------------------------------------------------------------------
// ��  ����ȡ����Ʒ��ʹ������
DWORD CEquipment::GetUseLock(void)
{
	return m_Property.dwUseLock;
}
//------------------------------------------------------------------------
// ��  ����ȡ�ð󶨱�־�������鿴�󶨱�־
BOOL CEquipment::GetBindFlag(DWORD dwFlag)
{
	return (m_Property.dwUseBind & dwFlag) == dwFlag ? TRUE : FALSE;
}
//------------------------------------------------------------------------