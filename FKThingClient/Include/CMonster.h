/**
*	created:		2012-7-3   1:47
*	filename: 		CMonster
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/SceneObj/ICreature.h"
#include "../../FKGame/SceneObj/IMonster.h"
#include "../../FKGame/SceneObj/IThingPart.h"
#include "../../FKGame/Common/ICreatureCommand.h"
#include "../../FKController/DataMgr/CDataUnit.h"
#include "../../FKController/DataMgr/CDataChunk.h"
#include "../../FKController/DataMgr/CDataStream.h"
#include "CThingClass.h"
#include "../../FKGame/Common/CreatureViewDef.h"
//------------------------------------------------------------------------
// ����
class CMonster : public IMonster,
				 public ICreatureSink
{
public:
	// �ͷ�
	virtual void			Release(void);

	// ����UID
	virtual void			SetUID(UID uid);

	// ȡ��UID
	virtual UID				GetUID(void);

	// ȡ�õ�ͼID, Ҳ���Ǹ���UID����������
	virtual DWORD			GetMapID(void);

	// ����λ��
	virtual void			SetLoc(POINTS ptLoc);

	// ȡ��λ��
	virtual POINTS			GetLoc(void);

	// ȡ������
	virtual IThingClass *	GetThingClass(void);

	/* 
	˽�ж����ֳ�����
	˵����
	˽�ж����ֳ���ָ�û��Լ��Ľ�ɫ��װ���ȵ���ϸ��Ϣ���������ǿ�������
	��dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ���
	�磺�û���¼ʱ��������ȡ����������ֳ�
	�ͻ��˸���dwClassID�������󣬵���OnGetPrivateContext()���ö����ֳ�
	*/
	virtual BOOL			OnSetPrivateContext(LPCSTR buf, int nLen);
	virtual BOOL			OnGetPrivateContext(LPSTR buf, int &nLen);
	
	/*
	���󹫿��ֳ�����
	˵����
	�����ֳ�Ϊ����������GameWorld�е���������鿴�Ķ�����Ϣ�����ɫ����HP��װ����״��
	������ȡ�ö����ֳ�����dwThingClassID + uid + wDataLen + data[wDataLen] ����ʽ�����ͻ��ˣ�
	�ͻ��˸���dwThingClassID�������󣬵���OnSetContext()���ö����ֳ�	
	*/
	virtual BOOL			OnSetPublicContext(LPCSTR buf, int nLen);
	virtual BOOL			OnGetPublicContext(LPSTR buf, int &nLen);
	
	/*
	����thing�����ݿ��ֳ�
	˵����
	OnGetDBContext() ���ڽ������ֳ����������ݿ�
	OnSetDBContext() ����ͨ�����ݿⷵ�ص����ݴ�������	
	*/
	virtual BOOL			OnSetDBContext(LPCSTR buf, int nLen);
	virtual BOOL			OnGetDBContext(LPSTR buf, int &nLen);

	// ִ������
	virtual int				OnCommand(DWORD dwCommand, LPCSTR buf = NULL, int nLen = 0);

	// ��Ϣ֪ͨ, ��ʹ�߼����������ת
	virtual void			OnMessage(DWORD dwMsg, LPCSTR buf, int nLen);

	// ������Ӧ
	virtual void			OnResponse(DWORD dwMsg, LPCSTR buf, int nLen);

	// ��  ��: ���ı�����ķ���¼�
	// ��  ��: [����] DWORD dwMsg - �¼�ID
	// ��  ��: [����] IEventVoteListener * pListener - �¼������
	// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
	virtual BOOL			SubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener, const char szDescription[32] = NULL);
	
	// ��  ����ȡ�����ı�����ķ���¼�,����delete pListenerָ��
	// ��  ����[����] DWORD dwMsg  - �¼�ID
	// ��  ����[����] IEventVoteListener * pListener  - �¼������	
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ
	virtual BOOL			UnSubscibeMessage(DWORD dwMsg, IEventVoteListener * pListener);
	
	// ��  ��: ���ı��������Ϊ�¼�
	// ��  ��: [����] DWORD dwMsg - �¼�ID
	// ��  ��: [����] IEventActionListener * pListener - �¼���Ϊִ����
	// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE	
	virtual BOOL			SubscibeMessage(DWORD dwMsg, IEventActionListener * pListener, const char szDescription[32] = NULL);
	
	// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
	// ��  ����[����] DWORD dwMsg  - �¼�ID
	// ��  ����[����] IEventActionListener * pListener  - �¼���Ϊִ����
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ	
	virtual BOOL			UnSubscibeMessage(DWORD dwMsg, IEventActionListener * pListener);
	
	// ��  ��: ���ı��������Ӧ�¼�
	// ��  ��: [����] DWORD dwMsg - �¼�ID
	// ��  ��: [����] IEventResponseListener * pListener - �¼���Ӧ��
	// ��  ��: [����] const char szDescription[32]  - ������������Ϣ,���Ծ�����ߵ�����, �ѱ����
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE	
	virtual BOOL			SubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener, const char szDescription[32] = NULL);
	
	// ��  ����ȡ�����ı��������Ϊ�¼�,����delete pListenerָ��
	// ��  ����[����] DWORD dwMsg  - �¼�ID
	// ��  ����[����] IEventResponseListener * pListener  - �¼���Ӧ��
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE�˶���Ϣ	
	virtual BOOL			UnSubscibeMessage(DWORD dwMsg, IEventResponseListener * pListener);


	virtual void			Accept(IMapItemVisitor &visitor);
	
	// ȡ�õ�����������ͼ�ӿ�
	virtual CMapItemView *	GetMapItem(void) const;

	// ������ͼ
	virtual void			SetMapItem(CMapItemView * pMapItemView);

	// ȡ����������
	virtual LPCSTR			GetName(void);
	
	// �������������������	
	virtual int				SetPropNum(DWORD dwPropID, int nValue);

	// ȡ�����������������
	virtual int				GetPropNum(DWORD dwPropID);

	// ����������ַ�������	
	virtual LPCSTR			SetPropStr(DWORD dwPropID, LPCSTR strPropValue);

	// ȡ��������ַ�������
	virtual LPCSTR			GetPropStr(DWORD dwPropID);

	// ������ﲿ��, ����Ѵ���,������ʧ��
	virtual BOOL			AddPart(IThingPart * pPart);
	
	// ɾ�����ﲿ��, �����ͷ�
	virtual BOOL			RemovePart(DWORD dwPartID);
	
	// ȡ�����ﲿ��
	virtual IThingPart *	GetPart(DWORD dwPartID);

	// ���õ�ǰ�ж�״̬
	virtual BOOL			GotoActState(DWORD dwActStateID, LPCSTR pContext = NULL, int nLen = 0);

	// ȡ�õ�ǰ�ж�״̬
	virtual ICreatureActState * GetCurActState(void);

	// ����һ��������
	virtual BOOL			AddCreatureCommand(ICreatureCommand * pCreatureCommand);

	// ȡ������ͷ��
	virtual IBitmap *		GetHeadBitmap(void);

	// ���ô�������״̬
	virtual void			SetDying(BOOL bDying);

	// �Ƿ�����������
	virtual BOOL			IsDying(void);

	// ȡ�ù���ID
	virtual DWORD			GetMonsterID(void);


	virtual void			OnCreatureMoveToTileCenter(const POINTS &ptCurTile, 
													   const POINTS &ptNextTile);

public:
	// ���캯��
	CMonster(void);
	// ��������
	~CMonster(void);

	// ����
	BOOL					Create(void);
	
protected:
	UID						m_uid;
	// ���Ա�
	SMONSTER_PROPERTY_PUBLIC m_Property;
	// �߼��������
	CThingClass				m_ThingClass;
	// �Ƿ�������̬
	BOOL					m_bIsDying;
	int						m_nLevel;
	int						m_nHPMaxCur;
	int						m_nMPMaxCur;

	CMapItemView *			m_pItemView;
	CMapItemView *			m_pItemHead;

	IThingPart *			m_ThingPart[THINGPARTID_THING_MAXID];
};
//------------------------------------------------------------------------