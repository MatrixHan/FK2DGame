/**
*	created:		2012-7-1   0:03
*	filename: 		ScriptAdapter_cse
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		csv���ܸ�ʽ�ű�������
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
class CScriptAdapter_cse;
//------------------------------------------------------------------------
class CScriptAdaptercse_row : public IScriptIterator
{
public:
	virtual int Size() const;	
	virtual _VARIANT& GetData(int nIndex);	
	virtual _VARIANT& operator [](int nIndex);	
	virtual _VARIANT& GetData(LPCSTR szName);	
	virtual _VARIANT& operator [](LPCSTR szName);	
	virtual void Release();

	CScriptAdaptercse_row();
	BOOL InitData(CScriptAdapter_cse* pScript, char* line, char cSeparator, BOOL bAllowSeries);
	virtual ~CScriptAdaptercse_row();
	
private:
	CScriptAdapter_cse* m_pScript;
	_VARIANT*           m_pData;
	char*               m_pLine;
	int				    m_nSize;
};
//------------------------------------------------------------------------
class CScriptAdapter_cse : public IScriptIterator
{
public:
	virtual int Size() const;	
	virtual _VARIANT& GetData(int nIndex);	
	virtual _VARIANT& operator[](int nIndex);	
	virtual _VARIANT& GetData(LPCSTR szName);	
	virtual _VARIANT& operator[](LPCSTR szName);	
	virtual void Release();

	CScriptAdapter_cse();
	virtual BOOL LoadScript(LPCSTR szFile);	
	virtual IScriptIterator* GetScriptIterator();	
	virtual ~CScriptAdapter_cse();

public:
	int GetRowIndex(LPCSTR lpRowTitle);
	int GetColIndex(LPCSTR lpColTitle);

protected:
	std::vector<_VARIANT>	 m_table; // �м�
	std::vector<std::string> m_rowTitle; // �б��⼯
	CScriptAdaptercse_row    m_colTitle; // �б��⼯,Ҳ���ǵ�һ��
};
//------------------------------------------------------------------------