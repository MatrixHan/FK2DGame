/**
*	created:		2012-6-24   23:08
*	filename: 		IControlModel
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��Ϸ������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
struct  IControlMode
{
	// ����Ѿ�����������ϢӦ����0
	virtual void ConnectSingal(void){};      //���Ӵ��ڵ��ź�

	virtual void DisConnectSingal(void){};   //ȡ�����Ӵ��ڵ��ź�

	// �μ�WM_LBUTTONUP
	virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam){return -1;}
	
	// �μ�WM_RBUTTONUP
	virtual LRESULT OnRButtonUp(WPARAM wParam, LPARAM lParam){return -1;}

	// �μ�WM_LBUTTONDOWN
	virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam){return -1;}
	
	// ����Ѿ�����������ϢӦ����0
	// �μ�WM_LBUTTONDOWN
	virtual LRESULT OnRButtonDown(WPARAM wParam, LPARAM lParam){return -1;}
	
	// ����Ѿ�����������ϢӦ����0
	// �μ�WM_MOUSEMOVE
	virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam){return -1;}
	
	// ����Ѿ�����˼�����ϢӦ����0
	// �μ�WM_KEYDOWN
	virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam){return -1;}
	
	// ����Ѿ�����˼�����ϢӦ����0
	// �μ�WM_KEYDOWN
	virtual LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam){return -1;}
	
	virtual void Release(void){delete this;}
};
//------------------------------------------------------------------------