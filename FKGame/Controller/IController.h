/**
*	created:		2012-6-21   23:24
*	filename: 		IController
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
struct IClientGlobal;
//------------------------------------------------------------------------
struct IDataReceiver
{
	virtual BOOL OnReceived(const char* pData, int nLen) = 0;	
};
//------------------------------------------------------------------------
// �û������¼�����
struct IController
{
	virtual ~IController(){}

	// ����Ѿ�����������ϢӦ����0
	// �μ�WM_LBUTTONUP
	virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam){return -1;}

	// �μ�WM_LBUTTONDOWN
	virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam){return -1;}

	// ����Ѿ�����������ϢӦ����0
	// �μ�WM_LBUTTONDOWN
	virtual LRESULT OnRButtonDown(WPARAM wParam, LPARAM lParam){return -1;}

	// ����Ѿ�����������ϢӦ����0
	// �μ�WM_RBUTTONUP
	virtual LRESULT OnRButtonUp(WPARAM wParam, LPARAM lParam){return -1;}

	// ����Ѿ�����������ϢӦ����0
	// �μ�WM_MOUSEMOVE
	virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam){return -1;}

	// ����Ѿ�����˼�����ϢӦ����0
	// �μ�WM_KEYDOWN
	virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam){return -1;}

	// ����Ѿ�����˼�����ϢӦ����0
	// �μ�WM_KEYDOWN
	virtual LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam){return -1;}
	// ͨ�������һ�����ڴ��������ڲ˵���dialog���û������¼�
	// ����Ѿ��������ϢӦ����0
	enum
	{
		enPlayerLogin = 2528	/*�û���½*/,
		enRecordName  = 2529	/*��¼�û���*/,
	};
	virtual LRESULT OnCommand(DWORD dwCmd,DWORD dwParam1,DWORD dwParam2){return - 1;}

	// ����û��������˳��������ظ�����ϵͳ���˳�
	enum 
	{
		ieEscape = -2			/*�û�����escape�����˳�*/,
		ieOK = 1				/*==TRUE����ʾ���¼�����*/,
		ieUpdate = 2			/*��ʾ��ͼ���������£������ʱ�����ƶ���λ�� */
	};
	virtual int Loop(){return 1;}

	virtual void Release()
	{
		delete this;
	}

	/*�����û����Ʒ�ʽ  
		mode ��ֵΪ NULL ʱ��ʾ��ֹ�û������¼�	  
		����ʧ�ܷ���False  , ��������ģʽ��ΪNULL
	*/		
	virtual BOOL changeControlMode(int mode){return TRUE;};

	//��Ҫ�������ȡ���ݵ�ģ�� , ͨ���˺���ע���,�ſ��յ���Ϣ 
	virtual BOOL AddReceiver(IDataReceiver *pReceiver, int MainMsgCode, int SubMsgCode) {return TRUE;};
	//�Ƴ�ģ�������
	virtual BOOL RemoveReceiver(IDataReceiver *pReceiver, int MainMsgCode, int SubMsgCode) {return TRUE;};

	//���
	virtual int  PushBufferToDataCenterQueue(int nQueueID, LPCSTR pBuffer, int nLen) {return -1;};
	virtual BOOL PopBufferFromDataCenterQueue(int nQueueID) {return NULL;};
	virtual BOOL DeleteDataFromDataCenterQueue(int nIndex) {return TRUE;};
	virtual BOOL ClearDataFromDataCenterQueue(int nQueueID) {return TRUE;};

	// ����������ݼӽ��� bLock: TRUE->���� FALSE->����
	virtual void LockReceiver(BOOL bLock) {};
	// �����Ƿ�ʹ���̴߳�������
	virtual void SetThreadRun(BOOL bRun) {};
	// �Ƿ�һ�
	virtual BOOL IsAutoOperate(void) {return FALSE;}; 
};
//------------------------------------------------------------------------
// ��Ϸ������Ϣ������������
class CControllerHelper
{
public:
	typedef BOOL (*ProcCreateController)(IClientGlobal *pGlobal, IController** ppController);

	CControllerHelper()
	{
		m_pInput = NULL;
		m_hdll = NULL;
	}
	~CControllerHelper()
	{
		Close();
	}
	BOOL Create(IClientGlobal *pGlobal)
	{
		try
		{
			LoadDLL();
			
			ProcCreateController proc;
			proc = (ProcCreateController)GetProcAddress(m_hdll, "dllCreateController");
			
			if (NULL == proc)
			{
				throw "Can not GetProAddress('dllCreateController')";
			}

			if (FALSE == proc(pGlobal, &m_pInput))
			{
				throw "dllCreateController() catch error";
			}

			return m_pInput != NULL;
		}
		catch(LPCSTR szMsg)
		{
			OutputDebugString(szMsg);
			OutputDebugString("\r\n");
			return FALSE;
		}
		catch(...)
		{
			OutputDebugString("dllCreateController() catch error\r\n");
			return FALSE;
		}
	}

	void Close()
	{
		if(m_pInput)
		{
			m_pInput->Release();
			m_pInput = NULL;
		}

		if(m_hdll)
		{
			FreeLibrary(m_hdll);
			m_hdll = NULL;
		}
	}
	
	LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam)
	{
		if(m_pInput == NULL)
			return -1;
		return m_pInput->OnLButtonUp(wParam, lParam);
	}
	
	LRESULT OnRButtonUp(WPARAM wParam, LPARAM lParam)
	{
		if(m_pInput == NULL)
			return -1;
		return m_pInput->OnRButtonUp(wParam, lParam);
	}

	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam)
	{
		if(m_pInput == NULL)
			return -1;
		return m_pInput->OnLButtonDown(wParam, lParam);
	}

	// ����Ѿ�����������ϢӦ����0
	// �μ�WM_LBUTTONDOWN
	LRESULT OnRButtonDown(WPARAM wParam, LPARAM lParam)
	{
		if(m_pInput == NULL)
			return -1;
		return m_pInput->OnRButtonDown(wParam, lParam);
	}

	LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		if(m_pInput == NULL)
			return -1;
		return m_pInput->OnMouseMove(wParam, lParam);
	}

	// ����Ѿ�����˼�����ϢӦ����0
	// �μ�WM_KEYDOWN
	LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam)
	{
		if(m_pInput == NULL)
			return -1;
		return m_pInput->OnKeyDown(wParam, lParam);
	}

	// ����Ѿ�����˼�����ϢӦ����0
	// �μ�WM_KEYDOWN
	LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam)
	{
		if(m_pInput == NULL)
			return -1;
		return m_pInput->OnKeyUp(wParam, lParam);
	}

	LRESULT OnCommand(DWORD dwCmd,DWORD dwParam1,DWORD dwParam2)
	{
		if(m_pInput == NULL)
			return -1;
		return m_pInput->OnCommand(dwCmd, dwParam1, dwParam2);
	}

	int Loop()
	{
		if(m_pInput == NULL)
			return 1;
		return m_pInput->Loop();
	}

	IController* operator ->()
	{
		return m_pInput;
	}

	void LoadDLL()
	{
		if(m_hdll == NULL)
		{
#ifdef _DEBUG
			m_hdll = LoadLibrary("FKController_D.dll");
			if(m_hdll == NULL)
				throw "�޷����� FKController_D.dll";
#else
			m_hdll = LoadLibrary("FKController.dll");
			if(m_hdll == NULL)
				throw "�޷����� FKController.dll";
#endif
		}
	}

public:
	IController	*m_pInput;
	HINSTANCE	m_hdll;
};
//------------------------------------------------------------------------