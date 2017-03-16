/**
*	created:		2012-7-2   21:56
*	filename: 		IBox
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/SceneObj/IMapThing.h"
//------------------------------------------------------------------------
struct ICreatureActState;
class CMapItemView;
//------------------------------------------------------------------------
struct IBox : public IMapThing
{
	// �õ�����
	virtual LPCSTR GetName() const = 0;

	// ����������
	// �򿪺���
	virtual BOOL Open() = 0;

	// �رպ���
	virtual void Close() = 0;

	// ����˵���Ӧ
	virtual void Respond(LPCSTR pBuf, int nLen) = 0;
	
	// �õ�����
	virtual DWORD GetType() const = 0;
	
	// �õ�id
	virtual DWORD GetID() const = 0;
	
	// ����Ȩ��
	virtual void SetRight(BOOL bRight) = 0;
	
	// �������ܲ��ܴ�
	virtual BOOL IsCanOpen(void) = 0;
};
//------------------------------------------------------------------------