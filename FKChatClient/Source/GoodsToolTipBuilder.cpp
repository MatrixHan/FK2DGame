/**
*	created:		2012-6-28   8:05
*	filename: 		GoodsToolTipBuilder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/GoodsToolTipBuilder.h"
#include "../IChatClient.h"
#include "../Include/ChatClient.h"
#include "../../FKGame/Common/CreaturePropertyDef.h"
//------------------------------------------------------------------------
CGoodsToolTipBuilder::CGoodsToolTipBuilder()
{
	m_pGoods = NULL;
}
//------------------------------------------------------------------------
CGoodsToolTipBuilder::~CGoodsToolTipBuilder()
{

}
//------------------------------------------------------------------------
//��һ���¶�������TOOLTIP������
BOOL CGoodsToolTipBuilder::SetObject(DWORD dwGoodsID, UID uid)
{
	CGoodsToolTipCommon::SetObject(dwGoodsID,  uid);

	m_GoodsProp = g_pGlobal->m_pDataCenter->GetGoodsBaseProperty(dwGoodsID);	

	return TRUE;
}
//------------------------------------------------------------------------
//��һ���¶�������TOOLTIP������
BOOL CGoodsToolTipBuilder::SetObject(DWORD dwGoodsID, IGoods *pGoods)
{
	CGoodsToolTipCommon::SetObject(dwGoodsID,  pGoods);
	m_GoodsProp = g_pGlobal->m_pDataCenter->GetGoodsBaseProperty(dwGoodsID);	
	return TRUE;
}
//------------------------------------------------------------------------
//Ϊ��Ʒ��TOOLTIP���һ����
void CGoodsToolTipBuilder::AddItem(int nItemID)
{
	switch(nItemID) {
	case enSoldPrice://�ۼ�
		AddSoldPrice();
		break;
	case enName://����
		AddName();
		break;
	case enNeedLevel:
		AddNeedLevel();
		break;
	case enDescription://����
		AddDescription();
		break;
	case enGoodsType:
		AddGoodsType();
		break;
	case enRank:
		AddRank();
		break;
	case enBlank:
		AddBlank();
		break;	
	case enBind:
		AddBind();
		break;
	}
}
//------------------------------------------------------------------------
void CGoodsToolTipBuilder::AddNeedLevel(void)
{
	char temp[48];
	if (m_GoodsProp.lAllowLevel > 0)
	{
		int nCurrentLevel = 0;
		if (g_pGlobal->m_pPlayer)
		{
			nCurrentLevel = g_pGlobal->m_pPlayer->GetPropNum(CREATURE_PROPID_LEVEL);
		}

		if (nCurrentLevel < m_GoodsProp.lAllowLevel)
		{
			sprintf(temp, "[:255,000,000,c]��Ҫ�ȼ�: %d", m_GoodsProp.lAllowLevel + 1);		
		}
		else
		{
			sprintf(temp, "[wc]��Ҫ�ȼ�: %d", m_GoodsProp.lAllowLevel + 1);			
		}

		strToolTip = strToolTip + temp + "\r\n";
	}
}
//------------------------------------------------------------------------
void CGoodsToolTipBuilder::AddSoldPrice(void)      //�ۼ�
{
	if (m_GoodsProp.lSellPrice <= 0)
	{
		return;
	}

	int nGold = m_GoodsProp.lSellPrice / 10000;
	int nSilver = (m_GoodsProp.lSellPrice % 10000) / 100;
	int nCopper = m_GoodsProp.lSellPrice % 100;

	char temp[64];
	sprintf(temp, "[:255,252,000,c]�����۸�: ");
	strToolTip = temp;

	if (nGold > 0)
	{
		sprintf(temp, "%d��", nGold);
		strToolTip = strToolTip + temp;		
	}

	if (nSilver > 0)
	{
		sprintf(temp, "%d��", nSilver);
		strToolTip = strToolTip + temp;		
	}

	if (nCopper > 0)
	{
		sprintf(temp, "%dͭ", nCopper);
		strToolTip = strToolTip + temp ;		
	}	
	strToolTip = strToolTip + "\r\n";
}
//------------------------------------------------------------------------
void CGoodsToolTipBuilder::AddName(void)            //����
{	
	std::string strFormat;
	if (m_GoodsProp.lClass == GOODS_CLASS_WARRANT)
	{
		switch(m_GoodsProp.lSubClass) {
		case WARRANT_SUBCLASS_STONE:
			strFormat = "[:088,208,243,c]";
			break;
		case WARRANT_SUBCLASS_RARE_ORE:
		case WARRANT_SUBCLASS_RARE_HERBAGE:
		case WARRANT_SUBCLASS_RARE_STONE:
			strFormat = "[:035,255,000,c]";
			break;
		default:
			strFormat = "[wc]";
		}
	}
	else
	{
		strFormat = "[wc]";
	}

	strToolTip = strToolTip + strFormat + m_GoodsProp.szName + "\r\n";
}
//------------------------------------------------------------------------
void CGoodsToolTipBuilder::AddDescription(void)
{
	//��Ʒ����	
	strToolTip = strToolTip + "[wc]" + m_GoodsProp.szDesc;
}
//------------------------------------------------------------------------
void CGoodsToolTipBuilder::AddGoodsType(void)
{
	//��Ʒ����	
	if (m_GoodsProp.lClass == GOODS_CLASS_EQUIPMENT)
	{
		return;
	}

	switch(m_GoodsProp.lClass) {
	case GOODS_CLASS_MEDICAMENT:
		{
			if (m_GoodsProp.lSubClass == 1)
			{
				strToolTip = strToolTip + "[wc]ҩƷ\r\n";	
			}
			else
			{
				strToolTip = strToolTip + "[wc]��Ʒ\r\n";	
			}
		}
		break;
	case GOODS_CLASS_WARRANT:
		{
			const static char* name[] = {"δ֪","��ʯ", "��ҩ", "����", "��ʯ", "������Ʒ", "����"};
			if (m_GoodsProp.lSubClass > 0 && m_GoodsProp.lSubClass <= WARRANT_SUBCLASS_OTHER)
			{
				strToolTip = strToolTip + "[wc]" + name[m_GoodsProp.lSubClass] + "\r\n";	
			}			
		}
		break;
	case GOODS_CLASS_VALUABLES:

		break;
	}	
}
//------------------------------------------------------------------------
void CGoodsToolTipBuilder::AddRank(void)
{
	if (m_GoodsProp.lSubClass == 0)
	{
		//С��Ϊ0, ����ʾ���� 
		return;
	}

	char temp[32];
	sprintf(temp, "����: %d\r\n", m_GoodsProp.lRank);
	strToolTip = strToolTip + "[wc]" + temp;
}
//------------------------------------------------------------------------
void CGoodsToolTipBuilder::AddBlank(void)
{
	strToolTip = strToolTip + "\r\n";
}
//------------------------------------------------------------------------