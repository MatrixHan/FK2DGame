/**
*	created:		2012-7-2   21:55
*	filename: 		UserSetting
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��д�û������ļ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
struct IUserSetting
{
public:
	// ��ȡ�趨
	virtual BOOL  ReadSetting(void) = NULL;
	// �����趨
	virtual BOOL  SaveSetting(void) = NULL;
	
	//��ȡ���ܿ�ݼ�
	virtual BOOL  ReadSkillShortcut(void) = NULL;
	//��ȡ���ܿ�ݼ�
	virtual BOOL  SaveSkillShortcut(void) = NULL;

public:
	/********************���л������****************************************/
	BOOL	m_bFullScreen;				//�Ƿ�ȫ����ʾ
	BOOL	m_bOpenTrace;				//�Ƿ��Trace����
	BOOL	m_bOpenSound;				//�Ƿ����Ч����
	BOOL	m_bOpenMusic;				//�Ƿ�����ֿ���
	long	m_lSoundValue;				//��Ϸ��Ч��С
	long	m_lMusicValue;				//�������ִ�С

	/********************���绷�����****************************************/
	char    m_szServerIP[64];			//��Ϸ��������ַ
	long	m_lPort;					//��Ϸ�˿�
	char    m_szUserName[32];			//Ĭ���û���
	long    m_dwRecordUserName;			//�Ƿ��¼�û���

	/********************��Ϸ�趨���****************************************/
	long	m_dwRightSkill;				//��ǰʹ�õ����ּ���
	long    m_dwLeftSkill;				//��ǰʹ�õ����ּ���
	long    m_dwShortcut[16];			//��ݼ���
	long    m_dwShortcutGoods[8];		//�����Ʒ��
	long	m_lShowNameFlag;			//����������ʾ����
	BOOL    m_bAcceptGoods;             //�Ƿ������Ʒ
	BOOL    m_bAcceptTrade;             //�Ƿ���

	BOOL    m_bSpeed;					//�Ƿ�����Ϸ�ٶ��Ż�ģʽ
};
//------------------------------------------------------------------------
#define  SHOW_NAME_MASK_SELF		0x00000001	// ��ʾ�Լ�������
#define  SHOW_NAME_MASK_ELSE		0x00000010	// ��ʾ�����˵�����
#define  SHOW_NAME_MASK_LEAGUE		0x00000100	// ��ʾ��������
#define  SHOW_NAME_MASK_MONSTER		0x00001000	// ��ʾ��������
//------------------------------------------------------------------------