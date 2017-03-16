/**
*	created:		2012-7-1   19:51
*	filename: 		Structs
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�����õ������ݽṹ
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#define SHADOW_ALPHA		180
//------------------------------------------------------------------------
// ������ (support bmp files and tga files)
enum PackType{
	ptUnknow=0,					// unknow
	ptSingleFramePrivatePalette,// a frame, private palette
	ptSingleFramePublicPalette,	// a frame, public palette
	ptMultiFramePrivatePalette,	// many frames, private palette
	ptMultiFramePublicPalette,	// many frames, public palette
	ptMultiStatePrivatePalette, // many states, many frames per state, private palette
	ptMultiStatePublicPalette,	// many states, many frames per state, public palette

	ptMaxPackType,
};
//------------------------------------------------------------------------
#include <pshpack1.h>
//------------------------------------------------------------------------
// RLE����ͷ
typedef struct __RLEHEADER // 20 bytes
{
	WORD wFlags;		// RLEѹ�����ݱ�ʶ(GF) (Graphic Flag? Gao Feng?)
	DWORD dwTotalSize;	// ���ṹ�����ɫ������ݵ�������С
	DWORD dwWidth;		// RLEͼƬ�Ŀ�
	DWORD dwHeight;		// RLEͼƬ�ĸ�
	BYTE nUseColor;		// ��ͼƬʹ�õĵ�ɫ�����ɫ��(�涨���255����ɫ,idx=255�ĵ㿴��͸����)
	BYTE bHaveAlphaChannel;// �Ƿ���alphaͨ������
	DWORD dwDataOffset;	// ͼƬ���ݵ�ƫ����
	__RLEHEADER(){memset(this, 0, sizeof(__RLEHEADER));wFlags='FG';}
	BOOL HavePalette(){return nUseColor!=0;}
	BOOL IsValid(){
		return (wFlags=='FG' && dwTotalSize>0 && dwWidth>0 && 
			dwHeight>0 && dwDataOffset>=sizeof(__RLEHEADER));}
} RLEHEADER;
//------------------------------------------------------------------------
// ���ļ�ͷ
typedef struct PACKFILEHEADER // 16 bytes
{
	unsigned int	identify;		// ��ʶ
	unsigned int	data_offset;	// �������ļ��е�ƫ��
	unsigned char	pack_type;		// ������
	unsigned char	color_depth;	// ɫ�����(8?15?16?24?32?)
	unsigned short	version;		// �汾(���ֽ�Ϊ���汾�����ֽ�Ϊ���汾)
	unsigned short	reserved1;		// ����1
	unsigned short	reserved2;		// ����2

	PACKFILEHEADER() {memset(this, 0, sizeof(PACKFILEHEADER)),identify='KAEP',version=(1<<8);}
	BOOL IsValid(){return (version==(1<<8) && pack_type<ptMaxPackType && identify=='KAEP');}
} stPackFileHeader;
//------------------------------------------------------------------------
// ��������Ϣͷ(�������ɫ�壬��������ṹ����)
typedef struct ANIINFOHEADER // 12 bytes
{
	unsigned short	struct_size;	// struct's size(maybe include palette, if have pal, follow this struct)
	unsigned short	use_color;		// palette use color count
	unsigned int	total_size;		// total frames size
	unsigned int	total_frame;	// total frame count 
	//unsigned char	pal[1];			// palette pointer
} stAniInfoHeader;
//------------------------------------------------------------------------
// ��������ÿһ֡���������
typedef struct FRAMEINDEX // 16 bytes
{
	unsigned int	data_offset;	// bits data offset in the data zone(�ڶ������������е�ƫ��)
	unsigned int	data_size;		// frame size
	unsigned short	clip_offx;		// the x-coord offset with clipped picture and original picture
	unsigned short	clip_offy;		// the y-coord offset with clipped picture and original picture
	unsigned short	width;			// frame width
	unsigned short	height;			// frame height
} stFrameIndex;
//------------------------------------------------------------------------
// ����Ϣͷ(�������ɫ�壬��������ṹ����)
typedef struct GROUPINFOHEADER // 16 bytes
{
	unsigned short	struct_size;	// struct's size(maybe include palette)
	unsigned short	use_color;		// palette use color count
	unsigned int	total_size;		// all frames size
	unsigned int	total_ani;		// all ani count 
	unsigned int	total_frame;	// all frame count 
	//unsigned char	pal[1];			// palette pointer
} stGroupInfoHeader;
//------------------------------------------------------------------------
// ����������
typedef struct ANIINDEX // 12 bytes
{
	unsigned int	ani_frames;		// ani's frame count
	unsigned int	key_frame;		// ���������ĵ�1֡�ڶ�����������֡�е���ţ���λ֡��
	unsigned int	ani_offset;		// ���������������������е�ƫ��λ�ã���λ�ֽڣ�
} stAniIndex;
//------------------------------------------------------------------------
#include <poppack.h>
//------------------------------------------------------------------------