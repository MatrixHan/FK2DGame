/**
*	created:		2012-6-23   23:28
*	filename: 		XLineParser
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../../FKGame/WndSys/WndSys.h"
#include "../Include/FKWndSysCommon.h"
//------------------------------------------------------------------------
XLineParser::XLineParser()
{
	m_nAlign = XLine::LeftAlign;
}
//------------------------------------------------------------------------
XLineParser::~XLineParser()
{

}
//------------------------------------------------------------------------
BOOL XLineParser::RegisterTagParser(CTagParser* pTagParser)
{
	pTagParser->SetLineParser(this);
	m_vTagParserList.push_back(pTagParser);
	return TRUE;
}
//------------------------------------------------------------------------
BOOL XLineParser::UnregisterTagParser(CTagParser* pTagParser)
{
	vector<CTagParser*>::iterator it;
	vector<CTagParser*>::iterator _end = m_vTagParserList.end();
	for (it=m_vTagParserList.begin(); it!=_end; ++it)
	{
		if ((*it) == pTagParser)
		{
			m_vTagParserList.erase(it);
			return TRUE;
		}
	}
	return FALSE;
}
//------------------------------------------------------------------------
void XLineParser::UnregisterAllTagParser()
{
	m_vTagParserList.clear();
}
//------------------------------------------------------------------------
// "...<key1 attrib list...>...<key2 attrib list...>... ......"
BOOL XLineParser::ParseLine(LPCSTR szLine)
{
	if (szLine == NULL) return TRUE;
	if (!m_pContainer)
	{
		TraceColorLn(MY_TRACE_COLOR, "XLineParser::ParseLine, m_pContainer is NULL");
		return FALSE;
	}

	m_nAlign = -1;
	if (*szLine == '\0') // һ������
	{
		TextContext context;
		m_nAlign = XLine::LeftAlign;
		m_pContainer->NewLine(m_nAlign);
		AddText(context);
		return TRUE;
	}

	int   nLen = strlen(szLine);
	char* pBeginTag = (char*)szLine;
	char* pBeginTagNext = 0;
	char* pEndTag = (char*)szLine + nLen;
	BOOL  bFirstTag = TRUE;
	char* pFindBegin = 0;
	char* pFindEnd = 0;
	char* pEnd = (char*)szLine + nLen;

	while (pBeginTag < pEnd)
	{
		// �Ȳ���һ�� '<'
		pFindBegin = strstr(pBeginTag, "<"); // �� '<'
		if (pFindBegin == 0) // ���д�nBeginTagPosλ�ÿ�ʼû���κα�ǩ������ʧ��
		{
			pEndTag = (char*)szLine + nLen - 1;
			goto ParseFailed;
		}

		// �б�ǩ������Ƿ�Ϸ�
		CTagParser* pTagParser = GetMatchTagParser(pFindBegin, pEnd);
		if (pTagParser == NULL) // ��֧�ֵı�ǩ������ʧ�ܣ�����'<'�������Ǳ�ǩ�Ŀ�ʼ
		{
			pEndTag = (char*)szLine + nLen - 1;
			goto ParseFailed;
		}


		// �ҵ�һ���Ϸ���ǩ�������Ƿ���ƥ��Ĺرձ�ǩ
		pFindEnd = pTagParser->FindEndTag(pFindBegin);
		if (pFindEnd == 0) // û��ƥ��Ĺرձ�ǩ������ʧ��
		{
			pEndTag = (char*)szLine + nLen - 1;
			goto ParseFailed;
		}

		// �ǵ�һ����ǩ�������Ƿ��Ƕ����ǩ��������ǣ����½�һ��
		if (bFirstTag)
		{
			bFirstTag = FALSE;
			if (pFindBegin > szLine) // ��һ����ǩǰ�滹�����֣�����ΪĬ���ı���ӵ����п�ͷ
			{
				if (m_nAlign == -1) // �½�һ��
				{
					m_nAlign = XLine::LeftAlign;
					m_pContainer->NewLine(m_nAlign);
				}
				TextContext context;
				context.strText.insert(context.strText.begin(), szLine, pFindBegin);
				AddText(context);
				pBeginTag = pFindBegin;
			}
			if (*(pFindBegin+1) != 'p' && *(pFindBegin+1) != 'P') // ���Ƕ����ǩ
			{
				if (m_nAlign == -1) // �½�һ��
				{
					m_nAlign = XLine::LeftAlign;
					m_pContainer->NewLine(m_nAlign);
				}
			}
		}

		// �ҵ��Ϸ��ı�ǩ�ԣ�������
		if (pBeginTag != pFindBegin) // ����������ǩ֮����Щ����û��д��ǩ
		{
			TextContext context;
			context.strText.insert(context.strText.begin(), pBeginTag, pFindBegin);
			AddText(context);
			pBeginTag = pFindBegin;
		}
		pBeginTag = pFindBegin;
		pEndTag = pFindEnd;
		if (!pTagParser->Parse((char*)szLine, pBeginTag, pEndTag))
			goto ParseFailed;
		pBeginTagNext = pEndTag + pTagParser->GetCloseKeyLen();
		pBeginTag = pBeginTagNext;
	}

	return TRUE;

ParseFailed:
	TextContext context;
	if (bFirstTag)
	{
		if (m_nAlign == -1) // �½�һ��
		{
			m_nAlign = XLine::LeftAlign;
			m_pContainer->NewLine(m_nAlign);
		}
	}
	// ˵��ǰ��Ľ����ɹ�
	if (pBeginTag != szLine)
		context.strText.insert(context.strText.begin(), pBeginTag, pEndTag+1);
	else
		context.strText	= szLine;
	AddText(context);
	return TRUE;
}
//------------------------------------------------------------------------
CTagParser* XLineParser::GetMatchTagParser(char* pBegin, char* pEnd)
{
	LPCSTR szTag = (LPCSTR)pBegin;
	vector<CTagParser*>::iterator it;
	vector<CTagParser*>::iterator _end = m_vTagParserList.end();
	for (it=m_vTagParserList.begin(); it!=_end; ++it)
	{
		if ((*it)->IsThisTag(szTag))
		{
			return (*it);
		}
	}
	return NULL;
}
//------------------------------------------------------------------------
// ���һ�����ı�
BOOL XLineParser::AddText(TextContext context)
{
	if (!m_pContainer)
	{
		TraceColorLn(MY_TRACE_COLOR, "XLineParser::AddText, m_pContainer is NULL");
		return FALSE;
	}

	ITTFont* pFont = XFontCreate::GetInstance()->CreateFont(context.strFontName.c_str(), context.nFontSize, 
		context.bBlod, context.bItalic, context.bUnderline, FALSE, 1024);

	// �½�һ���ı���Ŀ����������Ŀ����
	XItemText* pItem = new XItemText;
	pItem->SetText(context.strText.c_str());
	pItem->SetFont(pFont);
	pItem->SetTextColor(context.clrText);
	
	if (!m_pContainer->AddItem(static_cast<XItem*>(pItem), FALSE))
	{
		TraceColorLn(MY_TRACE_COLOR, "XLineParser::AddText, AddItem return FALSE");
		return FALSE;
	}
	
	return TRUE;
}
//------------------------------------------------------------------------