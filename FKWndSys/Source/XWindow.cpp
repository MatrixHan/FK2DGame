/**
*	created:		2012-6-23   21:09
*	filename: 		XWindow
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
#include "../Include/FKWndSysCommon.h"
#include "../../FKResManager/IFKResManager.h"
//------------------------------------------------------------------------
XWindow::XWindow()
{
	SetRectEmpty(&m_rcWnd);
	m_strCaption = "";
	m_nAlpha = 256;
	m_nID = 0;
	m_pWndBitmap = NULL;

	m_dwWndTraits = XWT_VISIBLE|XWT_ENABLED|XWT_MASK;

	m_pParent = NULL;
	m_pFocusChild = NULL;
	m_pSelfFont = NULL;
 
	m_cBackColor = 0;
	m_cForeColor = 0xffffff;

	m_nAlign = AS_LEFT;
	m_nToolTipIndex = -1;
	m_pResObj = NULL;
}
//------------------------------------------------------------------------
XWindow::XWindow(XWindow* pParentWnd)
{
	SetRectEmpty(&m_rcWnd);
	m_strCaption = "";
	m_nAlpha = 256;
	m_nID = 0;
	m_pWndBitmap = NULL;

	m_dwWndTraits = XWT_VISIBLE|XWT_ENABLED|XWT_MASK;

	m_pParent = pParentWnd;
	m_pFocusChild = NULL;
	m_pSelfFont = NULL;
	
	m_cBackColor = 0;
	m_cForeColor = 0xffffff;
	
	m_nAlign = AS_LEFT;
	m_nToolTipIndex = -1;
	m_pResObj = NULL;
}
//------------------------------------------------------------------------
XWindow::~XWindow()
{
	// ע�⣺���������в�Ҫ�����麯������Ϊ��ʱ����Ѿ�ɾ��
	// DestroyWindow();
}
//------------------------------------------------------------------------
XDesktop* XWindow::GetDesktop()
{
	return XDesktop::GetInstance();
}
//------------------------------------------------------------------------
ICanvas* XWindow::GetCanvas()
{
	return GetDesktop()->GetCanvas();
}
//------------------------------------------------------------------------
ITimeAxis* XWindow::GetTimeAxis()
{
	return GetDesktop()->GetTimeAxis();
}
//------------------------------------------------------------------------
ITTFont* XWindow::GetSysFont()
{
	return GetDesktop()->GetSysFont();
}
//------------------------------------------------------------------------
ITTFont* XWindow::SetFont(ITTFont* pNewFont)
{
	ITTFont* pOldFont = GetFont();

	// ����û�ı�
	if (pOldFont == pNewFont)
		return pOldFont;
	
	m_pSelfFont = pNewFont;
	return pOldFont;
}
//------------------------------------------------------------------------
ITTFont* XWindow::GetFont() 
{
	if (m_pSelfFont)
		return m_pSelfFont;
	return GetSysFont();
}
//------------------------------------------------------------------------
XWindow* XWindow::SetFocusHelper()
{
	if (IsDesktop() || IsMainWindow())
		return INVALID_XWND;
	
	// ����û�ı�
	if (IsFocus())
		return this;
	
	XWindow* pOldWnd = GetParent()->GetFocus();
	
	GetParent()->m_pFocusChild = this;
	// ����ı�
	if (pOldWnd)
		pOldWnd->OnKillFocus();
	
	if (GetParent()->m_pFocusChild == this)
	{
		OnSetFocus();
	}
	return pOldWnd;
}
//------------------------------------------------------------------------
// ������������ڲ������ý���
XWindow* XWindow::SetFocus()
{
    XWindow* pOldWnd;
	XWindow* pWnd;
	
    if ((pOldWnd = SetFocusHelper()) != INVALID_XWND)
	{
		pWnd = this;
        do{
            pWnd = pWnd->GetParent();
        } while (pWnd->SetFocusHelper() != INVALID_XWND);

		// ����ǰ��������ڵĽ��㴰�ڼ�¼�����洰����
		GetDesktop()->SetFocusWindow(this);
    }
	
    return pOldWnd;
}
//------------------------------------------------------------------------
XWindow* XWindow::GetFocus()
{
	return m_pFocusChild;
}
//------------------------------------------------------------------------
BOOL XWindow::IsFocus()
{
	if (IsMainWindow() || IsDesktop()) return FALSE;
	return GetParent()->GetFocus() == this;
}
//------------------------------------------------------------------------
XWindow* XWindow::GetGlobalFocus()
{
	return GetDesktop()->GetFocusWindow();
}
//------------------------------------------------------------------------
BOOL XWindow::IsGlobalFocus()
{
	return GetGlobalFocus() == this;
}
//------------------------------------------------------------------------
XWindow* XWindow::SetCapture()
{
	XWindow* pOldWnd = GetCapture();
	if (IsDesktop()) return pOldWnd;
	
	if (pOldWnd == this)
		return pOldWnd;
	
	GetDesktop()->SetCaptureWindow(this);
	return pOldWnd;
}
//------------------------------------------------------------------------
XWindow* XWindow::GetCapture()
{
	return GetDesktop()->GetCaptureWindow();
}
//------------------------------------------------------------------------
BOOL XWindow::IsCapture()
{
	return GetCapture() == this;
}
//------------------------------------------------------------------------
void XWindow::ReleaseCapture()
{
	GetDesktop()->SetCaptureWindow(NULL);
}
//------------------------------------------------------------------------
XWindow* XWindow::SetMouseMove()
{
	return GetDesktop()->SetMouseMoveWindow(this);
}
//------------------------------------------------------------------------
XWindow* XWindow::GetMouseMove()
{
	return GetDesktop()->GetMouseMoveWindow();
}
//------------------------------------------------------------------------
BOOL XWindow::IsMouseMove()
{
	return GetMouseMove() == this;
}
//------------------------------------------------------------------------
// ���û���ڣ�����֧��������
XWindow* XWindow::SetActive()
{
	return GetDesktop()->SetActiveWindow(this);
}
//------------------------------------------------------------------------
// �õ�����ڣ�����֧��������
XWindow* XWindow::GetActive()
{
	return GetDesktop()->GetActiveWindow();
}
//------------------------------------------------------------------------
BOOL XWindow::IsActive()
{
	return GetActive() == this;
}
//------------------------------------------------------------------------
int XWindow::SetCursor(int nType) 
{
	int nTypeOld = GetDesktop()->GetCurrentCursorType();
	if (nTypeOld == nType)
		return nTypeOld;

	POINT pt;
	if (XCursor::m_bUseOwnCursor)
	{
		GetCursor()->Show(FALSE);
		GetCursor()->InvalidRect();
	}
	GetCursor()->GetMouseXY(pt);
	GetDesktop()->SetCurrentCursorType(nType);
	GetCursor()->SetMouseXY(pt); // У�����λ��
	if (XCursor::m_bUseOwnCursor)
		GetCursor()->Show(TRUE);
	return nTypeOld;
}
//------------------------------------------------------------------------
XCursor* XWindow::GetCursor()
{
	return GetDesktop()->GetCurrentCursor();
}
//------------------------------------------------------------------------
void XWindow::WritePoint(POINT pt)
{
	GetDesktop()->SetBakPoint(pt);
}
//------------------------------------------------------------------------
POINT XWindow::ReadPoint()
{
	return GetDesktop()->GetBakPoint();
}
//------------------------------------------------------------------------
void XWindow::WriteRect(RECT& rc)
{
	GetDesktop()->SetBakRect(rc);
}
//------------------------------------------------------------------------
RECT& XWindow::ReadRect()
{
	return GetDesktop()->GetBakRect();
}
//------------------------------------------------------------------------
// �����ϵ�ĳ���Ƿ�͸��
BOOL XWindow::IsTransparent(POINT& pt)
{
	if (!m_pWndBitmap)
		return TRUE;
	POINT ptSelect = pt;
	ptSelect.x -= m_rcWnd.left;
	ptSelect.y -= m_rcWnd.top;
	return m_pWndBitmap->IsTransparent(ptSelect);
}
//------------------------------------------------------------------------
void XWindow::SetVisible(BOOL bVisible/* = TRUE*/)
{
	if (bVisible)
	{ 
		m_dwWndTraits |= XWT_VISIBLE;
		LoadAllImage();
		if (GetDesktop()->GetActiveWindow() == NULL)
			SetActive();
		
	}
	else
	{
		m_dwWndTraits &= ~XWT_VISIBLE;
		ReleaseAllImage();
		if (IsGlobalFocus())
			GetDesktop()->SetFocusWindow(NULL);
		else
		{
			// �Ӵ���
			for (_wnd_it it=m_lstWnds.begin(); it!=m_lstWnds.end(); it++)
			{
				if ((*it)->IsGlobalFocus())
				{
					GetDesktop()->SetFocusWindow(NULL);
					break;
				}
			}
		}
		if (IsMouseMove())
			GetDesktop()->SetMouseMoveWindow(NULL);
		else
		{
			// �Ӵ���
			for (_wnd_it it=m_lstWnds.begin(); it!=m_lstWnds.end(); it++)
			{
				if ((*it)->IsMouseMove())
				{
					GetDesktop()->SetMouseMoveWindow(NULL);
					break;
				}
			}
		}
		
		if (IsMainWindow() && IsActive())
		{
			GetDesktop()->SetActiveWindow(GetDesktop());
			return;			
		}
	}
}
//------------------------------------------------------------------------
void XWindow::SetEnabled(BOOL bEnabled/* = TRUE*/)
{
	if (bEnabled)
	{
		m_dwWndTraits |= XWT_ENABLED;
		if (GetDesktop()->GetActiveWindow() == NULL)
			SetActive();
	}
	else
	{
		m_dwWndTraits &= ~XWT_ENABLED;
		if (IsGlobalFocus())
			GetDesktop()->SetFocusWindow(NULL);
		else
		{
			// �Ӵ���
			for (_wnd_it it=m_lstWnds.begin(); it!=m_lstWnds.end(); it++)
			{
				if ((*it)->IsGlobalFocus())
				{
					GetDesktop()->SetFocusWindow(NULL);
					break;
				}
			}
		}
	}
}
//------------------------------------------------------------------------
void XWindow::SetMask(BOOL bMask/* = TRUE*/)
{
	if (bMask)
		m_dwWndTraits |= XWT_MASK;
	else
		m_dwWndTraits &= ~XWT_MASK;
}
//------------------------------------------------------------------------
void XWindow::SetMovable(BOOL bMovable/* = TRUE*/)
{
	if (bMovable)	
		m_dwWndTraits |= XWT_MOVABLE;
	else	
		m_dwWndTraits &= ~XWT_MOVABLE;
}
//------------------------------------------------------------------------
void XWindow::SetModal(BOOL bModal/* = TRUE*/)
{
	if (bModal)
		m_dwWndTraits |= XWT_MODAL;
	else
		m_dwWndTraits &= ~XWT_MODAL;
}
//------------------------------------------------------------------------
void XWindow::SetTopmost(BOOL bTopmost/* = TRUE*/)
{ 
	if (bTopmost)
		m_dwWndTraits |= XWT_TOPMOST;
	else	
		m_dwWndTraits &= ~XWT_TOPMOST;
}
//------------------------------------------------------------------------
void XWindow::SetWndTraits(DWORD dwTraits, BOOL bTraits)
{
	if (bTraits)
		m_dwWndTraits |= dwTraits;
	else
		m_dwWndTraits &= ~dwTraits;
}
//------------------------------------------------------------------------
void XWindow::AddWindow(XWindow* pWnd)
{
	if (!pWnd) return;

	// �����б����Ƿ���ڣ���������ɾ��
	int nVal;
	if ((nVal = FindWindow(pWnd)) >= 0) // ����
	{
		_wnd_it it = m_lstWnds.begin();
		advance(it, nVal);
		m_lstWnds.erase(it);
	}

	m_lstWnds.push_back(pWnd);
	pWnd->SetParent(this);
}
//------------------------------------------------------------------------
void XWindow::RemoveWindow(XWindow* pWnd)
{
	if (!pWnd) return;
	
	_wnd_it _end = m_lstWnds.end();
	for (_wnd_it it=m_lstWnds.begin(); it!=_end; ++it)
	{
		if (*it == pWnd)
		{
			m_lstWnds.erase(it);
			if (pWnd->IsCapture())
				pWnd->ReleaseCapture();
			SetMouseMove(); // ��ֹ���Ƴ��Ĵ������ͷź���MouseExit��Ϣ

			//����ǵ�ǰ���㴰��
			if (pWnd->IsFocus())
			{
				if(m_lstWnds.size())
					m_lstWnds.back()->SetFocus();
				else
				{
					m_pFocusChild = NULL;
					GetDesktop()->SetFocusWindow(NULL);
				}
			}
			return;
		}
	}
}
//------------------------------------------------------------------------
XWindow* XWindow::FindWindowByID(int nID)
{
	for (_wnd_it it=m_lstWnds.begin(); it!=m_lstWnds.end(); it++)
		if ((*it)->GetID() == nID)
			return (*it);

	return NULL;
} 
//------------------------------------------------------------------------
int XWindow::FindWindow(XWindow* pWnd)
{
	int i = 0;
	for (_wnd_it it=m_lstWnds.begin(); it!=m_lstWnds.end(); it++,i++)
		if (*it == pWnd)
			return i;

	return -1;
}
//------------------------------------------------------------------------
IBitmap* XWindow::SetBitmap(IBitmap* pBmp)
{
	IBitmap* pOldBmp = GetBitmap();
	m_pWndBitmap = pBmp;
	return pOldBmp;
}
//------------------------------------------------------------------------
IBitmap* XWindow::GetBitmap()
{
	return m_pWndBitmap;
}
//------------------------------------------------------------------------
void XWindow::SetAlpha(int nAlpha)
{
	m_nAlpha = nAlpha;
}
//------------------------------------------------------------------------
int XWindow::GetAlpha()
{
	return m_nAlpha;
}
//------------------------------------------------------------------------
void XWindow::SetWindowPos(RECT& rc)
{
	CopyRect(&m_rcWnd, &rc);
	InvalidRect();
}
//------------------------------------------------------------------------
void XWindow::GetWindowRect(LPRECT lpRect)
{
	CopyRect(lpRect, &m_rcWnd);
}
//------------------------------------------------------------------------
void XWindow::GetClientRect(LPRECT lpRect)
{
	CopyRect(lpRect, &m_rcWnd);
	OffsetRect(lpRect, -lpRect->left, -lpRect->top);
}
//------------------------------------------------------------------------
void XWindow::MoveWindow(int dx, int dy)
{
	InvalidRect();
	OffsetRect(&m_rcWnd, dx, dy);
	InvalidRect();

	// �Ӵ���
	if (!m_lstWnds.empty())
	{
		_wnd_it it;
		for (it = m_lstWnds.begin(); it != m_lstWnds.end(); it++)
		{
			(*it)->MoveWindow(dx, dy);
		}
	}
}
//------------------------------------------------------------------------
void XWindow::InvalidRect(LPRECT lpRect/* = NULL*/)	
{
	//���ӻ��ƾ���
	RECT rc;
	if (lpRect)
		CopyRect(&rc, lpRect);
	else
	{
		RECT rcParent;
		CopyRect(&rc, &m_rcWnd);
		if (m_pParent)
		{
			m_pParent->GetWindowRect(&rcParent);
			IntersectRect(&rc, &m_rcWnd, &rcParent);
		}
	}

	ICanvas* pCanvas = GetCanvas();
	pCanvas->AddDirtyRect(rc);
	if (!pCanvas->IsInEqualOut())
		pCanvas->AddOutDirtyRect(rc);
}
//------------------------------------------------------------------------
void XWindow::SetWindowText(LPCSTR lpszString)
{
	if (!lpszString) return;
	m_strCaption = lpszString;
	OnTextChanged();
}
//------------------------------------------------------------------------
LPCSTR XWindow::GetWindowText()
{
	return m_strCaption.c_str();
}
//------------------------------------------------------------------------
void XWindow::ToolTipSetInfo(IBitmap* pBitmap, LPCSTR szToolTip, WPixel cBackColor, WPixel cForeColor, int nInterval)
{
	XToolTip* pToolTip = (XToolTip*)GetDesktop()->GetSysToolTip();
	if (!pToolTip) return;
	pToolTip->SetInfo(pBitmap, szToolTip, cBackColor, cForeColor, nInterval);
}
//------------------------------------------------------------------------
void XWindow::ToolTipMoveTo(int x, int y)
{
	XToolTip* pToolTip = (XToolTip*)GetDesktop()->GetSysToolTip();
	if (pToolTip)
		pToolTip->SetNewPos(x, y);
}
//------------------------------------------------------------------------
void XWindow::SetID(int nID)
{
	m_nID = nID;
}
//------------------------------------------------------------------------
int XWindow::GetID()
{
	return m_nID;
}
//------------------------------------------------------------------------
XWindow* XWindow::SetParent(XWindow* pParent)
{
	return m_pParent = pParent;
}
//------------------------------------------------------------------------
XWindow* XWindow::GetParent()
{
	return m_pParent;
}
//------------------------------------------------------------------------
void XWindow::Release()
{
	// ������Ӵ��ڣ������ͷ�
	while (!m_lstWnds.empty())
	{
		XWindow* pWnd= *(m_lstWnds.begin());
		pWnd->Release();
	}

	// �ͷű�����
	DestroyWindow();
}
//------------------------------------------------------------------------
void XWindow::DestroyWindow()
{
	// �������ھ�����Ϊ��
	InvalidRect(NULL);

	if (m_pParent)
		m_pParent->RemoveWindow(this);

	SAFE_RELEASE_IMAGE(m_pWndBitmap);
	s_clicked.disconnectAll();

	delete this;
}
//------------------------------------------------------------------------
void XWindow::PostExitMessage()
{
	GetDesktop()->PostMessage(0, (WPARAM)this, 0);
}
//------------------------------------------------------------------------
XMainWnd* XWindow::GetMainWnd()
{
	XWindow* pWnd = this;
	while (pWnd)
	{
		if (pWnd->IsMainWindow())
			return (XMainWnd*)pWnd;
		pWnd = pWnd->GetParent();
	}
	return NULL;
}
//------------------------------------------------------------------------
void XWindow::RegisterAccelKey(UINT nKeyCode, UINT nShift, XWindow* pWnd)
{
	GetDesktop()->GetAccelTable()->AddAccelKey(nKeyCode, nShift, pWnd);
}
//------------------------------------------------------------------------
void XWindow::UnRegisterAccelKey(UINT nKeyCode, UINT nShift)
{
	GetDesktop()->GetAccelTable()->RemoveAccelKey(nKeyCode, nShift);
}
//------------------------------------------------------------------------
void XWindow::UnRegisterAccelKey()
{
	GetDesktop()->GetAccelTable()->RemoveAccelKey(this);
}
//------------------------------------------------------------------------
void XWindow::OnPaint(ICanvas* pCanvas)
{
	if (!IsVisible()) 
		return;
 
	RECT rc;
	if (m_pParent)
	{
		m_pParent->GetWindowRect(&rc);
		IntersectRect(&rc, &m_rcWnd, &rc);
	}
	else
		CopyRect(&rc, &m_rcWnd);
	static RECT rcScreen = {0, 0, 1024, 768};
	IntersectRect(&rc, &rcScreen, &rc);

	if (IsRectEmpty(&rc))
		return;
	
	// ����������࣬���Ʊ�����
	if (pCanvas->IsInDirty(rc))
	{
		OnEraseBkgnd(pCanvas, rc);
		OnDraw(pCanvas, rc);
	}

	// �����Ӵ���
	_wnd_it _end = m_lstWnds.end();
	for (_wnd_it it=m_lstWnds.begin(); it!=_end; ++it)
	{
		(*it)->OnPaint(pCanvas);
	}
}
//------------------------------------------------------------------------
// ��ǰ��
void XWindow::OnDraw(ICanvas* pCanvas, RECT& rcClip)
{

}
//------------------------------------------------------------------------
// ������
void XWindow::OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip)
{
	DWORD style = BLIT_NORMAL;

	// û��ͼƬ�����һ������
	if (!m_pWndBitmap)
	{
		if (m_nAlpha == 0) return;
		if (m_nAlpha < 256)
			pCanvas->DrawBox(rcClip, GetBackColor(), style|BLIT_TRANS, m_nAlpha); // WPixel(212,208,200)
		else
			pCanvas->Clear(GetBackColor(), &rcClip);
		return;
	}

	// ����ͼƬ����
	if (m_nAlpha < 256)
	{
		if (m_nAlpha == 0) return;
		m_pWndBitmap->SetUserData(m_nAlpha);
		style |= BLIT_TRANS;
	}
	
	if(IsMask())
		style |= BLIT_MASK;

	m_pWndBitmap->Draw(pCanvas, rcClip.left, rcClip.top, rcClip.right - rcClip.left, rcClip.bottom - rcClip.top, 
		rcClip.left - m_rcWnd.left, rcClip.top - m_rcWnd.top, style);
}
//------------------------------------------------------------------------
// ��Ϣ���
DWORD XWindow::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefMessage(uMsg, wParam, lParam);
}
//------------------------------------------------------------------------
// Ĭ����Ϣ����
DWORD XWindow::DefMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IsDesktop()) // ����-1��������
		return NOTHANDLE;
	return 0;	
}
//------------------------------------------------------------------------
// �����������ϲ���Ӵ���(��ѡ��)
XWindow* XWindow::GetChildWindowFromPoint(POINT pt, int nFlags)
{
	if (m_lstWnds.empty())
		return NULL;
	
	_wnd_it it = m_lstWnds.end();
	for (--it;; --it)
	{
		XWindow* pWnd = (*it);
		if (((nFlags&GCWP_SKIPINVISIBLE) && !pWnd->IsVisible()) ||
			((nFlags&GCWP_SKIPDISABLED) && !pWnd->IsEnabled()))
			goto _done;

		RECT rc;
		pWnd->GetWindowRect(&rc);
		if (PtInRect(&rc, pt) && !pWnd->IsTransparent(pt))
			return pWnd;

_done:
		if (it == m_lstWnds.begin())
			break;
	}
	//û�������Ӵ���֮��
	return NULL;
}
//------------------------------------------------------------------------
// �õ��������Ĵ���(��ѡ��)
XWindow* XWindow::GetWindowBelowCursor(POINT pt, int nFlags)
{
	XWindow* pWnd = GetChildWindowFromPoint(pt, nFlags);
	XWindow* pWndChild = pWnd;
	while (pWndChild)
	{
		pWndChild = pWndChild->GetChildWindowFromPoint(pt, nFlags);
		if (pWndChild)
			pWnd = pWndChild;
	}
	return pWnd;
}
//------------------------------------------------------------------------
DWORD XWindow::OnMouseDown(UINT nButton, UINT nShift, POINT pt)
{
	if (!IsVisible())
	{
		return -1;
	}

	if (nButton == LeftButton) // ȷ������Ѿ�����
	{
		SetCapture(); 
		SetFocus();
		if (IsMovable())
		{
			WritePoint(pt);
			WriteRect(m_rcWnd);
		}
	}

	// ͬʱ��������������Ϊ�����
	GetMainWnd()->SetActive();
	return 0;
}
//------------------------------------------------------------------------
DWORD XWindow::OnMouseUp(UINT nButton, UINT nShift, POINT pt)
{
	if (nButton == LeftButton) // ȷ������Ѿ��ͷ�
	{
		if (IsCapture())
			ReleaseCapture();
		
		if (IsMovable()) // �ж��Ӵ����Ƿ��Ƶ��˸�������
		{
			RECT rc;
			GetParent()->GetWindowRect(&rc);
			if (!IntersectRect(&rc, &rc, &m_rcWnd))
				SetWindowPos(ReadRect());
		}
	}
	return 0;
}
//------------------------------------------------------------------------
DWORD XWindow::OnMouseMove(UINT nButton, UINT nShift, POINT pt)
{
	if (nButton == LeftButton) // ��������϶�
	{
		if (IsMovable() && IsCapture())
		{
			int dx = pt.x - ReadPoint().x;
			int dy = pt.y - ReadPoint().y;
			MoveWindow(dx, dy);
			WritePoint(pt);
		}
	}
	
	// ����ڱ��������ƶ�
	SetMouseMove();

	// tooltip����
	XToolTip* pToolTip = (XToolTip*)GetDesktop()->GetSysToolTip();
	if (pToolTip && pToolTip->IsValid() && GetWindowType() >= WT_CONTROL
		&& GetWindowType() != WT_EDIT)
		pToolTip->SetNewPos(pt.x, pt.y);

	return 0;
}
//------------------------------------------------------------------------
void XWindow::LoadAllImage()
{
	if (m_pResObj && !m_pWndBitmap)
		m_pWndBitmap = CImageLoader::GetInstance()->LoadImage(m_pResObj->GetPropertyString("src"));
	
	// �Ӵ���
	_wnd_it _end = m_lstWnds.end();
	for (_wnd_it it=m_lstWnds.begin(); it!=_end; ++it)
		(*it)->LoadAllImage();
}
//------------------------------------------------------------------------
void XWindow::ReleaseAllImage()
{
	SAFE_RELEASE_IMAGE(m_pWndBitmap);
	// �Ӵ���
	_wnd_it _end = m_lstWnds.end();
	for (_wnd_it it=m_lstWnds.begin(); it!=_end; ++it)
		(*it)->ReleaseAllImage();
}
//------------------------------------------------------------------------
void XWindow::OnMouseEnter()
{ 
	XDesktop *pDesktop = GetDesktop();
	if (pDesktop == NULL || m_nToolTipIndex == -1)
	{
		return;
	}
	
	LPCSTR szToolTip = CToolTipLoader::GetInstance().GetToolTip(m_nToolTipIndex);
	if (szToolTip == NULL || strlen(szToolTip) == 0)
	{
		return;
	}

	XToolTip *pToolTip = (XToolTip *)pDesktop->GetSysToolTip();
	if (pToolTip)
	{
		pToolTip->SetText(szToolTip);
		POINT pt;
		pt.x = m_rcWnd.left;
		pt.y = m_rcWnd.top;
		pToolTip->Show(&pt);
	}
}
//------------------------------------------------------------------------