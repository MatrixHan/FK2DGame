/**
*	created:		2012-7-3   22:52
*	filename: 		CEquipment
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		װ��Ʒ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/SceneObj/IGoods.h"
#include "../../FKGame/SceneObj/IEquipment.h"
#include "../../FKGame/Common/GoodsPropertyDef.h"
#include "../Include/CThingClass.h"
//------------------------------------------------------------------------
class CEquipment : public IEquipment
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


	virtual void				Accept(IMapItemVisitor &visitor);
	
	// ȡ�õ�����������ͼ�ӿ�
	virtual CMapItemView *		GetMapItem(void) const;

	// ������ͼ
	virtual void				SetMapItem(CMapItemView * pMapItemView);

	// ȡ����ƷID
	virtual DWORD				GetGoodsID(void);

	// ȡ����Ʒ����
	virtual LPCSTR				GetName(void);

	// ȡ����Ʒ������������
	virtual int					GetPropNum(DWORD dwPropID);

	// ȡ����Ʒ���ַ�������
	virtual LPCSTR				GetPropStr(DWORD dwPropID);

	// ȡ����Ʒ����
	virtual DWORD				GetNumber(void);

	// ȡ����Ʒ��ʹ������
	virtual DWORD				GetUseLock(void);

	// ȡ�ð󶨱�־�������鿴�󶨱�־
	virtual  BOOL				GetBindFlag(DWORD dwFlag);

	// ȡ���;������ֵ
	virtual void				GetEndure(int &nCur, int &nCurMax);

	// ȡ��Ч��
	virtual IEffect *			GetEffect(int nIndex);

	// ����Ƿ���װ������װ��
	virtual	BOOL				CanEquip(ICreature * pCreature, int nEquipPos);

	// ȡ�þ����ȼ�
	virtual int					GetSmeltLevel(void);

	// ȡ��װ���Ĳ����
	virtual DWORD				GetSlotLeft(void);

	// ����
	BOOL						Create(void);

	CEquipment(void);

	~CEquipment(void);
	
private:
	// �������Ч��
	void						ClearEffect(void);

private:
	UID							m_uid;
	// ���Ա�
	SEQUIPMENT_PROPERTY_PRIVATE	m_Property;
	// ����
	char						m_szName[MAX_NAME_LEN];
	// �߼��������
	CThingClass					m_ThingClass;
	// Ч��
	IEffect *					m_Effect[GOODS_EFFECT_MAXCOUNT];
};

//------------------------------------------------------------------------

