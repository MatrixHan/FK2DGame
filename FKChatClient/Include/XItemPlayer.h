/**
*	created:		2012-6-28   2:25
*	filename: 		XItemPlayer
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���������ʾ����Ҷ�����Ŀ
*/
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
//------------------------------------------------------------------------
// ���������ʾ����Ҷ�����Ŀ
class XItemPlayer : public XItemLinkText  
{
public:
	XItemPlayer(void);
	virtual ~XItemPlayer(void);

public:
	virtual void Emit(void);
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
};
//------------------------------------------------------------------------