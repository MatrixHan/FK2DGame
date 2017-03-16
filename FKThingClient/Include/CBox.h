/**
*	created:		2012-7-3   0:39
*	filename: 		CBox
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IBox.h"
#include "CThingClass.h"
#include "../../FKGame/Common/BoxProp.h"
#include <vector>
using std::vector;
//------------------------------------------------------------------------
class CBox : public IBox
{
public:
	// �ͷ�
	virtual void Release(void);

	// ����UID
	virtual void SetUID(UID uid);

	// ȡ��UID	
	virtual UID GetUID(void);

	// ȡ�õ�ͼID, Ҳ���Ǹ���UID����������
	virtual DWORD GetMapID(void);

	// ����λ��
	virtual void SetLoc(POINTS ptLoc);

	// ȡ��λ��
	virtual POINTS GetLoc(void);

	// ȡ������
	virtual IThingClass* GetThingClass(void);

	/* 
	˽�ж����ֳ�����
	˵����
	˽�ж����ֳ���ָ�û��Լ��Ľ�ɫ��װ���ȵ���ϸ��Ϣ���������ǿ�������
	��dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ���
	�磺�û���¼ʱ��������ȡ����������ֳ�
	�ͻ��˸���dwClassID�������󣬵���OnGetPrivateContext()���ö����ֳ�
	*/
	virtual BOOL OnSetPrivateContext(LPCSTR buf, int nLen);
	virtual BOOL OnGetPrivateContext(LPSTR buf, int &nLen);
	
	/*
	���󹫿��ֳ�����
	˵����
	�����ֳ�Ϊ����������GameWorld�е���������鿴�Ķ�����Ϣ�����ɫ����HP��װ����״��
	������ȡ�ö����ֳ�����dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ��ˣ�
	�ͻ��˸���dwThingClassID�������󣬵���OnSetContext()���ö����ֳ�	
	*/
	virtual BOOL OnSetPublicContext(LPCSTR buf, int nLen);
	virtual BOOL OnGetPublicContext(LPSTR buf, int &nLen);	

	// ִ������
	virtual int OnCommand(DWORD dwCommand, LPCSTR buf, int nLen);

	// ��Ϣ֪ͨ, ��ʹ�߼����������ת��ֻ��FireVote��FireAction)
	virtual void OnMessage(DWORD dwMsg, LPCSTR buf, int nLen);

	// ������Ӧ
	virtual void OnResponse(DWORD dwMsg, LPCSTR buf, int nLen);

	// ��  ��: ���ı�����ķ���¼�
	// ��  ��: [����] DWORD dwMsg - �¼�ID
	// ��  ��: [����] IEventVoteListener * pListener - �¼������
	// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
	virtual BOOL SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32] = NULL);

	// ��  ����ȡ�����ı�����ķ���¼�,����delete pListenerָ��
	// ��  ����[����] DWORD dwMsg  - �¼�ID
	// ��  ����[����] IEventVoteListener * pListener  - �¼������	
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
	virtual BOOL UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener);

	// ��  ��: ���ı��������Ϊ�¼�
	// ��  ��: [����] DWORD dwMsg - �¼�ID
	// ��  ��: [����] IEventActionListener * pListener - �¼���Ϊִ����
	// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE	
	virtual BOOL SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32] = NULL);
	
	// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
	// ��  ����[����] DWORD dwMsg  - �¼�ID
	// ��  ����[����] IEventActionListener * pListener  - �¼���Ϊִ����
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ	
	virtual BOOL UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener);

	// ��  ��: ���ı��������Ӧ�¼�
	// ��  ��: [����] DWORD dwMsg - �¼�ID
	// ��  ��: [����] IEventResponseListener * pListener - �¼���Ӧ��
	// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE	
	virtual BOOL SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32] = NULL);
	
	// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
	// ��  ����[����] DWORD dwMsg  - �¼�ID
	// ��  ����[����] IEventResponseListener * pListener  - �¼���Ӧ��
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ	
	virtual BOOL UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener);

	virtual void Accept(IMapItemVisitor &visitor);	
	
	// ȡ�õ�ͼ���ָ��
	virtual CMapItemView* GetMapItem() const;
	
	// ������ͼ
	virtual void SetMapItem(CMapItemView * pMapItemView);	

	// �õ�����
	virtual LPCSTR GetName() const;
	virtual BOOL Open();	
	virtual void Close();	
	virtual void Respond(LPCSTR pBuf, int nLen);
	virtual DWORD GetType() const;
	virtual DWORD GetID() const;
	virtual void SetRight(BOOL bRight);		
	virtual BOOL IsCanOpen(void);

	// ����
	CBox();

	// ����
	~CBox();

	// ����
	BOOL Create();

private:
	void PrepareOpen(DWORD dwOpenTime);
	void OpenBox(LPCSTR pBuf, int nLen);
	void CloseBox();
	void ClearBox();
	void Command(LPCSTR pBuf, int nLen);

private:
	UID								m_uid;
	// ���Ա�
	SBOX_PROPERTY_LIST		        m_Property;
	// ��ͼ���
	CMapItemView *					m_pMapItemView;
	// �߼��������
	CThingClass						m_ThingClass;
	// �����������
	vector<DWORD>                   m_objects;
};
//------------------------------------------------------------------------