/**
*	created:		2012-6-23   20:57
*	filename: 		XDeskTop
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��Ϸ����
*/
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
#include "../Include/FKWndSysCommon.h"
//------------------------------------------------------------------------
XDesktop* XDesktop::m_pDesktop = NULL;
//------------------------------------------------------------------------
XDesktop::XDesktop()
{
	m_hWnd		= NULL;
	m_pCanvas	= NULL;
	m_pTimeAxis	= NULL;
	m_pSysFont	= NULL;
	m_sizeDesktop.cx = 1024;
	m_sizeDesktop.cy = 768;
	
	m_ppCursors				= NULL;
	m_nCursorCount			= 0;
	m_nCurrentCursorType	= CT_ARROW;
	
	m_pActiveWnd	= NULL;
	m_pCaptureWnd	= NULL;
	m_pFocusWnd		= NULL;
	m_pMouseMoveWnd	= NULL;
	
	m_pSysScrollBarBitmap	= NULL;
	m_pToolTip				= NULL;

	m_lstTopmostWnds.clear();
	m_ptBak.x = m_ptBak.y = -1;
	SetRectEmpty(&m_rcBak);
	m_lstMsg.clear();
	m_dwMouseKeyState = 0;

#ifdef _USE_IME_EX
	m_bRecordLayoutChange = TRUE;
	memset(m_hKeyboardLayoutList, 0, sizeof(m_hKeyboardLayoutList));
	m_nKeyboardLayoutListCount = 0;
	m_nEnImeIndex = m_nCurImeIndex = 0;

	m_nKeyboardLayoutListCount = GetKeyboardLayoutList(sizeof(m_hKeyboardLayoutList)/sizeof(HKL), 
		m_hKeyboardLayoutList);
	for (int i=0; i<m_nKeyboardLayoutListCount; i++)
	{
		if (!ImmIsIME(m_hKeyboardLayoutList[i]))
		{
			m_nEnImeIndex = m_nCurImeIndex = i;
		}
	}
#endif
}
//------------------------------------------------------------------------
XDesktop::~XDesktop()
{
}
//------------------------------------------------------------------------
void XDesktop::Release()
{
	SetFocusWindow(NULL);
	SetCaptureWindow(NULL);
	SetActiveWindow(NULL);
	
	// �����Topmost���ڣ������ͷ�
	while (!m_lstTopmostWnds.empty())
	{
		XWindow* pWnd= *(m_lstTopmostWnds.begin());
		pWnd->Release();
	}

	// ����������ڣ������ͷ�
	while (!m_lstWnds.empty())
	{
		XWindow* pWnd= *(m_lstWnds.begin());
		pWnd->Release();
	}
	
	// �ͷű�����
	DestroyWindow();
}
//------------------------------------------------------------------------
void XDesktop::DestroyWindow()
{
	SAFE_RELEASE_IMAGE(m_pWndBitmap);
	SAFE_RELEASE_IMAGE(m_pSysScrollBarBitmap);
	SAFE_RELEASE(m_pToolTip);
	
	if (m_ppCursors)
	{
		for (int i=0; i<m_nCursorCount; i++)
			m_ppCursors[i]->Release();
		delete[] m_ppCursors;
		m_ppCursors = NULL;
	}

	delete this;
}
//------------------------------------------------------------------------
XDesktop* XDesktop::GetInstance()
{
	if (XDesktop::m_pDesktop == NULL)
		XDesktop::m_pDesktop = new XDesktop;
	return XDesktop::m_pDesktop;
}
//------------------------------------------------------------------------
XWindow* XDesktop::SetActiveWindow(XWindow* pNewWnd)
{
	XWindow* pOldWnd = m_pActiveWnd;
	if (pNewWnd == pOldWnd)
		return pOldWnd;
	if (pNewWnd && !pNewWnd->IsMainWindow())
		return pOldWnd;

	m_pActiveWnd = pNewWnd;
	if (pOldWnd) // �ɴ���ʧȥ����
		pOldWnd->OnActivate(FALSE);

	if (pNewWnd && pNewWnd->IsActive())
	{
		pNewWnd->GetMainWnd()->BringToTopLevel();
		pNewWnd->OnActivate(TRUE);
	}
	return pOldWnd;
}
//------------------------------------------------------------------------
/*
XWindow* XDesktop::SetFocusHelper(XWindow* pNewWnd)
{
	// ����pNewWnd��Զ����ΪNULL
	if (pNewWnd == this || pNewWnd->IsMainWindow())
		return INVALID_XWND;
	
	// ����û�ı�
	if (pNewWnd->IsFocus())
		return pNewWnd;
	
	XWindow* pOldWnd = pNewWnd->GetParent()->GetFocus();
	
	pNewWnd->GetParent()->m_pFocusChild = pNewWnd;
	// ����ı�
	if (pOldWnd)
	{
		//PrintEventLn("OnKillFocus - %s", pOldWnd->GetWindowText());
		pOldWnd->OnKillFocus();
	}
	
	if (pNewWnd->GetParent()->m_pFocusChild == pNewWnd);
	{
		//PrintEventLn("OnSetFocus - %s", pNewWnd->GetWindowText());
		pNewWnd->OnSetFocus();
	}
	return pOldWnd;
}*/
//------------------------------------------------------------------------
XWindow* XDesktop::SetFocusWindow(XWindow* pNewWnd)
{
	XWindow* pOldWnd = m_pFocusWnd;
	if (pNewWnd == pOldWnd)
	{
		return pOldWnd;
	}
	m_pFocusWnd = pNewWnd;
#ifdef _USE_IME_EX
	if (m_pFocusWnd && m_pFocusWnd->GetWindowType() == WT_EDIT)
		SetIMEEnable(TRUE);
	else
		SetIMEEnable(FALSE);
#endif
	return pOldWnd;
	
	/*
	if (m_pFocusWnd == pNewWnd)
		return m_pFocusWnd;
    XWindow* pOldWnd = m_pFocusWnd;
	XWindow* pWnd = pNewWnd;
	m_pFocusWnd = pNewWnd;
	if (pOldWnd)
	{
		//PrintEventLn("OnKillFocus - %s", pOldWnd->GetWindowText());		
		pOldWnd->OnKillFocus();
	}
	
    if (pWnd && pWnd!=this && !pWnd->IsMainWindow())
	{
        do{
            pWnd = pWnd->GetParent();
        } while (SetFocusHelper(pWnd) != INVALID_XWND);
    }

	if (m_pFocusWnd && m_pFocusWnd == pNewWnd)
	{
		pNewWnd->GetParent()->m_pFocusChild = pNewWnd;
		//PrintEventLn("OnSetFocus - %s", pNewWnd->GetWindowText());
		pNewWnd->OnSetFocus();
	}
#ifdef _USE_IME_EX
	if (m_pFocusWnd && m_pFocusWnd->GetWindowType() == WT_EDIT)
		SetIMEEnable(TRUE);
	else
		SetIMEEnable(FALSE);
#endif
    return pOldWnd;*/
}
//------------------------------------------------------------------------
XWindow* XDesktop::SetCaptureWindow(XWindow* pNewWnd)
{
	XWindow* pOldWnd = m_pCaptureWnd;
	if (pNewWnd == pOldWnd)
		return pOldWnd;

	m_pCaptureWnd = pNewWnd;
	return pOldWnd;
}
//------------------------------------------------------------------------
XWindow* XDesktop::SetMouseMoveWindow(XWindow* pNewWnd)
{
	XWindow* pOldWnd = m_pMouseMoveWnd;
	if (pNewWnd == pOldWnd)
		return pOldWnd;

	XToolTip* pToolTip = (XToolTip*)m_pToolTip;

	if (pOldWnd)
	{
		s_mouseExit((DWORD)pOldWnd);
		pOldWnd->OnMouseExit();
		if (pToolTip && pToolTip->IsValid())
			pToolTip->Hide();
	}

	m_pMouseMoveWnd = pNewWnd;
	if (m_pMouseMoveWnd)
	{
		s_mouseEnter((DWORD)m_pMouseMoveWnd);
		m_pMouseMoveWnd->OnMouseEnter();
		if (pToolTip && pToolTip->IsValid() && m_pMouseMoveWnd->GetWindowType() >= WT_CONTROL)
		{
			POINT pt;
			GetCurrentCursor()->GetMouseXY(pt);
			pToolTip->Show(&pt);
		}
	}
	return pOldWnd;
}
//------------------------------------------------------------------------
#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)
#endif
//------------------------------------------------------------------------
int XDesktop::SetCurrentCursorType(int nType)
{
	int nOldType = GetCurrentCursorType();

	if (nType >= 0 && nType < GetCursorCount())
	{
		m_nCurrentCursorType = nType;
		if (!XCursor::m_bUseOwnCursor)
		{
			switch (nType) 
			{
			case CT_ARROW:
				::SetCursor(LoadCursor(NULL, IDC_ARROW));
				break;
			case CT_HAND:
				::SetCursor(LoadCursor(NULL, IDC_HAND));
				break;
			case CT_BEAN:
				::SetCursor(LoadCursor(NULL, IDC_IBEAM));
				break;
			case CT_VSIZE:
				::SetCursor(LoadCursor(NULL, IDC_SIZENS));
				break;
			default:
				break;
			}
		}
	}
	return nOldType;
}
//------------------------------------------------------------------------
XCursor* XDesktop::GetCursor(int nType)
{
	if (nType >= 0 && nType < GetCursorCount())
		return m_ppCursors[nType];
	return NULL;
}
//------------------------------------------------------------------------
void XDesktop::SetCursorOwnDraw(BOOL bOwnDraw)
{
	GetCurrentCursor()->SetCursorOwnDraw(bOwnDraw);
}
//------------------------------------------------------------------------
XWindow* XDesktop::SetSysToolTip(XWindow* pNewWnd)
{
	XWindow* pOldWnd = GetSysToolTip();
	m_pToolTip = pNewWnd;
	if (m_pToolTip)
	{
		m_pToolTip->SetAlpha(190);
	}
	return pOldWnd;
}
//------------------------------------------------------------------------
IBitmap* XDesktop::SetSysScrollBarBitmap(IBitmap* pNewBitmap)
{
	IBitmap* pOldBitmap = GetSysScrollBarBitmap();
	m_pSysScrollBarBitmap = pNewBitmap;
	return pOldBitmap;
}
//------------------------------------------------------------------------
// ��Ϣ�ַ�
DWORD XDesktop::DispatchMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	while (!m_lstMsg.empty()) // �ȴ�����Ϣ���У�����Ŀǰֻ�����ڹر���Ϣ
	{
		stMsg& msg = m_lstMsg.front();
		XWindow* pWnd = (XWindow*)msg.wParam;
		m_lstMsg.pop_front();
		if (pWnd)
			pWnd->Release();
	}

	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST+1) // +1Ϊ��֧��WM_MOUSEWHEEL
		return DispatchMouseMessage(message, wParam, lParam);
	else if (message >= WM_KEYFIRST && message <= WM_KEYLAST)
		return DispatchKeyMessage(message, wParam, lParam);

	return DispatchOtherMessage(message, wParam, lParam);
}	
//------------------------------------------------------------------------
// �����Ϣ�ַ�
DWORD XDesktop::DispatchMouseMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD dwReturn = NOTHANDLE;
	POINT pt;
	UINT nButton = 0;
	UINT nShift = 0;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	if (wParam&MK_SHIFT) nShift |= ShiftKey;
	if (wParam&MK_CONTROL) nShift |= CtrlKey;
	if (GetKeyState(VK_MENU) & 0x8000) nShift |= AltKey;;
	
	switch (message)
	{
	case WM_LBUTTONDOWN:
		{
			nButton = LeftButton;
			m_dwMouseKeyState |= LeftButtonDown;

			if (m_pCaptureWnd) // �в�׽
				return m_pCaptureWnd->OnMouseDown(nButton, nShift, pt);
			else
			{
				XWindow* pWnd = GetMouseMove();
				XMainWnd* pMainWnd = NULL;
				if (pWnd != NULL && (pMainWnd = pWnd->GetMainWnd()) != NULL)
					return pMainWnd->OnMouseDown(nButton, nShift, pt);

				// ���洦��
				return OnMouseDown(nButton, nShift, pt);
			}
		} break;
	case WM_MBUTTONDOWN:
		{
			nButton = MiddleButton;
			m_dwMouseKeyState |= MiddleButtonDown;
			
			if (m_pCaptureWnd) // �в�׽
				return m_pCaptureWnd->OnMouseDown(nButton, nShift, pt);
			else
			{
				XWindow* pWnd = GetMouseMove();
				XMainWnd* pMainWnd = NULL;
				if (pWnd != NULL && (pMainWnd = pWnd->GetMainWnd()) != NULL)
					return pMainWnd->OnMouseDown(nButton, nShift, pt);

				// ���洦��
				return OnMouseDown(nButton, nShift, pt);
			}
		} break;
	case WM_RBUTTONDOWN:
		{
			nButton = RightButton;
			m_dwMouseKeyState |= RightButtonDown;

			if (m_pCaptureWnd) // �в�׽
				return m_pCaptureWnd->OnMouseDown(nButton, nShift, pt);
			else
			{
				XWindow* pWnd = GetMouseMove();
				XMainWnd* pMainWnd = NULL;
				if (pWnd != NULL && (pMainWnd = pWnd->GetMainWnd()) != NULL)
					return pMainWnd->OnMouseDown(nButton, nShift, pt);

				// ���洦��
				return OnMouseDown(nButton, nShift, pt);
			}
		} break;
	case WM_LBUTTONUP:
		{
			nButton = LeftButton;
			m_dwMouseKeyState &= ~LeftButtonDown;

			if (m_pCaptureWnd) // �в�׽
			{
				RECT rc;
				m_pCaptureWnd->GetWindowRect(&rc);
				if (PtInRect(&rc, pt))
					m_pCaptureWnd->s_clicked();
				if (m_pCaptureWnd)
					return m_pCaptureWnd->OnMouseUp(nButton, nShift, pt);
			}
			else
			{
				XWindow* pWnd = GetMouseMove();
				XMainWnd* pMainWnd = NULL;
				if (pWnd != NULL && (pMainWnd = pWnd->GetMainWnd()) != NULL)
				{
					RECT rc;
					pWnd->GetWindowRect(&rc);
					if (PtInRect(&rc, pt))
						pWnd->s_clicked();
					return pMainWnd->OnMouseUp(nButton, nShift, pt);
				}
				
				// ���洦��
				s_clicked();
				return OnMouseUp(nButton, nShift, pt);
			}
		} break;
	case WM_MBUTTONUP:
		{	
			nButton = MiddleButton;
			m_dwMouseKeyState &= ~MiddleButtonDown;

			if (m_pCaptureWnd) // �в�׽
				return m_pCaptureWnd->OnMouseUp(nButton, nShift, pt);
			else
			{
				XWindow* pWnd = GetMouseMove();
				XMainWnd* pMainWnd = NULL;
				if (pWnd != NULL && (pMainWnd = pWnd->GetMainWnd()) != NULL)
					return pMainWnd->OnMouseUp(nButton, nShift, pt);

				// ���洦��
				return OnMouseUp(nButton, nShift, pt);
			}
		} break;
	case WM_RBUTTONUP:
		{	
			nButton = RightButton;
			m_dwMouseKeyState &= ~RightButtonDown;

			if (m_pCaptureWnd) // �в�׽
				return m_pCaptureWnd->OnMouseUp(nButton, nShift, pt);
			else
			{
				XWindow* pWnd = GetMouseMove();
				XMainWnd* pMainWnd = NULL;
				if (pWnd != NULL && (pMainWnd = pWnd->GetMainWnd()) != NULL)
					return pMainWnd->OnMouseUp(nButton, nShift, pt);
				
				// ���洦��
				return OnMouseUp(nButton, nShift, pt);
			}
		} break;
	case WM_MOUSEMOVE:
		{
			GetCurrentCursor()->MoveTo(pt); // ͬ�����¹��λ��
			
			if (wParam&MK_LBUTTON) nButton |= LeftButton;
			if (wParam&MK_RBUTTON) nButton |= RightButton;
			if (wParam&MK_MBUTTON) nButton |= MiddleButton;
			
			if (m_pCaptureWnd) // �в�׽
				return m_pCaptureWnd->OnMouseMove(nButton, nShift, pt);
			else
			{
				// ����Ƿ�ͣ�������µ�����һ�����Ӵ�����?
				XWindow* pWnd = GetWindowBelowCursor(pt, GCWP_SKIPINVISIBLE);
				if (pWnd)
				{
					SetMouseMoveWindow(pWnd);
					XMainWnd* pMainWnd = NULL;
					if ((pMainWnd = pWnd->GetMainWnd()) != NULL)
						return pMainWnd->OnMouseMove(nButton, nShift, pt);					
					//return pWnd->OnMouseMove(nButton, nShift, pt);
				}

				// ���洦��
				SetMouseMoveWindow(NULL);//SetMouseMove();
				return OnMouseMove(nButton, nShift, pt);
			}
		} break;
	case WM_MOUSEWHEEL:
		{
			if (wParam&MK_LBUTTON) nButton |= LeftButton;
			if (wParam&MK_RBUTTON) nButton |= RightButton;
			if (wParam&MK_MBUTTON) nButton |= MiddleButton;

			// ���λ��ӳ��ɿͻ�����(��ΪWM_MOUSEWHEEL����������������Ļ����)
			::ScreenToClient(m_hWnd, &pt);
			lParam = MAKELONG(pt.x, pt.y);
			short zDelta = HIWORD(wParam);
			zDelta /= WHEEL_DELTA;

			if (m_pCaptureWnd) // �в�׽
				return m_pCaptureWnd->OnMouseWheel(nButton, nShift, zDelta, pt);
			else
			{/*
				DWORD dwRtn = NOTHANDLE;
				XWindow* pWnd = GetFocusWindow();
				if (pWnd)
				{
					dwRtn = pWnd->OnMouseWheel(nButton, nShift, zDelta, pt);
					if (dwRtn != NOTHANDLE)
						return dwRtn;
				}*/
				
				// ������㴰��û�д������ǽ���Ϣ�����������Ĵ���(΢��û�����֧��!)
				XWindow* pWndMove = GetMouseMove();
				if (pWndMove)
					return pWndMove->OnMouseWheel(nButton, nShift, zDelta, pt);

				// ���洦��
				return OnMouseWheel(nButton, nShift, zDelta, pt);
			}
		} break;
	/*case WM_LBUTTONDBLCLK: // Ϊ��֧��˫������Ҫ WNDCLASS.stype |= CS_DBLCLKS 
		{
			TraceLn("left button double click");
		} break;
	case WM_MBUTTONDBLCLK:
		{
			TraceLn("middle button double click");
		} break;
	case WM_RBUTTONDBLCLK:
		{
			TraceLn("right button double click");
		} break;*/
	}
	return dwReturn;
}
//------------------------------------------------------------------------
// ������Ϣ�ַ�
DWORD XDesktop::DispatchKeyMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD dwReturn = NOTHANDLE;
	UINT nKeyCode = 0;
	UINT nKeyAscii = 0;
	UINT nShift = 0;
 
	if (GetKeyState(VK_SHIFT) & 0x8000)
	{
		nShift |= ShiftKey;
		m_dwMouseKeyState |= ShiftKeyDown;
	}
	else
		m_dwMouseKeyState &= ~ShiftKeyDown;
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		nShift |= CtrlKey;
		m_dwMouseKeyState |= CtrlKeyDown;
	}
	else
		m_dwMouseKeyState &= ~CtrlKeyDown;
	if (GetKeyState(VK_MENU) & 0x8000)
	{
		nShift |= AltKey;
		m_dwMouseKeyState |= AltKeyDown;
	}
	else
		m_dwMouseKeyState &= ~AltKeyDown;

	switch (message)
	{
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		{
			nKeyCode = wParam;
			//XWindow* pWnd = GetFocusWindow();

			if (!m_pFocusWnd || m_pFocusWnd->GetWindowType() != WT_EDIT)// || // ������㴰�ڲ���EDIT������ȼ�����
			 //(m_pFocusWnd->GetWindowType() == WT_EDIT && (nKeyCode < 'A' || nKeyCode >'Z')))
			{
				if (0 == m_AccelTable.HandleAccelKey(nKeyCode, nShift)) // �������ȼ��ͷ���
					return 0;
			}
			
			// ��Esc���Ĵ���
			if (nKeyCode == VK_ESCAPE && nShift == 0)
			{
				XMainWnd* pMainWnd = NULL;
				if (m_pActiveWnd)
					pMainWnd = m_pActiveWnd->GetMainWnd();
				if (pMainWnd && pMainWnd->CanEscape() && pMainWnd->IsVisible())
				{
					pMainWnd->OnEscape();
					SetActiveWindow(NULL);
					return 0;
				}

				_wnd_list* wndlist[] = {&m_lstTopmostWnds, &m_lstWnds};
				for (int i=0; i<sizeof(wndlist)/sizeof(_wnd_list*); i++)
				{
					if (!wndlist[i]->empty())
					{
						_wnd_rit _rend = wndlist[i]->rend();
						for (_wnd_rit it=wndlist[i]->rbegin(); it!=_rend; ++it)
						{
							pMainWnd = NULL;
							if (*it)
								pMainWnd = (*it)->GetMainWnd();
							if (pMainWnd && pMainWnd->CanEscape() && pMainWnd->IsVisible())
							{
								pMainWnd->OnEscape();
								return 0;
							}
						}
					}
				}
			}
			
			//if (pWnd) // ������еĽ��㴰�ڴ���
			//	return pWnd->GetMainWnd()->OnKeyDown(nKeyCode, nShift);
			if (m_pActiveWnd) // �������û�н���ʱ��������Լ�����
			{
				dwReturn = m_pActiveWnd->OnKeyDown(nKeyCode, nShift);
				if (dwReturn == 0) return dwReturn;
				if (0 == m_AccelTable.HandleAccelKey(nKeyCode, nShift)) // �������ȼ��ͷ���
					return 0;
			}

			// �������ڲ�����ʱ�������Լ�����
			return OnKeyDown(nKeyCode, nShift);
		} break;
	case WM_SYSKEYUP:		
	case WM_KEYUP:
		{
			nKeyCode = wParam;
			//XWindow* pWnd = GetFocusWindow();
			
			//if (pWnd) // ������еĽ��㴰�ڴ���
			//	return pWnd->GetMainWnd()->OnKeyUp(nKeyCode, nShift);
			if (m_pActiveWnd) // �������û�н���ʱ��������Լ�����
				return m_pActiveWnd->OnKeyUp(nKeyCode, nShift);

			// �������ڲ�����ʱ�������Լ�����
			return OnKeyUp(nKeyCode, nShift);
		} break;
	case WM_CHAR:
		{
			nKeyAscii = wParam;
			//XWindow* pWnd = GetFocusWindow();

			//if (pWnd) // ������еĽ��㴰�ڴ���
			//	return pWnd->OnChar(nKeyAscii);
			if (m_pActiveWnd) // �������û�н���ʱ��������Լ�����
				return m_pActiveWnd->OnChar(nKeyAscii);

			// �������ڲ�����ʱ�������Լ�����
			return OnChar(nKeyAscii);
		} break;
	}
	return dwReturn;
}
//------------------------------------------------------------------------
// ������Ϣ�ַ�
DWORD XDesktop::DispatchOtherMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	static int flags = GetTickCount();
	static int flags1 = 0;
	if (message == WM_PAINT)
	{
		OnPaint(m_pCanvas);
		return 0;
	}
