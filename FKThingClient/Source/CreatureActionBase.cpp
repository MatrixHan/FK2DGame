/**
*	created:		2012-7-3   23:51
*	filename: 		CreatureActionBase
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���������Ϊ,��Ӧ������Ϊ�¼��ķ���
*/
//------------------------------------------------------------------------
#include "../Include/ThingClientCommon.h"
#include "../../FKGame/SceneObj/ICreature.h"
#include "../../FKGame/SceneObj/IThingPart.h"
#include "../../FKGame/Common/ICreatureCommand.h"
#include "../Include/CThingClientFacade.h"
#include "../Include/CreatureActionBase.h"
#include "../Include/CCreatureActStateMachine.h"
#include "../Include/CreatureViewWrapper.h"
#include "../../FKGame/Common/CreaturePropertyDef.h"
#include "../../FKGame/Network/IAppNetworkLink.h"
#include "../../FKGame/Skill/ISkillClient.h"
#include "../../FKChatClient/Include/SysMessageDef.h"
//------------------------------------------------------------------------
void CCreatureAction_Creature_Born_CC::OnAction(IEvent * pEvent)
{
	if(pEvent == NULL)
	{
		return;
	}

	ICreature * pCreature = (ICreature *)pEvent->GetSrcPointer();
	if(pCreature == NULL)
	{
		return;
	}

	int	nContextLen = 0;
	char * pContext = (char *)pEvent->GetContext(nContextLen);

	// ��������ǰ��״̬
	OnProcState(pCreature, pContext, nContextLen);
}
//------------------------------------------------------------------------
void CCreatureAction_Creature_Born_CC::OnProcState(ICreature * pCreature, char * pContext, int nLen)
{
	// ����״̬���ݣ�Ʃ���������������״̬��������Ҫ����Ϊ������.���ѱ�����
	if(pCreature == g_pGlobal->m_pPlayer)
	{
		return;
	}
	
	int nCurActState = CREATURE_STATEID_STAND;
	if(pCreature->GetThingClass()->GetThingClass() == THINGCLASS_ACTOR)
	{
		if(nLen != sizeof(SACTOR_PROPERTY_PUBLIC))
		{
			return;
		}

		SACTOR_PROPERTY_PUBLIC * pProperty = (SACTOR_PROPERTY_PUBLIC *)(pContext);
		nCurActState = pProperty->nCurActState;
	}
	else if(pCreature->GetThingClass()->GetThingClass() == THINGCLASS_MONSTER)
	{
		if(nLen != sizeof(SMONSTER_PROPERTY_PUBLIC))
		{
			return;
		}

		SMONSTER_PROPERTY_PUBLIC * pProperty = (SMONSTER_PROPERTY_PUBLIC *)(pContext);
		nCurActState = pProperty->nCurActState;
	}

	// �������������ֱ�����ڵط��Ϳ�����
	if(nCurActState == CREATURE_STATEID_DIE && pCreature->GetThingClass()->GetThingClass() == THINGCLASS_ACTOR)
	{
		CCreatureViewWrapper viewwrapper(pCreature->GetMapItem());
		//CrtCmdDie,	��������
		//				dwParam1: 0��ʾ�����������ж�����1��ʾ�Ѿ��������޶���
		//				dwParam2: ?
		viewwrapper.Die(1, 0);		

		// ��Ϊ����̬
		pCreature->SetDying(TRUE);
	}
	else if(nCurActState == CREATURE_STATEID_SIT)
	{
		CCreatureViewWrapper viewwrapper(pCreature->GetMapItem());
		viewwrapper.Sit();
	}	
	else
	{
		int nWalkStyle = pCreature->GetPropNum(CREATURE_PROPID_WALKSTYLE);
		CCreatureViewWrapper viewwrapper(pCreature->GetMapItem());
		if(nWalkStyle == CREATURE_MOVESTYLE_RACE)
		{
			if(pCreature->GetThingClass()->GetThingClass() == THINGCLASS_MONSTER)
			{
				viewwrapper.SetMoveStyle(ActMoveStyleRide);
			}
		}
		else if(nWalkStyle == CREATURE_MOVESTYLE_WALK)
		{
			viewwrapper.SetMoveStyle(ActMoveStyleWalk);
		}
		else
		{		
			viewwrapper.SetMoveStyle(ActMoveStyleRun);
		}
	}
}
//------------------------------------------------------------------------
void CCreatureAction_Actor_Move_CC::OnAction(IEvent * pEvent)
{
	/*��Ϣ��ʽ	
	struct MSG_FORMAT
	{	
		int			nPathLen;		// ·������
		POINTS *	pPath;			// ·���ӵ㣬���ĳ��� = nPathLen * sizeof(POINTS)
	};//*/
	if(pEvent == NULL)
	{
		return;
	}

	ICreature * pCreature = (ICreature *)pEvent->GetSrcPointer();
	if(pCreature == NULL)
	{
		return;
	}

	int	nContextLen = 0;
	char * pContext = (char *)pEvent->GetContext(nContextLen);
	if(pContext == NULL || nContextLen < sizeof(int) + sizeof(POINTS))
	{
		return;
	}

	int nPathLen = *((int *)pContext);
	POINTS * pPath = (POINTS *)(pContext + sizeof(int));

	if(nPathLen > MAX_PATH_LEN)
	{
		return;
	}

	// ���ݺ������㷨���ÿͻ�������
	CCreatureViewWrapper ViewWrapper(pCreature->GetMapItem());
	ViewWrapper.Move(pPath, nPathLen);

	// ������������֤
	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_MOVE);
	DWORD dwTimeTick = g_pThingClientFacade->GetMsgTimeTick();
	CS_ACTION_HEAD_EX head(wMsgID, dwTimeTick);

	DWORD dwStrlen = sizeof(CS_ACTION_HEAD_EX);
	
	char szBuf[1024];
	CDataArchive dataarchive(szBuf, sizeof(szBuf));
	dataarchive.Write(&head, sizeof(CS_ACTION_HEAD_EX));
	dataarchive.Write(&nPathLen, sizeof(int));
	dataarchive.Write(pPath, sizeof(POINTS) * nPathLen);
	
	g_pGlobal->m_pAppNetworkLink->Send(szBuf, dataarchive.GetBufferOffset());
}
//------------------------------------------------------------------------
void CCreatureAction_Actor_PrepAttack_CC::OnAction(IEvent * pEvent)
{
	/*��Ϣ��ʽ	
	struct MSG_FORMAT
	{	
		SUseSkillContext *
	};//*/
	if(pEvent == NULL)
	{
		return;
	}
	
	ICreature * pCreature = (ICreature *)pEvent->GetSrcPointer();

	// ���������ǣ��Ż��з��ʹ���Ϣ�Ŀ���
	if(pCreature == NULL && pCreature == g_pGlobal->m_pPlayer)
	{
		return;
	}

	int	nContextLen = 0;
	char * _pContext = (char *)pEvent->GetContext(nContextLen);
	if(_pContext == NULL || nContextLen != sizeof(SUseSkillContext))
	{
		return;
	}

	SUseSkillContext *pContext = (SUseSkillContext *)_pContext;

	CCreatureView * pTargetView = NULL;
	if(pContext->uidTarget != INVALID_UID)
	{
		IThing * pThing = g_pThingClientFacade->GetThing(pContext->uidTarget);
		if(pThing == NULL)
		{
			// ��ͼ�ϣ��Ҳ��������
			// TraceLn("����ʱ����ͼ�ϣ��Ҳ��������");
			return;
		}
		else
		{
			pTargetView = (CCreatureView *)(((IMapThing *)(pThing))->GetMapItem());
		}
	}

	SSkillCreateParam context;
	context.nIndex = pContext->SkillData.wSkillID;
	context.nLevel = g_pThingClientFacade->GetSkillManager()->GetSkillInfo(pContext->SkillData, enSKILL_INFO_VIEWLEVEL);
	context.nCheckId = pContext->wCheckId;
	context.ptAttack.x = pContext->ptTargetTile.x;
	context.ptAttack.y = pContext->ptTargetTile.y;
	context.pAttackView = pTargetView;

	CCreatureViewWrapper ViewWrapper(pCreature->GetMapItem());
	ViewWrapper.PrepAttack((DWORD)&context);
}
//------------------------------------------------------------------------
void CCreatureAction_Actor_Attack_CC::OnAction(IEvent * pEvent)
{
	/*��Ϣ��ʽ	
	struct MSG_FORMAT
	{	
		SUseSkillContext *
	};//*/
	if(pEvent == NULL)
	{
		return;
	}
	
	ICreature * pCreature = (ICreature *)pEvent->GetSrcPointer();

	// ���������ǣ��Ż��з��ʹ���Ϣ�Ŀ���
	if(pCreature == NULL && pCreature == g_pGlobal->m_pPlayer)
	{
		return;
	}

	int	nContextLen = 0;
	char * _pContext = (char *)pEvent->GetContext(nContextLen);
	if(_pContext == NULL || nContextLen != sizeof(SUseSkillContext))
	{
		return;
	}

	SUseSkillContext *pContext = (SUseSkillContext *)_pContext;

	CCreatureView * pTargetView = NULL;
	if(pContext->uidTarget != INVALID_UID)
	{
		IThing * pThing = g_pThingClientFacade->GetThing(pContext->uidTarget);
		if(pThing == NULL)
		{
			// ��ͼ�ϣ��Ҳ��������
			// TraceLn("����ʱ����ͼ�ϣ��Ҳ��������");
			return;
		}
		else
		{
			pTargetView = (CCreatureView *)(((IMapThing *)(pThing))->GetMapItem());
		}
	}

	SSkillCreateParam context;
	context.nIndex = pContext->SkillData.wSkillID;
	context.nLevel = g_pThingClientFacade->GetSkillManager()->GetSkillInfo(pContext->SkillData, enSKILL_INFO_VIEWLEVEL);
	context.nCheckId = pContext->wCheckId;
	context.ptAttack.x = pContext->ptTargetTile.x;
	context.ptAttack.y = pContext->ptTargetTile.y;
	context.pAttackView = pTargetView;

	CCreatureViewWrapper ViewWrapper(pCreature->GetMapItem());
	ViewWrapper.Attack((DWORD)&context);
}
//------------------------------------------------------------------------
void CCreatureAction_Actor_StopAttack_CC::OnAction(IEvent * pEvent)
{
	if(pEvent == NULL)
	{
		return;
	}

	ICreature * pCreature = (ICreature *)pEvent->GetSrcPointer();
	if(pCreature == NULL)
	{
		return;
	}

	// ���������ǣ��Ż��з��ʹ���Ϣ�Ŀ���
	if(pCreature == NULL && pCreature == g_pGlobal->m_pPlayer)
	{
		return;
	}

	CCreatureViewWrapper viewwrapper(pCreature->GetMapItem());
	
	viewwrapper.StopAttack();
}
//------------------------------------------------------------------------
void CCreatureAction_Actor_ChangeMoveStyle_CC::OnAction(IEvent * pEvent)
{
	/**
	 * int // ��ǰ״̬	
	 */
	if (pEvent == NULL)
	{
		return;
	}

	ICreature* pCreature = (ICreature*)pEvent->GetSrcPointer();
	if (pCreature == NULL || pCreature != g_pGlobal->m_pPlayer)
	{
		return;
	}

	// ��������ˣ��������л���·ģʽ
	ICreatureActState * pCurActState = pCreature->GetCurActState();
	if(pCurActState == NULL || pCurActState->GetStateID() == CREATURE_STATEID_DIE)
	{
		return;
	}

	int nLen = 0;
	LPCSTR pBuf = (LPCSTR)pEvent->GetContext(nLen);
	pCreature->OnCommand(CREATURE_COMMANDID_CHANGEMOVESTYLE, pBuf, nLen);
}
//------------------------------------------------------------------------
void CCreatureAction_Actor_StopPreAttack_CC::OnAction(IEvent * pEvent)
{
	if(pEvent == NULL)
	{
		return;
	}

	ICreature * pCreature = (ICreature *)pEvent->GetSrcPointer();
	if(pCreature == NULL)
	{
		return;
	}

	// ���������ǣ��Ż��з��ʹ���Ϣ�Ŀ���
	if(pCreature == NULL && pCreature == g_pGlobal->m_pPlayer)
	{
		return;
	}

	CCreatureViewWrapper viewwrapper(pCreature->GetMapItem());
	
	viewwrapper.StopPrepAttack();
}
//------------------------------------------------------------------------
void CCreatureAction_Actor_Die_CC::OnAction(IEvent * pEvent)
{
	if(pEvent == NULL)
	{
		return;
	}

	ICreature * pCreature = (ICreature *)pEvent->GetSrcPointer();
	if(pCreature == NULL)
	{
		return;
	}

	CCreatureViewWrapper viewwrapper(pCreature->GetMapItem());
	//CrtCmdDie,	��������
	//				dwParam1: 0��ʾ�����������ж�����1��ʾ�Ѿ��������޶���
	//				dwParam2: ?
	viewwrapper.Die(0, 0);

	// ������Ӧ
	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_DIE);
	pCreature->OnResponse(wMsgID, NULL, 0);
}
//------------------------------------------------------------------------
void CCreatureAction_Monster_Die_CC::OnAction(IEvent * pEvent)
{
	if(pEvent == NULL)
	{
		return;
	}

	ICreature * pCreature = (ICreature *)pEvent->GetSrcPointer();
	if(pCreature == NULL)
	{
		return;
	}

	CCreatureViewWrapper viewwrapper(pCreature->GetMapItem());
	//CrtCmdDie,	��������
	//				dwParam1: 0��ʾ�����������ж�����1��ʾ�Ѿ��������޶���
	//				dwParam2: ?
	viewwrapper.Die(0, 0);

	// ������Ӧ
	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CC, MAINACTION_MSGID_DIE);
	pCreature->OnResponse(wMsgID, NULL, 0);

	// ����������
	pCreature->SetMapItem(NULL);	
	SAFE_RELEASE(pCreature);
}
//------------------------------------------------------------------------
void CCreatureAction_Actor_SystemMessage_CC::OnAction(IEvent * pEvent)
{
	if(pEvent == NULL)
	{
		return;
	}

	ICreature * pCreature = (ICreature *)pEvent->GetSrcPointer();
	if(pCreature == NULL || pCreature->GetThingClass()->GetThingClass() != THINGCLASS_ACTOR)
	{
		return;
	}
	IActor * pActor = static_cast<IActor *>(pCreature);

	int nContextLen = 0;
	char * pContext = (char *)pEvent->GetContext(nContextLen);

	char szBuffer[1024];
	CDataArchive dataarchive(szBuffer, sizeof(szBuffer));
	int nType = SMT_FLOAT;
	dataarchive.Write(&nType, sizeof(int));
	dataarchive.Write(&nContextLen, sizeof(int));
	dataarchive.Write(pContext, nContextLen);

	WORD wMsgID = MAKE_MSGID(MAINACTION_CIRCULTYPE_CSC, MAINACTION_MSGID_SYSTEMMESSAGE);
	pActor->OnMessage(wMsgID, szBuffer, dataarchive.GetBufferOffset());
}
//------------------------------------------------------------------------