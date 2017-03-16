/**
*	created:		2012-7-3   23:19
*	filename: 		CStall
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		̯λ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/Common/StallProp.h"
#include "../Include/IStall.h"
#include "../Include/CThingClass.h"
//------------------------------------------------------------------------
class CStall : public IStall
{
public:
	// �ͷ�
	virtual void				Release(void);

	// ����UID
	virtual void				SetUID(UID uid);

	// ȡ��UID
	virtual UID					GetUID(void);

	// ȡ�õ�ͼID, Ҳ���Ǹ���UID����������
	virtual DWORD				GetMapID(void);

	// ����λ��
	virtual void				SetLoc(POINTS ptLoc);

	// ȡ��λ��
	virtual POINTS				GetLoc(void);

	// ȡ������
	virtual IThingClass *		GetThingClass(void);

	/* 
	˽�ж����ֳ�����
	˵����
	˽�ж����ֳ���ָ�û��Լ��Ľ�ɫ��װ���ȵ���ϸ��Ϣ���������ǿ�������
	��dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ���
	�磺�û���¼ʱ��������ȡ����������ֳ�
	�ͻ��˸���dwClassID�������󣬵���OnGetPrivateContext()���ö����ֳ�
	*/
	virtual BOOL				OnSetPrivateContext(LPCSTR buf, int nLen);
	virtual BOOL				OnGetPrivateContext(LPSTR buf, int &nLen);
	
	/*
	���󹫿��ֳ�����
	˵����
	�����ֳ�Ϊ����������GameWorld�е���������鿴�Ķ�����Ϣ�����ɫ����HP��װ����״��
	������ȡ�ö����ֳ�����dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ��ˣ�
	�ͻ��˸���dwThingClassID�������󣬵���OnSetContext()���ö����ֳ�	
	*/
	virtual BOOL				OnSetPublicContext(LPCSTR buf, int nLen);
	virtual BOOL				OnGetPublicContext(LPSTR buf, int &nLen);
	
	/*
	����thing�����ݿ��ֳ�
	˵����
	OnGetDBContext() ���ڽ������ֳ����������ݿ�
	OnSetDBContext() ����ͨ�����ݿⷵ�ص����ݴ�������	
	*/
	virtual BOOL				OnSetDBContext(LPCSTR buf, int nLen);
	virtual BOOL				OnGetDBContext(LPSTR buf, int &nLen);

	// ִ������
	virtual int					OnCommand(DWORD dwCommand, LPCSTR buf, int nLen);

	// ��Ϣ֪ͨ, ��ʹ�߼����������ת
	virtual void				OnMessage(DWORD dwMsg, LPCSTR buf, int nLen);

	// ������Ӧ��FireResponse��
	virtual void				OnResponse(DWORD dwMsg, LPCSTR buf, int nLen);

	// ��  ��: ���ı�����ķ���¼�
	// ��  ��: [����] DWORD dwMsg - �¼�ID
	// ��  ��: [����] IEventVoteListener * pListener - �¼������
	// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
	virtual BOOL				SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32] = NULL);
	
	// ��  ����ȡ�����ı�����ķ���¼�,����delete pListenerָ��
	// ��  ����[����] DWORD dwMsg  - �¼�ID
	// ��  ����[����] IEventVoteListener * pListener  - �¼������	
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
	virtual BOOL				UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener);
	
	// ��  ��: ���ı��������Ϊ�¼�
	// ��  ��: [����] DWORD dwMsg - �¼�ID
	// ��  ��: [����] IEventActionListener * pListener - �¼���Ϊִ����
	// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE	
	virtual BOOL				SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32] = NULL);
	
	// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
	// ��  ����[����] DWORD dwMsg  - �¼�ID
	// ��  ����[����] IEventActionListener * pListener  - �¼���Ϊִ����
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ	
	virtual BOOL				UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener);
	
	// ��  ��: ���ı��������Ӧ�¼�
	// ��  ��: [����] DWORD dwMsg - �¼�ID
	// ��  ��: [����] IEventResponseListener * pListener - �¼���Ӧ��
	// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE	
	virtual BOOL				SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32] = NULL);
	
	// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
	// ��  ����[����] DWORD dwMsg  - �¼�ID
	// ��  ����[����] IEventResponseListener * pListener  - �¼���Ӧ��
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ	
	virtual BOOL				UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener);

	// visitor
	virtual void				Accept(IMapItemVisitor &visitor);
	
	// ȡ�õ�����������ͼ�ӿ�
	virtual CMapItemView *		GetMapItem(void) const;

	// ������ͼ
	virtual void				SetMapItem(CMapItemView * pMapItemView);

	// ȡ��̯��
	virtual UID					GetMaster(void);
	
public:
	// ����
	BOOL						Create(void);

	CStall(void);
	virtual ~CStall(void);

private:
	// ��������
	void						SetSloganBmp(void);

private:
	UID							m_uid;
	// ���Ա�
	SSTALL_PROPERTY_LIST		m_Property;
	// ��ͼ���
	CMapItemView *				m_pItemView;
	// �߼��������
	CThingClass					m_ThingClass;
	// ������Դ
	static IBitmap *			m_pSloganBmpRes;
	IBitmap *					m_pSloganBmp;
};
//------------------------------------------------------------------------