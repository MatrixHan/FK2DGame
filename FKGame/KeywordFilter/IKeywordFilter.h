/**
*	created:		2012-6-29   14:49
*	filename: 		IKeywordFilter
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ؼ��ֹ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
// �ؼ��ֵļ�����Ϊ����ʱ�����Ҫ��ȼ�
enum Keyword_Option
{
	enKL_Replace = 0,		// �滻
	enKL_Deny = 1,			// ������
};
//------------------------------------------------------------------------
// �ؼ��ֹ��˽ӿ�
struct IKeywordFilter
{
	// ���ٹؼ��ֹ���
	virtual void Release(void) = 0;

	/*
	 *	����: ����,���ṩ�滻�ַ�,
			  �����Ҫ�õ��滻�ؼ��ֺ���ַ���,���봫��lpszOut�Լ�dwOutSize
			  ���ұ�֤lpszOut�ѷ���ռ�,������ܵ����ڴ���ʷǷ���
		���룺lpszIn��Ҫ���˵��ַ���
			  lLevelLow��ʾ�滻�ü���͸ü������ϵĹؼ��֣�Ĭ��Ϊ0����ʾ�������йؼ���
		���أ��������0��ʾ����ͨ�������򷵻��ҵ�����߼����йؼ��ּ���
			  lpszOutΪ���滻�˹ؼ��ֵ��ַ���,��û���ҵ��ؼ���,��Ϊԭ�ַ���
			  dwOutSizeΪ�滻���ַ���lpszOut�ĳ��ȣ����Ϊstrlen(lpszIn)
			  ������Ȳ�����ֻ������ȷ�Χ�ڵ��ַ�
	 */
	virtual LONG Filter(LPCSTR lpszIn, LPSTR lpszOut = NULL,
		DWORD dwOutSize = 0, LONG lLevelLow = 0) = 0;

};
//------------------------------------------------------------------------
// ����ͨ�ùؼ��ֹ���(�������)
extern "C" __declspec(dllexport) BOOL CreateKeywordFilter(IKeywordFilter **ppKeywordFilter);
typedef BOOL (__cdecl *ProcCreateKeywordFilter)(IKeywordFilter **ppKeywordFilter);
//------------------------------------------------------------------------
// �ؼ��ֹ��˴���������
class CKeywordFilterHelper
{
public:
	CKeywordFilterHelper()
	{
		m_hDll = NULL;
		m_pKeywordFilter = NULL;
	}
	
	~CKeywordFilterHelper()
	{
	}

	// �����ؼ��ֹ���
	BOOL Create(void)
	{
		if (m_hDll != NULL)
		{
			return FALSE;
		}
		
		try
		{
#ifdef _DEBUG
			m_hDll = ::LoadLibrary("FKKeywordFilter_D.dll");
			if (NULL == m_hDll)
			{
				throw "�޷����� FKKeywordFilter_D.dll!";
			}
#else
			m_hDll = ::LoadLibrary("FKKeywordFilter.dll");
			if (NULL == m_hDll)
			{
				throw "�޷����� FKKeywordFilter.dll!";
			}
#endif
			
			ProcCreateKeywordFilter pProc = NULL;
			pProc = (ProcCreateKeywordFilter)::GetProcAddress(m_hDll, "CreateKeywordFilter");
			if (NULL == pProc)
			{
				throw "Cann't get process CreateKeywordFilter!";
			}
			
			if (FALSE == pProc(&m_pKeywordFilter))
			{
				throw "Create KeywordFilter error!";
			}
			
			return TRUE;
		}
		catch (LPCSTR szMsg)
		{
			OutputDebugString(szMsg);
			OutputDebugString("\r\n");
		}
		catch (...)
		{
			OutputDebugString("KeywordFilter unknow error!");
			OutputDebugString("\r\n");
		}
		
		return FALSE;
	}

	// �ͷų���ģ��
	void Close(void)
	{
		if (m_pKeywordFilter != NULL)
		{
			m_pKeywordFilter->Release();
			m_pKeywordFilter = NULL;
		}
		
		if (m_hDll != NULL)
		{
			::FreeLibrary(m_hDll);
			m_hDll = NULL;
		}
	}

public:	
	IKeywordFilter *m_pKeywordFilter;

private:
	HINSTANCE m_hDll;

};
//------------------------------------------------------------------------