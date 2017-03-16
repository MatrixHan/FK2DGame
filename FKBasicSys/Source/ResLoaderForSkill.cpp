/**
*	created:		2012-7-2   1:21
*	filename: 		ResLoaderForSkill
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/BasicCommon.h"
#include "../Include/ResLoader.h"
#include "../Include/Ani.h"
#include "../Include/AniGroup.h"
#include "../Include/AniObj.h"
#include "../Include/StaticObj.h"
#include "../Include/Magic.h"
#include "../Include/SkillCreate.h"
#include "../Include/ActorView.h"
#include "../Include/MonsterView.h"
//------------------------------------------------------------------------
// ��ȡ�⻷�ࡢ������ħ���ű�

int CResLoader::ReadAttachMagic(void* obj, void* buf)
{
	if (!obj || !buf)
		return FALSE;

	IResObject* pMagic = (IResObject*)obj;
	SAttachStateObjBuffer& ar = *(SAttachStateObjBuffer*)buf;

	// read data from magic script
	pMagic->GetPropertyLong("Delay", (long&)ar.nDelay, 60);
	pMagic->GetArrayLong("Anchor", (long*)&ar.ptAnchor, 2);
	pMagic->GetArrayLong("Offset", (long*)&ar.ptOffset, 2);
	pMagic->GetArrayLong("bAddCrtHeight", (long*)&ar.bAddCrtHeight, 0);

	pMagic->GetPropertyLong("LoopTime", (long&)ar.nLoopTime, 1);
	pMagic->GetPropertyLong("LoopStartIndex", (long&)ar.nLoopStartIndex, -1);
	pMagic->GetPropertyLong("LoopEndIndex", (long&)ar.nLoopEndIndex, -1);

	pMagic->GetPropertyLong("DrawOrder", (long&)ar.nDrawOrder, 0);

	pMagic->GetPropertyLong("SoundID", (long&)ar.dwSoundResID, 0);
	pMagic->GetPropertyLong("bSoundLoop", (long&)ar.bSoundLoop, 0);
	return TRUE;

}
//------------------------------------------------------------------------
// ������ħ���ű�
int CResLoader::ReadBaseMagic(void* obj, void* buf)
{
	if (!obj || !buf) return FALSE;

	IResObject* pMagic = (IResObject*)obj;
	SMagicArchive& ar = *(SMagicArchive*)buf;

	pMagic->GetArrayLong("Anchor", (long*)&ar.ptAnchor, 2);
	pMagic->GetArrayLong("Offset", (long*)&ar.ptOffset, 2);
	pMagic->GetPropertyLong("Delay", (long&)ar.nDelay, 30);
	pMagic->GetPropertyLong("LayerValue", (long&)ar.nLayerValue, 1);

	pMagic->GetPropertyLong("SoundID", (long&)ar.dwSoundResID, 0);
	pMagic->GetPropertyLong("bSoundLoop", (long&)ar.bSoundLoop, 0);
	return TRUE;
}
//------------------------------------------------------------------------
// ���������ڵ�ħ���ű�
int CResLoader::ReadLifeMagic(void* obj, void* buf)
{
	ASSERT(obj && buf);
	IResObject* pMagic = (IResObject*)obj;
	SMagicArchive_Life& ar = *(SMagicArchive_Life*)buf;

	// read data from magic script
	if (!ReadBaseMagic(obj, buf)) return FALSE;
	pMagic->GetPropertyLong("Life", (long&)ar.nLifeLine, -1);
	return TRUE;
}
//------------------------------------------------------------------------
// ������3��״̬��ħ��
int CResLoader::ReadThreeStateMagic(void* obj, void* buf)
{
	ASSERT(obj && buf);
	IResObject* pMagic = (IResObject*)obj;
	SMagicArchive_ThreeState& ar = *(SMagicArchive_ThreeState*)buf;

	// read data from magic script
	if (!ReadLifeMagic(obj, buf)) return FALSE;
	pMagic->GetPropertyLong("LoopStartIndex", (long&)ar.nLoopStartIndex, 0);
	pMagic->GetPropertyLong("LoopEndIndex", (long&)ar.nLoopEndIndex, 0);	
	return TRUE;
}
//------------------------------------------------------------------------
int CResLoader::ReadRainMagic(void* obj, void* buf)
{
	ASSERT(obj && buf);
	IResObject* pMagic = (IResObject*)obj;
	SMagicArchive_Rain& ar = *(SMagicArchive_Rain*)buf;

	// read data from magic script
	if (!ReadThreeStateMagic(obj, buf)) return FALSE;
	pMagic->GetArrayLong("StepOffset", (long*)&ar.ptStepOffset, 2);
	return TRUE;
}
//------------------------------------------------------------------------
// ����ħ���ű�
int CResLoader::ReadRingMagic(void* obj, void* buf)
{
	ASSERT(obj && buf);
	IResObject* pMagic = (IResObject*)obj;
	SMagicArchive_Ring& ar = *(SMagicArchive_Ring*)buf;

	// read data from magic script
	if (!ReadLifeMagic(obj, buf)) return FALSE;
	pMagic->GetPropertyLong("FramesPerDir", (long&)ar.nFramesPerDir, 1);
	return TRUE;
}
//------------------------------------------------------------------------
// ��ȡ����Զ�̹�����ħ���ű�(����򡢱�����)
int CResLoader::ReadSingleDirMagic(void* obj, void* buf)
{
	ASSERT(obj && buf);
	IResObject* pMagic = (IResObject*)obj;
	SMagicArchive_SingleDir& ar = *(SMagicArchive_SingleDir*)buf;

	// read data from magic script
	if (!ReadBaseMagic(obj, buf)) return FALSE;
	pMagic->GetPropertyLong("nDir", (long&)ar.nDir, 1);
	pMagic->GetPropertyLong("FramesPerDir", (long&)ar.nFramesPerDir, 1);
	return TRUE;
}
//------------------------------------------------------------------------
// ��ȡ�˸�������ħ���ű�(�����ն������ն��)
int CResLoader::ReadEightDirMagic(void* obj, void* buf)
{
	ASSERT(obj && buf);
	IResObject* pMagic = (IResObject*)obj;
	SMagicArchive_EightDir& ar = *(SMagicArchive_EightDir*)buf;

	// read data from magic script
	if (!ReadLifeMagic(obj, buf)) return FALSE;
	pMagic->GetPropertyLong("FramesPerDir", (long&)ar.nFramesPerDir, 1);
	return TRUE;
}
//------------------------------------------------------------------------
// ��ͨ��ħ��
int CResLoader::ReadGeneralMagic(void* obj, void* buf)
{
	ASSERT(obj && buf);
	IResObject* pMagic = (IResObject*)obj;
	SMagicArchive_General& ar = *(SMagicArchive_General*)buf;

	// read data from magic script
	if (!ReadBaseMagic(obj, buf))
		return FALSE;

	pMagic->GetPropertyLong("LoopStartIndex", (long&)ar.nLoopStartIndex, 0);
	pMagic->GetPropertyLong("LoopEndIndex", (long&)ar.nLoopEndIndex, 0);

	return TRUE;
}
//------------------------------------------------------------------------
// ����Χ���ܵ�����Ч
int CResLoader::ReadMouseMagic(void* obj, void* buf)
{
	ASSERT(obj && buf);
	IResObject* pMagic = (IResObject*)obj;
	SMagicArchive_Mouse& ar = *(SMagicArchive_Mouse*)buf;

	// read data from magic script
	if (!ReadBaseMagic(obj, buf))
		return FALSE;

	pMagic->GetPropertyLong("LoopStartIndex", (long&)ar.nLoopStartIndex, 0);
	pMagic->GetPropertyLong("LoopEndIndex", (long&)ar.nLoopEndIndex, 0);

	return TRUE;
}
//------------------------------------------------------------------------