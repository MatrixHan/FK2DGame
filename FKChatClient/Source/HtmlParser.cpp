/**
*	created:		2012-6-28   8:44
*	filename: 		HtmlParser
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ�ֶ�HTML�ű��Ľ���
*/
//------------------------------------------------------------------------
#include "../Include/HtmlParser.h"
//------------------------------------------------------------------------
CHtmlParser::CHtmlParser(void)
: m_lpszScript(NULL)
{
}
//------------------------------------------------------------------------
CHtmlParser::~CHtmlParser(void)
{
}
//-----------------------------------------------------------------------------
// ��  ��������HTML�ű�
// ��  ����[����] const char *lpszScript - HTML�ű�
// ����ֵ��CHtmlNode * - ������ȷ����ͷ�ڵ�ָ�룬���򷵻�NULL
//-----------------------------------------------------------------------------
CHtmlNode * CHtmlParser::Parse(const char * lpszScript)
{
	CHtmlNode * pRootNode = NULL;
	m_lpszScript = lpszScript;
	
	// �Ӹ��ڵ㿪ʼ�ݹ���������ڵ�
	SkipBlank();
	if (*m_lpszScript == '<')
	{
		pRootNode = new CHtmlNode();
		if (pRootNode == NULL)
		{			
			return NULL;
		}
		
		if (!ParseNode(pRootNode))
		{			
			delete pRootNode;
			return NULL;
		}
	}

	return pRootNode;
}
//-----------------------------------------------------------------------------
// ��  ��������HTML��һ���ڵ㣬�ӽڵ�ͨ���ݹ���� 
// ��  ����[����] CHtmlNode *pNode - �ڵ�ṹ��ָ��
// ����ֵ��bool - �����ɹ�����true,���򷵻�false
//-----------------------------------------------------------------------------
bool CHtmlParser::ParseNode(CHtmlNode *pNode)
{
	string strTagName;
	
	// ������ǩ����
	m_lpszScript++;
	if(!PickName(strTagName))
	{
		return false;
	}
	pNode->SetName(strTagName);

	// ������ǩ�����Ժ�����
	bool bResult = false;
	if (strTagName == "img" || strTagName == "br"
		  || strTagName == "window")
	{
		bResult = ParseNonregularNode(pNode);
	}
	else
	{
		bResult = ParseRegularNode(strTagName, pNode);
	}

	return bResult;
}

//-----------------------------------------------------------------------------
// ��  ������������ڵ㣬�����йرղ��ֵĽڵ㣬��<text></text> 
// ��  ����[����] string strTagName - ��ǩ������
// ��  ����[���] CHtmlNode *pNode - ��ǩ�Ľڵ�ָ��
// ����ֵ��bool - �����ɹ�����true�����򷵻�false
//-----------------------------------------------------------------------------
bool CHtmlParser::ParseRegularNode(string strTagName, CHtmlNode *pNode)
{				
	// ������ǩ����
	if (!ParseNonregularNode(pNode))
	{
		return false;
	}
	
	// ������ǩ������,���ܰ������ӽڵ�
	string strContent;
	while (*m_lpszScript != '<' || *(m_lpszScript + 1) != '/')
	{
		if (*m_lpszScript == '\0')
		{
			return false;
		}

		if (*m_lpszScript == '<')
		{			
			CHtmlNode * pSubNode = new CHtmlNode();
			if (pSubNode == NULL)
			{
				return false;
			}

			// �����ӽڵ�
			if (!ParseNode(pSubNode))
			{
				delete pSubNode;
				return false;
			}
			else
			{
				pNode->AddChild(pSubNode);
			}

			strContent += "<>";
		}
		else
		{		
			strContent += *m_lpszScript++;
		}
	}
	pNode->SetContent(strContent);	
	
	// ����ǩ�Ĺرղ���, ��</text>
	string strName;
	m_lpszScript += 2;
	PickName(strName);
	if (strName.compare(strTagName) != 0)
	{
		return false;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// ��  ��������������ڵ㣬����û�йرղ��ֵĽڵ㣬��<br>,<img>��
// ��  ����[����] string strTagName - ��ǩ������
// ��  ����[���] CHtmlNode *pNode - ��ǩ�Ľڵ�ָ��
// ����ֵ��bool - �����ɹ�����true�����򷵻�false
//-----------------------------------------------------------------------------
bool CHtmlParser::ParseNonregularNode(CHtmlNode *pNode)
{
	// ������ǩ����
	string strAttributeName;
	string strAttributeValue;
	SkipBlank();
	while (*m_lpszScript != '>')
	{
		// ����ǲ��ǽű�����
		if (*m_lpszScript == '\0')
		{
			return false;
		}
		
		// ��ȡ��������
		if (!PickName(strAttributeName))
		{
			return false;
		}
		
		// ��ȡ����ֵ
		if (!PickValue(strAttributeValue))
		{
			return false;
		}
		
		// ������Լ���û�зָ�
		if (*m_lpszScript != '>' && !iswspace(*m_lpszScript))
		{
			return false;
		}
		
		// ��ӵ��ڵ�
		if (!pNode->AddAttribute(strAttributeName, strAttributeValue))
		{
			return false;
		}
		
		SkipBlank();
	}
	m_lpszScript++;		
	
	return true;
}

//-----------------------------------------------------------------------------
// ��  �������Կհ��ַ� 
// ��  ����void
// ����ֵ��void
//-----------------------------------------------------------------------------
void CHtmlParser::SkipBlank(void)
{
	while (iswspace(*m_lpszScript))
	{
		m_lpszScript++;
	}
}

//-----------------------------------------------------------------------------
// ��  ������ȡ�����ı�
// ��  ����[���] string &strName - �����ı�
// ����ֵ��bool - �ܹ���ȷ��ȡ�����ı�����true�����򷵻�false
//-----------------------------------------------------------------------------
bool CHtmlParser::PickName(string & strName)
{
	strName.clear();

	while ((*m_lpszScript >= 'a' && *m_lpszScript <= 'z')
		|| (*m_lpszScript >= 'A' && *m_lpszScript <= 'Z'))
	{
		strName += *m_lpszScript++;
	}
	
	// �����е���ĸתΪСд
	for (string::iterator itr = strName.begin(); itr != strName.end(); itr++)
	{	
		*itr = tolower(*itr);
	}
	
	return (!strName.empty());
}

//-----------------------------------------------------------------------------
// ��  ������ȡ����ֵ�ı�
// ��  ����[���] string &strValue - ����ֵ�ı�
// ����ֵ��bool - �ܹ���ȷ��ȡ����ֵ����true�����򷵻�false
//-----------------------------------------------------------------------------
bool CHtmlParser::PickValue(string & strValue)
{
	// ��鿪ʼ�Ĳ���="
	if (*m_lpszScript != '='     || *(m_lpszScript + 1) != '\"'
		|| *m_lpszScript == '\0' || *(m_lpszScript + 1) == '\0')
	{
		return false;
	}
	m_lpszScript += 2;
	strValue.clear();
	
	// ��ȡ����ֵ
	while (*m_lpszScript != '\"')
	{
		if (*m_lpszScript == '\0')
		{
			return false;
		}

		strValue += *m_lpszScript++;
	}
	m_lpszScript++;
	
	return true;
}
//------------------------------------------------------------------------