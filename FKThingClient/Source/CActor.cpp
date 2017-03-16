/**
*	created:		2012-7-3   1:22
*	filename: 		CActor
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		������
*/
//------------------------------------------------------------------------
#include <windows.h>
#include <mmsystem.h>
#include "../Include/ThingClientCommon.h"
#include "../Include/CActor.h"
#include "../Include/CThingClientFacade.h"
#include "../Include/CreatureViewWrapper.h"
//------------------------------------------------------------------------
CActor::CActor(void)
{
	// ��ͨ���Գ�ʼ��
	m_uid = INVALID_UID;
	m_pItemView = NULL;
	m_pItemHead = NULL;
	m_bIsDying = FALSE;
	m_dwOldHeadResID = 0;
	ZeroMemory(m_ThingPart, sizeof(m_ThingPart));
	ZeroMemory(&m_Property, sizeof(SACTOR_PROPERTY_PRIVATE));

	// �߼��������ͳ�ʼ��
	m_ThingClass.SetThingClass(THINGCLASS_ACTOR);
	m_ThingClass.SetCreature();
}
//------------------------------------------------------------------------
CActor::~CActor(void)
{

}
//------------------------------------------------------------------------
void CActor::Release(void)
{	
	// ��һ�¼���ȥ,����ɾ��
	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_RELEASE);
	OnMessage(wMsgID, NULL, 0);

	// �ͷŲ���
	for(int i = 0; i < THINGPARTID_THING_MAXID; i++)
	{
		SAFE_RELEASE(m_ThingPart[i]);
	}

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

	// �ͷ�ͷ��
	SAFE_RELEASE(m_pItemHead);

	// ���MapItem
	if(m_pItemView != NULL)
	{
		POINTS _ptLoc = GetLoc();
		POINT  ptLoc = { _ptLoc.x, _ptLoc.y };
		if(g_pThingClientFacade->GetMapView()->RemoveItem(ptLoc, m_pItemView) == FALSE)
		{
			TraceLn("���MapItemʧ�ܣ�%s[%d, %d]", GetName(), ptLoc.x, ptLoc.y);
		}

		SAFE_RELEASE(m_pItemView);
	}

	delete this;
}
//------------------------------------------------------------------------
BOOL CActor::Create(void)
{
	// ������ͼ����
	struct stTemp
	{
		int				nType;		// ����ID	
		ICreatureSink * pCrtSink;	// 
	};

	// ȡ���Ա��Ա�ȷ�����Σ���׼�����λ��׼Ů���Σ�
	int nSex = GetPropNum(CREATURE_PROPID_SEX);	
	stTemp tempdata;
	tempdata.nType = nSex == ACTOR_SEX_MAN ? MAN_CREATURE_TYPE : FEMALE_CREATURE_TYPE;
	tempdata.pCrtSink = static_cast<ICreatureSink *>(this);
	IItemClassFactory * pItemClassFactory = g_pThingClientFacade->GetItemFactory();
	if(pItemClassFactory == NULL)
	{
		return FALSE;
	}

	if(m_Property.ptLoc.x == 0 || m_Property.ptLoc.y == 0)
	{
		TraceLn( "%s����ʧ�ܣ�����Ϊ�����꣡", GetName());
		return FALSE;
	}
	
	int nDataLen = sizeof(tempdata);
	m_pItemView = (CMapItemView *)pItemClassFactory->CreateItem(miActorObj, (const char *)&tempdata, nDataLen);
	if(m_pItemView == NULL)
	{
		return FALSE;
	}

	// ָ�������ڲ����Ҳ���ͼ��ʱ�����ض���ͼ������
	IConfigDataCenter * pConfigDataCenter = g_pThingClientFacade->GetConfigDataCenter();
	if(pConfigDataCenter == NULL)
	{
		return FALSE;
	}

	int nOccupation = GetPropNum(CREATURE_PROPID_OCCUPATION);
	CCreatureViewWrapper viewwrapper(m_pItemView);
	for(int i = 0; i < CrtPartMax; i++)
	{
		DWORD dwResID = pConfigDataCenter->GetNudePartForActor(nOccupation, i, nSex);
		viewwrapper.SpecialModule(i, dwResID);
	}

	// ˢ������
	UpdateAppearance();
	
	// ���������������
	viewwrapper.SetData((DWORD)this);
	viewwrapper.AddToMap(m_Property.ptLoc);
	viewwrapper.SetTile(m_Property.ptLoc);
	viewwrapper.SetName(GetName());
	viewwrapper.SetNameColor(m_Property.nNameColor);
	viewwrapper.AddFlag(CMapItemView::flagCanbeSelected);
	viewwrapper.SetDirection(m_Property.nDir);
	viewwrapper.SetFlagColorOnMinmap(0, 255, 255);

	int nHPCur = GetPropNum(CREATURE_PROPID_HPCUR);
	int nHPMax = GetPropNum(CREATURE_PROPID_HPMAXCUR);
	viewwrapper.SetBloodInfo(GetPropNum(CREATURE_PROPID_HPCUR), max(nHPCur, nHPMax));

	viewwrapper.AddFlag(CMapItemView::flagCanbeSelected);
	viewwrapper.SetMoveSpeedLevel((int *)&m_Property.nMoveSpeedLevel);
	
	return TRUE;
}
//------------------------------------------------------------------------
void CActor::SetUID(UID uid)
{
	m_uid = uid;
}
//------------------------------------------------------------------------
UID	CActor::GetUID(void)
{
	return m_uid;
}
//------------------------------------------------------------------------
DWORD CActor::GetMapID(void)
{
	return GETUIDSERVERID(m_uid);
}
//------------------------------------------------------------------------
void CActor::SetLoc(POINTS ptLoc)
{
	m_Property.ptLoc = ptLoc;
}
//------------------------------------------------------------------------
POINTS CActor::GetLoc(void)
{
	CCreatureViewWrapper viewwrapper(m_pItemView);
	
	POINT pos = viewwrapper.GetTile();
	POINTS loc = { pos.x, pos.y };

	return loc;
}
//------------------------------------------------------------------------
IThingClass * CActor::GetThingClass(void)
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
BOOL CActor::OnSetPrivateContext(LPCSTR buf, int nLen)
{
	if(buf == NULL || nLen != sizeof(SACTOR_PROPERTY_PUBLIC))
	{
		TraceLn("CActor::OnSetPrivateContext Failed! [nLen != sizeof(SACTOR_PROPERTY_PUBLIC)]");
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SACTOR_PROPERTY_PUBLIC));
	
	m_Property.nMoveSpeedLevel = m_Property.nMoveSpeedLevel < 0 ? 0 : m_Property.nMoveSpeedLevel;
	m_Property.nMoveSpeedLevel = m_Property.nMoveSpeedLevel >= MAX_MOVESPEED_LEVEL ? MAX_MOVESPEED_LEVEL - 1 : m_Property.nMoveSpeedLevel;

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CActor::OnGetPrivateContext(LPSTR buf, int &nLen)
{
	return FALSE;
}
//------------------------------------------------------------------------
// ���󹫿��ֳ�����
// ˵����
// �����ֳ�Ϊ����������GameWorld�е���������鿴�Ķ�����Ϣ�����ɫ����HP��װ����״��
// ������ȡ�ö����ֳ�����dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ��ˣ�
// �ͻ��˸���dwThingClassID�������󣬵���OnSetContext()���ö����ֳ�
BOOL CActor::OnSetPublicContext(LPCSTR buf, int nLen)
{
	return OnSetPrivateContext(buf, nLen);
}
//------------------------------------------------------------------------
BOOL CActor::OnGetPublicContext(LPSTR buf, int &nLen)
{	
	return FALSE;
}
//------------------------------------------------------------------------
// ����thing�����ݿ��ֳ�
// ˵����
// OnGetDBContext() ���ڽ������ֳ����������ݿ�
// OnSetDBContext() ����ͨ�����ݿⷵ�ص����ݴ�������
BOOL CActor::OnSetDBContext(LPCSTR buf, int nLen)
{	
	return FALSE;
}
//------------------------------------------------------------------------
BOOL CActor::OnGetDBContext(LPSTR buf, int &nLen)
{
	return FALSE;
}
//------------------------------------------------------------------------
int CActor::OnCommand(DWORD dwCommand, LPCSTR buf, int nLen)
{
	return 0;
}
//------------------------------------------------------------------------
void CActor::OnMessage(DWORD dwMsg, LPCSTR buf, int nLen)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return;
	}

	/*
	// ���ж�����Ƿ�ͨ��
	if(pEventServer->FireVote((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_ACTOR, (DWORD)this) == FALSE)
	{
		return;
	}

	// Ȼ����Ϊ�¼�
	pEventServer->FireAction((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_ACTOR, (DWORD)this);
	*/
}
//------------------------------------------------------------------------
// ��  ����������Ӧ
void CActor::OnResponse(DWORD dwMsg, LPCSTR buf, int nLen)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return;
	}

	/*
	// ������Ӧ
	pEventServer->FireResponse((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_ACTOR, (DWORD)this);
	*/
}
//------------------------------------------------------------------------
// ��  ��: ���ı�����ķ���¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventVoteListener * pListener - �¼������
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CActor::SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}

	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_ACTOR, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ı�����ķ���¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventVoteListener * pListener  - �¼������	
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CActor::UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}

	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_ACTOR, (DWORD)this);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ��: ���ı��������Ϊ�¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventActionListener * pListener - �¼���Ϊִ����
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CActor::SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_ACTOR, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventActionListener * pListener  - �¼���Ϊִ����
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CActor::UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_ACTOR, (DWORD)this);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ��: ���ı��������Ӧ�¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventResponseListener * pListener - �¼���Ӧ��
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CActor::SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_ACTOR, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventResponseListener * pListener  - �¼���Ӧ��
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CActor::UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_ACTOR, (DWORD)this);	
	*/
	return TRUE;
}
//------------------------------------------------------------------------
LPCSTR CActor::GetName(void)
{
	return GetPropStr(CREATURE_PROPID_NAME);
}
//------------------------------------------------------------------------
int	CActor::SetPropNum(DWORD dwPropID, int nValue)
{
	switch(dwPropID)
	{
		// �ȼ�
	case CREATURE_PROPID_LEVEL:
		{
			m_Property.nLevel = nValue;
		}
		break;
		// ����
	case CREATURE_PROPID_PHYLE:
		{
			m_Property.nPhyle = nValue;
		}
		break;
		// ְҵ
	case CREATURE_PROPID_OCCUPATION:
		{
			m_Property.nOccupation = nValue;
		}
		break;
		// ��ǰ����ֵ
	case CREATURE_PROPID_HPCUR:
		{
			m_Property.nHPCur = nValue;
		}
		break;
		// ��ǰ�������ֵ
	case CREATURE_PROPID_HPMAXCUR:
		{
			m_Property.nHPMaxCur = nValue;
		}
		break;
		// ��ǰ����ֵ	
	case CREATURE_PROPID_MPCUR:
		{
			m_Property.nMPCur = nValue;
		}
		break;
		// ��ǰ�����ֵ
	case CREATURE_PROPID_MPMAXCUR:
		{
			m_Property.nMPMaxCur = nValue;
		}
		break;
		// PKֵ
	case CREATURE_PROPID_PKVALUE:
		{
			m_Property.nPKValue = nValue;
		}
		break;
		// ��ǰ��·��ʽ
	case CREATURE_PROPID_WALKSTYLE:
		{
			m_Property.nWalkStyle = nValue;
		}
		break;
		// �ƶ��ٶȵȼ�
	case CREATURE_PROPID_MOVESPEEDLEVEL:
		{
			m_Property.nMoveSpeedLevel = nValue;
			m_Property.nMoveSpeedLevel = m_Property.nMoveSpeedLevel < 0 ? 0 : m_Property.nMoveSpeedLevel;
			m_Property.nMoveSpeedLevel = m_Property.nMoveSpeedLevel >= MAX_MOVESPEED_LEVEL ? MAX_MOVESPEED_LEVEL - 1 : m_Property.nMoveSpeedLevel;	
		}
		break;
		// װ��������ID
	case CREATURE_PROPID_EQUIPCONTAINERID:
		{
			m_Property.nEquipContainerID = nValue;
		}
		break;
		// ����ͷ��
	case CREATURE_PROPID_HEADRESID:
		{
			m_Property.nHeadResID = nValue;
		}
		break;
		// ������ɫ
	case CREATURE_PROPID_NAMECOLOR:
		{
			m_Property.nNameColor = nValue;
		}
		break;
		// ����ID
	case CREATURE_PROPID_CLANID:
		{
			m_Property.nClanID = nValue;
		}
		break;
	default:
		{
			PrintEventLn("error : CActor::GetPropNum : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return 0;
}
//------------------------------------------------------------------------
int CActor::GetPropNum(DWORD dwPropID)
{
	switch(dwPropID)
	{
		// �Ա� ��1 ���У�2 ��Ů��
	case CREATURE_PROPID_SEX:
		{
			return m_Property.nSex;
		}
		break;
		// �ȼ�
	case CREATURE_PROPID_LEVEL:
		{
			return m_Property.nLevel;
		}
		break;
		// ����
	case CREATURE_PROPID_PHYLE:
		{
			return m_Property.nPhyle;
		}
		break;
		// ְҵ
	case CREATURE_PROPID_OCCUPATION:
		{
			return m_Property.nOccupation;
		}
		break;
		// ��ǰ����ֵ
	case CREATURE_PROPID_HPCUR:
		{
			return m_Property.nHPCur;
		}
		break;
		// ��ǰ�������ֵ
	case CREATURE_PROPID_HPMAXCUR:
		{
			return m_Property.nHPMaxCur;
		}
		break;
		// ��ǰ����ֵ	
	case CREATURE_PROPID_MPCUR:
		{
			return m_Property.nMPCur;
		}
		break;
		// ��ǰ�����ֵ
	case CREATURE_PROPID_MPMAXCUR:
		{
			return m_Property.nMPMaxCur;
		}
		break;
		// UserID
	case CREATURE_PROPID_USERID:
		{
			return m_Property.nUserDBID;
		}
		break;
		// ActorID
	case CREATURE_PROPID_ACTORID:
		{
			return m_Property.nActorDBID;
		}
		break;
		// װ��������ID
	case CREATURE_PROPID_EQUIPCONTAINERID:
		{
			return m_Property.nEquipContainerID;
		}
		break;
		// PKֵ
	case CREATURE_PROPID_PKVALUE:
		{
			return m_Property.nPKValue;
		}
		break;
		// ��ǰ��·��ʽ
	case CREATURE_PROPID_WALKSTYLE:
		{
			return m_Property.nWalkStyle;
		}
		break;
		// �ƶ��ٶȵȼ�
	case CREATURE_PROPID_MOVESPEEDLEVEL:
		{
			return  m_Property.nMoveSpeedLevel;
		}
		break;
		// ����ͷ��
	case CREATURE_PROPID_HEADRESID:
		{
			return m_Property.nHeadResID;
		}
		break;
		// ������ɫ
	case CREATURE_PROPID_NAMECOLOR:
		{
			return m_Property.nNameColor;
		}
		break;
		// ����ID
	case CREATURE_PROPID_CLANID:
		{
			return m_Property.nClanID;
		}
		break;
		// ����
	case CREATURE_PROPID_DIR:
		{
			return m_Property.nDir;
		}
		break;
	default:
		{
			PrintEventLn("error : CActor::GetPropNum : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return 0;
}
//------------------------------------------------------------------------
LPCSTR CActor::SetPropStr(DWORD dwPropID, LPCSTR strPropValue)
{
	switch(dwPropID)
	{
		// 1  = ����
	case CREATURE_PROPID_NAME:
		{
			lstrcpyn(m_Property.szName, strPropValue, sizeof(m_Property.szName));
			return m_Property.szName;
		}
		break;
	default:
		{
			PrintEventLn("error : CActor::GetStrProp : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return "";
}
//------------------------------------------------------------------------
LPCSTR CActor::GetPropStr(DWORD dwPropID)
{
	switch(dwPropID)
	{
		// 1  = ����
	case CREATURE_PROPID_NAME:
		{
			return m_Property.szName;
		}
		break;
	default:
		{
			PrintEventLn("error : CActor::GetStrProp : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return "";
}
//------------------------------------------------------------------------
BOOL CActor::AddPart(IThingPart * pPart)
{
	if(pPart == NULL)
	{
		return FALSE;
	}

	if(pPart->GetPartID() < 0 || pPart->GetPartID() >= THINGPARTID_THING_MAXID)
	{
		return FALSE;
	}

	if(m_ThingPart[pPart->GetPartID()] != NULL)
	{
		return FALSE;
	}

	m_ThingPart[pPart->GetPartID()] = pPart;

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CActor::RemovePart(DWORD dwPartID)
{
	if(dwPartID < 0 || dwPartID >= THINGPARTID_THING_MAXID)
	{
		return FALSE;
	}

	m_ThingPart[dwPartID] = NULL;

	return TRUE;
}
//------------------------------------------------------------------------
IThingPart * CActor::GetPart(DWORD dwPartID)
{
	if(dwPartID < 0 || dwPartID >= THINGPARTID_THING_MAXID)
	{
		return NULL;
	}

	return m_ThingPart[dwPartID];	
}
//------------------------------------------------------------------------
DWORD CActor::GetUserDBID(void)
{
	return m_Property.nUserDBID;
}
//------------------------------------------------------------------------
DWORD CActor::GetActorDBID(void)
{
	return m_Property.nActorDBID;
}
//------------------------------------------------------------------------
DWORD CActor::GetActorClanID(void)
{
	return m_Property.nClanID;
}
//------------------------------------------------------------------------
BOOL CActor::GotoActState(DWORD dwActStateID, LPCSTR pContext, int nLen)
{
	return FALSE;
}
//------------------------------------------------------------------------
ICreatureActState * CActor::GetCurActState(void)
{
	return NULL;
}
//------------------------------------------------------------------------
void CActor::Accept(IMapItemVisitor &visitor)
{
	visitor.VisitActor();
}
//------------------------------------------------------------------------
CMapItemView * CActor::GetMapItem(void) const
{
	return m_pItemView;
}
//------------------------------------------------------------------------
void CActor::SetMapItem(CMapItemView * pMapItemView)
{
	m_pItemView = pMapItemView;
}
//------------------------------------------------------------------------
void CActor::OnCreatureMoveToTileCenter(const POINTS &ptCurTile, const POINTS &ptNextTile)
{

}
//------------------------------------------------------------------------
BOOL CActor::UpdateAppearance(void)
{
	IConfigDataCenter * pConfigDataCenter = g_pThingClientFacade->GetConfigDataCenter();
	if(pConfigDataCenter == NULL)
	{
		return FALSE;
	}

	int nOccupation = GetPropNum(CREATURE_PROPID_OCCUPATION);
	int nSex = GetPropNum(CREATURE_PROPID_SEX);
	CCreatureViewWrapper viewwrapper(m_pItemView);
	for(int i = 0; i < CrtPartMax; i++)
	{
		DWORD dwResID = pConfigDataCenter->GetNudePartForActor(nOccupation, i, nSex);
		viewwrapper.ChangeModule(i, dwResID);
	}

	return TRUE;	
}
//------------------------------------------------------------------------
BOOL CActor::AddCreatureCommand(ICreatureCommand * pCreatureCommand)
{
	return FALSE;
}
//------------------------------------------------------------------------
IBitmap * CActor::GetHeadBitmap(void)
{	
	if(m_pItemHead == NULL || m_dwOldHeadResID != m_Property.nHeadResID)
	{
		// �ͷž�ͷ��
		SAFE_RELEASE(m_pItemHead);

		// ������ͷ��
		IItemClassFactory * pItemClassFactory = g_pThingClientFacade->GetItemFactory();
		if(pItemClassFactory == NULL)
		{
			return NULL;
		}

		IConfigDataCenter * pDataCenter = g_pThingClientFacade->GetConfigDataCenter();
		if(pDataCenter == NULL)
		{
			return NULL;
		}
		DWORD dwTargetResID = pDataCenter->GetHeadResIDByIndex(m_Property.nHeadResID, ACTORHEAD_RESTYPE_TARGET);

		m_pItemHead = (CMapItemView *)pItemClassFactory->CreateItem(miStaticObj_Picture, (char*)&dwTargetResID, sizeof(DWORD));
		if(m_pItemHead == NULL)
		{
			return NULL;
		}

		m_dwOldHeadResID = m_Property.nHeadResID;
	}	

	return m_pItemHead->GetBitmap();
}
//------------------------------------------------------------------------
void CActor::SetDying(BOOL bDying)
{
	m_bIsDying = bDying;
}
//------------------------------------------------------------------------
BOOL CActor::IsDying(void)
{
	return m_bIsDying;
}
//------------------------------------------------------------------------