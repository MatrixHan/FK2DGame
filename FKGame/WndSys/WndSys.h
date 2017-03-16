/**
*	created:		2012-6-22   0:28
*	filename: 		WndSys
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#define _USE_IME_EX
//------------------------------------------------------------------------
#pragma warning( disable : 4251 )
//------------------------------------------------------------------------
#include "../Timer/ITimerAxis.h"
#include "../../FKCanvas/IFKCanvas.h"
#include <set>
#include <list>
#include <hash_map>
#include <string>
#include <vector>
//------------------------------------------------------------------------
using stdext::hash_map;
using std::string;
using std::vector;
using std::list;
//------------------------------------------------------------------------
// ���ӷ�ʽ����
//#if defined(WNDSYS_STATICLIB) || defined(_LIB)
//	#define WNDSYS_API
//	#if !defined(_LIB)
//		#ifdef _DEBUG
//			#pragma comment (lib,"FKWndSysStatic_D.lib")
//		#else
//			#pragma comment (lib,"FKWndSysStatic.lib")
//		#endif
//	#endif
//#elif defined(WNDSYS_EXPORTS)
#ifdef FKWNDSYS_EXPORTS
	#define WNDSYS_API __declspec(dllexport)
#else
	#define WNDSYS_API __declspec(dllimport)
	#ifdef _DEBUG
		#pragma comment (lib,"../../Bin/BinGame/Debug/FKWndSys_D.lib")
	#else
		#pragma comment (lib,"../../Bin/BinGame/Release/FKWndSys.lib")
	#endif
#endif
//------------------------------------------------------------------------
#ifndef TraceLn
	#ifdef _DEBUG
		#define TraceLn g_DefaultTrace.ThreadSafeDebugPrintLn
		#define TraceColorLn g_DefaultTrace.DebugPrintColorLn
	#else
		#define TraceLn __noop
		#define TraceColorLn __noop
	#endif
#endif

#ifndef	MY_TRACE_COLOR
#define MY_TRACE_COLOR		RGB(146,0,123)
#endif
//------------------------------------------------------------------------
// ����ϵͳ�汾
#ifndef WNDSYS_VERSION
#define WNDSYS_VERSION 101
#endif
//------------------------------------------------------------------------
#define WNDMGR_SCORLLBAR_FILE	"Common\\ScrollBar\\ScrollBar.bmp"
//------------------------------------------------------------------------
/*
 *	�ź����Ӻ�
 */
// �޲����ź�
#define CONN_0(theClass, memberFxn)							new Connection0<theClass>(this, &theClass::memberFxn)
// �������ź�(����:DWORD�����еĴ������ź����ս�����ʵ�ʲ����ĵ�ַת��ΪDWORD����)
#define CONN_1(theClass, memberFxn)							new Connection1<theClass>(this, &theClass::memberFxn)
// �����źţ�ʹ�������д���(�޲���)
#define CONN_clicked(classPtr, theClass, memberFxn)			classPtr->s_clicked.connect(CONN_0(theClass, memberFxn));
// �س����źţ�ʹ����XEdit(�޲���)
#define CONN_returnPressed(classPtr, theClass, memberFxn)	classPtr->s_returnPressed.connect(CONN_0(theClass, memberFxn));
// Radio��ѡ���źţ�ʹ����XRadio(�޲���)
#define CONN_selected(classPtr, theClass, memberFxn)		classPtr->s_selected.connect(CONN_0(theClass, memberFxn));
// �������źţ�ʹ�������д���(����ת��: XWindow* pWnd=(XWindow*)dwParam)
#define CONN_mouseEnter(classPtr, theClass, memberFxn)		classPtr->s_mouseEnter.connect(CONN_1(theClass, memberFxn));
// ����˳��źţ�ʹ�������д���(����ת��: XWindow* pWnd=(XWindow*)dwParam)
#define CONN_mouseExit(classPtr, theClass, memberFxn)		classPtr->s_mouseExit.connect(CONN_1(theClass, memberFxn));
// ����ѡ���źţ�ʹ����XRadioGroup(����ת��: int nIndex=*(int*)dwParam)
#define CONN_indexChecked(classPtr, theClass, memberFxn)	classPtr->s_indexChecked.connect(CONN_1(theClass, memberFxn));
// λ�øı��źţ�ʹ����XSlideBar,XScrollBar(����ת��: int nPos=*(int*)dwParam)
#define CONN_posChanged(classPtr, theClass, memberFxn)		classPtr->s_posChanged.connect(CONN_1(theClass, memberFxn));
// ѡ��ı��źţ�ʹ����XListBox(����ת��: int nIndex=*(int*)dwParam)
#define CONN_selChanged(classPtr, theClass, memberFxn)		classPtr->s_selChanged.connect(CONN_1(theClass, memberFxn));
// ѡ���źţ�ʹ����XCheckBox(����ת��: BOOL bChecked=*(BOOL*)dwParam)
#define CONN_checked(classPtr, theClass, memberFxn)			classPtr->s_checked.connect(CONN_1(theClass, memberFxn));
// �ı��ı��źţ�ʹ����XEdit(����ת��: LPCSTR lpStr=(LPCSTR)dwParam)
#define CONN_textChanged(classPtr, theClass, memberFxn)		classPtr->s_textChanged.connect(CONN_1(theClass, memberFxn));
// URL�źţ�ʹ����XLinkText(����ת��: LPCSTR lpURL=(LPCSTR)dwParam)
#define CONN_runURL(classPtr, theClass, memberFxn)			classPtr->s_runURL.connect(CONN_1(theClass, memberFxn));
// ���뷨�ı��ź�
#define CONN_imeChanged(classPtr, theClass, memberFxn)		classPtr->s_imeChanged.connect(CONN_1(theClass, memberFxn));
//------------------------------------------------------------------------
// �����źŶ���Ĺؼ���
#define Signals	public
//------------------------------------------------------------------------
// ����ϵͳû�д���ĳ����Ϣʱ�������ֵ��������ϵͳ��������
#define NOTHANDLE	(DWORD)(-1)	
//------------------------------------------------------------------------
// ��Ч����
#define INVALID_XWND	((XWindow*)0xFFFFFFFF)
//------------------------------------------------------------------------
// �������(Cursor Type)
#define CT_ARROW	0	// ��ͷ
#define CT_HAND		1	// ����
#define CT_BEAN		2	// I����
#define CT_VSIZE	3	// ��ֱ�仯�ߴ�
//------------------------------------------------------------------------
// ��������(XWindow Traits)
#define XWT_VISIBLE			0x0001	// �Ƿ�ɼ�
#define XWT_ENABLED			0x0002	// �Ƿ�ʹ��
#define XWT_MODAL			0x0004	// �Ƿ�ģ̬
#define XWT_MASK			0x0008	// �Ƿ񲻻���͸��ɫ
#define XWT_MOVABLE			0x0010	// �Ƿ�����ƶ�(���ܳ�Խ������)
#define XWT_TOPMOST			0x0040	// �Ƿ񶥲�
#define XWT_ACTIVE			0x0080	// �Ƿ񼤻�
#define XWT_ESCAPE			0x0100	// �Ƿ���ӦESC��
#define XWT_GENERAL			0x0200	// �Ƿ���ͨ�ô���
//------------------------------------------------------------------------
// ��ȡĳ��������Ӵ���ʱ�Ĺ���ѡ��
#define GCWP_ALL             0x0000		// �������κδ���
#define GCWP_SKIPINVISIBLE   0x0001		// �������ɼ��Ĵ���
#define GCWP_SKIPDISABLED    0x0002		// ������Ч״̬�Ĵ���
#define GCWP_SKIPTRANSPARENT 0x0004		// ����͸���Ĵ���
//------------------------------------------------------------------------
// ���뷽ʽ (Align Style)
#define AS_LEFT				0
#define AS_CENTER			1
#define AS_RIGHT			2
//------------------------------------------------------------------------
// ��������(OnMouseDown�Ⱥ����е�nButon��������ֵ)
enum MouseKeys
{
	LeftButton=1,	// ������
	RightButton=2,	// ����Ҽ�
	MiddleButton=4,	// ����м�
};
//------------------------------------------------------------------------
// ��������״̬
enum MouseKeyState
{
	LeftButtonDown=1,	// ���������ڰ���״̬
	RightButtonDown=2,	// ����Ҽ����ڰ���״̬
	MiddleButtonDown=4,	// ����м����ڰ���״̬
	ShiftKeyDown=0x10,
	CtrlKeyDown=0x20,
	AltKeyDown=0x40,
};
//------------------------------------------------------------------------
// ���������(OnMouseDown�Ⱥ����е�nShift��������ֵ)
enum ShiftKeys
{
	ShiftKey=1,		// Shift��
	CtrlKey=2,		// Ctrl��
	AltKey=4,		// Alt��
};
//------------------------------------------------------------------------
// Ŀǰ֧�ֵĴ�������
enum WindowType
{
	WT_UNKNOW=-1,
	WT_DESKTOP,
	WT_MAINWND,
	WT_WINDOW,
	WT_CONTROL,
	WT_BUTTON,
	WT_STATEBUTTON,
	WT_CHAT,
	WT_CHECKBOX,
	WT_EDIT,
	WT_LABEL,
	WT_LISTBOX,
	WT_PROGRESSBAR,
	WT_RADIO,
	WT_SCROLLBAR,
	WT_SLIDEBAR,
	WT_STATICBITMAP,
	WT_TOOLTIP,
	WT_LINKTEXT,
	WT_ITEMCONTAINER,
	WT_HTML,
	WT_TALK,
	WT_ANIMATE,
};
//------------------------------------------------------------------------
// ��ǰ����
class SlotHolder;
class CImageLoader;
class XFontCreate;
class XCursor;
class XWindow;
class XAccelTable;
class XDesktop;
class XMainWnd;
class XGeneralMainWnd;
class XControl;
class XButton;
class XStateButton;
class XLabel;
class XRadio;
class XRadioGroup;
class XCheckBox;
class XEdit;
class XSlideBar;
class XScrollBar;
class XProgressBar;
class XStaticBitmap;
class XListBox;
class XLinkText;
class XToolTip;
class XItem;
class XItemText;
class XItemLinkText;
class XItemImage;
class XItemLinkImage;
class XItemContainer;
class XLineParser;
class XChat;
class XHtml;
class XAnimate;
class XTalk;
class CWndSysHelper;
//------------------------------------------------------------------------
struct IBitmap;
struct IFileSystem;
struct IResObject;
//------------------------------------------------------------------------
// �źŲ����
class WNDSYS_API BasicConnection
{
public:
    virtual SlotHolder* getSlotHolder() const = 0;
	virtual void release() = 0;
};
//------------------------------------------------------------------------
class WNDSYS_API BasicConnection0 : public BasicConnection
{
public:
    virtual void trigger() = 0;
};
//------------------------------------------------------------------------
class WNDSYS_API BasicConnection1 : public BasicConnection
{
public:
    virtual void trigger(DWORD) = 0;
};
//------------------------------------------------------------------------
class WNDSYS_API BasicSignal
{
public:
    virtual void disconnectSlot(SlotHolder* slot) = 0;
	virtual void disconnectAll() = 0;
};
//------------------------------------------------------------------------
// ��ӵ����
class WNDSYS_API SlotHolder
{
private:
    typedef std::set<BasicSignal*>		SenderSet;
    typedef SenderSet::const_iterator	const_iterator;
public:
    SlotHolder() {}
    virtual ~SlotHolder();
    void connectTo(BasicSignal* sender);
    void disconnectFrom(BasicSignal* sender);
    void disconnectAll();
private:
    SenderSet _senders;
};
//------------------------------------------------------------------------
template<class TargetType>
class WNDSYS_API Connection0 : public BasicConnection0
{
public:
    Connection0() : _object(0), _voidMemberFunction(0) {}
    Connection0(TargetType* anObject, void (TargetType::*aMemberFunction)()) : _object(anObject), _voidMemberFunction(aMemberFunction) {}
    virtual void trigger()						{(_object->*_voidMemberFunction)();}
    virtual SlotHolder* getSlotHolder() const	{return _object;}
	virtual void release()						{delete this;}
private:
    TargetType* _object;
    void (TargetType::* _voidMemberFunction)();
};
//------------------------------------------------------------------------
template<class TargetType>
class WNDSYS_API Connection1 : public BasicConnection1
{
public:
    Connection1() : _object(0),_voidMemberFunction(0) {}
    Connection1(TargetType* anObject, void (TargetType::*aMemberFunction)(DWORD)) : _object(anObject), _voidMemberFunction(aMemberFunction) {}
    virtual void trigger(DWORD a1)				{(_object->*_voidMemberFunction)(a1);}
    virtual SlotHolder* getSlotHolder() const	{return _object;}
	virtual void release()						{delete this;}
private:
    TargetType* _object;
    void (TargetType::* _voidMemberFunction)(DWORD);
};
//------------------------------------------------------------------------
class WNDSYS_API Signal0 : public BasicSignal
{
protected:
    typedef std::list<BasicConnection0 *>  ConnectionList;
    ConnectionList _connectedSlots;
public:
    Signal0() {}
	virtual ~Signal0();
    virtual void disconnectAll();
    void disconnectSlot(SlotHolder* slot);
    void connect(BasicConnection0* conn);
    void emit();
    void operator()();
};
//------------------------------------------------------------------------
class WNDSYS_API Signal1 : public BasicSignal
{
protected:
	typedef std::list<BasicConnection1*>  ConnectionList;
    ConnectionList _connectedSlots;
public:
    Signal1() {}
	virtual ~Signal1();
    virtual void disconnectAll();
    void disconnectSlot(SlotHolder* slot);
    void connect(BasicConnection1* conn);
    void emit(DWORD dwParam);
    void operator()(DWORD dwParam);
};
//------------------------------------------------------------------------
// ͼ�����
// Ŀǰ֧��bmp,tga�ļ��أ��˼�������һ����ʵ���Ķ���ͨ���˶�����ص�ͼƬ
// Ҫ���ñ������������ͷ�
class WNDSYS_API CImageLoader  
{
public:
	struct stCacheImage					// ����ͼƬ
	{
		IBitmap*	pImage;				// ����λƬ��IBitmap�ӿ�ָ��
		int			nRefCount;			// ����λͼ�����ü���
	};
private:
	typedef hash_map<string, stCacheImage>				CacheImageList;
	typedef hash_map<string, stCacheImage>::iterator	CacheImageListPtr;
	static CImageLoader* _instance;		// ȫ��ͼƬ������ʵ��
	ICanvasHelper		m_CanvasHelper;	// ����ͼƬ�ĸ�����
	CacheImageList		m_vImageList;	// ͼƬ�����б�
	IFileSystem*		m_pFS;			// �ļ�ϵͳָ��
public:
	CImageLoader();
	~CImageLoader();
	
