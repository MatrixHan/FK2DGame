/**
*	created:		2012-6-23   0:19
*	filename: 		IThing
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IThingClass.h"
#include "../Common/Macro.h"
//------------------------------------------------------------------------
// ����
class  IEventVoteListener;
class  IEventActionListener;
class  IEventResponseListener;
//------------------------------------------------------------------------
// ����ӿ�
struct IThing
{
	// �ͷ�
	virtual void			Release(void) = NULL;
	// ����UID
	virtual void			SetUID(UID uid) = NULL;
	// ȡ��UID
	virtual UID				GetUID(void) = NULL;
	// ȡ�õ�ͼID, Ҳ���Ǹ���UID����������
	virtual DWORD			GetMapID(void) = NULL;
	// ����λ��
	virtual void			SetLoc(POINTS ptLoc) = NULL;
	// ȡ��λ��
	virtual POINTS			GetLoc(void) = NULL;
	// ȡ������
	virtual IThingClass *	GetThingClass(void) = NULL;

	/* 
	˽�ж����ֳ�����
	˵����
	˽�ж����ֳ���ָ�û��Լ��Ľ�ɫ��װ���ȵ���ϸ��Ϣ���������ǿ�������
	��dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ���
	�磺�û���¼ʱ��������ȡ����������ֳ�
	�ͻ��˸���dwClassID�������󣬵���OnGetPrivateContext()���ö����ֳ�
	*/
	virtual BOOL			OnSetPrivateContext(LPCSTR buf, int nLen) = NULL;
	virtual BOOL			OnGetPrivateContext(LPSTR buf, int &nLen) = NULL;
	/*
	���󹫿��ֳ�����
	˵����
	�����ֳ�Ϊ����������GameWorld�е���������鿴�Ķ�����Ϣ�����ɫ����HP��װ����״��
	������ȡ�ö����ֳ�����dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ��ˣ�
	�ͻ��˸���dwThingClassID�������󣬵���OnSetContext()���ö����ֳ�	
	*/
	virtual BOOL			OnSetPublicContext(LPCSTR buf, int nLen) = NULL;
	virtual BOOL			OnGetPublicContext(LPSTR buf, int &nLen) = NULL;
	
	// ִ������
	virtual int				OnCommand(DWORD dwCommand, LPCSTR buf = NULL, int nLen = 0) = NULL;
	// ��Ϣ֪ͨ, ��ʹ�߼����������ת
	virtual void			OnMessage(DWORD dwMsg, LPCSTR buf, int nLen) = NULL;
	// ������Ӧ
	virtual void			OnResponse(DWORD dwMsg, LPCSTR buf, int nLen) = NULL;

	// ��  ��: ���ı�����ķ���¼�
	// ��  ��: [����] DWORD dwMsg - �¼�ID
	// ��  ��: [����] IEventVoteListener * pListener - �¼������
	// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
	virtual BOOL			SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32] = NULL) = NULL;

	// ��  ����ȡ�����ı�����ķ���¼�,����delete pListenerָ��
	// ��  ����[����] DWORD dwMsg  - �¼�ID
	// ��  ����[����] IEventVoteListener * pListener  - �¼������	
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
	virtual BOOL			UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener) = NULL;

	// ��  ��: ���ı��������Ϊ�¼�
	// ��  ��: [����] DWORD dwMsg - �¼�ID
	// ��  ��: [����] IEventActionListener * pListener - �¼���Ϊִ����
	// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE	
	virtual BOOL			SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32] = NULL) = NULL;
	
	// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
	// ��  ����[����] DWORD dwMsg  - �¼�ID
	// ��  ����[����] IEventActionListener * pListener  - �¼���Ϊִ����
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ	
	virtual BOOL			UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener) = NULL;

	// ��  ��: ���ı��������Ӧ�¼�
	// ��  ��: [����] DWORD dwMsg - �¼�ID
	// ��  ��: [����] IEventResponseListener * pListener - �¼���Ӧ��
	// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE	
	virtual BOOL			SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32] = NULL) = NULL;
	
	// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
	// ��  ����[����] DWORD dwMsg  - �¼�ID
	// ��  ����[����] IEventResponseListener * pListener  - �¼���Ӧ��
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ	
	virtual BOOL			UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener) = NULL;
};
//------------------------------------------------------------------------