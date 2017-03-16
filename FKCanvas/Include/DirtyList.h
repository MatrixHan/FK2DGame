/**
*	created:		2012-6-20   19:58
*	filename: 		DirtyList
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
#define MAX_DIRTY_SIZE 200
#define SFREE -1
#define SEND  -2
//------------------------------------------------------------------------
class CDirtyList  
{
private:
	RECT m_rcList[MAX_DIRTY_SIZE];
	int  m_nPosList[MAX_DIRTY_SIZE];
	int* m_pPrev;
	int m_nCurp, m_nLastp;
	int m_nCount;
	RECT m_rcBorder;
	BOOL m_bWhole;
public:
	CDirtyList();
	virtual ~CDirtyList();
public:
	void	_ClearDirty();
	void	ClearDirty();													//�������List
	int		GetDirtyCount(){return m_nCount;}								//�õ�����εĸ���
	void	_AddDirtyRect(int left, int top, int right, int bottom);
	void    AddDirtyRectDirect(int left, int top, int right, int bottom);	//���������
	void	AddDirtyRect(RECT& rc);
	void	AddDirtyList(CDirtyList* pList);								//�ϲ�����DirtyList
	RECT*	SeekFirstDirty();												//ö�������
	RECT*	SeekNextDirty();
	void	PackDirty();													//�ϲ������
	void	PackDirty(int x, int y, int w, int h);
	void	RemoveDirty(RECT& rc);											//��ȥ�����
	void	SetBorder(RECT& rc){m_rcBorder = rc;}							//����������Ļ�ľ���
	BOOL	IsWholeDirty(){return m_bWhole;}								//�Ƿ�ȫ��
	BOOL	IsDirty(RECT& rc);												//���Щ�����Ƿ�Ϊ��
	void	Scroll(int x, int y);
	void	AddWholeScreen();
};
//------------------------------------------------------------------------