	static CImageLoader*	GetInstance();
	static void				ReleaseInstance();
	
	void     Clear();
	IBitmap* LoadImage(const char* szFileName);
	IBitmap* CopyImage(IBitmap* pImage);
	void	 FreeImage(const char* szFileName);
	void	 FreeImage(IBitmap* pImage);
	BOOL	 SetFileSystem(IFileSystem* pFS);
	IFileSystem* GetFileSystem(){return m_pFS;}
	
private:
	IBitmap* _loadImage(const char* szFileName);
	BYTE * __DecodeJpeg(BYTE *pData, int nLen);     //����һ��JPEG����,����һ�λ�����
};
//------------------------------------------------------------------------
// ͼ�����
// Ŀǰ֧��bmp,tga�ļ��أ��˼�������һ����ʵ���Ķ���ͨ���˶�����ص�ͼƬ
// Ҫ���ñ������������ͷ�
// �������������ȫһ��(�е��̬)
class WNDSYS_API CImageLoaderEx
{
public:
	struct stCacheImage			// ����ͼƬ
	{
		IBitmap*	pImage;		// ����λƬ��IBitmap�ӿ�ָ��
		int			nRefCount;	// ����λͼ�����ü���
	};
	
	private:
		typedef hash_map<string, stCacheImage>				CacheImageList;
		typedef hash_map<string, stCacheImage>::iterator	CacheImageListPtr;
		static CImageLoaderEx* _instance;		// ȫ��ͼƬ������ʵ��
		ICanvasHelper		m_CanvasHelper;	// ����ͼƬ�ĸ�����
		CacheImageList		m_vImageList;	// ͼƬ�����б�
		IFileSystem*		m_pFS;			// �ļ�ϵͳָ��
	public:
		CImageLoaderEx();
		~CImageLoaderEx();
		
		static CImageLoaderEx*	GetInstance();
		static void				ReleaseInstance();
		
		void     Clear();
		IBitmap* LoadImage(const char* szFileName);
		IBitmap* CopyImage(IBitmap* pImage);
		void	 FreeImage(const char* szFileName);
		void	 FreeImage(IBitmap* pImage);
		BOOL	 SetFileSystem(IFileSystem* pFS);
		IFileSystem* GetFileSystem(){return m_pFS;}
		
	private:
		IBitmap* _loadImage(const char* szFileName);
};
//------------------------------------------------------------------------
// ���崴��������ʵ������
// ͨ���˶��󴴽������壬��ͨ���˶������Ӧ���������ͷ�
class WNDSYS_API XFontCreate
{
public:
	struct stFontInfo	// ������Ϣ
	{
		enum {max_fontname_len=16};
		char szName[max_fontname_len];	// ������
		int  nSize;						// ����ߴ�(�ַ�����ʵ�ߴ�)
		int  bBlod;						// �Ƿ����
		int  bItalic;					// �Ƿ�б��
		int  bUnderline;				// �Ƿ����»���
		int  bWithOutline;				// �Ƿ������
		int  nCacheCharSize;			// Ҫ�����������ַ���Ŀ(Ŀǰ�ڱȽ�ʱ����)
		
		stFontInfo()		{memset(this,0,sizeof(stFontInfo));}
		BOOL operator == (const stFontInfo& info) const;
	};
	struct stCacheFont			// ��������
	{
		stFontInfo	info;		// �����������Ϣ
		HFONT		hFont;		// ��������ľ��
		ITTFont*	pFont;		// ���������ITTFont�ӿ�ָ��
		int			nRefCount;	// ������������ü���
	};
	typedef vector<stCacheFont>				CacheFontList;
	typedef vector<stCacheFont>::iterator	CacheFontListPtr;

private:
	static XFontCreate* _instance;		// ȫ�����崴��ʵ��
	ITTFont*			m_pDefFont;		// ϵͳĬ������
	ICanvasHelper		m_CanvasHelper;	// ��������ĸ�����
	CacheFontList		m_vFontList;	// �������建���б�
	
public:
	XFontCreate();
	~XFontCreate();
	
	static XFontCreate* GetInstance();
	static void			ReleaseInstance();
	
	void     Clear();
	ITTFont* GetDefaultFont()					{return m_pDefFont;}
	ITTFont* CreateFont(const char* szFontName, int nFontSize, BOOL bBlod = FALSE, 
		BOOL bItalic = FALSE, BOOL bUnderLine = FALSE, 
		BOOL bWithOutline = FALSE, int nCacheCharSize = 128);
	void	ReleaseFont(const char* szFontName, int nFontSize, BOOL bBlod = FALSE, 
		BOOL bItalic = FALSE, BOOL bUnderLine = FALSE, 
		BOOL bWithOutline = FALSE, int nCacheCharSize = 128);
	void	ReleaseFont(ITTFont* pFont);
	XFontCreate::stFontInfo* XFontCreate::GetFontInfo(ITTFont* pFont);
private:
	BOOL _createFont(stCacheFont& cachefont);
};
//------------------------------------------------------------------------
// ���(ע������໹��Ҫ���죬������һ�Ѷѵ�����)
class WNDSYS_API XCursor : public ITimerSink
{
private:
	IBitmap*	m_pWndBitmap;		// λͼָ��(����Ǳ���)
	RECT		m_rcWnd;			// ���ڱ߿�
	int			m_nAlpha;			// Alphaֵ
	BOOL		m_bMask;			// �Ƿ�͸��
	BOOL		m_bAnimate;			// �Ƿ��Ƕ������
	int			m_nInterval;		// �������ʱ��
	int			m_nFrames;			// ֡��
	POINT		m_ptHot;			// �ȵ�
	int			m_nCurrentFrame;	// ��ǰ֡
	POINT		m_ptCursor;			// ��ǰ֡��Դλͼ�ϵ������

public:
	static IBitmap*		m_pAttachWnd;			// ���������ͼƬ��ָ��Щ��Ʒ֮��Ķ���
	static DWORD		m_dwUserData;			// ��������õĸ�������
	static BOOL			m_bUseOwnCursor;		// �Ƿ��Ի���
	
public:
	XCursor();
	virtual ~XCursor();
    virtual void Release();

	// ITimeSink
	virtual void OnTimer(DWORD dwEventID);
	virtual IBitmap* SetBitmap(IBitmap* pBmp){
		IBitmap* pBmpOld = m_pWndBitmap;
		m_pWndBitmap = pBmp;
		return pBmpOld;
	}
	virtual IBitmap* GetBitmap()			{return m_pWndBitmap;}
	virtual void SetAlpha(int nAlpha)		{m_nAlpha = nAlpha;}
	virtual void SetMask(BOOL bMask = TRUE)	{m_bMask = bMask;}
	virtual void SetWindowPos(RECT& rc){CopyRect(&m_rcWnd, &rc);}
	virtual void GetWindowRect(LPRECT lpRect) {CopyRect(lpRect, &m_rcWnd);}
	virtual void SetAnimate(BOOL bAnimate)	{m_bAnimate = bAnimate;}
	virtual void SetInterval(int nInterval) {m_nInterval = nInterval;}
	virtual void SetFrames(int nFrames)		{m_nFrames = nFrames;}
	virtual void SetHots(POINT& pt)			{m_ptHot = pt;}
	virtual POINT GetHots()					{return m_ptHot;}
	virtual void InvalidRect(LPRECT lpRect = NULL);
	virtual void GetMouseXY(POINT& pt);
	virtual void SetMouseXY(POINT& pt);
	virtual void SetAttach(IBitmap* pAttachWnd);
	virtual IBitmap* GetAttach() {return XCursor::m_pAttachWnd;}
	virtual void SetUserData(DWORD dwUserData) {XCursor::m_dwUserData=dwUserData;}
	virtual DWORD GetUserData() {return XCursor::m_dwUserData;}
	
	virtual void OnDraw(ICanvas* pCanvas);
	virtual DWORD OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual DWORD MoveTo(POINT pt);
	virtual void Show(BOOL bShow = TRUE);

	void SetCursorOwnDraw(BOOL bOwnDraw);
};
//------------------------------------------------------------------------
// ���ڻ��࣬���д��ڶ��̳���SlotHolder(��ӵ����)
class WNDSYS_API XWindow : public SlotHolder
{
public:
	typedef list<XWindow*>						_wnd_list;
	typedef	list<XWindow*>::iterator			_wnd_it;
	typedef list<XWindow*>::reverse_iterator	_wnd_rit;
	_wnd_list	m_lstWnds;
protected:
	string			m_strCaption;	// ���ڱ���
	IBitmap*		m_pWndBitmap;	// λͼָ��(����Ǳ���)
	RECT			m_rcWnd;		// ���ڱ߿�
	int				m_nAlpha;		// Alphaֵ
	int				m_nID;			// ���ڱ�ʶ�Լ���ID��(ʵ������Դ����������ţ���ID��)
	
	XWindow*		m_pParent;		// ������
	XWindow*		m_pFocusChild;	// ӵ�н�����Ӵ���

	ITTFont*		m_pSelfFont;	// ����˽������
	DWORD			m_dwWndTraits;	// ��������

	WPixel			m_cForeColor, m_cBackColor;	// ǰ���ͱ�����ɫ
	int				m_nAlign;		// ���뷽ʽ
	IResObject*		m_pResObj;		// ���ڽű�����
Signals:
	Signal0			s_clicked;		// �����ź�
	int             m_nToolTipIndex;// ToolTip����
public:
	XWindow();
	XWindow(XWindow* pParentWnd);
	virtual ~XWindow();

    virtual void Release();

	// ϵͳ�������
	XDesktop*  GetDesktop();
	ICanvas*   GetCanvas();
	ITimeAxis* GetTimeAxis();
	ITTFont*   GetSysFont();

	// �������
	ITTFont* SetFont(ITTFont* pNewFont);
	ITTFont* GetFont();

	XWindow* SetFocus();
	XWindow* GetFocus();
	BOOL     IsFocus();
	XWindow* GetGlobalFocus();	// ����ϵͳ��һ��ȫ�ֵĽ��㣬��ʵ�ʾ��ǻ�����ڵĽ���
	BOOL     IsGlobalFocus();

	XWindow* SetCapture();
	XWindow* GetCapture();
	BOOL     IsCapture();
	void     ReleaseCapture();

	XWindow* SetMouseMove();
	XWindow* GetMouseMove();
	BOOL	 IsMouseMove();

	XWindow* SetActive();
	XWindow* GetActive();
	BOOL	 IsActive();
	
	int      SetCursor(int nType = CT_ARROW);
	XCursor* GetCursor();

	void	WritePoint(POINT pt);
	POINT	ReadPoint();
	void	WriteRect(RECT& rc);
	RECT&	ReadRect();

	BOOL IsMainWindow() {return GetWindowType()==WT_MAINWND;}
	BOOL IsDesktop()	{return GetWindowType()==WT_DESKTOP;}
	BOOL IsControl()	{return GetWindowType()==WT_CONTROL;}

	virtual BOOL IsTransparent(POINT& pt);

	// ��������
	virtual void SetVisible(BOOL bVisible = TRUE);
	virtual void SetEnabled(BOOL bEnabled = TRUE);
	void SetMask(BOOL bMask = TRUE);
	void SetMovable(BOOL bMovable = FALSE);
	void SetModal(BOOL bModal = FALSE);
	void SetTopmost(BOOL bTopmost = FALSE);
	void SetWndTraits(DWORD dwTraits, BOOL bTraits);
	
	BOOL IsVisible()	{return m_dwWndTraits & XWT_VISIBLE;}
	BOOL IsEnabled()	{return m_dwWndTraits & XWT_ENABLED;}
	BOOL IsMask()		{return m_dwWndTraits & XWT_MASK;}
	BOOL IsMovable()	{return m_dwWndTraits & XWT_MOVABLE;}
	BOOL IsModal()		{return m_dwWndTraits & XWT_MODAL;}
	BOOL IsTopmost()	{return m_dwWndTraits & XWT_TOPMOST;}

	int  GetToolTipIndex(void) {return m_nToolTipIndex;}
	void  SetToolTipIndex(int nIndex) {m_nToolTipIndex = nIndex;}
	DWORD GetWndTraits(){return m_dwWndTraits;}

	// ���ڹ������
	virtual void     AddWindow(XWindow* pWnd);
	virtual void     RemoveWindow(XWindow* pWnd);
	virtual XWindow* FindWindowByID(int nID);
	virtual int      FindWindow(XWindow* pWnd);

