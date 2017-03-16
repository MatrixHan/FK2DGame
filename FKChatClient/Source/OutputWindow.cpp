/**
*	created:		2012-6-28   10:39
*	filename: 		OutputWindow
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		ʵ�ִ�����ά��������Ϣ���������
*/
//------------------------------------------------------------------------
#include "../Include/ChatClient.h"
#include "../Include/OutputWindow.h"
#include "../Include/WindowCreator.h"
//------------------------------------------------------------------------
COutputWindow::COutputWindow(void)
: m_pChatOutput(NULL)
, m_pMoveButton(NULL)
, m_pResizeButton(NULL)
, m_pPromptBitmap(NULL)
, m_bPromptOn(FALSE)
{
	m_nAlpha = 100;
	
	// �������е�Ƶ�����ǿ�ͨ��
	for (int i = 0; i <= CHANNEL_MAX; i++)
	{
		m_channelState[i] = TRUE;
	}
}
//------------------------------------------------------------------------
COutputWindow::~COutputWindow(void)
{
	// ������ʷ��¼	
	while (m_history.begin() != m_history.end())
	{
		delete *(m_history.begin());
		m_history.pop_front();
	}	

	delete m_pChatOutput;
	delete m_pMoveButton;
	delete m_pResizeButton;
	delete m_pPromptBitmap;
}
//------------------------------------------------------------------------
BOOL COutputWindow::Create(IResObject * pResObject)
{
	int i = 0;	

	// �������������
	if (!CWindowCreator::CreateMainWnd(NULL, this, pResObject))
	{
		return FALSE;
	}

	// ����������Ϣ�����
	m_pChatOutput = new XChatEx(this);
	if (!CWindowCreator::CreateControl(this, m_pChatOutput, pResObject->GetSubObjectByIndex(i++)))
	{
		return FALSE;
	}
	m_pChatOutput->SetItemFactory(static_cast<IItemFactory *>(&m_itemFactory));

	
	// �����ı��С��ť
	m_pResizeButton = new XResizeButton(this);
	if (!CWindowCreator::CreateControl(this, m_pResizeButton, pResObject->GetSubObjectByIndex(i++)))
	{
		return FALSE;
	}
	
	// �����ƶ���ť
	m_pMoveButton = new XMoveButton(this);
	if (!CWindowCreator::CreateControl(this, m_pMoveButton, pResObject->GetSubObjectByIndex(i++)))
	{
		return FALSE;
	}	
	
	// ������ʾλͼ
	m_pPromptBitmap = new XStaticBitmap(this);
	if (!CWindowCreator::CreateControl(this, m_pPromptBitmap, pResObject->GetSubObjectByIndex(i++)))
	{
		return FALSE;
	}
	m_pPromptBitmap->SetEnabled(FALSE);

	SetWindowPos(m_rcWnd);
	SetVisible(FALSE);

	TraceLn("ChatClient - COutputWindow::Create OK");
	return TRUE;
}
//------------------------------------------------------------------------
void COutputWindow::ShowWindow(BOOL bVisible)
{
	SetVisible(bVisible);
}

//-------------------------------------------------------------------
// ��  �������ô��ڵ�͸���� 
// ��  ����[����] int nLevel - ͸����
// ����ֵ��void
//-------------------------------------------------------------------
void COutputWindow::SetTransparence(int nLevel)
{
	m_nAlpha = nLevel;
}

//-------------------------------------------------------------------
// ��  ������ʾ������Ϣ��ͬʱ�����¼ 
// ��  ����[����] const char * lpszData - ��������
// ��  ����[����] int nLen - ���ݳ���
// ����ֵ��BOOL - ���������ʾ�򷵻�TRUE�����򷵻�FALSE
//-------------------------------------------------------------------
BOOL COutputWindow::Display(const char * lpszData, int nLen)
{	
	// �����ݽ��н���
	CHtmlNode * pContentNode = m_chatParser.Parse(lpszData, nLen);
	if (pContentNode == NULL)
	{
		return FALSE;
	}
	
	// �����ʾ�Ľ���������ȷ�������¼
	if (DisplayMessage(pContentNode))
	{
		// �����¼����Ŀ�Ѵﵽ����������ɾ����һ����¼
		if (m_history.size() >= HISTORY_MAX)
		{
			delete *(m_history.begin());
			m_history.pop_front();
		}
		
		m_history.push_back(pContentNode);
		return TRUE;
	}
	else
	{
		PrintEventLn("ChatClient (FAIL) - COutputWindow::Display");
		delete pContentNode;
		return FALSE;
	}
}

