/**
*	created:		2012-6-29   14:35
*	filename: 		DataPacker
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���ݴ����(���ݵĳ��Ȳ��ܴ���2048), ������Ϻʹ������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#define PACKER_BUF_SIZE 2048
//------------------------------------------------------------------------
// ���ݴ����
class CDataPacker
{
public:
	CDataPacker(void)
	: m_nDataLen(0)
	{
		memset(m_buf, 0, PACKER_BUF_SIZE);
	}

	virtual ~CDataPacker(void)
	{
	}

	//-------------------------------------------------------------------
	// ��  �����������
	// ��  ����[����] const void * pData - Ҫ��ӵ�����
	// ��  ����[����] unsigned int nLen - Ҫ��ӵ����ݵĳ���
	// ����ֵ��BOOL - ������ӷ���TRUE,���򷵻�FALSE
	//-------------------------------------------------------------------
	BOOL AddData(const void * pData, unsigned int nLen)
	{
		if (m_nDataLen + nLen > PACKER_BUF_SIZE)
		{
			return FALSE;
		}

		memcpy(m_buf + m_nDataLen, pData, nLen);
		m_nDataLen += nLen;

		return TRUE;
	}

	//-------------------------------------------------------------------
	// ��  �������һ��BYTE����
	// ��  ����[����] BYTE nData - ����
	// ����ֵ��BOOL - ������ӷ���TRUE,���򷵻�FALSE
	//-------------------------------------------------------------------
	BOOL AddBYTE(BYTE nData)
	{
		return AddData(&nData, sizeof(BYTE));
	}

	//-------------------------------------------------------------------
	// ��  �������һ��WORD����
	// ��  ����[����] WORD wData - ����
	// ����ֵ��BOOL - ������ӷ���TRUE,���򷵻�FALSE
	//-------------------------------------------------------------------
	BOOL AddWORD(WORD wData)
	{
		return AddData(&wData, sizeof(WORD));
	}

	//-------------------------------------------------------------------
	// ��  �������һ��DWORD����
	// ��  ����[����] DWORD dwData - ����
	// ����ֵ��BOOL - ������ӷ���TRUE,���򷵻�FALSE
	//-------------------------------------------------------------------
	BOOL AddDWORD(DWORD dwData)
	{
		return AddData(&dwData, sizeof(DWORD));
	}

	//-------------------------------------------------------------------
	// ��  ������ȡ������ 
	//-------------------------------------------------------------------
	const char * GetBuffer(void) const
	{
		return m_buf;
	}

	//-------------------------------------------------------------------
	// ��  ������ȡ���ݵĳ��� 
	//-------------------------------------------------------------------
	int GetDataLen(void) const
	{
		return m_nDataLen;
	}

	void SeekTo(int nPtr)
	{
		if( nPtr < 0 || nPtr >PACKER_BUF_SIZE)
		{
			return;
		}
		m_nDataLen = nPtr;
	}

private:
	char m_buf[PACKER_BUF_SIZE];		// ������
	int m_nDataLen;						// ��������ݵĳ���
};
//------------------------------------------------------------------------