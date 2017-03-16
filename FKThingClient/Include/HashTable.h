/**
*	created:		2012-7-2   19:54
*	filename: 		HashTable
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
template<>
AFX_INLINE UINT AFXAPI HashKey(DWORD key)
{ // override HashKey for my usage
	return key;
}
//------------------------------------------------------------------------
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CHashTable  : public CMap<KEY, ARG_KEY,VALUE,ARG_VALUE>
{
public:
	class CMyAssoc : public CPair
	{
	public:
		CMyAssoc* pNext;
		UINT nHashValue;  // needed for efficient iteration

		CMyAssoc( ARG_KEY key ) : CPair( key ) {}
	};

	CHashTable(int nBlockSize = 10) 
		: CMap<KEY, ARG_KEY,VALUE,ARG_VALUE>(nBlockSize){};
	~CHashTable(){};

	// ����
	BOOL Find(ARG_KEY key)	// ����ָ��key�Ƿ���ڣ������޴���CMap::Lookup()���ؽ��Ŀ���,CMap::operator[] �����������ؼ�һ�����
	{
		UINT nHashBucket,nHashValue;
		CAssoc* pAssoc = GetAssocAt(key, nHashBucket,nHashValue);
		return pAssoc != NULL;
	};

	// ���ң�����ҵ��������ҵ��Ľ��ĵص�ַ���Ա������GetAt()ȡ�ý�㣬��һ�в�����Ϊ���ٶ���
	BOOL Find(ARG_KEY key,POSITION& pos)
	{
		ASSERT_VALID(this);

		UINT nHashBucket,nHashValue;
		CAssoc* pAssoc = GetAssocAt(key, nHashBucket,nHashValue);

		if(pAssoc == NULL)
			return FALSE;
		pos = (POSITION)pAssoc;
		return TRUE;
	}
	
	// ����Find(key,pos)���ص�pos��ȡ��ֵ
	VALUE& GetAt(POSITION pos)
	{
		ASSERT_VALID(this);
		return ((CAssoc*)pos)->value;
	}

	// ������ҵ�һ����Ч�����û�����ָ����һ����ſ�ʼ
	BOOL FindRandomNextNode(ARG_KEY key,POSITION& posNext,VALUE **ppValue = NULL)
	{ 
		// ���ڶ�ȡһ�������û�ʱ����õ�һ����㣬������һ����λ��
		if (m_pHashTable == NULL)
			return FALSE;
		UINT nHash = HashKey<ARG_KEY>(key) % m_nHashTableSize;
		UINT nEnumCount = 0;
		while(m_pHashTable[nHash] == NULL)
		{ 
			// ѭ������һ����Ч���
			//.....ע��˴����ܴ����������⣬�����������Ҵ���50 * m_nHashTableSizeʱ
			nHash++;
			if(nHash >= m_nHashTableSize)
				nHash = 0;
			nEnumCount++;
			if(nEnumCount >= m_nHashTableSize)
				return FALSE;
		}
		posNext = (POSITION)m_pHashTable[nHash];
		if(ppValue != NULL)
			*ppValue = &((CAssoc*)posNext)->value;
		GetNextNode(posNext,NULL);
		return TRUE;
	}
	// �˺�����CMap::GetNextAssoc()һ���������Բ��ط��ؽ��ֵ��Ҳ���Է��ؽ��ֵָ��
	void GetNextNode(POSITION& rNextPosition,VALUE** ppValue = NULL)
	{ 
		ASSERT_VALID(this);
		ASSERT(m_pHashTable != NULL);  // never call on empty map

		CAssoc* pAssocRet = (CAssoc*)rNextPosition;
		ASSERT(pAssocRet != NULL);

		if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)
		{
			// find the first association
			for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
				if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
					break;
			ASSERT(pAssocRet != NULL);  // must find something
		}

		// find next association
		ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));
		CMyAssoc* pAssocNext;
		if ((pAssocNext = ((CMyAssoc*)pAssocRet)->pNext) == NULL)
		{
			// go to next bucket
			for (UINT nBucket = (((CMyAssoc*)pAssocRet)->nHashValue % m_nHashTableSize) + 1;
			  nBucket < m_nHashTableSize; nBucket++)
				if ((pAssocNext = (CMyAssoc*)m_pHashTable[nBucket]) != NULL)
					break;
		}

		rNextPosition = (POSITION) pAssocNext;

		// fill in return data
		if(ppValue != NULL)
			*ppValue = &pAssocRet->value;
	}
};
//------------------------------------------------------------------------