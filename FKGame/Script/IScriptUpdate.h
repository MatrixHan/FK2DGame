/**
*	created:		2012-6-30   23:35
*	filename: 		IScriptUpdate
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ű�����ģ��ӿ��ļ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
//------------------------------------------------------------------------
struct IScriptIterator;
//------------------------------------------------------------------------
// ���ϱ�������
struct _VARIANT
{
	enum _TYPE_
	{
		enType_EMPTY =  0,
		enType_LONG		 ,	// ֧����������
		enType_FLOAT	 ,  // ֧�ָ�������
		enType_STRING	 ,  // ֧���ַ�����
		enType_ITERATOR  ,  // ֧�ֶ�ά�ű�
		enType_MAX		 ,
	}m_Type;
	
	union
	{
		long	lValue;		// ��������������
		double	dValue;		// �����Ǹ�������
		LPCSTR  string;		// �������ַ�������
		IScriptIterator * it; // �������ӽű� ����ʵ�������ά�Ľű�
	}m_Data;
	
};
//------------------------------------------------------------------------
// �ű�����������ʵ�ֽű������޹���
struct IScriptIterator
{
	virtual int Size() const = NULL;
	
	virtual _VARIANT& GetData(int nIndex) = NULL;
	
	virtual _VARIANT& operator[] (int nIndex) = NULL;
	
	virtual _VARIANT& GetData(LPCSTR szName) = NULL;
	
	virtual _VARIANT& operator [](LPCSTR szName) = NULL;
	
	virtual void Release() = NULL;	// �������ɹ������ͷ�!!
};
//------------------------------------------------------------------------
struct IResScript;
//------------------------------------------------------------------------
/*
 *	�ű����»ص��ӿ�
 */
struct IScriptUpdateSink
{
	// �ű�����ɹ�ʱ֪ͨ,����csv�ű�����it��ȡ������scp/res�ű�����pResScript��ȡ
	virtual BOOL OnScriptLoad(IScriptIterator *it, IResScript *pResScript, LPCSTR szFile) = NULL;	
	// �ű���̬����ʱ֪ͨ,����csv�ű�����it��ȡ������scp/res�ű�����pResScript��ȡ
	// ��Ҫ��OnScriptUpdate�е���ReleaseScript����,��Ϊ����һ���ű���Ӧ���Sink
	virtual BOOL OnScriptUpdate(IScriptIterator *it, IResScript *pResScript, LPCSTR szFile) = NULL;
};
//------------------------------------------------------------------------
/*
 *	�ű����¹�����
 */
struct IScriptUpdateManager
{
	// ��  ������ȡ�ű������Զ�ע��
	// ��  ����[����] LPCSTR szFile - �ű��ļ���
	// ��  ����[����] IScriptUpdateSink *pSink - ����ű��Ļص��ӿ�
	// ����ֵ��BOOL - �ɹ�����TRUE������ΪFALSE
	virtual BOOL LoadScript(LPCSTR szFile, IScriptUpdateSink *pSink) = NULL;
	
	// ��  �������½ű�
	// ��  ����[����] LPCSTR szFile - ���½ű��ļ���
	// ����ֵ��BOOL - �ɹ�����TRUE������ΪFALSE
	virtual BOOL UpdateScript(LPCSTR szFile) = NULL;	
	
	// ��  ����ע��ű��ص��ӿ�
	// ��  ����[����] LPCSTR szFile - ���½ű��ļ���
	// ��  ����[����] IScriptUpdateSink * pSink - �ű����»ص��ӿ�
	// ����ֵ����
	virtual void RegisterSink(LPCSTR szFile, IScriptUpdateSink *pSink) = NULL;
	
	// ��  ����ȡ��ע��ű��ص��ӿ�
	// ��  ����[����] LPCSTR szFile - ���½ű��ļ���
	// ��  ����[����] IScriptUpdateSink * pSink - �ű����»ص��ӿ�
	// ����ֵ����
	virtual void UnRegisterSink(LPCSTR szFile, IScriptUpdateSink *pSink) = NULL;
	
	// �ͷŹ�����
	virtual void Release() = NULL;
};
//------------------------------------------------------------------------
#pragma warning(disable : 4355)
#ifndef ASSERT
#		define ASSERT(p)	if(!(p)){ __asm{ int 3 }; }
#endif
//------------------------------------------------------------------------
// ʹ��������֧�ָ��������������ʹ��
class CVariant : protected _VARIANT
{
public:	
	CVariant( char * str ) : m_variant(*((_VARIANT *)this))
	{
		m_variant.m_Type = enType_STRING;
		m_variant.m_Data.string = str;
	}
	
	CVariant( long lValue ) : m_variant(*((_VARIANT *)this))
	{
		m_variant.m_Type = enType_LONG;
		m_variant.m_Data.lValue = lValue;
	}
	
	CVariant( double dValue ) : m_variant(*((_VARIANT *)this))
	{
		m_variant.m_Type = enType_FLOAT;
		m_variant.m_Data.dValue = dValue;
	}
	
	CVariant( IScriptIterator * it ) : m_variant(*((_VARIANT *)this))
	{
		m_variant.m_Type = enType_ITERATOR;
		m_variant.m_Data.it = it;
	}

	CVariant( _VARIANT & value ) : m_variant(value)
	{
	}

	CVariant() : m_variant(*((_VARIANT *)this))
	{
		m_variant.m_Type = enType_EMPTY;
		m_variant.m_Data.dValue = 0;
	}
	
	CVariant operator[](int nIndex)
	{
		ASSERT( m_variant.m_Type == enType_ITERATOR );
		
		return (*m_variant.m_Data.it)[nIndex];
	}

