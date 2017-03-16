/**
*	created:		2012-6-21   22:26
*	filename: 		IMapView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#ifndef VERSION_MAPVIEW
#define VERSION_MAPVIEW			1
#endif
//------------------------------------------------------------------------
#include "../../FKCanvas/IFKCanvas.h"
//------------------------------------------------------------------------
class CTile;
class CMapItem;
class CMapItemView;
struct IArchive;
struct IItemClassFactory;
struct IFileObject;
//------------------------------------------------------------------------
// ��CMapContainer�е�SNAP_TILE�ṹ��һ����
struct SNAP_TILE_INFO  
{
	POINT		ptTile;
	CTile*		pTile;
};
//------------------------------------------------------------------------
// ��CMapContainer�е�SNAP_ITEM�ṹ��һ����
struct SNAP_ITEM_INFO 
{
	POINT		ptTile;
	CMapItem*	pItem;
};
//------------------------------------------------------------------------
// ��������
enum enWeatherType
{
	enWeather_Type_None = 0,		// ������Ч��
	enWeather_Type_Rain,			// ����
	enWeather_Type_Snow,			// ��ѩ
	enWeather_Type_MeiGui,			// ��õ�廨��
};
//------------------------------------------------------------------------
// ������Ϣ
struct SWeatherInfo 
{
	enWeatherType	nWeatherType;	// ��������
	BOOL			bRandom;		// �Ƿ���������ΪTRUE�Ļ�����Ŀ��Ʋ�������
	int				nParticleNum;	// ��������		50��500 (õ�廨������䵽1000)
	int				nWindPower;		// ����			-100��100
	int				nSpeed;			// �����ٶ�		1��10
};
//------------------------------------------------------------------------
// ����ͼ�༭��ʹ�õĵ�ͼ��ʾ�ӿ�
struct IMapView
{
	// ����һ������
	virtual BOOL AddItem(const POINT& ptTile, CMapItemView* pItem, DWORD dwParam=0) = NULL;
	//�������Ƶ�һ���¿�
	virtual BOOL MoveItem(const POINT& ptFrom, const POINT& ptTo, CMapItemView* pItem, DWORD dwParam=0) = NULL;
	// ɾ������
	virtual BOOL RemoveItem(const POINT& ptTile, CMapItemView* pItem, DWORD dwParam=0) = NULL;
	// �������ռλ��Ϣ
	virtual BOOL AddItemOccupant(const POINT& ptTile, CMapItemView* pItem) = NULL;
	// ɾ������ռλ��Ϣ
	virtual BOOL RemoveItemOccupant(const POINT& ptTile, CMapItemView* pItem) = NULL;
	/* 
		���Թ��������ָ�Ķ���
		ptScreen ����Ļ���꣬����ڵ�ͼ���Ͻǵ�ƫ��
		pItemFoundAnchorTile : ����ҵ����󣬷��ش˶�����ŵ����ڵ�tile������Ͷ���ָ��
		dwFlag : 
			ָ������˱�־�Ķ�����ܱ�ѡ�С�
			��dwFlag == 0ʱ����ʾֻҪ��Զ���Ŀɼ���λ��Ϊѡ�У��ʺ��ڵ�ͼ�༭��
			��dwFlag == htCanSelʱ����ʾ����Ƿ���ͨ��CMapItemView::AddFlag()
			���ö����flagCanbeSelected��־���ʺ�����Ϸ������
	 */
	enum {htCanSel = 2/*ֻѡ��ָ����ѡ�еĶ��󣬼���Ȼ���ж���ɼ����򣬵�û���ÿ�ѡ�б�־Ҳѡ����*/};
	virtual CMapItemView* HitTest(IN const RECT& rcScreen, OUT POINT* pItemFoundAnchorTile, IN DWORD dwHitFlag = 0) = NULL;
	/* 
		��д��ͼ�浵
		bWriteΪTRUEʱ����ʾ���棬pItemCF����ΪNULL
		bWriteΪFALSEʱ����ʾ��ȡ��ͼ�����ѻָ���ͼ�ϵ����ж����ֳ���pItemCFΪ���󴴽���
		//	�ݲ����˲���
		//	bServerMap:
	 	//	��bServerMap��־�����Ǳ��������ר�õ�ͼ���ǿͻ��˵�ͼ��
		//	���ߵ������ǣ��������˵�ͼ����Ҫ�����ֻ��ҪCTile���ݼ���
		//	ֻ�Ե�ͼ�浵��Ч
	*/
	virtual BOOL OnSerialize(BOOL bWrite, IArchive* pArchive, IItemClassFactory* pItemCF, LPRECT lprcViewport, ITrace* pTrace) = NULL;
	// ��ͼ��������
	// �����ͼͼƬ(ԭ������ 1024*768 �Ĵ�Сһ��һ��������)
	// nStartRow,nStartCol,nRows,nCols�������������
	// fRate:������ͼƬ�뱣��ǰͼƬ��С�ı�ֵ
	// bDrawGround:�Ƿ���Ƶر�
	// bCombine:�Ƿ����ɵ�ÿ��ͼƬ�ϲ���һ��
	virtual BOOL SaveMapBitmapToFile(LPCSTR szFileName, int nStartRow, int nStartCol, int nRows, int nCols, 
		float fRate = 1.0f, BOOL bDrawGround = TRUE, BOOL bCombine = TRUE) = NULL;	
	// ��Ļ���
	virtual int GetMapWidth() const = NULL;
	virtual int GetMapHeight() const = NULL;
	/* ���ܣ���ȡ��ͼtile����Ŀ�ȡ���ͼtile���鳤������ȵ�:width=height+1
	   ˵����
			����ģ����Ҫ�������ͼ��ͬ�Ĵ�С��tile���飬�����ṩ�˺���
	*/
	virtual int GetMapTileWidth() = NULL;
	virtual int GetMapTileHeight() = NULL;
	// ��ȡ��ǰ�ɼ���ͼ���Ͻ�world����
	virtual int GetViewTopLeftX() const  = NULL;
	virtual int GetViewTopLeftY() const = NULL;
	// ���Ӵ��ھ��󣬷��ؾ�����������
	virtual const RECT& GetViewportRect() const = NULL;
	// �ӿڴ�С�ı䣬�մ�����ͼʱĬ�ϴ�СΪ1024 x 768
	virtual void OnSizeViewport(int nViewWidth, int nViewHeight) = NULL;
	// ������Ļ
	virtual BOOL ScrollViewport(int dx, int dy) = NULL;
	// ������Ļ�������ܰ�ptScreenCenter��λ����Ļ���ġ������Ǿ���ģʽ��Ҫ�������
	virtual BOOL ScrollToCenter(POINT ptScreenCenter) = NULL;
	// ���Ƶ�ͼ,rcΪ��ͼ��Ļ�����ݲ���
	virtual void OnDraw(ICanvas* pCanvas, const void* pParam) = NULL;
	virtual void DrawRectangleGrid(ICanvas* pCanvas, int nGridWidth, int nGridHeight, WPixel clrLine, WPixel clrText) = NULL;
	// ���ؿ�����ĵ����Ļ����
	virtual void ScreenToTile(IN const POINT& ptScreen, OUT POINT& ptTile) const = NULL;
	// ������Ļ���Ӧ��tile����
	virtual void TileToScreen(IN const POINT& ptTile, OUT POINT& ptTileCenter) const = NULL;
	// �������������Ӧ�Ŀ�����
	virtual void WorldToTile(IN const POINT& ptWorld, OUT POINT& ptTile) const = NULL;
	// ���ؿ����������
	virtual void TileToWorld(IN const POINT& ptTile, OUT POINT& ptTileCenter) const = NULL;
	// ���ؿ����
	// ���Ƿ���CTile����ͨ��CTile::IsValid()����tile�Ƿ���Ч
	virtual CTile* GetTile(const POINT& ptTile) const = NULL;
	// ������Ļ��������ȡ�ÿ�ָ��
	// ���Ƿ�����Ч��ַ��CTile������CTile.IsValid()�жϴ�tile�Ƿ���Ч
	virtual CTile* GetTileFromScreen(POINT ptScreen) const = NULL;
	// ���tile�����Ƿ���Ч��tile�Ƿ���Ч
	virtual BOOL IsValidTile(const POINT& ptTile) = NULL;
	// �����Ƿ��ڵر��ϻ�����ռλ��
	enum{maskDrawGrid = 1,maskDrawOccupant = 2,
		maskClearDrawGrid = 0xFFFFFFFE,maskClearDrawOccupant = 0xFFFFFFFD};
	// �����Ƿ���ʾ�ر��־ͼ��pParamһ��Ϊ��Ӧ���ͼͼ��
	//	ע��ͬʱ���ö�����ʱ��ֻ����һ������pParam
	// ��dwFlag == maskDrawOccupantʱ��dwParamΪռλ��ͼƬpBitmap��ͼƬ��СΪ64 x 32
	virtual void SetDrawFlag(DWORD dwFlag, DWORD dwClearFlag, void* pParam = 0) = NULL;
	virtual DWORD GetDrawFlag(DWORD dwFlag) = NULL;
	virtual void Release() = NULL;
	virtual BOOL SetPlayerItem(CMapItemView* pItem) = NULL;
	virtual CMapItemView* GetPlayerItem() = NULL;
	/*
	  ���ܣ����߼������ڹ㲥���������Ͻǵ����½ǵ�˳�򡣹㷺���ڷ�Χ�㲥���繥��������㲥��
	  ������
		���룺
			rcWorld		: ��Ϸ������������
			nListCount	: pListBuf�����ά��
		�����
			pListBuf	: �������ж���
			nListCount	: ���� pListBuf ����Ч�Ķ�������
	 */
	// ����ͬEnumTileByWorldRect()
	virtual BOOL SnapshotTileByWorldRect(IN const RECT& rcWorld, IN OUT int& nListCount, OUT SNAP_TILE_INFO* pListBuf) = NULL;
	virtual BOOL SnapshotItemByWorldRect(IN const RECT& rcWorld, IN OUT int& nListCount, OUT SNAP_ITEM_INFO* pListBuf) = NULL;
	// ����С��ͼ�Ƿ�ɼ�
	virtual void SetMiniMapVisible(BOOL bVisible) = NULL;
	// ����С��ͼ����ͼ����
	virtual void SetMiniMapViewport(LPRECT lprcViewport) = NULL;
	// ����С��ͼ�İ�͸��ֵ
	virtual void SetMiniMapTransparent(int nAlpha) = NULL;
	// �õ�ʾ��ͼ����ʾ״̬
	virtual BOOL GetTinyMapVisible() = NULL;
	// ����ʾ��ͼ����ʾ״̬
	virtual void SetTinyMapVisible(BOOL bVisible) = NULL;
	// Ѱ·���
	// Ѱ· , �����������׳����� ����LPCTSTR
	// ptFrom	:   ·������ʼ��
	// ptTo		:   ·����Ŀ���
	// ...		
	virtual BOOL FindPath(POINTS ptFrom, POINTS ptTo, POINTS** ppBuffer, int& nPathLen) = NULL;
	// Ѱ��һ����·��
	// ��ʱ����·����Ҫ�ܾ�ȷֻ��Ҫ����һ��ģ��·���Ϳ�
	virtual BOOL FindSimplePath(POINTS ptFrom, POINTS ptTo, POINTS** ppBuffer, int& nPathLen) = NULL;
	// �������ر��ռλ��(�������ڵ�ͼ�༭��)
	// ԭ���ڵ�ͼ�༭���϶���һ��ͼԪ��ռλ�飬Ȼ������Ϣ���Ƶ���ͼ��ʹ����ͬ��ͼԪ�ĵط�
	// ptTileInRect:ģ�������ڵ�ĳ��Tile����(ϵͳ���Զ�У�������ϵ�)
	// nGridWidth,nGridHeight:ͼԪ����Ŀ�ߣ���ͼԪ�Ŀ��
	// nTileRows,nTileCols:Ҫ�������������������(�����ϵ㿪ʼ��)
	virtual BOOL QuickFillOccupant(POINT& ptTileInRect, int nGridWidth, int nGridHeight, int nTileRows, int nTileCols) = NULL;
	// ����������֧��
	virtual void SetOptimizedScrollScreen(BOOL bOptimized) = NULL;
	virtual BOOL GetOptimizedScrollScreen() = NULL;
	// ���п���ʱ��ʱ���õ�ͼԤ�ȴ���һЩ����
	virtual void OnHandleIdle(DWORD dwParam) = NULL;
	// Ϊ�˷�ֹ��ͣ�ĸ��½ӿڣ��������һ������ķ���������Ҫ���빦�ܶ���ϣ���޸Ľӿ�ʱ��������ʱ�����������ʵ��
	virtual int OnSomeHandle(DWORD dwParam1, DWORD dwParam2) = NULL;
	// ����С��ͼ��С
	virtual void SetMiniMapBigOrSmall(BOOL bBig) = NULL;
	// �ı��ͼ����
	virtual void ChangeWeather(SWeatherInfo info) = NULL;
};
//------------------------------------------------------------------------
extern IMapView* CreateNewMapView(DWORD dwVer, int nMapWidth, int nMapHeight,
							  LPRECT lprcViewport, LPRECT lprcMiniMapViewport,
							  LPCSTR szGroundJpgFileDir,
							  IFileObject* pGridIdxArrayFile, ITrace* pTrace);