	// ��ʼ�����
	IBitmap* SetBitmap(IBitmap* pBmp);
	IBitmap* GetBitmap();
	void     SetAlpha(int nAlpha);
	int      GetAlpha();
	void	SetBackColor(int r, int g, int b)	{m_cBackColor = WPixel(r,g,b);}
	void	SetBackColor(WPixel color)			{m_cBackColor = color;}
	WPixel	GetBackColor()						{return m_cBackColor;}
	void	SetForeColor(int r, int g, int b)	{m_cForeColor = WPixel(r,g,b);}
	void	SetForeColor(WPixel color)			{m_cForeColor = color;}
	WPixel	GetForeColor()						{return m_cForeColor;}
	void	SetAlign(int nAlign)				{m_nAlign = nAlign;}
	int		GetAlign()							{return m_nAlign;}

	// �������
	virtual	void SetWindowPos(RECT& rc);
	virtual void GetWindowRect(LPRECT lpRect);
	virtual void GetClientRect(LPRECT lpRect);
	virtual void MoveWindow(int dx, int dy);
	virtual int  GetWidth()						{return m_rcWnd.right - m_rcWnd.left;}
	virtual int  GetHeight()					{return m_rcWnd.bottom - m_rcWnd.top;}
	virtual void InvalidRect(LPRECT lpRect = NULL);

	// �ı����
	virtual void   SetWindowText(LPCSTR lpszString);
	virtual LPCSTR GetWindowText();

	// ������ʾ
	virtual void ToolTipSetInfo(IBitmap* pBitmap, LPCSTR szToolTip, WPixel cBackColor, WPixel cForeColor, int nInterval=0);
	virtual void ToolTipMoveTo(int x, int y);

	// ���ڷ���
	virtual void	 SetID(int nID);
	virtual int		 GetID();
	virtual XWindow* GetParent();
	virtual XWindow* SetParent(XWindow* pParent);
	virtual int		 GetWindowType() {return WT_WINDOW;}
	XMainWnd*		 GetMainWnd();

	// ���ټ�(��ע��Ӣ����ĸʱ���Դ�д��ĸ��Ϊ����)
	void RegisterAccelKey(UINT nKeyCode, UINT nShift, XWindow* pWnd);
	void UnRegisterAccelKey(UINT nKeyCode, UINT nShift);
	void UnRegisterAccelKey();

	virtual XWindow* GetChildWindowFromPoint(POINT pt, int nFlags);
	XWindow* GetWindowBelowCursor(POINT pt, int nFlags);

	virtual void SetResObject(IResObject* pObj)	{m_pResObj = pObj;}
	virtual IResObject* GetResObject()			{return m_pResObj;}
	virtual void LoadAllImage();
	virtual void ReleaseAllImage();

public:	// ��Ϣ���
	// �����Ϣ
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseWheel(UINT nButton, UINT nShift, short zDelta, POINT pt){return NOTHANDLE;}

	// ������Ϣ
	virtual DWORD OnKeyDown(UINT nKeyCode, UINT nShift)	{return NOTHANDLE;}
	virtual DWORD OnKeyUp(UINT nKeyCode, UINT nShift)	{return NOTHANDLE;}
	virtual DWORD OnChar(UINT nKeyAscii)				{return NOTHANDLE;}
	virtual void  OnAccelKey(UINT nKeyCode, UINT nShift){}

	// �Ƴ���Ϣ
	virtual void PostExitMessage();

	// ������Ϣ
	virtual void OnSetFocus()	{}
	virtual void OnKillFocus()	{}

	// �����Ϣ�������¼�
	virtual void OnMouseEnter()	;
	virtual void OnMouseExit()	{}

	// �ı��ı�
	virtual void OnTextChanged() {}

	// ���ڼ���
	virtual void OnActivate(BOOL bActive) {}

	// ��ͼ��Ϣ
	virtual void OnPaint(ICanvas* pCanvas);
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip);
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip);

	// ��Ϣ����
	virtual DWORD OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual DWORD DefMessage(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	virtual void DestroyWindow();
	
private:
	XWindow* SetFocusHelper();
};
//------------------------------------------------------------------------
// ��ݼ���
class WNDSYS_API XAccelTable  
{
public:
	struct stAccelKey
	{
		UINT		nKeyCode;
		UINT		nShift;
		XWindow*	pWnd;
		
		stAccelKey(UINT Key, UINT Shift, XWindow* Wnd)
			: nKeyCode(Key), nShift(Shift), pWnd(Wnd) {}
	};
	typedef	vector<stAccelKey>				AccelKeys;
	typedef	vector<stAccelKey>::iterator	AccelKeysPtr;
protected:
	AccelKeys	m_AccelKeys;
public:
	XAccelTable();
	virtual ~XAccelTable();
	
	void  AddAccelKey(UINT nKeyCode, UINT nShift, XWindow* pWnd);
	void  RemoveAccelKey(UINT nKeyCode, UINT nShift);
	void  RemoveAccelKey(XWindow* pWnd);
	DWORD HandleAccelKey(UINT nKeyCode, UINT nShift);
};
//------------------------------------------------------------------------
// ���棬����˵�д��ڵ�ֱ�ӻ��ӵĸ����ڣ�һ������ֻ��һ�����棬������ӵ��
// ���ڵ����ԣ������д��ڹ������Ĺ��ܣ����ӹ���������ϵͳ����Ϣ����ͷַ�
class WNDSYS_API XDesktop : public XWindow
{
public:
	struct stMsg
	{
		UINT	uMsgID;
		WPARAM	wParam;
		LPARAM	lParam;
	};
	typedef list<XWindow*>				_wnd_list;
	typedef	list<XWindow*>::iterator	_wnd_it;
	typedef list<stMsg>					_msg_list;
	typedef	list<stMsg>::iterator		_msg_it;
private:
	HWND			m_hWnd;				// ���ھ��
	ICanvas*		m_pCanvas;			// ϵͳ����
	ITimeAxis*		m_pTimeAxis;		// ϵͳʱ����
	ITTFont*		m_pSysFont;			// ϵͳ����
	SIZE			m_sizeDesktop;		// �����С

	// ������
	XCursor**		m_ppCursors;		// ���еĹ������
	int				m_nCursorCount;		// ���й�����Ŀ
	int				m_nCurrentCursorType;// ��ǰʹ�õĹ������

	POINT			m_ptBak;			// ���ڱ���ĳЩ��
	RECT			m_rcBak;			// ���ڱ���ĳЩ����

	// �������
	XWindow*		m_pActiveWnd;		// ��ǰ��Ĵ���(����֧��������)
	XWindow*		m_pFocusWnd;		// ȫ�ֽ��㴰��
	XWindow*		m_pCaptureWnd;		// ��׽����
	XWindow*		m_pMouseMoveWnd;	// ����ƶ�����

	// ϵͳ��Դ
	IBitmap*		m_pSysScrollBarBitmap;	// ϵͳ������ͼƬ
	XWindow*		m_pToolTip;				// ���õ�ToolTip����

	_wnd_list		m_lstTopmostWnds;		// topmost �����б�

	XAccelTable		m_AccelTable;		// ���ټ���
	_msg_list		m_lstMsg;			// ��Ϣ����

	HKL				m_hKeyboardLayoutList[32];		// ���̲����б�(���뷨�б�)
	int				m_nKeyboardLayoutListCount;		// ���̲�����Ŀ(���뷨��Ŀ)
	int				m_nEnImeIndex;					// Ӣ�����뷨����
	int				m_nCurImeIndex;					// ��ǰ���뷨����
	BOOL			m_bRecordLayoutChange;			// �Ƿ��¼���뷨�ĸı�

	DWORD			m_dwMouseKeyState;				// ��ꡢ�������״̬

Signals:
	Signal1			s_mouseEnter;	// �������ź�
	Signal1			s_mouseExit;	// ����˳��ź�
	Signal1			s_imeChanged;
public:
	static XDesktop*	m_pDesktop;		// ��Ϸ����
public:
	XDesktop();
	virtual ~XDesktop();
	virtual int GetWindowType()				{return WT_DESKTOP;}
	virtual void Release();	

	void SetHWnd(HWND hWnd)					{m_hWnd = hWnd;}
	void SetCanvas(ICanvas* pCanvas)		{m_pCanvas = pCanvas;}
	void SetTimeAxis(ITimeAxis* pTimeAxis)	{m_pTimeAxis = pTimeAxis;}
	void SetSysFont(ITTFont* pSysFont)		{m_pSysFont = pSysFont;}
	void SetDesktopSize(SIZE size)			{m_sizeDesktop = size;}
	HWND GetHWnd()							{return m_hWnd;}
	ICanvas* GetCanvas()					{return m_pCanvas;}
	ITimeAxis* GetTimeAxis()				{return m_pTimeAxis;}
	ITTFont* GetSysFont()					{return m_pSysFont;}
	SIZE GetDesktopSize()					{return m_sizeDesktop;}
	static XDesktop* GetInstance();

	XWindow* SetActiveWindow(XWindow* pNewWnd);
	XWindow* SetFocusWindow(XWindow* pNewWnd);
	XWindow* SetCaptureWindow(XWindow* pNewWnd);
	XWindow* SetMouseMoveWindow(XWindow* pNewWnd);
	XWindow* GetActiveWindow()				{return m_pActiveWnd;}
	XWindow* GetFocusWindow()				{return m_pFocusWnd;}
	XWindow* GetCaptureWindow()				{return m_pCaptureWnd;}
	XWindow* GetMouseMoveWindow()			{return m_pMouseMoveWnd;}

	int SetCurrentCursorType(int nType);
	int	GetCurrentCursorType()				{return m_nCurrentCursorType;}
	XCursor* GetCursor(int nType);
	XCursor* GetCurrentCursor()				{return GetCursor(GetCurrentCursorType());}
	int  GetCursorCount()					{return m_nCursorCount;}
	void SetAllCursor(XCursor** ppCursors)	{m_ppCursors = ppCursors;}
	void SetCursorCount(int nCount)			{m_nCursorCount = nCount;}
	BOOL IsCursorOwnDraw()					{return XCursor::m_bUseOwnCursor;}
	void SetCursorOwnDraw(BOOL bOwnDraw);
	DWORD GetMouseKeyState()				{return m_dwMouseKeyState;}

	void SetBakPoint(POINT pt)				{m_ptBak = pt;}
	POINT GetBakPoint()						{return m_ptBak;}
	void SetBakRect(RECT& rc)				{CopyRect(&m_rcBak, &rc);}
	RECT& GetBakRect()						{return m_rcBak;}

	IBitmap* SetSysScrollBarBitmap(IBitmap* pNewBitmap);
	IBitmap* GetSysScrollBarBitmap()		{return m_pSysScrollBarBitmap;}
	XWindow* SetSysToolTip(XWindow* pNewWnd);
	XWindow* GetSysToolTip()				{return m_pToolTip;}
	XAccelTable* GetAccelTable()			{return &m_AccelTable;}

	DWORD DispatchMessage(UINT message, WPARAM wParam, LPARAM lParam);
	DWORD DispatchMouseMessage(UINT message, WPARAM wParam, LPARAM lParam);
	DWORD DispatchKeyMessage(UINT message, WPARAM wParam, LPARAM lParam);
	DWORD DispatchOtherMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void  PostMessage(UINT uMsgID, WPARAM wParam, LPARAM lParam);
	void  SetIMEEnable(BOOL bEnable = TRUE);

// ����
public:
	// �����Ϣ
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt)	{return NOTHANDLE;}
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt)	{return NOTHANDLE;}
	virtual DWORD OnMouseWheel(UINT nButton, UINT nShift, short zDelta, POINT pt)	{return NOTHANDLE;}

	virtual void OnPaint(ICanvas* pCanvas);
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip);

	// �����ڹ���
	virtual void	 AddWindow(XWindow* pWnd);
	virtual void	 RemoveWindow(XWindow* pWnd);
	virtual XWindow* FindWindowByID(int nID);
	virtual int      FindWindow(XWindow* pWnd);

	virtual XWindow* GetChildWindowFromPoint(POINT pt, int nFlags);
	virtual XWindow* GetFirstVisibleEnableWindow();
	
protected:
	virtual void DestroyWindow();
};
//------------------------------------------------------------------------
// �����ڣ������û���д���Զ��������ھ��̳��Ա���
class WNDSYS_API XMainWnd : public XWindow  
{
public:
	XMainWnd();
	XMainWnd(XWindow* pParentWnd);
	virtual ~XMainWnd();
	
	virtual int GetWindowType()		{return WT_MAINWND;}
	
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);
	
	// ������Ϣ
	virtual DWORD OnKeyDown(UINT nKeyCode, UINT nShift);
	virtual DWORD OnKeyUp(UINT nKeyCode, UINT nShift);
	virtual DWORD OnChar(UINT nKeyAscii);
	
	virtual void SetVisible(BOOL bVisible = TRUE);
	virtual void SetEnabled(BOOL bEnabled = TRUE);
	virtual BOOL CanEscape();
	virtual void OnEscape();
public:	// ��Ϣ���
	// ���ڼ���
	virtual void OnActivate(BOOL bActive);
	virtual void BringToTopLevel();	
