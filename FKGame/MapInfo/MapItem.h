/**
*	created:		2012-6-21   23:05
*	filename: 		MapItem
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKFileSystem/Include/FKTempl.h"
#include "../../FKGame/DataArchive/IDataArchive.h"
//------------------------------------------------------------------------
// �����ֵ������ʬ����Զ��ŵ���ײ�
// CMapContainer::AddLayerItem()ʱ����ݲ�ֵ��С����������뵽�ĸ�λ��
enum 
{
	milvNoSort = 0,/*����Ҫ����*/
	milvSpecificBlock = 2,
	milvLowest = 10, 
	milvMidle = 100,
	milvHight = 1000,
	milvHighest = 10000
};
//------------------------------------------------------------------------
// ռλ���ε�(top,left)Ҫ�������ŵ���붨��������ǵ��Ǹ�����
struct COccupantTileListHeader
{
	short nOffAnchorX;	// ������Է��õ�(��ê��)��ƫ��
	short nOffAnchorY;
	WORD wWidth;		// ռλ���εĿ�
	WORD wHeight;		// ռλ���εĸ�
};
//------------------------------------------------------------------------
//�����ͼ��ITEM��PIC ID
enum MINIITEMPICID
{
	PIC_MIN = 0,
	PIC_NPC = 0,
	PIC_ORE,
	PIC_MEDICIAL,
	PIC_PET,
	PIC_DOOR,
	PIC_TEAMMATE,
	PIC_OTHERACTOR,
	PIC_SELF,
	PIC_MONSTER,
	PIC_MAX,	
};
//------------------------------------------------------------------------
// ���ƣ�ռλ���б�
// ���ܣ����������ռλ�飬���ڴ��������򡢴����嵼�µر���ͨ����Ϣ
typedef CMultLenArray<COccupantTileListHeader, char> COccupantTileList;
//------------------------------------------------------------------------
// ����
class CMapItem
{
public:
	// ͨ�ÿ�������ʺ��ڻ�����Ϣ�Ĺ������ź���Ը�С��
	// ������OnCommand()�﷢���¼�ʱ��Ҳ����Ϣ�ķ�ʽ֪ͨ������������SendMessage()
	// ��windows��Ϣ���Ʋ�ͬ�������SendMessage()ֻ�ǵ���������Ľӿں���
	virtual int					OnCommand(DWORD dwCmdID, DWORD dwParam1,DWORD dwParam2)
	{
		return 0; 
	}
	// ��ȡ�������͡����ɫ��NPC����Ʒ�������ȣ��ǰ�λ�����
	virtual DWORD				GetType(void) const 
	{
		return 0; 
	}
	// ��ȡ�������ID����Ҫ���ڵ�ͼ�洢��Ҳ������ʶ��GetType()֮��ĸ��Ӷ����࣬��NPC�кܶ���
	// ����ClassID���Խ���ַǿ��ת����Ӧ�Ķ�����ָ��
	// ��ͼʱ�Ĵ洢��ʽһ��Ϊ : dwClassID + wLen + data
	virtual	DWORD				GetClassID(void) const
	{
		return 0; 
	}
	// �����ڵ�ͼ�е���ʾ�㣬��ʬ���ȵ������Ҫ�ͣ��в�ι�ϵ�Ķ���ͨ��CMapContainer::AddLayerItem()���뵽��ͼ������
	// ������ʵ�ֵĶ�����Ҫ���ǲ�ֵ
	virtual DWORD				GetLayerValue(void) 
	{
		return milvMidle; 
	}
	// ��ʱGetType()ֻ�ܷ��ض���Ĵ������ض�����������Լ���������,GetType()��ʵ��GetItemData������ʽ
	// ע����һ����������Ԥ��������
	virtual DWORD				GetItemData(void) const
	{
		return 0; 
	}
	// ���ö����˽�����ݣ�����ԭ�ȵ�ֵ
	// ���ڶ��������Զ������ݣ����ͼ�༭����Ҫ�����������
	virtual void				SetItemData(DWORD dwData)
	{

	}
	//	������ȡ���ݣ����ڴ�map�ļ��ж�ȡ�������ݻ���Ϸ�еĶ����ڵ�ͼ�е���Ϣ���浽map�ļ���
	//	������ӿ�ʵ�ֶ���ı���ʹ���ʱ�ĳ�ʼ��
	//	���룺
	//		bSaveΪTRUEʱ����ʾ���󱣴��������
	//		bSaveΪFALSEʱ,��ʾ��ȡ�����ֳ������ö�������
	//	���أ�
	//		����TRUE:
	//			�浵�ɹ�
	//		����FALSE��
	//			�������ɹ�������Ҫ����˶���
	//
	//	˵����
	//		��غ�������GetClassID()����ͼ�༭�������ͼʱ�Ĵ洢��ʽһ��Ϊ��dwClassID + wLen + data
	// enum{sWrite/*����*/ = TRUE,sRead/*����*/ = FALSE};
	virtual BOOL				OnSerialize(BOOL bWrite, IArchive* pArchive)
	{
		return FALSE; 
	}
	// ���������ֳ���������
	virtual BOOL				OnCreateItem(const char* buf, int nLen)
	{ 
		return FALSE; 
	}
	// ��ռλ�����ռλֵ������˶���Ӱ��ر�Ŀ�ͨ���ԣ��շ��ط�0ֵ
	// �ж�ռλ�б�Ķ�����޶�ռλ�б�Ķ���Ӧ�����������ֵ
	// �ж�ռλ�б�Ķ��󷵻ػ���ռλֵ
	// Ĭ��Ϊ�з��С����߸񵲺����Ӹ�
	virtual DWORD				GetOccupantValue(void) 
	{
		return 0x2000; 
	}
	// ��ȡռλ���б�Ĭ��Ϊ��ռλλ�б�
	virtual COccupantTileList * GetOccupantTileList(void) const
	{ 
		return NULL; 
	}
	// �����ͷ�
	virtual void				Release(void)
	{ 
		delete this; 
	}
	// ����(�磺���ؽű����ݵ�)
	virtual void				OnUpdate(void)
	{ 

	}
	// item�ڵ�ͼ�ϵ�λ�����
	virtual void				SetTile(POINT &ptTile)
	{
		m_ptTile = ptTile;
	}
	// ȡ��TILE
	virtual POINT				GetTile(void)
	{
		return m_ptTile;
	}
	// ������С��ͼ�ϵ���ɫ	
	virtual void SetFlagColorOnMinmap(DWORD dwColor)
	{

	};
	// ȡ����С��ͼ�ϵ���ɫ
	virtual DWORD GetFlagColorOnMinmap(void)
	{
		return 0;
	}
	CMapItem(void)
	{
		m_ptTile.x = 0;
		m_ptTile.y = 0;
	}
	virtual ~CMapItem()
	{

	}
	// �����Ƿ���С��ͼ����ʾ
	virtual void SetVisibleOnMinmap(BOOL bVisible)
	{
	};
	// ȡ���Ƿ���С��ͼ����ʾ
	virtual BOOL GetVisibleOnMinmap(void)
	{
		return FALSE;
	}

protected:
	POINT	m_ptTile;
};
//------------------------------------------------------------------------
// ռλ���б������, �̳д��༴��֧��ռλ���б�
class COccupant
{
public:

