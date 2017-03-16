/**
*	created:		2012-6-25   23:58
*	filename: 		IClientWorld
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
// ����
struct IThingTable;
//------------------------------------------------------------------------
// ����
struct IClientWorld
{
	// �ر�
	virtual void			Close(void) = NULL;
	// ��Ϣ
	virtual void			OnMessage(DWORD dwKeyMain, LPCSTR buf, int nLen) = NULL;
	// ȡ���߼����������
	virtual IThingTable *	GetThingTable(void) = NULL;
	// ȡ�õ�ǰ��̬��ͼID
	virtual int				GetCurStaticMapID(void) = NULL;
};
//------------------------------------------------------------------------