protected:
	virtual void DestroyWindow();
};
//------------------------------------------------------------------------
// �����ڣ������û���д���Զ��������ھ��̳��Ա���
class WNDSYS_API XGeneralMainWnd : public XMainWnd  
{
public:
	enum {LeftTop=0,Top,RightTop,Left,Right,
		LeftBottom,Bottom,RightBottom,
		SrcBack,Fill,
		CaptionPos,
		MaxAreaCount};
protected:
	struct AREA
	{
		long x,y,w,h;
	};
	AREA m_Pos[MaxAreaCount];
	/*
	λ����Ϣ����˳�����ϡ��ϣ����ϡ����ң����¡��£�
	���£���䱳����Դͼ����Ҫ��������
	//*/
	int				m_nBorderAlpha;				 //���ڱ߿��Alphaֵ
	int             m_caption_x, m_caption_y;    //������ʾ�����λ��
	void CalculateCaptionPos(void);              //����������������λ��
	
public:
	XGeneralMainWnd();
	XGeneralMainWnd(XWindow* pParentWnd);
	virtual ~XGeneralMainWnd();

	virtual void SetBorderAlpha(int alpha);			//���ñ߿��Alphaֵ
	virtual int GetBorderAlpha();					//��ȡ�߿��Alphaֵ
	virtual const AREA *GetGraphicsPos();           //��ȡԭͼƬ�ĸ���ͼλ��

	virtual BOOL InitParamList(long* posList, int nCount);
	virtual BOOL IsTransparent(POINT& pt);
	virtual	void SetWindowPos(RECT& rc);
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip);
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip);
	
	virtual void SetWindowText(LPCSTR lpszString); 
};
//------------------------------------------------------------------------
// �ؼ��࣬�ⲿ�Զ���ķǴ���ϵͳ�����еĿؼ���Ҫ�̳д���
class WNDSYS_API XControl : public XWindow  
{
public:
	XControl();
	XControl(XWindow* pParentWnd);
	virtual ~XControl();
	
	virtual int GetWindowType() {return WT_CONTROL;}

	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);
	
	// ������Ϣ
	virtual DWORD OnKeyDown(UINT nKeyCode, UINT nShift);
	virtual DWORD OnKeyUp(UINT nKeyCode, UINT nShift);
	virtual DWORD OnChar(UINT nKeyAscii);
};
//------------------------------------------------------------------------
// ��Ϣ��ʾ����
class WNDSYS_API XToolTip : public XWindow, public ITimerSink
{
protected:
	int		m_nMargin;		// �߿�հ�
	int		m_nLineSpace;	// �м��
	int		m_nInterval;	// ��ʾ��ʱ�䳤��
public:
	XToolTip();
	XToolTip(XWindow* pParentWnd);
	virtual ~XToolTip();
	virtual int GetWindowType()					{return WT_TOOLTIP;}
	
	virtual void OnTimer(DWORD dwEventID);
	
	void SetText(LPCSTR szTooltip)			{XWindow::SetWindowText(szTooltip);}
	void SetLineSpace(int nSpace)			{m_nLineSpace = max(0, nSpace);}
	int  GetLineSpace()						{return m_nLineSpace;}
	void SetInterval(int nInterval)			{m_nInterval = nInterval;}
	int  GetInterval()						{return m_nInterval;}
	
	void CalcPos(LPPOINT lpptPos);
	
	void SetInfo(IBitmap* pBitmap, LPCSTR szToolTip, WPixel cBackColor, WPixel cForeColor, int nInterval);
	void Show(LPPOINT lpptPos);
	void SetNewPos(int x, int y);
	void Hide();
	
	BOOL IsValid()							{return m_pWndBitmap || !m_strCaption.empty();}
	
	virtual BOOL IsTransparent(POINT& pt)	{return FALSE;}
public:
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip);
private:
	void LimitedToScreen(LPRECT lpRect); 
	
	//  ����������ڽ���µĸ�ʽ���� 
	//  �����ַ����к�����[:255,033,056,r]�ĸ�ʽ (��Ҫ�ַ��䲻�����ո�, ÿ����ɫֵҪ��3λ��)
	//  ����: �ַ���, WPIXELָ��, ��������ָ��(int *)
	//  ����: ��ʽ���Ĵ�С
	int __PhraseNewFormat(LPCSTR szStr, WPixel *pColor, int *pAlign);

	//  ������������ж��Ƿ���Ҫ����
	//  ����: �ַ���
	//  ����: �Ƿ���������
	BOOL __isNewLine(LPCSTR szStr);
};
//------------------------------------------------------------------------
// ��ť��
class WNDSYS_API XButton : public XWindow  
{
protected:
	IBitmap*	m_pBmpDown;		// ��ť����ʱ��ͼƬ
	IBitmap*	m_pBmpUp;		// ��ť�ͷ�ʱ��ͼƬ
	IBitmap*	m_pBmpHover;	// �����ͣʱ��ͼƬ
	IBitmap*	m_pBmpDisable;	// ��ť��Чʱ��ͼƬ
	int			m_nState;		// ��ť��ǰ��״̬
	
	WPixel      m_cForeColorHover;   //�����ͣʱ������ɫ
	WPixel      m_cForeColorDown;   //��갴��ʱ������ɫ
	WPixel      m_cForeColorDisable;   //��갴��ʱ������ɫ
	WPixel      m_cForeColorUp;   //��굯��ʱ������ɫ

public:
	XButton();
	XButton(XWindow* pParentWnd);
	virtual ~XButton();
	virtual int GetWindowType()						{return WT_BUTTON;}
	
	enum STATE{BS_UP=0, BS_ROLLOVER, BS_DOWN};
	
public:
	
	void SetBitmapDown(IBitmap* pBmp)	{m_pBmpDown = pBmp;}
	IBitmap* GetBitmapDown();
	void SetBitmapUp(IBitmap* pBmp)		{m_pBmpUp = pBmp;m_pWndBitmap=pBmp;}
	IBitmap* GetBitmapUp()				{return m_pBmpUp;}
	void SetBitmapHover(IBitmap* pBmp)	{m_pBmpHover = pBmp;}
	IBitmap* GetBitmapHover();
	void SetBitmapDisable(IBitmap* pBmp){m_pBmpDisable = pBmp;}
	IBitmap* GetBitmapDisable();
	void SetState(int bsState)			{m_nState = bsState;}
	int  GetState()						{return m_nState;}
	
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);

	//���ø�������µı���ɫ
	void SetDisableForeColor(long *color);
	void SetHoverForeColor(long *color);
	void SetDownForeColor(long *color);
	void SetUpForeColor(long *color);  

	void OnDraw(ICanvas* pCanvas, RECT& rcClip);
	
	virtual void OnMouseEnter();
	virtual void OnMouseExit();
	
	virtual void SetEnabled(BOOL bEnabled = TRUE);
	virtual void LoadAllImage();
	virtual void ReleaseAllImage();
protected:
	virtual void DestroyWindow();
};
//------------------------------------------------------------------------
// ״̬��ť��
class WNDSYS_API XStateButton : public XWindow
{
protected:
	IBitmap*	m_pBmpState1;	// ״̬1��Ӧ��ͼƬ
	IBitmap*	m_pBmpState2;	// ״̬2��Ӧ��ͼƬ
	int			m_nState;		// ��ǰ״̬
	bool		m_bShowState1;	// �Ƿ�������ʾ״̬1
public:
	XStateButton();
	XStateButton(XWindow* pParentWnd);
	virtual ~XStateButton();
	virtual int GetWindowType()						{return WT_STATEBUTTON;}
	
	enum STATE{BS_NORMAL=0, BS_ROLLOVER, BS_PRESSED};
	
public:
	virtual void SetBitmapState2(IBitmap* pBmp)	{m_pBmpState2=pBmp;}
	virtual void SetBitmapState1(IBitmap* pBmp)	{m_pBmpState1=pBmp;m_pWndBitmap=pBmp;}
	virtual void SetState(int nState);
	virtual int GetState()						{return (m_bShowState1)?0:1;}

	virtual void LoadAllImage();
	virtual void ReleaseAllImage();
	
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	
	virtual void OnMouseEnter();
	virtual void OnMouseExit();
	
protected:
	virtual void DestroyWindow();
};
//------------------------------------------------------------------------
// ��ǩ�࣬Ҳ�о�̬�ı���Ŀǰ����֧�ֵ���
class WNDSYS_API XLabel : public XWindow
{
	int			m_nMargin;	// �߿�հ�
public:
	XLabel();
	XLabel(XWindow* pParentWnd);
	virtual ~XLabel();
	virtual int GetWindowType()					{return WT_LABEL;}
	
	void SetMargin(int nMargin)				{m_nMargin = nMargin;}
	int  GetMargin()						{return m_nMargin;}
	
	// �麯������
	virtual BOOL IsTransparent(POINT& pt)	{return !IsEnabled();}
	
public:
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip);
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip);	
};
//------------------------------------------------------------------------
// ��ѡť
class WNDSYS_API XRadio : public XWindow
{
protected:
	int				m_nState;
	IBitmap*		m_pBmp;
	XRadioGroup*	m_pGroup;
	
Signals:
	Signal0			s_selected;
public:
	XRadio();
	XRadio(XWindow* pParentWnd);
	virtual ~XRadio();
	virtual int GetWindowType()					{return WT_RADIO;}
	
	virtual void Release();
	
	enum STATE{RS_UNCHECK=0, RS_CHECK};
	
public:
	void SetState(int cbsState = TRUE)		{m_nState = cbsState;}
	int  GetState()							{return m_nState;}
	void SetStateBitmap(IBitmap* pBmp)		{m_pBmp = pBmp;}
	void SetGroup(XRadioGroup* pGroup)		{m_pGroup = pGroup;}
	XRadioGroup* GetGroup()					{return m_pGroup;}

	virtual BOOL IsTransparent(POINT& pt)	{return FALSE;}

	virtual void LoadAllImage();
	virtual void ReleaseAllImage();
public:
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual void OnMouseEnter();
	virtual void OnMouseExit();
	
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip);
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip);
	
protected:
	virtual void DestroyWindow();
};
//------------------------------------------------------------------------
// ��ѡťȺ������
class WNDSYS_API XRadioGroup
{
Signals:
	Signal1		s_indexChecked;

public:
	XRadioGroup();
	virtual ~XRadioGroup();
	
	list<XRadio*>	m_lstRadios;
	
	XRadio* GetSelectedRadio();
	void	SelectRadio(XRadio* pRadio);
	int		GetIndexOfRadio(XRadio* pRadio);
	int		AddRadio(XRadio* pRadio);
	void	RemoveRadio(XRadio* pRadio);
	void	Release();
};
//------------------------------------------------------------------------
// ��ѡ��
class WNDSYS_API XCheckBox : public XWindow  
{
protected:
	int			m_nState;
	IBitmap*	m_pBmp;
	BOOL		m_bUseGray;
	
Signals:
	Signal1		s_checked;
public:
	XCheckBox();
	XCheckBox(XWindow* pParentWnd);
	virtual ~XCheckBox();
	virtual int GetWindowType()							{return WT_CHECKBOX;}
	
	enum STATE{CBS_UNCHECK=0, CBS_CHECK};
	
	void SetState(int cbsState = TRUE)				{m_nState = cbsState;}
	int  GetState()									{return m_nState;}
	void SetStateBitmap(IBitmap* pBmp)				{m_pBmp = pBmp;}

	virtual BOOL IsTransparent(POINT& pt)			{return FALSE;}

	virtual void LoadAllImage();
	virtual void ReleaseAllImage();
public:
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual void  OnMouseEnter();
	virtual void  OnMouseExit();
	
	virtual void  OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip);
	virtual void  OnDraw(ICanvas* pCanvas, RECT& rcClip);
	
protected:
	virtual void  DestroyWindow();
};
//------------------------------------------------------------------------
// �༭��
class WNDSYS_API XEdit : public XWindow, public ITimerSink
{
public:

	enum {LeftTop=0,Top,RightTop,Left,Right,
		LeftBottom,Bottom,RightBottom,
		SrcBack,Fill,
		MaxAreaCount};

	struct Selection		// ѡ��ṹ
    {
        int startIndex;
        int endIndex;
    };
	
protected:
	BOOL		m_bPassword;		// �Ƿ��������
	BOOL		m_bReadOnly;		// �Ƿ�ֻ��
	BOOL		m_bDigitOnly;		// ��������
	BOOL		m_bDrawBeam;		// �Ƿ񻭲����
	BOOL		m_bMultiLine;		// �Ƿ�֧�ֶ���
	BOOL		m_bAutoWrap;		// �Ƿ��Զ�����
	int			m_nCaretPos;		// ��������ַ����е�λ��
	POINT		m_ptCaretPos;		// �����������λ��(ƫ��ֵ)
	int			m_nMaxLen;			// �������ַ�����󳤶�
	Selection	m_sSel;				// ѡ����ı�
	int			m_nSelStart;		// ѡ��ʱ�Ŀ�ʼλ��
	POINT		m_ptOffset;			// EDIT���������EDIT����ƫ��
	char		m_cPasswordChar;	// ��������������ַ�
	int			m_nMargin;			// ��Ե���
	int			m_nLineSpace;		// �м��
	int			m_nValidWidth;		// ��Ч���
	int			m_nValidHeight;		// ��Ч�߶�
	XScrollBar*	m_pHScrollBar;		// ˮƽ������
	XScrollBar*	m_pVScrollBar;		// ��ֱ������

	BOOL m_bStretch;                 //�Ƿ�������ͼ   
	
	struct AREA
	{
		long x,y,w,h;
	};
	AREA m_Pos[MaxAreaCount];
	/*
	λ����Ϣ����˳�����ϡ��ϣ����ϡ����ң����¡��£�
	���£���䱳����Դͼ����Ҫ��������
	*/

Signals:
    Signal0		s_returnPressed;
    Signal1		s_textChanged;
public:
	
	XEdit();
	XEdit(XWindow* pParentWnd);
	virtual ~XEdit();
	virtual int GetWindowType()					{return WT_EDIT;}