// ע��szGroundJpgFileDir������Ҫ����������ΪNULL��Ϊ�����Կ��ǣ�Ӧ�ò�Ҳ��ָ��szGroundJpgFileDir
extern IMapView* LoadMapView(DWORD dwVer, IFileObject* pMapFile, IFileObject* pGridIdxArrayFile, IFileObject* pTinyMapFile,
						 LPCSTR szGroundJpgFileDir, LPCSTR szMapName, LPRECT lprcViewport, 
						 LPRECT lprcMiniMapViewport, LPCSTR szMiniMapJpgFileDir, BOOL bUseDynamicMap, int nMiniMapRate, 
						 IItemClassFactory *pItemCF, ITrace *pTrace);
//------------------------------------------------------------------------
// __LIB_USEMAPVIEW			// ���������ָ��ͨ��lib��ʽ���õ�ͼ��ͼ�ӿ�
//------------------------------------------------------------------------
class CMapViewHelper
{
public:
	typedef IMapView* (*ProcCreateNewMapView)(DWORD dwVer, int nMapWidth, int nMapHeight,
		LPRECT lprcViewport, LPRECT lprcMiniMapViewport,
		LPCSTR szGroundJpgFileDir, IFileObject* pGridIdxArrayFile, ITrace* pTrace);

