/**
*	created:		2012-7-2   1:12
*	filename: 		PalMgr
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "ObjPool.h"
//------------------------------------------------------------------------
enum PaletteType
{
	PalType_Light = 0,		// ����
	PalType_Frozen,			// ����
	PalType_Toxic,			// �ж�
	PalType_Berserk,		// ��
	PalType_Stone,			// ʯ��

	PalType_MaxType,		// �������
};
//------------------------------------------------------------------------
/*
 * ˼·��
 * ��list����ڵ���Ϣ������Ƶ���ķ�list��ͷ����Ƶ���ķ���listβ����������list�Ŀ�����ɾ����
 * �ڵ�Ĳ�ѯͨ��hash_map���ٲ�ѯ��hash_map��¼�˸��ڵ���list�е�λ�ã�������hash_map�Ŀ�����������
*/
typedef __int64  __KEY;
//------------------------------------------------------------------------
class CPalette
{
	BYTE	m_pal[512];
	int		m_nUseColor;
public:
	CPalette(){};
	CPalette(BYTE* pPal, int nUseColor) {SetPalette(pPal, nUseColor);}
	virtual ~CPalette(){};
	
	BYTE* GetPalette() {return m_pal;}
	int GetUseColor() {return m_nUseColor;}
	int GetPalSize() {return m_nUseColor<<1;}
	void SetPalette(BYTE* pPal, int nUseColor);
	void Release() {delete this;}

	void Modify(int nPalTyle);
	
	void Light();
	void Light(int nBrightness, int nContrast);
	void Blue();
	void Green();
	void Red();
};
//------------------------------------------------------------------------
// ��ɫ��ڵ�
class CPalNode
{
	CPalette*	m_pPal;
	__KEY		m_dwID;
public:
	CPalNode() : m_pPal(NULL),m_dwID(0) {}
	CPalNode(CPalette* pPal, __KEY dwID) : m_pPal(pPal),m_dwID(dwID) {}
	virtual ~CPalNode() {}
	__KEY GetKey() {return m_dwID;}
	void Release() {m_pPal->Release();}
	CPalette* GetPal() {return m_pPal;}
};
//------------------------------------------------------------------------
template <DWORD dwMaxPalCount = 100>// ���ɴ�ŵ�ɫ����
class CPalPool
{
private:
	typedef list<CPalNode>							NodeList;
	typedef list<CPalNode>::iterator				NodeListPtr;
	typedef hash_map<__KEY, NodeListPtr>			HashMap;
	typedef hash_map<__KEY, NodeListPtr>::iterator	HashMapPtr;

	NodeList	m_NodeList;		// �ڵ��б�
	HashMap		m_HashMap;		// ��ϣ��

public:
	CPalPool(){Close();}
	virtual ~CPalPool(){Close();}
	void Close()
	{
		// ���õ�ǰ�б��е�ÿ�������Release
		for (NodeListPtr it=m_NodeList.begin(); it!=m_NodeList.end(); it++)
			(*it).Release();

		m_NodeList.clear();
		m_HashMap.clear();
	}
	
	// ���һ���ڵ�,����list��ǰ��
	BOOL AddPal(CPalNode& pNode)
	{
		// ����ڵ��ܳߴ��ڵ���Ŀ������Χ����ɾ���б����Ľڵ�
		while (m_NodeList.size() > dwMaxPalCount)
		{
#ifdef _DEBUG
			static int flag = 0;
			if (flag == 0)
			{
				flag = 1;
				TraceLn("Warning: pal pool overflow!");
			}
#endif
			// remove the back node
			NodeListPtr it = --m_NodeList.end();
			(*it).Release();
			m_HashMap.erase((*it).GetKey());
			m_NodeList.pop_back();
		}

		// ����һ������,����¼����ϣ��
		m_NodeList.push_front(pNode);
		m_HashMap[pNode.GetKey()] = m_NodeList.begin();

		return TRUE;
	}
	
	// ɾ���ڵ㡣  ע:�ڵ㲢δRelease, �����Ǵӳ����Ƴ�,���ڵ�Ŀ���
	// Ȩ�������÷�,rtnNodeΪ���ؽڵ�����ã��Ƿ��ͷ��ɵ��÷����о���
	BOOL RemovePal(__KEY dwID, CPalNode& rtnNode)
	{
		HashMapPtr it = m_HashMap.find(dwID);
		if (it != m_HashMap.end()) // exist!
		{
			NodeListPtr list_it = (*it).second;
			rtnNode = (*list_it);

			m_HashMap.erase(dwID);
			m_dwMemSize -= rtnNode.GetSize();
			m_NodeList.erase(list_it);
			return TRUE;
		}
		return FALSE;
	}

	// ���Ҷ�Ӧ��ֵ�Ľڵ�
	BOOL Find(__KEY dwID, CPalNode*& pNode)
	{
		HashMapPtr it = m_HashMap.find(dwID);
		if (it != m_HashMap.end())
		{
			NodeListPtr list_it = (*it).second;
			
			// erase from back and move to front
			CPalNode node = *list_it;
			m_NodeList.erase(list_it);
			m_NodeList.push_front(node);
			pNode = &m_NodeList.front();
			m_HashMap[dwID] = m_NodeList.begin();

			return (pNode != NULL);
		}
		return FALSE;
	}
};
//------------------------------------------------------------------------
class CPalMgr  
{
	CPalPool<>		m_PalPool;	// ��ɫ�建���
public:
	CPalMgr() {}
	virtual ~CPalMgr() {m_PalPool.Close();};

	// ����ָ��IDָ�����͵ĵ�ɫ�壬û���򴴽�
	CPalette* RequestPal(DWORD dwID, int nCurPalType, int nPalIndex, BYTE* pSrcPal, int nUseColor, int nPalType = PalType_Light)
	{
		CPalNode* pNode;
		__KEY dwNewID = (nPalType<<24) | (nCurPalType<<16) | (nPalIndex+1); // ��ΪnPalIndex=-1,0,...�����Լ�1
		dwNewID = (dwNewID <<32) | dwID;
		if (m_PalPool.Find(dwNewID, pNode))
			return pNode->GetPal();

		if (!pSrcPal)
			return NULL;
		
		CPalette* pPal = new CPalette(pSrcPal, nUseColor);
		if (!pPal)
			return NULL;
		pPal->Modify(nPalType);
		CPalNode node(pPal, dwNewID);
		m_PalPool.AddPal(node);
		return node.GetPal();
	}
};
//------------------------------------------------------------------------
extern CPalMgr	g_PalMgr;
//------------------------------------------------------------------------