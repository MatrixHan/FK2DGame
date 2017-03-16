/**
*	created:		2012-6-22   0:20
*	filename: 		IClientSocket
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#ifndef MAX_NAME_STRING_LEN
#define MAX_NAME_STRING_LEN			32								// �û��������롢��������������õ������
#endif
//------------------------------------------------------------------------
#define VERSION_SingleSocketProxy	1
#define VERSION_CLIENTLINK			2
//------------------------------------------------------------------------
// ���������������ض���
enum PROXY_TYPE
{
	proxyNone = 0, proxy4 = 4,proxy5 = 5
};
//------------------------------------------------------------------------
struct NET_PROXY_OPTION 
{
	NET_PROXY_OPTION ()
	{
		Close();
	}
	void Close()
	{
		memset(this,0,sizeof(NET_PROXY_OPTION )) ;
	}
	BOOL		bEnableProxy;								// �Ƿ�ʹ�ô��������
	PROXY_TYPE	socksType;									// ������������ͣ�Ĭ��Ϊ��֧�ִ��������
	char		szProxyServerName[MAX_NAME_STRING_LEN];		// �����������
	WORD		wProxyPort;									// ������������Ӷ˿ںţ���1080
	WORD		wReserved;
	BOOL		bCheckAuthority;							// ����������Ƿ���Ҫ�����֤
	char		szProxyUserName[MAX_NAME_STRING_LEN];		// �����Ҫ��֤����Ҫ�����������û���������
	char		szProxyPassword[MAX_NAME_STRING_LEN];
};
//------------------------------------------------------------------------
struct IEncryptData;
struct IClientSocket;
struct IClientSocketSink;
struct IEventSink;
struct IThreadRoute;
struct ITrace;
//------------------------------------------------------------------------
struct IClientSocket
{
	// ��ǰͨ����ʶ
	virtual DWORD GetSocketID() = NULL;

	// �������ӣ����ӽ�������յ�OnConnectOK()ʱ����ʾ���ӳɹ����յ�OnConnectError()ʱ��ʾ����ʧ��
	// ���أ��û��ɺ��Է��ؽ������������ʱҲ�᷵��FALSE
	virtual BOOL Connect(LPCSTR szServerAddress, WORD wServerPort,NET_PROXY_OPTION* pProxyOption = NULL) = NULL;

	// �رմ����ӣ��´ο����ٴε���Connect()��������
	// ����ص�IClientSocketSink::OnConnectClose()
	virtual BOOL CloseConnect() = NULL;

	/*
	 ���ܣ��������ݰ�
	 ���룺nLen���ܳ���MAX_SOCKET_RAW_DATA_LEN,buf����Ϊ��
	 ���أ����ʹ�����������FALSE
		   ������ͳɹ�������TRUE��WSA_IO_INCOMPLETE
				WSA_IO_INCOMPLETE��ʾ���ݷ��ͳɹ�����û���������ͳ�ȥ
				�����ļ����͵ȴ������ݴ���ʱ�����ã�Ӧ�ò���Ը����������ֵ�����ӳ��ٷ�
			    WSA_IO_INCOMPLETE��Overlapped sockets��WSASend()����WSAEWOULDBLOCK��Ϣʱ���ص�
				�����ϵ�ͬ��WSA_IO_INCOMPLETE,��ΪIClientSocket��ȴ�ʱ���ٴη���
	 ˵����1�������㷨Ϊ���������ͣ����ݷ���ͨʱ�Ž����ݷ��뷢�Ͷ��У�����WSA_IO_INCOMPLETE���ȴ��´η���
		   2�����socket����ʧ�ܣ�ϵͳ����OnConnectClose�з��س�����Ϣ����˲����ڷ��ʹ�������������
	*/
	virtual int Send(LPCSTR buf,int nLen) = NULL;

	// ���ܣ����յ�OnDataArrived()֪ͨʱ�����ܵ��ô˽ӿڽ�������
	// ���룺nBufLenΪbuf��С
	// ���أ�buf�к��г���ΪnLen�����ݣ��������TRUE����nLenΪ0����ʾ��ʱ�����ݷ���
	virtual BOOL Recv(int nMaxBufLen,/*out*/char* buf,/*out*/int& nLen) = NULL;

	// ��ѯ��ǰ�Ƿ�������״̬
	virtual BOOL IsConnected() = NULL;

	/* �л�����һ��״̬
	   �磺
		  Ӧ�ò�ĵ�¼̬������̬����������ʵ�֣���ʵ��IClientSocketSink
		  ����¼����Ժ󣬵��ô˺����ӵ�¼̬�л�������̬
	*/
	virtual void GotoNextState(IClientSocketSink* pSink) = NULL;


	// �������Ի�ȡ������һЩ�ڲ����ݣ��統ǰ���Ͷ��г��ȡ��շ��ٶȡ���ǰsocket�Ƿ������ȡ��ݲ�֧�ִ˹��ܡ�
	enum 
	{
		setMaxSendQueueCount	= 1,		// ���ͻ��������󳤶����ã�Ĭ��Ϊ10000��dwParamΪ�µĳ��ȣ�pData���ص�ǰ����
		getMaxSendQueueCount	= 2,
		getSocketHandle			= 3,		// ��ȡsocket������Ա�ֱ�Ӷ�socket���в���������setsockopt()
	};
	/*
		����Ϊ�������ܴ��Ӧ������һ������ķ��Ͷ��У�ioctrl(IClientSocket::setMaxSendQueueCount,1000000,NULL);
	*/
	virtual BOOL ioctrl(DWORD dwCmd,DWORD dwParam,/*out*/void* pData) = NULL;

	// �ͷŶ���
	virtual void Release() = NULL;
};
//------------------------------------------------------------------------
/*
 * ˵����
 *			��ΪIClientSocketSink��IThreadRoute�б��ص�������:
 *			���ڷ������˳��򣬽�����IClientSocketSink�����ڷ������߼���ʵ�֣���ʱ������
 *		����������ͬһ�̣߳����ؿ����߳�ͬ������
 *			�����ڿͻ��˳���ͨѶ�����������ͬһ�̣߳���Ҫ�����߳�ͬ������
 *
 */
