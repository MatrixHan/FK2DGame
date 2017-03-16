/**
*	created:		2012-6-28   8:40
*	filename: 		HtmlNode
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		HTMLEx�ڵ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <list>
#include <hash_map>
#include <string>
#include <Windows.h>
//------------------------------------------------------------------------
using std::list;
using std::string;
using stdext::hash_map;
//------------------------------------------------------------------------
// HTML�ڵ���
class CHtmlNode  
{
public:
	typedef list<CHtmlNode *> NodeList;

public:
	CHtmlNode(void);
	virtual ~CHtmlNode(void);

public:
	bool HasChildren(void) const;
	const NodeList & GetChildren(void) const;
	bool GetAttribute(string strName, string & strValue) const;
	bool GetAttribute(string strName, int & nValue, int nDefault) const;
	DWORD GetData(void) const;
	string GetContent(void) const;
	string GetName(void) const;
	void AddChild(CHtmlNode * pNode);
	bool AddAttribute(string strName, string strValue);
	void SetAttribute(string strName, string strValue);
	void SetAttribute(string strName, int nValue);
	void SetContent(string strContent);
	void SetName(string strName);	
    void SetParent(CHtmlNode *pParent);
	void SetData(DWORD dwData) ;
	CHtmlNode* GetParent();
    CHtmlNode* GetFirstParent();

private:
    CHtmlNode *m_pParentNode;				//�����
	string m_strName;						// ����
	string m_strContent;					// ����
	NodeList m_children;					// �ӽڵ��
	DWORD  m_dwData;
	hash_map<string, string> m_attributes;	// ���Ա�	
};
//------------------------------------------------------------------------