	virtual BOOL InitParamList(long* posList, int nCount);
public:
	// �������
	const AREA * GetGraphicsPos();
	void SetPassword(BOOL bPassword = TRUE) {m_bPassword = bPassword;}
	BOOL IsPassword()						{return m_bPassword;}
	void SetReadOnly(BOOL bReadOnly = TRUE) {m_bReadOnly = bReadOnly;}
	BOOL IsReadOnly()						{return m_bReadOnly;}
	void SetDigitOnly(BOOL bDigitOnly = TRUE) {m_bDigitOnly = bDigitOnly;}
	BOOL IsDigitOnly()						{return m_bDigitOnly;}
	void SetDrawBeam(BOOL bDrawBeam = TRUE) {m_bDrawBeam = bDrawBeam;}
	BOOL IsDrawBeam()						{return m_bDrawBeam;}
	void SetMultiLine(BOOL bMultiLine = TRUE) {m_bMultiLine = bMultiLine;}
	BOOL IsMultiLine()						{return m_bMultiLine;}
	void SetAutoWrap(BOOL bAutoWrap = TRUE);
	BOOL IsAutoWrap()						{return m_bAutoWrap;}
	void SetCaretPos(int nPos);
	int  GetCaretPos()						{return m_nCaretPos;}
	int  SetMaxLen(int nMaxLen)				{return m_nMaxLen = nMaxLen;}
	int  GetMaxLen()						{return m_nMaxLen;}
	void SetMargin(int nMargin)				{m_nMargin = nMargin;}
	int  GetMargin()						{return m_nMargin;}
	void SetPasswordChar(char c)			{m_cPasswordChar = c;}
	char GetPasswordChar()					{return m_cPasswordChar;}
	void SetHScrollBar(XScrollBar* pBar);
	void SetVScrollBar(XScrollBar* pBar);

	Selection GetSelection()				{return m_sSel;}
	void SetSelection(int nStart, int nEnd);
	void ClearSelection();
	void DeleteSelectedText();
	BOOL IsValidSelection();

	virtual BOOL IsTransparent(POINT& pt)	{return !IsEnabled();}

public: // ����
	void doSelAll();
	void doCopy();
	void doPaste();
	void doCut();
	void doBackSpace();
	void doDelete();
	void doCrLf();	// �س�����
	
	void AdjustHScroll();
	void AdjustVScroll();
	
public:
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseWheel(UINT nButton, UINT nShift, short zDelta, POINT pt);
	virtual void  OnMouseEnter();
	virtual void  OnMouseExit();
	virtual void  OnSetFocus();
	virtual void  OnKillFocus();

	virtual DWORD OnKeyDown(UINT nKeyCode, UINT nShift);
	virtual DWORD OnChar(UINT nKeyAscii);

	virtual void SetWindowText(LPCSTR lpszString);
	virtual void SetWindowPos(RECT& rc);
	
	virtual void OnTimer(DWORD dwEventID);
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip);
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip);
	virtual void OnOtherChar(UINT nKeyAscii);

	int  GetValidWidth()	{return m_nValidWidth;}
	void SetValidWidth(int nValidWidth);
	int  GetValidHeight() {return m_nValidHeight;}
	void SetValidHeight(int nValidHeight);

protected:
	int GetCharIndexByOffset(LPCSTR lpStr, POINT pt, BOOL bCmpHalfChar=FALSE, BOOL bCalcCaretPos=FALSE);
	int _GetCharIndexByOffset(char* pFirst, char* pLast, char* pBegin, char* pEnd, POINT& pt, BOOL bCmpHalfChar, BOOL bCalcCaretPos);
	void _CalcOffsetByCharIndex(LPCSTR lpStr, int nIndex, POINT& pt);
	void CalcCaretPos();
	void GetCaretDisc(POINT& pt);

	virtual int GetCharBytes(int nPos);
	void OnLeftArrow(UINT nShift);
	void OnRightArrow(UINT nShift);
	void OnUpArrow(UINT nShift);
	void OnDownArrow(UINT nShift);

protected:
	virtual void DestroyWindow();
	int GetLastSpaceBefore(int pos);
	int GetNextWordStartPosAfter(int pos);
	string stringUpToCaret();
	string stringFromCaret();
	string GetDisplayString(string& str);
	int GetDisplayStringWidth(string& str);
	void DrawString(ICanvas* pCanvas, RECT& rcClip, string& strText, WPixel& clr, POINT& ptCurPos);
	void DrawSelectedString(ICanvas* pCanvas, RECT& rcClip, string& strText, WPixel& clr, POINT& ptCurPos);
	void DrawUnSelectText(ICanvas* pCanvas, RECT& rcClip, POINT& ptCurPos, string& strText);
	void DrawSelectText(ICanvas* pCanvas, RECT& rcClip, POINT& ptCurPos, string& strText);
	void OnHScrollPosChanged(DWORD dwParam);
	void OnVScrollPosChanged(DWORD dwParam);
	void ReDrawCaret();
	void CalcValidSize();
private:
	void CalcValidSize_MultiLine_AutoWrap();
	void CalcValidSize_MultiLine_NoAutoWrap();
	void CalcValidSize_NoMultiLine();
};
//------------------------------------------------------------------------
// �������ؼ�
class WNDSYS_API XSlideBar : public XWindow  
{
protected:
	IBitmap*	m_pBmp;
	int			m_nStyle;
	int			m_nMin;
	int			m_nMax;
	int			m_nPos;
	RECT		m_rcBlock;
	
Signals:
	Signal1		s_posChanged;
public:
	XSlideBar();
	XSlideBar(XWindow* pParentWnd);
	virtual ~XSlideBar();
	virtual int GetWindowType()						{return WT_SLIDEBAR;}
	
	enum STYLE{S_HORZ=0, S_VERT};
	
	virtual void SetBlockBitmap(IBitmap* pBmp)	{m_pBmp = pBmp;}
	virtual void SetStyle(int nStyle)			{m_nStyle = nStyle;}
	virtual int  GetStyle()						{return m_nStyle;}
	virtual void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);
	virtual void GetRange(int& nMin, int& nMax) {nMin = m_nMin;nMax = m_nMax;}
	virtual void SetBlockRect(LPRECT lpRect)	{CopyRect(&m_rcBlock, lpRect);}
	virtual void SetPos(int nPos);
	virtual int GetPos()						{return m_nPos;}

	virtual void LoadAllImage();
	virtual void ReleaseAllImage();
public:
	virtual BOOL IsTransparent(POINT& pt)		{return FALSE;}
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);
	virtual void OnMouseEnter();
	virtual void OnMouseExit();
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip);
	
protected:
	virtual void DestroyWindow();
};
//------------------------------------------------------------------------
// �������ؼ�
class WNDSYS_API XScrollBar : public XWindow, public ITimerSink
{
protected:
	int			m_nStyle;			// ����������(ˮƽ����ֱ��)
	int			m_nMin;				// ������Χ����Сֵ
	int			m_nMax;				// ������Χ�����ֵ
	int			m_nPos;				// ��ǰ����λ��
	SIZE		m_Size;				// ������ť�ĳߴ��С
	RECT		m_rcBlock;			// ����ľ�������
	RECT		m_rcLeftTop;		// ����ϰ�ť�ľ�������
	RECT		m_rcRightBottom;	// �һ��°�ť�ľ�������
	int			m_nPressFlags;		// ���������±�־
	XWindow*	m_pAttach;			// �ù����������Ĵ���
	int			m_nVisibleCount;	// �ɼ��������Ŀ��,���ڼ��㻬����ĳ���
	int			m_nTimerTick;		// ����ʱ�Ӽ���
	int			m_nLineDelta;		// ������(����)��ťʱ��λ�øı���٣�Ĭ��Ϊ1
	POINT		m_ptPageScrollPos;	// ҳ�������սᴦ���λ��
	
Signals:
	Signal1		s_posChanged;		// ������λ�øı�
public:
	XScrollBar();
	XScrollBar(XWindow* pParentWnd);
	virtual ~XScrollBar();
	virtual int GetWindowType()		{return WT_SCROLLBAR;}
	
	// ����Ч���ı�����ռ���ص���(��)��
	enum {DELTA_3D=7};
	enum STYLE{S_HORZ=0, S_VERT};
	enum PRESSFLAGS{PF_NONE=0,PF_LEFTTOP,PF_BLOCK,PF_RIGHTBOTTOM,PF_LEFTTOPPAGE,PF_RIGHTBOTTOMPAGE};
	enum ScrollBarBitmapPos// ����������ͼƬԪ���ڱ�׼������λͼ�е�λ������
	{
		LeftUp=0, LeftDown, LeftDisable,
		RightUp, RightDown, RightDisable,
		TopUp, TopDown, TopDisable,
		BottomUp, BottomDown, BottomDisable,
		BlockHorzUp, BlockHorzDown, 
		BlockVertUp, BlockVertDown, 
		ScrollBarBackGroundHorz, ScrollBarBackGroundVert,
	};
	
public:
	void SetStyle(int nStyle)					{m_nStyle = nStyle;}
	int  GetStyle()								{return m_nStyle;}
	void SetScrollRange(int nMin, int nMax, BOOL bRedraw = FALSE);
	void GetScrollRange(int& nMin, int& nMax)	{nMin = m_nMin;nMax = m_nMax;}
	int  GetMaxRange()							{return m_nMax;}
	void SetScrollPos(int nPos, BOOL bRedraw = TRUE);
	int  GetScrollPos()							{return m_nPos;}
	void SetVisibleCount(int nCount)			{m_nVisibleCount = nCount;}
	int  GetVisibleCount()						{return m_nVisibleCount;}
	void SetSize(SIZE& size);
	void GetSize(SIZE& size)					{size.cx = m_Size.cx;size.cy = m_Size.cy;}
	void SetLineDelta(int nDelta)				{m_nLineDelta = nDelta;}
	virtual void LineLeftUp();
	virtual void LineRightDown();
	virtual void PageLeftUp();
	virtual void PageRightDown();
	
	XWindow* Attach(XWindow* pWnd);
	void Detach()								{m_pAttach = NULL;}
	void Refresh();

	virtual BOOL IsTransparent(POINT& pt)		{return FALSE;}
public:
	virtual void  OnTimer(DWORD dwEventID);
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip);
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip);
	virtual void MoveWindow(int dx, int dy);
	
public:
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseWheel(UINT nButton, UINT nShift, short zDelta, POINT pt);

	virtual DWORD OnKeyDown(UINT nKeyCode, UINT nShift);
	
	virtual void OnMouseEnter();
	virtual void OnMouseExit();
	
	virtual void OnPaint(ICanvas* pCanvas);
protected:
	virtual void DestroyWindow();
	virtual void DrawLeftTopButton(ICanvas* pCanvas, int nScrollBarStyle, RECT& rcLeftTop, RECT& rcClip, DWORD dwBlitStyle);
	virtual void DrawRightBottomButton(ICanvas* pCanvas, int nScrollBarStyle, RECT& rcRightBottom, RECT& rcClip, DWORD dwBlitStyle);
	virtual void DrawTrackBar(ICanvas* pCanvas, int nScrollBarStyle, RECT& rcTraceBar, RECT& rcClip, DWORD dwBlitStyle);
	virtual void DrawSlideBlock(ICanvas* pCanvas, int nScrollBarStyle, RECT& rcSlideBlock, RECT& rcClip, DWORD dwBlitStyle);
	virtual void CheckPageScroll();
};
//------------------------------------------------------------------------
// �������ؼ�
class WNDSYS_API XProgressBar : public XWindow  
{
protected:
	int			m_nStyle;	// ˮƽ����ֱ��
	int			m_nMin;		// ��Сֵ
	int			m_nMax;		// ���ֵ
	int			m_nPos;		// ��ǰλ��
public:
	XProgressBar();
	XProgressBar(XWindow* pParentWnd);
	virtual ~XProgressBar();
	virtual int GetWindowType()					{return WT_PROGRESSBAR;}
	
	enum STYLE{S_HORZ=0, S_VERT};
	
	void SetStyle(int nStyle)				{m_nStyle = nStyle;}
	int  GetStyle()							{return m_nStyle;}
	void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);
	void GetRange(int& nMin, int& nMax)		{nMin = m_nMin;nMax = m_nMax;}
	
	void SetPos(int nPos);
	int  GetPos()							{return m_nPos;}
	virtual BOOL IsTransparent(POINT& pt)	{return FALSE;}
	
public:
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip);
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip);
};
//------------------------------------------------------------------------
// ��̬ͼƬ�ؼ�
class WNDSYS_API XStaticBitmap : public XWindow
{
protected:
	BOOL  m_bStretch;                       //�Ƿ�֧������
public:
	XStaticBitmap();
	XStaticBitmap(XWindow* pParentWnd);
	virtual ~XStaticBitmap();
	virtual int GetWindowType()					{return WT_STATICBITMAP;}
	virtual void SetStretch(BOOL flag);
	virtual BOOL IsTransparent(POINT& pt)		{return !IsEnabled();}
	
public:
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip);
};
//------------------------------------------------------------------------
// �����ؼ�
class WNDSYS_API XAnimate : public XWindow, public ITimerSink
{
protected:
	enum {statePlay=0,statePause,};
	int		m_nFrameCount;	// ֡��
	int		m_nCurFrame;	// ��ǰ֡
	int		m_nDelay;		// ������ʱ
	int		m_nOffX;		// ��ǰ֡��xƫ��
	int		m_nFrameWidth;	// ��֡���
	int		m_nState;		// ��ǰ����״̬
public:
	XAnimate();
	XAnimate(XWindow* pParentWnd);
	virtual ~XAnimate();
	
	virtual int GetWindowType()							{return WT_ANIMATE;}
	
