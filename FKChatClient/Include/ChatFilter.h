/**
*	created:		2012-6-28   6:45
*	filename: 		ChatFilter
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		���������
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include <string>
#include <hash_map>
#include <Windows.h>
using stdext::hash_map;
using std::string;
//------------------------------------------------------------------------
// ���������
class CChatFilter  
{
public:
	typedef BOOL (CChatFilter::*CMD_FUNC)(const char *);

public:
	CChatFilter(void);
	virtual ~CChatFilter(void);

public:
	string Filter(const char * pszText);
	BOOL OnPing(const char * pszText);
	
private:
	BOOL ParseCmd(string strText);	

private:
	typedef hash_map<string, CMD_FUNC> CmdMap;
	typedef hash_map<string, CMD_FUNC>::iterator CmdMapItr;
	typedef hash_map<string, CMD_FUNC>::const_iterator CmdMapCItr;
	
	CmdMap m_cmds;
};
//------------------------------------------------------------------------
// ����
struct SCmd
{
	const char * pName;
	CChatFilter::CMD_FUNC funcCmd;
};
//------------------------------------------------------------------------