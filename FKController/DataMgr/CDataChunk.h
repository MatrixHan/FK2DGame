/**
*	created:		2012-6-24   20:20
*	filename: 		CDataChunk
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		DataChunk���ݽṹͷ�ļ�
					����ṹ���£�
					�����ݳ���(4�ֽ�) + N�����ݿ�
					���ݿ�ṹ���£�
					4�ֽ�Keyֵ + 1�ֽ����� + ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <list>
using std::list;
//------------------------------------------------------------------------
class CDataUnit;
//------------------------------------------------------------------------
// DataChunk��
class CDataChunk
{
public:
	// ���캯��
	CDataChunk();
	// ���캯��
	CDataChunk(int nMaxBufLen);
	// ��������
	~CDataChunk();

	// ����DataChunk������������ݳ���
	BOOL SetMaxBufferLen(int nMaxBufLen);
	// ��ȡDataChunk������������ݳ���
	int GetMaxBufferLen(void);

	// ����DataChunk����������
	BOOL SetBuffer(char *pBuf, int nLen);
	// ��ȡDataChunk����������
	BOOL GetBuffer(char *pBuf, int nLen);
	// ��ȡDataChunk���������ݳ���
	int GetBufferLen(void);
	// ���DataChunk����������
	void Clear(void);

	// ����һ������
	BOOL SetValue(CDataUnit &DataUnit);
	// ��ȡһ������
	CDataUnit* GetValue(int nKey);
	// ɾ��һ������
	void DelValue(int nKey);
	// ��ȡ���ݸ���
	int  GetValueNum(void);

	// �����������chunk�ײ�
	void ResetBegin(void);
	// �ڵ����������������ָ��
	CDataUnit* GetLoop(void);

private:
	int							m_nMaxBufLen;	// ����������ݳ���
	int							m_nCurBufLen;	// ��ǰ�������ݳ���
	list<CDataUnit*>			m_DataList;		// ����List��
	list<CDataUnit*>::iterator	m_itLoop;		// ���ڱ���ȡ���ݵ��м������

};
//------------------------------------------------------------------------