/**
*	created:		2012-7-2   19:57
*	filename: 		IUnknownEx
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
//��Ʒ�汾��
#define PRODUCT_VER				2
//------------------------------------------------------------------------
//�汾����
#define INTERFACE_VERSION(cbMainVer,cbSubVer)															\
	(DWORD)(																						\
	(((BYTE)(PRODUCT_VER))<<24)+																	\
	(((BYTE)(cbMainVer))<<16)+																		\
	((BYTE)(cbSubVer)<<8))

//------------------------------------------------------------------------
//��ȡ��Ʒ�汾
inline BYTE GetProductVer(DWORD dwVersion)
{
	BYTE * cbVersion=(BYTE *)&dwVersion;
	return cbVersion[3];
}
//------------------------------------------------------------------------
//��ȡ���汾
inline BYTE GetMainVer(DWORD dwVersion)
{
	BYTE * cbVersion=(BYTE *)&dwVersion;
	return cbVersion[2];
}
//------------------------------------------------------------------------
//��ȡ�Ӱ汾
inline BYTE GetSubVer(DWORD dwVersion)
{
	BYTE * cbVersion=(BYTE *)&dwVersion;
	return cbVersion[1];
}
//------------------------------------------------------------------------
//��ȡ����汾
inline BYTE GetBulidVer(DWORD dwVersion)
{
	BYTE * cbVersion=(BYTE *)&dwVersion;
	return cbVersion[0];
}
//------------------------------------------------------------------------
//�汾�ԱȺ���
inline bool InterfaceVersionCheck(DWORD dwQueryVer, DWORD dwInterfaceVer)
{
	if (GetProductVer(dwQueryVer)!=GetProductVer(dwInterfaceVer)) return false;
	if (GetMainVer(dwQueryVer)!=GetMainVer(dwInterfaceVer)) return false;
	if (GetSubVer(dwQueryVer)>GetSubVer(dwInterfaceVer)) return false;
	return true;
};
//------------------------------------------------------------------------
//�򵥽ӿ������������˵��
typedef void * (__cdecl ModuleCreateFunc)(const GUID & Gudi, DWORD dwInterfaceVer);
//------------------------------------------------------------------------
#define VER_IUnknownEx INTERFACE_VERSION(1,1)
static const GUID IID_IUnknownEx={0x92d87e91,0x5a5b,0x4a45,0x8c,0xfd,0x38,0x2f,0xe6,0xb3,0x2b,0xb};
//------------------------------------------------------------------------
//�����ӿ�
struct IUnknownEx
{
	//�Ƿ���Ч
	virtual bool __cdecl IsValid()=NULL;
	//�ͷŶ���
	virtual bool __cdecl Release()=NULL;
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer)=NULL;
};
//------------------------------------------------------------------------