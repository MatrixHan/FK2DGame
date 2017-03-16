/**
*	created:		2012-6-20   18:50
*	filename: 		WindowRleBitmap
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../IFKCanvas.h"
#include "WindowBitmap.h"
//------------------------------------------------------------------------
class WRleBitmap : public WBitmap, public IRleBitmap
{
public:
	WRleBitmap();
	virtual ~WRleBitmap();
public:
	//ʵ��IRleBitmap
	//ʵ��IBitmap
	//��ʼ������
	virtual BOOL Create(int nWidth, int nHeight) ;																			//����λͼ
	virtual BOOL Create(int nWidth, int nHeight, void* ptr) ;																//��������������λͼ
	virtual BOOL Create(IBitmap* parent, int x, int y, int nWidth, int nHeight);											//�Ӹ�λͼ�ϴ�������
	virtual IBitmap* SubBitmap(int x, int y, int nWidth, int nHeight) ;														//������λͼ
	virtual int GetWidth() const ;																							//��ÿ��
	virtual int GetHeight() const ;																							//��ø߶�

	//��ͼ���
	virtual BOOL Clear(WPixel clr, RECT* pRc = NULL) ;																			//����
	virtual BOOL Draw(int x, int y, IBitmap* bmp, DWORD dwStyle = BLIT_NORMAL) ;												//�ѱ��λͼ���Ƶ���λͼ
	virtual void Draw(int x, int y, const char* s, WPixel clr) ;																//��ӡASCII�ַ�
	virtual BOOL Draw(int dx, int dy, int w, int h, IBitmap* bmp, int sx, int sy, DWORD dwStyle = BLIT_NORMAL);
	virtual BOOL DrawAlpha(int dx, int dy, IAlphaBitmap* bmp, DWORD dwStyle = BLIT_COPY);										//�ѱ��λͼ���Ƶ���λͼ
	virtual BOOL DrawAlpha(int dx, int dy, int w, int h, IAlphaBitmap* bmp, int sx, int sy, DWORD dwStyle = BLIT_COPY);			//��ǿ��AlphaDraw
	virtual BOOL DrawRle(int dx, int dy, IRleBitmap* bmp, DWORD dwStyle = BLIT_COPY);											//�ѱ��rleλͼ���Ƶ���λͼ
	virtual BOOL DrawRle(int dx, int dy, int w, int h, IRleBitmap* bmp, int sx, int sy, DWORD dwStyle = BLIT_COPY);				//��ǿ��rle����
	virtual BOOL DrawRle(int dx, int dy, IRleBitmap* bmp, BYTE* pPal, DWORD dwStyle = BLIT_COPY);								//�ѱ��rleλͼ���Ƶ���λͼ
	virtual BOOL DrawRle(int dx, int dy, int w, int h, IRleBitmap* bmp, BYTE* pPal, int sx, int sy, DWORD dwStyle = BLIT_COPY); //��ǿ��rle����
	
	virtual BOOL Blit2DC(HDC hDC,int dx,int dy,int x,int y,int w,int h)  ;														//BLIT һ�����β��ֵ� DC
	virtual BOOL DrawBox(RECT& rc, WPixel clr, DWORD dwStyle = BLIT_NORMAL, int alpha = 255);
	virtual BOOL DrawLine(POINT& ptFrom, POINT& ptTo, WPixel clr, DWORD dwStyle = BLIT_NORMAL, int alpha = 255);
	//����
	virtual WPixel* operator[](int y)  const;						//����һ��ɨ����ָ��
	virtual void* GetBmpStruct() const {return m_pBmpStruct;}		//��������
	virtual BOOL Load(void *bmpdata) ;								//������������λͼ
	virtual BOOL Save(const char *filename) const ;					//����λͼ
	virtual void SetUserData(DWORD userdata) ;						//�����û�����
	virtual DWORD GetUserData() const ;								//�õ��û�����
	virtual BOOL IsValid() const;
	virtual void Release() ;

	virtual void SetData(void* pBmp){m_pBmpStruct = pBmp;}
	virtual BOOL Scroll(int x, int y);

	virtual BOOL DrawText(ITTFont*, int x, int y, const char* str, WPixel color, DWORD style=FONT_ALPHA);
	virtual BOOL DrawTextEx(ITTFont*, int x, int y, const char* str, WPixel color, WPixel outlinecolor, DWORD style=FONT_ALPHA);
	virtual BOOL IsTransparent(POINT& ptSelect);
	virtual IBitmap* SubTile(POINT ptLeftTop);
	virtual BOOL LoadEx(void* customdata);
	virtual DWORD GetSize();																			//�õ�ͼƬ��ռ�ڴ�Ĵ�С
	virtual BOOL DrawPixel(int x, int y, WPixel clr, DWORD dwStyle = BLIT_NORMAL, int alpha = 255);
	virtual BOOL DrawText(int x, int y, const char* str, WPixel color, DWORD style=FONT_ALPHA);			//�����������
	
	// ��IBitmap����Canvas
	virtual BOOL Draw(ICanvas* pCanvas, int dx, int dy, DWORD dwStyle = BLIT_NORMAL);
	virtual BOOL Draw(ICanvas* pCanvas, int dx, int dy, int w, int h, int sx, int sy, DWORD dwStyle = BLIT_NORMAL);
	
	// IRleBitmap
	virtual int GetUseColor();							// �õ���ɫ��ʹ�õ���ɫ��
	virtual BYTE* GetRleData();							// �õ�rle����
	virtual void SetRleData(BYTE* pData, int nSize);	// ����rle����
	virtual BYTE* GetPalette();							// �õ���ɫ��
	virtual void SetPalette(BYTE* pPal, int nSize);		// �����µĵ�ɫ��
	virtual BOOL HaveAlphaChannel();					// �Ƿ����alphaͨ��?
	virtual BOOL IsHiColor();							// �Ƿ���16λRLE
protected:
	void Destroy();
};
//------------------------------------------------------------------------