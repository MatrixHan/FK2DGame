/**
*	created:		2012-7-4   1:03
*	filename: 		SContainer
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#define MAINCONTAINER_CSC_ADD		0		//���������һ����Ʒ
#define MAINCONTAINER_CSC_ADDALL	1		//���һ��������������Ʒ������һ��������
#define MAINCONTAINER_CSC_DESTORY	2		//������Ʒ
#define MAINCONTAINER_CSC_REMOVE	3		//�������Ƴ���Ʒ
#define MAINCONTAINER_CSC_UPDATE	4		//֪ͨ�ͻ��˸���

#define MAINCONTAINER_CSC_OPEN		5		//��
#define MAINCONTAINER_CSC_CLOSE		6		//�ر�
#define MAINCONTAINER_CSC_USE		7		//ʹ��
//------------------------------------------------------------------------
///////////////////////////��Ʒ���Ͷ���//////////////////////////////////////
#define GOODS_TYPE_REALY			1		//��ʵ��Ʒ
#define GOODS_TYPE_REFER			2		//��Ʒ����
#define GOODS_TYPE_SKILL			3		//����
//------------------------------------------------------------------------
/////////////////////////������־����/////////////////////////////
#define CONTAINER_FLAGTYPE_PACKET		1		//��Ʒ��
#define CONTAINER_FLAGTYPE_EQUIP		2		//װ����
#define CONTAINER_FLAGTYPE_WAREHOUSE	3		//�ֿ�
#define CONTAINER_FLAGTYPE_TRADE		4		//����
#define CONTAINER_FLAGTYPE_MONSTER_BOX	5		//�������ı���
#define CONTAINER_FLAGTYPE_RESOURCE_BOX 6		//��ʯҩ�ݵȵ�
#define CONTAINER_FLAGTYPE_SYSTEM_BOX	7		//ϵͳ�����ı���
//------------------------------------------------------------------------
/////////////////////////�������ͻ���ͨѶ����ֳ�/////////////////////////////
//���������ֳ�
struct SOpenContainerContext
{
	DWORD dwContainerID;
	SOpenContainerContext()
	{
		memset(this, 0, sizeof(SOpenContainerContext));
	}
};
//------------------------------------------------------------------------
//�ر��������ֳ�
#define SCloseContainerContext SOpenContainerContext
//------------------------------------------------------------------------
//�ͻ��˷���������Ʒ����ֳ�
struct SGoodsAddContext
{
	DWORD dwTargetContainerID;	//Ŀ������id
	int nTargetLocation;		//��Ŀ�������е�λ��	
	UID uidGoods;				//��Ʒuid
	int nGoodsNumber;			//��Ʒ��Ŀ
	DWORD dwSrcContainerID;		//Դ����id
	int nSrcLocation;			//��Դ�����е�λ��	
	SGoodsAddContext()
	{
		dwTargetContainerID = 0;
		nTargetLocation = -1;
		uidGoods = 0;
		nGoodsNumber = 0;
		dwSrcContainerID = 0;
		nSrcLocation = -1;
	}
};
//------------------------------------------------------------------------
//�ͻ��˷������Ľ�һ��������������Ʒת�Ƶ���һ�������е��ֳ�
struct SContainerToContainer 
{
	DWORD dwTargetContainerID;	//Ŀ������id
	DWORD dwSrcContainerID;		//Դ����id
	SContainerToContainer()
	{
		memset(this, 0 ,sizeof(SContainerToContainer));
	}
};
//------------------------------------------------------------------------
//������Ʒ�ֳ�
struct SDestoryGoodsContext
{
	UID uidGoods;				//��Ʒuid
	DWORD dwSrcContainerID;		//Դ����id
	int nSrcLocation;			//��Դ�����е�λ��
	SDestoryGoodsContext()
	{
		memset(this, 0, sizeof(SDestoryGoodsContext));
	}
};
//------------------------------------------------------------------------
//���¿ͻ���
struct SContainerUpdateHead
{
	SContainerUpdateHead()
	{
		dwContainerID = 0;
	}
	DWORD dwContainerID;		//����id	
	//��һ�����߶��SSingleUpdateInfo
};
//------------------------------------------------------------------------
struct SSingleUpdateInfo
{
	UID uidGoods;				//��Ʒuid
	int nLocation;				//�����������е�λ��
	BOOL bLock;					//�Ƿ�����
	DWORD dwNumber;				//��Ŀ

	SSingleUpdateInfo()
	{
		uidGoods = INVALID_UID;
		nLocation = -1;
		bLock = FALSE;
		dwNumber = 0;
	}
};
//------------------------------------------------------------------------
//ʹ����Ʒ�ֳ�
struct SUseGoodsContext
{	
	DWORD dwSrcContainerID;		//Դ����id
	int nSrcLocation;			//��Դ�����е�λ��	
	SUseGoodsContext()
	{
		memset(this, 0, sizeof(SUseGoodsContext));
	}
};
//------------------------------------------------------------------------
/////////////////////////�¼�����ֳ�/////////////////////////////////////////
//���������Ӧ�ֳ�
struct SSGoodsResponseContext
{
	DWORD dwOperatorID;		//������
	DWORD dwContainerID;	//��������ID
	UID uidGoods;			//��ƷUID
	int nLocation;			//��Ʒ����λ��
	DWORD dwSrcContainerID; //��ƷԴ����ID
	SSGoodsResponseContext()
	{
		memset(this, 0, sizeof(SSGoodsResponseContext));
	}
};
//------------------------------------------------------------------------
//��Ʒ��ӻ���ɾ������ֳ�,��ʵ����Ӧ�ֳ���ͬ
#define SSGoodsVoteContext	SSGoodsResponseContext
#define SSGoodsUseContext	SSGoodsResponseContext
//------------------------------------------------------------------------

