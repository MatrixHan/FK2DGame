/**
*	created:		2012-6-26   3:09
*	filename: 		WinMsgDef
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����ϵͳ�������볣���Ķ���
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
// ȫ����Ϣ
#define IDM_FULLSCREEN                  1016
// ��Ҿ�����Ϣ
#define IDM_CENTER                      1015
// ������Ⱦģʽ��Ϣ
#define IDM_FLAG                        1017
// PING��Ϣ
#define IDM_PING                        1112
// ��ȡ��������Ϣ
#define IDM_GET_SERVER_INFO             1113
//------------------------------------------------------------------------
// �������ô���
#define WND_GRAPHICSSETTING                   183
const int WE_GRAPHICSSETTING_APPLYSETTING        = 1;    //�������ô���, Ҫ���������������趨
const int WE_GRAPHICSSETTING_SETSKILLDATAFLAG    = 2;    //���ü�����Чλ,��ʾ���������Ѿ�������
const int WE_WC_GRAPHICSSETTING_SETLOCKFIRE      = 3;    //�����Զ�����״̬, WPARAMΪ״̬
const int WE_AW_GRAPHICSSETTING_FULLSCREENMODE   = 4;    //����ȫ��״̬, WPARAMΪ״̬
const int WE_WC_GRAPHICSSETTING_DISPACTORNAME    = 5;    //������ʾ�������, WPARAMΪ״̬
const int WE_WC_GRAPHICSSETTING_DISPCREATUREHP   = 6;    //��ʾ����Ѫ��, WPARAMΪ״̬
//------------------------------------------------------------------------
//��ʽ WE_ + CW/WC + �������� + ��Ϣ����
const int WE_CW_ALLWND_SHOW				= 99;		//ÿ��������ʾ������ʱ�����ᷢ�ʹ���Ϣ��WPARAMΪ1ʱ����ʱ���ͣ�WPARAMΪ0ʱ������ʱ����

//֪ͨ����ע��(ȡ��ע��)��ݼ�
const int WE_AW_ALLWND_REGACCELE          = 1000;     //�������ʼ����, ��������Ҫ�Ĵ��ڷ��ʹ���Ϣ, 
const int WE_AW_ALLWND_UNREGACCELE        = 1001;     //�л���ͼʱ, ��������Ҫ�Ĵ��ڷ��ʹ���Ϣ, ֪ͨ��ȡ��ע���ݼ�


#define WND_CUSTOMMSGBOX				10			//�Զ���Ի���
// ������
const int WE_CW_MSGBOX_CANCEL				= 0;	//����һ���Ի���
const int WE_CW_MSGBOX_CONFIRM				= 1;	//����һ���Ի���
const int WE_CW_MSGBOX_SHOW					= 2;	//���õ�����Ϣ������趨��Ϊ�Զ�����Ϣ��WPARAMΪ���ݴ�С��LARPAMΪSETCUSTOMMSGBOX�ṹ
const int WE_CW_MSGBOX_RETRY				= 3;    //����
const int WE_CW_MSGBOX_QUIT					= 4;    //�˳�
const int WE_CW_MSGBOX_SETBTN				= 5;    //WPARM:1 = ȷ����2 = ȡ����3 = ���ԣ� 4 = �˳��� LPARAMΪ��Ӧ�Ļص�����
const int WE_CW_MSGBOX_SETCONTEXT			= 6;    //�趨���ƶԻ���������ģ�����趨�ˣ�����û������˴˶Ի���ʱ�����ظ��û�
const int WE_CW_MSGBOX_HIDE					= 7;    //������ǰ������Ϣ��
const int WE_CW_MSGBOX_POP					= 8;	//�öԷ���ֹͣ���߿�ʼ������WPARM:1 = ������2 = ֹͣ
const int WE_CW_MSGBOX_TIMER				= 9;	//�趨����ʱ������WPARM:1 = ��ת��2 = ֹͣ
const int WE_CW_MSGBOX_UPDATETEXT			= 10;	//���µ�ǰ������ʾ���ı�,(ֻ�����ı�,���ڲ��ر�) WPARM Ϊ�ַ�ָ��
const int WE_CW_MSGBOX_CLEAR				= 11;   //������Ϣ����������е���Ϣ,���رյ�ǰ��Ϣ����(���������ʾ�Ļ�)
const int WE_AW_MSGBOX_GETCOURSE			= 12;	//��ѯ���ڵ�����ԭ�� WPARAM Ϊint* , ���ڽ���ԭ��
const int WE_AW_MSGBOX_CLEARONCE			= 13;	//���һ��Ի��� WPARAM Ϊԭ��, ��ָ��,��ǰ����ԭ����˲���һ��ʱ,�Ż�����
const int WE_AW_MSGBOX_SETCURSORTOBTN		= 14;	//������Ƶ�ĳ��BTN��, wParamΪ��ť����; ���������Ч(��ť����ʾ),û���κη�Ӧ



enum    //����ԭ��Ի�����������ԭ��
{
	POPCOURSE_UNKNOWN = 0,    			//����ԭ��
	POPCOURSE_TASKLOG,        			//������־
	POPCOURSE_DIVIDEGOODS,    			//Ϊ�����Ʒ����
	POPCOURSE_DISCARDGOODS,     		//Ϊ��������
	POPCOURSE_RESELECTACTOR,    		//Ϊ��ѡ��ɫ����
	POPCOURSE_EXITGAME,         		//Ϊ�뿪��Ϸ����
	POPCOURSE_FORGETSKILL,				//Ϊ���Ǽ��ܶ���
	POPCOURSE_TEAM,						//�����ԭ��
	POPCOURSE_QQLOGIN,					//QQ��¼��ԭ��	
};
#define MAKECOURSE(MAIN, SUB)  ( ((MAIN)&0x00ffff)  | (((SUB)&0x00ffff) << 16))


#define WND_INPUTWND					20			//���봰��

#define WND_INTRO						30			// ��������
const int WE_AW_INTRO_CANCELMOVIE				= 10;  //��Ҿ���ȡ����������, �ɶԻ���ص�������
const int WE_AW_INTRO_TRYMOVIEAGAIN				= 11;  //��Ҿ����ٳ��Զ�������, �ɶԻ���ص�������


#define WND_SELECTWORLD					40			// ѡ����Ϸ���細��

//��½�� WND_LOGIN
const int WE_WC_LOGIN_CONFIRM				= 0;	//�����½	wParam : �û��� lParam : PASSWORDS�ṹָ��
const int WE_WC_LOGIN_CANCEL				= 1;	//ȡ����½
const int WE_WC_LOGIN_REGISTER				= 2;	//ע��
const int WE_CW_LOGIN_USERNAME				= 7;	//Ĭ�ϵ�½�û���
const int WE_CW_LOGIN_MSG					= 8;	//������ʾ��Ϣ
const int WE_CW_LOGIN_LOCK       			= 12;   //��������,���û�����ѡ��
const int WE_CW_LOGIN_UNLOCK       			= 13;   //�����������
const int WE_CW_LOGIN_WAIT       			= 14;   //��¼ʱ��Ҫ�Ŷ�   wParam������ǰ�������
const int WE_CW_LOGIN_ERRORINFO       		= 15;   //��¼ʱ,�ӷ��������صĴ�����Ϣ
const int WE_AW_LOGIN_RELOGIN       		= 16;   //��anywhere������,Ҫ���������ӵ�¼��
const int WE_CW_LOGIN_CLEARPASSWORD    		= 17;   //������������, Ҫ�����������е�����
const int WE_WW_LOGIN_SHOWCAPTIONWARNING	= 18;   //���ô�Сд�������ʾ���
const int WE_WW_LOGIN_SOFTKBINPUT       	= 19;   //�û���������������ˣ�wParam ΪLPCSTR,ָ���û����µļ���
const int WE_WW_LOGIN_QQLOGIN           	= 20;   //���������ڻ���Ƶ���ڴ���, Ҫ����QQ��ʽ��¼
const int WE_WC_LOGIN_SETPASSPORT         	= 21;   //�����ܱ�, wParamΪ�Ƿ�ʹ���ܱ�, lParam Ϊ�ܱ�ָ��LPCSTR
const int WE_WW_LOGIN_INITGRAPHICSCOMPLETE 	= 22;   //��ʼ������ͼ�����
const int WE_WW_LOGIN_GETACCOUNTNAME    	= 23;   //ȡ�ʺ���, wParamΪ������ָ��, lParamΪ��������С
const int WE_WC_SETEXTRAPASSWORD         	= 24;   //���ø�������, wParamΪ��������LPCSTR


#define WND_LOGIN				    		50			// ��¼����
#define WND_LOGIN_BAK						52			// ��¼���ڵı���
const int WE_WW_LOGINBAK_SHOWLABEL      	= 40;   //��ʾ�ײ���LABEL�ؼ�, wParamΪ�Ƿ���ʾ, lParamΪ��ʾ����LPCSTR

#define WND_LOGIN_BAK_NOTICE				54			// ��¼���ڹ���


#define WND_MINIMAP						60			//С��ͼ
//С��ͼ����
const int WE_CW_MAP_REFRESH					= 0;
const int WE_CW_MAP_SETMAPAREA				= 1;
const int WE_WW_MAP_SETREGIONTIP			= 2;    //���õ�ǰ�������ʾ, wPARAMΪLPCSTR

const int WE_CW_CHAT_ACTIVATE       		= 9999;   //��������Ի���


#define WND_ACTORSELECT					80				// ��ɫѡ�񴰿�
#define WND_ACTORSELECT_BAK				81				// ��ɫѡ�񴰿ڱ���
//��ɫѡ�񴰿� WND_ACTORSELECT
const int WE_WC_ACTOR_ENTERGAME				=	0;		//������Ϸ
const int WE_WC_ACTOR_QUITGAME				=	1;		//�˳���Ϸ
const int WE_WC_ACTOR_DELETE				=	3;		//ɾ����ɫ
const int WE_CW_ACTOR_ERROR                 =   4;       //������Ϣ 
const int WE_CW_REFRESH_ACTORINFO			=	7;		//֪ͨ����,ˢ���µĽ�ɫ����(wParamΪ������ָ��,lParamΪ���ݳ���)
const int WE_CW_ACTOR_SETLOCK			    =	8;		//֪ͨ����,����(wParamΪ��������)
const int WE_AW_ACTORSELECT_CLEARPORT	  	=	9;		// С��ʱ������, ���������ɫ�Ķ˿���Ϣ, ��ֹ�û���û����ͨ��¼��ǰ,���½�����Ϸ


// ��ɫ��������
#define WND_ACTORCREATE					82
//��ɫ�������� WND_ACTORCREATE
const int WE_WC_ACTORCREATE_QUEST	=	0;		//����������� ������ɫ����
const int WE_CW_ACTORCREATE_ERROR   =   1;      //������������, ��ʾ������Ϣ



//���˵�����
#define WND_MAIN                         90	
const int WE_CW_MAIN_SHORTCUT               = 0;   //������������,Ҫ������ҵĿ��ָ��,
												   //��ڲ���: wParam ��������������ĵ�ַ, ex.  21��ʾ2��1��
												   //          lParam ��һ��int �ĵ�ַ,���ڷ��ؼ���ID
												   //���ڲ���: wParam ���ڹ������Ƿ����˸�ָ��,���缼�ܵ��ǲ��ᴦ���
												   //          lParam ����û�д���ʱ,���ֵ�Ǽ���ID			

const int WE_WC_MAIN_SKILL               = 1;   //���ڴ���������,��ʾ�����������˿��������
const int WE_WC_MAIN_GOODS               = 2;   //���ڴ���������,��ʾ�����������˿������Ʒ wParamΪĿ������, lParamΪUID�ĵ�ַ  
const int WE_AW_MAIN_SAVESHORTCUT        = 3;   //Ҫ�󱣴��û��Ŀ�ݼ����趨
const int WE_AW_MAIN_LOADSHORTCUT        = 4;   //Ҫ���ȡ�û��Ŀ�ݼ����趨
const int WE_CW_MAIN_SKILL_SELF          = 5;   //�������������Լ�ʹ�ü���,  wParamΪ����ID
const int WE_AW_MAIN_CHANGEPKMODE        = 6;   //��PKģ�鷢����,֪ͨ����PKģʽ . wParamΪ��ǰģʽ
const int WE_CW_MAIN_MSGCENTER_PUSH      = 7;   //������������,��ʾ��Ϣ��������Ϣ��ȥ��, wParamΪ���к�
const int WE_CW_MAIN_MSGCENTER_STOPFLASH = 8;   //������������,��ʾ����ֹͣ������, wParamΪ���к�
const int WE_WC_MAIN_MSGCENTER_POP       = 9;   //����������ȥ , ��ʾ��ҵ������Ϣ , ������ϢwParamΪ��Ϣ���к�
const int WE_AW_MAIN_GETSHORTCUTDATA     = 10;  //g_UserSetting����,Ҫ��ȡSHORTCUT����(ֻȡǰ10��), wParamΪһ������ָ��
const int WE_AW_MAIN_SETSHORTCUTPAGE     = 11;  //���ÿ������ҳ��, wParamΪҳ��(��������ʱ�ᷢ����, ����Ϊ��һҳ)



//ְҵ���ܴ���
#define WND_VOCATIONSKILL                        100       
const int WE_VOCATIONSKILL_LOAD               = 1;   //���¶�ȡ����ְҵ���� wParam,Ϊ���ְҵ,  lParamΪ��������,��(ְҵ, ����,����)

//�������ܴ���
#define WND_PRACTICESKILL                        102  
const int WE_PRACTISESKILL_LOAD               = 1;   //���¶�ȡ����ѵ������ wParam,Ϊ���ְҵ,  lParamΪ��������,��(ְҵ, ����,����)

//�������ܴ���
#define WND_WORKSKILL                            104  



// ϵͳ����
#define WND_SYSTEMMENU  					     106
const int WE_CW_SHOWMENU                 = 1;   //������������,Ҫ����ѡ��ɫ
const int WE_WC_CLEARCMMMAND             = 2;   //���ڷ���������,Ҫ��ɾ����ҵ��������
const int WE_AW_SHOWEXITWND              = 3;   //��֪ʲô�ط�������, Ҫ���˳���Ϸ



#define WND_SYSTEMMENU_SOUND                     107  


//����Ŀ��������
#define WND_EXTRASHORTCUT                     115
const int WE_AW_EXTRASHORTCUT_SAVESHORTCUT        = 3;   //Ҫ�󱣴��û��Ŀ�ݼ����趨
const int WE_AW_EXTRASHORTCUT_LOADSHORTCUT        = 4;   //Ҫ���ȡ�û��Ŀ�ݼ����趨, wParamΪְҵID 
const int WE_AW_EXTRASHORTCUT_BINDALLITEM         = 5;   //�����ʼ����ɺ�,MAIN���ڻᷢ����Ϣ����
const int WE_CW_EXTRASHORTCUT_SHORTCUT            = 6;   //������������,Ҫ������ҵĿ��ָ��,
														 //��ڲ���: wParam ��������������ĵ�ַ, ex.  21��ʾ2��1��
														 //          lParam ��һ��int �ĵ�ַ,���ڷ��ؼ���ID
														 //���ڲ���: wParam ���ڹ������Ƿ����˸�ָ��,���缼�ܵ��ǲ��ᴦ���
													     //          lParam ����û�д���ʱ,���ֵ�Ǽ���ID			
const int WE_CW_EXTRASHORTCUT_CHANGESKILLSHAPE    = 7;   //���ת��������̬��, wParamΪ��ǰ��̬
const int WE_CW_EXTRASHORTCUT_SKILLDATAOK    = 8;   //����ģ�鷢����,��ʾ���������Ѿ��ӷ��������ص�������
const int WE_AW_EXTRASHORTCUT_GETSHORTCUTDATA     = 10;  //g_UserSetting����,Ҫ��ȡSHORTCUT����(ֻȡǰ10��), wParamΪһ������ָ��


//Ŀ�껷����
#define WND_TARGETRIM                    120
const int WE_CW_TARGET_SET			         = 0;   //��������Ŀ��, WPARAM��һ��TARGETINFOָ��, WPARAMΪNULLʱ���ش���
const int WE_WC_TARGET_TRACE			     = 9;   //����ĳһĿ��, WPARAM��һ������UID��ַָ��, ������־���, ��СҪΪ1
const int WE_AW_TARGET_CHANGEMONSTERHOLDER    = 10;  //֪ͨĿ�껷,���������  wParamΪһ��SC_HOLDPRIZE_OBSERVE_CONTEXTָ��
const int WE_AC_TARGET_SETTARGET              = 11;  //֪ͨ������Ŀ�껷,����Ŀ��  wParam Ϊһ��UIDָ��
const int WE_AC_TEAMTARGET_ATTACK             = 12;  //������Ӵ���û�д���ID, �����Ϣ�ŵ�Ŀ�껷������ 
                                                     //��ҵļ��ܵ����˶��Ѵ���ʩ��, wParamΪĿ��UID��ַ
const int WE_CW_TARGET_CHANGECAPTION	  = 13;   //�ӳ�����
const int WE_WC_TARGET_CANCELTARGET       = 14;      //֪ͨ������,ȡ��Ŀ�껷


//�����Ϣ����
#define WND_PLAYERINFO                    122
const int WE_CW_PLAYERINFO_SET			            = 0;   //����������
const int WE_CW_PLAYERINFO_STARTLISTENBORN			= 1;   //��ʼ�������������Ϣ
const int WE_CW_PLAYERINFO_CHANGECAPTION			= 2;   //�ӳ�����


//װ������
#define WND_EQUIPMENT                    125
const int WE_CW_UPDATEEQUIP   			= 0;   // wParam - int* ��Ҫ���µ�λ���б� // lParam - int ��Ҫ���µĸ���
const int WE_AW_SETLIGHT                = 5;   // ����ĳ��VIEWBOX    wParam ΪIContainerGoodsָ�� 
const int WE_AW_RESETLIGHT              = 6;   // ȡ������ȫ��VIEWBOX
const int WE_AW_SHOWEQUIPMENT           = 7;   // ��ʾ�����װ��, wParamΪ����UID�ĵ�ַ
const int WE_AW_SETTOOLTIP              = 8;   // ����TOOLTIP, wParamΪ����ְҵ

//��������
#define WND_PACKAGE                      130
const int WE_CW_DISCARDGOODS   			= 4;   // ������������,Ҫ��������ϵ���Ʒ
const int WE_AW_USEGOODS   		    	= 5;   // ��֪��ʲô�ط�������,Ҫ��ʹ����ƷwParamΪ��ƷID, lParamΪ��ƷUIDָ��(���ΪNULL, ����Զ�ѡͬ�����Ʒ��ʹ��) 


//ʰȡ(����)����
#define WND_TREASUREBOX                   135 
const int WE_TREASURE_PROGRESSBAR_SHOW   			= 2;   // ����ģ�鴫������Ҫ����ʾ��������wParam Ϊʱ�䣨ms), wParamΪ0�����ؽ�����
const int WE_TREASURE_OPEN           			    = 3;   // ����ģ�鴫������Ҫ��򿪱��䡡wParam ���ӣɣģ�lParamΪ�գɣĵ�ַ
const int WE_TREASURE_CLOSE           			    = 4;   // ����ģ�鴫������Ҫ��رձ��䡡wParam ���ӣɣģ�lParamΪ�գɣĵ�ַ
const int WE_CW_TREASURE_AUTOPICKUP		            = 5;   // ������������,Ҫ���Զ�����

//����������
#define WND_PROCESSBAR                    137 
const int WE_PROCESSBAR_SHOW   		     	= 2;   // Ҫ����ʾ��������wParam Ϊʱ�䣨ms) , lParamΪ����(LPCSTR)
const int WE_PROCESSBAR_HIDE   		     	= 3;   // Ҫ�����ؽ�����

//LUAר�ý���������
#define WND_LUAPROCESSBAR                    138 
//������Ϣ��ͬ����Ľ���������һ��,�ٶ�һ��
const int WE_LUAPROCESSBAR_SETPOS            =  4;   //wParam Ϊx ����, lParmΪY����



//��������
#define WND_PHARMACY                      145

//�ϳɴ���
#define WND_COMPOSE                       154

//��������
#define WND_REFINE                        147 

//��������
#define WND_EPURATE                       149

//��Ƕ����
#define WND_INLAY                         152

//���˴���
#define WND_TRADE                         166

//�ֿⴰ��
#define WND_STORE                         168

//���֮�佻�״���
#define WND_DEALING                         170

//LOADING����
#define WND_LOADING                         171
const int WE_CW_LOGINLOADING                         = 1;  //����������,��ʾ��ɫ�ĵ�¼�����Ѿ���ͨ��������,Ҫ���ѡ�˽��浲ס

//��������
#define WND_DEAD                            173

//�����ͼ����
#define WND_WORLDMAP                        174 
const int WE_WORLDMAP_UPDATETEAMATE   		     	= 3;   //�����ط�����, ���¶�����Ϣ;wParamΪUID[5] ��ָ��
const int WE_WORLDMAP_ADDTASKPOINT   		     	= 4;   //�����ط�����, �ڴ��ͼ��������ʾһ������� wParamΪһ��LUATASKPOINTָ��
const int WE_WORLDMAP_REMOVETASKPOINT   		    = 5;   //�����ط�����, ɾ�������
const int WE_WC_WORLDMAP_LONGMOVE   		        = 6;   //����������, ���������ƶ�(wparamΪx, lparam Ϊy)  
const int WE_WC_WORLDMAP_LOADMAPBLOCK  		        = 9;   //����������, ���µ�ͼռλ�� (�޲���, �������Լ���global��ȡ)
 

//��������(EX)
#define WND_PHARMACY_EX                      175

//�ϳɴ���(EX)
#define WND_COMPOSE_EX                        176

//��������(EX)
#define WND_REFINE_EX                         177 

//��������(EX)
#define WND_EPURATE_EX                        178

//��Ƕ����(EX)
#define WND_INLAY_EX                          179

//���������ܴ���
#define WND_REQUEST_WORKSKILL                 180

//������ҵ�װ������
#define WND_EQUIPMENT_OTHER                   182
const int WE_EQUIPMENTOTHER_UPDATEROLE        = 1;    //���½�ɫ��Ϣ, wParamΪIActorָ��

//�������崰��
#define WND_CREATE_CLAN                     184

//��������ҵ��������
#define WND_NONCLAN_PANEL                   185

//������崰��
#define WND_CLAN_PANEL                      186

//�����ԱȨ�޴���
#define WND_CLAN_MEMBERACC                  187

//���������������Ĵ���
#define WND_CLAN_SENDINV                    188

//���������������Ĵ���
#define WND_CLAN_RECVINV                    189

//�鿴������ϸ��Ϣ�Ĵ���
#define WND_CLAN_INTRO                      190

//�����е����ֱ༭����
#define WND_CLAN_TEXTEDIT                   191


#define WND_CLAN_PERSONACC                  192

#define WND_CLAN_LBATTEL                    193

#define WND_CLAN_MSGAR                      194


//�Զ��ظ���Ϣ
#define WND_AUTOREVIVE                      195
const int WE_AUTOREVIVE_LOADSETTING    = 1;    //��ȡ�ظ�����

//���������
#define WND_PETCOMMAND                      198
const int WE_CW_PETCOMMAND_QUESTEXECUTE            = 1;     //������������,Ҫ��ִ�г���ָ��
									        			    //��ڲ���: wParam ����ָ��												
const int WE_WC_PETCOMMAND_EXECUTE                 = 2;     //���ڷ���������,Ҫ��ִ�г���ָ��
												            //��ڲ���: wParam ����ָ��		

//�������´���
#define WND_DROPSCROLL                      199             //�������´���
const int WE_WW_DROPSCROLL_ADD                     = 1;     //������������,Ҫ����Ӿ�������
															//��ڲ���: wParam Ϊ LUAADDSCROLLָ��	
const int WE_WW_DROPSCROLL_REMOVE                  = 2;     //������������,Ҫ��ɾ����������
															//��ڲ���: wParam Ϊ LUAADDSCROLLָ��
const int WE_WW_DROPSCROLL_CLEARALL                = 3;     //Ҫ��ɾ�����о�������, С��ʱ����

//������Ƶ�Ĵ���
#define WND_MOVIE                           200 
const int WE_WW_MOVIE_FINISH                         = 1;   //MMSYSTEM.dll������, ��ʾ��Ƶ���������

//������Ա��������
#define WND_CREATOR                         201

//����̴���
#define WND_SOFTKEYBOARD                         202

//�����봰��
#define  WND_VALIDATIONCODE                      203
const int WE_WC_VALIDATIONCODE_SUBMIT		= 1;    // ȷ��
const int WE_WC_VALIDATIONCODE_REFRESH		= 2;	// ˢ��


//�����봰��
#define  WND_GENERALPASSWORD                       205
const int WE_AW_GENERALPASSWORD_LAUNCH       	= 1;    // ��ʾ���봰��, wParamΪIWndCallback ָ��, lParamΪҪ��ʾ���ַ���ָ��


#define WND_CLAN_CREATEUNION                       206 

#define WND_CLAN_HOMESTEAD                         209   //�����԰����

#define WND_CLAN_SKILL                             211   //���弼�ܴ���
const int WE_AW_CLANDATA_UPDATA                  = 3;     //�յ������������������, Ҫ�������弼������, wParamΪLUACLANDATAָ��
const int WE_AW_CLANSKILLDATA_UPDATA             = 4;     //�յ����������弼�ܵ�������, Ҫ�������弼������, wParamΪLUACLANSKILLDATAָ��

#define WND_EXTRAPASSWORD                          213   //�������봰��
const int WE_WC_GETEXTRAPASSWORD                 = 2;     //���ø�������, wParamΪһ��������ָ��, lParamΪ��������С

#define WND_COMPOSEV2                              214   //�ϳɴ���V2 ��

#define WND_FUSIONV2                              215   //��������V2 ��

#define WND_PHARMACYV2                              216   //��������V2 ��

#define WND_INLAYV2                               217   //��Ƕ����V2 ��

#define WND_REFINEV2                               218  //��������V2 ��

#define WND_EPURATEV2                              219   //��������V2��

#define WND_TRADECITY                              220   //�̳Ǵ���
