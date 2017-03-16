/**
*	created:		2012-6-26   1:07
*	filename: 		CThingTable
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "PreInclude.h"
#include "CThingTable.h"
//------------------------------------------------------------------------
CThingTable::CThingTable(void)
{
	m_TableEnd = m_table.end();
}
//------------------------------------------------------------------------
CThingTable::~CThingTable(void)
{
	Close();
}
//------------------------------------------------------------------------
// ˵����CThingTable���ص�������ѻ��ղ����Ķ���
//	   ��������ڵ�ͼ�����Ͷ������������������
//		   �Ժ�Ľ�CMapʵ�֣�����Ҳͬʱ�õ�����
void CThingTable::Close(void)
{	
	THASH_THINGTABLE_ITERATOR it;
	int nSize = m_table.size();
	while(m_table.size() > 0)
	{
		it = m_table.begin();
		IThing * pThing = (*it).second;//(*it)->second;	
		pThing->Release(); //ֻ����it++�����ͷţ���Ϊit++�����pThing
	}
}
//------------------------------------------------------------------------
IThing * CThingTable::Find(UID uid)
{
	THASH_THINGTABLE_ITERATOR it = m_table.find(uid);
	if(it == m_TableEnd)
	{
		return NULL;
	}
	return (*it).second;
}
//------------------------------------------------------------------------
// ���أ�
//	  ����ɹ������hash�����Ѿ�����ͬ���󣬷��ر��еĶ���
//	  ���򷵻�pThing
//      ���ʧ�ܣ�����NULL	
IThing * CThingTable::Add(IThing * pThing)
{
	THASH_THINGTABLE_ITERATOR it = m_table.find( pThing->GetUID() );
	if( it != m_table.end() )
	{
		return (*it).second;
	}
	m_table.insert( make_pair( pThing->GetUID(), pThing ) );
	return pThing;
}
//------------------------------------------------------------------------
// �Ƴ�
BOOL CThingTable::Remove(UID uid)
{
	int nTestDebug = m_table.size();
	BOOL bResult = m_table.erase(uid);
	nTestDebug = m_table.size();

	return bResult;
}
//------------------------------------------------------------------------
// �Ƴ�
BOOL CThingTable::Remove(IThing * pThing)
{
	return Remove(pThing->GetUID());
}
//------------------------------------------------------------------------
// ��  ����ȡ��thing�б�
TVECTOR_THING * CThingTable::GetThingList(void)
{
	// ���
	m_vectorThing.clear();

	// ���
	THASH_THINGTABLE_ITERATOR it = m_table.begin();
	for( ; it != m_table.end(); ++it)
	{
		m_vectorThing.push_back((*it).second);
	}

	return (&m_vectorThing);
}
//------------------------------------------------------------------------