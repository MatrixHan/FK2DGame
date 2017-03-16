/**
*	created:		2012-6-25   0:39
*	filename: 		GameController
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��Ϸ�߼�����,������ּ����������,������Ϸ����
*/
//------------------------------------------------------------------------
#include "IControlModel.h"
#include "../../FKGame/Controller/IController.h"
#include "../../FKGame/Network/IClientSocket.h"
#include "../../FKTrace/IFKTrace.h"
#include "../ValidationCode/WndCallback.h"
#include "BaseControlMode.h"
//------------------------------------------------------------------------
//����createConnection() ʱ,,ָ��������һ���շ�״̬
#define SINKSTATE_LOGIN         0
#define SINKSTATE_ACTORSELECT   1
#define SINKSTATE_SCENE         2
#define SINKSTATE_RUNNING       3
#define SINKSTATE_MAPCHINGING   4
//------------------------------------------------------------------------
#define FKTEST_MSG_CENTER_TYPE_MAX 7
#ifndef MSG_CENTER_TYPE_MAX
#define MSG_CENTER_TYPE_MAX FKTEST_MSG_CENTER_TYPE_MAX
#endif
//------------------------------------------------------------------------
// ��Ϸ������Ϣ������
class CGameControl : public IController
{
private:
	struct DataCenterMessage
	{
		int nMsgID;            
		char *pBuffer;
		int nLen;
	};
	std::list<DataCenterMessage> m_lsQueue[MSG_CENTER_TYPE_MAX];
	int m_nMsgID_Count;    //��ϢID������

public:
	virtual LRESULT     OnRButtonUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT     OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT		OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT		OnRButtonDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT		OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual LRESULT		OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT		OnKeyUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT		OnCommand(DWORD dwCmd,DWORD dwParam1,DWORD dwParam2);

	// ����������ݼӽ��� bLock: TRUE->���� FALSE->����
	virtual void		LockReceiver(BOOL bLock);
	// �����Ƿ�ʹ���̴߳�������
	virtual void		SetThreadRun(BOOL bRun);

	virtual int			Loop();
	// �ͷŹ���
	virtual void		Release(void);
public:
	CGameControl();
	virtual ~CGameControl();

	// ��������
	BOOL				Create(void);

	/*������������  , SinkState ָ��������һ���۵�����, 
	   SINKSTATE_LOGIN        -- ��¼   
	   SINKSTATE_ACTORSELECT  --����ѡ��    
	   SINKSTATE_SCENE        --������     
	   SINKSTATE_RUNNING      --����
    */
	BOOL				createConnection(int SinkState);

	//���Ŀ���ģʽ
	virtual BOOL changeControlMode(int mode);

	//��Ҫ�������ȡ���ݵ�ģ�� , ͨ���˺���ע���,�ſ��յ���Ϣ 
	virtual BOOL AddReceiver(IDataReceiver *pReceiver, int MainMsgCode, int SubMsgCode) ;
	
	//�Ƴ�ģ�������
	virtual BOOL RemoveReceiver(IDataReceiver *pReceiver, int MainMsgCode, int SubMsgCode) ;
	
	virtual int  PushBufferToDataCenterQueue(int nQueueID, LPCSTR pBuffer, int nLen) ;
	
	virtual BOOL PopBufferFromDataCenterQueue(int nQueueID);
	
	virtual BOOL DeleteDataFromDataCenterQueue(int nIndex) ;
	
	virtual BOOL ClearDataFromDataCenterQueue(int nQueueID);

	virtual BOOL IsAutoOperate(void);

private:
	CBaseControlMode    m_ControlMode1;
	IControlMode        m_DefaultMode;
 	IControlMode      * m_pControlMode;
	CIWndMgrHelper		m_WndHelper;
	CWndCallback		m_WndCallback;
	CClientLinkHelper	m_SocketHelper;

	CWMConnection<CGameControl> m_MsgCenterPop;    //����֪ͨ��Ϣ����,������Ϣ
	void OnMsgCenterClicked( WPARAM wParam, LPARAM lParam,BOOL &bContinue);		
};
//------------------------------------------------------------------------
extern CGameControl * g_pGameControl;
//------------------------------------------------------------------------