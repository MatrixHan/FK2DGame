/**
*	created:		2012-6-21   23:01
*	filename: 		Tile
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "MapItem.h"
//------------------------------------------------------------------------
struct IArchive;
typedef CMultLenArray<DWORD,CMapItem*>		CLayerList;
//------------------------------------------------------------------------
/* ����ر��ϵ�һ��CTile

   ˵����
	1��һ��tile�Ϲ���һ��CLayerList��һ��CMapItem
	2��CLayerList���� : Ϊ���ѭ����������ʱ��ɾ�����⣬ɾ������ʱ����ָ������Ϊ��
*/ 
//------------------------------------------------------------------------
class CTile
{
public:
	CTile()
	{
		m_dwFlag = fInitData;		// Ĭ��Ϊ�յ���Ч��
		m_pLayerList = NULL;
	}
	~CTile()
	{
		Close();
	}
/*
                  0000 0000 0000 0000 0000 0000 0000 0000
              32 _|||| |||| |||| |||| |||| |||| |||| ||||_  1. ���Ƿ���Ч
              31 __||| |||| |||| |||| |||| |||| |||| |||__  2. m_pMapItemָ��Ψһ�Ķ��󣬷����ʾָ��Ϊm_pLayerList
              30 ___|| |||| |||| |||| |||| |||| |||| ||___  3.
              29 ____| |||| |||| |||| |||| |||| |||| |____  4.
              28 ______|||| |||| |||| |||| |||| ||||______  5. �ر��Ƿ��
              27 _______||| |||| |||| |||| |||| |||_______  6. �����ڵر����Ƿ��
              26 ________|| |||| |||| |||| |||| ||________  7. �ر�����Ӹ�
              25 _________| |||| |||| |||| |||| |_________  8.
              24 ___________|||| |||| |||| |||1_____________9. 
									  |||| ||2______________10.
									  |||| |4_______________11.
									  |||| 8________________12.
									  |||1__________________13.
									  ||2___________________14.���嵼�µ����߸�
									  |4____________________15.���嵼�µ����Ӹ�
									  8___________________  16.���嵼�·��и���






*/
	enum 
	{
		maskValid = 0x1,														/*���Ƿ���Ч*/

		/////////////////////// ���߸�ֵ ////////////////////////
		// 0��ʾ�޸񵲣�ֵԽ�󣬸���Խ��
		maskBlock_Surface = 0x0010,												/*�ر��ֵ���ر�Ĺ̶�����*/
		maskBlock_Item =    0x2000,												/*�����˵�����������¼����µر�ĸ�ֵ(��ĳ�����ʩ��)*/
		maskBlock = (maskBlock_Surface | maskBlock_Item),						/*�ر�����干ͬ�����˿�ĸ�ֵ*/

		//////////////////////// ���и�ֵ ///////////////////////
		// 0��ʾ�޸񵲣�1��ʾ�и�
		maskFlyBlock_Surface = 0x0020,											/*�ر��µķ��и�ֵ*/
		maskFlyBlock_Item =    0x8000,											/*���嵼�µķ��и�ֵ,0��ʾ�޸񵲣�1��ʾ�и�*/
		maskFlyBlock = (maskFlyBlock_Surface | maskFlyBlock_Item),				/*��Ѱ��ǰ������Ƿ��ͨ�����ɵر�����干ͬ������*/

		//////////////////////// ���Ӹ�ֵ ///////////////////////
		// 0��ʾ�޸񵲣�1��ʾ�и񵲣�һ��ָ�ر���������塢����ĸ�
		maskDiffuseBlock_Surface = 0x0040,										/*�ر��µ����Ӹ�ֵ*/
		maskDiffuseBlock_Item =    0x4000,										/*���嵼�µ����Ӹ�ֵ,0��ʾ�޸񵲣�1��ʾ�и�*/
		maskDiffuseBlock = (maskDiffuseBlock_Surface | maskDiffuseBlock_Item),	/*��Ѱ��ǰ�������Ƿ��ͨ�����ɵر�����干ͬ������*/

		///////////////// ����������߸񵲡����и�����Ӹ񵲵�ͳһ����////////
		// �ر�����߸񵲡����и񵲡����Ӹ�
		maskOccupant_Surface = (maskBlock_Surface | maskFlyBlock_Surface | maskDiffuseBlock_Surface),
		// ����������߸񵲺ͷ��и�
		maskOccupant_Item = (maskBlock_Item | maskFlyBlock_Item | maskDiffuseBlock_Item),
		// ������͵ر��ħ���񵲺����߸�
		maskOccupant = maskOccupant_Surface | maskOccupant_Item,

