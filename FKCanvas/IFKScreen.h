/**
*	created:		2012-6-19   12:56
*	filename: 		IFKScreen
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
struct ICanvas;
struct ITrace;
//------------------------------------------------------------------------
struct IScreen
{
	virtual BOOL Create(HWND hwnd, int x, int y, int w, int h) = NULL;						//����
	virtual ICanvas* GetCanvas() = NULL;
	virtual BOOL MessageHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = NULL;	//��Ϣ����
	virtual void KeepFrames(int frames) = NULL;												//�����ȶ�֡
	virtual void SetKeepFrames(BOOL b) = NULL;
	virtual void ShowFPS(BOOL b) = NULL;													//��ʾ֡��

	virtual BOOL OnUpdate() = NULL;															//���´���
	virtual void Release() = NULL;															//�ͷŽ��
	virtual HWND GetHWnd() = NULL;
};
//------------------------------------------------------------------------
class CIScreenHelper
{
    typedef BOOL (*ProcCreateIScreen) (DWORD dwVersion, IScreen** ppScreen, ITrace* pTrace );
public:
    BOOL  Create(IScreen** ppScreen, ITrace* pTrace = NULL)
    {
        try
        {
            if (m_hDll==NULL)
		#ifdef _DEBUG
			m_hDll = LoadLibrary("FKCanvas_D.dll");
		#else
			m_hDll = LoadLibrary("FKCanvas.dll");
		#endif
			if(m_hDll == NULL)
                throw "��ȡFKCanvas.dllʧ�ܣ�";

            ProcCreateIScreen CreateObject;

            CreateObject = (ProcCreateIScreen)::GetProcAddress(m_hDll,"CreateIScreen");

            if (CreateObject==NULL)
                throw "�޷����� screen ��Ļ����";

            if (!CreateObject(0x1, ppScreen, pTrace) || *ppScreen==NULL)
                throw "���� screen ��Ļ����ʧ�ܣ�";

        }
        catch(LPSTR err_msg)
        {
            OutputDebugString(err_msg);
            OutputDebugString("\r\n");
            return false;
        }
        catch(...)
        {
            OutputDebugString("��Ļ���������쳣��\r\n");
            return false;
        }
        return true;
    }
    void  Close()
    {
        if (m_hDll)
        {
           ::FreeLibrary(m_hDll);
           m_hDll = NULL;
        }
    }
public:
	CIScreenHelper(){m_hDll = NULL;}
	~CIScreenHelper()
	{
		Close();
	}
private:
    HINSTANCE  m_hDll;
};
//------------------------------------------------------------------------