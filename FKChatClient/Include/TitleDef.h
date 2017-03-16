/**
*	created:		2012-6-28   9:32
*	filename: 		TitleDef
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <string>
#include <Windows.h>
using std::string;
//------------------------------------------------------------------------
// ���޴�ʹ��
#define INFINITY_TIME_USE  -1000
// ��Ч�ƺ�ID
#define INVALID_TITLE_ID (-1)
//------------------------------------------------------------------------
#pragma pack(push,1)
//------------------------------------------------------------------------
// �ƺ�״̬
enum TITLE_STATUS
{
	 UNKNOW_TITLE,     //  ���˽�
     UNDERSTAND_TITLE, //  �˽�
     HOLD_TITLE,       //  ӵ��
	 USE_TITLE         //  ʹ�óƺ�
};
//------------------------------------------------------------------------
struct STitle
{
   	string  strUseFun;          //  ʹ�óƺŵĻص�����
	string  strUnUseFun;        //  ��ʹ�óƺŵĻص�����
	string  strDeleteFun;     //  ɾ���ƺŵĻص�����
    long    lTitleId;           //  �ƺ�ID
	string  strName;            //  �ƺ�����
	short   sCanUseTimes;       //  ��ʹ�õĴ���
	long    lCanUseTime;        //  �����õ�ʱ��
	string  strTitleName;       //  ʵ������
	BOOL    bLogoutOff;         //  �����Ƿ�ȡ��
	BOOL    bCanOff;            //  �ܷ�ȡ��
    BOOL    bCanDel;            //  �ܷ�ɾ��
	long    lTimeDel;           //  ��ʱʱ��,���������ʱ��ͻ��Զ�ɾ��
    string  strCondition;       //  ��ȡ����˵��
	string  strMethod;          //  ��ȡ����˵��
	string  strDes;             //  ����
	string  strExDes;           //  ����������Ϣ
};
//------------------------------------------------------------------------
// ������ϵĳƺ�����ṹ��Ҫ���ڷ�������
struct SActorTitle
{
    long   lTitleId;      // �ƺ�ID
	string strName;       // �ƺ�����
	long   lGetTime;      // ��ȡ����ƺŵ�ʱ��
	short  sLeftUsedTimes; // ʣ��ʹ�ô���
    long   lCurLeftTime;  // ����ʹ�õĳƺŻ�ʣ�¶���ʱ��
	BYTE   byStatus;      // ��ǰ�ƺŵ�״̬
};
//------------------------------------------------------------------------
// ����2���ṹ��Ҫ�ÿͻ���
struct SActorTitle2
{
   long  lTitleId;     // �ƺ�ID 
   char  szName[32];   // ����
   long  lGetTime;     // ���ʱ��
   long  lStatus;      // �ƺ�״̬
};
//------------------------------------------------------------------------
struct SActorTitle2Ex
{
    SActorTitle2 title; // �ƺ�
	DWORD dwData;       // ��չ����
};
//------------------------------------------------------------------------
enum Title_CMD
{
   TITLE_CMD_GET,          // ��ȡ�Լ������гƺ�
   TITLE_CMD_USE,          // ʹ�óƺ�
   TITLE_CMD_UNUSE,        // ��ʹ�óƺ�
   TITLE_CMD_DELETE,       // ɾ���ƺ�
   TITLE_CMD_GETOTHER,     // ��ȡĳ����ҵĳƺ�
   TITLE_CMD_STATUSCHANGE, // ״̬�ı�֪ͨ
   TITLE_CMD_ADD,          // ����һ���ƺ�
   TITLE_CMD_MAX

};
//------------------------------------------------------------------------
#pragma pack(pop)
//------------------------------------------------------------------------