		/////////////////////// ����ʱ̬��־λ /////////////////////
		// maskTileType = 0x1FF0000, /*��ʶ�����ͣ���ݵء�ɳ�ص�*/
		maskIsMapItem = 0x2,													/*m_pMapItemָ��Ψһ�Ķ��󣬷����ʾָ��Ϊm_pLayerList*/
		maskIsLayerList =  0xFFFFFFFD,											/*m_pLayerList*/
		fInitData = 0,															/*��ʼֵ����Ч�顢��ͨ��*/

		fMostTileValue = maskValid,												/*�󲿷ֿ��ֵ�������ͬ����˴���ʱ��ֻ��һ���ֽ�:0x04*/
		fMostTileValueFlag = 0x04,												// ����ֽڱ�ʾ����ĵر���ͨ����Ч��

	};
	//------------------------------------------------------------------------
	//����߻�д�ű�����ֵ���Ѹ��ָ�ֵ��1��2��3��4��������Ӧ
	enum		
	{
		masktype_block = 0x1,					// �����赲
		masktype_fly,							// �����赲
		masktype_flood,							// �����赲
		masktype_block_and_fly,					// ���߷����赲
		masktype_block_and_flood,				// ���ߺ������赲
		masktype_fly_and_flood,					// ���������赲
		masktype_block_fly_flood,				// ���߷��������赲
	};
	//------------------------------------------------------------------------
	// �����Ƿ�˿��ͨ��
	// ���ͨ�����ɵر�Ϳ������ж���ͬ������
	DWORD IsBlock()const
	{
		return m_dwFlag & maskBlock;
	}
	DWORD IsNotBlock()const
	{
		return !(m_dwFlag & maskBlock);
	}
	// ���ش˿��Ƿ��з��и�
	// ��ķ��и����ɵر�Ϳ������ж���ͬ����
	DWORD IsFlyBlock()const
	{
		return m_dwFlag & maskFlyBlock;
	}
	// ���ش˿��Ƿ������Ӹ�
	// ������Ӹ����ɵر�Ϳ������ж���ͬ����
	DWORD IsDiffuseBlock()const
	{
		return m_dwFlag & maskDiffuseBlock;
	}
	// �ر��Ƿ��и�
	DWORD IsBlock_Surface()const
	{
		return m_dwFlag & maskBlock_Surface;
	}
	// ���õر��и�
	void Set_Block_Surface(DWORD dwFlag = maskBlock_Surface)
	{
		m_dwFlag |= dwFlag & maskBlock_Surface;
	}
	// ����ر��
	void Clear_Block_Surface()
	{
		m_dwFlag &= ~maskBlock_Surface;
	}
	// �Ƿ�Ϊ�����ﵼ�¸�
	DWORD IsBlock_Item()const
	{
		return m_dwFlag & maskBlock_Item;
	}
	// ���õ����ﵼ�¸�
	void Set_Block_Item(DWORD dwFlag = maskBlock_Item)
	{
		m_dwFlag |= dwFlag & maskOccupant_Item;
	}
	// ����������
	void Clear_Block_Item()
	{
		m_dwFlag &= ~maskBlock_Item;
	}
	// �Ƿ�Ϊ�����ﵼ�·��и�
	DWORD IsFlyBlock_Surface()const
	{
		return m_dwFlag & maskFlyBlock_Surface;
	}
	// ���õ����ﵼ�·��и�
	void Set_FlyBlock_Surface(DWORD dwFlag = maskFlyBlock_Surface)
	{
		m_dwFlag |= dwFlag & maskFlyBlock_Surface;
	}
	// �����������и�
	void Clear_FlyBlock_Surface()
	{
		m_dwFlag &= ~maskFlyBlock_Surface;
	}
	// �Ƿ�Ϊ�����ﵼ�·��и�
	DWORD IsFlyBlock_Item()const
	{
		return m_dwFlag & maskFlyBlock_Item;
	}
	// ���õ����ﵼ�·��и�
	void Set_FlyBlock_Item(DWORD dwFlag = maskFlyBlock_Item)
	{
		m_dwFlag |= dwFlag & maskFlyBlock_Item;
	}
	// �����������и�
	void Clear_FlyBlock_Item()
	{
		m_dwFlag &= ~maskFlyBlock_Item;
	}
	// �Ƿ�Ϊ�����ﵼ�����Ӹ�
	DWORD IsDiffuseBlock_Surface()const
	{
		return m_dwFlag & maskDiffuseBlock_Surface;
	}
	// ���õ����ﵼ�����Ӹ�
	void Set_DiffuseBlock_Surface(DWORD dwFlag = maskDiffuseBlock_Surface)
	{
		m_dwFlag |= dwFlag & maskDiffuseBlock_Surface;
	}
	// ������������Ӹ�
	void Clear_DiffuseBlock_Surface()
	{
		m_dwFlag &= ~maskDiffuseBlock_Surface;
	}
	// �Ƿ�Ϊ�����ﵼ�����Ӹ�
	DWORD IsDiffuseBlock_Item()const
	{
		return m_dwFlag & maskDiffuseBlock_Item;
	}
	// ���õ����ﵼ�����Ӹ�
	void Set_DiffuseBlock_Item(DWORD dwFlag = maskDiffuseBlock_Item)
	{
		m_dwFlag |= dwFlag & maskDiffuseBlock_Item;
	}
	// ������������Ӹ�
	void Clear_DiffuseBlock_Item()
	{
		m_dwFlag &= ~maskDiffuseBlock_Item;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///////////////// occupant��ʾ���߸񵲺ͷ��и�ͳһ���� ////////////////////////////

	// �ر�����ߺͷ��и�ֵͳһ����
	DWORD IsOccupant_Surface()const
	{
		return m_dwFlag & maskOccupant_Surface;
	}
	void Set_Occupant_Surface(DWORD dwFlag)
	{
		m_dwFlag |= dwFlag & maskOccupant_Surface;
	}
	void Clear_Occupant_Surface()
	{
		m_dwFlag &= ~maskOccupant_Surface;
	}
	DWORD IsOccupant_Item()const
	{
		return m_dwFlag & maskOccupant_Item;
	}
	void Set_Occupant_Item(DWORD dwFlag = maskOccupant_Item)
	{
		m_dwFlag |= dwFlag & maskOccupant_Item;
	}
	void Clear_Occupant_Item()
	{
		m_dwFlag &= ~maskOccupant_Item;
	}
	// ͬ�ϣ�ռ����б��б������һ��BYTE���飬һ��BYTE��Ӧ
	void Set_Occupant_Item_Byte(BYTE cbFlag)
	{
		m_dwFlag |= (((DWORD)cbFlag) << 8) & maskOccupant_Item;
	}
	void Clear_Occupant_Item_Byte(BYTE cbFlag = (BYTE)(maskOccupant >> 8))
	{
		m_dwFlag &= ~(((DWORD)cbFlag) << 8);
	}
	
	// ���ݵ�ǰ�������ж����������¸����
	// �Կ�Ķ�����ɾ�Ժ���ô˲���
	// ˵����ֻ�ܸ��ݴ�tile�ϵĵ�����д����޷������Ǵ˿�Ķ�ռλ��ռλ��Ϣ���µ��˿�
	void OnUpdateItems()
	{
		//Clear_Occupant_Item();
		int nCount = GetLayerCount();
		for(int i = 0; i < nCount; i++)
			Set_Occupant_Item(GetLayer(i)->GetOccupantValue());
	}
	// ɾ������ʱ��������˶����ռλ��Ϣ��Ȼ�󽫴��������������ռλ��Ϣ���µ�tile��
	void RemoveItemOccupant(DWORD dwFlag)
	{
		m_dwFlag &= ~dwFlag;
		int nCount = GetLayerCount();
		for(int i = 0; i < nCount; i++)
			Set_Occupant_Item(GetLayer(i)->GetOccupantValue());
	}
	int GetLayerCount() const
	{
		if(m_pMapItem == NULL)
			return 0;
		else if(m_dwFlag & maskIsMapItem)
			return 1;
		else 
			return m_pLayerList->GetCount();
	}
	// ��ǰֻ��һ�������m_pMapItemָ��һ��CMapItem
	BOOL IsMapItemValid()
	{
		return m_dwFlag & maskIsMapItem;
	}
	CMapItem* GetLayer(int nIndex)
	{
		if(m_dwFlag & maskIsMapItem) // ��ʱֻ��һ����㣬����nIndexֵ
			return m_pMapItem;
		else
		{
			if (nIndex < m_pLayerList->GetCount())
				return (*m_pLayerList)[nIndex];
		}
		return NULL;
	}
	void SetOneLayer(CMapItem* pItem)
	{
		m_pMapItem = pItem;
		m_dwFlag |= maskIsMapItem;
	}
	void SetLayerList(CLayerList* pLL)
	{
		m_pLayerList = pLL;
		m_dwFlag &= maskIsLayerList;
	}
	void SetNoLayer()
	{
		m_pMapItem = NULL;
		m_dwFlag &= maskIsLayerList;
	}

	CMapItem* GetMapItem()
	{
		return m_pMapItem;
	}

	CLayerList* GetLayerList()const
	{
		return m_pLayerList;
	}

	// ֱ�ӻ�ȡָ���ַ���Ա㵱����䳤ʱ�ı�ָ���ֵ
	CLayerList** GetLayerListPointer()
	{
		return &m_pLayerList;
	}

	// ���ô˿�Ϊ������Ч������
	void SetInValid()
	{
		Close();
	}
	// �Ƿ�Ϊ�е�ͼ��Χ֮�ڵ���Ч�飬���m_dwFlagΪ0����ʾ�˿�Ϊ����ͼ����Ч�飬����ʹ��ͼ���ö�ά���ݱ�ʾ
	BOOL IsValid()const
	{
		return (BOOL) (m_dwFlag & maskValid);
	}

	// �����һ������ĵر����ͣ����޵������ͨ���ر���ѹ���洢
	BOOL IsMostTileValue() const
	{
		return m_dwFlag == fMostTileValue && GetLayerCount() == 0;
	}
	void SetMostValue()
	{
		m_dwFlag = fMostTileValue;
	}

	DWORD GetFlag() const
	{
		return m_dwFlag;
	}
	void SetFlag(DWORD dwFlag)
	{
		m_dwFlag = dwFlag;
	}

	// ���pItem�Ƿ��ڴ�tile��
	BOOL FindItem(CMapItem* pItem)
	{
		int nCount = GetLayerCount();
		for(int i = 0; i < nCount; i++)
		{
			if(GetLayer(i) == pItem)
				return TRUE;
		}
		return FALSE;
	}

	void Create()
	{
		m_dwFlag = maskValid | fInitData;	// ��Ч�顢��ͨ�����޵��������ͨ��
		m_pLayerList = NULL;
	}
	void Close()
	{
		m_dwFlag = fInitData;
	}
	
	// bClearOccupantWhenRead ΪTRUEʱ���ڷ������˶��ر���Ϣʱ����Ԥ������ر��ϵ�maskOccupant_Item
	BOOL OnSerialize(BOOL bWrite,IArchive* pArchive,BOOL bClearOccupantWhenRead)
	{
		/* 
			��ʽ��
				wLayerNum + Layer0 + Layer1 + .......
		*/
		//enum{tileBlockFlag/*һ��tile����ʼ��־*/ = 0x613};
		struct MAP_ARCHIVE_TILE_HEADER
		{
			//DWORD dwTileBlockFlag;
			DWORD dwFlag;
		};
		MAP_ARCHIVE_TILE_HEADER mt;

		if(bWrite)
		{ // ��������CTile���Լ�������ϵ�����CMapItem
			MAP_ARCHIVE_TILE_HEADER mt;
			mt.dwFlag = m_dwFlag;
			// дCTile����
			pArchive->Write((char*)&mt,sizeof(mt));
		}
		else
		{ // ��ȡ�����CTile���Լ�������ϵ�����CMapItem
			pArchive->Read((char*)&mt,sizeof(mt));
			m_dwFlag = mt.dwFlag;
			// ��maskOccupant_Item��������ﵼ�µ�ռλ���ô��ǣ��������ռλ��Ϣ���ϴα����ͼ�Ժ�����Ѿ������ı�
			// ���ص�ͼʱ��ӵ��������ᵼ��ռλ��Ϣ�õ�����
			if(bClearOccupantWhenRead)
				m_dwFlag |= mt.dwFlag & (~CTile::maskOccupant_Item);
			
		}
		return TRUE;
	}

protected:
	DWORD		m_dwFlag;
	union
	{
		CLayerList*	m_pLayerList;
		CMapItem*	m_pMapItem;			// ֻ��һ������ʱ����m_pMapItem�������ж������϶���ʱ����m_pLayerList����
										// ��Ϊ�󲿷ֿ���ֻ��һ������
	};
};
//------------------------------------------------------------------------