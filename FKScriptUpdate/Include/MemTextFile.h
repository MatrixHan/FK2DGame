/**
*	created:		2012-7-1   0:05
*	filename: 		MemTextFile
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ڴ��ı��ļ�(���мӽ��ܹ���)
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <stdio.h>
#include <windows.h>
//------------------------------------------------------------------------
class CMemTextFile  
{
protected:
	char*	m_pBuffer;		// �ڴ��ļ����棬�����˽��ܺ���ı��ļ�����
	int		m_nBufSize;		// ����ߴ�
	int		m_nFileSize;	// �ļ��ߴ�
	char*	m_pPos;			// �ļ�ָ��λ�ã�����GetLine�Ĳ���
public:
	CMemTextFile() : m_pBuffer(NULL), m_nBufSize(0), m_nFileSize(0), m_pPos(NULL) {}
	~CMemTextFile();

	BOOL IsEnd()							{return m_pPos - m_pBuffer >= m_nFileSize;}
	BOOL Open(LPCSTR szFileName);
	BOOL Save(LPCSTR szFileName);
	char* GetLine(char* pString, int nSize);
	BOOL MakeSimpleMap(BYTE* buf, int nLen);
};
//------------------------------------------------------------------------
