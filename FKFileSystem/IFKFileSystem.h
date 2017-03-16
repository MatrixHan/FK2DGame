/**
*	created:		2012-6-15   22:04
*	filename: 		IFKFileSystem
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
struct ITrace;
//------------------------------------------------------------------------
// �ļ�ϵͳ�汾��
#define VERSION_QUERY_FILE_SYSTEM		1
// �ļ�ϵͳ����(File System Style)�궨��
#define FSS_DIRECTORY			0	// �ļ�ϵͳ��Ŀ¼̬��ʽ����
#define FSS_PACKAGE				1	// �ļ�ϵͳ�԰�̬��ʽ����
// �����ļ�������(CreateFile Style)�궨��
#define CFS_NEW					0	// �½�һ���ļ������������
#define CFS_UPDATE				1	// ����һ���ļ����ļ��������
// �ַ������ȶ���
#define MAX_BLOCK_NAME_LEN		32  // ����������
#define MAX_RELATIVE_PATH_LEN	64  // ������·������
// �ļ�ϵͳѹ���ͽ���ص�����,dwFileCountsΪÿ�λص�ʱ�����˵��ļ���Ŀ
typedef void (CALLBACK* PACKPROC)(DWORD dwFileCounts);

//------------------------------------------------------------------------
// �ļ�����ӿڡ�����������MFC��CFile���CStdioFile
struct IFileObject
{
	/* �й��ļ���ģʽ����Ϣ: 
	"r"		��    ,�ļ���������ʧ��
	"w"		д    ,�ļ��������򴴽�,�������������
	"a"		���  ,�ļ��������򴴽�
	"r+"	��д  ,�ļ���������ʧ��
	"w+"	��д  ,�ļ��������򴴽�,�������������
	"a+"	�����,�ļ��������򴴽�
	"t"		�ı���ʽ
	"b"		�����Ʒ�ʽ
	*/

	// �ļ��򿪹ر�(Ĭ����ֻ������������ʽ��)
	virtual BOOL Open(LPCSTR lpszMode = "rb") = 0;
	virtual BOOL Close() = 0;
	
	// ���Ķ�д
	// ���ڰ��ļ��е��ļ�����ܾ�����Խ��Ķ�д
	virtual UINT Read(void* lpBuf, UINT nCount) = 0;
	virtual BOOL Write(const void* lpBuf, UINT nCount) = 0;

	// �ı��ļ�����(��CStdioFile��ĳ�Ա��������)
	// ���ڰ��ļ��е��ļ�����ܾ�����Խ��Ķ�д
	// �������ļ�β��,��������ֵΪNULL
	virtual LPSTR ReadString(LPSTR lpsz, UINT nMax) = 0;
	virtual BOOL WriteString(LPCSTR lpsz) = 0;

	// �ļ�ָ��λ�ö�λ
	// ���ļ��Ĳ����뵥���ļ�����
	// �ο�: SEEK_SET �ļ�ͷ; SEEK_CUR �ļ���ǰλ��; SEEK_END �ļ�β
	virtual BOOL Seek(LONG lOff, UINT nFrom = SEEK_SET) = 0;
	virtual BOOL SeekToBegin() = 0;
	virtual BOOL SeekToEnd() = 0;
	virtual int	GetPosition() = 0;

	// �ļ��������
	// ���ļ���֧��
	virtual BOOL Flush() = 0;

	// ��ȡ�ļ�����(�ļ�û��ʱҲ�ܻ�ȡ����)
	// ����ǰ��ļ�,����ָѹ����ĳ���
	virtual DWORD GetLength() = 0;
	virtual BOOL SetLength(DWORD dwNewLen) = 0;

	// ����ļ���,����"filetitle.ext"
	virtual LPCSTR GetFileName() = 0;
	// ����ļ�������,����"filetitle"
	virtual BOOL GetFileTitle(char* buf) = 0;
	// ����ļ���·����,����"d:\dxGame\filetitle.ext"
	virtual LPCSTR GetFilePath() = 0;

	// �ļ��Ƿ����
	// ע: ���ļ���֧��,��̬ʱ��ͨ��IFileSystem->GetFile(...)���ж��Ƿ����
	virtual BOOL IsExist() = 0;
	// �ͷ�(��ס:��������������)
	virtual BOOL Release() = 0;
};

//------------------------------------------------------------------------
// �ļ�ϵͳ�ӿڣ����ļ�������в���
struct IFileSystem
{
	// �����ļ�
	// ����ļ�����,����ʧ��,����NULL,��Trace����;
	// ����ֵ: �ɹ������ļ�����;ʧ�ܷ���NULL
	// ע: 1���û����Լ��ͷŻ�õ��ļ�����IFileObject->Release()
	//	   2���ļ����������·��
	//	   3������pData��nSize���������ڰ�̬�д����ļ�;��Ŀ¼̬ʱ����
	//	   4��Ŀ¼̬ʱ,�ò����½�һ���ļ�(û�д�,�û��Լ����ô�ģʽ����!!)
	virtual IFileObject* CreateFile(LPCSTR szFileName, char* pData = NULL, int nSize = 0, UINT nStyle = CFS_NEW) = 0;

