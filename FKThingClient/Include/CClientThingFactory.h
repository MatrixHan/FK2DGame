/**
*	created:		2012-7-2   22:22
*	filename: 		CClientThingFactory
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����һ���߼���������ͣ����ֳ�������һ���߼�����ָ��
*					�߼����������ͼ�ϵ���,����,NPC,װ��,���͵��
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/SceneObj/IThingFactory.h"
//------------------------------------------------------------------------
class CThingFactory : public IThingFactory
{
public:
	//////////////////////////////////////////////////////////
	/******************** IThingFactory *********************/
	// ÿһ���߼����������ѡ��������ϸ���ֳ����������ô��Ե��ֳ�����
	// ����Ӧ������Ϸ�е�����������Ҫ�����Լ��Ĺ������������ȸ�������
	// ������Ʒ����ÿһ����Ʒ�����㿴����ֻ�ܿ������ǵĽ�ɫ����,�Ա�
	// װ���ȣ� ���ǵ���ϸ���Ի�����Ʒ������Ʒ�������ǲ��ᷢ������
	// ������Ҫ֧�����ַ�ʽ����
	virtual IThing *	CreatePrivateThing(DWORD dwThingClassID, UID uid, const char * buf, int nLen);

	virtual IThing *	CreatePublicThing(DWORD dwThingClassID, UID uid, const char * buf, int nLen);

	virtual void		Release(void);

private:
	// ��������
	IThing *			CreateActor_private(UID uid, LPCSTR buf, int nLen);
	IThing *			CreateActor_public(UID uid, LPCSTR buf, int nLen);

	// ��������
	IThing *			CreateMonster_public(UID uid, LPCSTR buf, int nLen);

	// ������Ʒ
	IThing *			CreateGoods_private(UID uid, LPCSTR buf, int nLen);
	IThing *			CreateGoods_public(UID uid, LPCSTR buf, int nLen);

	// ������ͼͨ��
	IThing *			CreateChunnel_public(UID uid, LPCSTR buf, int nLen);
	
	// ��������
	IThing *            CreateBox_public(UID uid, LPCSTR buf, int nLen);

	// ����̯λ
	IThing *			CreateStall_public(UID uid, LPCSTR buf, int nLen);
};
//------------------------------------------------------------------------