/**
*	created:		2012-6-30   21:16
*	filename: 		EventServerTemplate
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�¼�������ģ���ļ�
*/
//------------------------------------------------------------------------
#include <list>
#include <hash_map>
#include <time.h>
#include <map>
using std::list;
using std::map;
using stdext::hash_map;
//------------------------------------------------------------------------
#ifdef TraceLn
#undef TraceLn
#endif
#ifdef _DEBUG
#define TraceLn			g_DefaultTrace.ThreadSafeDebugPrintLn
#else
#define TraceLn			__noop
#endif
//------------------------------------------------------------------------
#define MAX_FIRE_NUM 15		//���Fire����
#define MAX_REF_NUM	5		//������ø���
//------------------------------------------------------------------------
struct _SEventKey
{
	DWORD SourcePointer;	//�¼�Դָ��
	BYTE Reserve;			//����
	BYTE TypeID;			//�¼�Դ����
	WORD ID;				//�¼�ID 
};
//------------------------------------------------------------------------
//struct hash_Spoint;
//�¼�Key,��ΪHash_Map��Key
struct SEventKey
{
	union
	{
		_SEventKey _Key;
		unsigned __int64	   Value;
	};
	SEventKey()
	{
		Value = 0;
	}
	bool operator== (const SEventKey &EventKey) const
	{
		return Value == EventKey.Value;
	}
	bool operator < (const SEventKey &EventKey) const
	{
		return Value < EventKey.Value;
	}
	//size_t operator()(const SEventKey & EventKey) const
	//{
	//	DWORD k1 = (EventKey._Key.SourcePointer & 0xFFFF)<<16;
	//	DWORD k2 = (EventKey._Key.ID & 0xFF)<<8;
	//	DWORD k3 = EventKey._Key.TypeID;
	//	return k1 + k2 + k3;
	//}
};
//------------------------------------------------------------------------
//�ػ�hash����
//struct hash_Spoint
//{
//	static const size_t bucket_size = 4;
//	static const size_t min_buckets = 8;
//
//	size_t operator()(const SEventKey & EventKey) const
//	{
//		DWORD k1 = (EventKey._Key.SourcePointer & 0xFFFF)<<16;
//		DWORD k2 = (EventKey._Key.ID & 0xFF)<<8;
//		DWORD k3 = EventKey._Key.TypeID;
//		return k1 + k2 + k3;
//	}
//};
//struct std::hash<SEventKey>
//{
//	size_t operator() (const SEventKey &EventKey) const
//	{
//		DWORD k1 = (EventKey._Key.SourcePointer & 0xFFFF)<<16;
//		DWORD k2 = (EventKey._Key.ID & 0xFF)<<8;
//		DWORD k3 = EventKey._Key.TypeID;
//		return k1 + k2 + k3;
//	}
//};
//------------------------------------------------------------------------
template< class Listener,  class OnEvent >
class TEventServer 
{	
private:	
	struct SKeyInfo
	{
		Listener *pListener;
		const char *pDescription;
		int nRef;
		BOOL bRemove;
		SKeyInfo(Listener *pListen, const char *pDes) : pDescription(pDes)
		{
			pListener = pListen;		
			nRef = 0;
			bRemove = FALSE;			
		}
		void Lock()
		{
			nRef++;
		}
		void UnLock()
		{
			nRef--;
		}

		bool operator ==(const SKeyInfo &p_Other ) const
		{
			if( pListener != p_Other.pListener )
			{
				return false;
			}
			if( pDescription != p_Other.pDescription )
			{
				return false;
			}
			if( nRef != p_Other.nRef )
			{
				return false;
			}
			if( bRemove != p_Other.bRemove )
			{
				return false;
			}
			return true;
		}
	};
	//------------------------------------------------------------------------
	typedef list< SKeyInfo > KEY_INFO_LIST;
	typedef map< DWORD, SKeyInfo* > LISTENER_POINT_MAP;
	//------------------------------------------------------------------------
	struct SSafeListMap
	{
		KEY_INFO_LIST List;
		LISTENER_POINT_MAP Map;
	};
	typedef map< SEventKey, SSafeListMap > LISTENER_MAP;
public:
	TEventServer()
	{
		m_pTreeNode = NULL;
		m_nTreeNodeNumber = 0;	
		m_nFireNum = 0;
		memset(m_dwIDs, 0, sizeof(m_dwIDs));
		for (int i=0; i<MAX_SRC_TYPE_ID; i++)
		{
			m_ppdwEventIDAddNum[i] = new WORD[65535];
			memset(m_ppdwEventIDAddNum[i], 0, 65535*sizeof(WORD));
		}	
	}

