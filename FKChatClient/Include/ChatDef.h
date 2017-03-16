/**
*	created:		2012-6-28   4:41
*	filename: 		ChatDef
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����Ƶ���Ķ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
typedef __int64 UID;
//------------------------------------------------------------------------
#define CHAT_MESSAGE_MAX	400			// ��Ϣ����󳤶�
#define CHAT_NAME_MAX		256			// ������Ƶ���󳤶�
#define CHAT_LOCAL_MAX		10			// ����Ƶ���������Ϊ���ĵķ�Χ
//------------------------------------------------------------------------
// ������ʾ���ڵ���ʾ��ʽ
enum CHAT_WINDOW
{
	CW_ALTERNATE,	// ����
	CW_HIDE,		// ����
	CW_SHOW			// ��ʾ
};
//------------------------------------------------------------------------
// �Զ��������
enum CHAT_CUSTOM_CMD
{
	CHAT_CMD_NONE,
	CHAT_CMD_PING,
	CHAT_CMD_BULLETIN,
	CHAT_CMD_SHUTUP,
	CHAT_CMD_KICK,
	CHAT_CMD_FIND,
	CHAT_CMD_GOTO,
	CHAT_CMD_FAIL,
	CHAT_CMD_FETCH,
	CHAT_CMD_FETCH2,              // ��DBID����
	CHAT_CMD_UPDATEFREEWORLDMSG   // �������������������
};
//------------------------------------------------------------------------
// Ƶ������(��СƵ������Ϊ0,�Ա��ڲ���)
enum CHANNEL
{
	CHANNEL_SYSTEM = 0,		// ϵͳ
	CHANNEL_LOCAL,			// ����
	CHANNEL_PRIVATE,		// ˽��
	CHANNEL_TEAM,			// ����
	CHANNEL_CLAN,			// ����
	CHANNEL_WORLD,			// ����
	CHANNEL_MONSTER        // ����Ƶ��
};
//------------------------------------------------------------------------
// ����Ƶ���ķ��Զ���ʱ������
#define COLDTIME_CHAT_TYPE				3
#define COLDTIME_CHAT_WORLD_VALUE		1  //����Ƶ��
#define COLDTIME_CHAT_ELSE_VALUE        2  //����Ƶ��


#define CHANNEL_MIN CHANNEL_SYSTEM		// ��СƵ��
#define CHANNEL_MAX CHANNEL_WORLD		// ���Ƶ��
#define CHANNEL_COUNT (CHANNEL_MAX + 1)	// Ƶ������
//------------------------------------------------------------------------
// ����ǲ�����Ч��Ƶ��
#define IS_VALID_CHANNEL(c) (CHANNEL_MIN <= (c) && (c) <= CHANNEL_MAX)
//------------------------------------------------------------------------
#pragma pack(push, 1)
//------------------------------------------------------------------------
// ������Ϣͷ
struct SChatMsgHeader
{
	BYTE nChannel;				// Ƶ��
	UID  uidSender;				// ������UID
	char szSender[32];			// ������
};
//------------------------------------------------------------------------
// ϵͳ������Ϣ
struct SSystemChatMsg
{
	SChatMsgHeader header;
	BYTE nCustomCmd;
	WORD wTextLen;
	char szText[1];
};
//------------------------------------------------------------------------
// ����������Ϣ
struct SLocalChatMsg
{
	SChatMsgHeader header;
	WORD wTextLen;
	char szText[1];
};
//------------------------------------------------------------------------
// ���˶�����Ϣ
struct SPrivateChatMsg
{
	SChatMsgHeader header;
	char szReceiver[32];
	WORD wTextLen;
	char szText[1];
};
//------------------------------------------------------------------------
// ����������Ϣ
struct STeamChatMsg
{
	SChatMsgHeader header;
	DWORD dwTeamId;
	WORD wTextLen;
	char szText[1];
};
//------------------------------------------------------------------------
// ����������Ϣ
struct SClanChatMsg
{
	SChatMsgHeader header;
	DWORD dwDBID;           //�������⴦��,��Ҫ����DBID
    DWORD dwClanId;
	WORD wTextLen;
	char szText[1];
};
//------------------------------------------------------------------------
// ����������Ϣ
struct SWorldChatMsg
{
	SChatMsgHeader header;
	WORD wTextLen;
	char szText[1];
};
//------------------------------------------------------------------------
// ����������Ϣ
struct SMonsterChatMsg
{
	SChatMsgHeader header;
	WORD wTextLen;
	char szText[1];
};
//------------------------------------------------------------------------
#pragma pack(pop)
//------------------------------------------------------------------------