	CVariant operator[](LPCSTR szName)
	{
		ASSERT( m_variant.m_Type == enType_ITERATOR );
		
		return (*m_variant.m_Data.it)[szName];
	}

	int Bound()
	{
		ASSERT( m_variant.m_Type == enType_ITERATOR );
		return m_variant.m_Data.it->Size();
	}

	operator LPCSTR()
	{
		if( m_variant.m_Type != enType_STRING )
		ChangeType( enType_STRING);
		return m_variant.m_Data.string;
	}

	operator long()
	{
		if( m_variant.m_Type != enType_LONG )
		ChangeType( enType_LONG);
		return m_variant.m_Data.lValue;
	}

	operator double()
	{
		if( m_variant.m_Type != enType_FLOAT )
		ChangeType( enType_FLOAT);
		return m_variant.m_Data.dValue;
	}

	typedef void ( CVariant::*CHANGETYPEPOC)();
	void ChangeType( _TYPE_ type )
	{
		static CHANGETYPEPOC	s_change_poc[enType_MAX][enType_MAX] =
		{
			{	NULL,NULL,NULL,NULL,NULL, },
			{	NULL,NULL,&CVariant::ChangeType_long2float ,NULL,NULL },
			{	NULL,&CVariant::ChangeType_float2long,NULL,NULL,NULL  },
			{   NULL,&CVariant::ChangeType_str2long,&CVariant::ChangeType_str2float, NULL, NULL },
			{	NULL,NULL,NULL,NULL,NULL, }
		};

		if( m_variant.m_Type == enType_EMPTY )
			return;
		if( m_variant.m_Type == type )
			return;
		if( s_change_poc[m_variant.m_Type][type] == NULL )
			ASSERT(0);

		(this->*s_change_poc[m_variant.m_Type][type])();
	}

	// -------------------------------------------------------------------
	void ChangeType_long2float()
	{
		m_variant.m_Data.dValue = m_variant.m_Data.lValue;
		m_variant.m_Type = enType_FLOAT;
	}
	
	void ChangeType_float2long()
	{
		m_variant.m_Data.lValue = (long)m_variant.m_Data.dValue;
		m_variant.m_Type = enType_LONG;
	}
	
	void ChangeType_str2long()
	{
		long lTemp = 0;
		sscanf( m_variant.m_Data.string , "%lld" , &lTemp );
		m_variant.m_Data.lValue = lTemp;
		m_variant.m_Type = enType_LONG;
	}
	
	void ChangeType_str2float()
	{
		float dTemp = 0.0;
		sscanf( m_variant.m_Data.string , "%f" , &dTemp );
		m_variant.m_Data.dValue = dTemp;
		m_variant.m_Type = enType_FLOAT;
	}

public:
	_VARIANT	& m_variant;
};
//------------------------------------------------------------------------
#pragma pack()
//------------------------------------------------------------------------
struct IResMgr;
struct ITrace;
//------------------------------------------------------------------------
/*
 *	�ű����¹�����HELPER��
 */
class CScriptUpdateManagerHelper
{
private:
	HINSTANCE m_hdll;
	char m_szErrMsg[128];								//������Ϣ
public:
	IScriptUpdateManager* m_pManager;
public:
	// �����๤��
	typedef BOOL (*ProcCreateScriptUpdateManager)(
		IScriptUpdateManager ** ppManager, IResMgr *pResMgr, ITrace *pTrace);
	
	CScriptUpdateManagerHelper() : m_hdll( NULL ) ,m_pManager(NULL)
	{
		m_szErrMsg[0] = 0;
	}
	
	~CScriptUpdateManagerHelper()
	{
		Close();
	}
	
	IScriptUpdateManager* operator ->()
	{
		return m_pManager;
	}
	
	void Close()
	{
		if (m_pManager!=NULL)
		{
			m_pManager->Release();
			m_pManager = NULL;
		}
		
		if (m_hdll!=NULL)
		{
			FreeLibrary(m_hdll);
			m_hdll = NULL;			
		}
	}
	
	BOOL Create(IResMgr *pResMgr, ITrace *pTrace)
	{
		Close();
		
		try
		{
			if( !LoadDLL() )
				return FALSE;
			
			ProcCreateScriptUpdateManager proc;
			proc = (ProcCreateScriptUpdateManager)GetProcAddress(m_hdll, "CreateScriptUpdateManager");
			
			if(proc == NULL)
				throw "Can't GetProcAddress('CreateScriptUpdateManager')";
			
			if(!proc(&m_pManager, pResMgr, pTrace))
				throw "CreateScriptUpdateManager() error!";

			return m_pManager != NULL;
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
	
	BOOL LoadDLL()
	{
		if(m_hdll == NULL)
		{
#ifdef _DEBUG
			m_hdll = LoadLibrary( "FKScriptUpdate_D.dll" );
			if(m_hdll == NULL)
				throw "�޷����� FKScriptUpdate_D.dll";
#else
			m_hdll = LoadLibrary( "FKScriptUpdate.dll" );
			if(m_hdll == NULL)
				throw "�޷����� FKScriptUpdate.dll";
#endif
			return TRUE;
		}
		return FALSE;
	}
	//�жϽӿ�ָ���Ƿ���Ч
	BOOL IsValid()
	{
		return (m_pManager != NULL);
	}
	char* GetErrMsg()
	{
		return m_szErrMsg;
	}
	IScriptUpdateManager* GetScriptUpdateManager()
	{
		return m_pManager;
	}
	
};
//------------------------------------------------------------------------
