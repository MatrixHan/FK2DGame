/**
*	created:		2012-6-19   21:24
*	filename: 		WindowFont
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
/*
** ˼·����ȡ���漼����������
** ʵ�֣���Ҫ����һ���ַ�ʱ�����ڻ����������(���ֲ���)���ҵ�ֱ�ӻ��ƣ�
		�Ҳ��������(�����)һ���ַ����棬��ͨ��TextOut�����ַ�������
		�ڴ��У�Ȼ�������Ҫ(�������)����һ���ַ�������ӳ���һ����
		���Ļ�������������ˡ��ַ��ڽ���MMX����ʱ����Ҫ��������ַ���
		ӳ���
	
** �޸ļ�¼��
	1.ԭ��ÿ���ַ���ӳ�����ʵʱnew�����ģ����ڸ�Ϊ�ڴ�������ʱһ���Դ�����
	2.ԭ����֧�ֻ������ֵ������ߣ����ڼ�����֧�֡�
*/
//------------------------------------------------------------------------
#include "../../FKTrace/IFKTrace.h"
#include "../IFKCanvas.h"
#include "../Include/StructCommon.h"
#include "../Include/WindowChar.h"
#include "../Include/WindowFont.h"
#include "../asm/asm.h"
//------------------------------------------------------------------------
// ���ٻ���
void XFont::Destroy()
{
	if (m_nCacheSize == 0)
		return;

	delete[] m_CharBuf;
	delete[] m_Data;
	delete[] m_FontData;

	DeleteObject(m_FontBmp);
	DeleteDC(m_FontDC);
	m_nCacheNum = m_nCacheSize = 0;
	m_nFreq = 0;
}
//------------------------------------------------------------------------
// ��չ����Ĵ�С
void XFont::ExpandCacheSize(int newsize)
{
	Data* newbuf;
	WFontData* new_font_buf;
	newbuf = new Data[newsize];
	new_font_buf = new WFontData[newsize];
	memcpy(newbuf, m_Data, m_nCacheSize*sizeof(Data));
	memcpy(new_font_buf, m_FontData, m_nCacheSize*sizeof(WFontData));

	TEXTMETRIC tm;
	GetTextMetrics(m_FontDC, &tm);
	int nCharSize;
	if (m_bOutline)
		nCharSize = (tm.tmHeight+2)*(tm.tmMaxCharWidth+2);
	else
		nCharSize = tm.tmHeight*tm.tmMaxCharWidth;
	nCharSize = (nCharSize + 3) & -4; // 4�ֽڶ���,���ڿ��ٶ�λ
	int nAllCacheSize = nCharSize*newsize;
	BYTE* newCharBuf = new BYTE[nAllCacheSize];
	memcpy(newCharBuf, m_CharBuf, m_nCacheSize*nCharSize);
	m_FontData->data = newCharBuf;
	for (int i=m_nCacheSize; i<newsize; i++)
		m_FontData[i].data = m_FontData[i-1].data + nCharSize;

	delete[] m_Data;
	delete[] m_FontData;
	delete[] m_CharBuf;

	m_nCacheSize = newsize;
	m_Data = newbuf;
	m_FontData = new_font_buf;
	m_CharBuf = newCharBuf;
}
//------------------------------------------------------------------------
// �ڻ���������һ���ַ�����������ڷ���<0��ֵ
// ��������
int XFont::SearchChar(unsigned int c)const
{
	int begin, end, middle;
	unsigned int Char;
	begin = 0;
	end = m_nCacheNum - 1;
	while (begin <= end)
	{
		middle = (begin + end)/2;
		Char = m_Data[middle].c;
		if (c == Char)
			return middle;
		else if (c < Char)
			end = middle - 1;
		else
			begin = middle + 1;
	}
	return -begin-1;
}
//------------------------------------------------------------------------
// �õ��ַ��ڻ����е�λ��
int XFont::GetCachePos(unsigned int c, int pos)
{
	int nPos = 0;
	if (m_nCacheNum >= m_nCacheSize) // Cache��,ɾ�������Cache��
	{
		int min = m_nFreq + 1, min_idx = -1;
		
		// ����freq��С���Ǹ�id
		for (int i=0; i<m_nCacheNum; i++)
		{
			if (m_Data[i].freq == 1)
			{
				min = 1;
				min_idx = i;
				break;
			}
			if (m_Data[i].freq < min)
			{
				min = m_Data[i].freq;
				min_idx = i;
			}
		}
		
		if (m_Locked != 0 && min >= m_Locked)
		{
			if (m_nCacheSize < 1024)
				ExpandCacheSize(m_nCacheSize*2);
			else
				ExpandCacheSize(m_nCacheSize + 1024);
			
			for (int i=m_nCacheNum; i>pos; i--)
				m_Data[i] = m_Data[i-1];
			
			nPos = m_nCacheNum;
			m_Data[pos].freq = m_nFreq;
			m_Data[pos].c = c;
			m_Data[pos].idx = m_nCacheNum;
			++ m_nCacheNum;
		}
		else // ɾ���������Cache�ģ��������µ�
		{
			int idx = m_Data[min_idx].idx;
			nPos = idx;
			
			if (pos == m_nCacheNum)	// ������һ��
				-- pos;
			
			if (pos > min_idx)	// ����λ����ɾ����ĺ���
			{
				for (int i=min_idx; i<pos; i++)
					m_Data[i] = m_Data[i+1];
				m_Data[pos].freq = m_nFreq;
				m_Data[pos].c = c;
				m_Data[pos].idx = idx;
			}
			else// ����λ����ɾ�����ǰ���������ɾ����
			{
				for (int i=min_idx; i>pos; i--)
					m_Data[i] = m_Data[i-1];
				m_Data[pos].freq = m_nFreq;
				m_Data[pos].c = c;
				m_Data[pos].idx = idx;
			}
			
			// �������ݵ�freq�����е���
			m_nFreq -= min;
			if (m_Locked > 0)
				m_Locked -= min;
			for (int i=0; i<m_nCacheNum; i++)
				m_Data[i].freq -= min;
		}
	}
	else // Cache����,���
	{
		nPos = m_nCacheNum;
		for (int i=m_nCacheNum; i>pos; i--)
			m_Data[i] = m_Data[i-1];
		m_Data[pos].freq = m_nFreq;
		m_Data[pos].c = c;
		m_Data[pos].idx = m_nCacheNum;
		++ m_nCacheNum;
	}
	return nPos;
}
//------------------------------------------------------------------------
// �õ�һ���ַ����ݣ����������棬����Ҳ������´���һ��
void* XFont::GetChar(unsigned int c)
{
	int i, j, pos;
	WFontData* temp;
	if (m_nCacheSize == 0)
		return 0;
	++ m_nFreq;

	// ���ڻ�������
	pos = SearchChar(c);

	// �ҵ�!
	if (pos >= 0)
	{
		m_Data[pos].freq = m_nFreq;
		return &m_FontData[m_Data[pos].idx];
	}

	// δ�ҵ�
	pos = -pos - 1;

	int nPos = GetCachePos(c, pos);
	temp = &m_FontData[nPos];
	
	// �����ַ����ݵ� temp->data;
	// �� DC ȡ (�޷�����)
	static RECT rect;
	static char str[3] = {0, 0, 0};
	SIZE si;

	rect.top = 0;
	rect.left = 0;
	rect.right = m_MaxWidth;
	rect.bottom = m_Height;

	// �����ַ����õ��ַ���С
	if (c < 256) // ���ֽ��ַ�
	{
		str[0] = c;
		str[1] = 0;
		TextOut(m_FontDC, 0, 0, str, 1); // ����������ٶ��� DrawText ��6��
		GetTextExtentPoint32(m_FontDC, str, 1, &si);
	}
	else // ˫�ֽ��ַ�
	{
		str[0] = c >> 8;
		str[1] = c & 0xff;
		TextOut(m_FontDC, 0, 0, str, 2);
		GetTextExtentPoint32(m_FontDC, str, 2, &si);
	}

	// ����ַ��ϲ����ж��ٿհ���
	WORD* line = m_Buffer;
	int from;
	for (i=0; i<si.cy; i++,line+=m_MaxWidth)
	{
		for (j=0; j<si.cx; j++)
			if (line[j] != 0)
				goto _find;
	}
_find:
		from = i;
		si.cy -= from;
		int ox = si.cx;
		int oy = si.cy;
		if (m_bOutline) // ��������
		{
			si.cx += 2;	
			si.cy += 2;
		}
		if (si.cx > m_MaxWidth)
			si.cx = m_MaxWidth;
		if (si.cy > m_Height)
			si.cy = m_Height;
		temp->w = si.cx;
		temp->h = si.cy;
		temp->width = si.cx;
		temp->kx = 0;
		temp->ky = -from;
		temp->size = si.cx*si.cy;
		line = m_Buffer + from*m_MaxWidth;
		BYTE* des = temp->data;

		// �ַ���0x10��ʾ,������0xFF��ʾ
		if (m_bOutline)
		{
			memset(temp->data, 0, temp->size);
			line = m_Buffer + from*m_MaxWidth;
			des = temp->data;
			des += si.cx;	// ��������
			for (i=0; i<oy; i++,line+=m_MaxWidth,des+=si.cx)
			{
				for (j=0; j<ox; j++)
				{
					if (line[j] != 0) // �����ַ�������
					{
						int k = j + 1;	// ��������
						des[k] = 0x10;
						
						// left-top
						if (des[k-si.cx-1] == 0)
							des[k-si.cx-1] = 0xFF;
						// top
						if (des[k-si.cx] == 0)
							des[k-si.cx] = 0xFF;
						// right-top
						if (des[k-si.cx+1] == 0)
							des[k-si.cx+1] = 0xFF;
						// left
						if (des[k-1] == 0)
							des[k-1] = 0xFF;
						// right
						if (des[k+1] == 0)
							des[k+1] = 0xFF;
						// left-bottom
						if (des[k+si.cx-1] == 0)
							des[k+si.cx-1] = 0xFF;
						// bottom
						if (des[k+si.cx] == 0)
							des[k+si.cx] = 0xFF;
						// right-bottom
						if (des[k+si.cx+1] == 0)
							des[k+si.cx+1] = 0xFF;
					}
				}
			}
		}
		else
		{
			for (i=0; i<oy; i++,line+=m_MaxWidth,des+=si.cx)
			{
				for (j=0; j<ox; j++)
					des[j] = (line[j] == 0) ? 0 : 0x10;
				
				for (; j<si.cx; j++)
					des[j] = 0;
			}
		}

	return temp;
}
//------------------------------------------------------------------------
// ���崴��(��ʼ��)
// Ŀǰ��Ϸû��ʹ�ô�alpha�����壬����alpha��������ָ���Ƿ������(alpha>0)
// cachesize: �����С��һ�����������������������Ļ�ϻ��Ƶ����ִ���Ƕ��٣���ô
//            �Ϳ��Խ������С��Ϊ���٣�Ϊ�˲�Ӱ�����ܣ�һ�������Ϊ128~256֮��
BOOL XFont::Create(HFONT font, int alpha /* = 16 */, int cachesize /* = 128 */)
{
	if (alpha > 1) alpha = 1;
	if (alpha < 0) alpha = 0;
	if (cachesize <= 0) cachesize = 128;
	int BMPINFO16[13] =
	{
		sizeof(BITMAPINFOHEADER),
			0,	// ���
			0,	// �߶�
			0x100001,BI_BITFIELDS,0,0,0,0,0,
			0xf800,0x7e0,0x1f
	};

	if (m_nCacheSize != 0)
		Destroy();

	// ����һ������DC
	m_FontDC = CreateCompatibleDC(NULL);
	if (font != 0)
		SelectObject(m_FontDC, font);

	// ��ȡ���ֵĳߴ�
	SIZE si;
	TEXTMETRIC tm;
	GetTextMetrics(m_FontDC, &tm);
	// �����е�Ӣ�������ڻ��ƺ���ʱ��ʵ���ǵ��õ�ϵͳĬ�ϵĺ������壬
	// ���Ե���tm.tmMaxCharWidth < Ĭ�Ϻ�������Ŀ�ȣ��ʴ˴���������
	GetTextExtentPoint32(m_FontDC, "��", 2, &si);
	m_MaxWidth = (tm.tmMaxCharWidth < si.cx) ? si.cx : tm.tmMaxCharWidth;
	m_Height = (tm.tmHeight < si.cy) ? si.cy : tm.tmHeight;
	m_bOutline = alpha;
	if (m_bOutline)	// ��������
	{
		m_Height += 2;
		m_MaxWidth += 2;
	}
	int nCharSize = m_MaxWidth*m_Height; // һ���ַ���ӳ���Ĵ�С
	nCharSize = (nCharSize + 3) & -4; // 4�ֽڶ���,���ڿ��ٶ�λ
	m_MaxWidth = (m_MaxWidth + 3) & -4; // 4���ض���

	// ��������
	m_nCacheNum = 0;
	m_Locked = 0;
	m_nCacheSize = cachesize;
	m_Data = new Data[m_nCacheSize];
	m_FontData = new WFontData[m_nCacheSize];

	// ���������ַ������ݻ���
	int nAllCacheSize = nCharSize*cachesize;
	m_CharBuf = new BYTE[nAllCacheSize];
	m_FontData->data = m_CharBuf;
	for (int i=1; i<m_nCacheSize; i++)
		m_FontData[i].data = m_FontData[i-1].data + nCharSize;
	if (m_Data == 0 || m_FontData == 0 || m_CharBuf == 0)
		return FALSE;

	// ����һ��Сλͼ���ڻ����ַ�(�ڱ�������ɫ����)
	BMPINFO16[1] = m_MaxWidth;
	BMPINFO16[2] = -m_Height;
	m_FontBmp = CreateDIBSection(m_FontDC, (BITMAPINFO*)&(BMPINFO16[0]),
		DIB_RGB_COLORS, (void**)&m_Buffer, 0, 0);
	SelectObject(m_FontDC, m_FontBmp);
	SetTextColor(m_FontDC, 0xffffff);
	SetBkColor(m_FontDC, 0);
	
	return TRUE;
}
//------------------------------------------------------------------------
// ��ȡ�ַ��������ؿ��
DWORD XFont::GetLength(const char* str)
{
	DWORD len = 0;

	for (int i=0; str[i]; i++)
	{
		unsigned int a, b;
		a = (unsigned char)str[i];
		if (a > 0x80) // ˫�ֽ�����
		{
			++i;
			b = (unsigned char)str[i];
			if (b == 0)
				break;
			a = a*256 + b;
		}

		len += GetCharLength(a);
	}
	
	return len;
}
//------------------------------------------------------------------------
// ��ȡ�ַ������ؿ��
DWORD XFont::GetCharLength(const WORD c)
{
	WChar tmp;
	tmp.Create((ITTFont*)this, c, 0);
	return tmp.GetWidth();
}
//------------------------------------------------------------------------
// ��������ӿ�
extern "C" __declspec(dllexport) 
BOOL CreateITTFont(DWORD dwVersion, ITTFont** ppITTFont, ITrace* pTrace)
{
#ifdef _DEBUG
	if (pTrace)
		g_DefaultTrace.Create(pTrace);
#endif
	XFont* pFont = new XFont;
	if (!pFont)
		return FALSE;
	*ppITTFont = static_cast<ITTFont*>(pFont);
	return *ppITTFont != NULL;
}
//------------------------------------------------------------------------