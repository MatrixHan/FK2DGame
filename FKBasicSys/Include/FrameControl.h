/**
*	created:		2012-7-2   2:03
*	filename: 		FrameControl
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		֡������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
class CFrameControl
{
	struct stFrameInfo
	{
		int nFrameIdx;		// ֡����
		int nLoopCount;		// ֡ѭ������
	};
	stFrameInfo*	m_pFrameInfoList;	// ֡��Ϣ�б�
	int				m_nCounts;			// ��Ҫ����֡���Ƶ���Ŀ����֡��Ϣ�б�ĳ���
	int				m_nCurPos;			// ֡���Ƶ�ǰ���е����ĸ�λ��?
	int				m_nLoopCounts;		// ֡ѭ������
	
public:
	CFrameControl() : m_pFrameInfoList(NULL), m_nCounts(0), m_nCurPos(0), m_nLoopCounts(0) {}
	virtual ~CFrameControl() {Clear();}
	void InitFrameInfoList(int nCounts) // ��ʼ��֡��Ϣ�б�
	{
		if (nCounts > m_nCounts)
		{
			delete[] m_pFrameInfoList;
			m_pFrameInfoList = new stFrameInfo[nCounts];
		}
		Clear();
		memset(m_pFrameInfoList, 0xFF, sizeof(stFrameInfo)*nCounts);
		ASSERT(m_pFrameInfoList);
		m_nCurPos = nCounts;
	}
	void Clear()
	{
		m_nCounts = m_nCurPos = m_nLoopCounts = 0;
	}
	void Close()
	{
		SAFE_DELETE_ARRAY(m_pFrameInfoList);
		m_nCounts = m_nCurPos = m_nLoopCounts = 0;
	}
	bool IsValid()
	{
		return (m_pFrameInfoList!=NULL && m_nCounts!=0);
	}
	void AddFrameInfo(int nFrame, int nLoopCount) // ���֡��Ϣ
	{
		if (m_nCurPos > 0) // ������
		{
			m_pFrameInfoList[m_nCounts].nFrameIdx  = nFrame;
			m_pFrameInfoList[m_nCounts].nLoopCount = nLoopCount;
			m_nCounts ++;
			m_nCurPos --;
		}
	}
	bool Eval(int& nCurFrame, int& nFrames) // ֡������ʱ������true
	{
		if (m_pFrameInfoList && m_pFrameInfoList[m_nCurPos].nFrameIdx == nCurFrame)
		{
			if (++m_nLoopCounts >= m_pFrameInfoList[m_nCurPos].nLoopCount)
			{
				m_nLoopCounts = 0;
				nCurFrame ++;
				if (++m_nCurPos >= m_nCounts)
				{
					m_nCurPos = 0;
					if (nCurFrame >= nFrames)
					{
						nCurFrame = 0;
						return true;
					}
				}
			}
		}
		else
		{
			if (++nCurFrame >= nFrames)
			{
				nCurFrame = 0;
				return true;
			}
		}
		return false;
	}
};
//------------------------------------------------------------------------