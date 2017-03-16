/**
*	created:		2012-6-30   5:17
*	filename: 		ISound
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ģ��ӿ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
struct IResObject;
struct IClientGlobal;
//------------------------------------------------------------------------
#define  INVALID_CHANNELID -1
//------------------------------------------------------------------------
struct ISound
{
	virtual bool Create(int Max_Channel) = 0;

	//////////////////////////////////////////////////////////////////
	//�������: ���������ļ���ID , �Ƿ�ѭ��, ���ŵ�λ��, (�������������վ��0,0 (��Ļ����)λ��)
	//���������Ƶ���ɣģ�����ֹͣЧ��
	//////////////////////////////////////////////////////////////////
	virtual int PlaySound(int id, BOOL bLoop = FALSE, int offsetX = 0, int offsetY = 0) = 0;

	//ֹͣ��Ч
	virtual void StopSound(int nChunnelID) = 0;

	//��������
	virtual void PlayMusic(int id, BOOL bLoop = TRUE) = 0;

	//ֹͣ����
	virtual void StopMusic() = 0;

	virtual void Release() = 0;

	virtual void SetSoundTable(IResObject *pResObj) = 0;

	//������������ (0 -- 100)
	virtual void SetMusicVolume(int nDegree) = 0;

	//��ȡ��������
	virtual int  GetMusicVolume(void) = 0;

	//������Ч���� (0 -- 100)
	virtual void SetSoundVolume(int nDegree) = 0;
	
	//��ȡ��Ч����
	virtual int  GetSoundVolume(void) = 0;

	//�������־���״̬
	virtual void SetMusicMute(BOOL bState) = 0;

	//��ȡ���־���״̬
	virtual BOOL GetMusicMute(void) = 0;
	
	//������Ч����״̬
	virtual void SetSoundMute(BOOL bState) = 0;

	//��ȡ��Ч����״̬
	virtual BOOL GetSoundMute(void) = 0;

	//��������ѭ������
	virtual void SetMusicLoop(BOOL bState) = 0;
	
	//��ȡ����ѭ������
	virtual BOOL GetMusicLoop(void) = 0; 

	//��������λ��(���ڴ���������Ч)
	virtual void UpdateActorLoc(const POINT &pt) {};
	virtual void UpdateActorLoc(const POINTS &pts) {};

	//����������ڷ�����Ƶ��
	virtual void ClearAllSound(){};

	//�ӽű��л�ȡ���е�ͼ�Ļ�����Ч�б�
	virtual BOOL LoadEnvironmentSoundScript(IResObject *pResScript){return TRUE;};
	
	//�л�������Ч����
	virtual void SwitchEnvironmentSoundConfig(int nKey) {};

	//���Ż�����Ч
	virtual int PlayEnvirSound(int id, BOOL bLoop = FALSE, int offsetX = 0, int offsetY = 0) {return INVALID_CHANNELID;};

	//��λ��ǰ������Ч����(���罫������Ч���´β���ʱ����0�ȹ���), ���л���ͼʱ��
	virtual void ResetCurrentEnvironmentSoundConfig(void){};

	//�������������ļ�, ��������ʱ�Ƿ��Ƿ�
	virtual void TestAllSoundFile(void){};

	//������Ƶ
	virtual BOOL PlayVideo(const char * szFileName){return FALSE;};

	//ֹͣ��Ƶ
	virtual void StopVideo(void){};

	//ֹͣĳһID������(����еĻ�)
	virtual void StopSoundBySoundID(int nSoundID){};
};
//------------------------------------------------------------------------
class CSoundHelper
{
private:
	typedef bool (*CreateSoundProc)(IClientGlobal *pGlobal, ISound **sound, int Max_Channel);
	HMODULE m_hdll;

public:
	CSoundHelper()
	{
		m_pSound = NULL;
		m_hdll = NULL;
	}

	~CSoundHelper()
	{
		Close();
	}

	BOOL Create(int Max_Channel ,IClientGlobal *pGlobal)
	{
		
		Close();//���ͷžɵ�
		
		try
		{
			
#ifdef _DEBUG
			m_hdll = ::LoadLibrary("FKSound_D.dll");
			if (m_hdll == NULL)
				throw "�޷����� FKSound_D.dll";
#else				
			m_hdll = ::LoadLibrary("FKSound.dll");
			if (m_hdll == NULL)
				throw "�޷����� FKSound.dll";
#endif
			

			CreateSoundProc CreateSound = NULL;
			CreateSound = (CreateSoundProc)GetProcAddress(m_hdll,"CreateSound");
			if (CreateSound == NULL)
				throw "Can not GetProcAddress('CreateSound')";
			
			if (!CreateSound(pGlobal, &m_pSound, Max_Channel))
			{
				
				throw "call fail in function : CreateSound()";
			}
				
			
			return m_pSound != NULL;
		}
		catch(LPCSTR szMsg)
		{
			OutputDebugString(szMsg);
			OutputDebugString("\r\n");
			
			return FALSE;
		}
		catch(...)
		{
			OutputDebugString("CreateSound::Create() : catch error\r\n");
			
			return FALSE;
		}

		return TRUE;
	}

	void Close()
	{
		if (m_pSound)
		{
			m_pSound->Release();
			m_pSound = NULL;
		}

		if (m_hdll)
		{
			::FreeLibrary(m_hdll);
			m_hdll = NULL;
		}
	}



public:
	ISound *m_pSound;
};
//------------------------------------------------------------------------