	typedef IMapView* (*ProcLoadMapView)(DWORD dwVer,
		IFileObject* pMapFile, IFileObject* pGridIdxArrayFile, IFileObject* pTinyMapFile,
		const char* szGroundJpgFileDir, LPCSTR szMapName, LPRECT lprcViewport, 
		LPRECT lprcMiniMapViewport, LPCSTR szMiniMapJpgFileDir, BOOL bUseDynamicMap, int nMiniMapRate, 
		IItemClassFactory *pItemCF,ITrace *pTrace);
	
	CMapViewHelper()
	{
		m_pMapView = NULL;
		m_hdll = NULL;
	}
	~CMapViewHelper()
	{
		Close();
	}

	BOOL CreateNewMap(int nMapWidth, int nMapHeight,
		LPRECT lprcViewport, LPRECT lprcMiniMapViewport,
		LPCSTR szGroundJpgFileDir, IFileObject* pGridIdxArrayFile, ITrace* pTrace)
	{
		Close();
		if(!LoadDll())
			return NULL;
#if !defined(__LIB_USEMAPVIEW)
		ProcCreateNewMapView proc = (ProcCreateNewMapView)GetProcAddress(m_hdll, "dllCreateNewMapView");
		if(proc == NULL)
		{
			OutputDebugString("Can not GetProcAddress() : dllCreateNewMapView\r\n");
			return FALSE;
		}
			
		m_pMapView = proc(VERSION_MAPVIEW, nMapWidth, nMapHeight,
			lprcViewport, lprcMiniMapViewport,
			szGroundJpgFileDir, pGridIdxArrayFile, pTrace);
#else
		m_pMapView = CreateNewMapView(VERSION_MAPVIEW,nMapWidth,nMapHeight,
			lprcViewport, lprcMiniMapViewport,
			szGroundJpgFileDir,szGridIndexArray,pTrace);
#endif
		return m_pMapView != NULL;
	}

