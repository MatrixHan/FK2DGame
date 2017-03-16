/**
*	created:		2012-7-1   0:08
*	filename: 		ScriptAdapter_csv
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ű�����-����2ά.csv�ű�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/Script/IScriptUpdate.h"
#include "ScriptUpdate.h"
#include <vector>
#include <string>
using std::vector;
using std::string;
//------------------------------------------------------------------------
class CScriptAdapter_csv;
//------------------------------------------------------------------------
enum
{
	enMax_Line_Len = 1024*8,
	enMax_Segment_Count = 1024,
};
//------------------------------------------------------------------------
class CScriptAdaptercsv_row : public IScriptIterator
{
public:
	virtual int Size() const;	
	virtual _VARIANT& GetData(int nIndex);	
	virtual _VARIANT& operator [](int nIndex);	
	virtual _VARIANT& GetData(LPCSTR szName);	
	virtual _VARIANT& operator [](LPCSTR szName);	
	virtual void Release();
	CScriptAdaptercsv_row();
	virtual ~CScriptAdaptercsv_row();
	void Close();
	BOOL InitData(CScriptAdapter_csv *pScript, 
		char *line, 
		char cSeparator/*�ָ���*/, 
		BOOL bAllowSeries/*�Ƿ�������������*/);

private:
	_VARIANT			*m_pData;
	int					m_nSize;
	CScriptAdapter_csv	*m_pScript;
	char				*m_pLine;
};
//------------------------------------------------------------------------
class CScriptAdapter_csv : public IScriptIterator
{
public:
	CScriptAdapter_csv();
	virtual ~CScriptAdapter_csv();

	virtual BOOL LoadScript(LPCSTR szFile);	
	virtual IScriptIterator *GetScriptIterator();

	virtual int Size() const;	
	virtual _VARIANT& GetData(int nIndex);	
	virtual _VARIANT& operator[](int nIndex);	
	virtual _VARIANT& GetData(LPCSTR szName);	
	virtual _VARIANT& operator[](LPCSTR szName);	
	virtual void Release();
	int  FindPropName(LPCSTR szName);
private:
	int  FindObjectName(LPCSTR szName);	
	void Close();
private:
	vector< _VARIANT >			m_RowTable;
	int							m_nSize;
	vector< string >			m_ObjectNameTable;
	CScriptAdaptercsv_row		m_title;			// ����
};
//------------------------------------------------------------------------