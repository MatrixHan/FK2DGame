/**
*	created:		2012-7-1   0:04
*	filename: 		ScriptAdapter_cse
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		csv���ܸ�ʽ�ű�������
*/
//------------------------------------------------------------------------
#include "../Include/ScriptAdapter_cse.h"
#include "../Include/MemTextFile.h"
//------------------------------------------------------------------------
CScriptAdaptercse_row::CScriptAdaptercse_row()
	: m_pScript(NULL), m_pData(NULL), m_pLine(NULL), m_nSize(0)
{

}
//------------------------------------------------------------------------
CScriptAdaptercse_row::~CScriptAdaptercse_row()
{
	if (m_pData)
	{
		delete[] m_pData;
	}

	if (m_pLine)
	{
		free(m_pLine);
	}
}
//------------------------------------------------------------------------
int CScriptAdaptercse_row::Size() const
{
	return m_nSize;
}
//------------------------------------------------------------------------
_VARIANT& CScriptAdaptercse_row::GetData(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_nSize)
	{
		return empty_variant;
	}
	
	return m_pData[nIndex];
}
//------------------------------------------------------------------------
_VARIANT& CScriptAdaptercse_row::operator [](int nIndex)
{
	return GetData(nIndex);
}
//------------------------------------------------------------------------
_VARIANT& CScriptAdaptercse_row::GetData(LPCSTR szName)
{
	int nIndex = m_pScript->GetColIndex(szName);
	if (nIndex < 0 || nIndex >= m_nSize)
	{
		return empty_variant;
	}

	return m_pData[nIndex];
}
//------------------------------------------------------------------------
_VARIANT& CScriptAdaptercse_row::operator[](LPCSTR szName)
{
	return GetData(szName);
}
//------------------------------------------------------------------------
void CScriptAdaptercse_row::Release()
{
	delete this;
}
//------------------------------------------------------------------------
BOOL CScriptAdaptercse_row::InitData(CScriptAdapter_cse* pScript, char* line, char cSeparator, BOOL bAllowSeries)
{
	const static size_t __max_seg_len__ = 1024;

	if (pScript == NULL || line == NULL)
	{
		return FALSE;
	}

	int* pPosTemp = new int[__max_seg_len__];
	int nLineLen = 0;
	char c = line[nLineLen++];
	BOOL bRecord = TRUE;
	
	while (c != 0 && c != '\r' && c != '\n')
	{
		if (c == cSeparator)
		{
			//c = 0;
			line[nLineLen-1] = 0;

			if (bAllowSeries) // �����������֣�����','�ָ�
			{
				if (bRecord)
				{
					pPosTemp[m_nSize++] = nLineLen - 1;				
				}
				else
				{
					bRecord = TRUE;
				}
			}
			else
			{
				// �������������֣�����' '�ָ�
				bRecord = TRUE;
			}
		}
		else
		{
			if (bRecord)
			{
				pPosTemp[m_nSize++] = nLineLen - 1;
				bRecord = FALSE;
			}
		}
		
		c = line[nLineLen++];
		if (m_nSize >= __max_seg_len__)
		{
			break;
		}
	}
	
	line[nLineLen-1] = 0;
	
	m_pLine = (char*)malloc(nLineLen);
	memcpy(m_pLine, line, nLineLen);

	m_pData = new _VARIANT[m_nSize];
	for (int i = 0; i < m_nSize; ++i)
	{
		m_pData[i].m_Type = _VARIANT::enType_STRING;
		m_pData[i].m_Data.string = &m_pLine[pPosTemp[i]];
	}
	
	delete[] pPosTemp;
	m_pScript = pScript;
	return TRUE;
}
//------------------------------------------------------------------------
CScriptAdapter_cse::CScriptAdapter_cse()
{

}
//------------------------------------------------------------------------
CScriptAdapter_cse::~CScriptAdapter_cse()
{

}
//------------------------------------------------------------------------
BOOL CScriptAdapter_cse::LoadScript(LPCSTR szFile)
{
	// ÿ����󳤶�
	const static size_t __max_len__ = 1024*8;

	// ���ļ�
	CMemTextFile stream;
	if (!stream.Open(szFile))
	{
		TraceLn( "load file %s failed!", szFile);
		return FALSE;
	}

	// ���ж�ȡ
	char* line = (char*)malloc(__max_len__);

	// ��ȡ���⣬ע�������Ӧ�����б���
	if (stream.GetLine(line, __max_len__) == NULL)
	{
		free(line);
		return FALSE;
	}

	// ȷ������ʲô���ŷָ���
	char cSeparator = ' ';
	BOOL bAllowSeries = FALSE;
	for (int i = 0; line[i] != 0 && line[i] != '\r'; ++i)
	{
		if (line[i] == ',')
		{
			cSeparator = ',';
			bAllowSeries = TRUE;
			break;
		}
	}
	
	// �õ��б��⣬Ҳ���ǵ�һ��
	if (!m_colTitle.InitData(this, line, cSeparator, bAllowSeries))
	{
		free(line);
		return FALSE;
	}	
	
	// ������������
	while (!stream.IsEnd())
	{
		if (stream.GetLine(line, __max_len__) == NULL)
		{
			continue;
		}

		CScriptAdaptercse_row* pRow = new CScriptAdaptercse_row;
		if (pRow == NULL
			|| !pRow->InitData(this, line, cSeparator, bAllowSeries)
			|| pRow->Size() <= 0)
		{
			SAFE_RELEASE(pRow);
			continue;
		}
		
		// �м�������һ��Ԫ��
		_VARIANT data;
		data.m_Type = _VARIANT::enType_ITERATOR;
		data.m_Data.it = pRow;
		m_table.push_back(data);

		// �б�����ȡ����
		m_rowTitle.push_back(pRow->GetData(0).m_Data.string);
	}

	free(line);
	return TRUE;
}
//------------------------------------------------------------------------
IScriptIterator* CScriptAdapter_cse::GetScriptIterator()
{
	return static_cast<IScriptIterator*>(this);
}
//------------------------------------------------------------------------
int CScriptAdapter_cse::Size() const
{
	return m_table.size();
}
//------------------------------------------------------------------------
_VARIANT& CScriptAdapter_cse::GetData(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_table.size())
	{
		return empty_variant;
	}

	return m_table[nIndex];
}
//------------------------------------------------------------------------
_VARIANT& CScriptAdapter_cse::operator[](int nIndex)
{
	return GetData(nIndex);
}
//------------------------------------------------------------------------
_VARIANT& CScriptAdapter_cse::GetData(LPCSTR szName)
{
	int nIndex = GetRowIndex(szName);
	return GetData(nIndex);	
}
//------------------------------------------------------------------------
_VARIANT& CScriptAdapter_cse::operator[](LPCSTR szName)
{
	return GetData(szName);
}
//------------------------------------------------------------------------
void CScriptAdapter_cse::Release()
{
	for (std::vector<_VARIANT>::const_iterator it = m_table.begin();
		it != m_table.end(); ++it)
	{
		_VARIANT colObj = *it;
		if (colObj.m_Type == _VARIANT::enType_ITERATOR)
		{
			IScriptIterator* pScriptIterator = colObj.m_Data.it;
			if (pScriptIterator != NULL)
			{
				pScriptIterator->Release();
			}
		}
	}

	m_table.clear();
	m_rowTitle.clear();

	delete this;
}
//------------------------------------------------------------------------
// �����б���õ�������
int CScriptAdapter_cse::GetRowIndex(LPCSTR lpRowTitle)
{
	int nIndex = 0;
	for (std::vector<std::string>::const_iterator it = m_rowTitle.begin();
		it != m_rowTitle.end(); ++it, ++nIndex)
	{
		if (stricmp((*it).c_str(), lpRowTitle) == 0)
		{
			return nIndex;
		}
	}

	return -1;
}
//------------------------------------------------------------------------
// �����б���õ�������
int CScriptAdapter_cse::GetColIndex(LPCSTR lpColTitle)
{
	for (int i = 0; i < m_colTitle.Size(); ++i)
	{
		if (stricmp(m_colTitle[i].m_Data.string, lpColTitle) == 0)
		{
			return i;
		}
	}
	
	return -1;
}
//------------------------------------------------------------------------