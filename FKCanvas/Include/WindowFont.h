/**
*	created:		2012-6-19   21:14
*	filename: 		WindowFont
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../IFKCanvas.h"
//------------------------------------------------------------------------
class XFont  : public ITTFont  
{
	struct Data{
		unsigned int c;
		int freq;
		unsigned short idx;
	};
private:
	Data* m_Data;							// �����ַ����ݵ�˳���б�
	int m_nCacheSize;						// �����С
	int m_nCacheNum;						// �Ѿ��������Ŀ
	WFontData* m_FontData;					// ������ַ�����
	BYTE*	m_CharBuf;						// �ַ����ݻ���
	HDC m_FontDC;							// �������DC
	HBITMAP m_FontBmp;						// alpha=0ʱ������λͼ
	WORD* m_Buffer;							// �ֻ���
	int m_Height;							// ����߶�
	int m_Locked;							// 
	int m_nFreq;							// Ƶ��
	int m_bOutline;							// �Ƿ��������
	int m_MaxWidth;							// �����ַ���λͼ�������
private:
	int SearchChar(unsigned int c) const;	// �� Cache ������һ���ַ�
	void ExpandCacheSize(int newsize);		// ��չ CacheSize
	void Destroy();
public:
	XFont() : m_nCacheSize(0), m_Height(0), m_FontDC(0), m_bOutline(0), m_nFreq(0), m_CharBuf(0) {}
	virtual ~XFont()			{Destroy();}
	
	virtual BOOL Create(HFONT font, int alpha = 16, int cachesize = 128);
	virtual void* GetChar(unsigned int c);
	virtual void Release()		{delete this;}
	virtual DWORD GetLength(const char* str);
	virtual DWORD GetCharLength(const WORD c);
	virtual int GetHeight() const	{return m_Height;}
	virtual void Lock()			{m_Locked = m_nFreq + 1;}
	virtual void UnLock()		{m_Locked = 0;}
	BOOL HasOutline()			{return m_bOutline;}
	int GetCachePos(unsigned int c, int pos);
};
//------------------------------------------------------------------------