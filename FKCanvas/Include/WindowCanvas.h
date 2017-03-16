/**
*	created:		2012-6-20   19:57
*	filename: 		WindowCanvas
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../IFKCanvas.h"
#include "DirtyList.h"
#include "WindowBitmap.h"
//------------------------------------------------------------------------
class WCanvas : public ICanvas  
{
private:
	IBitmap*	m_pBmp;
	CDirtyList	m_rcDirty;
	CDirtyList	m_rcOutDirty;
	WBmpStruct	m_bmpStruct;	// ��������δ���
	WBitmap		m_bmp;			// ��������δ���ָ�� m_pBmp ��ĳ����λͼ
	BOOL		m_bInEqualOut;
private:
	void SetBorder();
	void SubBmpStruct(RECT& rc);
public:
	WCanvas();
	virtual ~WCanvas();

	// ��������
	virtual IBitmap* SelectBitmap(IBitmap* pBitmap) ;															//ѡ��һ��IBitmap
	virtual IBitmap* GetBitmap(void) const ;

	//��Canvas�Ľ���
	virtual BOOL Draw(int x, int y, ICanvas* pCanvas, DWORD dwStyle = BLIT_NORMAL) ;							//�ѱ��λͼ���Ƶ���λͼ
	virtual BOOL Draw(int dx, int dy, int w, int h, ICanvas* bmp, int sx, int sy, DWORD dwStyle = BLIT_NORMAL); //��ǿ��Draw

	//������븲�Ǿ��ι���
	virtual void AddDirtyRect(RECT& area) ;	
	virtual void AddDirtyRect(int x, int y, int w, int h){
		RECT rc;
		rc.left = x;
		rc.top = y;
		rc.right = x + w;
		rc.bottom = y + h;
		AddDirtyRect(rc);
	}

	virtual void AddInOutRect(RECT& area);
	virtual void AddInOutWholeScreen();
	virtual void AddInOutOverlay(RECT& area);
	virtual void AddOverlay(RECT& area) ;
	virtual void ResetDirtyRect(void) ;
	virtual void AddWholeScreen(void);
	virtual void Merge(void);
	virtual void* GetDirtyList();
	virtual void AddDirtyList(void* pList);
	virtual BOOL IsInDirty(RECT& rc) {return m_rcDirty.IsDirty(rc);}
	virtual int	GetDirtyCount() {return m_rcDirty.GetDirtyCount();}					// ��ȡ��ǰ���������Ŀ

	//�����ι���
	virtual void AddOutDirtyRect(RECT& area);
	virtual void AddOutDirtyRect(int x, int y, int w, int h);
	virtual void AddOutOverlay(RECT& area);
	virtual void ResetOutDirtyRect(void);
	virtual void AddOutWholeScreen(void);
	virtual void MergeOut(void);

	virtual void* GetOutDirtyList();
	virtual void AddOutDirtyList(void* pList);
	virtual BOOL IsOutDirty(RECT & rc)
	{
		if(m_bInEqualOut)
			return m_rcDirty.IsDirty(rc);
		return m_rcOutDirty.IsDirty(rc);
	}
	virtual void DrawDirtyList(ICanvas* pICanvas,WPixel color);
	virtual void DrawOutDirtyList(ICanvas *pICanvas, WPixel color);

	//ʵ�ֵ�IBitmap�ӿ�
	//��ʼ������
	virtual BOOL Create(int nWidth, int nHeight) ;																				//����λͼ
	virtual BOOL Create(int nWidth, int nHeight, void* ptr) ;																	//��������������λͼ
	virtual BOOL Create(IBitmap* parent, int x, int y, int nWidth, int nHeight);												//�Ӹ�λͼ�ϴ�������
	virtual IBitmap* SubBitmap(int x, int y, int nWidth, int nHeight) ;															//������λͼ
	virtual int GetWidth() const ;																								//��ÿ��
	virtual int GetHeight() const ;																								//��ø߶�

	//��ͼ���
	virtual BOOL Clear(WPixel clr, RECT* pRc = NULL) ;																			//����
	virtual BOOL Draw(int x, int y, IBitmap* bmp, DWORD dwStyle = BLIT_NORMAL) ;												//�ѱ��λͼ���Ƶ���λͼ
	virtual BOOL Draw(int dx, int dy, int w, int h, IBitmap* bmp, int sx, int sy, DWORD dwStyle = BLIT_NORMAL);					//��ǿ��Draw
	virtual BOOL DrawAlpha(int dx, int dy, IAlphaBitmap* bmp, DWORD dwStyle = BLIT_COPY);										//�ѱ��λͼ���Ƶ���λͼ
	virtual BOOL DrawAlpha(int dx, int dy, int w, int h, IAlphaBitmap* bmp, int sx, int sy, DWORD dwStyle = BLIT_COPY);			//��ǿ��AlphaDraw
	virtual BOOL DrawRle(int dx, int dy, IRleBitmap* bmp, DWORD dwStyle = BLIT_COPY);											//�ѱ��rleλͼ���Ƶ���λͼ
	virtual BOOL DrawRle(int dx, int dy, int w, int h, IRleBitmap* bmp, int sx, int sy, DWORD dwStyle = BLIT_COPY);				//��ǿ��rle����
	virtual BOOL DrawRle(int dx, int dy, IRleBitmap* bmp, BYTE* pPal, DWORD dwStyle = BLIT_COPY);								//�ѱ��rleλͼ���Ƶ���λͼ
	virtual BOOL DrawRle(int dx, int dy, int w, int h, IRleBitmap* bmp, BYTE* pPal, int sx, int sy, DWORD dwStyle = BLIT_COPY); //��ǿ��rle����
	virtual void Draw(int x, int y, const char* s, WPixel clr) ;																//��ӡASCII�ַ�
	virtual BOOL Update2DC(HDC hDC,int dx,int dy,int x,int y,int w,int h) ;														//BLIT һ�����β��ֵ� DC
	virtual BOOL Blit2DC(HDC hDC,int dx,int dy,int x,int y,int w,int h) ;
	virtual BOOL DrawBox(RECT& rc, WPixel clr, DWORD dwStyle = BLIT_NORMAL, int alpha = 255);
	virtual BOOL DrawLine(POINT& ptFrom, POINT& ptTo, WPixel clr, DWORD dwStyle = BLIT_NORMAL, int alpha = 255);

	//����
	virtual WPixel* operator[](int y) const;																					//����һ��ɨ����ָ��
	virtual void* GetBmpStruct() const;																							//��������
	virtual BOOL Load(void *bmpdata) ;																							//������������λͼ
	virtual BOOL Save(const char *filename) const ;																				//����λͼ
	virtual void SetUserData(DWORD userdata) ;																					//�����û�����
	virtual DWORD GetUserData() const ;																							//�õ��û�����
	virtual BOOL IsValid() const;
	virtual void Release() ;
	virtual void SetData(void* pBmp);
	virtual BOOL Scroll(int x, int y);
	virtual BOOL IsInEqualOut() {return m_bInEqualOut;}
	virtual void SetInEqualOut(BOOL b) {m_bInEqualOut = b;}

	virtual BOOL DrawText(ITTFont*, int x, int y, const char* str, WPixel color, DWORD style=FONT_ALPHA);
	virtual BOOL DrawTextEx(ITTFont*, int x, int y, const char* str, WPixel color, WPixel outlinecolor, DWORD style=FONT_ALPHA);
	virtual BOOL IsTransparent(POINT& ptSelect);
	virtual IBitmap* SubTile(POINT ptLeftTop);
	virtual BOOL LoadEx(void* customdata){return TRUE;}
	virtual DWORD GetSize();																									//�õ�ͼƬ��ռ�ڴ�Ĵ�С
	virtual BOOL DrawPixel(int x, int y, WPixel clr, DWORD dwStyle = BLIT_NORMAL, int alpha = 255);
	virtual BOOL DrawText(int x, int y, const char* str, WPixel color, DWORD style=FONT_ALPHA);									//�����������
	virtual BOOL DrawClipText(ITTFont* pfont, int x, int y, const char* str, WPixel color, RECT rcClip, DWORD style=FONT_ALPHA);//���ü�����������.���TTF����	
	virtual BOOL Draw(ICanvas* pCanvas, int dx, int dy, DWORD dwStyle = BLIT_NORMAL);											//��IBitmap����Canvas
	virtual BOOL Draw(ICanvas* pCanvas, int dx, int dy, int w, int h, int sx, int sy, DWORD dwStyle = BLIT_NORMAL);
};
//------------------------------------------------------------------------
