/**
*	created:		2012-7-3   2:24
*	filename: 		CreatureCommandBase
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�����������
					�������������,����Ҫ����ִ��ĳһ����ʱ��ֱ�ӵ��ô˽ӿ�, ����ڵ�������ķ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/Common/ICreatureCommand.h"
//------------------------------------------------------------------------
// ����
struct ICreature;
//------------------------------------------------------------------------
//  ����COMMON
#define DECLARE_SINGLE_COMMAND(class_name)						\
	private:													\
			class_name(void);									\
			static class_name			m_Instance;				\
	public:														\
			static ICreatureCommand *	GetInstance(void);		\

//------------------------------------------------------------------------
// ʵ����COMMON
#define INSTANTIATION_SINGLE_COMMAND(class_name)				\
	class_name			class_name::m_Instance;					\
																\
	ICreatureCommand *	class_name::GetInstance(void)			\
	{															\
			return static_cast<ICreatureCommand *>(&m_Instance);\
	}															\

//------------------------------------------------------------------------
// ʵ��һ���ն��� ���������кţ�CREATURE_COMMANDID_INVALID��
class CCreatureCommand_Null : public ICreatureCommand
{
public:	
	// ȡ�ò���ID
	virtual DWORD		GetCommandID(void);
	
	// ִ������
	virtual int			OnCommand(ICreature * pMaster, LPCSTR pContext = NULL, int nLen = 0);

	DECLARE_SINGLE_COMMAND(CCreatureCommand_Null)
};
//------------------------------------------------------------------------
// ��������վ��֮����������кţ�CREATURE_COMMANDID_STAND��
class CCreatureCommand_Stand : public ICreatureCommand
{
public:
	// ȡ�ò���ID
	virtual DWORD		GetCommandID(void);
	
	// ִ������, TRUE : �ɹ���FALSE ��ʧ��
	virtual int			OnCommand(ICreature * pMaster, LPCSTR pContext = NULL, int nLen = 0);

	DECLARE_SINGLE_COMMAND(CCreatureCommand_Stand)
};
//------------------------------------------------------------------------
// �ı��ƶ���ʽ�����Զ�����״̬ת��/���磺���� < -- > �ܲ� < -- > ���� ���������кţ�CREATURE_COMMANDID_CHANGEMOVESTYLE��
class CCreatureCommand_ChanageMoveStyle : public ICreatureCommand
{
public:
	// ȡ�ò���ID
	virtual DWORD		GetCommandID(void);
	
	// ִ������, TRUE : �ɹ���FALSE ��ʧ��
	virtual int			OnCommand(ICreature * pMaster, LPCSTR pContext, int nLen);
	
	DECLARE_SINGLE_COMMAND(CCreatureCommand_ChanageMoveStyle)
};
//------------------------------------------------------------------------