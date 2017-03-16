/**
*	created:		2012-6-23   21:37
*	filename: 		ImageLoader
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		imageͼƬ���������������ͻ��棬��֧��bmp��tga,jpg
*/
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
#include "../../../Depend/ijl/ijl.h"
#include "../Include/FKWndSysCommon.h"
#include "../../FKFileSystem/IFKFileSystem.h"
//------------------------------------------------------------------------
#pragma comment (lib, "../../Depend/ijl/ijl15.lib")
//------------------------------------------------------------------------
CImageLoader* CImageLoader::_instance = NULL;
//------------------------------------------------------------------------
CImageLoader::CImageLoader()
{
	m_vImageList.clear();
	m_pFS = NULL;
	CImageLoader::_instance = this;			// ���ֱ�Ӷ���������ȫ�ֱ���������Ҫnew����
}
//------------------------------------------------------------------------
CImageLoader::~CImageLoader()
{
	Clear();
}
//------------------------------------------------------------------------
// ���ͼƬ������ʵ��(ʼ�ձ��ֵ�ʵ��)
CImageLoader* CImageLoader::GetInstance()
{
#ifdef _DEBUG
	if (CImageLoader::_instance != NULL)
	{
		static int flag = 0;
		if (flag == 0)
		{
			flag = 1; 
			if (!CImageLoader::_instance->GetFileSystem())
				OutputDebugString("Warning:CImageLoader::m_pFS == NULL!!!\r\n");
		}
	}
#endif
	if (CImageLoader::_instance == NULL)
		CImageLoader::_instance = new CImageLoader;
	return CImageLoader::_instance;
}
//------------------------------------------------------------------------
// �ͷ�ͼƬ������ʵ��
void CImageLoader::ReleaseInstance()
{
	if (CImageLoader::_instance)
	{
		CImageLoader::_instance->Clear();
		CImageLoader* p = CImageLoader::_instance;
		CImageLoader::_instance = NULL;
		delete p;
	}
}
//------------------------------------------------------------------------
// �����Դ
void CImageLoader::Clear()
{
	CacheImageListPtr _end = m_vImageList.end();
	PrintEventLn("CImageLoader::Clear, %d images is not release", m_vImageList.size());
	for (CacheImageListPtr it=m_vImageList.begin(); it!=_end; ++it)
	{
		(*it).second.pImage->Release();
	}
	m_vImageList.clear();
}
//------------------------------------------------------------------------
// ����ĳ��ͼƬ����������д��ڴ�ͼƬ����ֱ�ӷ��ػ����е�ͼƬ��ͬʱ����ͼƬ���ü�����1
// szFileName �ɴ������·���������·��(��֧��"./"��"../"��".\"��"..\"֮������·����
// ����֧��"face.bmp"��"face\face01.bmp"֮������·��)
IBitmap* CImageLoader::LoadImage(const char* szFileName)
{
	if (szFileName == NULL || szFileName[0] == '\0')
		return NULL;

	CacheImageListPtr it = m_vImageList.find(szFileName);
	if (it != m_vImageList.end())
	{
		(*it).second.nRefCount ++;
		return (*it).second.pImage;
	}

	// ������û�и�ͼƬ��������
	stCacheImage cacheimage;
	cacheimage.nRefCount = 0;

	if ((cacheimage.pImage = _loadImage(szFileName)) == NULL)
		return NULL;

	cacheimage.nRefCount = 1;
	m_vImageList[szFileName] = cacheimage;
#ifdef _DEBUG
	if (m_vImageList.size() >= 500)
	{
		OutputDebugString("WARNING: Image count >= 500 !!!!!\r\n");
	}
#endif
	return cacheimage.pImage;
}
//------------------------------------------------------------------------
// ����һ��ͼƬ����������д��ڴ�ͼƬ����ֱ�ӽ���ͼƬ���ü�����1
IBitmap* CImageLoader::CopyImage(IBitmap* pImage)
{
	if (pImage == NULL)
		return NULL;
	CacheImageListPtr _end = m_vImageList.end();
	for (CacheImageListPtr it=m_vImageList.begin(); it!=_end; ++it)
	{
		stCacheImage CacheImage = (*it).second;
		if (CacheImage.pImage == pImage)
		{
			(*it).second.nRefCount++;
			return pImage;
		}
	}
	return NULL;
}
//------------------------------------------------------------------------
// �ͷ�ĳ��ͼƬ����������д��ڴ�ͼƬ����ֱ�ӽ���ͼƬ���ü�����1
// �����������Ϊ0����ɾ�������еĸ�ͼƬ
void CImageLoader::FreeImage(const char* szFileName)
{
	if (szFileName == NULL || szFileName[0] == '\0')
		return;
	
	CacheImageListPtr it = m_vImageList.find(szFileName);
	if (it != m_vImageList.end())
	{
		if (--((*it).second.nRefCount) == 0)
		{
			(*it).second.pImage->Release();
			m_vImageList.erase(it);
#ifdef _DEBUG
			if (m_vImageList.size() <= 0)
			{
				OutputDebugString("WARNING: Image count <= 0 !!!!!\r\n");
			}
#endif
		}
	}
}
//------------------------------------------------------------------------
// �ͷ�ĳ��ͼƬ����������д��ڴ�ͼƬ����ֱ�ӽ���ͼƬ���ü�����1
// �����������Ϊ0����ɾ�������еĸ�ͼƬ
void CImageLoader::FreeImage(IBitmap* pImage)
{
	if (pImage == NULL)
		return;
	CacheImageListPtr _end = m_vImageList.end();
	for (CacheImageListPtr it=m_vImageList.begin(); it!=_end; ++it)
	{
		stCacheImage CacheImage = (*it).second;
		if (CacheImage.pImage == pImage)
		{
			if (--CacheImage.nRefCount == 0)
			{
				CacheImage.pImage->Release();
				m_vImageList.erase(it);
#ifdef _DEBUG
				if (m_vImageList.size() <= 0)
				{
					OutputDebugString("WARNING: Image count <= 0 !!!!!\r\n");
				}
#endif
			}
			break;
		}
	}
}
//------------------------------------------------------------------------
BOOL CImageLoader::SetFileSystem(IFileSystem* pFS)
{
	if (pFS == NULL)
		return FALSE;
	m_pFS = pFS;
	return TRUE;
}
//------------------------------------------------------------------------
// ����һ��������ͼƬ
IBitmap* CImageLoader::_loadImage(const char* szFileName)
{
	if (m_pFS == NULL)
		return NULL;
	// ����ͼƬ����
	int nImageType = -1;
	char szExt[8];
	int nLen = strlen(szFileName);
	strcpy(szExt, &szFileName[nLen-3]);
	if (strcmpi(szExt, "bmp") == 0)
		nImageType = 0;
	else if (strcmpi(szExt, "tga") == 0)
		nImageType = 1;
	else if (strcmpi(szExt, "jpg") == 0)
		nImageType = 2;

	if (nImageType == -1)
		return NULL;

	// ���ļ�
	IBitmap* pImage = NULL;
	IFileObject* pFileObj;
	if ((pFileObj = m_pFS->GetFile(szFileName)) == NULL)
	{
		PrintEventLn("Error: GetFile() : can not get file[%s]", szFileName);
		return NULL;
	}
	if (!pFileObj->Open())
	{
		PrintEventLn("Error: Open() : can not open file[%s]", szFileName);
		return NULL;
	}
	DWORD dwSize = pFileObj->GetLength();
	BYTE* pbuf = new BYTE[dwSize];
	if (pFileObj->Read(pbuf, dwSize) != (UINT)dwSize)
	{
		pFileObj->Release();
		delete[] pbuf;
		PrintEventLn("Error: Read() : read file error[%s]", szFileName);
		return NULL;
	}
	pFileObj->Release();

	// ����ͼƬ����
	if (nImageType == 0)
	{
		if (!m_CanvasHelper.CreateIBitmap(&pImage))
		{
			delete[] pbuf;
			return NULL;
		}
	}
	else if (nImageType == 1)
	{
		IAlphaBitmap* pAlphaImage;
		if (!m_CanvasHelper.CreateIAlphaBitmap(&pAlphaImage))
		{
			delete[] pbuf;
			return NULL;
		}
		pImage = static_cast<IBitmap*>(pAlphaImage);
	}
	else if (nImageType == 2)   
	{
		//�Ƚ�����ͨλͼ
		if (!m_CanvasHelper.CreateIBitmap(&pImage))
		{
			delete[] pbuf;
			return NULL;
		}
		//��JPG���ݽ���
		BYTE* pTempbuf = __DecodeJpeg(pbuf, dwSize);
		if (pTempbuf == NULL)   //����ʧ��
		{  
			delete[] pbuf;
			return NULL;
		}
		delete[] pbuf;      //����ɹ���,����ԭ���Ļ�����
		pbuf = pTempbuf;
	}

	// ����
	if (!pImage->Load(pbuf))
	{
		pImage->Release();
		delete[] pbuf;
		return NULL;
	}

	delete[] pbuf;
	return pImage;
}
//------------------------------------------------------------------------
BYTE * CImageLoader::__DecodeJpeg(BYTE *pData, int nLen)
{
	if (pData == NULL)
	{
		return NULL;
	}

	BYTE *pDIBData = NULL;
	BOOL bIjlInited = FALSE;

	JPEG_CORE_PROPERTIES jcprop;
	IJLERR jerr;	
	try
	{
		jerr = ijlInit(&jcprop);
		if (jerr != IJL_OK)
		{
			return NULL;
		}
		bIjlInited = TRUE;   //ָ���Ѿ���ʼ��
				
		jcprop.JPGBytes = pData;
		jcprop.JPGSizeBytes = nLen;
		jerr = ijlRead(&jcprop, IJL_JBUFF_READPARAMS);
		if (jerr != IJL_OK)
		{		
			ijlFree(&jcprop);
			return NULL;
		}

		int nWidth = jcprop.JPGWidth;
		int nHeight = jcprop.JPGHeight;
		int nChannels = 3;  
		int nBytePerLine = nWidth * nChannels;
		int nBytePerLinePad = IJL_DIB_PAD_BYTES(nWidth, nChannels);
		int nDIBImageSize = (nBytePerLine + nBytePerLinePad) * nHeight;
		pDIBData = new BYTE[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nDIBImageSize];
		if (pDIBData == NULL)
		{
			ijlFree(&jcprop);			
			return NULL;
		}

		BITMAPFILEHEADER filehead;
		memset(&filehead, 0, sizeof(BITMAPFILEHEADER));
		filehead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nDIBImageSize;
		filehead.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		filehead.bfType = 19778;   //��'BM'
		memcpy(pDIBData, &filehead, sizeof(BITMAPFILEHEADER));

		BITMAPINFOHEADER infohead;
		memset(&infohead, 0, sizeof(BITMAPINFOHEADER));
		infohead.biSize = sizeof(BITMAPINFOHEADER);
		infohead.biWidth = nWidth;
		infohead.biHeight = nHeight;
		infohead.biPlanes = 1;
		infohead.biBitCount = 24;
		infohead.biCompression = BI_RGB;
		memcpy(pDIBData + sizeof(BITMAPFILEHEADER), &infohead, sizeof(BITMAPINFOHEADER));

		jcprop.DIBWidth = nWidth;
		jcprop.DIBHeight = nHeight;
		jcprop.DIBChannels = nChannels;
		jcprop.DIBColor = IJL_BGR;
		jcprop.DIBPadBytes = nBytePerLinePad;
		jcprop.DIBBytes = pDIBData + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		switch(jcprop.JPGChannels) {
		case 1:
			jcprop.JPGColor = IJL_G;
			break;
		case 3:
			jcprop.JPGColor = IJL_YCBCR;
			break;
		default:
			jcprop.JPGColor = IJL_OTHER;
			jcprop.DIBColor = IJL_OTHER;
		}

		jerr = ijlRead(&jcprop, IJL_JBUFF_READWHOLEIMAGE);
		if (jerr != IJL_OK)
		{			
			ijlFree(&jcprop);			
			delete[] pDIBData;			
			return NULL;
		}

		ijlFree(&jcprop);	

		int nNumLine = nBytePerLine + nBytePerLinePad;   //ÿ���ֽ���
		BYTE *pTemp = new BYTE[nNumLine];
		if (pTemp)
		{
			BYTE *pDataStart = pDIBData + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
			int nHalfHeight = nHeight / 2;
			for (int i = 0; i < nHalfHeight; ++i)
			{
				BYTE *pSrc1 = pDataStart + i * nNumLine;
				BYTE *pSrc2 = pDataStart + (nHeight - 1 - i) * nNumLine;
				memcpy(pTemp, pSrc1, nNumLine);
				memcpy(pSrc1, pSrc2, nNumLine);
				memcpy(pSrc2, pTemp, nNumLine);
			}
			delete[] pTemp;
		}
		
		return pDIBData;
	}
	catch (...) {
		if (pDIBData)
		{
			delete[] pDIBData;
		}
		if (bIjlInited)
		{
			ijlFree(&jcprop);
		}
		return NULL;
	}
	
	return NULL;
}
//------------------------------------------------------------------------
CImageLoaderEx* CImageLoaderEx::_instance = NULL;
//------------------------------------------------------------------------
CImageLoaderEx::CImageLoaderEx()
{
	m_vImageList.clear();
	m_pFS = NULL;
	CImageLoaderEx::_instance = this;			// ���ֱ�Ӷ���������ȫ�ֱ���������Ҫnew����
}
//------------------------------------------------------------------------
CImageLoaderEx::~CImageLoaderEx()
{
	Clear();
}
//------------------------------------------------------------------------
// ���ͼƬ������ʵ��(ʼ�ձ��ֵ�ʵ��)
CImageLoaderEx* CImageLoaderEx::GetInstance()
{
#ifdef _DEBUG
	if (CImageLoaderEx::_instance != NULL)
	{
		static int flag = 0;
		if (flag == 0)
		{
			flag = 1; 
			if (!CImageLoaderEx::_instance->GetFileSystem())
				OutputDebugString("Warning:CImageLoaderEx::m_pFS == NULL!!!\r\n");
		}
	}
#endif
	if (CImageLoaderEx::_instance == NULL)
		CImageLoaderEx::_instance = new CImageLoaderEx;
	return CImageLoaderEx::_instance;
}
//------------------------------------------------------------------------
// �ͷ�ͼƬ������ʵ��
void CImageLoaderEx::ReleaseInstance()
{
	if (CImageLoaderEx::_instance)
	{
		CImageLoaderEx::_instance->Clear();
		CImageLoaderEx* p = CImageLoaderEx::_instance;
		CImageLoaderEx::_instance = NULL;
		delete p;
	}
}
//------------------------------------------------------------------------
// �����Դ
void CImageLoaderEx::Clear()
{
	CacheImageListPtr _end = m_vImageList.end();
	PrintEventLn("CImageLoaderEx::Clear, %d images is not release", m_vImageList.size());
	for (CacheImageListPtr it=m_vImageList.begin(); it!=_end; ++it)
	{
		(*it).second.pImage->Release();
	}
	m_vImageList.clear();
}
//------------------------------------------------------------------------
// ����ĳ��ͼƬ����������д��ڴ�ͼƬ����ֱ�ӷ��ػ����е�ͼƬ��ͬʱ����ͼƬ���ü�����1
// szFileName �ɴ������·���������·��(��֧��"./"��"../"��".\"��"..\"֮������·����
// ����֧��"face.bmp"��"face\face01.bmp"֮������·��)
IBitmap* CImageLoaderEx::LoadImage(const char* szFileName)
{
	if (szFileName == NULL || szFileName[0] == '\0')
		return NULL;

	CacheImageListPtr it = m_vImageList.find(szFileName);
	if (it != m_vImageList.end())
	{
		(*it).second.nRefCount ++;
		return (*it).second.pImage;
	}

	// ������û�и�ͼƬ��������
	stCacheImage cacheimage;
	cacheimage.nRefCount = 0;

	if ((cacheimage.pImage = _loadImage(szFileName)) == NULL)
		return NULL;

	cacheimage.nRefCount = 1;
	m_vImageList[szFileName] = cacheimage;
#ifdef _DEBUG
	if (m_vImageList.size() >= 500)
	{
		OutputDebugString("WARNING: Image count >= 500 !!!!!\r\n");
	}
#endif
	return cacheimage.pImage;
}
//------------------------------------------------------------------------
// ����һ��ͼƬ����������д��ڴ�ͼƬ����ֱ�ӽ���ͼƬ���ü�����1
IBitmap* CImageLoaderEx::CopyImage(IBitmap* pImage)
{
	if (pImage == NULL)
		return NULL;
	CacheImageListPtr _end = m_vImageList.end();
	for (CacheImageListPtr it=m_vImageList.begin(); it!=_end; ++it)
	{
		stCacheImage CacheImage = (*it).second;
		if (CacheImage.pImage == pImage)
		{
			(*it).second.nRefCount++;
			return pImage;
		}
	}
	return NULL;
}
//------------------------------------------------------------------------
// �ͷ�ĳ��ͼƬ����������д��ڴ�ͼƬ����ֱ�ӽ���ͼƬ���ü�����1
// �����������Ϊ0����ɾ�������еĸ�ͼƬ
void CImageLoaderEx::FreeImage(const char* szFileName)
{
	if (szFileName == NULL || szFileName[0] == '\0')
		return;
	
	CacheImageListPtr it = m_vImageList.find(szFileName);
	if (it != m_vImageList.end())
	{
		if (--((*it).second.nRefCount) == 0)
		{
			(*it).second.pImage->Release();
			m_vImageList.erase(it);
#ifdef _DEBUG
			if (m_vImageList.size() <= 0)
			{
				OutputDebugString("WARNING: Image count <= 0 !!!!!\r\n");
			}
#endif
		}
	}
}
//------------------------------------------------------------------------
// �ͷ�ĳ��ͼƬ����������д��ڴ�ͼƬ����ֱ�ӽ���ͼƬ���ü�����1
// �����������Ϊ0����ɾ�������еĸ�ͼƬ
void CImageLoaderEx::FreeImage(IBitmap* pImage)
{
	if (pImage == NULL)
		return;
	CacheImageListPtr _end = m_vImageList.end();
	for (CacheImageListPtr it=m_vImageList.begin(); it!=_end; ++it)
	{
		stCacheImage CacheImage = (*it).second;
		if (CacheImage.pImage == pImage)
		{
			if (--CacheImage.nRefCount == 0)
			{
				CacheImage.pImage->Release();
				m_vImageList.erase(it);
#ifdef _DEBUG
				if (m_vImageList.size() <= 0)
				{
					OutputDebugString("WARNING: Image count <= 0 !!!!!\r\n");
				}
#endif
			}
			break;
		}
	}
}
//------------------------------------------------------------------------
BOOL CImageLoaderEx::SetFileSystem(IFileSystem* pFS)
{
	if (pFS == NULL)
		return FALSE;
	m_pFS = pFS;
	return TRUE;
}
//------------------------------------------------------------------------
// ����һ��������ͼƬ
IBitmap* CImageLoaderEx::_loadImage(const char* szFileName)
{
	if (m_pFS == NULL)
		return NULL;
	// ����ͼƬ����
	int nImageType = -1;
	char szExt[8];
	int nLen = strlen(szFileName);
	strcpy(szExt, &szFileName[nLen-3]);
	if (strcmpi(szExt, "bmp") == 0)
		nImageType = 0;
	else if (strcmpi(szExt, "tga") == 0)
		nImageType = 1;
  
	if (nImageType == -1)
		return NULL;

	// ���ļ�
	IBitmap* pImage = NULL;
	IFileObject* pFileObj;
	if ((pFileObj = m_pFS->GetFile(szFileName)) == NULL)
	{
		PrintEventLn("Error: GetFile() : can not get file[%s]", szFileName);
		return NULL;
	}
	if (!pFileObj->Open())
	{
		PrintEventLn("Error: Open() : can not open file[%s]", szFileName);
		return NULL;
	}
	DWORD dwSize = pFileObj->GetLength();
	BYTE* pbuf = new BYTE[dwSize];
	if (pFileObj->Read(pbuf, dwSize) != (UINT)dwSize)
	{
		pFileObj->Release();
		delete[] pbuf;
		PrintEventLn("Error: Read() : read file error[%s]", szFileName);
		return NULL;
	}
	pFileObj->Release();

	// ����ͼƬ����
	if (nImageType == 0)
	{
		if (!m_CanvasHelper.CreateIBitmap(&pImage))
		{
			delete[] pbuf;
			return NULL;
		}
	}
	else if (nImageType == 1)
	{
		IAlphaBitmap* pAlphaImage;
		if (!m_CanvasHelper.CreateIAlphaBitmap(&pAlphaImage))
		{
			delete[] pbuf;
			return NULL;
		}
		pImage = static_cast<IBitmap*>(pAlphaImage);
	}

	// ����
	if (!pImage->Load(pbuf))
	{
		pImage->Release();
		delete[] pbuf;
		return NULL;
	}

	delete[] pbuf;
	return pImage;
}
//------------------------------------------------------------------------