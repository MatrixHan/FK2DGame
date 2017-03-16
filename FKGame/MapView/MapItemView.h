/**
*	created:		2012-6-29   19:29
*	filename: 		MapItemView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../MapInfo/MapItem.h"
#include <list>
using namespace std;
//------------------------------------------------------------------------
struct ICanvas;
struct IBitmap;
struct IMapView;
//------------------------------------------------------------------------
enum MouseEvent{mouseMove = 1,moveLeftClick = 2, mouseRightClick = 3,mouseLeave/*ʧȥ����*/ = 4};
//------------------------------------------------------------------------
class CMapItemView	: public CMapItem// ������������ͼ�ӿ�
{
public:
	CMapItemView()
	{
		m_dwFlag = 0;
		m_nFrameOld = -1;
		m_bNeedSetOldDirty = FALSE;
		SetRectEmpty(&m_rcArea);
		SetRectEmpty(&m_rcAreaOld);
		(__int64&)m_ptLeft = 0;
		(__int64&)m_ptRight = 0;
		(__int64&)m_ptWold = 0;
	}
	virtual ~CMapItemView()	{}

	// ��ͼѭ�����������Ƶ�ָ����ICanvas��
	// dwFlag : ���������ʱ���Ƿ��͸�����Ƶ�ѡ��
	virtual void OnDraw(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0) = NULL;

	// ��ȡ������δ�С��Χ����ê��ƫ��Ϊ(0,0)���������ê��ģ�����Ļ�޹�
	// ���Ҫ��ö�������Ļ�ϵķ�Χ������OffsetRect(pItemView->GetShowRect(),ptTileCenter);
	virtual RECT GetShowRect() = NULL;

	// ��ȡê��
	// ê�� �� ����ͼƬ���Ͻ� �� ����ͼƬ�����ÿ����ĵ��ص��ĵ� ��ƫ��
	virtual POINT GetAnchor() const = NULL;

	// ʵ��CMapItem::GetLayerValue()���󲿷ֶ������в�
	// �����ڵ�ͼ�е���ʾ�㣬��ʬ���ȵ������Ҫ�͡�
	// �в�ι�ϵ�Ķ���ͨ��CMapContainer::AddLayerItem()���뵽��ͼ������
	// ������ʵ�ֵĶ�����Ҫ���ǲ�ֵ
	virtual DWORD GetLayerValue()	{return milvMidle;}

	// ��ȡ��ǰ��ʾ��ͼ��
	virtual IBitmap* GetBitmap() = NULL;

	// ������ȡ���ݣ����ڴ��ļ��д�������
	// bSaveΪTRUEʱ����ʾ���󱣴����ݣ�����Ϊ��buf�ж�ȡ�����ֳ����д�������
	// 
	BOOL OnSerialize(BOOL bWrite,IArchive* pArchive){return FALSE;};

	/* ���ܣ� ����������ӿں��뿪�ӿ�ʱ���ص�
	 �������������Ԥ����Դ���ͷ���Դ
	 bVisableΪTRUEʱ��ʾ��������ʾ�������ʾ�����뿪�ӿ�
	 ���أ�
			���������Ұ���뿪��Ұ�ɹ�����ͼϵͳ���ݷ���ֵ�����Ƿ񽫶��������ʾ���л����ʾ������ɾ��
	 ˵����
			����������ǿɼ���ֻ��Ҫ���뿪��Ұʱ����FALSE
			����������ǲ��ɼ���ֻ��Ҫ�ڽ�����Ұʱ����FALSE
	*/ 
	virtual BOOL OnEnterViewport(BOOL bVisable) = NULL;

	// ���Ե��û�����ĳ��С����ʱ�Ƿ�ѡ��˶���
	// ptScreenΪ����ڴ˶�����ſ����ĵ������ƫ��ֵ
	virtual BOOL HitTest(const RECT& rcScreen)	{return FALSE;}

