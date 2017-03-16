/**
*	created:		2012-6-24   20:21
*	filename: 		CDataChunk
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		DataChunk���ݽṹʵ���ļ�
*/
//------------------------------------------------------------------------
#include "CDataUnit.h"
#include "CDataChunk.h"
//------------------------------------------------------------------------
// ���캯��
CDataChunk::CDataChunk()
{
	m_nMaxBufLen = 4;
	m_nCurBufLen = 4;
	m_DataList.clear();
	m_itLoop = m_DataList.begin();
}
//------------------------------------------------------------------------
// ���캯��
CDataChunk::CDataChunk(int nMaxBufLen)
{
	m_nMaxBufLen = nMaxBufLen;
	m_nCurBufLen = 4;
	m_DataList.clear();
	m_itLoop = m_DataList.begin();
}
//------------------------------------------------------------------------
// ��������
CDataChunk::~CDataChunk()
{
	Clear();
}
//------------------------------------------------------------------------
// ����DataChunk������������ݳ���
BOOL CDataChunk::SetMaxBufferLen(int nMaxBufLen)
{
	if (nMaxBufLen <= GetBufferLen())
	{
		return FALSE;
	}

	m_nMaxBufLen = nMaxBufLen;

	return TRUE;
}
//------------------------------------------------------------------------
// ��ȡDataChunk������������ݳ���
int CDataChunk::GetMaxBufferLen(void)
{
	return m_nMaxBufLen;
}
//------------------------------------------------------------------------
// ����DataChunk����������
BOOL CDataChunk::SetBuffer(char *pBuf, int nLen)
{
	int	nDataChunkLen = *((int*)pBuf);
	if (nDataChunkLen > nLen)
	{
		return FALSE;
	}

	if (nDataChunkLen > m_nMaxBufLen)
	{
		return FALSE;
	}

	Clear();

	char *pCurPos = pBuf + 4;

	while (pCurPos < pBuf + nDataChunkLen)
	{
		CDataUnit DataUnit(*((int*)pCurPos), *(pCurPos + 4), pCurPos + 5);
		if (FALSE == SetValue(DataUnit))
		{
			return FALSE;
		}

		pCurPos += (5 + DataUnit.GetLength());
	}

	return TRUE;
}
//------------------------------------------------------------------------
// ��ȡDataChunk����������
BOOL CDataChunk::GetBuffer(char *pBuf, int nLen)
{
	int	nBufferLen = GetBufferLen();
	if (NULL == pBuf || nLen < nBufferLen)
	{
		return FALSE;
	}

	*((int*)pBuf) = nBufferLen;
	char *pCurPos = pBuf + 4;

	list<CDataUnit*>::iterator it = m_DataList.begin();
	for (; it != m_DataList.end(); ++it)
	{
		CDataUnit *pDataUnit = (*it);
		*((int*)pCurPos) = pDataUnit->GetKey();
		*(pCurPos + 4) = pDataUnit->GetType();
		memcpy(pCurPos + 5, pDataUnit->GetBuffer(), pDataUnit->GetLength());
		pCurPos += (5 + pDataUnit->GetLength());
	}

	return TRUE;
}
//------------------------------------------------------------------------
// ��ȡDataChunk���������ݳ���
int CDataChunk::GetBufferLen(void)
{
	return m_nCurBufLen;
}
//------------------------------------------------------------------------
// ���DataChunk����������
void CDataChunk::Clear(void)
{
	list<CDataUnit*>::iterator it = m_DataList.begin();
	for (; it != m_DataList.end(); ++it)
	{
		delete (*it);
	}

	m_DataList.clear();
	m_nCurBufLen = 4;
}
//------------------------------------------------------------------------
// ����һ������
BOOL CDataChunk::SetValue(CDataUnit &DataUnit)
{
	list<CDataUnit*>::iterator it = m_DataList.begin();
	for (; it != m_DataList.end(); ++it)
	{
		CDataUnit *pDataUnit = (*it);
		if (pDataUnit->GetKey() == DataUnit.GetKey())
		{
			int	nOldLen = pDataUnit->GetLength();
			int	nNewLen = DataUnit.GetLength();

			if (nOldLen >= nNewLen || nNewLen - nOldLen + GetBufferLen() <= m_nMaxBufLen)
			{
				pDataUnit->Set(DataUnit);
				m_nCurBufLen += (nNewLen - nOldLen);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}

	if (5 + DataUnit.GetLength() + GetBufferLen() <= m_nMaxBufLen) // 5 = 4�ֽ�Keyֵ + 1�ֽ�����
	{
		CDataUnit *pDataUnit = new CDataUnit(DataUnit);
		if (NULL == pDataUnit)
		{
			return FALSE;
		}

		m_DataList.push_back(pDataUnit);
		m_nCurBufLen += (5 + pDataUnit->GetLength()); // 5 = 4�ֽ�Keyֵ + 1�ֽ�����
		return TRUE;
	}

	return FALSE;
}
//------------------------------------------------------------------------
// ��ȡһ������
CDataUnit* CDataChunk::GetValue(int nKey)
{
	list<CDataUnit*>::iterator it = m_DataList.begin();
	for(; it != m_DataList.end(); ++it)
	{
		CDataUnit *pDataUnit = (*it);
		if (pDataUnit->GetKey() == nKey)
		{
			return pDataUnit;
		}
	}

	return NULL;
}
//------------------------------------------------------------------------
// ɾ��һ������
void CDataChunk::DelValue(int nKey)
{
	list<CDataUnit*>::iterator it = m_DataList.begin();
	for(; it != m_DataList.end(); ++it)
	{
		CDataUnit *pDataUnit = (*it);
		if (pDataUnit->GetKey() == nKey)
		{
			m_nCurBufLen -= (5 + pDataUnit->GetLength()); // 5 = 4�ֽ�Keyֵ + 1�ֽ�����
			delete pDataUnit;
			m_DataList.erase(it);
			return;
		}
	}
}
//------------------------------------------------------------------------
// ��ȡ���ݸ���
int CDataChunk::GetValueNum(void)
{
	return m_DataList.size();
}
//------------------------------------------------------------------------
// �����������chunk�ײ�
void CDataChunk::ResetBegin(void)
{
	m_itLoop = m_DataList.begin();
}
//------------------------------------------------------------------------
// �ڵ����������������
CDataUnit* CDataChunk::GetLoop(void)
{
	if (m_itLoop == m_DataList.end())
	{
		return NULL;
	}

	CDataUnit *pDataUnit = (*m_itLoop);
	++m_itLoop;

	return pDataUnit;
}
//------------------------------------------------------------------------