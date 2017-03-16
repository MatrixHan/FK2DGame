/**
*	created:		2012-6-26   0:45
*	filename: 		CClientWorld
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ͻ�����Ϸ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IClientWorld.h"
#include "../SceneObj/CThingTable.h"
#include "../Controller/IController.h"
#include "../Event/IEvent.h"
//------------------------------------------------------------------------
class CMainActionDataReceiver : public IDataReceiver
{
public:
	virtual BOOL	OnReceived(const char* pData, int nLen);	
};
//------------------------------------------------------------------------
class CPrivateContextDataReceiver : public IDataReceiver
{
public:
	virtual BOOL	OnReceived(const char* pData, int nLen);
};
//------------------------------------------------------------------------
class CPublicContextDataReceiver : public IDataReceiver
{
public:
	virtual BOOL	OnReceived(const char* pData, int nLen);
};
//------------------------------------------------------------------------
class CChangeMapDataReceiver : public IDataReceiver
{
public:
	virtual BOOL	OnReceived(const char* pData, int nLen);
};
//------------------------------------------------------------------------
class CCountFeeDataReceiver : public IDataReceiver
{
public:
	virtual BOOL	OnReceived(const char* pData, int nLen);
};
//------------------------------------------------------------------------
// ��  ����С�˴���
class CReselectHeroActionListener : public IEventActionListener
{
public:
	virtual void OnAction(IEvent * pEvent);
};
//------------------------------------------------------------------------
// ��  �����ͻ�����Ϸ����
class CClientWorld : public IClientWorld
{
public:
	// �ر�
	virtual void					Close(void);

	// ��Ϣ
	virtual void					OnMessage(DWORD dwKeyMain, LPCSTR buf, int nLen);

	// ȡ���߼����������
	virtual IThingTable *			GetThingTable(void);

	// ȡ�õ�ǰ��̬��ͼID
	virtual int						GetCurStaticMapID(void);

	// ��ʼ��
	BOOL							Init(void);

	// ����
	BOOL							Create(void);

	CClientWorld(void);

	~CClientWorld(void);

private:
	// �ͻ����߼���������б�
	CThingTable						m_ThingTable;
public:
	// ��Ϣͨ��
	CMainActionDataReceiver			m_MainActionDataReceiver;
	CPrivateContextDataReceiver		m_PrivateContextDataReceiver;
	CPublicContextDataReceiver		m_PublicContextDataReceiver;
	CChangeMapDataReceiver			m_ChangeMapDataReceiver;
	CReselectHeroActionListener		m_ReselectHeroActionListener;
	CCountFeeDataReceiver			m_CountFeeDataReceiver;
};
//------------------------------------------------------------------------