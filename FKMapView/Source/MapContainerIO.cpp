/**
*	created:		2012-7-1   6:28
*	filename: 		MapContainerIO
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/MapContainer.h"
#include "../../FKGame/SceneObj/IItemClassFactory.h"
//------------------------------------------------------------------------
// �������Ҫ�����ͼ�϶����ռλ����Ϣ���ɴ�����ĺ�
//#define _NOT_LOAD_ITEM_OCCUPANT
//#define _NOT_SAVE_ITEM_OCCUPANT
//------------------------------------------------------------------------
void CMapContainer::ViewRectToPreReadRect(RECT& rcView, RECT& rcPreRead, int nMapWidth, int nMapHeight)
{
	rcPreRead.left = (rcView.right + rcView.left)/2 - (TILES_PREREAD_WIDTH)/2;
	rcPreRead.top = (rcView.bottom + rcView.top)/2 - (TILES_PREREAD_HEIGHT)/2;
	if (rcPreRead.left < 0)
		rcPreRead.left = 0;
	if (rcPreRead.top < 0)
		rcPreRead.top = 0;
	if (rcPreRead.left > nMapWidth - TILES_PREREAD_WIDTH)
		rcPreRead.left = nMapWidth - TILES_PREREAD_WIDTH;
	if (rcPreRead.top > nMapHeight - TILES_PREREAD_HEIGHT)
		rcPreRead.top = nMapHeight - TILES_PREREAD_HEIGHT;
	int nGridX = rcPreRead.left >> SHIFT_WIDTH;
	int nGridY = rcPreRead.top >> SHIFT_HEIGHT;
	rcPreRead.left = nGridX << SHIFT_WIDTH;
	rcPreRead.top = nGridY << SHIFT_HEIGHT;
	rcPreRead.right = rcPreRead.left + TILES_PREREAD_WIDTH;
	rcPreRead.bottom = rcPreRead.top + TILES_PREREAD_HEIGHT;
}
//------------------------------------------------------------------------
// ��ֻʵ����client�˵Ĵ洢���Ժ��Ƿ�������ʵ��
BOOL CMapContainer::OnSaveMap(IArchive* pArchive, LPCSTR szGroundJpgDir, ITrace* pTrace)
{
	g_DefaultTrace.Create(pTrace);	
	return OnSaveMap112(pArchive, szGroundJpgDir, GRID_WIDTH, GRID_HEIGHT, pTrace);

	if(!IsValid())
	{
		TraceLn("Error : CMapContainer::OnSerialize() : not Create() yet");
		return FALSE;
	}
	TraceLn(">>>>>>>>>>>>>>>>>>>> Write map data <<<<<<<<<<<<<<<<");

	MAP_ARCHIVE_HEADER* pMH = (MAP_ARCHIVE_HEADER*)pArchive->GetBuffer();
	// 1��д��������
	MAP_ARCHIVE_HEADER mh;
	mh.Init(m_nMapWidth,m_nMapHeight,szGroundJpgDir);
	pArchive->Write((char*)&mh,sizeof(MAP_ARCHIVE_HEADER));
	

	// 2��д�����ϵ�����
	for(int row = 0; row < m_nMatrixWidth; row++)
	{
		CTile* pTile = &m_pTiles[row * m_nMatrixWidth];
		int col = 0;
		for(;col < m_nMatrixWidth && !pTile->IsValid(); col++,pTile++);	// �ų�ǰ����Ч���

		// �����ʽ��CTile + wItemCount + CMapItem0 + CMapItem1 + .....
		// CMapItem��ʽ��dwClassID + wObjectLen + ObjectData
		MAP_ARHCIVE_LINE_HEADER* pMALH = (MAP_ARHCIVE_LINE_HEADER*)pArchive->GetBuffer();
		pMALH->nFromX = col;
		
		pArchive->Seek(sizeof(MAP_ARHCIVE_LINE_HEADER));

		int nTileCount = 0;
		for(;col < m_nMatrixWidth && pTile->IsValid();nTileCount++,col++,pTile++)
		{
			// ����CTile
			if(pTile->IsMostTileValue())	// �����һ������ĵر����ͣ����޵������ͨ���ر���ѹ���洢
			{
				BYTE cbMostData = CTile::fMostTileValueFlag;
				pArchive->Write((char*)&cbMostData,1);
				continue;
			}

			pTile->OnSerialize(TRUE,pArchive,0);

			// ����wItemCount
			WORD wItemCount = pTile->GetLayerCount();
			pArchive->Write((char*)&wItemCount,sizeof(wItemCount));

			// ����CTile�ϵ�����CMapItem����
			for(WORD i = 0; i < wItemCount; i++)
			{
				CMapItem* pItem = pTile->GetLayer(i);

				// ����dwClassID
				DWORD dwClassID = pItem->GetClassID();
				pArchive->Write((char*)&dwClassID,sizeof(dwClassID));

				// ���ƫ���Ƕ��󳤶�dwObjectLen��λ�ã�����洢�����Ժ��ټ�����󳤶�
				WORD* pObjectLen = (WORD*)pArchive->GetBuffer();
				pArchive->Seek(sizeof(WORD));
				int nBeginOff = pArchive->GetBufferOffset();

				// ��������ֳ�
				if(!pItem->OnSerialize(TRUE,pArchive))
					TraceLn("Error : CMapContainer::OnSerialize() : OnSerialize() error,dwClassID = %d,@Tile(%d,%d)",dwClassID,col,row);

				// ������󳤶�dwObjectLen
				*pObjectLen = (WORD)(pArchive->GetBufferOffset() - nBeginOff);
			}
		}
		pMALH->nTileCount = nTileCount;
	}
	pMH->dwSize = (DWORD)((char*)pArchive->GetBuffer() - (char*)pMH);
	TraceLn(">>>>>>>>>>>>>>>>>>>> End map data <<<<<<<<<<<<<<<<");
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMapContainer::OnLoadMap(IArchive* pArchive,IItemClassFactory* pItemCF, 
							  LPRECT lprcViewport, ITrace* pTrace)
{
	g_DefaultTrace.Create(pTrace);
	
	// ��ȡ��ͼ�ļ�ͷ
	MAP_ARCHIVE_HEADER _mah;
	pArchive->Read((char*)&_mah, sizeof(MAP_ARCHIVE_HEADER));
	if (!_mah.IsValid())
	{
		return FALSE;
	}
	lstrcpy(m_szGround, _mah.szGroundJpgDir);
	pArchive->SeekTo(0);
	if (_mah.dwVersion == 0x00010102)
		return OnLoadMap112(pArchive, pItemCF, lprcViewport, pTrace);
	// �����ȡ1.11��ĸ�ʽ
	if (IsUseDynamicMap())
	{
		TraceLn( "��ͼ�ļ��汾̫�ͣ���֧�ֶ�̬��ͼ");
		return FALSE;
	}
	TraceLn(">>>>>>>>>>>>>>>>>>>> read map data <<<<<<<<<<<<<<<<");

	MAP_ARCHIVE_HEADER mh;
	pArchive->Read((char*)&mh,sizeof(MAP_ARCHIVE_HEADER));
	if(!mh.IsValid())
	{
		TraceLn("Error : CMapContainer::OnSerialize() map file is not valid,Version = %06X",mh.dwVersion);
		return FALSE;
	}

	// ����ͼ�Ŀ�߽���64(32)���봦��
	mh.nMapWidth &= ~63;
	mh.nMapHeight &= ~31;

	RECT rcTilesPreRead;
	if (IsUseDynamicMap())
		ViewRectToPreReadRect(*lprcViewport, rcTilesPreRead, mh.nMapWidth, mh.nMapHeight);
	else
		SetRect(&rcTilesPreRead, 0, 0, mh.nMapWidth, mh.nMapHeight);
	if (!Create(mh.nMapWidth, mh.nMapHeight, rcTilesPreRead, pTrace))
		return FALSE;
	for(int row = 0; row < m_nMatrixWidth; row++)
	{
		MAP_ARHCIVE_LINE_HEADER malh;
		if(!pArchive->Read(&malh,sizeof(malh)))
			break;
		if(malh.nFromX < 0 || malh.nFromX >= m_nMatrixWidth || malh.nTileCount < 0 || malh.nTileCount >= m_nMatrixWidth)
			break;
		CTile* pTile = &m_pTiles[row * m_nMatrixWidth + malh.nFromX];
		int col = malh.nFromX;
		for(int t = 0 ;t < malh.nTileCount ; t++,col++,pTile++)
		{
			// ��ȡCTile
			BYTE cbMostData = 0;
			pArchive->Read((char*)&cbMostData,1);
			if(cbMostData == CTile::fMostTileValueFlag)	// �����һ������ĵر����ͣ����޵������ͨ���ر���ѹ���洢
			{
				pTile->SetMostValue();
				continue;
			}
			pArchive->Seek(-1);

			if(!pTile->OnSerialize(FALSE,pArchive,TRUE))
				break;
			
			// ��ȡwItemCount
			WORD wItemCount = 0;
			pArchive->Read(&wItemCount,sizeof(wItemCount));
			// ��ȡCTile�ϵ�����CMapItem����
			for(WORD wItem = 0; wItem < wItemCount;wItem++)
			{
				// ��ȡdwClassID
				DWORD dwClassID = 0;
				if(!pArchive->Read((char*)&dwClassID,sizeof(dwClassID)))
					break;
				WORD wObjectLen = 0;
				if(!pArchive->Read((char*)&wObjectLen,sizeof(wObjectLen)))
					break;
				
				int nBeginOff = pArchive->GetBufferOffset();	// ��ס��ȡ����ǰ��ƫ��
				// ��������
				CMapItem* pItem = pItemCF->CreateItem(dwClassID,pArchive);
				POINT pt = {malh.nFromX + t,row};
				if(pItem == NULL)
				{
					TraceLn("Error : CMapContainer::OnSerialize() : dwClassID = %d,@Tile(%d,%d)",dwClassID,pt.x,pt.y);
				}
				else
				{
					if (!AddItem(pt, pItem))
						pItem->Release();
					else
						m_MapItemList.push_back(pItem);
					// END
					if (pArchive->GetBufferOffset() != nBeginOff + wObjectLen)
					{
						TraceLn("Error : CMapContainer::OnSerialize() : current archive offset = %d,but %d + %d wanted,dwClassID = %d,@Tile(%d,%d)",
							pArchive->GetBufferOffset(),nBeginOff,wObjectLen,dwClassID,pt.x,pt.y);
						pArchive->SeekTo(nBeginOff + wObjectLen);	// ǿ�ƽ�ָ���ƶ����������ݳ���֮���λ�ã���ֹ���������ٶ�����
					}
				}
					
			}
		}
	}
	TraceLn(">>>>>>>>>>>>>>>>>>>> End map data <<<<<<<<<<<<<<<<");
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMapContainer::OnLoadMap112(IArchive* pArchive,IItemClassFactory* pItemCF,LPRECT lprcViewport, ITrace* pTrace)
{
	m_pArchive = pArchive;
	m_pItemCF = pItemCF;
	TraceLn(">>>>>>>>>>>>>>>>>>>> ��ʼ��ȡ��ͼ���� <<<<<<<<<<<<<<<<");
#ifdef _DEBUG
	DWORD dwStart = GetTickCount();
#endif
	// ��ȡ��ͼ�ļ�ͷ
	MAP_ARCHIVE_HEADER mh;
	int nMapOffset = pArchive->GetBufferOffset();
	pArchive->Read((char*)&mh, sizeof(MAP_ARCHIVE_HEADER));
	if (!mh.IsValid())
	{
		return FALSE;
	}
	
	// ����ͼ�Ŀ�߽���64(32)���봦��
	mh.nMapWidth &= ~63;
	mh.nMapHeight &= ~31;

	// �ӿ�У��
	RECT rcRect = {0, 0, mh.nMapWidth, mh.nMapHeight};
	if (!IntersectRect(&rcRect, &rcRect, lprcViewport))
	{
		::MessageBox(NULL, "CMapView::OnLoadMap112(), ������������ڵ�ͼ��Χ֮��", "ERROR", MB_OK);
		return FALSE;
	}

	// �ü�����
	if (lprcViewport->left < 0)
	{
		lprcViewport->left = 0;
		lprcViewport->right = 800;
	}
	if (lprcViewport->right > mh.nMapWidth)
	{
		lprcViewport->right = mh.nMapWidth;
		lprcViewport->left = mh.nMapWidth - 800;
	}
	if (lprcViewport->top < 0)
	{
		lprcViewport->top = 0;
		lprcViewport->bottom = 600;
	}
	if (lprcViewport->bottom > mh.nMapHeight)
	{
		lprcViewport->bottom = mh.nMapHeight;
		lprcViewport->top = mh.nMapHeight - 600;
	}
	
	// ������ͼ
	RECT rcTilesPreRead;
	if (IsUseDynamicMap())
		ViewRectToPreReadRect(*lprcViewport, rcTilesPreRead, mh.nMapWidth, mh.nMapHeight);
	else
		SetRect(&rcTilesPreRead, 0, 0, mh.nMapWidth, mh.nMapHeight);
	if (!Create(mh.nMapWidth, mh.nMapHeight, rcTilesPreRead, pTrace))
		return FALSE;
	
	// ����������
	int nGridRow = (m_nMapHeight - 1)/mh.nGridHeight + 1;
	int nGridCol = (m_nMapWidth - 1)/mh.nGridWidth + 1;
	TraceLn("����ͼ���� = %d ���� = %d", nGridRow, nGridCol );
	
	// 2����¼ӳ����ַ,ӳ����м�¼ÿ�������������ļ�ͷ��ƫ��
	m_pMapTable = (DWORD*)pArchive->GetBuffer();
	
	// 3����ȡÿ����������Ϣ
	if (!IsUseDynamicMap())
	{
		int nTileRow = mh.nGridHeight / 32;
		int nTileCol = mh.nGridWidth / (64/2);
		POINT ptTileLeftTop;
		POINT ptLeftTop;
		for (int row=0; row<nGridRow; row++)
		{
			for (int col=0; col<nGridCol; col++)
			{
				// ��λÿ��������λ��
				pArchive->SeekTo(m_pMapTable[row*nGridCol+col] + nMapOffset);
				
				ptLeftTop.x = col*mh.nGridWidth;
				ptLeftTop.y = row*mh.nGridHeight;
				
				m_MapCo.PixelToTile(ptLeftTop, ptTileLeftTop);			
				if (!_LoadBlock(pArchive, ptTileLeftTop, nTileRow, nTileCol, pItemCF))
					return FALSE;
			}
		}	
	}
#ifdef _DEBUG
	TraceLn("����ͼ��������ʱ�� = %d", GetTickCount()-dwStart);
#endif	
	TraceLn(">>>>>>>>>>>>>>>>>>>> ��ȡ��ͼ������� <<<<<<<<<<<<<<<<");
	return TRUE;
}
//------------------------------------------------------------------------
// �����ڱ���ͼ�ϼ�������һ��������ͼͬ����С�ĵ�ͼ�ļ�
BOOL CMapContainer::OnAddMap112(IArchive* pArchive, IItemClassFactory* pItemCF, LPRECT lprcViewport, ITrace* pTrace)
{
	TraceLn(">>>>>>>>>>>>>>>>>>>> read map data <<<<<<<<<<<<<<<<");
	// ��ȡ��ͼ�ļ�ͷ
	MAP_ARCHIVE_HEADER mh;
	int nMapOffset = pArchive->GetBufferOffset();
	pArchive->Read((char*)&mh, sizeof(MAP_ARCHIVE_HEADER));
	if (!mh.IsValid())
	{
		TraceLn("Error : CMapContainer::OnSerialize() map file is not valid,Version = %06X",mh.dwVersion);
		return FALSE;
	}
	if (mh.nMapWidth != m_nMapWidth || mh.nMapHeight != m_nMapHeight)
	{
		TraceLn("����ĵ�ͼ�ĵ��뵱ǰ�ĵ�ͼ�ĵ���С��ƥ��!");
		return FALSE;
	}
	
	// ����������
	int nGridRow = (m_nMapHeight - 1)/mh.nGridHeight + 1;
	int nGridCol = (m_nMapWidth - 1)/mh.nGridWidth + 1;
	
	// 2����¼ӳ����ַ,ӳ����м�¼ÿ�������������ļ�ͷ��ƫ��
	DWORD* pMapTable = (DWORD*)pArchive->GetBuffer();
	
	// 3����ȡÿ����������Ϣ
	if (!IsUseDynamicMap())
	{
		int nTileRow = mh.nGridHeight / 32;
		int nTileCol = mh.nGridWidth / (64/2);
		POINT ptTileLeftTop;
		POINT ptLeftTop;
		for (int row=0; row<nGridRow; row++)
		{
			for (int col=0; col<nGridCol; col++)
			{
				// ��λÿ��������λ��
				pArchive->SeekTo(pMapTable[row*nGridCol+col] + nMapOffset);
				
				ptLeftTop.x = col*mh.nGridWidth;
				ptLeftTop.y = row*mh.nGridHeight;
				
				m_MapCo.PixelToTile(ptLeftTop, ptTileLeftTop);
				if (!_LoadBlock(pArchive, ptTileLeftTop, nTileRow, nTileCol, pItemCF))
					return FALSE;
			}
		}	
	}
	
	TraceLn(">>>>>>>>>>>>>>>>>>>> End map data <<<<<<<<<<<<<<<<");
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMapContainer::_LoadBlock(IArchive* pArchive, POINT ptTileLeftTop, 
							   int nTileRow, int nTileCol, IItemClassFactory* pItemCF)
{
	POINT ptCurTile = ptTileLeftTop;
	BOOL bIsEmptyTile = FALSE;
	int nCount = 0;
	CTile* pTile = NULL;

	for (int row=0; row<nTileRow; row++) // һ����֮������
	{
		ptCurTile.x = ptTileLeftTop.x + row;
		ptCurTile.y = ptTileLeftTop.y - row;
		for (int col=0; col<nTileCol; col++) // ����
		{
			if (nCount == 0) // ��ȡ����
			{
				BYTE btyData;
				pArchive->Read(&btyData, sizeof(btyData));
				if (btyData & 0x80) // �����ķǿ�TILE
				{
					bIsEmptyTile = FALSE;
					nCount = btyData - 128 + 1;
				}
				else // �����Ŀ�TILE
				{
					bIsEmptyTile = TRUE;
					nCount = btyData + 1;
				}
			}

			pTile = GetTilePtr(ptCurTile);
			if (pTile)
			{
				if (!bIsEmptyTile)
					_LoadTileInfo(pArchive, pTile, ptCurTile, pItemCF);
				nCount --;
			}
			
			ptCurTile.x += !(col&1); // odd
			ptCurTile.y += col&1;
		}
	}

	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMapContainer::_LoadTileInfo(IArchive* pArchive, CTile* pTile, 
								  POINT& ptTile, IItemClassFactory* pItemCF)
{
	// ��ȡ Flags (1/2 bytes)
#ifdef _NOT_LOAD_ITEM_OCCUPANT
	BYTE nFlags = 0;
#else
	WORD nFlags = 0;
#endif
	pArchive->Read((char*)&nFlags, sizeof(nFlags));
	DWORD dwFlags = (DWORD)nFlags;
	dwFlags |= pTile->GetFlag();
	pTile->SetFlag(dwFlags);
	
	// ��ȡItemCount (1 byte)
	BYTE nItemCount = 0;
	pArchive->Read((char*)&nItemCount, sizeof(nItemCount));
	
	// ����CTile�ϵ�����CMapItem����
	for (int i=0; i<nItemCount; i++)
	{
		// ��ȡdwClassID (1 bytes)
		BYTE nClassID = 0;
		if (!pArchive->Read((char*)&nClassID, sizeof(nClassID)))
			break;

		int nBeginOff = pArchive->GetBufferOffset();	// ��ס��ȡ����ǰ��ƫ��

		if (pItemCF == NULL) // for server
		{
			pArchive->Seek(sizeof(DWORD)); // ֻ��¼��һ��ID
		}
		else
		{
			// ��������
			CMapItem* pItem = pItemCF->CreateItem((DWORD)nClassID, pArchive);
			if (pItem == NULL)
			{
				if ((DWORD)pArchive->GetBufferOffset() != (nBeginOff + sizeof(DWORD))) // ֻ��¼��һ��ID
				{
					pArchive->SeekTo(nBeginOff + sizeof(DWORD));	// ǿ�ƽ�ָ���ƶ����������ݳ���֮���λ�ã���ֹ���������ٶ�����
				}
				continue;
			}
			else
			{
#ifdef _DEBUG
				if ((DWORD)pArchive->GetBufferOffset() != (nBeginOff + sizeof(DWORD))) // ֻ��¼��һ��ID
				{
					TraceLn("Error: CMapContainer::_LoadTileInfo, CreateItem() read data size error, dwClassID=%d,@Tile(%d,%d)",
						nClassID,ptTile.x,ptTile.y);
					pArchive->SeekTo(nBeginOff + sizeof(DWORD));	// ǿ�ƽ�ָ���ƶ����������ݳ���֮���λ�ã���ֹ���������ٶ�����
				}
#endif
				if (!AddItem(ptTile, pItem))
					pItem->Release();
				else
					m_MapItemList.push_back(pItem);
			}
		}
	}
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMapContainer::OnSaveMap112(IArchive* pArchive,LPCSTR szGroundJpgDir,
								int nGridWidth, int nGridHeight, ITrace* pTrace)
{
	g_DefaultTrace.Create(pTrace);
	if ((nGridWidth&63) || (nGridHeight&31))
	{
		TraceLn("����ߴ������64,32��������");
		return FALSE;
	}

	if(!IsValid())
	{
		TraceLn("Error : CMapContainer::OnSerialize() : not Create() yet");
		return FALSE;
	}

	// ����������
	int nGridRow = PixelCood2GridCood(m_nMapHeight, nGridHeight);
	int nGridCol = PixelCood2GridCood(m_nMapWidth, nGridWidth);

	
	MAP_ARCHIVE_HEADER* pMH = (MAP_ARCHIVE_HEADER*)pArchive->GetBuffer();
	int nMapOffset = pArchive->GetBufferOffset();
	// 1�������ͼ�ļ�ͷ
	MAP_ARCHIVE_HEADER mh;
	mh.Init(m_nMapWidth,m_nMapHeight,szGroundJpgDir);
	mh.nGridWidth = nGridWidth;
	mh.nGridHeight = nGridHeight;
	pArchive->Write((char*)&mh,sizeof(MAP_ARCHIVE_HEADER));

	// 2��Ԥ��ӳ���,ӳ����м�¼ÿ�������������ļ�ͷ��ƫ��
	int nMapTableItemCount = nGridRow*nGridCol;
	DWORD* pMapTablePos = (DWORD*)pArchive->GetBuffer();
	pArchive->Seek(nMapTableItemCount*sizeof(DWORD));

	// 3������ÿ����������Ϣ
	int nTileRow = nGridHeight / 32;
	int nTileCol = nGridWidth / (64/2);
	POINT ptTileLeftTop;
	POINT ptLeftTop;
	for (int row=0; row<nGridRow; row++)
	{
		for (int col=0; col<nGridCol; col++)
		{
			// ����ӳ�������
			pMapTablePos[row*nGridCol+col] = pArchive->GetBufferOffset() - nMapOffset;

			ptLeftTop.x = col*nGridWidth;
			ptLeftTop.y = row*nGridHeight;

			m_MapCo.PixelToTile(ptLeftTop, ptTileLeftTop);
			if (!_SaveBlock(pArchive, ptTileLeftTop, nTileRow, nTileCol))
				return FALSE;
		}
	}

	pMH->dwSize = (DWORD)((char*)pArchive->GetBuffer() - (char*)pMH);
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMapContainer::_SaveBlock(IArchive* pArchive, POINT ptTileLeftTop, int nTileRow, int nTileCol)
{
	POINT ptCurTile = ptTileLeftTop;
	BOOL bIsEmptyTile = FALSE;
	int nCount = 0;
	CTile* pTile=NULL;
	POINT ptTileList[130];
	for (int row=0; row<nTileRow; row++) // һ����֮������
	{
		ptCurTile.x = ptTileLeftTop.x + row;
		ptCurTile.y = ptTileLeftTop.y - row;
		for (int col=0; col<nTileCol; col++) // ����
		{
			pTile = GetTilePtr(ptCurTile);
			if (pTile)
			{
				if (pTile->IsMostTileValue()
#ifdef _NOT_SAVE_ITEM_OCCUPANT
					|| (pTile->IsOccupant_Item()&&
					!pTile->IsOccupant_Surface()&&
					pTile->GetLayerCount()==0)
#endif
					) // ��TILE
				{
					if (bIsEmptyTile) // �ظ�
					{
						nCount++;
						if (nCount >= 128)
						{
							_SaveMultiEmptyTileInfo(pArchive, nCount);
							nCount = 0;
						}
					}
					else
					{
						// ��¼�ǿ�TILE��Ϣ
						_SaveMultiTileInfo(pArchive, ptTileList, nCount);
						nCount = 1;
					}
					bIsEmptyTile = TRUE;
				}
				else // �ǿ�TILE
				{
					if (bIsEmptyTile)
					{
						_SaveMultiEmptyTileInfo(pArchive, nCount);
						nCount = 0;
						ptTileList[nCount++] = ptCurTile;
					}
					else
					{
						// Write non-empty tile info
						ptTileList[nCount] = ptCurTile;
						nCount ++;
						if (nCount >= 128)
						{
							_SaveMultiTileInfo(pArchive, ptTileList, nCount);
							nCount = 0;
						}
					}
					bIsEmptyTile = FALSE;
				}
			}
			
			ptCurTile.x += !(col&1); // odd
			ptCurTile.y += col&1;
		}
	}
	
	// ���һ�ε�����
	if (bIsEmptyTile)
		_SaveMultiEmptyTileInfo(pArchive, nCount);
	else
		_SaveMultiTileInfo(pArchive, ptTileList, nCount);
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMapContainer::_SaveMultiEmptyTileInfo(IArchive* pArchive, int nCount)
{
	if (nCount == 0)
		return TRUE;
	if (nCount > 128)
	{
		TraceLn("CMapContainer::_SaveMultiEmptyTileInfo, repeat count more than 128!");
		return FALSE;
	}
	// ��д��������TILE�ĸ���(ƫ����-1,Ϊ����0~127���1~128)
	BYTE bytCount = nCount-1;
	pArchive->Write((char*)&bytCount, sizeof(bytCount));
	return TRUE;
}
//------------------------------------------------------------------------
BOOL CMapContainer::_SaveMultiTileInfo(IArchive* pArchive, POINT* ptTileList, int nCount)
{
	if (nCount == 0)
		return TRUE;
	if (nCount > 128)
	{
		TraceLn("CMapContainer::_SaveMultiTileInfo, repeat count more than 128!");
		return FALSE;
	}

	// ��д�������ǿ�TILE�ĸ���(ƫ����127,Ϊ����128~255���1~128,���λ�����Ƿǿ�TILE)
	BYTE bytCount = nCount-1 + 128;
	pArchive->Write((char*)&bytCount,sizeof(bytCount));

	for (int i=0; i<nCount; i++)
	{
		CTile* pTile = GetTilePtr(ptTileList[i]);
		if (pTile)
			if (!_SaveTileInfo(pArchive, pTile))
				return FALSE;
	}
	return TRUE;
}
//------------------------------------------------------------------------
// �����ʽ��CTile(1/2bytes) + nItemCount(1byte) + CMapItem0(5bytes) + CMapItem1(5bytes) + ...
// CMapItem��ʽ(5bytes)��nClassID(1byte) + ObjectID(4bytes)
BOOL CMapContainer::_SaveTileInfo(IArchive* pArchive, CTile* pTile)
{
	// ���� Flags (1/2 bytes)
	DWORD dwFlags = pTile->GetFlag();
#ifdef _NOT_SAVE_ITEM_OCCUPANT
	dwFlags &= ~CTile::maskOccupant_Item; // ȥ��ռλ����item��ֵ
	BYTE nFlags = (BYTE)dwFlags;
#else
	WORD nFlags = (WORD)dwFlags;
#endif
	pArchive->Write((char*)&nFlags, sizeof(nFlags));
	
	// ����ItemCount (1 byte)
	BYTE nItemCount = pTile->GetLayerCount();
#ifdef _DEBUG
	BYTE* pCount = (BYTE*)pArchive->GetBuffer();
#endif
	pArchive->Write((char*)&nItemCount, sizeof(nItemCount));
	
	// ����CTile�ϵ�����CMapItem����
	for (int i=0; i<nItemCount; i++)
	{
		CMapItem* pItem = pTile->GetLayer(i);
		
		// ����dwClassID (1 bytes)
		BYTE nClassID = (BYTE)pItem->GetClassID();
#ifdef _DEBUG
		switch (nClassID) 
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 7:
		case 8:
		case 9:
		case 13:
		case 14:
		case 15:
		case 16:
		case 27:
		case 29:
			break;
		default: // �������������浽��ͼ��
			{
				(*pCount) --;
				continue;
			}	
			break;
		}
#endif
		pArchive->Write((char*)&nClassID, sizeof(nClassID));
		
		int nBeginOffset = pArchive->GetBufferOffset();

		// ��������ֳ�(ֻ��¼ID����Ϊ�����ռλ����Ϣ�������ⲿҪ�޸�) (4 bytes)
		if (!pItem->OnSerialize(TRUE, pArchive))
		{
			TraceLn("Error : CMapContainer::_SaveTileInfo, OnSerialize() error,dwClassID = %d",nClassID);
			return FALSE;
		}
	}
	return TRUE;
}
//------------------------------------------------------------------------
// ��������ȡ�ر���Ϣ
BOOL CMapContainer::OnReadServerMap(IArchive *pArchive, ITrace *pTrace)
{
	return OnReadServerMap112(pArchive, pTrace);
	g_DefaultTrace.Create(pTrace);
	TraceLn(">>>>>>>>>>>>>>>>>>>> read map data <<<<<<<<<<<<<<<<");
	
	MAP_ARCHIVE_HEADER mh;
	pArchive->Read((char*)&mh,sizeof(MAP_ARCHIVE_HEADER));
	if(!mh.IsValid())
	{
		TraceLn("Error : CMapContainer::OnReadServerMap() map file is not valid,Version = %06X",mh.dwVersion);
		return FALSE;
	}
	
	RECT rcTilesPreRead = {0, 0, mh.nMapWidth, mh.nMapHeight};
	if (!Create(mh.nMapWidth, mh.nMapHeight, rcTilesPreRead, NULL))
		return FALSE;
	int row = 0;
	for(; row < m_nMatrixWidth; row++)
	{
		MAP_ARHCIVE_LINE_HEADER malh;
		if(!pArchive->Read(&malh,sizeof(malh)))
			break;
		if(malh.nFromX < 0 || malh.nFromX >= m_nMatrixWidth || malh.nTileCount < 0 || malh.nTileCount >= m_nMatrixWidth)
			break;
		CTile* pTile = &m_pTiles[row * m_nMatrixWidth + malh.nFromX];
		int col = malh.nFromX;
		for(int t = 0 ;t < malh.nTileCount ; t++,col++,pTile++)
		{
			// ��ȡCTile
			BYTE cbMostData = 0;
			pArchive->Read((char*)&cbMostData,1);
			if(cbMostData == CTile::fMostTileValueFlag)	// �����һ������ĵر����ͣ����޵������ͨ���ر���ѹ���洢
			{
				pTile->SetMostValue();
				continue;
			}
			pArchive->Seek(-1);
			
			if(!pTile->OnSerialize(FALSE,pArchive,FALSE))
				break;
			
			// ��ȡwItemCount
			WORD wItemCount = 0;
			pArchive->Read(&wItemCount,sizeof(wItemCount));
			// ��ȡCTile�ϵ�����CMapItem����
			for(WORD wItem = 0; wItem < wItemCount;wItem++)
			{
				// ��ȡdwClassID
				DWORD dwClassID = 0;
				if(!pArchive->Read((char*)&dwClassID,sizeof(dwClassID)))
					break;
				WORD wObjectLen = 0;
				if(!pArchive->Read((char*)&wObjectLen,sizeof(wObjectLen)))
					break;
				// ����������Ҫ�����ͻ���item��ֱ������������������Ҫ�趨һЩ������ר�õ�item
				//........
				pArchive->Seek(wObjectLen);	
			}
		}
	}
	if(row < m_nMatrixWidth)
	{
		TraceLn("CMapContainer::OnReadServerMap(sRead) failed!!");
		return FALSE;
	}
	TraceLn(">>>>>>>>>>>>>>>>>>>> End map data <<<<<<<<<<<<<<<<");
	return TRUE;	
}
//------------------------------------------------------------------------
// ��������ȡ�ر���Ϣ
BOOL CMapContainer::OnReadServerMap112(IArchive *pArchive, ITrace *pTrace)
{
	m_pArchive = pArchive;
	m_pItemCF = NULL;
	TraceLn(">>>>>>>>>>>>>>>>>>>> read map data <<<<<<<<<<<<<<<<");
#ifdef _DEBUG
	DWORD dwStart = GetTickCount();
#endif
	// ��ȡ��ͼ�ļ�ͷ
	MAP_ARCHIVE_HEADER mh;
	int nMapOffset = pArchive->GetBufferOffset();
	pArchive->Read((char*)&mh, sizeof(MAP_ARCHIVE_HEADER));
	if (!mh.IsValid())
	{
		TraceLn("Error : CMapContainer::OnReadServerMap112() map file is not valid,Version = %06X",mh.dwVersion);
		return FALSE;
	}
	
	// ����ͼ�Ŀ�߽���64(32)���봦��
	mh.nMapWidth &= ~63;
	mh.nMapHeight &= ~31;
	
	// ������ͼ
	RECT rcTilesPreRead = {0, 0, mh.nMapWidth, mh.nMapHeight};
	if (!Create(mh.nMapWidth, mh.nMapHeight, rcTilesPreRead, pTrace))
		return FALSE;
	
	// ����������
	int nGridRow = (m_nMapHeight - 1)/mh.nGridHeight + 1;
	int nGridCol = (m_nMapWidth - 1)/mh.nGridWidth + 1;
	
	// 2����¼ӳ����ַ,ӳ����м�¼ÿ�������������ļ�ͷ��ƫ��
	m_pMapTable = (DWORD*)pArchive->GetBuffer();
	
	// 3����ȡÿ����������Ϣ
	int nTileRow = mh.nGridHeight / 32;
	int nTileCol = mh.nGridWidth / (64/2);
	POINT ptTileLeftTop;
	POINT ptLeftTop;
	for (int row=0; row<nGridRow; row++)
	{
		for (int col=0; col<nGridCol; col++)
		{
			// ��λÿ��������λ��
			pArchive->SeekTo(m_pMapTable[row*nGridCol+col] + nMapOffset);
			
			ptLeftTop.x = col*mh.nGridWidth;
			ptLeftTop.y = row*mh.nGridHeight;
			
			m_MapCo.PixelToTile(ptLeftTop, ptTileLeftTop);
			if (!_LoadBlock(pArchive, ptTileLeftTop, nTileRow, nTileCol, NULL))
				return FALSE;
		}
	}	
#ifdef _DEBUG
	TraceLn("time = %d", GetTickCount()-dwStart);
#endif	
	TraceLn(">>>>>>>>>>>>>>>>>>>> End map data <<<<<<<<<<<<<<<<");
	return TRUE;
}
//------------------------------------------------------------------------
/*
 A �� B
 dx=0,dy<0
 (0,0)
����������������������
��                B ��
��(0,-dy)           ��
����������������������
���w�w�w�w�w�w�w�w�w��
����������������������(kx,ky)
��                  ��
��A                 ��
����������������������(nCols,nRows)
 kx=nCols;
 ky=nRows+dy;
*/
//------------------------------------------------------------------------
void CMapContainer::_MovePreReadRect_Top(BLOCK_INFO* pBlockInfo, int nRows, int nCols, 
				 int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy)
{
	int row,col;
	// ��[0,-dy,kx,ky)�Ŀ���Ϣ����ƶ�(-dy)��
	int kx = nCols;
	int ky = nRows+dy;
	int dy_nCols = dy*nCols;
	int row_nCols = (ky-1)*nCols;
	for (row=ky-1; row>=(-dy); row--,row_nCols-=nCols)
	{
		for (col=0; col<nCols; col++)
		{
			CopyBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols-dy_nCols+col].ptLeftTop, nTileRow, nTileCol);
		}
	}

	// ����ԭ��
	SetOriginTile(ptNewTileOrigin);

	// ǰ(-dy)�м����µ���Ϣ
	row_nCols = 0;
	for (row=0; row<(-dy); row++,row_nCols+=nCols)
	{
		for (col=0; col<nCols; col++)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}
}
//------------------------------------------------------------------------
/*
 A �� B
 dx>0,dy<0
 (0,0)
��������������������������������
��        ��                B ��
��        ��(dx,-dy)          ��
�����������੤��������        ��
��        ���w�w�w�w��        ��
��        �����������੤��������
��                  ��(kx,ky) ��
��A                 ��        ��
��������������������������������(nCols,nRows)
 kx=nCols-dx;
 ky=nRows+dy;
*/
//------------------------------------------------------------------------
void CMapContainer::_MovePreReadRect_RightTop(BLOCK_INFO* pBlockInfo, int nRows, int nCols, 
				int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy)
{
	int row,col;
	// ����[dx,-dy,kx,ky)�Ŀ���Ϣ�������ƶ�(-dy)��(dx��)
	int kx = nCols-dx;
	int ky = nRows+dy;
	int dy_nCols = dy*nCols;
	int row_nCols;
	for (col=dx; col<kx; col++)
	{
		row_nCols = -dy_nCols;
		for (row=-dy; row<ky; row++,row_nCols+=nCols)
		{
			CopyBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols-dy_nCols+(col-dx)].ptLeftTop, nTileRow, nTileCol);
		}
	}
	
	// ����ԭ��
	SetOriginTile(ptNewTileOrigin);
	
	// ���dx�м����µ���Ϣ
	for (col=kx; col<nCols; col++)
	{
		row_nCols = 0;
		for (row=0; row<ky; row++,row_nCols+=nCols)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}

	// ǰ(-dy)�м����µ���Ϣ
	row_nCols = 0;
	for (row=0; row<(-dy); row++,row_nCols+=nCols)
	{
		for (col=dx; col<kx; col++)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}
}
//------------------------------------------------------------------------
/*
 A �� B
 dx>0,dy=0
 (0,0)   (dx,0)
�����������Щ��������Щ���������
��        ���w�w�w�w��       B��
��        ���w�w�w�w��        ��
��        ���w�w�w�w��        ��
��A       ���w�w�w�w��(kx,ky) ��
�����������ة��������ة���������(nCols,nRows)
 kx=nCols-dx;
 ky=nRows;
*/
//------------------------------------------------------------------------
void CMapContainer::_MovePreReadRect_Right(BLOCK_INFO* pBlockInfo, int nRows, int nCols, 
				int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy)
{
	int row,col;
	// ����[dx,0,kx,ky)�Ŀ���Ϣ�����ƶ�dx��
	int kx = nCols-dx;
	int ky = nRows;
	int row_nCols = 0;
	for (col=dx; col<kx; col++)
	{
		row_nCols = 0;
		for (row=0; row<ky; row++,row_nCols+=nCols)
		{
			CopyBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+(col-dx)].ptLeftTop, nTileRow, nTileCol);
		}
	}
	
	// ����ԭ��
	SetOriginTile(ptNewTileOrigin);
	
	// �ұ�dx�м����µ���Ϣ
	for (col=kx; col<nCols; col++)
	{
		row_nCols = 0;
		for (row=0; row<ky; row++,row_nCols+=nCols)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}
}
//------------------------------------------------------------------------
/*
 A �� B
 dx>0,dy>0
 (0,0)
��������������������������������
��A                 ��        ��
��      (dx,dy)     ��        ��
��        �����������੤��������
��        ���w�w�w�w��        ��
�����������੤��������(kx,ky) ��
��        ��                  ��
��        ��                 B��
�����������ة�������������������(nCols,nRows)
 kx=nCols-dx;
 ky=nRows-dy;
*/
//------------------------------------------------------------------------
void CMapContainer::_MovePreReadRect_RightDown(BLOCK_INFO* pBlockInfo, int nRows, int nCols,
				int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy)
{
	int row,col;
	// ����[dx,dy,kx,ky)�Ŀ���Ϣ�������ƶ�dx��(dy��)
	int kx = nCols - dx;
	int ky = nRows - dy;
	int dy_nCols = dy*nCols;
	int row_nCols;
	for (col=dx; col<kx; col++)
	{
		row_nCols = dy_nCols;
		for (row=dy; row<ky; row++,row_nCols+=nCols)
		{
			CopyBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols-dy_nCols+(col-dx)].ptLeftTop, nTileRow, nTileCol);
		}
	}
	
	// ����ԭ��
	SetOriginTile(ptNewTileOrigin);
	
	// ���dx�м����µ���Ϣ
	for (col=kx; col<nCols; col++)
	{
		row_nCols = dy_nCols;
		for (row=dy; row<nRows; row++,row_nCols+=nCols)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}
	
	// ���dy�м����µ���Ϣ
	row_nCols = ky*nCols;
	for (row=ky; row<nRows; row++,row_nCols+=nCols)
	{
		for (col=dx; col<kx; col++)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}
}
//------------------------------------------------------------------------
/*
 A �� B
 dx=0,dy>0
 (0,0)
����������������������
��A                 ��
��(0,dy)            ��
����������������������
���w�w�w�w�w�w�w�w�w��
����������������������(kx,ky)
��                  ��
��                 B��
����������������������(nCols,nRows)
 kx=nCols;
 ky=nRows-dy;
*/
//------------------------------------------------------------------------
void CMapContainer::_MovePreReadRect_Down(BLOCK_INFO* pBlockInfo, int nRows, int nCols, 
				int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy)
{
	int row,col;
	// ����[0,dy,kx,ky)�Ŀ���Ϣ�����ƶ�dy��
	int kx = nCols;
	int ky = nRows - dy;
	int dy_nCols = dy*nCols;
	int row_nCols = dy_nCols;
	for (row=dy; row<ky; row++,row_nCols+=nCols)
	{
		for (col=0; col<nCols; col++)
		{
			CopyBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols-dy_nCols+col].ptLeftTop, nTileRow, nTileCol);
		}
	}

	// ����ԭ��
	SetOriginTile(ptNewTileOrigin);

	// ���dy�м����µ���Ϣ
	row_nCols = ky*nCols;
	for (row=ky; row<nRows; row++,row_nCols+=nCols)
	{
		for (col=0; col<nCols; col++)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}
}
//------------------------------------------------------------------------
/*
 A �� B
 dx<0,dy>0
 (0,0)
��������������������������������
��        ��A                 ��
��        ��(-dx,dy)          ��
�����������੤��������        ��
��        ���w�w�w�w��        ��
��        �����������੤��������
��                  ��(kx,ky) ��
��                 B��        ��
��������������������������������(nCols,nRows)
 kx=nCols+dx;
 ky=nRows-dy;
*/
//------------------------------------------------------------------------
void CMapContainer::_MovePreReadRect_LeftDown(BLOCK_INFO* pBlockInfo, int nRows, int nCols, 
				int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy)
{
	int row,col;
	// ����[-dx,dy,kx,ky)�Ŀ���Ϣ�������ƶ�һ(-dx)��(dy��)
	int kx = nCols + dx;
	int ky = nRows - dy;
	int dy_nCols = dy*nCols;
	int row_nCols;
	for (col=kx-1; col>=(-dx); col--)
	{
		row_nCols = dy_nCols;
		for (row=dy; row<ky; row++,row_nCols+=nCols)
		{
			CopyBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols-dy_nCols+(col+(-dx))].ptLeftTop, nTileRow, nTileCol);
		}
	}
	
	// ����ԭ��
	SetOriginTile(ptNewTileOrigin);
	
	// ���(-dx)�м����µ���Ϣ
	for (col=0; col<(-dx); col++)
	{
		row_nCols = dy_nCols;
		for (row=dy; row<nRows; row++,row_nCols+=nCols)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}
	
	// ���dy�м����µ���Ϣ
	row_nCols = ky*nCols;
	for (row=ky; row<nRows; row++,row_nCols+=nCols)
	{
		for (col=(-dx); col<kx; col++)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}
}
//------------------------------------------------------------------------
/*
 A �� B
 dx<0,dy=0
 (0,0)   (-dx,0)
�����������Щ��������Щ���������
��        ���w�w�w�w��       A��
��        ���w�w�w�w��        ��
��        ���w�w�w�w��        ��
��B       ���w�w�w�w��(kx,ky) ��
�����������ة��������ة���������(nCols,nRows)
 kx=nCols+dx;
 ky=nRows;
*/
//------------------------------------------------------------------------
void CMapContainer::_MovePreReadRect_Left(BLOCK_INFO* pBlockInfo, int nRows, int nCols, 
				int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy)
{
	int row,col;
	// ����[-dx,0,kx,ky)�Ŀ���Ϣ����ƶ�(-dx)��
	int kx = nCols + dx;
	int ky = nRows;
	int row_nCols = 0;
	for (col=kx-1; col>=(-dx); col--)
	{
		row_nCols = 0;
		for (row=0; row<nRows; row++,row_nCols+=nCols)
		{
			CopyBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+(col+(-dx))].ptLeftTop, nTileRow, nTileCol);
		}
	}
	
	// ����ԭ��
	SetOriginTile(ptNewTileOrigin);
	
	// ���(-dx)�м����µ���Ϣ
	for (col=0; col<(-dx); col++)
	{
		row_nCols = 0;
		for (row=0; row<nRows; row++,row_nCols+=nCols)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}
}
//------------------------------------------------------------------------
/*
 A �� B
 dx<0,dy<0
 (0,0)
��������������������������������
��B                 ��        ��
��      (-dx,-dy)   ��        ��
��        �����������੤��������
��        ���w�w�w�w��        ��
�����������੤��������(kx,ky) ��
��        ��                  ��
��        ��                 A��
�����������ة�������������������(nCols,nRows)
 kx=nCols+dx;
 ky=nRows+dy;
*/
//------------------------------------------------------------------------
void CMapContainer::_MovePreReadRect_LeftTop(BLOCK_INFO* pBlockInfo, int nRows, int nCols, 
				int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy)
{
	int row,col;
	// ����[-dx,-dy,kx,ky)�Ŀ���Ϣ�������ƶ�(-dx)��((-dy)��)
	int kx = nCols + dx;
	int ky = nRows + dy;
	int dy_nCols = dy*nCols;
	int row_nCols;
	for (col=kx-1; col>=(-dx); col--)
	{
		row_nCols = -dy_nCols;
		for (row=(-dy); row<ky; row++,row_nCols+=nCols)
		{
			CopyBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols-dy_nCols+(col+(-dx))].ptLeftTop, nTileRow, nTileCol);
		}
	}
	
	// ����ԭ��
	SetOriginTile(ptNewTileOrigin);
	
	// ���(-dx)�м����µ���Ϣ
	for (col=0; col<(-dx); col++)
	{
		row_nCols = 0;
		for (row=0; row<ky; row++,row_nCols+=nCols)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}
	
	// ����(-dy)�м����µ���Ϣ
	row_nCols = 0;
	for (row=0; row<(-dy); row++,row_nCols+=nCols)
	{
		for (col=(-dx); col<kx; col++)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}
}
//------------------------------------------------------------------------
/*
 A �� B
 dx?,dy?
 (0,0)
����������������������������������������������
��                      ��A                 ��
��                      ��                  ��  
��                      ��                  ��
��                      ��                  ��
����������������������  ����������������������
��                  ��                      ��
��                  ��                      ��
��                  ��                      ��
��                 B��                      ��
����������������������������������������������(nCols,nRows)
 kx=nCols+dx;
 ky=nRows-dy;
*/
//------------------------------------------------------------------------
void CMapContainer::_MovePreReadRect_All(BLOCK_INFO* pBlockInfo, int nRows, int nCols, 
				int nTileRow, int nTileCol, POINT& ptNewTileOrigin, int dx, int dy)
{
	int row,col;
	// ����ԭ��
	SetOriginTile(ptNewTileOrigin);
	
	// ÿ��ÿ�м����µ���Ϣ
	for (col=0; col<nCols; col++)
	{
		int row_nCols = 0;
		for (row=0; row<nRows; row++,row_nCols+=nCols)
		{
			LoadBlockInfo(pBlockInfo[row_nCols+col].ptLeftTop, 
				pBlockInfo[row_nCols+col].nIndex, nTileRow, nTileCol);
		}
	}
}
//------------------------------------------------------------------------