struct IClientSocketSink
{
	// ���ӷ�����ʧ��
	// szMsgΪ������Ϣ�������ݡ�dwErrorCodeΪsocket������룬�μ�Win32 API : GetLastError()
	virtual void OnConnectError(DWORD dwSocketID,LPCSTR szMsg,DWORD dwErrorCode) = NULL;

	// ���ӽ���
	virtual void OnConnectOK(DWORD dwSocketID) = NULL;
	
	// �����ݿ��Խ���
	// ˵������ʱ�ײ����m_pClientSocket->Recv()��ϵͳsocket���������
	// ע��	����������ݰ����߳���IThreadRoute����ͬһ�̣߳�
	//		����һ������(ILinkWnd)�������ͨѶ����ILinkWnd���븺����һ���������ݰ�����
	virtual void OnDataArrived(DWORD dwSocketID) = NULL;

	/* ��ǰ���ӶϿ�
	   szMsgΪ������Ϣ�������ݡ�dwErrorCodeΪsocket������룬�μ�Win32 API : GetLastError()
	   ע�⣺
			1��Ӧ�ò����ڶ�����¼��������Ժ�ִ��IClientSocket::CloseConnect()����������Դ
	  		   ����������Ϊ��Ϊ��֤Ӧ�ò������ӶϿ�ʱ�ܽ��յ��������ݣ�IClientSocket��û������������Դ
			   ������HWNDͬ�����߳�ģʽ��������Ҫ����ΪPostMessage()���͵���Ϣ������ʱ��ǳ���
			2��CreateSingleSocketProxy()������IClientSocket�Ѿ������û����CloseConnect()����
	*/
	virtual void OnConnectClose(DWORD dwSocketID,LPCSTR szMsg,DWORD dwErrorCode) = NULL;
};
//------------------------------------------------------------------------
/*
 *  XXXXXXXXX�˽ӿ��Ѿ���ΪIKeyStream�����ý�ɾ��֮XXXXXXXXXXXXXXXX
 *
 * ���ܣ������������ܻ���ܣ�ͬʱ���а���������
 * ˵����1��ͨ���ӿڵķ�ʽʵ��������ͨ�����ֲ�ͬ�ļ����ֶν��м���
 *		 2����һ�����ݰ�δ����ʱ��IEncryptData���嵱ǰ���ݣ��ȴ����ݰ��ĵ���һ�뷵�ظ��û���
 *			����nMaxDataBufLen���ȡMAX_SOCKET_RAW_DATA_LEN
 */
