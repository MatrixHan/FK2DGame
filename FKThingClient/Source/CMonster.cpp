/**
*	created:		2012-7-3   1:49
*	filename: 		CMonster
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����
*/
//------------------------------------------------------------------------
#include <windows.h>
#include <mmsystem.h>
#include "../Include/ThingClientCommon.h"
#include "../Include/CMonster.h"
#include "../Include/CThingClientFacade.h"
#include "../Include/CreatureViewWrapper.h"
//------------------------------------------------------------------------
CMonster::CMonster(void)
{
	// ��ͨ���Գ�ʼ��
	m_uid = INVALID_UID;
	m_pItemView = NULL;
	m_pItemHead = NULL;
	m_bIsDying = FALSE;
	m_nLevel = 0;
	m_nHPMaxCur = 0;
	m_nMPMaxCur = 0;

	memset(&m_Property, 0, sizeof(m_Property));
	ZeroMemory(m_ThingPart, sizeof(m_ThingPart));

	// �߼��������ͳ�ʼ��
	m_ThingClass.SetThingClass(THINGCLASS_MONSTER);
	m_ThingClass.SetCreature();
}
//------------------------------------------------------------------------
CMonster::~CMonster(void)
{

}
//------------------------------------------------------------------------
void CMonster::Release(void)
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
BOOL CMonster::Create(void)
{
	// ������ͼ����
	struct stTemp
	{
		int				nMonsterResID;		// ������ԴID	
		ICreatureSink * pCrtSink;			// 
	}tempdata;

	IConfigDataCenter * pConfigDataCenter = g_pThingClientFacade->GetConfigDataCenter();
	if(pConfigDataCenter == NULL)
	{
		TraceLn("%s����ʧ�ܣ��޷�ȡ����������", GetName());
		return FALSE;
	}

	if(m_Property.ptLoc.x == 0 || m_Property.ptLoc.y == 0)
	{
		TraceLn("%s����ʧ�ܣ�����Ϊ�����꣡", GetName());
		return FALSE;
	}

	int nMonsterID = GetMonsterID();
	MONSTER_CONFIG_INFOR monsterinfor = pConfigDataCenter->GetMonsterInfor(GetMonsterID());
	if(monsterinfor.lmonsterid == 0)
	{
		TraceLn("%s����ʧ�ܣ��޷�ȡ��������ֵ", GetName());
		return FALSE;
	}
	m_nLevel	= monsterinfor.llevel;
	m_nHPMaxCur = monsterinfor.lhp;
	m_nMPMaxCur = monsterinfor.lmp;

	tempdata.nMonsterResID	= monsterinfor.lresid;
	tempdata.pCrtSink		= static_cast<ICreatureSink *>(this);
	IItemClassFactory * pItemClassFactory = g_pThingClientFacade->GetItemFactory();
	if(pItemClassFactory == NULL)
	{
		return FALSE;
	}
	
	int nDataLen = sizeof(tempdata);
	m_pItemView = (CMapItemView *)pItemClassFactory->CreateItem(miNpcObj, (const char *)&tempdata, nDataLen);
	if(m_pItemView == NULL)
	{
		return FALSE;
	}
	
	// ���������������
	CCreatureViewWrapper viewwrapper(m_pItemView);
	viewwrapper.SetData((DWORD)this);
	viewwrapper.AddToMap(m_Property.ptLoc);
	viewwrapper.SetTile(m_Property.ptLoc);
	viewwrapper.SetName(GetName());
	viewwrapper.AddFlag(CMapItemView::flagCanbeSelected);
	viewwrapper.SetDirection(m_Property.nDir);
	int nHPCur = GetPropNum(CREATURE_PROPID_HPCUR);
	int nHPMax = GetPropNum(CREATURE_PROPID_HPMAXCUR);
	viewwrapper.SetBloodInfo(GetPropNum(CREATURE_PROPID_HPCUR), max(nHPCur, nHPMax));
	
	viewwrapper.AddFlag(CMapItemView::flagCanbeSelected);
	viewwrapper.SetMoveSpeedLevel((int *)&m_Property.nMoveSpeedLevel);
	viewwrapper.Born();

	switch(monsterinfor.ltype)
	{
	case MONSTER_TYPE_GENERAL:
	case MONSTER_TYPE_EXCELLENT:
	case MONSTER_TYPE_RARE:
	case MONSTER_TYPE_BOSS:
	case MONSTER_TYPE_BEASTIE:
		{
			viewwrapper.SetFlagColorOnMinmap(255, 0, 0);
		}
		break;
	case MONSTER_TYPE_SAFEGUARD:
	case MONSTER_TYPE_HATCHETMAN:
		{
			viewwrapper.SetFlagColorOnMinmap(126, 0, 252);
		}
		break;
	case MONSTER_TYPE_NOATTACKNPC:
	case MONSTER_TYPE_ATTACKNPC:
		{
			viewwrapper.SetNameColor(252, 136, 0);
			viewwrapper.SetFlagColorOnMinmap(253, 167, 1);			
		}
		break;
	}
	
	return TRUE;
}
//------------------------------------------------------------------------
void CMonster::SetUID(UID uid)
{
	m_uid = uid;
}
//------------------------------------------------------------------------
UID	CMonster::GetUID(void)
{
	return m_uid;
}
//------------------------------------------------------------------------
DWORD CMonster::GetMapID(void)
{
	return GETUIDSERVERID(m_uid);
}
//------------------------------------------------------------------------
void CMonster::SetLoc(POINTS ptLoc)
{
	m_Property.ptLoc = ptLoc;
}
//------------------------------------------------------------------------
POINTS CMonster::GetLoc(void)
{
	CCreatureViewWrapper viewwrapper(m_pItemView);
	
	POINT pos = viewwrapper.GetTile();
	POINTS loc = { pos.x, pos.y };

	return loc;
}
//------------------------------------------------------------------------
IThingClass * CMonster::GetThingClass(void)
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
BOOL CMonster::OnSetPrivateContext(LPCSTR buf, int nLen)
{
	if(buf == NULL || nLen != sizeof(SMONSTER_PROPERTY_PUBLIC))
	{
		TraceLn("CMonster::OnSetPrivateContext Failed! [nLen != sizeof(SMONSTER_PROPERTY_PUBLIC)]");
		return FALSE;
	}

	memcpy(&m_Property, buf, sizeof(SMONSTER_PROPERTY_PUBLIC));
	m_Property.nMoveSpeedLevel = m_Property.nMoveSpeedLevel < 0 ? 0 : m_Property.nMoveSpeedLevel;
	m_Property.nMoveSpeedLevel = m_Property.nMoveSpeedLevel >= MAX_MOVESPEED_LEVEL ? MAX_MOVESPEED_LEVEL - 1 : m_Property.nMoveSpeedLevel;

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMonster::OnGetPrivateContext(LPSTR buf, int &nLen)
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
BOOL CMonster::OnSetPublicContext(LPCSTR buf, int nLen)
{
	return OnSetPrivateContext(buf, nLen);
}
//------------------------------------------------------------------------
BOOL CMonster::OnGetPublicContext(LPSTR buf, int &nLen)
{	
	// �ͻ��˲�������������
	return FALSE;
}
//------------------------------------------------------------------------
// ����thing�����ݿ��ֳ�
// ˵����
// OnGetDBContext() ���ڽ������ֳ����������ݿ�
// OnSetDBContext() ����ͨ�����ݿⷵ�ص����ݴ�������
BOOL CMonster::OnSetDBContext(LPCSTR buf, int nLen)
{	
	return FALSE;
}
//------------------------------------------------------------------------
BOOL CMonster::OnGetDBContext(LPSTR buf, int &nLen)
{
	return FALSE;
}
//------------------------------------------------------------------------
int CMonster::OnCommand(DWORD dwCommand, LPCSTR buf, int nLen)
{
	return 0;
}
//------------------------------------------------------------------------
void CMonster::OnMessage(DWORD dwMsg, LPCSTR buf, int nLen)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return;
	}

	/*
	// ���ж�����Ƿ�ͨ��
	if(pEventServer->FireVote((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_MONSTER, (DWORD)this) == FALSE)
	{
		return;
	}

	// Ȼ����Ϊ�¼�
	pEventServer->FireAction((BYTE *)buf, nLen, dwMsg, SRC_TYPE_ID_MONSTER, (DWORD)this);
	*/
}
//------------------------------------------------------------------------
void CMonster::OnResponse(DWORD dwMsg, LPCSTR buf, int nLen)
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
BOOL CMonster::SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_MONSTER, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ı�����ķ���¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventVoteListener * pListener  - �¼������	
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CMonster::UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_MONSTER, (DWORD)this);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ��: ���ı��������Ϊ�¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventActionListener * pListener - �¼���Ϊִ����
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CMonster::SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_MONSTER, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventActionListener * pListener  - �¼���Ϊִ����
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CMonster::UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_MONSTER, (DWORD)this);	
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ��: ���ı��������Ӧ�¼�
// ��  ��: [����] DWORD dwMsg - �¼�ID
// ��  ��: [����] IEventResponseListener * pListener - �¼���Ӧ��
// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
BOOL CMonster::SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32])
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->AddListener(pListener, dwMsg, SRC_TYPE_ID_MONSTER, (DWORD)this, szDescription);
	*/
	return TRUE;
}
//------------------------------------------------------------------------
// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
// ��  ����[����] DWORD dwMsg  - �¼�ID
// ��  ����[����] IEventResponseListener * pListener  - �¼���Ӧ��
// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
BOOL CMonster::UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener)
{
	IEventServer * pEventServer = g_pThingClientFacade->GetEventServer();
	if(pEventServer == NULL)
	{
		return FALSE;
	}
	
	/*
	return pEventServer->RemoveListener(pListener, dwMsg, SRC_TYPE_ID_MONSTER, (DWORD)this);	
	*/
	return TRUE;
}
//------------------------------------------------------------------------
LPCSTR CMonster::GetName(void)
{
	return GetPropStr(CREATURE_PROPID_NAME);
}
//------------------------------------------------------------------------
int	CMonster::SetPropNum(DWORD dwPropID, int nValue)
{
	switch(dwPropID)
	{
		// �ȼ�
	case CREATURE_PROPID_LEVEL:
		{
			m_nLevel = nValue;
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
			m_nHPMaxCur = nValue;
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
			m_nMPMaxCur = nValue;
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
		// ������Ӫ
	case CREATURE_PROPID_MONSTERCAMP:
		{
			m_Property.nMonsterCamp = nValue;
		}
		break;
	default:
		{
			PrintEventLn("error : CMonster::SetPropNum : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return 0;
}
//------------------------------------------------------------------------
int CMonster::GetPropNum(DWORD dwPropID)
{
	switch(dwPropID)
	{
		// �ȼ�
	case CREATURE_PROPID_LEVEL:
		{
			return m_nLevel;
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
			return m_nHPMaxCur;
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
			return m_nMPMaxCur;
		}
		break;
		// ����ID
	case CREATURE_PROPID_MONSTERID:
		{
			return m_Property.nMonsterID;
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
		// ����
	case CREATURE_PROPID_DIR:
		{
			return m_Property.nDir;
		}
		break;
		// ������Ӫ
	case CREATURE_PROPID_MONSTERCAMP:
		{
			return m_Property.nMonsterCamp;
		}
		break;
	default:
		{
			PrintEventLn("error : CMonster::GetPropNum : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return 0;
}
//------------------------------------------------------------------------
LPCSTR CMonster::SetPropStr(DWORD dwPropID, LPCSTR strPropValue)
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
			PrintEventLn("error : CMonster::SetPropStr : Invalid Propertiy ID[ %d ]!", dwPropID);
		}
		break;
	}

	return "";
}
//------------------------------------------------------------------------
LPCSTR CMonster::GetPropStr(DWORD dwPropID)
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
BOOL CMonster::AddPart(IThingPart * pPart)
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
BOOL CMonster::RemovePart(DWORD dwPartID)
{
	if(dwPartID < 0 || dwPartID >= THINGPARTID_THING_MAXID)
	{
		return FALSE;
	}

	m_ThingPart[dwPartID] = NULL;

	return TRUE;
}
//------------------------------------------------------------------------
IThingPart * CMonster::GetPart(DWORD dwPartID)
{
	if(dwPartID < 0 || dwPartID >= THINGPARTID_THING_MAXID)
	{
		return NULL;
	}

	return m_ThingPart[dwPartID];	
}
//------------------------------------------------------------------------
BOOL CMonster::GotoActState(DWORD dwActStateID, LPCSTR pContext, int nLen)
{
	return FALSE;
}
//------------------------------------------------------------------------
ICreatureActState * CMonster::GetCurActState(void)
{
	return NULL;
}
//------------------------------------------------------------------------
void CMonster::Accept(IMapItemVisitor &visitor)
{
	visitor.VisitMonster();
}
//------------------------------------------------------------------------
CMapItemView * CMonster::GetMapItem(void) const
{
	return m_pItemView;
}
//------------------------------------------------------------------------
void CMonster::SetMapItem(CMapItemView * pMapItemView)
{
	m_pItemView = pMapItemView;
}
//------------------------------------------------------------------------
void CMonster::OnCreatureMoveToTileCenter(const POINTS &ptCurTile, const POINTS &ptNextTile)
{

}
//------------------------------------------------------------------------
BOOL CMonster::AddCreatureCommand(ICreatureCommand * pCreatureCommand)
{
	return FALSE;
}
//------------------------------------------------------------------------
IBitmap * CMonster::GetHeadBitmap(void)
{	
	if(m_pItemHead == NULL)
	{
		IConfigDataCenter * pDataCenter = g_pThingClientFacade->GetConfigDataCenter();
		if(pDataCenter == NULL)
		{
			return NULL;
		}

		MONSTER_CONFIG_INFOR & monsterinfor = pDataCenter->GetMonsterInfor(m_Property.nMonsterID);
		if(monsterinfor.lmonsterid == 0)
		{
			return NULL;
		}

		IItemClassFactory * pItemClassFactory = g_pThingClientFacade->GetItemFactory();
		if(pItemClassFactory == NULL)
		{
			return NULL;
		}

		m_pItemHead = (CMapItemView *)pItemClassFactory->CreateItem(miStaticObj_Picture, (char*)&(monsterinfor.lheadresid), sizeof(DWORD));
		if(m_pItemHead == NULL)
		{
			return NULL;
		}
	}	

	return m_pItemHead->GetBitmap();
}
//------------------------------------------------------------------------
DWORD CMonster::GetMonsterID(void)
{
	return GetPropNum(CREATURE_PROPID_MONSTERID);
}
//------------------------------------------------------------------------
void CMonster::SetDying(BOOL bDying)
{
	m_bIsDying = bDying;
}
//------------------------------------------------------------------------
BOOL CMonster::IsDying(void)
{
	return m_bIsDying;
}
//------------------------------------------------------------------------