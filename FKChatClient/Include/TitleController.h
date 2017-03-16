/**
*	created:		2012-6-28   9:18
*	filename: 		TitleController
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/Event/IEvent.h"
#include "../../FKGame/Controller/IController.h"
//------------------------------------------------------------------------
typedef __int64 UID;
//------------------------------------------------------------------------
class CTitleController:public IEventActionListener,
	public IDataReceiver
{
public:
	CTitleController();
	virtual ~CTitleController();
public:
	virtual void OnAction(IEvent *pEvent);

	virtual BOOL OnReceived(const char* pData, int nLen);
	// ��ȡ�ƺ�
	void GetTitle();

	// ʹ�óƺ�
	void UserTitle(long lTitleId);

	// ��ʹ�óƺ�
	void UnUseTitle(long lTitleId);

	// ɾ���ƺ�
	void DeleteTitle(long lTitleId);

	// ��ȡ�����˵ĳƺ�
	void GetOtherTitle(UID uid);

	// ��ȡ�ƺŷ���
	void GetTitleResponse(LPCSTR pBuf,int nLen);

	// ʹ�óƺŷ���
	void UseTitleResponse(LPCSTR pBuf,int nLen);

	// ��ʹ�óƺ�
	void UnUseTitleResponse(LPCSTR pBuf,int nLen);

	//ɾ���ƺ���Ӧ
	void DeleteTitleResponse(LPCSTR pBuf,int nLen);

	// ��ȡ��ĳ����ҵĳƺ�
	void GetOtherTitleResponse(LPCSTR pBuf,int nLen);

	// ״̬�ı�֪ͨ
	void TitleStatusChange(LPCSTR pBuf,int nLen);

	// ����һ���ƺ�
	void AddTitleResponse(LPCSTR pBuf,int nLen);

};
//------------------------------------------------------------------------