struct IEncryptData
{	
	// ���ܣ���ʼ�����ܲ㣬���÷��ͺͽ��ճ�ʼ��Կ
	// ˵��������Init()�ӿ���Ϊ�������û������ӶϿ��Ժ���������ͨѶ����ʼ��Կ
	//
	virtual BOOL Init(DWORD dwInitKeyData,DWORD dwInitUnKeyData,DWORD dwParam) = NULL;

	// ���ܣ���������
	// ���룺nLen���ܳ���32*1024,nMaxDataBufLenΪszData�ĳ��ȣ����ܺ�ĳ��Ȳ��ó�����nMaxDataBufLen
	// ���أ����ܺ�����ݷ���szData�У�nLenΪ���ܺ�����ݳ��ȣ�
	virtual BOOL KeyData(/*in out*/char* szData, /*in out*/int& nLen,int nMaxDataBufLen) = NULL;

	// ���ܣ���������
	// ���룺nLen���ܳ���16*1024,nMaxDataBufLenΪszData�ĳ��ȣ����ܺ�ĳ��Ȳ��ó����˳���
	// ���أ����ܺ�����ݷ���szData�У�nLenΪ���ܺ�����ݳ��ȣ�
	virtual BOOL UnKeyData(/*in out*/char* szData, /*in out*/int& nLen,int nMaxDataBufLen) = NULL;
};
//------------------------------------------------------------------------
/*
*	���ܣ�
*		���������ӣ�֧�ֶ��߳�ͬ��֧��(����HWND)�͵��̸߳������ݴ���
*/
struct ILinkProxy
{
	virtual BOOL AddSocket(DWORD dwSocketID,IClientSocketSink *pSink,IClientSocket** ppClientSocket) = NULL;
	virtual BOOL RemoveSocket(IClientSocket* pClientSocket) = NULL;
	virtual void Release() = NULL;
};
//------------------------------------------------------------------------
extern "C" BOOL CreateLinkProxyWnd(IThreadRoute *pThreadRoute,ITrace* pTrace,BOOL bSyncByWnd,BOOL bInSameThread,/*out*/ILinkProxy** ppLinkProxy);
// ���룺
//		dwSocketID	:	�൱��SetTimer()�е�dwTimerID���û��ô�ID��ʶ��ǰ����
//						�����������û���һ��IClientSocketSink���������ӣ�һ��ȡ�û������ӵ���������
//						����ͨ��IClientSocket::GetSocketID()��ȡ����ʱ��dwSocketID
//		bInSameThread : ���̺߳�ͨѶ�߳��Ƿ���ͬһ�̣߳�������Ӧ�����������ͬһ�̣߳��Ӷ�����ʹ���û�����
extern "C"  BOOL CreateClientSocket(DWORD dwSocketID,IClientSocketSink *pSink,IThreadRoute *pThreadRoute, ITrace* pTrace,BOOL bInSameThread,/*out*/IClientSocket** ppClientSocket);

