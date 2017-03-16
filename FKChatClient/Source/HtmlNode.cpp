/**
*	created:		2012-6-28   8:41
*	filename: 		HtmlNode
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ��HTML�Ľڵ㣬ά���ڵ����Ϣ
*/
//------------------------------------------------------------------------
#include "../Include/HtmlNode.h"
//------------------------------------------------------------------------
CHtmlNode::CHtmlNode(void)
:m_pParentNode(NULL)
{
}
//------------------------------------------------------------------------
CHtmlNode::~CHtmlNode(void)
{
	// �����ӽڵ�
	while (m_children.begin() != m_children.end())
	{
		delete *(m_children.begin());
		m_children.pop_front();
	}
}
//------------------------------------------------------------------------
void CHtmlNode::SetName(string strName)
{
	m_strName = strName;
}
//------------------------------------------------------------------------
void CHtmlNode::SetContent(string strContent)
{
	m_strContent = strContent;
}
//------------------------------------------------------------------------
void CHtmlNode::SetAttribute(string strName, string strValue)
{
	m_attributes[strName] = strValue;
}
//------------------------------------------------------------------------
void CHtmlNode::SetAttribute(string strName, int nValue)
{
	char buf[100] = "";

	sprintf(buf, "%d", nValue);
	m_attributes[strName] = buf;
}

//-----------------------------------------------------------------------------
// ��  �������һ������ 
// ��  ����[����] string strName - ��������
// ��  ����[����] string strValue - ����ֵ
// ����ֵ��bool - ��ӳɹ�����true�����򷵻�false
//-----------------------------------------------------------------------------
bool CHtmlNode::AddAttribute(string strName, string strValue)
{
	if (m_attributes.find(strName) == m_attributes.end())
	{
		m_attributes[strName] = strValue;
		return true;
	}
	else
	{
		return false;
	}
}
//-----------------------------------------------------------------------------
// ��  �������ø���� 
// ��  ����[����] CHtmlNode * pParent - �ӽڵ�ָ��
// ����ֵ��void
//-----------------------------------------------------------------------------

void CHtmlNode::SetParent(CHtmlNode *pParent)
{
	m_pParentNode = pParent;
}
//-----------------------------------------------------------------------------
// ��  ������ȡ�����
// ����ֵ��CHtmlNode
//-----------------------------------------------------------------------------
CHtmlNode* CHtmlNode::GetParent()
{
   return m_pParentNode;
}
//-----------------------------------------------------------------------------
// ��  ������ȡ��һ�������
// ����ֵ��CHtmlNode
//-----------------------------------------------------------------------------
CHtmlNode* CHtmlNode::GetFirstParent()
{
   CHtmlNode *pParent = m_pParentNode;
   while(pParent)
   {
	   if( pParent->GetParent() == NULL )
	   {
		   break;
	   }
	   pParent = pParent->GetParent();

   }
   return pParent;
}
//-----------------------------------------------------------------------------
// ��  �������һ���ӽڵ� 
// ��  ����[����] CHtmlNode * pNode - �ӽڵ�ָ��
// ����ֵ��void
//-----------------------------------------------------------------------------
void CHtmlNode::AddChild(CHtmlNode * pNode)
{
	pNode->SetParent(this);
	m_children.push_back(pNode);
}
//------------------------------------------------------------------------
string CHtmlNode::GetName(void) const
{
	return m_strName;
}
//------------------------------------------------------------------------
string CHtmlNode::GetContent(void) const
{
	return m_strContent;
}
//-----------------------------------------------------------------------------
// ��  ������ȡ����ֵ 
// ��  ����[����] string strName - ��������
// ��  ����[���] string & strValue - ����ֵ
// ����ֵ��bool - ����ܹ���ȡ����ֵ����true�����򷵻�false
//-----------------------------------------------------------------------------
bool CHtmlNode::GetAttribute(string strName, string & strValue) const
{
	hash_map<string, string>::const_iterator itr = m_attributes.find(strName);
	if (itr != m_attributes.end())
	{
		strValue = (*itr).second;
		return true;
	}
	else
	{	
		strValue = "";
		return false;
	}
}
//-----------------------------------------------------------------------------
// ��  ������ȡ����ֵ 
// ��  ����[����] string strName - ��������
// ��  ����[���] string & strValue - ����ֵ
// ��  ����[���] int nDefault - Ĭ������ֵ
// ����ֵ��bool - ����ܹ���ȡ����ֵ����true�����򷵻�false
//-----------------------------------------------------------------------------
bool CHtmlNode::GetAttribute(string strName, int & nValue, int nDefault) const
{
	hash_map<string, string>::const_iterator itr = m_attributes.find(strName);
	if (itr != m_attributes.end())
	{
		nValue = atoi((*itr).second.c_str());
		return true;
	}
	else
	{	
		nValue = nDefault;
		return false;
	}
}
//------------------------------------------------------------------------
const CHtmlNode::NodeList & CHtmlNode::GetChildren(void) const
{
	return m_children;
}
//------------------------------------------------------------------------
DWORD CHtmlNode::GetData(void) const
{
    return m_dwData;
}
//------------------------------------------------------------------------
void CHtmlNode::SetData(DWORD dwData)
{
    m_dwData = dwData;
}
//-----------------------------------------------------------------------------
// ��  ��������ǲ������ӽڵ� 
// ��  ����void
// ����ֵ��bool - ����з���true�����򷵻�false
//-----------------------------------------------------------------------------
bool CHtmlNode::HasChildren(void) const
{
	return (m_children.size() > 0);
}
//------------------------------------------------------------------------