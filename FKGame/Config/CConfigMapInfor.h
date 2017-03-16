/**
*	created:		2012-6-30   23:33
*	filename: 		CConfigMapInfor
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��ͼ��Ϣ����
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "../Script/IScriptUpdate.h"
#include "../Common/IConfigDataCenter.h"
//------------------------------------------------------------------------
// ��ͼ��Ϣ���ýű�����
#ifdef _DEBUG
#define MAPINFOR_CONFIG_SCRIPTNAME	"Data\\Scp\\mapinfor.scp"
#else
#define MAPINFOR_CONFIG_SCRIPTNAME	"Data\\Scp\\mapinfor.res"
#endif
//------------------------------------------------------------------------
class CConfigMapInfor : public IScriptUpdateSink
{
	typedef vector<MAP_INFOR>	TVECTOR_MAPINFOR;
public:
	// �ű�����ɹ�ʱ֪ͨ,ע��it�п���Ϊ��
	virtual BOOL	OnScriptLoad(IScriptIterator * it, IResScript * pResScript, LPCSTR szFile);
	
	// �ű���̬����ʱ֪ͨ,ע��it�п���Ϊ��
	virtual BOOL	OnScriptUpdate(IScriptIterator * it, IResScript * pResScript, LPCSTR szFile);
	
	// ����
	BOOL			Create(void);

	// ȡ�����е�ͼ����Ϣ, IN : pInfor �����ɵ�MAP_INFOR����, OUT : ��ǰ�ж����ŵ�ͼ
	BOOL			GetMapInfor(MAP_INFOR * pInfor, DWORD &dwCount);
	
	// ȡ��ĳһ�ŵ�ͼ����Ϣ
	MAP_INFOR *		GetMapInfor(DWORD dwMapID);
	
	// ȡ�õ�ǰ��ͼ����
	DWORD			GetMapCount(void);

private:
	TVECTOR_MAPINFOR	m_vectorMapInfor;
};
//------------------------------------------------------------------------