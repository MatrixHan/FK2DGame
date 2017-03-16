/**
*	created:		2012-6-23   20:48
*	filename: 		XAccelTable
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���ټ���
*/
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
//------------------------------------------------------------------------
XAccelTable::XAccelTable()
{
	m_AccelKeys.clear();
}
//------------------------------------------------------------------------
XAccelTable::~XAccelTable()
{

}
//------------------------------------------------------------------------
// ��ӻ��滻
void XAccelTable::AddAccelKey(UINT nKeyCode, UINT nShift, XWindow* pWnd)
{
	//if (nKeyCode >= 97 && nKeyCode <= 122) // a -> z ��Сд��ĸת���ɴ�д��ĸ
	//	nKeyCode -= 32;
	AccelKeysPtr _end = m_AccelKeys.end();
	for (AccelKeysPtr it=m_AccelKeys.begin(); it!=_end; ++it)
	{
		if ((*it).nKeyCode == nKeyCode && (*it).nShift == nShift)
		{
			(*it).pWnd = pWnd; // �滻
			return;
		}
	}

	stAccelKey ak(nKeyCode, nShift, pWnd);
	m_AccelKeys.push_back(ak);
}
//------------------------------------------------------------------------
void XAccelTable::RemoveAccelKey(UINT nKeyCode, UINT nShift)
{
	AccelKeysPtr _end = m_AccelKeys.end();	
	for (AccelKeysPtr it=m_AccelKeys.begin(); it!=_end; ++it)
	{
		if ((*it).nKeyCode == nKeyCode && (*it).nShift == nShift)
		{
			m_AccelKeys.erase(it);
			return;
		}
	}
}
//------------------------------------------------------------------------
void XAccelTable::RemoveAccelKey(XWindow* pWnd)
{
	AccelKeysPtr it = m_AccelKeys.begin();
	AccelKeysPtr _end = m_AccelKeys.end();
	for (; it!=_end;)
	{
		if ((*it).pWnd == pWnd)
		{
			m_AccelKeys.erase(it);
			_end = m_AccelKeys.end();
		}
		else
			++it;
	}
}
//------------------------------------------------------------------------
DWORD XAccelTable::HandleAccelKey(UINT nKeyCode, UINT nShift)
{
	AccelKeysPtr _end = m_AccelKeys.end();
	for (AccelKeysPtr it=m_AccelKeys.begin(); it!=_end; ++it)
	{
		if ((*it).nKeyCode == nKeyCode && (*it).nShift == nShift)
		{
			(*it).pWnd->OnAccelKey(nKeyCode, nShift);
			return 0;
		}
	}
	return NOTHANDLE;
}
//------------------------------------------------------------------------