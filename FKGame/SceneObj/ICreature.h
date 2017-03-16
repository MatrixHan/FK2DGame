/**
*	created:		2012-6-23   0:24
*	filename: 		ICreature
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IMapThing.h"
#include "ICreatureActState.h"
//------------------------------------------------------------------------
// ����
struct IThingPart;
struct ICreatureCommand;
struct IBitmap;
//------------------------------------------------------------------------
// ����ӿ�
struct ICreature : public IMapThing
{
	// ȡ����������
	virtual LPCSTR				GetName(void) = NULL;
	// �������������������
	virtual int					SetPropNum(DWORD dwPropID, int nValue) = NULL;
	// ȡ�����������������
	virtual int					GetPropNum(DWORD dwPropID) = NULL;
	// ����������ַ�������
	virtual LPCSTR				SetPropStr(DWORD dwPropID, LPCSTR strPropValue) = NULL;
	// ȡ��������ַ�������
	virtual LPCSTR				GetPropStr(DWORD dwPropID) = NULL;
	// ������ﲿ��, ����Ѵ���,������ʧ�ܣ��߼������ͷ�ʱ���������ͷ�
	virtual BOOL				AddPart(IThingPart * pPart) = NULL;
	// ɾ�����ﲿ��, �����ͷ�
	virtual BOOL				RemovePart(DWORD dwPartID) = NULL;
	// ȡ�����ﲿ��
	virtual IThingPart *		GetPart(DWORD dwPartID) = NULL;
	// ���õ�ǰ�ж�״̬
	virtual BOOL				GotoActState(DWORD dwActStateID, LPCSTR pContext = NULL, int nLen = 0) = NULL;
	// ȡ�õ�ǰ�ж�״̬
	virtual ICreatureActState * GetCurActState(void) = NULL;
	// ����һ��������
	virtual BOOL				AddCreatureCommand(ICreatureCommand * pCreatureCommand) = NULL;
	// ȡ������ͷ��
	virtual IBitmap *			GetHeadBitmap(void) = NULL;
	// ���ô�������״̬
	virtual void				SetDying(BOOL bDying) = NULL;
	// �Ƿ�����������
	virtual BOOL				IsDying(void) = NULL;
};
//------------------------------------------------------------------------