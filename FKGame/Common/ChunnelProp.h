/**
*	created:		2012-7-2   22:00
*	filename: 		ChunnelProp
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��ͼͨ�����Զ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#pragma pack(1)
//------------------------------------------------------------------------
enum 
{
	CHUNNEL_TRANS_MODE_POINTING = 1,				// 1  = ������
	CHUNNEL_TRANS_MODE_RAND,						// 2  = �������
	CHUNNEL_TRANS_MODE_LIST,						// 3  = �б���
	CHUNNEL_TRANS_MODE_ECTYPE,						// 4  = ��������
};
//------------------------------------------------------------------------
#define CHUNNEL_TARGET_MAXCOUNT						10
//------------------------------------------------------------------------
// ����Ŀ�����Ϣ
struct SCHUNNEL_TARGET
{
	DWORD			dwTragetMapID;					// Ŀ���ͼID
	POINTS			ptTragetPos;					// Ŀ��λ��
	DWORD			dwRadius;						// ����뾶
};
//------------------------------------------------------------------------
// �����б�
struct SCHUNNEL_PROPERTY_LIST
{
	char			szName[MAX_NAME_LEN];			// 1  = ����
	POINTS			ptLoc;							// 2  = λ�� 
	DWORD			dwResID;						// 3  = ��ԴID	
	DWORD			dwTransMode;					// 4  = ����ģʽ��1�������ͣ�2 ��������ͣ�3���б��ͣ�
	DWORD			dwTargetCount;					// 5  = Ŀ������

	SCHUNNEL_TARGET TargetInfor[CHUNNEL_TARGET_MAXCOUNT];
};
//------------------------------------------------------------------------
#pragma pack()
//------------------------------------------------------------------------