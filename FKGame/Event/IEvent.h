/**
*	created:		2012-6-24   21:20
*	filename: 		IEvent
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�¼�ϵͳ�ӿ�ͷ�ļ�
*/
//------------------------------------------------------------------------
/*
	�¼�ϵͳʹ��˵��:
	�����¼�������
	CEventServerHelper EventServerHelper;
	if(EventServerHelper.Create(pResMgr, pFileName, bEncrypt))
	{
		//...
	}

	�����߾���
	class CMoveAction : public IEventActionListener
	{
		virtual void OnAction(IEvent *pEvent)
		{
			//...
		}
	};

	CMoveAction MoveAction;
	if (EventServerHelper.IsValid())
	{
		EventServerHelper->AddListener(this, MSG_THING_MOVE, SRC_TYPE_ID_PLAYER, 0,
		"CMoveAction") ;
	}

	�����߾���1
	class CMoveEvent : public IEvent
	{
	public:
		CMoveEvent(CPlay *pPlayer):m_pPlayer(pPlayer){}
		WORD GetID() {return MSG_THING_MOVE;}	
		BYTE GetSrcTypeID() {return SRC_TYPE_ID_PLAYER;}
		DWORD GetSrcPointer() {return (DWORD)m_pPlayer;}
		BYTE* GetContext(int &nLen) {nLen = 0; return NULL;}
	private:
		CPlayer *m_pPlayer;
	}

	CPlayer Player;
	CMoveEvent MoveEvent;
	if (EventServerHelper.IsValid())
	{
		EventServerHelper->FireAction(&MoveEvent);
	}

	�����߾���2 
	struct SContext
	{
	//..
	};
	SContext Context;
	if (EventServerHelper.IsValid())
	{
	EventServerHelper->FireAction(&Context, sizeof(SContext),
		MSG_THING_MOVE, SRC_TYPE_ID_PLAYER, (DWORD)&Player);
	}
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <windows.h>
//------------------------------------------------------------------------
enum
{
	SRC_TYPE_ID_ROOT = 0,			//����ID ��
	SRC_TYPE_ID_THING = 1,			//THING
	SRC_TYPE_ID_CREATURE =2,		//����
	SRC_TYPE_ID_ACTOR = 3,			//���
	SRC_TYPE_ID_MONSTER = 4,		//����
	SRC_TYPE_ID_CHAT = 5,			//����
	SRC_TYPE_ID_GOODS = 6,			//��Ʒ
	SRC_TYPE_ID_GOODS_CONTAINER = 7,//ͨ����Ʒ��
	SRC_TYPE_ID_FRIEND = 8,			//����
	SRC_TYPE_ID_CHUNNEL = 9,		//������
	SRC_TYPE_ID_BOX = 10,			//����
	SRC_TYPE_ID_STATUS = 11,		//״̬
	SRC_TYPE_ID_TEAM = 12,			//��� 
	SRC_TYPE_ID_SYSTEM = 13,		//ϵͳ�¼�
	SRC_TYPE_ID_RESOURCE = 14,		//��Դ	
	SRC_TYPE_ID_STALL = 15,			//̯λ
	SRC_TYPE_ID_CLAN  = 16,         //����
	SRC_TYPE_ID_RELATION = 17,		//����ϵ
	MAX_SRC_TYPE_ID,
};
//------------------------------------------------------------------------
class IEventVoteListener;
class IEventActionListener;
class IEventResponseListener;
//------------------------------------------------------------------------
class IEvent
{
public:
	virtual ~IEvent(){};
	//��ȡ�¼�ID
	virtual WORD GetID() = NULL;
	//��ȡ�¼�Դ����ID
	virtual BYTE GetSrcTypeID() = NULL;
	//��ȡ�¼�Դָ��
	virtual DWORD GetSrcPointer() = NULL;
	// ��  ������ȡ�ֳ�
	// ��  ����[���] int &nLen  - �ֳ���С	
	// ����ֵ��BYTE* - �ֳ�ָ��
	virtual BYTE* GetContext(int &nLen) = NULL;
};
//------------------------------------------------------------------------
class CEventServerHelper;
//------------------------------------------------------------------------
class IEventServer
{
public:
	virtual ~IEventServer(){}
	//��������¼�
	virtual BOOL FireVote(IEvent *pEvent) = NULL;
	virtual BOOL FireVote(BYTE *pContext, int nLen,
		WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer) = NULL;
	//������Ϊ
	virtual void FireAction(IEvent *pEvent) = NULL;
	virtual void FireAction(BYTE *pContext, int nLen,
		WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer) = NULL;
	//������Ӧ
	virtual void FireResponse(IEvent *pEvent) = NULL;
	virtual void FireResponse(BYTE *pContext, int nLen,
		WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer) = NULL;

	// ��  ��������Listener,���ķ���¼�
	// ��  ����[���] IEventVoteListener *pListener  - �¼������	
	// ��  ����[���] WORD wEventID  - �¼�ID
	// ��  ����[���] DWORD dwSrcPointer  - �¼�Դָ�� 	
	// ��  ����[���] const char *pDescription  - ������������Ϣ,���Ծ�����ߵ�����	
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
	virtual BOOL AddListener(IEventVoteListener *pListener,
		WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer = 0,
		const char *pDescription = NULL) = NULL;

	// ��  ��������Listener,������Ϊ�¼�
	// ��  ����[���] IEventActionListener *pListener  - �¼���Ϊִ����	
	// ��  ����[���] WORD wEventID  - �¼�ID
	// ��  ����[���] DWORD dwSrcPointer  - �¼�Դָ�� 	
	// ��  ����[���] const char *pDescription  - ������������Ϣ,���Ծ�����ߵ�����	
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
	virtual BOOL AddListener(IEventActionListener *pListener,
		WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer = 0,
		const char *pDescription = NULL) = NULL;

	// ��  ��������Listener,������Ӧ�¼�
	// ��  ����[���] IEventResponseListener *pListener  - �¼���Ӧ��	
	// ��  ����[���] WORD wEventID  - �¼�ID
	// ��  ����[���] DWORD dwSrcPointer  - �¼�Դָ�� 	
	// ��  ����[���] const char *pDescription  - ������������Ϣ,���Ծ�����ߵ�����	
	// ����ֵ��BOOL - ���ĳɹ����򷵻�TRUE,���򷵻�FALSE
	virtual BOOL AddListener(IEventResponseListener *pListener,
		WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer = 0,
		const char *pDescription = NULL) = NULL;

	// ��  ����ɾ��Listener,ȡ�����ķ���¼�,����delete pListenerָ��
	// ��  ����[���] IEventVoteListener *pListener  - �¼������	
	// ��  ����[���] WORD wEventID  - �¼�ID
	// ��  ����[���] DWORD dwSrcPointer  - �¼�Դָ�� 		
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE
	virtual BOOL RemoveListener(IEventVoteListener *pListener,
		WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer = 0) = NULL;

	// ��  ����ɾ��Listener,ȡ��������Ϊ�¼�,����delete pListenerָ��
	// ��  ����[���] IEventActionListener *pListener  - �¼���Ϊִ����	
	// ��  ����[���] WORD wEventID  - �¼�ID
	// ��  ����[���] DWORD dwSrcPointer  - �¼�Դָ�� 		
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE
	virtual BOOL RemoveListener(IEventActionListener *pListener,
		WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer = 0) = NULL;

	// ��  ����ɾ��Listener,ȡ��������Ӧ�¼�,����delete pListenerָ��
	// ��  ����[���] IEventResponseListener *pListener  - �¼���Ӧ��	
	// ��  ����[���] WORD wEventID  - �¼�ID
	// ��  ����[���] DWORD dwSrcPointer  - �¼�Դָ�� 		
	// ����ֵ��BOOL - ȡ�����ĳɹ����򷵻�TRUE,���򷵻�FALSE
	virtual BOOL RemoveListener(IEventResponseListener *pListener,
		WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer = 0) = NULL;

protected:
	virtual void Release() = NULL;
	friend CEventServerHelper;
};
//------------------------------------------------------------------------
//�¼������
class IEventVoteListener 
{
public:
	virtual BOOL OnVote(IEvent *pEvent) = NULL;	
};
//------------------------------------------------------------------------
//��Ϊִ����
class IEventActionListener 
{
public:
	virtual void OnAction(IEvent *pEvent) = NULL;
};
//------------------------------------------------------------------------
//�¼���Ӧ��
class IEventResponseListener 
{
public:
	virtual void OnResponse(IEvent *pEvent) = NULL;
};
//------------------------------------------------------------------------
struct IResMgr;
struct ITrace;
//------------------------------------------------------------------------
class CEventServerHelper
{
private:
	typedef BOOL (*ProcCreateEventServer)(IEventServer **ppEventServer, 
		IResMgr *pResMgr,ITrace *pTrace, const char *pFileName, BOOL bEncrypt);	
public:
	CEventServerHelper()
	{
		m_hDll = NULL;
		m_pEventServer = NULL;
		m_szErrMsg[0] = 0;
	}
	~CEventServerHelper()
	{		
		if (m_pEventServer != NULL)
		{
			m_pEventServer->Release();
			m_pEventServer = NULL;
		}
		if (m_hDll != NULL)//ע�⣬�����lib��ʽ������m_hDll����NULL,���������::FreeLibary()���ᱻ����
		{
			::FreeLibrary(m_hDll);
			m_hDll=NULL;
		}
	}	
	// ��  ���������¼�������
	// ��  ����[���] IResMgr *pResMgr  - ��Դ������	
	// ��  ����[���] ITrace *pTrace  - Trace	
	// ��  ����[���] const char *pFileName  - �ű��ļ���,����·��
	// ��  ����[���] BOOL bEncrypt  - �ļ��Ƿ����,��չ��Ϊscp��û�м��ܵ�,��չ��Ϊres���Ǽ��ܵ�
	// ����ֵ��BOOL - �����ɹ��򷵻�TRUE,���򷵻�FALSE,�ظ����ò����ظ�����,���ֵ���
	BOOL Create(IResMgr *pResMgr,ITrace *pTrace, const char *pFileName = NULL, BOOL bEncrypt = FALSE)//�����ӿڶ���
	{	
		if (IsValid())
		{
			//����ͬһ������
			return TRUE;
		}
		try
		{
			if (m_hDll == NULL)
			{
				//��һ�ε���ʱδ����dll,������м���dll
#ifdef _DEBUG
				m_hDll = ::LoadLibrary("FKEvent_D.dll");
				if (m_hDll == NULL)
					throw "�޷����� FKEvent_D.dll";
#else
				m_hDll = ::LoadLibrary("FKEvent.dll");
				if (m_hDll == NULL)
					throw "�޷�����FKEvent.dll";
#endif
			}
			//������dll��������������ӿڶ���Ĵ�������
			ProcCreateEventServer proc = NULL;
			proc=(ProcCreateEventServer)::GetProcAddress(m_hDll,
				"CreateEventServer");
			if (proc == NULL)
				throw "Can't GetProcAddress('CreateEventServer')";
			if (!proc(&m_pEventServer, pResMgr, pTrace, pFileName, bEncrypt))
				throw "CreateEventServer() error!";
			
			return TRUE;//�ӿڶ��󴴽��ɹ�
		}
		catch (LPCSTR szMsg) 
		{
			lstrcpyn(m_szErrMsg, szMsg, sizeof(m_szErrMsg));
			return FALSE;
		}
		catch(...)
		{
			lstrcpyn(m_szErrMsg, "Unknown Error!", sizeof(m_szErrMsg));
			return FALSE;
		}
	}
	
	//����->,��helper���������ֱ��תΪ�ӿڵ�����
	IEventServer* operator ->()
	{
		return m_pEventServer;
	}
	IEventServer* GetEventServer()
	{
		return m_pEventServer;
	}
	//�жϽӿ�ָ���Ƿ���Ч
	BOOL IsValid()
	{
		return (m_pEventServer != NULL);
	}
	char* GetErrMsg()
	{
		return m_szErrMsg;
	}
public:
	IEventServer *m_pEventServer;						//�ӿڶ���
	HINSTANCE m_hDll;									//��̬���ӿ��������־
	char m_szErrMsg[128];								//������Ϣ
};
//------------------------------------------------------------------------