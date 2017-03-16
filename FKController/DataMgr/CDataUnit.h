/**
*	created:		2012-6-24   20:11
*	filename: 		CDataUnit
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		DataUnit���ݽṹͷ�ļ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
// DataUnit�������Ͷ���
enum
{
	DU_NUMBER = 0,	// ��������
	DU_STRING,		// �ַ�������
};
//------------------------------------------------------------------------
// DataUnit������������
union UDataUnit
{
	int		nNumber;	// ����
	char	*szString;	// �ַ���ָ��
};
//------------------------------------------------------------------------
// DataUnit��
class CDataUnit
{
private:
	void init();

public:
	// ���캯��
	CDataUnit();
	// ���캯��
	CDataUnit(CDataUnit &DataUnit);
	// ���캯��
	CDataUnit(int nKey, int nNumber);
	// ���캯��
	CDataUnit(int nKey, char *szString);
	// ���캯��
	CDataUnit(int nKey, int nType, char *pBuf);
	// ��������
	~CDataUnit();

	// ��������
	void Set(CDataUnit &DataUnit);
	// ��������
	void Set(int nKey, int nNumber);
	// ��������
	void Set(int nKey, char *szString);
	// ��������
	void Set(int nKey, char *szString, int nLen);
	// ��������
	void Set(int nKey, int nType, char *pBuf);
	// �������
	void Clear(void);

	// intǿ��ת������
	operator int();
	// char*ǿ��ת������
	operator char*();

	// ��ȡKeyֵ
	int GetKey(void);
	// ��ȡ����
	int GetType(void);
	// ��ȡ����ָ��
	char* GetBuffer(void);
	// ��ȡ���ݳ���
	int GetLength(void);
	// �����Ƿ�Ϊ��
	BOOL IsEmpty(void);

private:
	int			m_nKey;		// Keyֵ
	int			m_nType;	// ����
	UDataUnit	m_Data;		// ����

};
//------------------------------------------------------------------------