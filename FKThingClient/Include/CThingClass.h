/**
*	created:		2012-7-2   23:36
*	filename: 		CThingClass
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��ΪTHING���ʹ������νṹ������򵥵���һ��DWORD��ʾ��
					���õ�ʱ��Ͳ������ˣ�����˵Ҫ�ж�һ�������Ƿ�Ϊ��Ʒ
					Ҫ�����ж��Ƿ�Ϊװ����ҩƷ�����ǲ��ϣ��Ƚ��鷳�����ڶ�
					�񻯣��ͺ��ö���
					
					����Ҳ����һ��DWORD��ǰ8λ��ʾTHINGCLASS����, ��24λ�Ǹ��ֱ�־�����磺�ǲ�����Ʒ���ǲ�������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../../FKGame/SceneObj/IThingClass.h"
//------------------------------------------------------------------------
class CThingClass : public IThingClass
{
public:
	// ��������ֵ
	virtual DWORD	GetClassVal(void);

	// ��������THINGCLASS_ACTOR, THINGCLASS_MONSTER����
	virtual DWORD	GetThingClass(void);

	// �����Ƿ�Ϊ����
	virtual BOOL	IsCreature(void);	

	// �����Ƿ�Ϊװ��
	virtual BOOL	IsEquipment(void);

	// �����Ƿ�ΪҩƷ
	virtual BOOL	IsMedicament(void);

	// �����Ƿ�Ϊƾ֤
	virtual BOOL	IsWarrant(void);

	// �����Ƿ�ΪԪ��
	virtual BOOL	IsValuables(void);

	// ������Ʒ����
	void			SetThingClass(DWORD dwThingClass);

	// ����Ϊ����
	void			SetCreature(void);

	// ����Ϊװ��
	void			SetEquipment(void);

	// ����ΪҩƷ
	void			SetMedicament(void);

	// ����Ϊƾ֤
	void			SetWarrant(void);

	// ����ΪԪ��
	void			SetValuables(void);

	CThingClass(void);

	~CThingClass(void);
private:
	DWORD			m_dwClassVal;
};
//------------------------------------------------------------------------