	virtual void SetFrameCount(int nCount)			{m_nFrameCount = nCount;}
	virtual int  GetFrameCount()					{return m_nFrameCount;}
	virtual void SetDelay(int nDelay);
	virtual int  GetDelay()							{return m_nDelay;}
	virtual void Play();
	virtual void Pause();
	virtual void Resume();
	virtual void SetCurFrame(int nFrame);
	virtual BOOL IsPlaying()						{return m_nState == statePlay;}
	virtual BOOL IsTransparent(POINT& pt)		{return !IsEnabled();}
public:
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip);
	virtual void OnTimer(DWORD dwEventID);
	
protected:
	virtual void DestroyWindow();
};
//------------------------------------------------------------------------
// �б��ؼ�
class WNDSYS_API XListBox : public XWindow  
{
protected:
	struct ItemData		// ��Ŀ�ṹ
	{
		DWORD dwItemData;
		string str;
		BOOL operator < (const ItemData& itemdata)
		{
			return str < itemdata.str;
		}
	};
	
	typedef list<ItemData>				ItemList;
	typedef list<ItemData>::iterator	ItemListPtr;
	
	int				m_nSelectIndex;		// ѡ����Ŀ������
	ItemList		m_lstItem;			// ��Ŀ�б�
	XScrollBar*		m_pVScrollBar;		// ��ֱ������
	int				m_nVisibleCount;	// �ɼ�����Ŀ��
	int				m_nItemHeight;		// ��Ŀ�ĸ߶ȣ������������֧�����е���Ŀ����ͳһ�ĸ߶�
	BOOL			m_bAutoSort;		// �Ƿ��Զ�����
	
Signals:
	Signal1			s_selChanged;
public:
	XListBox();
	XListBox(XWindow* pParentWnd);
	virtual ~XListBox();
	virtual int GetWindowType()		{return WT_LISTBOX;}
	
	virtual void SetAutoSort(BOOL bSort)	{m_bAutoSort = bSort;}
	virtual BOOL IsAutoSort()				{return m_bAutoSort;}
	
	// һ�����
	virtual int GetCount();
	virtual int ItemFromPoint(POINT pt);
	virtual void SetItemHeight(int nHeight);
	virtual int GetItemHeight();
	virtual int SetTopIndex(int nIndex);
	virtual int GetTopIndex();
	
	// ��Ŀ����
	virtual int AddItem(LPCSTR lpszItem);
	virtual int AddItem(LPCSTR lpszItem, DWORD dwItemData);
	virtual int RemoveItem(int nIndex);
	virtual int InsertItem(int nIndex, LPCSTR lpszItem);
	virtual void RemoveAll();
	virtual int FindItem(int nStartAfter, LPCSTR lpszItem);
	virtual int SelectItem(int nStartAfter, LPCSTR lpszItem);
	virtual LPCSTR GetText(int nIndex);
	virtual int SetItemData(int nIndex, DWORD dwItemData);
	virtual DWORD GetItemData(int nIndex);
	
	// ��ѡ
	virtual int SetCurSel(int nSelect);
	virtual int GetCurSel();
	
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip);
	virtual void SetWindowPos(RECT& rc);
	virtual void SetScrollBar(XWindow* pVScroll);
	
	virtual void ParseStringToListBox(LPCSTR lpszContext, char cSeparator = ',');
	virtual BOOL IsTransparent(POINT& pt)		{return FALSE;}

public:
	virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseWheel(UINT nButton, UINT nShift, short zDelta, POINT pt);
	
	virtual DWORD OnKeyDown(UINT nKeyCode, UINT nShift);
	
	virtual void OnMouseEnter();
	virtual void OnMouseExit();
	
	virtual void OnSetFocus();
	virtual void OnKillFocus();
private:
	void OnScrollPosChanged(DWORD dwParam);
};
//------------------------------------------------------------------------
// ���ӿؼ�
class WNDSYS_API XLinkText : public XWindow  
{
protected:
	WPixel	m_cNotVisitedColor;	// δ���ʹ���������ɫ
	WPixel	m_cVisitedColor;	// ���ʹ���������ɫ
	WPixel	m_cRollOverColor;	// ��ͣʱ����ɫ
	BOOL	m_bVisited;			// �Ƿ���ʹ�
	WPixel	m_cCurColor;		// ��ǰӦ����ɫ
	string	m_strURL;			// ���ӵ�ַ
	
Signals:
	Signal1	s_runURL;
public:
	XLinkText();
	XLinkText(XWindow* pParentWnd);
	virtual ~XLinkText();
	virtual int GetWindowType()									{return WT_LINKTEXT;}
	
	void SetNotVisitedColor(int red, int green, int blue)	{m_cNotVisitedColor = WPixel(red, green, blue);}
	void SetVisitedColor(int red, int green, int blue)		{m_cVisitedColor = WPixel(red, green, blue);}
	void SetRollOverColor(int red, int green, int blue)		{m_cRollOverColor = WPixel(red, green, blue);}
	void SetURL(LPCSTR szURL)								{m_strURL = szURL;}
	LPCSTR GetURL()											{return m_strURL.c_str();}
	void SetVisited(BOOL bVisited = TRUE)					{m_bVisited = bVisited;}
	BOOL IsVisited()										{return m_bVisited;}
	
	virtual BOOL IsTransparent(POINT& pt)					{return FALSE;}
public:
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual void OnMouseEnter();
	virtual void OnMouseExit();
	
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip);
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip);
};
//------------------------------------------------------------------------
// Item��Item�������
class XItem;
struct IItemContainer
{
	virtual void SetDirty(XItem* pItem) = 0;
};
class XLine;
struct ILineContainer
{
	virtual void SetDirty(XLine* pLine, XItem* pItem) = 0;
};
// Item����,����ϣ������ItemContainer�е���Ŀ������ʵ������ӿ�
class WNDSYS_API XItem : public ITimerSink
{
public:
	enum {NotParent=-1,};
	enum LinkStyle {lsNoLink=0,lsLinkPrev=1,lsLinkNext=2};
	int				m_nLinkStyle;	// item��ǰ���е���������
	int				m_nOffsetX;		// item�����е�xƫ��
	int				m_nWidth;		// item�Ĵ�С
	int				m_nHeight;
	int				m_nParent;		// ��item����
	IItemContainer* m_pContainer;	// item����
	DWORD			m_dwUserData;	// �û�����
public:
	enum {InvalidPos=-1,};
	XItem() : m_nLinkStyle(lsNoLink), m_nOffsetX(0), 
		m_nWidth(0), m_nHeight(0), m_nParent(NotParent), m_pContainer(NULL), m_dwUserData(0) {}
	XItem(XItem* pItem);
	virtual ~XItem()	{}
	// �Ƿ���Ч
	virtual BOOL IsValid() = 0;
	// �Ƿ��ܹ����ָ�
	virtual BOOL CanSplit()	= 0;
	// �õ����(����ȣ��綯���践�������)
	virtual int  GetWidth() = 0;
	// �õ��߶�(���߶ȣ��綯���践�����߶�)
	virtual int  GetHeight() = 0;
	// ����ָ�λ��(��nFromPos��ʼ����ΪxDisc��λ��,����λ�õĸ����ǳ���ģ��ȿ�����
	// �ı����ַ���������Ҳ������ͼƬ�е�����ƫ�Ƶȣ����xDisc=-1,���ʾ��nFromPos��
	// ��Ŀĩβ)
	virtual int  CalcSplitPos(int nFromPos, int xDisc) {return InvalidPos;}
	// �ָ�һ����ĿΪ��������Ŀ
	virtual void SplitItem(int nLimitedWidth, XItem*& pItem1, XItem*& pItem2){};
	// ������������
	virtual void SetLinkStyle(int nStyle)		{m_nLinkStyle|=nStyle;}
	// �õ���������
	virtual int  GetLinkStyle()					{return m_nLinkStyle;}
	// ����x��ƫ��
	virtual void SetOffsetX(int nOffX)			{m_nOffsetX = nOffX;}
	// �õ�x��ƫ��
	virtual int  GetOffsetX()					{return m_nOffsetX;}
	// ���ø�item
	virtual void SetParent(int nParent)			{m_nParent = nParent;}
	// �õ���item
	virtual int  GetParent()					{return m_nParent;}
	// �û�����
	virtual void  SetUserData(DWORD dwUserData)	{m_dwUserData = dwUserData;}
	virtual DWORD GetUserData()					{return m_dwUserData;}
	// �õ��������(��������ڱ���Ŀ�ϵ���״)
	virtual int  GetCursorType()				{return CT_ARROW;}
	// �ͷ�
	virtual void Release()						{delete this;}
	// ��ͼ�¼�
	virtual void OnDraw(ICanvas* pCanvas, int x, int y, RECT& rcClip, int alpha)	{}
	// ִ�б���Ŀ�Ļص�����
	virtual void Emit()							{}
	// ����¼�
	virtual void OnMouseEnter()	{}
	virtual void OnMouseExit()	{}
	//virtual DWORD OnMouseDown(UINT nButton, UINT nShift, POINT pt)	{return 0;}
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt)	{return 0;}
	// ʱ���¼�
	virtual void OnTimer(DWORD dwEventID) {}
	// ����
	void SetContainer(IItemContainer* pContainer)	{m_pContainer = pContainer;}
	IItemContainer* GetContainer()					{return m_pContainer;}
	void SetDirty()									{if (m_pContainer) m_pContainer->SetDirty(this);}
	virtual BOOL SaveToHtml(FILE* hFile)	{return TRUE;}
};
//------------------------------------------------------------------------
// �ı���Ŀ
class WNDSYS_API XItemText : public XItem
{
protected:
	WPixel		m_clrText;		// �ı�����ɫ
	ITTFont*	m_pFont;		// �����ı�ʱʹ�õ�����
	string		m_strText;		// �ı��ַ���
public:
	XItemText() : m_clrText(0), m_pFont(0) {m_strText.clear();}
	XItemText(XItemText* pItem);
	virtual ~XItemText() {}
	
	virtual BOOL IsValid()			{return (m_pFont && !m_strText.empty());}
	virtual BOOL CanSplit()			{return TRUE;}
	virtual int  GetWidth()			{if(m_nWidth==0)CalcSize();return m_nWidth;}
	virtual int  GetHeight()		{if(m_nHeight==0)CalcSize();return m_nHeight;}
	virtual void OnDraw(ICanvas* pCanvas, int x, int y, RECT& rcClip, int alpha);
	virtual int  CalcSplitPos(int nFromPos, int xDisc);
	virtual void SplitItem(int nLimitedWidth, XItem*& pItem1, XItem*& pItem2);
	
	void		SetFont(ITTFont* pFont)	{m_pFont = pFont;}
	ITTFont*	GetFont()				{return m_pFont;}
	void		SetText(LPCSTR szText)	{m_strText = szText;}
	LPCSTR		GetText()				{return m_strText.c_str();}
	void		SetTextColor(WPixel clr){m_clrText = clr;}
	WPixel		GetTextColor()			{return m_clrText;}
	int			GetTextLength()			{return m_strText.length();}
	virtual BOOL SaveToHtml(FILE* hFile);
protected:
	void CalcSize();
};
//------------------------------------------------------------------------
// �����ӵ��ı���Ŀ
class WNDSYS_API XItemLinkText : public XItemText
{
protected:
	string		m_strLinkText;		// ���ӵ�����
	WPixel		m_clrNotVisited;	// δ���ʹ���������ɫ
	WPixel		m_clrVisited;		// ���ʹ���������ɫ
	WPixel		m_clrRollOver;		// ��ͣʱ����ɫ
	BOOL		m_bVisited;			// �Ƿ���ʹ�
	BOOL		m_bRollOver;		// ��ǰ�Ƿ���ͣ
public:
	XItemLinkText() : m_strLinkText(""), m_clrNotVisited(0),
		m_clrVisited(0), m_clrRollOver(0), m_bVisited(FALSE), m_bRollOver(FALSE) {}
	XItemLinkText(XItemLinkText* pItem);
	void	SetLinkText(LPCSTR szLink)		{m_strLinkText = szLink;}
	LPCSTR	GetLinkText()					{return m_strLinkText.c_str();}
	void	SetNotVisitedColor(WPixel clr)	{m_clrNotVisited = clr;}
	WPixel  GetNotVisitedColor()			{return m_clrNotVisited;}
	void	SetVisitedColor(WPixel clr)		{m_clrVisited = clr;}
	WPixel  GetVisitedColor()				{return m_clrVisited;}
	void	SetRolloverColor(WPixel clr)	{m_clrRollOver = clr;}
	WPixel  GetRolloverColor()				{return m_clrRollOver;}
	void	SetVisited(BOOL visited = TRUE) {m_bVisited = visited;}
	BOOL	IsVisited()						{return m_bVisited;}
	