#ifdef _USE_IME_EX
	else if(message == WM_INPUTLANGCHANGEREQUEST)
	{
		//PrintEventLn("message=0x%08x, wParam=0x%08x, lParam=0x%08x", message, wParam, lParam);
		if (GetTickCount() - flags < 200)
			flags1 = 1;
		flags = GetTickCount();
	}
	else if (message == WM_INPUTLANGCHANGE)
	{
		//PrintEventLn("message=0x%08x, wParam=0x%08x, lParam=0x%08x", message, wParam, lParam);
		if (flags1 == 0){
		if (m_bRecordLayoutChange)
		{
			for (int i=0; i<m_nKeyboardLayoutListCount; i++)
			{
				if (m_hKeyboardLayoutList[i] == (HKL)lParam)
				{
					m_nCurImeIndex = i;
					break;
				}
			}

			struct stTmp
			{
				HKL hkl;
				int flag;
			} tmp;
			tmp.hkl = m_hKeyboardLayoutList[m_nCurImeIndex];
			tmp.flag = 0;
			s_imeChanged((DWORD)&tmp);

			if ((!m_pFocusWnd || m_pFocusWnd->GetWindowType() != WT_EDIT))
			{
				SetIMEEnable(FALSE);
			}
		}
		}
		else
		{
			flags1 = 0;
		}
	}