	// ��ȡռλ���б�
	virtual COccupantTileList * GetOccupantTileList(void) const 
	{
		return m_pOccupantTileList; 
	}
	// ȡ��ռλ����Ϣ
	virtual BOOL				OnSerialize(BOOL bWrite,IArchive* pArchive)
	{
		if(bWrite != TRUE)
		{
			if(m_pOccupantTileList)
			{
				return pArchive->Write((char*)m_pOccupantTileList,m_pOccupantTileList->GetMemSize());
			}
			else
			{ // �����޶�ռλ���󡣿�������Ϊ������δ�������
				//TRACE("Error : COccupant::OnSerialize() : m_pOccupantTileList == NULL\r\n");
				return FALSE;
			}
		}
		else
		{ // ����
			COccupantTileList* pList = (COccupantTileList*)pArchive->GetBuffer();
			if(!SetOccupantTileList(pList))
				return FALSE;
			pArchive->Seek(pList->GetMemSize());
			return TRUE;
		}
	}
	// ����ռλ���б�
	// ���pListΪNULL����ʾ�����ǰռλ���б�
	virtual BOOL				SetOccupantTileList(const COccupantTileList* pList)
	{
		if(m_pOccupantTileList)
		{
			delete m_pOccupantTileList;
			m_pOccupantTileList = NULL;
		}
		if(pList == NULL)
		{
			return TRUE;
		}

		if(!pList->IsValid())
		{
			//TRACE("Error : CStaticPicWithOccupant::SetOccupantTileList() : pList is invalid\r\n");
			return FALSE;
		}

		if(NULL == (m_pOccupantTileList = COccupantTileList::Create(pList->GetArraySize())))
		{
			return FALSE;
		}

		int nSize = pList->GetMemSize();
		memcpy(m_pOccupantTileList,pList,nSize);

		return TRUE;
	}

	COccupant(void)
	{
		m_pOccupantTileList = NULL;
	}
	virtual ~COccupant(void)
	{
		SetOccupantTileList(NULL);
	}
protected:
	COccupantTileList *			m_pOccupantTileList;
};
//------------------------------------------------------------------------