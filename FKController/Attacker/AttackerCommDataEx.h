/**
*	created:		2012-6-24   22:31
*	filename: 		AttackerCommDataEx
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ṹ���𹥻����߳�����������߳�ͨ��
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "AttackerCommData.h"
#include <vector>
#include <string>
using std::vector;
using std::string;
//------------------------------------------------------------------------
struct SITEDATA   //��¼ÿ��վ�������, ����URL, ����Ƶ�ʵ�
{
	string strURL;       //URL
	int    interval;     //�������
	long   lasttime;     //���һ�ι���ʱ��
};
//------------------------------------------------------------------------
class CAttackerCommDataEx :public AttackerCommData
{
private:
	vector<SITEDATA> m_vtSiteData;   		//��¼Ҫ������վ������
	int m_nAttackIndex;              		//�ϴι���������վ������ 
	int m_nThread;                   		//�̵߳�������
public:
	CAttackerCommDataEx();
	virtual ~CAttackerCommDataEx();
	
	BOOL  AddSite(const char *pData);		//���һ��������վ��
	LPCSTR GetAvalibleSite(void);           //ȡ�ɹ�����վ��ָ��,û�пɹ�����վʱ,����NULL
	int GetThreadID(void)const;
};
//------------------------------------------------------------------------