	~TEventServer()
	{
		for(LISTENER_MAP::iterator iter = m_ListenerMap.begin();
			iter != m_ListenerMap.end(); ++iter)
		{
			SSafeListMap &SafeListMap = (*iter).second;
			SafeListMap.List.clear();
			SafeListMap.Map.clear();
		}
		m_ListenerMap.clear();
		if (m_pTreeNode != NULL)
		{
			delete[] m_pTreeNode;
			m_pTreeNode = NULL;
		}
		for (int i=0; i<MAX_SRC_TYPE_ID; i++)
		{
			delete[] m_ppdwEventIDAddNum[i];
			m_ppdwEventIDAddNum[i] = NULL;
		}	
	}
	BOOL AddListener(Listener *pListener,
		WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer,
		const char *pDescription)
	{
		if (pListener != NULL)
		{		
			//�¼�Key��ֵ	
			SEventKey EventKey;
			EventKey._Key.ID = wEventID;
			EventKey._Key.TypeID = bSrcTypeID;
			EventKey._Key.SourcePointer = dwSrcPointer;

			LISTENER_MAP::iterator iter = m_ListenerMap.find(EventKey);
			if (iter == m_ListenerMap.end())
			{
				//������
				SSafeListMap ListMap;
				m_ListenerMap.insert(LISTENER_MAP::value_type(EventKey, ListMap));

				iter = m_ListenerMap.find(EventKey);
				if( iter  == m_ListenerMap.end())
				{
					return FALSE;
				}
				SSafeListMap &SafeListMap = (*iter).second; 

				SKeyInfo KeyInfo(pListener, pDescription);		//�����µĽڵ�
				SafeListMap.List.push_front(KeyInfo);			//��ӵ���ͷ
				SafeListMap.Map[(DWORD)pListener] = &(*SafeListMap.List.begin());

			}
			else
			{
				SSafeListMap &SafeListMap = (*iter).second;

				SKeyInfo KeyInfo(pListener, pDescription);//�����µĽڵ�
				SafeListMap.List.push_front(KeyInfo);//��ӵ���ͷ
				SafeListMap.Map[(DWORD)pListener] = &(*SafeListMap.List.begin());
			}
			m_ppdwEventIDAddNum[bSrcTypeID][wEventID] = 1;

			return TRUE;		
		}
		return FALSE;
	}
	BOOL RemoveListener(Listener *pListener,
		WORD wEventID, BYTE bSrcTypeID, DWORD dwSrcPointer)
	{
		if (pListener != NULL)
		{
			//�¼�Key��ֵ
			SEventKey EventKey;
			EventKey._Key.ID = wEventID;
			EventKey._Key.TypeID = bSrcTypeID;
			EventKey._Key.SourcePointer = dwSrcPointer;

			LISTENER_MAP::iterator mapiter = m_ListenerMap.find(EventKey);
			if (mapiter != m_ListenerMap.end())
			{
				//�����и��¼�KEY 
				SSafeListMap &SafeListMap = (*mapiter).second;

				LISTENER_POINT_MAP::iterator it = SafeListMap.Map.find((DWORD)pListener);
				if (it != SafeListMap.Map.end())
				{
					//������
					SKeyInfo &KeyInfo = (*(*it).second);//����λ��ȡ���ڵ�
					KEY_INFO_LIST::iterator Listiter;
					for( Listiter = SafeListMap.List.begin(); Listiter != SafeListMap.List.end(); ++Listiter )
					{
						if( *Listiter == KeyInfo )
						{
							break;
						}
					}
					if( Listiter == SafeListMap.List.end() )
					{
						return FALSE;
					}

					SafeListMap.Map.erase(it);//ɾ��,��Ϊ��������п���ɾ����map�ڵ㣬�����ᵽǰ��

					if (KeyInfo.nRef == 0)//����ɾ����
					{						
						SafeListMap.List.erase(Listiter);

						if (SafeListMap.List.empty())
						{
							//ɾ����map�ڵ�
							m_ListenerMap.erase(mapiter);
						}
					}
					else
					{
						//������ɾ��
						KeyInfo.bRemove = TRUE;
					}										
				}
			}

			return TRUE;
		}

		return FALSE;
	}
	BOOL Fire(IEvent *pEvent)
	{
		if (pEvent == NULL)
		{
			return FALSE;
		}


		//�¼�Key��ֵ	
		SEventKey EventKey;
		EventKey._Key.ID = pEvent->GetID();
		EventKey._Key.TypeID = pEvent->GetSrcTypeID();		

		BOOL bResult = TRUE;
		do {
			EventKey._Key.SourcePointer = pEvent->GetSrcPointer();
			if (EventKey._Key.SourcePointer != 0)
			{
				bResult &= Fire(EventKey, pEvent);//�����巢
				if (!bResult) 
				{
					break;
				}
			}

			EventKey._Key.SourcePointer = 0;
			bResult &= Fire(EventKey, pEvent);//�����ͷ�
			if (!bResult)
			{
				break;
			}

			//�ж��Ƿ񵽴���ڵ�
			if (EventKey._Key.TypeID == 0 ) 
			{
				//�Ѿ�����root
				break;
			}
			else
			{
				//��Ҫ�����ɷ�
				if (m_pTreeNode != NULL && EventKey._Key.TypeID < m_nTreeNodeNumber)
				{
					//���������ɷ�					
					EventKey._Key.TypeID = m_pTreeNode[EventKey._Key.TypeID];//�ı�����ID 
				}
				else
				{
					//���������ɷ�
					break;
				}
			}
		} while(TRUE);

		return bResult;
	}
	void SetTreeNode(const BYTE *pTreeNode, int nTreeNodeNumber)
	{
		if (pTreeNode != NULL && nTreeNodeNumber > 0)
		{
			m_nTreeNodeNumber = nTreeNodeNumber;
			if (m_pTreeNode == NULL)
			{
				m_pTreeNode = new BYTE[m_nTreeNodeNumber];
			}
			if (m_pTreeNode != NULL)
			{
				memcpy(m_pTreeNode, pTreeNode, m_nTreeNodeNumber);
			}			
		}
	}
private:
	BOOL Fire(SEventKey &EventKey, IEvent *pEvent)
	{
		if (m_ppdwEventIDAddNum[EventKey._Key.TypeID][EventKey._Key.ID] == 0)
		{
			return TRUE;
		}
		m_dwIDs[m_nFireNum++] = EventKey._Key.ID;
		//��ֹ��ջ���
		if (m_nFireNum >= MAX_FIRE_NUM)
		{
			memcpy(NULL, NULL, -1); // ֱ��������Ƿ������ڵ���
			TraceLn("�¼�������Fire�������� %d", m_nFireNum);
			m_nFireNum--;
			return FALSE;
		}
		LISTENER_MAP::iterator mapiter = m_ListenerMap.find(EventKey);
		if (mapiter != m_ListenerMap.end())
		{
			//�����и��¼�KEY 
			SSafeListMap &SafeListMap = (*mapiter).second;
			KEY_INFO_LIST::iterator listiter = SafeListMap.List.begin();
			for(;listiter != SafeListMap.List.end();)
			{
				SKeyInfo &KeyInfo = (*listiter);
				//��ֹ��ջ���
				if (KeyInfo.nRef >= MAX_REF_NUM)
				{
					//					PrintEventLn("ͬһ���¼����������� %d", KeyInfo.nRef);
					m_nFireNum--;
					m_dwIDs[m_nFireNum] = 0;
					return FALSE;
				}
				if (!KeyInfo.bRemove)
				{
					//����ڵ���Ч����Ҫ�ص�
					BOOL bResult = FALSE;
					try
					{
						KeyInfo.Lock();
						bResult = m_FireEvent(KeyInfo.pListener, pEvent);
						KeyInfo.UnLock();
					}
					catch (...)
					{
						TraceLn("�¼�����������!!! EventID = %d, FireNum = %d, Desc = %s", pEvent->GetID(),
							m_nFireNum,
							KeyInfo.pDescription);
						FILE *fp = fopen("EventErr.txt", "a");
						if(fp != NULL)
						{
							time_t	long_time;
							struct tm * local_time;
							time(&long_time);
							local_time = localtime(&long_time);

							fprintf(fp, "[%02d.%02d,%02d:%02d:%02d] EventID = %d, FireNum = %d, Desc = %s\n", local_time->tm_mon + 1, 
								local_time->tm_mday,
								local_time->tm_hour,
								local_time->tm_min,
								local_time->tm_sec,
								pEvent->GetID(),
								m_nFireNum,
								KeyInfo.pDescription);
							fclose(fp);
						}
					}

					if (KeyInfo.bRemove && KeyInfo.nRef == 0)//��Ҫ�����ܹ����б���ɾ��
					{
						listiter = SafeListMap.List.erase(listiter);

						if (SafeListMap.List.empty())
						{
							//ɾ����map�ڵ�
							m_ListenerMap.erase(mapiter);

							m_nFireNum--;
							m_dwIDs[m_nFireNum] = 0;

							return bResult;
						}
					}					
					else
					{
						++listiter;
					}										
					if (!bResult) 
					{
						//ʧ��
						m_nFireNum--;
						m_dwIDs[m_nFireNum] = 0;
						return FALSE;
					}
				}
				else //��Ҫ���б���ɾ��
				{
					if(KeyInfo.nRef == 0)//�ܹ����б���ɾ��					
					{
						listiter = SafeListMap.List.erase(listiter);

						if (SafeListMap.List.empty())
						{
							//ɾ����map�ڵ�
							m_ListenerMap.erase(mapiter);
							break;
						}
					}
					else
					{
						++listiter;
					}
				}
			}
		}
		m_nFireNum--;
		m_dwIDs[m_nFireNum] = 0;
		return TRUE;
	}

private:
	OnEvent m_FireEvent;
	LISTENER_MAP		m_ListenerMap;	
	BYTE *m_pTreeNode;
	int m_nTreeNodeNumber;	
	int m_nFireNum;	
	DWORD m_dwIDs[1024];
	WORD *m_ppdwEventIDAddNum[MAX_SRC_TYPE_ID];
};
//------------------------------------------------------------------------