/**
*	created:		2012-6-29   14:55
*	filename: 		WndDef
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#define TITLE_WND 456
//------------------------------------------------------------------------
#define WND_CHAT						70			// ���촰��
const static int WE_AW_UPDATE_IMEICON   =  4;		//IME���ڷ���, Ҫ��������뷨ͼƬ
const static int WE_AW_CONTENT_ADDGOODS =  5;		//������������,����װ��wParamΪװ��UID
//------------------------------------------------------------------------
#define WND_CHAT_CHANNEL                71          // �����Ӵ��� :Ƶ��ѡ�񴰿�
const static int WE_WW_CHANNEL_SETCHANNEL  =  2;    // Ƶ���Ӵ����յ�����Ϣ��,����Ƶ�� wparamΪĿ��Ƶ��
//------------------------------------------------------------------------
#define WND_CHAT_FACE                   72          // �����Ӵ��� :���鴰��
#define WND_CHAT_IME                    73          // �����Ӵ��� :���뷨����
const static int WE_AW_DISCONNECT_IMEUPDATE            =  5;    //Ҫ���˶����뷨�ĸ�����Ϣ
//------------------------------------------------------------------------
#define WND_CHAT_COLOR                  74          // �����Ӵ��� :��ɫ����
#define WND_CHAT_PERSON                 75          // �����Ӵ��� :��ʷ��ɫ����
#define WND_CHAT_CONTENT                76          // �����Ӵ��� :��ʷ�������ݴ���
//------------------------------------------------------------------------
#define WND_INPUTWND					20			//���봰��
//------------------------------------------------------------------------
// ���������
const int WE_CW_INPUT_CANCEL				= 0;	//����һ���Ի���
const int WE_CW_INPUT_OK					= 1;	//����һ���Ի���
const int WE_CW_INPUT_MSG					= 2;	//���õ�����Ϣ
const int WE_WC_INPUT_VALUE					= 3;	//֪ͨ������ֵ
const int WE_WC_INPUT_CANCEL				= 4;    //ȡ������
const int WE_CW_INPUT_SET                   = 5;    //�������ã�������󳤶ȺͿ����뺺��
const int WE_CW_INPUT_POP					= 6;	//�öԷ���ֹͣ���߿�ʼ������WPARM:1 = ������2 = ֹͣ
const int WE_CW_INPUT_TIMER					= 7;	//�趨����ʱ������WPARM:1 = ��ת��2 = ֹͣ
const int WE_AW_INPUT_GETCOURSE				= 8;	//��ѯ���ڵ�����ԭ�� WPARAM Ϊint* , ���ڽ���ԭ��
const int WE_CW_INPUT_CLEAR				    = 9;   	//������Ϣ����������е���Ϣ,���رյ�ǰ��Ϣ����(���������ʾ�Ļ�)
const int WE_CW_INPUT_CLEARONCE				= 10;  	//���һ���Ի���
//------------------------------------------------------------------------