// ����һ�����ӣ���һ���̹߳���һ�����ӣ��ʺ������Ӻ��٣�����Ҫ�󲻸ߵ���������д��ڵĿͻ��˳���ͨѶ����
extern "C" BOOL CreateSingleSocketProxy(DWORD dwSocketID,IClientSocketSink* pSink,ITrace* pTrace,/*out*/IClientSocket** ppClientSocket);
//------------------------------------------------------------------------
class CSingleSocketProxyHelper
{
public:
	typedef BOOL (*ProcCreateSingleSocketProxy)(DWORD dwVersion,DWORD dwSocketID,IClientSocketSink* pSink,ITrace* pTrace,/*ou*/IClientSocket** ppClientSocket);
	CSingleSocketProxyHelper()
	{
		m_hdll = NULL;
		m_pClientSocket = NULL;
	}
	~CSingleSocketProxyHelper()
	{
		Close();
	}
	void Close()
	{
		if(m_pClientSocket)
		{
			m_pClientSocket->Release();
			m_pClientSocket = NULL;
		}
		if(m_hdll)
		{
			FreeLibrary(m_hdll);
			m_hdll = NULL;
		}
	}
	BOOL Create(DWORD dwSocketID,IClientSocketSink* pSink,ITrace* pTrace)
	{
		Close();
		try
		{
#if defined(_DEBUG)
			m_hdll = ::LoadLibrary("SocketD.dll");
			if(m_hdll == NULL)
				throw "can not load SocketD.dll";
#else
			m_hdll = ::LoadLibrary("Socket.dll");
			if(m_hdll == NULL)
				throw "can not load Socket.dll";
#endif
			ProcCreateSingleSocketProxy proc = (ProcCreateSingleSocketProxy)GetProcAddress(m_hdll, "dllCreateSingleSocketProxy");
			if(proc == NULL)
				throw "Can not GetProcAddress('dllCreateSingleSocketProxy')!";
			if(!proc(VERSION_SingleSocketProxy,dwSocketID,pSink,pTrace,&m_pClientSocket))
				throw "CreateSingleSocketProxy() failed";
			return m_pClientSocket != NULL;

		}
		catch(LPCSTR szMsg)
		{
			OutputDebugString(szMsg);OutputDebugString("\r\n");
			return FALSE;
		}
		catch(...)
		{
			OutputDebugString("Error : CSingleSocketProxyHelper::Create() : catch error!\r\n");
			return FALSE;
		}
		return TRUE;
	}

public:
	IClientSocket* m_pClientSocket;
private:
	HINSTANCE		m_hdll;
};
//------------------------------------------------------------------------
// ������͵�¼�����ӹ���
/* �����¼�������
 ˵����
		1�������ӿ��Ժ������е�dwSocketIDֵ
		2������е�¼���̣��������״̬ģʽ��
			����¼̬������̬��ʵ��IClientLinkSink�ӿڣ�
			����¼����Ժ����IClientLink::GotoNextState(IClientLinkSink* pSink)ʵ��״̬�л�
 */
struct IClientLinkSink
{
	// ���ӷ�����ʧ��
	// szMsgΪ������Ϣ�������ݡ�dwErrorCodeΪsocket������룬�μ�Win32 API : GetLastError()
	virtual void OnConnectError(DWORD dwSocketID,LPCSTR szMsg,DWORD dwErrorCode) = NULL;

	// ���ӽ���
	virtual void OnConnectOK(DWORD dwSocketID) = NULL;
	
	// �յ�һ�����ݰ�
	// ע��	����������ݰ����߳���IThreadRoute����ͬһ�̣߳�
	//		����һ������(ILinkWnd)�������ͨѶ����ILinkWnd���븺����һ���������ݰ�����
	virtual BOOL OnRecv(DWORD dwSocketID,LPCSTR buf,int nLen) = NULL;

	// ��ǰ���ӶϿ�
	// szMsgΪ������Ϣ�������ݡ�dwErrorCodeΪsocket������룬�μ�Win32 API : GetLastError()
	virtual void OnConnectClose(DWORD dwSocketID,LPCSTR szMsg,DWORD dwErrorCode) = NULL;
};
//------------------------------------------------------------------------
// ֧�ֵ�¼���̡����ͼ��ܹ��������
struct IClientLink
{
	// ��ǰͨ����ʶ
	// �����ӿ��Ժ���dwSocketIDֵ
	virtual DWORD GetSocketID() = NULL;