	// ���ĳ���ļ�����(�����򿪶�Ӧ�ļ�!)
	// ����ֵ: �ɹ������ļ�����;ʧ�ܷ���NULL
	// �ļ����������·��
	virtual IFileObject* GetFile(LPCSTR szFileName) = 0;

	// ����Ŀ¼(Ŀ¼̬֧�ֶ༶����,��̬��֧��)
	// �ɹ�����TRUE;ʧ�ܷ���FALSE
	// Ŀ¼���������·��
	virtual BOOL CreateDirectory(LPCSTR szDirectoryName) = 0;

	// ɾ��ָ�����ļ�
	// �����ɾ���ļ�������,�������˲���(Ȩ��ִ����ɾ��),����TRUE,��Trace����;
	// ����ֵ: �ɹ�����TRUE;ʧ�ܷ���FALSE
	// �ļ����������·��;���ļ��е�ɾ�����������Ǳ�ʶΪ��Ч����
	virtual BOOL DeleteFile(LPCSTR szFileName) = 0;

	// ɾ��Ŀ¼(�����༶��Ŀ¼����ļ�)
	// �����ɾ��Ŀ¼������,�������˲���(Ȩ��ִ����ɾ��),����TRUE,��Trace����;
	// ����ֵ: �ɹ�����TRUE;ʧ�ܷ���FALSE
	// Ŀ¼���������·��;���ļ��е�ɾ�����������Ǳ�ʶΪ��Ч����
	virtual BOOL DeleteDirectory(LPCSTR szDirectoryName) = 0;

	// �ļ�(Ŀ¼)������
	// ������������ļ�������,�����ʧ��,����FALSE,��Trace����;
	// ����ֵ: �ɹ�����TRUE;ʧ�ܷ���FALSE
	// ע: 1�������޸�����,������ı�·��; 2���������·��
	virtual BOOL Rename(LPCSTR szOldFileName,LPCSTR szNewFileName) = 0;
	
	// 1����Ŀ¼̬�ļ�ϵͳ���Ƴɰ�̬�ļ�ϵͳ���򷴹���(nFlags == 0)
	// 2������ǰ�ļ���ϵͳ���Ƶ���һ���ļ���ϵͳ��(nFlags != 0)
	// ���ڰ��ļ�ϵͳ�У�������ɾʱ���ֺܶ�ն������Ƶ����ļ���ϵͳ���Խ���������
	// ע: ����Ĳ������þ���·��!!!
	virtual BOOL CopyToFileSystem(LPCSTR szNewFileSystemName, int nFlags = 0, PACKPROC lpfnPackProC = NULL) = 0;

	// �ͷ�
	// ע�������ͷ��Լ�,����������������IFileObject
	virtual BOOL Release() = 0;
	
	virtual IFileObject* GetFirstFile() = 0;
	virtual IFileObject* GetNextFile() = 0;
	virtual DWORD GetLength() = 0;
	virtual IFileObject* GetIndexFile(LPCSTR szFileName, DWORD dwCompSize) = 0;
};


//------------------------------------------------------------------------
// �ļ�ϵͳ������
class CIFileSystemHelper
{
public:
	typedef BOOL (*ProcCreateFileSystem)(DWORD dwExpectVersion,
			LPCSTR szFileSystemName, DWORD dwStyle, ITrace* pTrace,
			IFileSystem** ppFileSystem,	void* Reserved );

	CIFileSystemHelper()
	{
		m_hdll = NULL;
	}
	~CIFileSystemHelper()
	{
		if(m_hdll)
		{
			::FreeLibrary(m_hdll);
			m_hdll = NULL;
		}
	}
	BOOL Create(IFileSystem** ppFileSystem, LPCSTR szFileSystemName, DWORD dwStyle, ITrace* pTrace,	void* Reserved = NULL)
	{
		try
		{
			if(m_hdll == NULL)
			{
				#ifdef _DEBUG
					m_hdll = ::LoadLibrary("FKFileSystem_D.dll");
					if(m_hdll == NULL)
						throw "��ȡ FileSystemD.dll ʧ�ܣ�";
				#else
					m_hdll = ::LoadLibrary("FKFileSystem.dll");
					if(m_hdll == NULL)
						throw "��ȡ FileSystem.dll ʧ�ܣ�";
				#endif
			}
			ProcCreateFileSystem proc;
			proc = (ProcCreateFileSystem)GetProcAddress(m_hdll, "CreateFileSystem");
			if(proc == NULL)
				throw "�޷���ȡ���� CreateFileSystem ��ַ��')";

			if(!proc(VERSION_QUERY_FILE_SYSTEM, szFileSystemName, dwStyle, pTrace, ppFileSystem, Reserved))
				throw "CreateFileSystem() ����";
			return ppFileSystem != NULL;
		}
		catch(LPCSTR szMsg)
		{
			OutputDebugString(szMsg);OutputDebugString("\r\n");
			return FALSE;
		}
		catch(...)
		{
			OutputDebugString("CreateFileSystem() �����쳣��\r\n");
			return FALSE;
		}
	}
private:
	HINSTANCE		m_hdll;
};
//------------------------------------------------------------------------