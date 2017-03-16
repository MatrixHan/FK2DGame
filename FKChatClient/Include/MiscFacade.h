/**
*	created:		2012-6-28   8:58
*	filename: 		MiscFacade
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���������ʵ��һЩ���ù���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/WorkSkill/IWorkSkillClient.h"
#include "../../FKGame/Container/IContainerHolder.h"
#include "../../FKGame/Container/IContainerClient.h"
#include "../../FKGame/SceneObj/IMedicament.h"
#include "../../FKGame/SceneObj/ICreature.h"
//------------------------------------------------------------------------
namespace MISCFACADE
{
	//ͨ����ƷID, ȡ��Ʒ����
	LPCSTR GetGoodsName(DWORD dwGoodsID);
	
	//������ﱳ����, ĳ����Ʒ�ĸ��� (ֻ������ɾ����)
	int GetGoodsNumFromPackage(DWORD dwGoodsID);
	
	//ͨ��UIDȡĳ��Thing
	IThing *GetThingFromUID(UID uidThing);
	
	//ȡ���ĳ�����ܵĵȼ�
	int GetPlayerSkillLevel(int nSkillID);
	
	//ȡ��������,����ƿ������(û�п�ʼ������)
	int GetEmptyBottle(int nBottleID);
	
	//ȡThing������
	int GetThingClass(UID uidThing);
	
	//ͨ����������ID,,ȡ��������, ���ûѧ��,����NULL
	IWorkSkill * GetWorkSkill(int nSkillID);
	
	//��ȡĳ�������Ƿ�����
	BOOL IsDead(UID uidCreature);

	//ȡ���ﲿ��
	void *GetThingPart(ICreature *pCreature, int nPartIndex);

	//�ж��Ƿ�����ĳ����
	BOOL MasteredSkill(int nSkillID);

	//�ж�UID�Ƿ��ٻ���
	BOOL isSummonPet(UID uidCreature);

	//ȡ��ǰ���ﱳ��
	IContainer *GetActorPackage(void);

	//�ж���Ʒ�Ƿ�һ���;�Ϊ0 ��װ��
	BOOL isBrokenEquipment(UID uidThing);

	//�ж�����ٻ���������ֻ��
	int GetPetCount(IActor *pActor);

	//�ж�һ��UID�Ƿ�����ƿ
	BOOL isBottle(UID uidThing);

	//�ж�һ������ƿ�Ƿ���������
	BOOL isBottleRefining(IMedicament *pMedicament);

	//�ж�һ����Ʒ�ܷ���
	BOOL GoodsCanDiscard(UID uidGoods);

	//�ж�һ��UID�Ƿ�Ԫ��
	BOOL isPlayTimeTreasure(UID uidThing);

	//��һ����ֵ��Χ��������
	int CorrectRange(int val, int low, int high);

	//��szSrc�����ݲ��뵽szDst��nPos��λ����(szDstҪ�㹻��)
	void StringInsert(char *szDst, char *szSrc, int nPos);

	//������װID, ��ѯ���ϴ��˶��ټ���������װ
	int GetSuitEquipedCount(int nSuitID);
};
//------------------------------------------------------------------------