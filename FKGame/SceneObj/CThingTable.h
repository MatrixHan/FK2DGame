/**
*	created:		2012-6-26   0:49
*	filename: 		CThingTable
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�ͻ����߼���������б�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IThing.h"
#include "IThingTable.h"
#include <hash_map>
//------------------------------------------------------------------------
#define THING_TABLE_SIZE	1024
typedef IThing *			THINGPTR;
//------------------------------------------------------------------------
typedef stdext::hash_map< UID, THINGPTR >	THASH_THINGTABLE;
typedef THASH_THINGTABLE::iterator	THASH_THINGTABLE_ITERATOR;
//------------------------------------------------------------------------
// ����
class CThingTable : public IThingTable
{
public:
	// �ر�
	virtual void				Close(void);

	// ����
	virtual IThing *			Find(UID uid);

	// ���
	virtual IThing *			Add(IThing * pThing);

	// �Ƴ�
	virtual BOOL				Remove(UID uid);

	// �Ƴ�
	virtual BOOL				Remove(IThing* pThing);

	// ȡ��thing�б�
	virtual TVECTOR_THING *		GetThingList(void);
public:
	CThingTable(void);
	virtual ~CThingTable(void);
private:
	THASH_THINGTABLE			m_table;
	THASH_THINGTABLE_ITERATOR	m_TableEnd;
	TVECTOR_THING				m_vectorThing;
};
//------------------------------------------------------------------------