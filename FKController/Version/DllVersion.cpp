/**
*	created:		2012-6-24   20:35
*	filename: 		DllVersion
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "DllVersion.h"
#include "../FKControllerInclude.h"
//------------------------------------------------------------------------
CDllVersion g_DllVersion;
static char *szFileName = "FKController.dll";
//------------------------------------------------------------------------
CDllVersion::CDllVersion()
{
	m_bIsValid = FALSE;
}
//------------------------------------------------------------------------
CDllVersion::~CDllVersion()
{

}
//------------------------------------------------------------------------
BOOL CDllVersion::isValid(void)
{
	return m_bIsValid;
};
//------------------------------------------------------------------------
void CDllVersion::Init(void)
{
	DWORD dwDllHandle = 0;		
	DWORD dwFileInfoSize = GetFileVersionInfoSize(szFileName, &dwDllHandle);
	if (dwFileInfoSize == 0)
	{
		tPrintEventLn("ȡ�ļ���Ϣ��Сʧ��");
		return;
	}

	BYTE* pVerData = new BYTE[dwFileInfoSize] ;
	if (GetFileVersionInfo(szFileName, 0, dwFileInfoSize, pVerData) == FALSE)
	{
		delete []pVerData;
		tPrintEventLn("�޷�ȡ���ļ��汾��Ϣ!");
		return;			
	}

	VS_FIXEDFILEINFO *pInfo;
	unsigned int nLen;
	if (VerQueryValue(pVerData, "\\", (void **)&pInfo, &nLen) == FALSE)
	{
		delete []pVerData;
		tPrintEventLn("�����ļ��ļ��汾ʱ����!");
		return;
	}

	m_Ver[0] = (pInfo->dwProductVersionMS >> 16) & 0x0ffff;
	m_Ver[1] = pInfo->dwProductVersionMS & 0x0ffff;
	m_Ver[2] = (pInfo->dwProductVersionLS >> 16) & 0x0ffff;
	m_Ver[3] = pInfo->dwProductVersionLS & 0x0ffff;

	m_bIsValid = TRUE;

	delete []pVerData;
	return;
}
//------------------------------------------------------------------------
BOOL CDllVersion::isVersionPass(const char *pData, int nLen)
{
	int nNumComma = 0;
	for (int i = 0; i < nLen; ++i)
	{
		if (pData[i] == ',')
		{
			nNumComma++;
		}
	}

	if (nNumComma != 3)
	{//�������������İ汾�д�, ���ÿͻ��˽�ȥ
		tPrintEventLn("�������������İ汾���д�");
		return FALSE;	
	}

	int ServerVer[4];
	sscanf(pData, "%d,%d,%d,%d", ServerVer, ServerVer + 1, ServerVer + 2, ServerVer + 3);

	if (m_Ver[0] != ServerVer[0])
	{
		return m_Ver[0] > ServerVer[0];
	}

	if (m_Ver[1] != ServerVer[1])
	{
		return m_Ver[1] > ServerVer[1];
	}

	if (m_Ver[2] != ServerVer[2])
	{
		return m_Ver[2] > ServerVer[2];
	}

	if (m_Ver[3] != ServerVer[3])
	{
		return m_Ver[3] > ServerVer[3];
	}

	//�汾��ȫ���
	return TRUE;
}
//------------------------------------------------------------------------