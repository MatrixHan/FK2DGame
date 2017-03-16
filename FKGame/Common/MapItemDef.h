/**
*	created:		2012-6-30   5:33
*	filename: 		MapItemDef
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
enum enMapItemClassID
{
	miUnknow						= 0,	// δ֪����

	// ��̬����(���ӡ���ľ��Сɽ��Ѫƿ����ʯ��)
	miStaticObj_NoOcc_NoTrans		= 5,	// ��̬����(��ռλ��Ϣ�������͸)
	miStaticObj_NoOcc_Trans			= 1,	// ��̬����(��ռλ��Ϣ����Ҫ��͸)
	miStaticObj_SingleOcc_NoTrans	= 7,	// ��̬����(��ռλ��Ϣ�������͸)
	miStaticObj_SingleOcc_Trans		= 8,	// ��̬����(��ռλ��Ϣ����Ҫ��͸)
	miStaticObj_MultiOcc_NoTrans	= 9,	// ��̬����(��ռλ��Ϣ�������͸)
	miStaticObj_MultiOcc_Trans		= 2,	// ��̬����(��ռλ��Ϣ����Ҫ��͸)

	// ����������(���桢���̡�����ţ�����)
	miAniObj_NoOcc_RGB				= 3,	// ��������(��ռλ��Ϣ������alpha)
	miAniObj_NoOcc_RGBA				= 13,	// ��������(��ռλ��Ϣ������alpha)
	miAniObj_SingleOcc_RGB			= 14,	// ��������(��ռλ��Ϣ������alpha)
	miAniObj_SingleOcc_RGBA			= 15,	// ��������(��ռλ��Ϣ������alpha)
	miAniObj_MultiOcc_RGB			= 4,	// ��������(��ռλ��Ϣ������alpha)
	miAniObj_MultiOcc_RGBA			= 16,	// ��������(��ռλ��Ϣ������alpha)

	miUserDefine					= 6,	// �û��Զ�������������־�ȣ�
	miMagicObj						= 10,	// ħ������
	miNpcObj						= 11,	// NPC�͹���
	miActorObj						= 12,	// ��ɫ����
	miStaticObj_Picture				= 17,	// ��Ҫ���ڰ���ͼ�꽨������
	miStaticObj_Article				= 19,	// ������Ʒ�������д����߼���Ʒ��ͼ��
	miTrapObj						= 23,	// �������
	miSpecificBlock					= 27,	// �ϲ������
	miSpecificBlockDown				= 29,	// �²������
	miAniObj_Transprot				= 32,	// ������
	miStaticObj_Booth				= 33,	// ��̯

	miMaxClassID					= 36	// ��ǰ��ʶ������������
};
//------------------------------------------------------------------------