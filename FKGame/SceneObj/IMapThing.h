/**
*	created:		2012-6-23   0:23
*	filename: 		IMapThing
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
// ˵��:
//				  �����ڵ�ͼ����ITEM�Ķ��������һЩͨ�õĲ���
//				  ����Release��ʱ��Ҫ�ӵ�ͼ��ɾ��MapItem��
//				  ����Ҫ������д�ͻ��˴���(THING.DLL)��ʱ����
//				  �����������㵽һ��ITEM��ʱ����Ҫ�ж����Item
//				  �ǲ������˰�����������˾͵������򴰿ڣ��ǲ���
//				  ���ﰡ������ǹ���Ͷ�����й����ȵȵȵȡ�����
//				  ���䷱���Ҳ�����.
//				  ���ǵ�ITEM�Ͽ��Խ��ܵĲ����������Խ��,������
//				  �Ҽ��������סCTRL+�Ҽ����...Ϊ������չ���趨��
//				  һ��OnMouseLeftDown,OnMouseOver�Ľӿڵķ����Ͳ�
//				  �Ǻܺ�,�����������Visitor�����ģʽ,���в�����
//				  ֮����ָ����
//				 
//				  ��: 
//				  ���ַ����Ƚӿڷ�ʽ����һ���ȽϺõĵط����������
//				  �㵽ITEM����ʲô������ΪThingFactory�Ҳ��������
//				  ���ǿͻ����ϲ�Ĵ����ڹ�����,ThingFactoryֻ����
//				  ÿ��Thing�Լ����߼�,��Ҫ�����ݺͷ�����ͬ��,����
//				  �ù��ľ�����Ϸ�߼���
//------------------------------------------------------------------------
#include "IThing.h"
//------------------------------------------------------------------------
// ����
class CMapItemView;
//------------------------------------------------------------------------
// ����
struct IMapItemVisitor
{
	virtual void VisitHero(void)		{ };
	virtual void VisitActor(void)		{ };
	virtual void VisitMonster(void)		{ };
	virtual void VisitGoods(void)		{ };
	virtual void VisitChapman(void)		{ };
	virtual void VisitSmith(void)		{ };
	virtual void VisitLode(void)		{ };
	virtual void VisitSnare(void)		{ };
	virtual void VisitChunnel(void)		{ };
	virtual void VisitBody(void)		{ };
	virtual void VisitWareHouse(void)	{ };
	virtual void VisitGuard(void)		{ };
	virtual void VisitTaskNPC(void)		{ };
	virtual void VisitBox(void)			{ };
	virtual void VisitStall(void)		{ };
};
//------------------------------------------------------------------------
struct IMapThing : public IThing
{
	// visitor
	virtual void			Accept(IMapItemVisitor &visitor) = NULL;
	// ȡ�õ�����������ͼ�ӿ�
	virtual CMapItemView *	GetMapItem(void) const = NULL;
	// ������ͼ
	virtual void			SetMapItem(CMapItemView * pMapItemView) = NULL;
};
//------------------------------------------------------------------------