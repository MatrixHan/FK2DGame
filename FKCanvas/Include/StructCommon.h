/**
*	created:		2012-6-19   13:05
*	filename: 		StructCommon
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../IFKCanvas.h"
//------------------------------------------------------------------------
#pragma pack (push)
#pragma pack (1)
//------------------------------------------------------------------------
struct TGAFILEHEADER{
	BYTE bIDLength;						//������Ϣ����
	BYTE bPalType;						//��ɫ����Ϣ(��֧��)
	BYTE bImageType;					//ͼ������(ֻ֧�� 3,10)
	WORD wPalIndex;						//��ɫ���һ������ֵ
	WORD wPalLength;					//��ɫ��������
	BYTE bPalBits;						//һ����ɫ�嵥λλ��(15,16,24,32)
	WORD wLeft;							//ͼ���������(��������)
	WORD wBottom;						//ͼ��׶�����(��������)
	WORD wWidth;						//ͼ����
	WORD wHeight;						//ͼ��߶�
	BYTE bBits;							//һ������λ��
	BYTE bAlphaBits:4;					//Alphaλ��
	BYTE bReserved:1;					//����,����Ϊ0
	BYTE bTopDown:1;					//Ϊ1��ʾ���ϵ���(ֻ֧�ִ��µ���)
	BYTE bInterleaving:2;				//����(һ��Ϊ0)
};
//------------------------------------------------------------------------
typedef struct __RLEHEADER				// 20 bytes
{
	WORD wFlags;						// RLEѹ�����ݱ�ʶ(GF) (Graphic Flag? Gao Feng?)
	DWORD dwTotalSize;					// ���ṹ�����ɫ������ݵ�������С
	DWORD dwWidth;						// RLEͼƬ�Ŀ�
	DWORD dwHeight;						// RLEͼƬ�ĸ�
	BYTE nUseColor;						// ��ͼƬʹ�õĵ�ɫ�����ɫ��(�涨���255����ɫ,idx=255�ĵ㿴��͸����)
	BYTE bHaveAlphaChannel;				// �Ƿ���alphaͨ������
	DWORD dwDataOffset;					// ͼƬ���ݵ�ƫ����
	__RLEHEADER(){memset(this, 0, sizeof(__RLEHEADER));wFlags='FG';}
	BOOL HavePalette(){return (nUseColor!=0) && (dwDataOffset!=sizeof(__RLEHEADER));}
	BOOL IsHiColor() {return (nUseColor==16) && (dwDataOffset==sizeof(__RLEHEADER));}
	BOOL IsValid(){
		return (wFlags=='FG' && dwTotalSize>0 && dwWidth>0 && 
			dwHeight>0 && dwDataOffset>=sizeof(__RLEHEADER));}
} RLEHEADER;
//------------------------------------------------------------------------
struct WBmpStruct
{
	int kx;					//��ʼ����
	int ky;					//��ʼ����
	int w;					//���
	int h;					//�߶�
	int pitch;				//�п��ֽ�
	void* ptr;				// ��һɨ����ָ��
	DWORD userdata;			//�û�����
	BOOL bChild;			//�Ƿ�Ϊ��λͼ

	WBmpStruct(){Empty();}
	WBmpStruct(const WBmpStruct&);
	~WBmpStruct();
	WBmpStruct& operator=(const WBmpStruct& bmp);
	void Empty() {bChild = FALSE; kx = ky = 0, w = h = 0, ptr = 0; userdata = 0; pitch = 0;} //��սṹ
};
//------------------------------------------------------------------------
struct WAlphaBmpStruct : public WBmpStruct
{
	BYTE* channel;			//Alphaͨ��
	void* cmem;				//Alpha ͨ���ڴ��
	WAlphaBmpStruct(){Empty();}
	~WAlphaBmpStruct(){if(!bChild) delete []cmem; cmem = 0;}
	void Empty(){ bChild = FALSE;w = h = 0; ptr = 0, cmem = 0;}
};
//------------------------------------------------------------------------
struct WRleBmpStruct
{
	RLEHEADER rleheader;
	void* ptr;
	BYTE* pal;
	DWORD userdata;
	WRleBmpStruct(){memset(this, 0, sizeof(WRleBmpStruct));}
	void Clear()
	{
		if (ptr) {delete[] ptr;ptr=0;}
		if (pal && rleheader.HavePalette()) {delete[] pal;pal=0;}
	}
};
//------------------------------------------------------------------------
struct WFontData
{
	int w;				//���
	int h;				//�߶�
	int width;			//�ֿ�
	int kx;				//X ƫ��
	int ky;				//Y ƫ��
	int size;			//����
	BYTE* data;			//ʵ������
};
//------------------------------------------------------------------------
struct WCharStruct
{
	int kx;
	int ky;
	int w;				// ���
	int h;				// �߶�
	BYTE *data;			// ��һɨ����ָ��
	WPixel color;		// ��ɫ
	WPixel outline;		// ��������ɫ
	short int width;	// �ֿ�(���ּ��)

	WCharStruct(const WCharStruct& c){ w=c.w, h = c.h, data = c.data, color = c.color; outline = c.outline;}
	WCharStruct(){kx = ky = w = h = 0;
	data = NULL;
	color = outline = 0;
	width = 0;}
};
//------------------------------------------------------------------------
typedef DWORD (_stdcall *BLIT_FUNC)(DWORD,DWORD);
typedef unsigned long WSERR;
#define WS_OK 0
//------------------------------------------------------------------------
struct WBoxStruct
{
	int kx;
	int ky;
	int w;
	int h;
	int userdata;
	WPixel color;
};
//------------------------------------------------------------------------
struct WLineStruct
{
	int kx;
	int ky;
	int x;									// Xƫ��
	int y;									// Yƫ��
	bool positive;
	WPixel color;							// ��ɫ
	int userdata;							// �û�����
};
//------------------------------------------------------------------------
#pragma pack (pop)
//------------------------------------------------------------------------