	virtual int  GetCursorType()			{return CT_HAND;}
	virtual void OnDraw(ICanvas* pCanvas, int x, int y, RECT& rcClip, int alpha);
	virtual void SplitItem(int nLimitedWidth, XItem*& pItem1, XItem*& pItem2);
	virtual void OnMouseEnter();
	virtual void OnMouseExit();
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual BOOL SaveToHtml(FILE* hFile);
};
//------------------------------------------------------------------------
class WNDSYS_API XItemBookmarkText : public XItemText
{
protected:
	string	m_strBookmark;	// ��ǩ��
public:
	XItemBookmarkText() : XItemText()		{m_strBookmark.clear();}
	XItemBookmarkText(XItemBookmarkText* pItem)	: XItemText(pItem),
		m_strBookmark(pItem->m_strBookmark) {}
	void SetBookmark(LPCSTR szBookmark)		{m_strBookmark = szBookmark;}
	LPCSTR GetBookmark()					{return m_strBookmark.c_str();}
	virtual BOOL SaveToHtml(FILE* hFile);
};
//------------------------------------------------------------------------
// ͼƬ��Ŀ
// ע��Ĭ�϶���ͼƬ��ÿһ֡�����޼������(Ĭ��ÿ֡��С���)
class WNDSYS_API XItemImage : public XItem
{
protected:
	IBitmap*	m_pImage;		// ֻ���ã���ɾ��
	int			m_nFrameCount;	// ֡��(����Ƕ�����m_nFrameCount>1������m_nFrameCount=1)
	int			m_nCurFrame;	// ��ǰ֡
	int			m_nDelay;		// ������ʱ
public:
	XItemImage() : m_pImage(NULL), m_nFrameCount(0), m_nCurFrame(0), m_nDelay(200) {}
	virtual ~XItemImage(){}
	virtual BOOL IsValid()			{return (m_pImage!=NULL && m_nFrameCount!=0);}
	virtual BOOL CanSplit()			{return FALSE;}
	virtual int  GetWidth()			{if(m_nWidth==0)m_nWidth=m_pImage->GetWidth()/m_nFrameCount;return m_nWidth;}
	virtual int  GetHeight()		{if(m_nHeight==0)m_nHeight=m_pImage->GetHeight();return m_nHeight;}
	virtual void OnDraw(ICanvas* pCanvas, int x, int y, RECT& rcClip, int alpha);
	virtual void OnTimer(DWORD dwEventID);
	virtual void Release();
	
	void SetBitmap(IBitmap* pBmp)	{m_pImage = pBmp;}
	IBitmap* GetBitmap()			{return m_pImage;}
	void SetFrameCount(int nCount);
	int  GetFrameCount()			{return m_nFrameCount;}
	void SetDelay(int nDelay)		{m_nDelay = nDelay;}
	int  GetDelay()					{return m_nDelay;}
	virtual BOOL SaveToHtml(FILE* hFile);
};
//------------------------------------------------------------------------
class WNDSYS_API XItemLinkImage : public XItemImage
{
protected:
	string	m_strLinkText;
public:
	XItemLinkImage()					{m_strLinkText.clear();}
	void	SetLinkText(LPCSTR szLink)	{m_strLinkText = szLink;}
	LPCSTR	GetLinkText()				{return m_strLinkText.c_str();}

	virtual int  GetCursorType()		{return CT_HAND;}
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual BOOL SaveToHtml(FILE* hFile);
};
//------------------------------------------------------------------------
// �й�����
class WNDSYS_API XLine : public IItemContainer
{
protected:
	typedef vector<XItem*>					Items;
	typedef vector<XItem*>::iterator		ItemsPtr;
	Items			m_vItems;		// item* ����
	int				m_nAlign;		// ���뷽ʽ(���С���)
	int				m_nWidth;		// ��ǰitemʵ��ռ�еĿ��
	int				m_nHeight;		// �������߶�
	int				m_nOffsetY;		// ����������������y��ƫ�ƣ����ٶ�λ
	int				m_nLineSpace;	// ����������֮����м��
	ILineContainer* m_pContainer;	// ����ָ��,����ָILineContainer*
	
public:
	enum Align{LeftAlign=0,CenterAlign,RightAlign};
	XLine() : m_nAlign(LeftAlign), m_nWidth(0), m_nHeight(0), m_nOffsetY(0), m_nLineSpace(0), m_pContainer(0) {m_vItems.clear();}
	XLine(int nAlign, ILineContainer* pContainer)
	{
		m_nAlign = nAlign;
		m_pContainer = pContainer;
		m_nWidth = m_nHeight = m_nOffsetY = m_nLineSpace = 0;
		m_vItems.clear();
	}
	void SetAlign(int nAlign)			{m_nAlign = nAlign;}
	int  GetAlign()						{return m_nAlign;}
	int  GetHeight()					{return m_nHeight;}
	void SetHeight(int h)				{m_nHeight = h;}
	int  GetWidth()						{return m_nWidth;}
	void SetWidth(int w)				{m_nWidth = w;}
	BOOL IsEmpty()						{return m_vItems.empty();}
	int  GetItemCount()					{return m_vItems.size();}
	XItem* GetItem(int nItem)			{return m_vItems[nItem];}
	void SetOffsetY(int nOffsetY)		{m_nOffsetY = nOffsetY;}
	int  GetOffsetY()					{return m_nOffsetY;}
	void SetLineSpace(int nSpace)		{m_nLineSpace = nSpace;}
	int  GetLineSpace()					{return m_nLineSpace;}
	virtual void OnDraw(ICanvas* pCanvas, int x, int y, RECT& rcClip, int alpha);
	void AddItem(XItem* pItem);
	void Clear();
	virtual void SetDirty(XItem* pItem);
	BOOL SaveToHtml(FILE* hFile);
};
//------------------------------------------------------------------------
// item�������࣬��������ꡢ�����¼���û���Դ���������������֧�ֹ�����
class WNDSYS_API XItemContainer : public XWindow, public ILineContainer
{
protected:
	typedef vector<XItem*>					Items;
	typedef vector<XItem*>::iterator		ItemsPtr;
	typedef vector<XLine*>					Lines;
	typedef vector<XLine*>::iterator		LinesPtr;
	
	Items		m_vSplitItems;	// ���ڱ��ָ����ĿӦ�ý���һ���б�,�÷ָ�����Ŀ��ס�Լ��ĸ���Ŀ������
	Lines		m_vLines;		// ����������
	int			m_nMouseHover;	// ���ͣ���� Line(����),item(����)
	BOOL		m_bLineWrap;	// �Ƿ���Ҫ�Զ�����
	int			m_nMaxWidth;	// �����������
	int			m_nMaxHeight;	// ���������߶�
	POINT		m_ptOffset;		// ���ڹ������µ�x��y�����ƫ��
	XScrollBar*	m_pHScrollBar;	// ˮƽ������
	XScrollBar*	m_pVScrollBar;	// ��ֱ������
private:
	int			m_nMargin;		// �߿�հ������С
	
public:
	XItemContainer();
	XItemContainer(XWindow* pParentWnd);
	virtual ~XItemContainer();
	
	// ����
	void SetMargin(int nMargin)		{m_nMargin = max(0, nMargin);}
	int  GetMargin()				{return m_nMargin;}
	void SetAutoWrap(BOOL bWrap)	{m_bLineWrap = bWrap;}
	BOOL IsAutoWrap()				{return m_bLineWrap;}
	int  GetMaxWidth()				{return m_nMaxWidth;}
	void SetMaxWidth(int nWidth)	{m_nMaxWidth = nWidth;}
	int  GetMaxHeight()				{return m_nMaxHeight;}
	void SetMaxHeight(int nHeight)	{m_nMaxHeight = nHeight;}
	void SetOffset(POINT ptOff)		{m_ptOffset = ptOff;}
	void SetOffset(int x, int y)	{m_ptOffset.x = x;m_ptOffset.y = y;}
	POINT GetOffset()				{return m_ptOffset;}
	POINT* GetOffsetPtr()			{return &m_ptOffset;}
	int  GetLineCount()				{return m_vLines.size();}
	XLine* GetLine(int nLine)		{return m_vLines[nLine];}
	void SetHScrollBar(XScrollBar* pBar);
	void SetVScrollBar(XScrollBar* pBar);
	
	// ����
	virtual void DeleteLines(int nLines);
	virtual void Clear();
	virtual XLine* NewLine(int nAlign = XLine::LeftAlign, int nLineSpace = 2);
	virtual BOOL AddItem(XItem* pItem, BOOL bNewLine, int nAlign = XLine::LeftAlign, int nLineSpace = 2);
	virtual void BringToTopLine(int nLine);
	virtual void SetDirty(XLine* pLine, XItem* pItem);	
	void AdjustHScroll();
	void AdjustVScroll();
	BOOL SaveToHtml(LPCSTR szFileName, LPCSTR szTitle=NULL, WPixel backcolor=0);
protected:
	virtual int	GetMaxValidWidth();
	int  GetItemByOffset(int nLine, int nOffsetX);
	void OnItemChanged(int nLine, int nItem);
	void OnHScrollPosChanged(DWORD dwParam);
	void OnVScrollPosChanged(DWORD dwParam);
	void OnClick()	{this->SetFocus();}
	
public:
	virtual DWORD OnMouseMove(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseUp(UINT nButton, UINT nShift, POINT pt);
	virtual DWORD OnMouseWheel(UINT nButton, UINT nShift, short zDelta, POINT pt);
	virtual DWORD OnKeyDown(UINT nKeyCode, UINT nShift);
	virtual void OnDraw(ICanvas* pCanvas, RECT& rcClip);
	virtual BOOL IsTransparent(POINT& pt)		{return FALSE;}
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip) {}
	virtual	void SetWindowPos(RECT& rc);
protected:
	virtual void DestroyWindow();
private:
	int  GetAlignOffset(XLine* pLine);
};
//------------------------------------------------------------------------
// ���������࣬�н��������
class WNDSYS_API XParserBase
{
protected:
	XItemContainer* m_pContainer;	// item����
public:
	XParserBase() : m_pContainer(NULL) {}
	virtual ~XParserBase() {}
	
	void SetItemContainer(XItemContainer* pContainer)	{m_pContainer = pContainer;}
	XItemContainer* GetItemContainer()					{return m_pContainer;}
	virtual BOOL ParseLine(LPCSTR szLine)				{return FALSE;}
};
//------------------------------------------------------------------------
// ���ֱ�ǩ��������
struct Context
{
};
//------------------------------------------------------------------------
struct ParagraphContext : public Context
{
	int			nAlign;
	int			nLineSpace;
	ParagraphContext() {clear();}
	void clear() {nAlign=0;nLineSpace=2;}
};
//------------------------------------------------------------------------
struct TextContext : public Context
{
	string		strText;		// ����
	WPixel		clrText;		// �ı���ɫ
	string		strFontName;	// ������
	int			nFontSize;		// �����С
	BOOL		bBlod;			// ����
	BOOL		bItalic;		// б��
	BOOL		bUnderline;		// �»���
	TextContext() {clear();}
	void clear(){
		strText=" ",clrText=WPixel(255,255,255),strFontName="����",nFontSize=12;
		bBlod=0,bItalic=0,bUnderline=0;
	}
};
//------------------------------------------------------------------------
struct LinkTextContext : public TextContext
{
	string		strLinkText;	// ���ӵ�����
	LinkTextContext() {clear();}
	void clear(){TextContext::clear();strLinkText=strText;bUnderline=0;}
};
//------------------------------------------------------------------------
struct BookmarkTextContext : public TextContext
{
	string		strBookmark;	// ��ǩ��
	BookmarkTextContext() {clear();}
	void clear(){TextContext::clear();strBookmark=strText;}
};
struct ImageContext : public Context
{
	string		strImage;		// ͼƬ��
	int			nCount;			// ֡��
	int			nDelay;			// ������ʱ
	int			nType;			// �������ͣ��ⲿ�ļ�(0)���ǰ����ļ�(1)��
	ImageContext() {clear();}
	void clear() {strImage="",nCount=1,nDelay=200,nType=0;}
};
//------------------------------------------------------------------------
// ���ֱ�ǩ������
class WNDSYS_API CTagParser
{
protected:
	XLineParser*		m_pLineParser;
public:
	CTagParser();
	virtual ~CTagParser();

	virtual LPCSTR	GetOpenKey()		{return "";}
	virtual int		GetOpenKeyLen()		{return 0;}
	virtual LPCSTR	GetCloseKey()		{return "";}
	virtual int		GetCloseKeyLen()	{return 0;}
	virtual BOOL	HasCloseTag()		{return TRUE;}
	virtual BOOL	Parse(char* szLine, char* pBegin, char* pEnd)	{return FALSE;}
	virtual BOOL	Emit() {return FALSE;}

public:
	void SetLineParser(XLineParser* pLineParser)	{m_pLineParser = pLineParser;}
	XLineParser* GetLineParser()					{return m_pLineParser;}

