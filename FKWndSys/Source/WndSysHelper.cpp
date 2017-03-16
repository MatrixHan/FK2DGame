/**
*	created:		2012-6-23   0:53
*	filename: 		WndSysHelper
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
#include "../../FKResManager/IFKResManager.h"
#include "../../FKTrace/IFKTrace.h"
//------------------------------------------------------------------------
CFKTrace g_DefaultTrace;
//------------------------------------------------------------------------
CWndSysHelper::CWndSysHelper()
{
	m_pFileSystem = NULL;
	m_pResObject = NULL;
}
//------------------------------------------------------------------------
CWndSysHelper::~CWndSysHelper()
{
}
//------------------------------------------------------------------------
void CWndSysHelper::CloseWndSys()
{
	GetGameDesktop()->Release();
	CImageLoader::ReleaseInstance();
	XFontCreate::ReleaseInstance();
}
//------------------------------------------------------------------------
XWindow* CWndSysHelper::CreateWndSys(HWND hWnd, ICanvas* pCanvas, 
									 ITimeAxis* pTimeAxis, 
									 IFileSystem* pFileSystem, IResObject* pResObject, 
									 ITTFont* pSysFont)
{
	m_pFileSystem = pFileSystem;
	m_pResObject = pResObject;

	// ������Ϸ����
	XDesktop* pDesktop = XDesktop::GetInstance();
	pDesktop->SetHWnd(hWnd);
	pDesktop->SetCanvas(pCanvas);
	pDesktop->SetTimeAxis(pTimeAxis);

	//long lRetval;
	LPCSTR pString;
	IBitmap* pBmp;	
	RECT rc;
	SetRectEmpty(&rc);
	rc.right = pCanvas->GetWidth();
	rc.bottom = pCanvas->GetHeight();
	pDesktop->SetWindowPos(rc);

	// src (����ͼƬ)
	pString = pResObject->GetPropertyString("src");
	if (pString && NULL != (pBmp = CImageLoader::GetInstance()->LoadImage(pString)) )
		pDesktop->SetBitmap(pBmp);

	// caption (���⣬�ı�)
	if (pString = pResObject->GetPropertyString("caption"))
		pDesktop->SetWindowText(pString);
	else
	{
		if (pString = pResObject->GetObjectName())
		pDesktop->SetWindowText(pString);
	}

	// ϵͳ����
	if (!pSysFont)
	{
		long lRetval = 0;
		if ((pString = pResObject->GetPropertyString("fontname")) && 
			pResObject->GetPropertyLong("fontsize", lRetval, 12) && lRetval != 0)
			pSysFont = XFontCreate::GetInstance()->CreateFont(pString, lRetval, FALSE, FALSE, FALSE, FALSE, 256);
		else
			pSysFont = XFontCreate::GetInstance()->GetDefaultFont();
	}
	pDesktop->SetSysFont(pSysFont);

	// ��������
	pDesktop->SetMovable(FALSE);

	// �������еĹ��
	IResObject* pCursorObject = m_pResObject->GetSubObjectByName("CursorType");
	int nCursorCount = pCursorObject->GetSubObjectCount();
	XCursor** ppCursors = NULL;
	ppCursors = new XCursor*[nCursorCount];
	for (int i = 0; i < nCursorCount; i++)
	{
		ppCursors[i] = CreateXCursor(pCursorObject->GetSubObjectByIndex(i));
		if (!ppCursors[i])
			return NULL;
	}

	pDesktop->SetCursorCount(nCursorCount);
	pDesktop->SetAllCursor(ppCursors);
	pDesktop->SetCursor(CT_ARROW);
	pDesktop->GetCurrentCursor()->Show(TRUE);
	pDesktop->GetCurrentCursor()->InvalidRect();

	// �������ڴ����������ı���ͼƬ
	pDesktop->SetSysScrollBarBitmap(CImageLoader::GetInstance()->LoadImage(WNDMGR_SCORLLBAR_FILE));

	// ��������ToolTip�ؼ�
	XToolTip* pToolTip = new XToolTip(pDesktop);
	pToolTip->SetVisible(FALSE);
	pToolTip->SetMovable(FALSE);
	pToolTip->SetEnabled(FALSE);
	pToolTip->SetTopmost(TRUE);
	pDesktop->SetSysToolTip((XWindow*)pToolTip);

	return pDesktop;
}
//------------------------------------------------------------------------
XCursor* CWndSysHelper::CreateXCursor(IResObject* pResObject)
{
	XCursor* pCursors = new XCursor;
	// Create
	LPCSTR pString;
	long lRetval;
	int nAlpha;
	int nFrames;
	IBitmap* pCursor = NULL;

	pString = pResObject->GetPropertyString("src");
	if (pString && (NULL!=(pCursor=CImageLoader::GetInstance()->LoadImage(pString))))
		pCursors->SetBitmap(pCursor);
	else
	{
		delete pCursors;
		return NULL;
	}
	
	pResObject->GetPropertyLong("alpha", (long&)nAlpha, 256);
	pCursors->SetAlpha(nAlpha);

	pResObject->GetPropertyLong("mask", lRetval, TRUE);
	pCursors->SetMask(lRetval);

	pResObject->GetPropertyLong("animate", lRetval, FALSE);
	pCursors->SetAnimate(lRetval);

	pResObject->GetPropertyLong("interval", lRetval, 0);
	pCursors->SetInterval(lRetval);

	POINT pt;
	pResObject->GetArrayLong("hot", (long*)&pt, 0);
	pCursors->SetHots(pt);

	pResObject->GetPropertyLong("frames", (long&)nFrames, 0);
	pCursors->SetFrames(nFrames);

	RECT rc;
	SetRect(&rc, 0, 0, pCursors->GetBitmap()->GetWidth()/nFrames, pCursors->GetBitmap()->GetHeight());
	OffsetRect(&rc, pt.x, pt.y);
	pCursors->SetWindowPos(rc);

	return pCursors;
}
//------------------------------------------------------------------------
XWindow* CWndSysHelper::CreateDialogByResObject(XWindow* pParentWnd, IResObject* pResObject, int nID)
{
	if (!pResObject) return NULL;
	IResObject* pSubResObject;
	XWindow* pChildWnd;

	// ��������
	XWindow* pWindow = CreateXWindow(pParentWnd, pResObject, nID);

	// �����Ӵ���
	for(int i = 0; i < pResObject->GetSubObjectCount(); i ++)
	{
		pSubResObject = pResObject->GetSubObjectByIndex(i);
		if(!pSubResObject)
			goto err;

		
		if(pChildWnd = CreateXWindow(pWindow, pSubResObject, i))
		{
			//pWindow->AddWindow(pChildWnd);
		}
		
	}
	return pWindow;
	
err:
	TraceLn("CWndSysHelper::CreateDialogByResObject() Create failed!");
	return NULL;
}
//------------------------------------------------------------------------
XWindow* CWndSysHelper::CreateXWindow(XWindow* pParentWnd, IResObject* pResObject, int nID, XWindow* pSubclassWnd)
{
	if (!pParentWnd || !pResObject)
		return NULL;

	long lRetval;
	LPCSTR pString;
	IBitmap* pBmp = NULL;

	// ���Ͳ��������ܴ���
	LPCSTR szType = pResObject->GetTypeName();
	if (!szType || *szType == '\0')
		szType = pResObject->GetPropertyString("type");
	if (!szType)
	{
		//TraceColorLn(MY_TRACE_COLOR, "CWndSysHelper::CreateXWindow() object(\"%s\") not find 'type' property!");
		return NULL;
	}

	// ��������
	if (stricmp(szType, "window") == 0)
	{
		XMainWnd* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XMainWnd(pParentWnd);
		else
			pWnd = (XMainWnd*)pSubclassWnd;
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		pWnd->SetMovable(TRUE);

		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}

	// ����ͨ�ô���
	if (stricmp(szType, "generalwindow") == 0)
	{
		XGeneralMainWnd* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XGeneralMainWnd(pParentWnd);
		else
			pWnd = (XGeneralMainWnd*)pSubclassWnd;
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		pWnd->SetWndTraits(XWT_GENERAL, TRUE);
		// ��ȡ�����б�һ��9�����꣬ÿ������4������
#define LONG_COUNT	XGeneralMainWnd::MaxAreaCount*4
		long pos[LONG_COUNT];
		if (LONG_COUNT > pResObject->GetArrayLong("paramlist"))
			return NULL;

		int nLen = pResObject->GetArrayLong("paramlist", (long*)&pos, LONG_COUNT);
		if (!pWnd->InitParamList(pos, LONG_COUNT))
			return NULL;
		
		long lBorderAlpha;
		if (pResObject->GetPropertyLong("borderalpha", lBorderAlpha))
		{
			pWnd->SetBorderAlpha(lBorderAlpha);
		}


		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	// �����ؼ�
	if (stricmp(szType, "control") == 0)
	{
		XControl* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XControl(pParentWnd);
		else
			pWnd = (XControl*)pSubclassWnd;
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	// ������ť
	else if (stricmp(szType, "button") == 0)
	{
		XButton* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XButton(pParentWnd);
		else
			pWnd = (XButton*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		// ���ذ��£������״̬��ͼƬ
		pString = pResObject->GetPropertyString("upimg");
		if (pString && NULL != (pBmp = GetBitmap(pString)) )
			pWnd->SetBitmapUp(pBmp);

		pString = pResObject->GetPropertyString("downimg");
		if (pString && NULL != (pBmp = GetBitmap(pString)) )
			pWnd->SetBitmapDown(pBmp);

		pString = pResObject->GetPropertyString("hoverimg");
		if (pString && NULL != (pBmp = GetBitmap(pString)) )
			pWnd->SetBitmapHover(pBmp);
		
		pString = pResObject->GetPropertyString("disableimg");
		if (pString && NULL != (pBmp = GetBitmap(pString)) )
			pWnd->SetBitmapDisable(pBmp);
		
		//��ȡ����ǰ��ɫ
		long color[3];
		if (pResObject->GetArrayLong("downforecolor", color, 3))
		{			
			pWnd->SetDownForeColor(color);
			
		};

		if (pResObject->GetArrayLong("hoverforecolor", color, 3))
		{
			pWnd->SetHoverForeColor(color);				
		}		

		if (pResObject->GetArrayLong("disableforecolor", color, 3))
		{					
			pWnd->SetDisableForeColor(color);
		};

		if (pResObject->GetArrayLong("forecolor", color, 3))
		{					
			pWnd->SetUpForeColor(color);
		};


		pParentWnd->AddWindow(pWnd);
		return pWnd;	
	}
	// ������ǩ
	else if (stricmp(szType, "label") == 0)
	{
		XLabel* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XLabel(pParentWnd);
		else
			pWnd = (XLabel*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);

		// �߿�հ�
		pResObject->GetPropertyLong("margin", lRetval, 2);
		pWnd->SetMargin(lRetval);
		
		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}

	// ������ѡť
	else if (stricmp(szType, "radio") == 0)
	{
		XRadio* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XRadio(pParentWnd);
		else
			pWnd = (XRadio*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		// ��ѡť��״̬ͼ��
		pString = pResObject->GetPropertyString("stateimg");
		if (pString && NULL != (pBmp = GetBitmap(pString)) )
			pWnd->SetStateBitmap(pBmp);
		
		// forecolor ǰ��(����)��ɫ
		long color[3];
		pResObject->GetArrayLong("forecolor", color, 0);
		pWnd->SetForeColor(color[0], color[1], color[2]);
		
		pParentWnd->AddWindow(pWnd);
		return pWnd;	
	}
	// ������ѡťȺ��
	else if (stricmp(szType, "radiogroup") == 0)
	{
		XRadioGroup* pWnd = new XRadioGroup();

		int nCheckIndex;
		pResObject->GetPropertyLong("checkindex", (long&)nCheckIndex, 0);
		
		// �����Ӵ���
		XWindow* pChildWnd;
		IResObject* pSubResObject;
		for(int i = 0; i < pResObject->GetSubObjectCount(); i ++)
		{
			pSubResObject = pResObject->GetSubObjectByIndex(i);
			if (!pSubResObject)
				return NULL;
			
			if(pChildWnd = CreateXWindow(pParentWnd, pSubResObject, nID*10+i))
			{
				((XRadio*)pChildWnd)->SetGroup(pWnd);
				pWnd->AddRadio((XRadio*)pChildWnd);
				if (nCheckIndex == i)
					((XRadio*)pChildWnd)->SetState(XRadio::RS_CHECK);
				else
					((XRadio*)pChildWnd)->SetState(XRadio::RS_UNCHECK);
			}
		}
	}
	// ������ѡ��
	else if (stricmp(szType, "checkbox") == 0)
	{
		XCheckBox* pWnd = NULL; 
		if (pSubclassWnd == NULL)
			pWnd = new XCheckBox(pParentWnd);
		else
			pWnd = (XCheckBox*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		// ��ѡ���״̬ͼ��
		pString = pResObject->GetPropertyString("stateimg");
		if (pString && NULL != (pBmp = GetBitmap(pString)) )
			pWnd->SetStateBitmap(pBmp);
		
		// forecolor ǰ��(����)��ɫ
		long color[3];
		pResObject->GetArrayLong("forecolor", color, 0);
		pWnd->SetForeColor(color[0], color[1], color[2]);
		
		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	// �����ı���
	else if (stricmp(szType, "edit") == 0)
	{
		XEdit* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XEdit(pParentWnd);
		else
			pWnd = (XEdit*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		// ��ȡ�����б�һ��9�����꣬ÿ������4������
		if (pResObject->GetArrayLong("paramlist") >= XEdit::MaxAreaCount * 4)
		{
			long pos[XEdit::MaxAreaCount*4];
			
			int nLen = pResObject->GetArrayLong("paramlist", (long*)&pos);
			if (!pWnd->InitParamList(pos, nLen))
				return NULL;
		}
		 
 
		// �Ƿ�֧�ֶ���
		pResObject->GetPropertyLong("multiline", lRetval, 0);
		pWnd->SetMultiLine(lRetval);
		
		// �Ƿ�֧���Զ�����
		pResObject->GetPropertyLong("autowrap", lRetval, 0);
		pWnd->SetAutoWrap(lRetval);
					
		// password���Ƿ��������
		pResObject->GetPropertyLong("password", lRetval, FALSE);
		pWnd->SetPassword(lRetval);
		
		// readonly �Ƿ�ֻ��״̬
		pResObject->GetPropertyLong("readonly", lRetval, FALSE);
		pWnd->SetReadOnly(lRetval);
		
		// readonly �Ƿ�ֻ��״̬
		pResObject->GetPropertyLong("digitonly", lRetval, FALSE);
		pWnd->SetDigitOnly(lRetval);
		
		// �ı�������Ƴ���(1024)
		pResObject->GetPropertyLong("maxlen", lRetval, 1024);
		pWnd->SetMaxLen(lRetval);
		
		// �ı���߿�հ�
		pResObject->GetPropertyLong("margin", lRetval, 2);
		pWnd->SetMargin(lRetval);

		// ���������ַ�
		pResObject->GetPropertyLong("passwordchar", lRetval, '*');
		pWnd->SetPasswordChar((char)lRetval);

		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	// ����������
	else if (stricmp(szType, "slidebar") == 0)
	{
		XSlideBar* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XSlideBar(pParentWnd);
		else
			pWnd = (XSlideBar*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		// ������
		pString = pResObject->GetPropertyString("blockimg");
		if (pString && NULL != (pBmp = GetBitmap(pString)) )
			pWnd->SetBlockBitmap(pBmp);
		
		// �������
		RECT rc;
		if (!pResObject->GetArrayLong("blockrect", (long*)&rc))
		{
			SetRect(&rc, 0, 0, pBmp->GetWidth(), pBmp->GetHeight());
		}
		pWnd->SetBlockRect(&rc);
		
		// ����������(ˮƽ����ֱ��)
		pResObject->GetPropertyLong("style", lRetval, 0);
		pWnd->SetStyle(lRetval);
		
		// ��Χ
		long range[2];
		if (!pResObject->GetArrayLong("range", range))
		{
			range[0] = 0;
			range[1] = 99;
		}
		pWnd->SetRange(range[0], range[1]);
		
		// ��ǰλ��
		pResObject->GetPropertyLong("pos", lRetval, 0);
		pWnd->SetPos(lRetval);
		
		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	// ����������
	else if (stricmp(szType, "scrollbar") == 0)
	{
		// �����˳���ܵߵ�
		XScrollBar* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XScrollBar(pParentWnd);
		else
			pWnd = (XScrollBar*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		// ����������(ˮƽ����ֱ��)
		pResObject->GetPropertyLong("style", lRetval, 0);
		pWnd->SetStyle(lRetval);
		 
		// ��Χ
		long range[2];
		if (!pResObject->GetArrayLong("range", range))
		{
			range[0] = 0;
			range[1] = 99;
		}
		pWnd->SetScrollRange(range[0], range[1]);
		
		// �������
		SIZE size;
		if (!pResObject->GetArrayLong("size", (long*)&size))
		{
			size.cx = size.cy = GetGameDesktop()->GetSysScrollBarBitmap()->GetHeight();
		}
		pWnd->SetSize(size);
		
		// ��ǰλ��
		pResObject->GetPropertyLong("pos", lRetval, 0);
		pWnd->SetScrollPos(lRetval, FALSE);

		// �ɼ���Ŀ��
		pResObject->GetPropertyLong("visiblecount", lRetval, 0);
		pWnd->SetVisibleCount(lRetval);

		pWnd->Refresh();
		
		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	else if (stricmp(szType, "progressbar") == 0)
	{
		XProgressBar* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XProgressBar(pParentWnd);
		else
			pWnd = (XProgressBar*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		// ����������(ˮƽ����ֱ��)
		pResObject->GetPropertyLong("style", lRetval, 0);
		pWnd->SetStyle(lRetval);
		
		// ��Χ
		long range[2];
		if (!pResObject->GetArrayLong("range", range))
		{
			range[0] = 0;
			range[1] = 99;
		}
		pWnd->SetRange(range[0], range[1]);
		
		// ��ǰλ��
		pResObject->GetPropertyLong("pos", lRetval, 0);
		pWnd->SetPos(lRetval);
		
		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	else if (stricmp(szType, "staticbitmap") == 0)
	{
		XStaticBitmap* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XStaticBitmap(pParentWnd);
		else
			pWnd = (XStaticBitmap*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		long stretchrc;		
		if (pResObject->GetPropertyLong("stretch", stretchrc))			  
		{			
			pWnd->SetStretch(stretchrc);
		}
		

		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	else if (stricmp(szType, "animate") == 0)
	{
		XAnimate* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XAnimate(pParentWnd);
		else
			pWnd = (XAnimate*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		pResObject->GetPropertyLong("frame", lRetval, 1);
		pWnd->SetFrameCount(lRetval);
		pResObject->GetPropertyLong("delay", lRetval, 200);
		pWnd->SetDelay(lRetval);

		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	else if (stricmp(szType, "itemcontainer") == 0)
	{
		XItemContainer* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XItemContainer(pParentWnd);
		else
			pWnd = (XItemContainer*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		// �����߿�հ�
		pResObject->GetPropertyLong("margin", lRetval, 2);
		pWnd->SetMargin(lRetval);
		
		// �Ƿ��Զ�����
		pResObject->GetPropertyLong("autowrap", lRetval, 0);
		pWnd->SetAutoWrap(lRetval);

		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	else if (stricmp(szType, "chat") == 0)
	{
		XChat* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XChat(pParentWnd);
		else
			pWnd = (XChat*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		// �����߿�հ�
		pResObject->GetPropertyLong("margin", lRetval, 2);
		pWnd->SetMargin(lRetval);
		
		// �Ƿ��Զ�����
		pResObject->GetPropertyLong("autowrap", lRetval, 0);
		pWnd->SetAutoWrap(lRetval);
		
		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	else if (stricmp(szType, "html") == 0)
	{
		XHtml* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XHtml(pParentWnd);
		else
			pWnd = (XHtml*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		// �����߿�հ�
		pResObject->GetPropertyLong("margin", lRetval, 2);
		pWnd->SetMargin(lRetval);
		
		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	else if (stricmp(szType, "talk") == 0)
	{
		XTalk* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XTalk(pParentWnd);
		else
			pWnd = (XTalk*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		// �Ի���߿�հ�
		pResObject->GetPropertyLong("margin", lRetval, 2);
		pWnd->SetMargin(lRetval);
		
		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	else if (stricmp(szType, "listbox") == 0)
	{
		XListBox* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XListBox(pParentWnd);
		else
			pWnd = (XListBox*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		if (pResObject->GetSubObjectCount())
		{
			XWindow* pScroll = CreateXWindow(pWnd, pResObject->GetSubObjectByIndex(0), 0);
			pWnd->SetScrollBar(pScroll);
			static_cast<XScrollBar*>(pScroll)->Attach(static_cast<XWindow*>(pWnd));
		}
		
		// �б������
		if (pString = pResObject->GetPropertyString("context"))
			pWnd->ParseStringToListBox(pString);
		
		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	// ���������ı�
	else if (stricmp(szType, "linktext") == 0)
	{
		XLinkText* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XLinkText(pParentWnd);
		else
			pWnd = (XLinkText*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		long color[3];
		pResObject->GetArrayLong("notvisitedcolor", color, 0); // δ���ʹ���������ɫ
		pWnd->SetNotVisitedColor(color[0], color[1], color[2]);
		pResObject->GetArrayLong("visitedcolor", color, 0); // ���ʹ���������ɫ
		pWnd->SetVisitedColor(color[0], color[1], color[2]);
		pResObject->GetArrayLong("rollovercolor", color, 0); // ��ͣʱ����ɫ
		pWnd->SetRollOverColor(color[0], color[1], color[2]);
		pWnd->SetURL(pResObject->GetPropertyString("url"));
		
		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	// ĳЩ����¿��ܻᴴ���Լ�˽�е�tooltip
	else if (stricmp(szType, "tooltip") == 0)
	{
		XToolTip* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XToolTip(pParentWnd);
		else
			pWnd = (XToolTip*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		pParentWnd->AddWindow(pWnd);
		return pWnd;
	}
	// ����״̬��ť
	else if (stricmp(szType, "statebutton") == 0)
	{
		XStateButton* pWnd = NULL;
		if (pSubclassWnd == NULL)
			pWnd = new XStateButton(pParentWnd);
		else
			pWnd = (XStateButton*)pSubclassWnd;
		
		pWnd->SetParent(pParentWnd);
		CreateStockProperty(pWnd, pResObject, nID);
		
		// ���ذ��£������״̬��ͼƬ
		pString = pResObject->GetPropertyString("state2img");
		if (pString && NULL != (pBmp = GetBitmap(pString)) )
			pWnd->SetBitmapState2(pBmp);

		pString = pResObject->GetPropertyString("state1img");
		if (pString && NULL != (pBmp = GetBitmap(pString)) )
			pWnd->SetBitmapState1(pBmp);

		pParentWnd->AddWindow(pWnd);
		return pWnd;	
	}

	return NULL;
}
//------------------------------------------------------------------------
// ������������
void CWndSysHelper::CreateStockProperty(XWindow* pWnd, IResObject* pResObject, int nID)
{
	long lRetval;
	LPCSTR pString;
	int nAlpha;
	IBitmap* pBmp;
	RECT rc;
	SetRectEmpty(&rc);
	XWindow* pParentWnd = pWnd->GetParent();
	if (!pParentWnd)
	{
		TraceLn( "CWndSysHelper::CreateStockProperty,Parent window is NULL");
		return;
	}
	pWnd->SetResObject(pResObject);

	pWnd->SetID(nID);

	// src (����ͼƬ)
	pString = pResObject->GetPropertyString("src");
	if (pString && NULL != (pBmp = GetBitmap(pString)) )
		pWnd->SetBitmap(pBmp);

	// rect (left,top,width,height)
	if (!pResObject->GetArrayLong("rect", (long*)&rc)) // ������
	{
		if (pBmp) // �б���ͼƬ�����Ա���ͼƬ�Ĵ�С��������
		{
			SetRect(&rc, 0, 0, pBmp->GetWidth(), pBmp->GetHeight());
		}
		else
		{
			TraceLn("CWndSysHelper::CreateXWindow() object('%s') not find 'rect' property!", pResObject->GetObjectName());
			OutputDebugString("not find 'rect' property!\r\n");
		}
	}
	rc.right += rc.left;
	rc.bottom += rc.top;
	pWnd->SetWindowPos(rc);

	// caption (���⣬�ı�)
	if (pString = pResObject->GetPropertyString("caption"))
		pWnd->SetWindowText(pString);
	else
	{
		//if (pString = pResObject->GetObjectName())
		pString = "";
		pWnd->SetWindowText(pString);
	}

	// forecolor ǰ��(����)��ɫ
	long color[3];
	if (pResObject->GetPropertyString("forecolor"))
	{
		pResObject->GetArrayLong("forecolor", color, 3);
		pWnd->SetForeColor(WPixel(color[0], color[1], color[2]));
	}
	
	// backcolor ������ɫ
	if (pResObject->GetPropertyString("backcolor"))
	{
		pResObject->GetArrayLong("backcolor", color, 3);
		pWnd->SetBackColor(WPixel(color[0], color[1], color[2]));
	}

	// align
	pResObject->GetPropertyLong("align", lRetval, 0);
	pWnd->SetAlign(lRetval);

	// alpha
	pResObject->GetPropertyLong("alpha", (long&)nAlpha, 256);
	pWnd->SetAlpha(nAlpha);

	// mask
	pResObject->GetPropertyLong("mask", lRetval, TRUE);
	pWnd->SetMask(lRetval);

	// modal
	pResObject->GetPropertyLong("modal", lRetval, FALSE);
	pWnd->SetModal(lRetval);

	// enabled
	if (pParentWnd->IsEnabled())
	{
		pResObject->GetPropertyLong("enabled", lRetval, TRUE);
		pWnd->SetWndTraits(XWT_ENABLED, lRetval);
	}
	else
	{
		pWnd->SetWndTraits(XWT_ENABLED, FALSE);
	}

	// movable
	pResObject->GetPropertyLong("movable", lRetval, FALSE);
	pWnd->SetMovable(lRetval);

	// visible
	if (pParentWnd->IsVisible())
	{
		pResObject->GetPropertyLong("visible", lRetval, TRUE);
		pWnd->SetWndTraits(XWT_VISIBLE, lRetval);
		// 2005.4.29 by PeakGao Ϊ�ڴ��Ż����޸�
		//if (lRetval == 1)
		//	pWnd->LoadAllImage();
	}
	else
	{
		pWnd->SetWndTraits(XWT_VISIBLE, FALSE);
	}

	//ToolTip����
	if (pResObject->GetPropertyLong("tooltip", lRetval, -1) == TRUE)
	pWnd->SetToolTipIndex(lRetval);

	if (pWnd->IsMainWindow())
	{
		pResObject->GetPropertyLong("escape", lRetval, 0);
		pWnd->SetWndTraits(XWT_ESCAPE, lRetval);
	}
	
	// topmost
	pResObject->GetPropertyLong("topmost", lRetval, FALSE);
	pWnd->SetTopmost(lRetval);
}
//------------------------------------------------------------------------
IBitmap* CWndSysHelper::GetBitmap(LPCSTR szFile)
{
	//return NULL;
	return CImageLoader::GetInstance()->LoadImage(szFile);
}
//------------------------------------------------------------------------
// ��������ϵͳ����Ϣ����
DWORD CWndSysHelper::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD dwReturn = NOTHANDLE;
	XDesktop* pGameDesktop = GetGameDesktop();
	if (pGameDesktop) // ����ϵͳ�Ѵ����ã��������
		dwReturn = pGameDesktop->DispatchMessage(message, wParam, lParam);
	return dwReturn;
}
//------------------------------------------------------------------------
void CWndSysHelper::OnPaint(ICanvas* pCanvas)
{
	GetGameDesktop()->OnPaint(pCanvas);
}
//------------------------------------------------------------------------