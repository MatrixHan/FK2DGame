/**
*	created:		2012-6-24   20:16
*	filename: 		CDataStream
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "CDataUnit.h"
#include "CDataStream.h"
//------------------------------------------------------------------------
CDataStreamBuild::CDataStreamBuild(void)
{
	m_nOffset = 0;
	m_pBindBuffer = NULL;
	m_nMaxSize = 0;
	m_nTotalDataSize = 0;
}
//------------------------------------------------------------------------
void CDataStreamBuild::Init(void* szBindBuffer, int nMaxSize)
{
	if (NULL == szBindBuffer || nMaxSize < 4)
	{
		return;
	}

	// Ҫƫ���ĸ��ֽ�, �����洢�����ݳ���
	m_nOffset = sizeof(int);

	// ��ǰ������ָ��
	m_pBindBuffer = szBindBuffer;
	ZeroMemory(m_pBindBuffer, nMaxSize);

	// ���볤��
	m_nTotalDataSize = 0;
	memcpy(m_pBindBuffer, &m_nTotalDataSize, sizeof(int));

	// ��������󳤶�
	m_nMaxSize = nMaxSize;
}
//------------------------------------------------------------------------
CDataStreamBuild::CDataStreamBuild(void* szBindBuffer, int nMaxSize)
{
	m_nOffset = 0;
	m_pBindBuffer = NULL;
	m_nMaxSize = 0;
	m_nTotalDataSize = 0;

	Init(szBindBuffer, nMaxSize);
}
//------------------------------------------------------------------------
CDataStreamBuild::~CDataStreamBuild()
{

}
//------------------------------------------------------------------------
// ����д�뻺�塣nLen < 4K
// �����и��ٶ���������밴˳��д����������ݣ���ΪCMapContainer�������ʱ
BOOL CDataStreamBuild::Write(const void* buf, int nLen)
{
	if (NULL == buf || nLen <= 0)
	{
		return FALSE;
	}

	if((m_nOffset + nLen + sizeof(int)) > m_nMaxSize)
	{
		return FALSE;
	}

	// д�����ݿ�ĳ���
	memcpy((char*)m_pBindBuffer + m_nOffset, &nLen, sizeof(int));
	m_nOffset += sizeof(int);

	// д�����ݿ�
	memcpy((char*)m_pBindBuffer + m_nOffset, buf, nLen);
	m_nOffset += nLen;

	m_nTotalDataSize += (sizeof(int) + nLen);
	memcpy(m_pBindBuffer, &m_nTotalDataSize, sizeof(int));

	return TRUE;
}
//------------------------------------------------------------------------
// ����д��DataUnit
BOOL CDataStreamBuild::Write(CDataUnit &DataUnit)
{
	char	szBuf[1024];
	int		nKey = DataUnit.GetKey();

	memcpy(szBuf, &nKey, sizeof(int));
	memcpy(szBuf + sizeof(int), DataUnit.GetBuffer(), DataUnit.GetLength());

	return Write(szBuf, DataUnit.GetLength() + sizeof(int));
}
//------------------------------------------------------------------------
// ��ȡ��ǰ����ָ����ָ�����ݵ�ַ���ʺ�ֱ�Ӳ���������
void * CDataStreamBuild::GetBuffer()
{
	return (char*)m_pBindBuffer + m_nOffset;
}
//------------------------------------------------------------------------
// ��õ�ǰ����ָ��ƫ��
int CDataStreamBuild::GetBufferOffset()
{
	return m_nOffset;
}
//------------------------------------------------------------------------
// ��ȡ����ʱ����ȡʣ�����ݳ��ȣ�д����ʱ��������Ч����������
int CDataStreamBuild::GetLeftBufferLen()
{
	return m_nMaxSize - m_nOffset;
}
//------------------------------------------------------------------------
// ȡ��Buffer�����ݳ���
int CDataStreamBuild::GetTotalDataLen(void)
{
	return m_nTotalDataSize;
}
//------------------------------------------------------------------------
CDataStreamParse::CDataStreamParse(void)
{
	m_pParseBuffer = NULL;
	m_nOffset = 0;
	m_nSize = 0;
	m_nTotalDataSize = 0;
}
//------------------------------------------------------------------------
CDataStreamParse::CDataStreamParse(void * szParseBuffer, int nSize)
{
	m_pParseBuffer = NULL;
	m_nOffset = 0;
	m_nSize = 0;
	m_nTotalDataSize = 0;

	Load(szParseBuffer, nSize);
}
//------------------------------------------------------------------------
CDataStreamParse::~CDataStreamParse(void)
{

}
//------------------------------------------------------------------------
// ����Ҫ������BUFF
BOOL CDataStreamParse::Load(void * szParseBuffer, int nSize)
{
	if(NULL == szParseBuffer || nSize < sizeof(int))
	{
		return FALSE;
	}

	int nTotalDataSize = *((int *)szParseBuffer);
	if (nSize != nTotalDataSize + 4)
	{
		return FALSE;
	}

	m_pParseBuffer = szParseBuffer;
	m_nSize = nSize;
	m_nOffset = sizeof(int);
	m_nTotalDataSize = nTotalDataSize;

	return TRUE;
}
//------------------------------------------------------------------------
// ��ȡ��ǰ����ָ����ָ�����ݵ�ַ���ʺ�ֱ�Ӳ���������
void * CDataStreamParse::GetBuffer(void)
{
	return (char*)m_pParseBuffer + m_nOffset;
}
//------------------------------------------------------------------------
// �ӵ�ǰ����ָ�봦���chunk
BOOL CDataStreamParse::GetChunk(void* buf, int &nLen)
{
	if ((m_nOffset + sizeof(int)) > m_nSize)
	{
		return FALSE;
	}

	int nDataLen = *((int *)((char*)m_pParseBuffer + m_nOffset));
	m_nOffset += 4;
	if(nDataLen > nLen)
	{
		return FALSE;
	}

	if((m_nOffset + nDataLen) > m_nSize)
	{
		return FALSE;
	}

	memcpy(buf, (char*)m_pParseBuffer + m_nOffset, nDataLen);
	m_nOffset += nDataLen;
	nLen = nDataLen;

	return TRUE;
}
//------------------------------------------------------------------------
// �Ƶ��������ײ�
int CDataStreamParse::Reset(void)
{
	int nOld = m_nOffset;
	m_nOffset = sizeof(int);

	return nOld;
}
//------------------------------------------------------------------------
// ��õ�ǰ����ָ��ƫ��
int	CDataStreamParse::GetBufferOffset(void)
{
	return m_nOffset;
}
//------------------------------------------------------------------------
// ��ȡʣ�����ݳ���
int	CDataStreamParse::GetLeftBufferLen(void)
{
	return m_nSize - m_nOffset;
}
//------------------------------------------------------------------------
// ȡ��Buffer�����ݳ���
int CDataStreamParse::GetTotalDataLen(void)
{
	return m_nTotalDataSize;
}
//------------------------------------------------------------------------
BOOL CDataStreamParse::IsEnd(void)
{
	return (m_nOffset >= m_nSize) ? TRUE : FALSE;
}
//------------------------------------------------------------------------