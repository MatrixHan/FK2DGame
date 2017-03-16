/**
*	created:		2012-7-1   2:59
*	filename: 		MiniMap
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		С��ͼ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKCanvas/IFKCanvas.h"
#include "../../FKFileSystem/IFKFileSystem.h"
#include "../../FKGame/SceneObj/IItemClassFactory.h"
#include "../../FKGame/Common/MapItemDef.h"
#include "../Include/MiniMapView.h"
//------------------------------------------------------------------------
#define		MINIMAP_SMALL_WIDTH				119				// С��ͼ��С�ͣ��Ŀ�
#define		MINIMAP_SMALL_HEIGHT			119				// С��ͼ��С�ͣ��ĸ�
//------------------------------------------------------------------------
#define		MINIMAP_SMALL_LEFT_IN_SCREEN	880			// С��ͼ��С�ͣ�����Ļ�е�x����
#define		MINIMAP_SMALL_TOP_IN_SCREEN		25				// С��ͼ��С�ͣ�����Ļ�е�y����
//------------------------------------------------------------------------
#define		MINIMAP_BIG_WIDTH				193				// С��ͼ�����ͣ��Ŀ�
#define		MINIMAP_BIG_HEIGHT				196				// С��ͼ�����ͣ��ĸ�
//------------------------------------------------------------------------
#define		MINIMAP_BIG_LEFT_IN_SCREEN		799				// С��ͼ�����ͣ�����Ļ�е�x����
#define		MINIMAP_BIG_TOP_IN_SCREEN		23				// С��ͼ�����ͣ�����Ļ�е�y����
//------------------------------------------------------------------------
class CMiniMap
{
	typedef void (CMiniMap::*ProcDrawObj)(ICanvas* pCanvas, int xPos, int yPos, DWORD dwOccVal, WPixel wpColor);
	static ProcDrawObj m_bufDrawObjFuncs[miMaxClassID];

	BOOL		m_bVisible;			// С��ͼ�Ƿ���ʾ
	RECT		m_rcView;			// С��ͼ��ͼ����
	int			m_nOldX, m_nOldY;	// ���ǵľ�λ��
	int			m_nMapWidth;		// ��ͼ�Ŀ��
	int			m_nMapHeight;		// ��ͼ�ĸ߶�
	int			m_nMiniWidth;		// С��ͼ�Ŀ��
	int			m_nMiniHeight;
	int			m_nMiniMapRate;		// С��ͼ�����ű�
	POINT		m_ptMainPlayer;		// ����λ��
	CMiniMapView	m_MiniMapView;	// С��ͼ�ر���ͼ
	BOOL		m_bMiniMapBigOrSmall;	// С��ͼ�Ǵ���С��FALSE��С

public:
	ICanvasHelper	m_CanvasHelper;
	CMiniMap();
	virtual ~CMiniMap();

	BOOL Create(int nMapWidth, int nMapHeight, LPRECT lprcView, LPCSTR szMiniMapJpgFileDir, int nMiniMapRate, IFileObject* pTinyMapFile=NULL);
	// ������΢��ͼ�������������ڵ�ͼ�����ã�Ҳ������ĳ������������
	void SetViewport(LPRECT lprcView);
	// ���õ�ǰ��ͼ��������ݣ����ı��ͼ����Ҫ����������Щ����
	BOOL SetMapData(int nMapWidth, int nMapHeight, LPCSTR szMiniMapJpgFileDir);
	// ������΢��ͼ�Ŀɼ���
	void SetVisible(BOOL bVisible) {m_bVisible = bVisible;}
	// ����С��ͼ�İ�͸��ֵ
	void SetMiniMapTransparent(int nAlpha)	{m_MiniMapView.SetAlpha(nAlpha);}
	
	// ����С��ͼ��С
	void SetMiniMapBigOrSmall(BOOL bBig);
	// ��ȡС��ͼ��С
	BOOL GetMiniMapBigOrSmall();

	// ��Ⱦ��΢��ͼ
	void OnDraw(ICanvas* pCanvas, int nMainPlayerX, int nMainPlayerY, BOOL bForceTrans);
	// �����������ָ����ı�־(xPos,yPos������������)
	void DrawRectangleFlag(ICanvas* pCanvas, int xPos, int yPos, WPixel clr);
	// ��Ϸ�˳�ǰ�ͷ���΢��ͼ
	void Release()			{delete this;}

	BOOL IsVisible() {return m_bVisible;}

	void DrawObjByClassID(ICanvas* pCanvas, int xPos, int yPos, DWORD dwOccVal, DWORD dwClassID, WPixel wpColor);

	int GetViewWidth()	{return m_rcView.right - m_rcView.left;}
	int GetViewHeight()	{return m_rcView.bottom - m_rcView.top;}
	int GetViewLeft()	{return m_rcView.left;}
	int GetViewTop()	{return m_rcView.top;}

	void CalcMousePos(POINT& pt, POINT& ptMouse);

	void Close();
	void DrawMainActor(ICanvas* pCanvas, WPixel wpColor);
private:
	void drawFlags(ICanvas* pCanvas, int x, int y, WPixel clr);
	void drawRect(ICanvas* pCanvas, RECT& rc, WPixel clr);
	void DrawCreatureFlags(ICanvas* pCanvas, int xPos, int yPos, DWORD dwOccVal, WPixel wpColor);
	void DrawOreFlags(ICanvas* pCanvas, int xPos, int yPos, DWORD dwOccVal, WPixel wpColor);

	void __CalPicPos(int &xPos, int &yPos);  //����ͼƬ����ʾλ�ã����ϣ�
};
//------------------------------------------------------------------------