	char* FindEndTag(char* pBegin);
	BOOL IsThisTag(LPCSTR szTag);

protected:
	char GetLowerChar(char* c);
	void SkipSpace(char* szLine, char*& pBegin, char* pEnd);
	void SkipSpaceAndQuot(char* szLine, char*& pBegin, char* pEnd);
	int  ConvertStringToInt(LPCSTR szString);
	BOOL GetPropertyString(char* szLine, char*& pBegin, char* pEnd, char* buffer, int bufsize);
};
//------------------------------------------------------------------------
class WNDSYS_API CTagParserParagraph : public CTagParser
{
protected:
	ParagraphContext	context;
public:
	virtual LPCSTR GetOpenKey()			{return "<p";}
	virtual int	   GetOpenKeyLen()		{return 2;}
	virtual LPCSTR GetCloseKey()		{return ">";}
	virtual int  GetCloseKeyLen()		{return 1;}
	virtual BOOL HasCloseTag()			{return FALSE;}
	virtual BOOL Parse(char* szLine, char* pBegin, char* pEnd);
	virtual BOOL Emit();
};
//------------------------------------------------------------------------
class WNDSYS_API CTagParserText : public CTagParser
{
protected:
	TextContext		context;
public:
	virtual LPCSTR GetOpenKey()			{return "<text";}
	virtual int	   GetOpenKeyLen()		{return 5;}
	virtual LPCSTR GetCloseKey()		{return "</text>";}
	virtual int  GetCloseKeyLen()		{return 7;}
	virtual BOOL Parse(char* szLine, char* pBegin, char* pEnd);
	virtual BOOL Emit();
};
//------------------------------------------------------------------------
class WNDSYS_API CTagParserLink : public CTagParser
{
protected:
	LinkTextContext	context;
public:
	virtual LPCSTR GetOpenKey()			{return "<link";}
	virtual int	   GetOpenKeyLen()		{return 5;}
	virtual LPCSTR GetCloseKey()		{return "</link>";}
	virtual int  GetCloseKeyLen()		{return 7;}
	virtual BOOL Parse(char* szLine, char* pBegin, char* pEnd);
	virtual BOOL Emit();
};
//------------------------------------------------------------------------
class WNDSYS_API CTagParserBookmark : public CTagParser
{
protected:
	BookmarkTextContext	context;
public:
	virtual LPCSTR GetOpenKey()			{return "<bookmark";}
	virtual int	   GetOpenKeyLen()		{return 9;}
	virtual LPCSTR GetCloseKey()		{return "</bookmark>";}
	virtual int  GetCloseKeyLen()		{return 11;}
	virtual BOOL Parse(char* szLine, char* pBegin, char* pEnd);
	virtual BOOL Emit();
};
//------------------------------------------------------------------------
class WNDSYS_API CTagParserImage : public CTagParser
{
protected:
	ImageContext	context;
public:
	virtual LPCSTR GetOpenKey()			{return "<img";}
	virtual int	   GetOpenKeyLen()		{return 4;}
	virtual LPCSTR GetCloseKey()		{return ">";}
	virtual int  GetCloseKeyLen()		{return 1;}
	virtual BOOL HasCloseTag()			{return FALSE;}
	virtual BOOL Parse(char* szLine, char* pBegin, char* pEnd);
	virtual BOOL Emit();
};
//------------------------------------------------------------------------
// �н�����
#define TAG_MARK_BEGIN	'<'
#define TAG_MARK_END	'>'
class WNDSYS_API XLineParser : public XParserBase
{
private:
	int						m_nAlign;
	vector<CTagParser*>		m_vTagParserList;	// ���ֱ�ǩ�������б�
public:
	XLineParser();
	virtual ~XLineParser();
	
	BOOL RegisterTagParser(CTagParser* pTagParser);
	BOOL UnregisterTagParser(CTagParser* pTagParser);
	void UnregisterAllTagParser();
	CTagParser* GetMatchTagParser(char* pBegin, char* pEnd);
	virtual BOOL ParseLine(LPCSTR szLine);
protected:
	BOOL AddText(TextContext context);
};
//------------------------------------------------------------------------
// �����
class WNDSYS_API XChat : public XWindow
{
protected:
	class XChatScrollBar : public XScrollBar
	{
	public:
		XChatScrollBar(XWindow* pParentWnd) : XScrollBar(pParentWnd) {}
		virtual void LineLeftUp();
		virtual void LineRightDown();
	};
	BOOL				m_bAutoSroll;	// �Ƿ��Զ�����
	int					m_nMaxLines;	// ������������
	XChatScrollBar*		m_pVScrollBar;	// ��ֱ������
	XItemContainer*		m_pContainer;	// item����
	XParserBase*		m_pParser;		// ��ʽ�ı�������
	
public:
	XChat();
	XChat(XWindow* pParentWnd);
	virtual ~XChat();
	virtual int GetWindowType()				{return WT_CHAT;}
	
	// for item container
	void SetMargin(int nMargin);
	void SetAutoWrap(BOOL bWrap);
	
	// parser
	void SetParser(XParserBase* pParser);
	XParserBase* GetParser()			{return m_pParser;}
	
	// item container
	void SetItemContainer(XItemContainer* pContainer)	{m_pContainer = pContainer;}
	XItemContainer* GetItemContainer()	{return m_pContainer;}
	
	XScrollBar* GetVScrollBar()			{return m_pVScrollBar;}
	
	void SetAutoScroll(BOOL bAuto=TRUE)	{m_bAutoSroll = bAuto;}
	BOOL IsAutoScroll()					{return m_bAutoSroll;}
	void SetMaxLines(int nLines)		{m_nMaxLines = max(10, nLines);}
	int  GetMaxLines()					{return m_nMaxLines;}
	
	void Clear();
	BOOL ParseString(LPCSTR szUnFormatText);
	
public:
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip) {}
	virtual void SetWindowPos(RECT& rc);
	virtual BOOL IsTransparent(POINT& pt)		{return !IsEnabled();}
	virtual DWORD OnMouseWheel(UINT nButton, UINT nShift, short zDelta, POINT pt);
	virtual DWORD OnKeyDown(UINT nKeyCode, UINT nShift);
	virtual DWORD OnKeyUp(UINT nKeyCode, UINT nShift);
	
public:
	void AdjustScroll();
};
//------------------------------------------------------------------------
// ��HTML�ؼ�
class WNDSYS_API XHtml : public XWindow  
{
public:
	class XHtmlScrollBar : public XScrollBar
	{
	public:
		XHtmlScrollBar(XWindow* pParentWnd) : XScrollBar(pParentWnd) {}
		virtual void LineLeftUp();
		virtual void LineRightDown();
	};
	// ��ת�ı�(����ҳ�����)
	class XItemJumpText : public XItemLinkText
	{
		XHtml*		m_pXHtml;
	public:
		virtual void Emit();	
		void SetXHtmlPtr(XHtml* pXHtml) {m_pXHtml = pXHtml;}
	};
	class CTagParserJump : public CTagParserLink
	{
		XHtml*		m_pXHtml;
	public:
		virtual BOOL Emit();
		void SetXHtmlPtr(XHtml* pXHtml) {m_pXHtml = pXHtml;}
	};
	class CTagParserBookmarkEx : public CTagParserBookmark
	{
		XHtml*		m_pXHtml;
	public:
		virtual BOOL Emit();
		void SetXHtmlPtr(XHtml* pXHtml) {m_pXHtml = pXHtml;}
	};	
	typedef string		Line;
	typedef list<Line>	Lines;
	typedef list<Line>::iterator	LinesPtr;
	// �ļ�����������
	class FileHelper
	{
	private:
		enum {buffer_size = 2048};
	public:
		static bool Read(const char* szFileName, Lines& lines);
	};
public:
	typedef hash_map<string, int>				BookmarkList;
	typedef hash_map<string, int>::iterator		BookmarkListPtr;
	typedef hash_map<string, BOOL>				JumpList;
	typedef hash_map<string, BOOL>::iterator	JumpListPtr;
	typedef vector<string>						NavigateList;
	typedef vector<string>::iterator			NavigateListPtr;

	XHtmlScrollBar*	m_pHScrollBar;	// ˮƽ������
	XHtmlScrollBar*	m_pVScrollBar;	// ��ֱ������
	XItemContainer* m_pContainer;	// item����

	// ���������
	XLineParser				m_LineParser;			// �н�����
	CTagParserParagraph		m_TagParserParagraph;	// �����ǩ������
	CTagParserText			m_TagParserText;		// һ���ı���ǩ������
	CTagParserJump			m_TagParserJump;		// ��ת��ǩ������
	CTagParserBookmarkEx	m_TagParserBookmark;	// ��ǩ��ǩ������
	CTagParserImage			m_TagParserImage;		// ͼƬ��ǩ������

	// ҳ����Ϣ��¼
	BookmarkList			m_BookmarkList;			// ��ǩ�б�,intָ������
	JumpList				m_JumpList;				// ��ת�б�,BOOL�����Ƿ��Ѿ���ת��

	// ��ǰxhtm�Ĺ���Ŀ¼,ָ�򱣴�xhtm�ű���λ��
	string					m_strWorkingDir;		// ��ǰxhtm�ؼ��Ĺ���Ŀ¼
	string					m_strFileName;			// ��ǰxhtm�ؼ����صĽű��ļ���
	NavigateList			m_NavigateList;			// ���ڡ�ǰ�����͡����ˡ�����
	int						m_nCurNavigateIdx;		// ��ǰ��������
public:
	XHtml();
	XHtml(XWindow* pParentWnd);
	virtual ~XHtml();
	virtual int GetWindowType()			{return WT_HTML;}
	
	void SetMargin(int nMargin)		{m_pContainer->SetMargin(nMargin);}
	
	void Clear();
	void SetWorkingDir(LPCSTR szWorkingDir);
	XItemContainer* GetItemContainer()	{return m_pContainer;}
	
	// xhtm��������
	virtual void GoBack();
	virtual void GoForward();
	virtual BOOL CanBack();
	virtual BOOL CanForward();
	virtual void Navigate(LPCSTR strUrl);
	BOOL AddLine(LPCSTR szLineText);
	void RegisterTagParser(CTagParser* pTagParser);

	// ��ת����ǩ����
	virtual void SetJumpVisited(LPCSTR strJump, BOOL bVisited);
	virtual void AddBookmarkToHashmap(LPCSTR strBookmark);
	virtual void AddJumpToHashMap(LPCSTR strJump, BOOL bVisited);
	virtual BOOL GetJumpVisited(LPCSTR strJump);

public:
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip) {}
	virtual void SetWindowPos(RECT& rc);
	virtual BOOL IsTransparent(POINT& pt)		{return FALSE;}
	virtual DWORD OnMouseWheel(UINT nButton, UINT nShift, short zDelta, POINT pt);
	virtual DWORD OnKeyDown(UINT nKeyCode, UINT nShift);
	virtual DWORD OnKeyUp(UINT nKeyCode, UINT nShift);

protected:
	void JumpToUrl(LPCSTR strUrl);

private:
	void LocateBookmark(LPCSTR strBookmark);
	void LoadFile(LPCSTR szFileName);
	void RegisterAllParser();
};
//------------------------------------------------------------------------
// Talk�ؼ�
class WNDSYS_API XTalk : public XWindow
{
public:
	class XItemTaskText : public XItemLinkText
	{
	protected:
		XTalk*			m_pXTalk;
		int				m_nTaskID;
	public:
		
		void SetTaskID(int nTaskID)		{m_nTaskID = nTaskID;}	// ������������
		long GetTaskID()				{ return m_nTaskID;}	// ȡ����������
		void SetTalkPtr(XTalk* pXTalk)	{m_pXTalk = pXTalk;}
		virtual void Emit();	
	};
	class CTagParserTaskText : public CTagParserLink
	{
	protected:
		XTalk*		m_pXTalk;
	public:
		void SetTalkPtr(XTalk* pXTalk)	{m_pXTalk = pXTalk;}
		virtual BOOL Emit();
	};
	typedef XHtml::XHtmlScrollBar	XTalkScrollBar;
public:
	XTalkScrollBar*	m_pVScrollBar;	// ��ֱ������
	XItemContainer* m_pContainer;	// item����
	
	// ���������
	XLineParser				m_LineParser;			// �н�����
	CTagParserParagraph		m_TagParserParagraph;	// �����ǩ������
	CTagParserText			m_TagParserText;		// һ���ı���ǩ������
	CTagParserTaskText		m_TagParserTaskText;	// �����ı���ǩ������
	CTagParserImage			m_TagParserImage;		// ͼƬ��ǩ������
	
public:
	XTalk();
	XTalk(XWindow* pParentWnd);
	virtual ~XTalk();
	
	virtual int GetWindowType()			{return WT_TALK;}
	void SetMargin(int nMargin)		{m_pContainer->SetMargin(nMargin);}
	void Clear();
	XItemContainer* GetItemContainer()	{return m_pContainer;}
	
	BOOL AddLine(LPCSTR szLineText);
	void RegisterTagParser(CTagParser* pTagParser);
public:
	virtual void OnEraseBkgnd(ICanvas* pCanvas, RECT& rcClip) {}
	virtual BOOL IsTransparent(POINT& pt)		{return FALSE;}
	virtual DWORD OnMouseWheel(UINT nButton, UINT nShift, short zDelta, POINT pt);
	virtual void OnTaskTextClick(LPCSTR szText)	{}
	virtual void SetWindowPos(RECT& rc);
private:
	void RegisterAllParser();
};
//------------------------------------------------------------------------
//����TOOLTIP��, ���ڱ�ģ���п��Ե��õ�TOOLTIP
class WNDSYS_API CToolTipLoader  
{
private:
	stdext::hash_map<int, std::string> m_hmapToolTip;
	static CToolTipLoader __instance;
public:
	void AddToolTip(int nID, LPCSTR szStr);
	LPCSTR GetToolTip(int nID);	
	static CToolTipLoader &GetInstance(void);
};
//------------------------------------------------------------------------
// ����ϵͳ������
class WNDSYS_API CWndSysHelper  
{
public:
	CWndSysHelper();
	virtual ~CWndSysHelper();
	
protected:
	IFileSystem*	m_pFileSystem;
	IResObject*		m_pResObject;
	ICanvasHelper	m_ICanvasHelper;
	
public:
	XWindow* CreateWndSys(HWND hWnd, ICanvas* pCanvas, ITimeAxis* pTimeAxis, IFileSystem* pFileSystem, IResObject* pResObject, ITTFont* pSysFont);
	XWindow* CreateDialogByResObject(XWindow* pParentWnd, IResObject* pResObject, int nID);
	XWindow* CreateXWindow(XWindow* pParentWnd, IResObject* pResObject, int nID, XWindow* pSubclassWnd = NULL);
	IBitmap* GetBitmap(LPCSTR szFile);
	XDesktop* GetGameDesktop(){return XDesktop::GetInstance();}
	DWORD OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void OnPaint(ICanvas* pCanvas);
	void CreateStockProperty(XWindow* pWnd, IResObject* pResObject, int nID);
	void CloseWndSys();
	
private:
	XCursor* CreateXCursor(IResObject* pResObject);
};
//------------------------------------------------------------------------