	/*
		�������Ӳ���¼������

		������
			szServerAddress��wServerPort:
				��������ַ�Ͷ˿�
			pProxyOption:
				������������������ΪNULL����ʾ��֧�ִ��������

		���أ�
			�û��ɺ��Է��ؽ������������ʱҲ�᷵��FALSE
			�ɹ���TRUE
			ʧ�ܣ�FALSE
				�������ʧ�ܣ�IClientLinkSink::OnConnectError()�����ص�
				������ӳɹ������ڵȴ���¼���ʱ���ӶϿ���IClientLinkSink::OnConnectClose()��������
	*/
	virtual BOOL Connect(LPCSTR szServerAddress, WORD wServerPort,NET_PROXY_OPTION* pProxyOption = NULL) = NULL;

	/* �л�����һ��״̬
	   �磺
		  Ӧ�ò�ĵ�¼̬������̬����������ʵ�֣���ʵ��IClientLinkSink
		  ����¼����Ժ󣬵��ô˺����ӵ�¼̬�л�������̬
	*/
	virtual void GotoNextState(IClientLinkSink* pSink) = NULL;

	// �رմ����ӣ��´ο����ٴε���Connect()�������ӣ�IClientLinkSink::OnLoginResult()���ᱻ����
	// ����ص�IClientSocketSink::OnConnectClose()
	virtual BOOL CloseConnect() = NULL;

	virtual BOOL GetCallList( void ** pList , int & nLen , int nVer ) = NULL;
	
	/*
	 ���ܣ��������ݰ�
	 ���룺nLen���ܳ���16 x 1024,buf����Ϊ��
	 ���أ����ʹ�����������FALSE
		   ������ͳɹ�������TRUE��WSA_IO_INCOMPLETE
				WSA_IO_INCOMPLETE��ʾ���ݷ��ͳɹ�����û���������ͳ�ȥ
				�����ļ����͵ȴ������ݴ���ʱ�����ã�Ӧ�ò���Ը����������ֵ�����ӳ��ٷ�
			    WSA_IO_INCOMPLETE��Overlapped sockets��WSASend()����WSAEWOULDBLOCK��Ϣʱ���ص�
				�����ϵ�ͬ��WSA_IO_INCOMPLETE,��ΪIClientSocket��ȴ�ʱ���ٴη���
	 ˵����1�������㷨Ϊ���������ͣ����ݷ���ͨʱ�Ž����ݷ��뷢�Ͷ��У�����WSA_IO_INCOMPLETE���ȴ��´η���
		   2�����socket����ʧ�ܣ�ϵͳ����OnConnectClose�з��س�����Ϣ����˲����ڷ��ʹ�������������
	*/
	virtual int Send(LPCSTR buf,int nLen) = NULL;

	// ��ѯ��ǰ�Ƿ�������״̬
	virtual BOOL IsConnected() = NULL;

	// ����һЩ����
	virtual BOOL ioctrl(DWORD dwCmd,DWORD dwParam,/*out*/void* pData) = NULL;

	// �ͷŶ���
	virtual void Release() = NULL;
};
//------------------------------------------------------------------------
struct IKeyStream
{
	enum {MaxBufLen = 16* 1024 + 256/**/};

	/* ���ܣ���ʼ�����ܲ㣬���÷��ͺͽ��ճ�ʼ��Կ
	 ˵��������InitKey()�ӿ���Ϊ�ˣ�
				1�������û������ӶϿ��Ժ���������ͨѶ����ʼ���շ���Կ
				2���û���¼�����������ʱ����������Ҫ��ʼ��������Կ��
				3�����������ص�¼���ʱ�������һ�����_���ͻ���Ҳ��Ҫ����շ���Կ
	*/
	virtual BOOL InitKey(LPCSTR buf,int nLen) {return FALSE;};


	virtual BOOL KeyFirstData(/*in out*/char* szData, /*in*/int nMaxDataBufLen,/*in out*/int& nLen) = NULL;

	virtual BOOL UnKeyFirstData(/*in out*/char* szData,/*in*/int nMaxDataBufLen,/*in out*/int& nLen) = NULL;

	// ���ܣ���������
	// ���룺nLen���ܳ���MaxBufLen,nMaxDataBufLenΪszData�ĳ��ȣ����ܺ�ĳ��Ȳ��ó�����nMaxDataBufLen
	// ���أ����ܺ�����ݷ���szData�У�nLenΪ���ܺ�����ݳ��ȣ�
	virtual BOOL KeyData(/*in out*/char* szData, /*in*/int nMaxDataBufLen,/*in out*/int& nLen) = NULL;


