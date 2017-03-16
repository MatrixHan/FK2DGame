/**
*	created:		2012-6-28   7:43
*	filename: 		EquipToolTipBuilder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		�������������װ����TOOLTIP
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "IToolTipGenerator.h"
#include "../../FKGame/Common/IConfigDataCenter.h"
#include "../../FKGame/SceneObj/IEquipment.h"
//------------------------------------------------------------------------
class CEquipToolTipBuilder : public CToolTipGenerator  
{
public:
	enum
	{
		enSoldPrice,      		//�ۼ�
		enName,           		//����
		enBind,           		//��
		enUnify,          		//Ψһ
		enCanSold,        		//�ܷ����
		enAttackSpeed,    		//�����ٶ�
		enPhysicalAttack, 		//������
		enMagicalAttack,        //ħ������
		enPhysicalDefence,		//�������
		enMagicalDefence,       //ħ������  
		enWeight,		     	//����
		enDur,               	//�;�
		enNeedLevel,         	//��Ҫ�ȼ�
		enNeedTrible,        	//����
		enExtraAttribute,    	//��������
		enInlay,             	//��Ƕ
		enVocation,          	//��Ҫְҵ
		enRemain,            	//ʣ�����
		enDescription,       	//����
		enEquipPosition,     	//װ��λ��
		enRank,
		enSlotNum,           	//�����
		enBlank,             	//����
		enRepairPrice,       	//����۸�
		enSuitEffectDesc,    	//��װЧ�����
		enUnknown,           	//δ֪����
	};

private:
	EQUIPMENT_BASEPROPERTY m_GoodsProp;
	IEquipment *m_pEquipment;
	
	//ÿ��С��������TOOLTIP������Լ�������
	void AddSoldPrice(void);      			//�ۼ�
	void AddName(void);           			//����
	void AddBind(void);           			//��
	void AddUnify(void);          			//Ψһ
	void AddCanSold(void);        			//�ܷ����
	void AddAttackSpeed(void);    			//�����ٶ�
	void AddPhysicalAttack(void); 			//������
	void AddMagicalAttack(void);        	//ħ������
	void AddPhysicalDefence(void);			//�������
	void AddMagicalDefence(void);       	//ħ������  
	void AddWeight(void);		     		//����
	void AddDur(void);               		//�;�
	void AddNeedLevel(void);         		//��Ҫ�ȼ�
	void AddNeedTrible(void);				//����
	void AddExtraAttribute(void);    		//��������
	void AddInlay(void);             		//��Ƕ
	void AddVocation(void);          		//��Ҫְҵ
	void AddRemain(void);            		//ʣ�����
	void AddDescription(void);       		//����
	void AddEquipPosition(void);       		//װ��λ��
	void AddRank(void);                		//����
	void AddSlotNum(void);             		//�ӿ���
	void AddBlank(void);               		//����
	void AddRepairPrice(void);         		//�޸��۸�
	void AddSuitEffect(void);				//��װЧ��

	typedef void (CEquipToolTipBuilder::*ITEMFUNCTION)(void);
	ITEMFUNCTION functions[enUnknown];

	int __GetEffectCount(void);
public:

	CEquipToolTipBuilder();
	virtual ~CEquipToolTipBuilder();

	//��һ���¶�������TOOLTIP������
	BOOL SetObject(DWORD dwGoodsID, UID uid);
    //��һ���¶�������TOOLTIP������
	BOOL SetObject(DWORD dwGoodsID, IGoods *pGoods);
	//Ϊ��Ʒ��TOOLTIP���һ����
	void AddItem(int nItemID);	
};
//------------------------------------------------------------------------