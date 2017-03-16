/**
*	created:		2012-6-28   6:45
*	filename: 		ChatFilter
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ����������������˵������ʵ�����
*/
//------------------------------------------------------------------------
#include "../Include/ChatFilter.h"
#include "../Include/ChatClient.h"
//------------------------------------------------------------------------
// �����
const SCmd CMD_TABLE[] = 
{
	{"ping", &CChatFilter::OnPing}
};
//------------------------------------------------------------------------
CChatFilter::CChatFilter(void)
{
	// ��������
	for (int i = 0; i < sizeof(CMD_TABLE) / sizeof(SCmd); i++)
	{
		m_cmds[CMD_TABLE[i].pName] = CMD_TABLE[i].funcCmd;
	}
}
//------------------------------------------------------------------------
CChatFilter::~CChatFilter(void)
{
}

//-------------------------------------------------------------------
// ��  �������ı����й���
// ��  ����[����] const char * pszText - ��Ҫ���˵��ı�
// ����ֵ��string - ���ع��˺���ı�
//-------------------------------------------------------------------
string CChatFilter::Filter(const char * pszText)
{
	int nLen = strlen(pszText);

	if (nLen == 0)
	{
		return "";
	}

	// ȥ��ǰ��Ŀո�
	while (isascii(*pszText) && isspace(*pszText))
	{
		pszText++;
	}

	const char * pStart = pszText;
	const char * pEnd = pStart;

	// ȥ������Ŀո�
	while (*pszText != '\0')
	{
		if (!isascii(*pszText) || !isspace(*pszText))
		{
			pEnd = pszText + 1;
		}

		pszText++;
	}

	// ȥ��ǰ��ո��õ����ı�
	nLen = pEnd - pStart;
	string strText(pStart, nLen);
	
	// ����ȥ����ɫֵ���ı��ĳ���
	pszText = strText.c_str();
	while (*pszText != '\0')
	{
		if (*pszText == '&' && isdigit(*(pszText + 1)))
		{
			if (isdigit(*(pszText + 2)))
			{
				pszText += 3;
				nLen -= 3;
			}
			else
			{
				pszText += 2;
				nLen -= 2;
			}
		}
		else
		{
			pszText++;
		}
	}
	if (nLen <= 0)
	{
		return "";
	}

	// ����ʹ�ñ���ĸ���
	int nFaceCount = 0;
	pszText = strText.c_str();
	while (*pszText != '\0')
	{
		if (*pszText == '#' && isdigit(*(pszText + 1)))
		{
			nFaceCount++;
		}

		pszText++;
	}
	if (nFaceCount > 12)
	{
		CChatClient::Instance()->DisplayFloatMessage(SMT_FLOAT, "��ʹ����̫��ı��飡");
		return "";
	}

	// ��������
	if (ParseCmd(strText))
	{
		return "";
	}

	return strText;	
}

//-------------------------------------------------------------------
// ��  �������������������� 
//-------------------------------------------------------------------
BOOL CChatFilter::ParseCmd(string strText)
{
	if (strText.size() < 2 || strText[0] != '/')
	{
		return FALSE;
	}
	
	// ��ȡ����
	string strCmd;
	for (int i = 1; i < strText.size(); i++)
	{
		char c = strText[i];

		if (isspace(c) || c == '\0')
		{
			break;
		}

		strCmd += tolower(c);
	}

	CmdMapItr itr = m_cmds.find(strCmd);
	if (itr != m_cmds.end())
	{
		// ִ������
		return (this->*((*itr).second))(strText.c_str() + strCmd.size());
	}
	
	return FALSE;
}

//-------------------------------------------------------------------
// ��  ����Ping���� 
//-------------------------------------------------------------------
BOOL CChatFilter::OnPing(const char * pszText)
{
	CChatClient::Instance()->PingStart();
	return FALSE;
}
//------------------------------------------------------------------------