/**
*	created:		2012-6-23   0:22
*	filename: 		IThingClass
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <Windows.h>
//------------------------------------------------------------------------
/* ��׼DWORDλʾ��ͼ:
					0000 0000 0000 0000 0000 0000 0000 0000
				32 _|||| |||| |||| |||| |||| |||| |||| ||||_  1.
				31 __||| |||| |||| |||| |||| |||| |||| |||__  2. 
				30 ___|| |||| |||| |||| |||| |||| |||| ||___  3.
				29 ____| |||| |||| |||| |||| |||| |||| |____  4.
				28 ______|||| |||| |||| |||| |||| ||||______  5. 
				27 _______||| |||| |||| |||| |||| |||_______  6. 
				26 ________|| |||| |||| |||| |||| ||________  7.
				25 _________| |||| |||| |||| |||| |_________  8.
				24 ___________|||| |||| |||| ||||_____________9. 
				23 ____________||| ||||	|||| |||______________10.
				22 _____________|| ||||	|||| ||_______________11.
				21 ______________| ||||	|||| |________________12.
				20 ________________||||	||||__________________13.
				19 _________________|||	|||___________________14.
				18 __________________||	||____________________15.
				17 ___________________|	|_____________________16. 
 */

/* 					0000 0000 0000 0000 0000 0000 00000000
				32 _|||| |||| |||| |||| |||| |||| --------
				31 __||| |||| |||| |||| |||| ||||     |______ 1 ~ 8. THINGCLASS����
				30 ___|| |||| |||| |||| |||| |||| 
				29 ____| |||| |||| |||| |||| |||| 
				28 ______|||| |||| |||| |||| |||| 
				27 _______||| |||| |||| |||| |||| 
				26 ________|| |||| |||| |||| |||| 
				25 _________| |||| |||| |||| |||| 
				24 ___________|||| |||| |||| ||||_____________9.  �Ƿ�Ϊ����
				23 ____________||| ||||	|||| |||______________10. �Ƿ�Ϊװ��
				22 _____________|| ||||	|||| ||_______________11. �Ƿ�ΪҩƷ
				21 ______________| ||||	|||| |________________12. �Ƿ�Ϊƾ֤
				20 ________________||||	||||__________________13. �Ƿ�ΪԪ��
				19 _________________|||	|||___________________14.
				18 __________________||	||____________________15.
				17 ___________________|	|_____________________16. 
*/
//------------------------------------------------------------------------
// ��־λ
#define THINGCLASS_FLAG_CREATURE		0x00000100	// Ϊ����
#define THINGCLASS_FLAG_EQUIPMENT		0x00000200	// Ϊװ��
#define THINGCLASS_FLAG_MEDICAMENT		0x00000400	// ΪҩƷ
#define THINGCLASS_FLAG_WARRANT			0x00000800	// Ϊƾ֤
#define THINGCLASS_FLAG_VALUABLES		0x00001000	// ΪԪ��
//------------------------------------------------------------------------
// ����
struct IThingClass
{
	// ��������ֵ
	virtual DWORD	GetClassVal(void) = NULL;
	// ��������THINGCLASS_ACTOR, THINGCLASS_MONSTER����, Ҳ���ǵͰ�λ��
	virtual DWORD	GetThingClass(void) = NULL;
	// �����Ƿ�Ϊ����
	virtual BOOL	IsCreature(void) = NULL;
	// �����Ƿ�Ϊװ��
	virtual BOOL	IsEquipment(void) = NULL;
	// �����Ƿ�ΪҩƷ
	virtual BOOL	IsMedicament(void) = NULL;
	// �����Ƿ�Ϊƾ֤
	virtual BOOL	IsWarrant(void) = NULL;
	// �����Ƿ�ΪԪ��
	virtual BOOL	IsValuables(void) = NULL;
};
//------------------------------------------------------------------------