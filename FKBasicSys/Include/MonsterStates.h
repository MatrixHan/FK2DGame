/**
*	created:		2012-7-2   3:35
*	filename: 		MonsterStates
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		����״̬����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
class CMonsterView;
//------------------------------------------------------------------------
// ͨ��״̬
class CMstState
{
public:
	CMstState();
	virtual ~CMstState();
	
	virtual void Start();
	virtual void End();
	virtual void Stop()	{};
	
	virtual void OnTimer(DWORD dwEventID);

	void SetDelay(int nDelay);
	int  GetDelay();

	void SetAttackFrame(int nFrame);
	void SetAttackSoundFrame(int nFrame);
	

	void DelAllAttribute();				// ɾ�����и�������

	void SetPlayCommondAttackSound();	// ������ͨ������Ч�ڸö�����
	void SetMagicCheckID(int nCheckID);	// ����ħ���ڸö�����
	void SetPreAttack();				// ������������
	void SetAttackOnce();				// ����һ���Թ�������
	void SetAttackCtrl();				// ���ó�����������

public:
	CMonsterView	*m_pMst;
	int				m_nDelay;			// ÿ��״̬�Ķ�����ʱ
	int				m_nAttackFrame;		// ������
	int				m_nAttackSoundFrame;// ������Ч��
	
	// ��������
	int				m_nMagicCheckID;	// ��ǰ״̬��Ӧ�ļ��ܽϼ�ID
	BOOL			m_bPreAttack;		// ��������
	BOOL			m_bAttackOnce;		// һ���Թ�������
	BOOL			m_bAttackCtrl;		// ������������

	BOOL			m_bPlayCommondAttackSound;	// �Ƿ񲥷���ͨ������Ч
};
//------------------------------------------------------------------------
// �ƶ�״̬
class CMstStateMove : public CMstState
{
public:
	virtual void Start();
	virtual void End();
	virtual void OnTimer(DWORD dwEventID);
};
//------------------------------------------------------------------------
// վ��״̬
class CMstStateStand : public CMstState
{
public:
	virtual void OnTimer(DWORD dwEventID);
};
//------------------------------------------------------------------------
// ����״̬
class CMstStateFallow : public CMstState
{
public:
	virtual void Start();
	virtual void OnTimer(DWORD dwEventID);
};
//------------------------------------------------------------------------
// ����״̬
class CMstStateDie : public CMstState
{
public:
	virtual void Start();
	virtual void OnTimer(DWORD dwEventID);
};
//------------------------------------------------------------------------
// ����״̬
class CMstStateBehit : public CMstState
{
public:
	virtual void Start();
	virtual void OnTimer(DWORD dwEventID);
	virtual void Stop();
	
public:
	int m_nState;
	int m_nBeHitLoopBeginFrame;
	int m_nBeHitLoopEndFrame;
};
//------------------------------------------------------------------------
// ״̬��
class CMstFSM
{
protected:
	CMstState*			m_pCurrentState;
	CMstState*			m_pStates[CrtStateMax];

	CMstStateStand		m_StateStand;			//0		վ��
	CMstState			m_StateRideStand;		//17	����վ��			
	CMstState			m_StateSit;				//6		����

	CMstStateBehit		m_StateBehit;			//5		����
	CMstStateDie		m_StateDie;				//3		����

	CMstStateMove		m_StateWalk;			//1		����
	CMstStateMove		m_StateRun;				//2		����
	CMstStateMove		m_StateRideRun;			//18	������

	CMstStateFallow		m_StateFallow;			//4		����
	CMstState			m_StatePre1;			//7		����1
	CMstState			m_StatePre2;			//8		����2

	CMstState			m_StateMagic1;			//9		ʩ��1
	CMstState			m_StateMagic2;			//10	ʩ��2
	CMstState			m_StateSingleAttack1;	//11	������������1
	CMstState			m_StateSingleAttack2;	//12	������������2
	CMstState			m_StateSingleAttack3;	//13	������������3
	CMstState			m_StateDoubleAttack1;	//14	˫����������1
	CMstState			m_StateDoubleAttack2;	//15	˫����������2
	CMstState			m_StateArrowAttack;		//16	������������
	CMstState			m_StateCommonAttack1;	//19	��ͨ����1
	CMstState			m_StateCommonAttack2;	//20	��ͨ����2
	CMstState			m_StateSkillAttack1;	//21	���ܹ���1
	CMstState			m_StateSkillAttack2;	//22	���ܹ���2

public:
	CMstFSM();
	virtual ~CMstFSM();
	bool Create(CMonsterView* pMst);

	bool IsState(CMstState& State);
	bool GotoState(CMstState& NewState);
	void SetAllStateDelay(int* pDelay);
	void SetAllStateAttackFrame(int* pFrame);
	void SetAllStateAttackSoundFrame(int* pFrame);
	void SetMoveDelay(int* pDelay);
	CMstState* GetCurState();

	virtual void OnTimer(DWORD dwEventID);
};
//------------------------------------------------------------------------