	/*
	 *	pMapFile: ��ͼ�ļ�
	 *	pGridIdxArrayFile: �ر����������ļ�
	 *	pTinyMapFile: ��ͼ���ͼ�ļ�
	 *	szgroundJpgFiledir: �ر�jpg�ļ�Ŀ¼
	 *	szMapName: ��ͼ����
	 *	lprcViewport: ��ͼ��ǰ���ӿ�(��=1024,��=768)
	 *	lprcMiniMapViewport: С��ͼ�ڴ��ͼ�е��ӿ�
	 *	szMiniMapJpgFileDir: С��ͼjpg�ļ�Ŀ¼
	 *	bUseDynamicMap: �Ƿ�ʹ�ö�̬��ͼ����(����ͼ��Сʱ���벻Ҫʹ�ö�̬��ͼ����)
	 *	nMiniMapRate: С��ͼͼƬ�����ű���(Ĭ��Ϊ20)
	 *	pItemCF: item����
	 *	pTrace: Trace�ӿ�
	 */
	BOOL LoadMap(IFileObject* pMapFile, IFileObject* pGridIdxArrayFile, IFileObject* pTinyMapFile,
		LPCSTR szGroundJpgFileDir, LPCSTR szMapName, LPRECT lprcViewport, 
		LPRECT lprcMiniMapViewport, LPCSTR szMiniMapJpgFileDir, BOOL bUseDynamicMap, int nMiniMapRate, 
		IItemClassFactory* pItemCF, ITrace* pTrace)
	{
		Close();
		if(!LoadDll())
			return FALSE;
#if !defined(__LIB_USEMAPVIEW)
		ProcLoadMapView proc= (ProcLoadMapView)GetProcAddress(m_hdll, "dllLoadMapView");
		if(proc == NULL)
		{
			OutputDebugString("Can not GetProcAddress() : dllLoadMapView\r\n");
			return FALSE;
		}
		m_pMapView = proc(VERSION_MAPVIEW, pMapFile, pGridIdxArrayFile, pTinyMapFile,
			szGroundJpgFileDir, szMapName, lprcViewport, 
			lprcMiniMapViewport, szMiniMapJpgFileDir, bUseDynamicMap, nMiniMapRate, 
			pItemCF, pTrace);
#else
		m_pMapView = LoadMapView(VERSION_MAPVIEW, pMapFile, pGridIdxArrayFile, pTinyMapFile,
			szGroundJpgFileDir, szMapName, lprcViewport, 
			lprcMiniMapViewport, szMiniMapJpgFileDir, bUseDynamicMap, nMiniMapRate, 
			pItemCF, pTrace);
#endif
		return m_pMapView != NULL;
	}

	void Close()
	{
		if(m_pMapView)
		{
			m_pMapView->Release();
			m_pMapView = NULL;
		}	
		if(m_hdll)
		{
			FreeLibrary(m_hdll);
			m_hdll = NULL;
		}
	}
	BOOL LoadDll()
	{
		try
		{
//#ifdef _USE // ����ͼ�༭����
//			if(m_hdll == NULL)
//				m_hdll = ::LoadLibrary("FKMapView_U.dll");
//			if(m_hdll == NULL)
//				throw "�޷���ȡFKMapView_U.dll";
//#elif defined(_DEBUG)
#ifdef _DEBUG
			if(m_hdll == NULL)
				m_hdll = ::LoadLibrary("FKMapView_D.dll");
			if(m_hdll == NULL)
				throw "�޷���ȡFKMapViewD.dll";
#else
			if(m_hdll == NULL)
				m_hdll = ::LoadLibrary("FKMapView.dll");
			if(m_hdll == NULL)
				throw "�޷���ȡFKMapView.dll";
#endif
			return TRUE;
		}
		catch(LPCSTR szMsg)
		{
			OutputDebugString(szMsg);OutputDebugString("\r\n");
			return FALSE;
		}
	}
	IMapView* operator->()
	{
		return m_pMapView;
	}
public:
	IMapView*	m_pMapView;
private:
	HINSTANCE		m_hdll;
};
//------------------------------------------------------------------------