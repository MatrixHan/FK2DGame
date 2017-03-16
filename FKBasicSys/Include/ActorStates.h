/**
*	created:		2012-7-2   3:45
*	filename: 		ActorsState
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���״̬����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
class CActorView;
//------------------------------------------------------------------------
// ͨ��״̬
class CActState
{
public:
	CActState();
	virtual ~CActState();

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
	CActorView		*m_pAct;
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
class CActStateMove : public CActState
{
public:
	virtual void Start();
	virtual void End();
	virtual void OnTimer(DWORD dwEventID);
};
//------------------------------------------------------------------------
// ����״̬
class CActStateFallow : public CActState
{
public:
	virtual void OnTimer(DWORD dwEventID);
};
//------------------------------------------------------------------------
// ����״̬
class CActStateDie : public CActState
{
public:
	virtual void OnTimer(DWORD dwEventID);
};
//------------------------------------------------------------------------
// ����״̬
class CActStateBehit : public CActState
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
// ����״̬
class CActStateAttack : public CActState
{
public:
	virtual void OnTimer(DWORD dwEventID);
};
//------------------------------------------------------------------------
// ״̬��
class CActFSM
{
protected:
	CActState*			m_pCurrentState;
	CActState*			m_pStates[CrtStateMax];

	CActState			m_StateStand;				//0		վ��	
	CActState			m_StateRideStand;			//17	����վ��			
	CActState			m_StateSit;					//6		����

	CActStateBehit		m_StateBehit;				//5		����
	CActStateDie		m_StateDie;					//3		����

	CActStateMove		m_StateWalk;				//1		����
	CActStateMove		m_StateRun;					//2		����
	CActStateMove		m_StateRideRun;				//18	������

	CActStateFallow		m_StatePreFallow;			//4		����
	CActState			m_StatePre1;				//7		����1
	CActState			m_StatePre2;				//8		����2
	
	CActState			m_StateMagic1;				//9		ʩ��1
	CActState			m_StateMagic2;				//10	ʩ��2
	CActStateAttack		m_StateSingleAttack1;		//11	������������1
	CActStateAttack		m_StateSingleAttack2;		//12	������������2
	CActStateAttack		m_StateSingleAttack3;		//13	������������3
	CActStateAttack		m_StateDoubleAttack1;		//14	˫����������1
	CActStateAttack		m_StateDoubleAttack2;		//15	˫����������2
	CActStateAttack		m_StateArrowAttack;			//16	������������
	CActStateAttack		m_StateCommonAttack1;		//19	��ͨ����1
	CActStateAttack		m_StateCommonAttack2;		//20	��ͨ����2
	CActStateAttack		m_StateSkillAttack1;		//21	���ܹ���1
	CActStateAttack		m_StateSkillAttack2;		//22	���ܹ���2

public:
	CActFSM();
	virtual ~CActFSM();
	bool Create(CActorView* pAct);

	bool IsState(CActState& State);
	bool GotoState(CActState& NewState);
	void SetAllStateDelay(int* pDelay);
	void SetAllStateAttackFrame(int* pFrame);
	void SetAllStateAttackSoundFrame(int* pFrame);
	void SetMoveDelay(int* pDelay);
	CActState* GetCurState();

	virtual void OnTimer(DWORD dwEventID);
};
//------------------------------------------------------------------------