//-------------------------------------------------------------------
// ��  ��������Ƶ���Ƿ���ʾ������ʾ�����ε�����
// ��  ����[����] int nChannel - ָ��Ƶ��
// ��  ����[����] BOOL bOnOff - �Ƿ���ʾ
// ����ֵ��void
//-------------------------------------------------------------------
void COutputWindow::ChannelOn(int nChannel, BOOL bOnOff)
{
	// ��ֹԽ��
	if (!IS_VALID_CHANNEL(nChannel))
	{
		return;
	}

	m_channelState[nChannel] = bOnOff;
	m_pChatOutput->Clear();

	// ֻ��ʾ�����εļ�¼
	list<CHtmlNode *>::iterator itr = m_history.begin();
	for (; itr != m_history.end(); itr++)
	{
		DisplayMessage(*itr);
	}
}
//------------------------------------------------------------------------
void COutputWindow::SetWindowPos(RECT & rc)
{
	int nSize;		
	RECT rcButton;
	
	XMainWnd::SetWindowPos(rc);

	nSize = GetDesktop()->GetSysScrollBarBitmap()->GetHeight();

	// ������Ϣ������λ��
	if (m_pChatOutput != NULL) 
	{		
		// ȷ��������������		
		RECT rcScrollBar;
		rcScrollBar.left = rc.left;
		rcScrollBar.right = rcScrollBar.left + nSize;
		rcScrollBar.top = rc.top + nSize;
		rcScrollBar.bottom = rc.bottom - nSize;
		
		// ȷ������������
		RECT rcContainer;
		CopyRect(&rcContainer, &rc);
		rcContainer.left = rcScrollBar.right;		
				
		m_pChatOutput->SetWindowPos(rcScrollBar, rcContainer);
	}

	// ���øı��С��ť��λ��
	if (m_pResizeButton != NULL) 
	{	
		rcButton.left = rc.left+2;
		rcButton.right = rcButton.left + nSize;
		rcButton.top = rc.top+2;
		rcButton.bottom = rcButton.top + nSize;
		
		m_pResizeButton->SetWindowPos(rcButton);
	}

	// �����ƶ���ť��λ��
	if (m_pMoveButton != NULL) 
	{
		rcButton.left = rc.left+2;
		rcButton.right = rcButton.left + nSize;
		rcButton.top = rc.bottom - nSize+2;
		rcButton.bottom = rc.bottom;
		
		m_pMoveButton->SetWindowPos(rcButton);
	}

	// ������ʾͼ���λ��
	if (m_pPromptBitmap != NULL)
	{
		rcButton.left = rc.left;
		rcButton.right = rcButton.left + nSize;
		rcButton.bottom = rc.bottom - nSize;
		rcButton.top = rcButton.bottom - nSize;

		m_pPromptBitmap->SetWindowPos(rcButton);
	}
}
//------------------------------------------------------------------------
void COutputWindow::MoveWindow(int dx, int dy)
{
	XMainWnd::MoveWindow(dx, dy);

	// ͬʱҪ�����ؼ���λ��
	SetWindowPos(m_rcWnd);
}

//-------------------------------------------------------------------
// ��  ���� ��ʾһ��������Ϣ
// ��  ����[����] const CHtmlNode * pContent - ���ݽڵ�
// ����ֵ��BOOL - ��ʾ��ȷ����TRUE�����򷵻�FALSE
//-------------------------------------------------------------------
BOOL COutputWindow::DisplayMessage(const CHtmlNode * pContent)
{
	// ��ȡƵ��
	int nChannel;
	pContent->GetAttribute("channel", nChannel, 1);

	// ȷ�����Ƶ���������Ƿ�Ҫ��ʾ
	if (!m_channelState[nChannel])
	{
		return TRUE;
	}

	// ��ʾ��Ϣ������
	return m_pChatOutput->DisplayMessage(pContent);
}

//-------------------------------------------------------------------
// ��  �����ǲ���͸���� 
//-------------------------------------------------------------------
BOOL COutputWindow::IsTransparent(POINT& pt)
{
	// ���SHIFT�����£���͸��
	if (GetAsyncKeyState(VK_LSHIFT) || GetAsyncKeyState(VK_RSHIFT))
	{
		return FALSE;
	}
	
	if (m_pChatOutput != NULL && PtInRect(&(m_pChatOutput->GetContainerRect()), pt))
	{
		POINT ptTmp;
		ptTmp.x = 0;
		ptTmp.y = 0;
		OnMouseMove(0, 0, ptTmp);		
		
		return TRUE;
	}
	
	return FALSE;	
}

//-------------------------------------------------------------------
// ��  ����������Ϣ����ʾ 
//-------------------------------------------------------------------
void COutputWindow::OnTimer(DWORD dwEventID)
{
	if (m_pPromptBitmap->IsVisible())
	{
		m_pPromptBitmap->SetVisible(FALSE);
	}
	else
	{
		m_pPromptBitmap->SetVisible(TRUE);
	}
}

//-------------------------------------------------------------------
// ��  ������ʼ��ʾ 
//-------------------------------------------------------------------
void COutputWindow::SetPromptOn(BOOL bOn)
{
	ITimeAxis * pTimeAxis = GetTimeAxis();
	if (pTimeAxis == NULL)
	{
		return;
	}

	if (bOn && !m_bPromptOn)
	{
		pTimeAxis->SetTimer(0, static_cast<ITimerSink *>(this), 500, "COutputWindow::SetPromptOn");
		m_bPromptOn = TRUE;
	}
	else if (!bOn && m_bPromptOn)
	{
		pTimeAxis->KillTimer(0, static_cast<ITimerSink *>(this));
		m_pPromptBitmap->SetVisible(FALSE);
		m_bPromptOn = FALSE;
	}
}
//------------------------------------------------------------------------
DWORD COutputWindow::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == 0)
	{	
		SetPromptOn(wParam);
	}

	return 0;
}
//------------------------------------------------------------------------
void COutputWindow::Clear()
{
	m_pChatOutput->Clear();
	// ������ʷ��¼	
	while (m_history.begin() != m_history.end())
	{
		delete *(m_history.begin());
		m_history.pop_front();
	}
}
//------------------------------------------------------------------------