#endif
	// ���ڵ�ͼ�ϵ��������(����û�취����̬֧�֣���Ϊ��ͼϵͳ�����������ϵͳ��ͷ�ļ�������ֱ������һ������)
	else if (message == 0x499)
	{
		SetCursor((int)wParam);
	}

	return OnMessage(message, wParam, lParam);
}
//------------------------------------------------------------------------
void XDesktop::PostMessage(UINT uMsgID, WPARAM wParam, LPARAM lParam)
{
	stMsg msg;
	msg.uMsgID = uMsgID;
	msg.wParam = wParam;
	msg.lParam = lParam;
	m_lstMsg.push_back(msg);
	::PostMessage(m_hWnd, WM_NULL, 0, 0); // postһ������Ϣ��ʹ֮����������DispatchMessage
}
//------------------------------------------------------------------------
void XDesktop::SetIMEEnable(BOOL bEnable)
{
#ifdef _USE_IME_EX
	if (bEnable)
	{
		m_bRecordLayoutChange = FALSE;
		HKL hkl = m_hKeyboardLayoutList[m_nCurImeIndex];
		if (ImmIsIME(hkl))
			ActivateKeyboardLayout(hkl, KLF_SETFORPROCESS);
		m_bRecordLayoutChange = TRUE;
	}
	else
	{
		m_bRecordLayoutChange = FALSE;
		ActivateKeyboardLayout(m_hKeyboardLayoutList[m_nEnImeIndex], KLF_SETFORPROCESS);
		m_bRecordLayoutChange = TRUE;
	}

	int nImeIndex = ((bEnable) ? m_nCurImeIndex : m_nEnImeIndex);
	struct stTmp
	{
		HKL hkl;
		int flag;
	} tmp;
	tmp.hkl = m_hKeyboardLayoutList[nImeIndex];
	tmp.flag = 1;
	s_imeChanged((DWORD)&tmp);
#endif
}
//------------------------------------------------------------------------
void XDesktop::OnPaint(ICanvas* pCanvas)
{
	XWindow::OnPaint(pCanvas);
	
	// ����topmost����
	_wnd_it _end = m_lstTopmostWnds.end();
	for (_wnd_it it=m_lstTopmostWnds.begin(); it!=_end; ++it)
	{
		(*it)->OnPaint(pCanvas);
	}

	// ���� ToolTip
	if (m_pToolTip && m_pToolTip->IsVisible())
		m_pToolTip->OnPaint(pCanvas);
}
//------------------------------------------------------------------------
// �������汳��
void XDesktop::OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip)
{
	DWORD style = BLIT_NORMAL;
	
	if (m_pWndBitmap == NULL) // ��Ϸ����û�б���
		return;
	
	if (m_nAlpha < 256)
	{
		if (m_nAlpha == 0) return;
		m_pWndBitmap->SetUserData(m_nAlpha);
		style |= BLIT_TRANS;
	}
	
	if(IsMask())
		style |= BLIT_MASK;

	m_pWndBitmap->Draw(pCanvas, m_rcWnd.left, m_rcWnd.top, 
		m_rcWnd.right - m_rcWnd.left, m_rcWnd.bottom - m_rcWnd.top, 
		0, 0, style);
}
//------------------------------------------------------------------------
// ���һ��������
void XDesktop::AddWindow(XWindow* pWnd)
{
	if (!pWnd || !pWnd->IsMainWindow())
		return;

	_wnd_list* wndlst = &m_lstWnds; // ���ò������¸�ֵ��������ָ�����
	if (pWnd->IsTopmost())
		wndlst = &m_lstTopmostWnds;
	
	// �����б����Ƿ���ڣ���������ɾ��
	int nVal;
	if ((nVal = FindWindow(pWnd)) >= 0) // ����
	{
		_wnd_it it = wndlst->begin();
		advance(it, nVal);
		wndlst->erase(it);
	}

	wndlst->push_back(pWnd);
	pWnd->SetParent(this);
}
//------------------------------------------------------------------------
// ɾ��һ��������
void XDesktop::RemoveWindow(XWindow* pWnd)
{
	ASSERT(pWnd);
	
	_wnd_list* wndlst = &m_lstWnds; // ���ò������¸�ֵ��������ָ�����
	if (pWnd->IsTopmost())
		wndlst = &m_lstTopmostWnds;
	
	_wnd_it _end = wndlst->end();
	for (_wnd_it it=wndlst->begin(); it!=_end; ++it)
	{
		if (*it == pWnd)
		{
			wndlst->erase(it);
			if (pWnd->IsCapture())
				pWnd->ReleaseCapture();
			if (pWnd->IsMouseMove())
				SetMouseMove(); // ��ֹ���Ƴ��Ĵ������ͷź���MouseExit��Ϣ

			// ����ǵ�ǰ�����
			if (pWnd->IsActive())
			{
				if (pWnd->IsTopmost() && m_lstTopmostWnds.size())
				{
					m_lstTopmostWnds.back()->SetActive();
				}
				else
				{
					if (m_lstWnds.size())
						m_lstWnds.back()->SetActive();
					else
						SetActiveWindow(NULL);
				}
			}
			return;
		}
	}
}
//------------------------------------------------------------------------
// ͨ��ID���Ҵ���
XWindow* XDesktop::FindWindowByID(int nID)
{
	// ���� Topmost �����б�
	for (_wnd_it it=m_lstTopmostWnds.begin(); it!=m_lstTopmostWnds.end(); it++)
		if ((*it)->GetID() == nID)
			return (*it);

	// �����������б�
	for (_wnd_it it=m_lstWnds.begin(); it!=m_lstWnds.end(); it++)
		if ((*it)->GetID() == nID)
			return (*it);
	
	return NULL;
}
//------------------------------------------------------------------------
// ���Ҵ��ڵ�˳��ţ�������ڣ��򷵻������б��е�˳��ţ����򣬷���-1
int XDesktop::FindWindow(XWindow* pWnd)
{
	_wnd_list* wndlst = &m_lstWnds; // ���ò������¸�ֵ��������ָ�����
	if (pWnd->IsTopmost())
		wndlst = &m_lstTopmostWnds;

	int i = 0;
	for (_wnd_it it=wndlst->begin(); it!=wndlst->end(); it++,i++)
		if (*it == pWnd)
			return i;
			
	return -1;
}
//------------------------------------------------------------------------
DWORD XDesktop::OnMouseDown(UINT nButton, UINT nShift, POINT pt)
{
	if (m_pActiveWnd) // �ɴ���ʧȥ����
		m_pActiveWnd->OnActivate(FALSE);

	SetActiveWindow(NULL);
	SetFocusWindow(NULL);
	return NOTHANDLE;
}
//------------------------------------------------------------------------
// �����������ϲ���Ӵ���(��ѡ��)
XWindow* XDesktop::GetChildWindowFromPoint(POINT pt, int nFlags)
{
	_wnd_list* wndlist[] = {&m_lstTopmostWnds, &m_lstWnds};
	for (int i=0; i<sizeof(wndlist)/sizeof(_wnd_list*); i++)
	{
		if (!wndlist[i]->empty())
		{
			_wnd_rit _rend = wndlist[i]->rend();
			for (_wnd_rit it=wndlist[i]->rbegin(); it!=_rend; ++it)
			{
				XWindow* pWnd = (*it);
				if (((nFlags&GCWP_SKIPINVISIBLE) && !pWnd->IsVisible()) ||
					((nFlags&GCWP_SKIPDISABLED) && !pWnd->IsEnabled()))
				{
				}
				else
				{
					RECT rc;
					pWnd->GetWindowRect(&rc);
					if (PtInRect(&rc, pt) && !pWnd->IsTransparent(pt))
						return pWnd;
				}
			}
		}
	}
	//û�������Ӵ���֮��
	return NULL;
}
//------------------------------------------------------------------------
XWindow* XDesktop::GetFirstVisibleEnableWindow()
{
	_wnd_list* wndlist[] = {&m_lstWnds, &m_lstTopmostWnds};
	for (int i=0; i<sizeof(wndlist)/sizeof(_wnd_list*); i++)
	{
		if (!wndlist[i]->empty())
		{
			_wnd_it _end = wndlist[i]->end();
			for (_wnd_it it=--wndlist[i]->end(); it!=_end; --it)
			{
				XWindow* pWnd = (*it);
				if ((pWnd->IsVisible()) && (pWnd->IsEnabled()))
					return pWnd;
			}
		}
	}
	//û�������Ӵ���֮��
	return NULL;
}
//------------------------------------------------------------------------