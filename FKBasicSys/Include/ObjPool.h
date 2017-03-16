/**
*	created:		2012-7-1   19:48
*	filename: 		ObjPool
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���󻺳��
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#ifdef _DEBUG
#define __USE_MALLOC
#endif
//------------------------------------------------------------------------
#include <hash_map>
#include <list>
#include <map>
using std::map;
using std::list;
using stdext::hash_map;
//------------------------------------------------------------------------
// ������Ҫ����Ķ������ʵ�ֵĽӿ�
struct IPoolNodeObject
{
	virtual DWORD GetSize() = NULL;
	virtual void Release() = NULL;
};
//------------------------------------------------------------------------
// �ؽڵ�
struct CPoolNode
{
	IPoolNodeObject* m_pObj;
	DWORD			 m_dwID;
	int				 m_nRefCount;

	bool operator==( const CPoolNode& p_Other )const
	{
		if( p_Other.m_dwID == this->m_dwID )
		{
			return true;
		}
		return false;
	}

	CPoolNode(): m_pObj(0), m_dwID(0), m_nRefCount(0){}
	virtual ~CPoolNode(){}
	CPoolNode(const CPoolNode& apn): m_pObj(apn.m_pObj), m_dwID(apn.m_dwID), m_nRefCount(apn.m_nRefCount) {}
	CPoolNode(IPoolNodeObject* pObj, DWORD dwID): m_pObj(pObj), m_dwID(dwID){m_nRefCount=0;}
	CPoolNode& operator = (const CPoolNode& pn){m_pObj=pn.m_pObj;m_dwID=pn.m_dwID;m_nRefCount=pn.m_nRefCount;return *this;}
	// �õ�����
	IPoolNodeObject* GetPoolNodeObject(){return m_pObj;}
	// ��ȡ����Ĵ�С
	virtual DWORD GetSize(){if (m_pObj) return m_pObj->GetSize();return 0;}
	// ÿ��������һ����ֵ��֮��Ӧ
	virtual DWORD GetKey(){return m_dwID;}
	// �õ��ڵ�����ô���
	virtual int GetRefCount() {return m_nRefCount;}
	// ���һ������
	virtual int AddRef() {return ++m_nRefCount;}
	// ��ȥһ������(���ǵ�����صĿɻ����ԣ������������ü���Ϊ0ʱ������Release����)
	virtual int SubRef() {if(--m_nRefCount<=0) m_nRefCount=0;return m_nRefCount;}
	// �ͷŶ���(���������ü���)
	virtual void ReleasePoolNodeObject(){if(m_pObj){m_pObj->Release();m_pObj=0;}}
};
//------------------------------------------------------------------------
/*
 * ���룺
 * ��	PoolNodeType		������̳���CPoolNode��ʵ��CPoolNode�����й���
 *		key_type			�������ͣ���DWORD,__int64��

 * ˼·��
 * �������ü���, ����ص����������ⲿ��̬����(Ĭ��128M)
 * ���� list : һ����׼�б���¼����ʹ�õĽڵ㣻һ���ɻ����б���¼���ӵ��Ľڵ�, ������list�Ŀ�����ɾ����
 * �ڵ�Ĳ�ѯͨ��hash_map���ٲ�ѯ��hash_map��¼�˸��ڵ�������list�е�λ�ã�������hash_map�Ŀ�����������
 * ������Ϊ0ʱ����Ҫɾ�����ŵ��ɻ���list��
*/
//------------------------------------------------------------------------
#define KBYTE     1024
#define MBYTE     KBYTE * KBYTE
#define GBYTE     MBYTE * MBYTE
//------------------------------------------------------------------------
template <class PoolNodeType = CPoolNode, class key_type = DWORD>
class CObjectPool
{
private:
	typedef list<PoolNodeType>							NodeList;
	//typedef list<PoolNodeType>::iterator				NodeListPtr;
	typedef hash_map<key_type, PoolNodeType>			HashMap;
	//typedef hash_map<key_type, PoolNodeType>::iterator	HashMapPtr;

	DWORD		m_dwMemSize;		// ��ǰ���ж�����ռ���ڴ���
	NodeList	m_NodeList;			// ��׼�ڵ��б�
	NodeList	m_RecycledNodeList;	// �ɻ��սڵ��б�
	HashMap		m_HashMap;			// ��ϣ��
	DWORD		m_dwMaxPoolSize;	// ������ܹ����ɵ�����ڴ���

public:
	CObjectPool()
	{
		Close();
		m_dwMaxPoolSize = 0 * MBYTE;
	}

	virtual ~CObjectPool(){Close();}
	void Close()
	{
		// ���õ�ǰ�б��е�ÿ�������Release
		for ( list<PoolNodeType>::iterator it=m_NodeList.begin(); it!=m_NodeList.end(); it++)
			(*it).ReleasePoolNodeObject();
		for ( list<PoolNodeType>::iterator it=m_RecycledNodeList.begin(); it!=m_RecycledNodeList.end(); it++)
			(*it).ReleasePoolNodeObject();

		m_NodeList.clear();
		m_RecycledNodeList.clear();
		m_HashMap.clear();
		m_dwMemSize = 0;
	}
	