	// ���ܣ���������
	// ���룺nLen���ܳ���MaxBufLen,nMaxDataBufLenΪszData�ĳ��ȣ����ܺ�ĳ��Ȳ��ó����˳���
	// ���أ����ܺ�����ݷ���szData�У�nLenΪ���ܺ�����ݳ��ȣ�
	virtual BOOL UnKeyData(/*in out*/char* szData,/*in*/int nMaxDataBufLen,/*in out*/int& nLen) = NULL;

};
//------------------------------------------------------------------------
/*
  ���ܣ�
		����һ��֧�ַ�������ܡ���¼���ӹ������
  ˵�������ù�ϵ����

			    IClientLinkSink				 IClientSocketSink
		Ӧ�ò� <--------------- IClientLink <------------------- IClientSocket
		       ---------------->			------------------->
			     IClientLink				 IClientSocket
  ������
	pSink			: ͨѶ�¼�������

  ��ѡ����:
    pOption			: ����δ�������Զ����IKeyStream�ӿڡ���������ѡ���
    pClientSocket	: �����ǵ��߳�ģʽ����߳�ģʽ��IClientSocket
					  ��pClientSocket == NULL,��ʾȡ��CreateClientLink�ڲ�ͨ��
						CreateSingleSocketProxy()����һ������HWNDͬ����IClientSocket
	pTrace			: ͨѶ��ĵ�����Ϣ��ʾ�ӿ�

�����أ�
		���ش�������IClientLink����
*/
extern "C" IClientLink* CreateClientLink(DWORD dwSocketID,IClientLinkSink* pSink,ITrace* pTrace = NULL,IClientSocket* pClientSocket = NULL,void* pOption = NULL);
//------------------------------------------------------------------------
class CClientLinkHelper
{
public:
	typedef IClientLink* (*ProcCreateClientLink)(DWORD dwVersion,DWORD dwSocketID,IClientLinkSink* pSink,ITrace* pTrace,IClientSocket* pClientSocket,void* pOption);
	CClientLinkHelper()
	{
		m_hdll = NULL;
		m_pClientLink = NULL;
	}
	~CClientLinkHelper()
	{
		Close();
	}
	void Close()
	{
		if(m_pClientLink)
		{
			m_pClientLink->Release();
			m_pClientLink = NULL;
		}
	}
	BOOL Create(DWORD dwSocketID,IClientLinkSink* pSink,ITrace* pTrace = NULL,IClientSocket* pClientSocket = NULL,void* pOption = NULL)
	{
		if(m_pClientLink)
		{
			try
			{
				m_pClientLink->Release();
			}
			catch(...)
			{
			}

			m_pClientLink = NULL;
		}

		try
		{
#if defined(_DEBUG)
			m_hdll = ::LoadLibrary("FKSocket_D.dll");
			if(m_hdll == NULL)
				throw "�޷����� FKSocket_D.dll";
#else
			m_hdll = ::LoadLibrary("FKSocket.dll");
			if(m_hdll == NULL)
				throw "�޷����� Socket.dll";
#endif
			ProcCreateClientLink proc = (ProcCreateClientLink)GetProcAddress(m_hdll, "dllCreateClientLink");
			if(proc == NULL)
				throw "Can not GetProcAddress('dllProcCreateClientLink')!";
			m_pClientLink = proc(VERSION_CLIENTLINK,dwSocketID,pSink,pTrace,pClientSocket,pOption);
			if(m_pClientLink == NULL)
				throw "dllCreateClientLink() failed";
			return TRUE;

		}
		catch(LPCSTR szMsg)
		{
			OutputDebugString(szMsg);OutputDebugString("\r\n");
			return FALSE;
		}
		catch(...)
		{
			OutputDebugString("Error : CClientLinkHelper::Create() : catch error!\r\n");
			return FALSE;
		}
		return TRUE;
	}

public:
	IClientLink* m_pClientLink;
private:
	HINSTANCE		m_hdll;
};
//------------------------------------------------------------------------