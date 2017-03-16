/**
*	created:		2012-6-24   20:15
*	filename: 		CDataStream
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��������д

					����ṹ���£�
					�����ݳ���(4�ֽ�) + N�����ݿ�(���ݿ鳤�� + ����)
					ʹ�ã�CDataStream��Ҫ�ⲿ�ṩ������������һ���ֲ�buffer��������:
					char buf[1024];
					CDataStream datastream(buf,1024);
					datastream.Write("hello",5);
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
class CDataUnit;
//------------------------------------------------------------------------
class CDataStreamBuild
{
public:
	CDataStreamBuild(void);
	CDataStreamBuild(void * szBindBuffer, int nMaxSize);
	~CDataStreamBuild(void);
public:
	void			Init(void * szBindBuffer, int nMaxSize);	

	// ����д�뻺�塣nLen < 4K
	// �����и��ٶ���������밴˳��д�����������
	virtual BOOL	Write(const void* buf, int nLen);

	// ����д��DataUnit
	virtual BOOL	Write(CDataUnit &DataUnit);

	// ��ȡ��ǰ����ָ����ָ�����ݵ�ַ���ʺ�ֱ�Ӳ���������
	virtual void *	GetBuffer(void);

	// ��õ�ǰ����ָ��ƫ��
	virtual int		GetBufferOffset(void);

	// д����ʱ��������Ч����������
	virtual int		GetLeftBufferLen(void);

	// ȡ��Buffer�����ݳ���
	virtual int		GetTotalDataLen(void);
private:
	// ��ǰ������ָ��ƫ��
	int				m_nOffset;
	// ��ǰ������ָ��
	void * 			m_pBindBuffer;
	// ��������󳤶�
	int				m_nMaxSize;
	// �����ݳ���
	int				m_nTotalDataSize;
};
//------------------------------------------------------------------------
class CDataStreamParse
{
public:
	CDataStreamParse(void);
	CDataStreamParse(void * szParseBuffer, int nSize);
	~CDataStreamParse(void);
public:
	// ����Ҫ������BUFF
	BOOL			Load(void * szParseBuffer, int nSize);

	// ��ȡ��ǰ����ָ����ָ�����ݵ�ַ���ʺ�ֱ�Ӳ���������
	virtual void *	GetBuffer(void);

	// �ӵ�ǰ����ָ�봦���chunk
	virtual BOOL	GetChunk(void* buf, int &nLen);
	
	// �Ƶ��������ײ�
	virtual int		Reset(void);

	// ��õ�ǰ����ָ��ƫ��
	virtual int		GetBufferOffset(void);
	
	// ��ȡʣ�����ݳ���
	virtual int		GetLeftBufferLen(void);	

	// ȡ��Buffer�����ݳ���
	virtual int		GetTotalDataLen(void);

	// �Ƿ���bufferβ
	virtual BOOL	IsEnd(void);
private:
	// ��ǰ������ָ��
	void *			m_pParseBuffer;
	// ��ǰ������ָ��ƫ��
	int				m_nOffset;
	// ��������С
	int				m_nSize;
	// ��������С
	int				m_nTotalDataSize;
};
//------------------------------------------------------------------------