	void Recycle()
	{
		list<PoolNodeType>::iterator rlist_it=m_RecycledNodeList.begin(); 
		while (rlist_it!=m_RecycledNodeList.end())
		{
			m_dwMemSize -= (*rlist_it).GetSize();
			(*rlist_it).ReleasePoolNodeObject();
			m_HashMap.erase((*rlist_it).GetKey());
			m_RecycledNodeList.erase(rlist_it);
			rlist_it=m_RecycledNodeList.begin();
		}
	}
	
	void AddSize(DWORD dwSize)
	{
		m_dwMemSize += dwSize;
	}

	// �жϻ�����Ƿ��Ѿ����
	BOOL IsOverflow()
	{
		MEMORYSTATUS ms;
		GlobalMemoryStatus(&ms);
		if(ms.dwAvailPhys < 100 * MBYTE)
		{
			if(m_dwMaxPoolSize >= 10 * MBYTE)
				m_dwMaxPoolSize -= 10 * MBYTE;
		}
		else
		{
			if(m_dwMaxPoolSize <= 90 * MBYTE)
				m_dwMaxPoolSize += 10 * MBYTE;
		}

		return (m_dwMemSize > m_dwMaxPoolSize);
	}

	BOOL AddObjectRef(PoolNodeType& node)
	{
		if (IsOverflow())
		{
			if(m_RecycledNodeList.size() > 1)
			{
				list<PoolNodeType>::iterator rlist_it=m_RecycledNodeList.begin(); 
				while (rlist_it!=m_RecycledNodeList.end())
				{
					m_dwMemSize -= (*rlist_it).GetSize();
					(*rlist_it).ReleasePoolNodeObject();
					m_HashMap.erase((*rlist_it).GetKey());
					m_RecycledNodeList.erase(rlist_it);
					rlist_it=m_RecycledNodeList.begin();
					if (!IsOverflow())
						break;
				}
			}
		}
		// ���һ������,����¼����ϣ��
		node.AddRef();
		m_NodeList.push_back(node);
		list<PoolNodeType>::iterator itTmp = m_NodeList.end();
		--itTmp;
		m_HashMap[node.GetKey()] = (*itTmp); //--m_NodeList.end();

		return TRUE;
	}
	
	// �Ƴ��ڵ�
	// ������ü���Ϊ0��������ػ��пռ��򽫶����ӵ�����Ͱ��������������ɾ��
	BOOL RemoveObjectRef(key_type idKey)
	{
		hash_map<key_type, PoolNodeType>::iterator it = m_HashMap.find(idKey);
		if (it != m_HashMap.end()) // exist!
		{
			PoolNodeType list_it = (*it).second;

			if (list_it.GetRefCount() >0 && list_it.SubRef() == 0) // �ӵ��ɻ����б��β��
			{
				m_RecycledNodeList.push_back(list_it);
				list<PoolNodeType>::iterator itTmp = m_RecycledNodeList.end();
				--itTmp;
				m_HashMap[idKey] = (*itTmp);
				//m_NodeList.erase(list_it);
				m_NodeList.remove( list_it );

				// �����������ͷſɻ����б�ͷ���Ľڵ㣬ֱ������ز����
				if (IsOverflow() && m_RecycledNodeList.size() > 1)
				{
					list<PoolNodeType>::iterator rlist_it=m_RecycledNodeList.begin(); 
					while (rlist_it!=m_RecycledNodeList.end())
					{
						m_dwMemSize -= (*rlist_it).GetSize();
						(*rlist_it).ReleasePoolNodeObject();
						m_HashMap.erase((*rlist_it).GetKey());
						m_RecycledNodeList.erase(rlist_it);
						rlist_it=m_RecycledNodeList.begin();
						if (!IsOverflow())
							break;
					}					
				}
			}
			return TRUE;
		}
		return FALSE;
	}

	// ���Ҷ�Ӧ��ֵ�Ľڵ�
	// �ҵ���������ü���������ڻ����б��У��������׼�б���
	BOOL Find(key_type idKey, PoolNodeType** ppNode)
	{
		hash_map<key_type, PoolNodeType>::iterator it = m_HashMap.find(idKey);
		if (it != m_HashMap.end()) // exist!
		{
			PoolNodeType list_it = (*it).second;
			
			// ����ڿɻ����б��У���ŵ���׼�б���
			if (list_it.GetRefCount() == 0)
			{
				m_NodeList.push_back(list_it);			// push to back

				// m_HashMap[idKey] = --m_NodeList.end();	// reset pointer
				list<PoolNodeType>::iterator itTmp = m_NodeList.end();
				--itTmp;
				m_HashMap[idKey] = (*itTmp);

				// m_RecycledNodeList.erase(list_it);		// delete node
				m_RecycledNodeList.remove( list_it );

				list_it = (*itTmp);
				// list_it = --m_NodeList.end();			// update pointer
			}

			list_it.AddRef();

			*ppNode = &(list_it);
			return (ppNode != NULL);
		}

		return FALSE;
	}
};
//------------------------------------------------------------------------