	// ���ö���ѡ��״̬
	// ������󲻿ɱ�ѡ�У����󷵻�FALSE�������ѡ�гɹ�������TRUE
	virtual BOOL SetSelected(BOOL bSelected = TRUE, BOOL bHighLight = TRUE){return FALSE;}

	// ���ö�������״̬ ������0�������� 1����Ȧ���� 2����Ȧ���� 3����Ȧ���� 4����Ȧ������
	// ������󲻿ɱ����������󷵻�FALSE������������ɹ�������TRUE
	virtual BOOL SetLocked(int nLockType = 1){return FALSE;}

	// ��λ�����item��־
	enum
	{
		flagNone=0,
		flagVisable=0x1,			// �����Ƿ�����ʾ�б���
		flagCanbeSelected = 0x2,	// �����Ƿ��ѡ��
		flagSelected = 0x4,			// �����Ƿ�ѡ��
		flagRectangle=0x8,			// ��ʾ����ľ��ο�
		flagInfo=0x10,				// ��ʾ�������Ϣ(���ܲ�����������)
		flagHiliht=0x20,			// ������ʾ����
		flagBloodSlot=0x40,			// ��ʾѪ��
		flagName=0x80,				// ��ʾ��������
		flagPaoPao=0x100,			// ��ʾpaopao��Ϣ
		flagGroup=0x200,			// ��ʾ�����Ϣ
	};
	/*
	 *	����λ����˵����
	 *		flagVisable : �����������˳�Ԥ������ʱ��ͨ�����ô�λ��ס�Ƿ�����ʾ�б��е�״̬
	 *		flagCanbeSelected : ��ͼ�༭������ѡ���κ�itemview���󣬵���Ϸ������ֻ��ѡ���߼�����������Ϸ�����򴴽�����ʱ���ô�λ
	 */
	virtual void AddFlag(DWORD dwFlag)
	{
		m_dwFlag |= dwFlag;
	}
	virtual void RemoveFlag(DWORD dwFlag = 0xFFFFFFFF)
	{
		m_dwFlag &= ~dwFlag;
	}
	virtual DWORD GetFlag()
	{
		return m_dwFlag;
	}

	// һЩ��������
	BOOL IsVisable()
	{
		return m_dwFlag & flagVisable;
	}
	BOOL IsSelected()
	{
		return m_dwFlag & flagSelected;
	}
	
	// ��������һЩ���������Ѫ�ۡ��������ݡ���Ʒ���ȣ���Щ������Զ�������ϲ�
	virtual void OnDrawTopmostObject(ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag = 0) {}

	// item�ڵ�ͼ�ϵ�λ�����
	virtual void SetTile(POINT& ptTile, IMapView* pMapView) = NULL;
	virtual POINT GetTile() = NULL;

	// �Ƚ���ʾ����˳��
	BOOL operator < (const CMapItemView& item);

	// ��ԴID
	virtual DWORD GetResID() = NULL;

	// ����
	virtual void SetDirty(ICanvas* pCanvas, const POINT& ptViewTop) {}

	// ��ȡ�ɵ��������Ҫ���ڰѶ���ӵ�ͼ��ɾ��ʱ��ȡ���������
	virtual void GetOldDirtyRect(list<RECT> &DirtyRectList) {}

	// �Ƿ�Ǳˮ���۹���
	virtual BOOL IsSinking() {return FALSE;}
private:
	DWORD	m_dwFlag;
public:
	// ������������
	RECT	m_rcArea;			// ��ǰ�������λ�ã������������ͼ��(left,top)������ŵ����tile����,(right,bottom)Ϊ�Ҷ˵����tile����
	POINT	m_ptLeft;			// �����������꣬���
	POINT	m_ptRight;			// �����������꣬�Ҷ�
	POINT	m_ptWold;			// �����������꣬Tile��

	RECT	m_rcAreaOld;		// �ϴλ�ͼ�ľ���λ��
	int		m_nFrameOld;		// ����
	BOOL	m_bNeedSetOldDirty;	// �Ƿ���Ҫ�Ѿɵľ�������
};
//------------------------------------------------------------------------