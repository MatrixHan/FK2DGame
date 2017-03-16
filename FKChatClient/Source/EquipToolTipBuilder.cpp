/**
*	created:		2012-6-28   8:00
*	filename: 		EquipToolTipBuilder
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../Include/EquipToolTipBuilder.h"
#include "../Include/MiscFacade.h"
#include "../Include/ChatClient.h"
#include "../../FKGame/ClientWorld/IClientWorld.h"
#include "../../FKGame/ClientWorld/IThingTable.h"
#include "../../FKGame/Common/CreaturePropertyDef.h"
#include "../../FKGame/Effect/IEffectClient.h"
//------------------------------------------------------------------------
CEquipToolTipBuilder::CEquipToolTipBuilder()
{
	m_pEquipment = NULL;
	functions[0] = &CEquipToolTipBuilder::AddSoldPrice ;      			//�ۼ�
	functions[1] = &CEquipToolTipBuilder::AddName ;           			//����
	functions[2] = &CEquipToolTipBuilder::AddBind ;           			//��
	functions[3] = &CEquipToolTipBuilder::AddUnify ;          			//Ψһ
	functions[4] = &CEquipToolTipBuilder::AddCanSold ;        			//�ܷ����
	functions[5] = &CEquipToolTipBuilder::AddAttackSpeed ;    			//�����ٶ�
	functions[6] = &CEquipToolTipBuilder::AddPhysicalAttack ; 			//������
	functions[7] = &CEquipToolTipBuilder::AddMagicalAttack ;			//ħ������
	functions[8] = &CEquipToolTipBuilder::AddPhysicalDefence ;			//�������
	functions[9] = &CEquipToolTipBuilder::AddMagicalDefence ;			//ħ������  
	functions[10] = &CEquipToolTipBuilder::AddWeight ;					//����
	functions[11] = &CEquipToolTipBuilder::AddDur ;               		//�;�
	functions[12] = &CEquipToolTipBuilder::AddNeedLevel ;         		//��Ҫ�ȼ�
	functions[13] = &CEquipToolTipBuilder::AddNeedTrible ;				//����
	functions[14] = &CEquipToolTipBuilder::AddExtraAttribute ;    		//��������
	functions[15] = &CEquipToolTipBuilder::AddInlay ;             		//��Ƕ
	functions[16] = &CEquipToolTipBuilder::AddVocation ;          		//��Ҫְҵ
	functions[17] = &CEquipToolTipBuilder::AddRemain ;            		//ʣ�����
	functions[18] = &CEquipToolTipBuilder::AddDescription ;       		//����
	functions[19] = &CEquipToolTipBuilder::AddEquipPosition;      		//װ��λ��
	functions[20] = &CEquipToolTipBuilder::AddRank;               		//����
	functions[21] = &CEquipToolTipBuilder::AddSlotNum;            		//��Ӳ����
	functions[22] = &CEquipToolTipBuilder::AddBlank;					//��ӿ���	
	functions[23] = &CEquipToolTipBuilder::AddRepairPrice;				//�������۸�
	functions[24] = &CEquipToolTipBuilder::AddSuitEffect;				//��װ���
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddRepairPrice(void)
{
	int nCur = m_pEquipment->GetPropNum(GOODS_PROPID_ENDURE_CUR);
	int nCurMax = m_pEquipment->GetPropNum(GOODS_PROPID_ENDURE_CURMAX);
	int nPrice;
	if( nCur >= m_GoodsProp.lEndureMax || nCur <0 || m_GoodsProp.lBuyPrice <=0 || nCurMax<=0 )
	{
		nPrice = 0; 		
	}
	else
	{
		nPrice  = static_cast<int>( ((float)(nCurMax-nCur))/((float)m_GoodsProp.lEndureMax)*((float)m_GoodsProp.lBuyPrice) ); 
	}

	if (nPrice == 0)
	{
		return ;
	}

	int nGold = nPrice / 10000;
	int nSilver = (nPrice % 10000) / 100;
	int nCopper = nPrice % 100;

	char temp[64];
	sprintf(temp, "[:255,252,000,c]����۸�: ");	
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
CEquipToolTipBuilder::~CEquipToolTipBuilder()
{

}
//------------------------------------------------------------------------
BOOL CEquipToolTipBuilder::SetObject(DWORD dwGoodsID, UID uid)
{
	strToolTip.clear();	
	m_pEquipment = (IEquipment *)g_pGlobal->m_pClientWorld->GetThingTable()->Find(uid);
	if (m_pEquipment == NULL)
	{
		return FALSE;
	}
	m_GoodsProp = g_pGlobal->m_pDataCenter->GetEquipmentBaseProperty(dwGoodsID, m_pEquipment->GetPropNum( GOODS_PROPID_SMELTLEVEL));	

	return TRUE;
}
//------------------------------------------------------------------------
//��һ���¶�������TOOLTIP������
BOOL CEquipToolTipBuilder::SetObject(DWORD dwGoodsID, IGoods *pGoods)
{
	strToolTip.clear();	
	m_pEquipment = (IEquipment *)pGoods;
	m_GoodsProp = g_pGlobal->m_pDataCenter->GetEquipmentBaseProperty(dwGoodsID, m_pEquipment->GetPropNum( GOODS_PROPID_SMELTLEVEL));	
	return TRUE;
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddItem(int nItemID)
{
	if (m_pEquipment == NULL)
	{
		return;
	}

	if (nItemID >= 0 && nItemID < enUnknown)
	{		
		(this->*functions[nItemID])();
	}
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddSoldPrice(void)      //�ۼ�
{	
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
void CEquipToolTipBuilder::AddName(void)            //����
{	
	static const char *formatstr[6] = {
		"[wc]",
		"[:035,255,000,c]",
		"[:010,101,251,c]",
		"[:247,006,233,c]",
		"[:255,132,000,c]",
		"[:255,000,000,c]",
	};

	static const int MapTable[GOODS_EFFECT_MAXCOUNT + 1] = 
	{0, 1,1,1, 2,2, 3,3,4,5,5};		

	LPCSTR szFormat = formatstr[MapTable[__GetEffectCount()]];

	strToolTip = strToolTip + szFormat + m_GoodsProp.szName ;

	//�����ȼ�
	int nSmeltLevel = m_pEquipment->GetPropNum( GOODS_PROPID_SMELTLEVEL);
	if (nSmeltLevel > 0)
	{
		char temp[4];		
		sprintf(temp, "%d", nSmeltLevel);
		strToolTip = strToolTip + " +" + temp +"\r\n";
	}
	else
	{
		strToolTip = strToolTip + "\r\n";
	}
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddBind(void)            //��
{
	if (m_GoodsProp.lHoldBind)
	{
		if (m_pEquipment->GetBindFlag(GOODS_BINDFLAG_SETHOLD))
		{
			strToolTip = strToolTip + "[:255,153,000,c]�Ѱ�" + "\r\n";			
		}
		else
		{
			strToolTip = strToolTip + "[:255,153,000,c]ʰȡ���" + "\r\n";
		}
		return;
	}

	if (m_GoodsProp.lUseBind)
	{
		if (m_pEquipment->GetBindFlag(GOODS_BINDFLAG_SETUSE))
		{
			strToolTip = strToolTip + "[:255,153,000,c]�Ѱ�" + "\r\n";	
		}
		else
		{
			strToolTip = strToolTip + "[:255,153,000,c]ʹ�ú��" + "\r\n";
		}
		return;
	}	
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddUnify(void)           //Ψһ
{
	//Ψһ	
	if (m_GoodsProp.lHoldNum == 1)
	{
		strToolTip = strToolTip + "[wc]Ψһ" + "\r\n";
	}
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddCanSold(void)         //�ܷ����
{	
	//�޷�����
	if (m_pEquipment->GetBindFlag(GOODS_BINDFLAG_SALE) == FALSE)
	{
		strToolTip = strToolTip + "[wc]�޷�����" + "\r\n";
	}
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddAttackSpeed(void)     //�����ٶ�
{
	//�����ٶ�
	static const char *szSleeptable[] = {"����","����","����","��","һ��","��","�Ͽ�","�ܿ�","����"};						
	if (m_GoodsProp.lAttackSpeed >= 0 && m_GoodsProp.lAttackSpeed < 9) 
	{
		strToolTip = strToolTip + "[wc]" + "�����ٶ�: " + szSleeptable[m_GoodsProp.lAttackSpeed] + "\r\n";
	}
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddPhysicalAttack(void)  //������
{ 
	//��������
	char temp[100];
	int nMinAtt = m_GoodsProp.lP_AttackMin;
	int nMaxAtt = m_GoodsProp.lP_AttackMax;
	if (nMaxAtt > 0)
	{
		sprintf(temp, "[wc]������: %d - %d", nMinAtt, nMaxAtt);
		strToolTip = strToolTip + temp + "\r\n";
	}	
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddMagicalAttack(void)         //ħ������
{
	char temp[100];
	int nMinAtt = m_GoodsProp.lM_AttackMin;
	int nMaxAtt = m_GoodsProp.lM_AttackMax;
	if (nMaxAtt > 0)
	{
		sprintf(temp, "[wc]��������: %d - %d", nMinAtt, nMaxAtt);
		strToolTip = strToolTip + temp + "\r\n";
	}	
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddPhysicalDefence(void)     //�������
{
	char temp[100];
	int nMinDef = m_GoodsProp.lP_DefendMin;
	int nMaxDef = m_GoodsProp.lP_DefendMax;
	if (nMaxDef > 0)
	{
		sprintf(temp, "[wc]�������: %d - %d", nMinDef, nMaxDef);
		strToolTip = strToolTip + temp + "\r\n";
	}	
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddMagicalDefence(void)        //ħ������  
{
	char temp[100];
	int nMinDef = m_GoodsProp.lM_DefendMin;
	int nMaxDef = m_GoodsProp.lM_DefendMax;
	if (nMaxDef > 0)
	{
		sprintf(temp, "[wc]��������: %d - %d", nMinDef, nMaxDef);
		strToolTip = strToolTip + temp + "\r\n";
	}	
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddWeight(void) 		     //����
{
	char temp[32];
	sprintf(temp, "[wc]����: %d\r\n", m_GoodsProp.lWeight);
	strToolTip = strToolTip + temp;
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddDur(void)                //�;�
{
	//�;�	
	char temp[48];
	int nMaxDup  = m_pEquipment->GetPropNum(GOODS_PROPID_ENDURE_CURMAX);
	int nNowDup  = m_pEquipment->GetPropNum(GOODS_PROPID_ENDURE_CUR);			
	if (nNowDup == 0)
	{
		sprintf(temp, "[rc]�;�: %d / %d", 0, nMaxDup / EQUIPMENT_ENDURE_SHOWSPACE);	
	}	
	else
	{
		int nPlus = 1;
		if (nMaxDup == nNowDup)
		{
			nPlus = 0;
		}
		sprintf(temp, "[wc]�;�: %d / %d", nNowDup / EQUIPMENT_ENDURE_SHOWSPACE + nPlus, nMaxDup / EQUIPMENT_ENDURE_SHOWSPACE);
	}

	strToolTip = strToolTip + temp + "\r\n";
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddNeedLevel(void)          //��Ҫ�ȼ�
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
void CEquipToolTipBuilder::AddNeedTrible(void)        //����
{

}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddExtraAttribute(void)     //��������
{
	//��������
	LPCSTR szStr = NULL;
	for (int i = 0; i < GOODS_EFFECT_MAXCOUNT; ++i)
	{//��ÿ�����Բ�ѯһ��
		IEffect *pEffect = m_pEquipment->GetEffect(i);
		if (pEffect && strlen(szStr = pEffect->GetDescription()) > 0)
		{//������Ч��	
			switch(i) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				strToolTip = strToolTip + "[:252,248,144,c]" + szStr + "\r\n";
				break;
			case 6:
				strToolTip = strToolTip + "[:255,132,000,c]" + szStr + "\r\n";
				break;
			default:
				strToolTip = strToolTip + "[:088,208,243,c]" + szStr + "\r\n";
			}
		}	
	}
}
//------------------------------------------------------------------------
int  CEquipToolTipBuilder::__GetEffectCount(void)
{
	int nCount = 0;
	if (m_pEquipment)
	{
		for (int i = 0; i < GOODS_EFFECT_MAXCOUNT; ++i)
		{
			IEffect *pEffect = m_pEquipment->GetEffect(i);
			if (pEffect)
			{
				nCount ++;			
			}					
		}
	}	
	return nCount;
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddInlay(void)              //��Ƕ
{

}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddVocation(void)           //��Ҫְҵ
{
	// ʹ��ְҵ
	char temp[64];
	char *szOccupationTable[] = {"սʿ", "��ʦ", "��ʦ", "����"};			
	if (m_GoodsProp.lAllowOccupation != 0 && m_GoodsProp.lAllowOccupation <= 4)
	{
		int nCurrentVocation = 0;
		if (g_pGlobal->m_pPlayer)
		{
			nCurrentVocation = g_pGlobal->m_pPlayer->GetPropNum(CREATURE_PROPID_OCCUPATION);
		}

		if (nCurrentVocation == m_GoodsProp.lAllowOccupation)
		{
			sprintf(temp, "[wc]��Ҫְҵ: %s", szOccupationTable[m_GoodsProp.lAllowOccupation - 1]);
		}
		else
		{
			sprintf(temp, "[:255,000,000,c]��Ҫְҵ: %s", szOccupationTable[m_GoodsProp.lAllowOccupation - 1]);	
		}

		strToolTip = strToolTip + temp + "\r\n";
	}
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddRemain(void)             //ʣ�����
{

}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddDescription(void)        //����
{
	AddBlank();
	//��Ʒ����	
	strToolTip = strToolTip + "[:035,255,000,c]" + m_GoodsProp.szDesc;
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddEquipPosition(void)
{	
	//װ��λ��
	const static char *szPartName[] = {
		"ͷ��"
		, "�粿"
		, "�ز�"
		, "�Ȳ�"			
		, "��Ʒ" 
		, "����" 
		, "��ָ" 
		, "����" 
		, "����" 
		, "��ָ"
		, "����" 
		, "�Ų�" 
		, "����" 
		, "����" 
		, "����" 
		, "����" 
	};

	if (m_GoodsProp.lEquipPos1 >= 0 && m_GoodsProp.lEquipPos1 <= 15)
	{
		strToolTip = strToolTip + "[wc]" + szPartName[m_GoodsProp.lEquipPos1]+ "\r\n";
	}
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddRank(void)
{
	char temp[32];
	sprintf(temp, "����: %d\r\n", m_GoodsProp.lRank);
	strToolTip = strToolTip + "[wc]" + temp;
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddSlotNum(void)
{
	if (m_pEquipment)
	{
		int nSlotNum = m_pEquipment->GetSlotLeft();
		if (nSlotNum)
		{
			char temp[32];
			sprintf(temp, "ʣ�� %d �����\r\n", nSlotNum);
			strToolTip = strToolTip + "[:088,208,243,c]" + temp;	
		}		
	}		
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddBlank(void)
{
	strToolTip = strToolTip + "\r\n";
}
//------------------------------------------------------------------------
void CEquipToolTipBuilder::AddSuitEffect(void)
{
	if (g_pGlobal->m_pDataCenter == NULL)
	{
		return;
	}

	SUITEQUIP_EFFECT *pSuitEffect = g_pGlobal->m_pDataCenter->GetSuitEquipEffect(m_GoodsProp.lSuitEquipID);
	if (pSuitEffect == NULL
		|| pSuitEffect->lMaxPiece < 2)  
	{
		return;
	}

	//���Ҵ����˶��ټ�װ��
	int nEquipCount =  MISCFACADE::GetSuitEquipedCount( m_GoodsProp.lSuitEquipID );

	char temp[255];
	sprintf(temp, "[:035,255,000,c]%s (%d/%d)\r\n", pSuitEffect->szName, nEquipCount, pSuitEffect->lMaxPiece);
	strToolTip = strToolTip + temp;

	IEffectFactory *pEffectFactory = g_pGlobal->GetEffectFactory();
	if (nEquipCount > 0
		&& nEquipCount < EQUIPPOS_INDEX_MAX
		&& pEffectFactory)
	{
		for (int i = 0; i < 12; ++i)
		{
			int nEffectID = pSuitEffect->lEffect[nEquipCount][i];
			if (nEffectID != 0)
			{
				strToolTip = strToolTip + "��װ:" + g_pGlobal->GetEffectFactory()->GetDescription(nEffectID)
					+ "\r\n";
			}				
		}			
	}
}
//------------------------------------------------------------------------