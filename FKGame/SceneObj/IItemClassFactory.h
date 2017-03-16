/**
*	created:		2012-6-26   0:09
*	filename: 		IItemClassFactory
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		��ͼ���󴴽����๤���ӿ�
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
class CMapItem;
struct IArchive;
struct IResScript;
class CMapItemView;
struct ICanvas;
//------------------------------------------------------------------------
// �ͻ����๤��
struct IItemClassFactory
{
	// ���ض�������ӿ�ָ��
	virtual CMapItem* CreateItem(DWORD dwClassID, IArchive* pArchive) = NULL;
	virtual CMapItem* CreateItem(DWORD dwClassID, const char* buf, int nLen) = NULL;
	virtual void Release() = NULL;

	// �õ�basicsys���ص����нű�,ppAllScp=NULLʱ���������нŲ�����Ŀ
	virtual int GetAllScript(IResScript** ppAllScp) = NULL;
	// ���¼��ؽű�(�ʺ��ڵ�ͼ�༭������ͣ�ĸ��½ű�����Ч��������Ҫ�˳��༭���ؽ�)
	virtual int ReLoadScript(int nScpTypeID) = NULL;
	// ������壬���ڵ�ͼת��ʱ
	virtual void ClearItemBuffer() = NULL;

	//�����ǵ�½������ʾ����Ĵ���Ϊ�˱���ӿڵ��������࣬��Ҫ�Բ������ʽ��ʵ��
	//����ͼ��
	virtual BOOL ChangeModule (int nActIndex, int nIndex, DWORD dwResID, BYTE nPal = 0, BYTE nAlpha = 0) = NULL;
	virtual void OnDraw(int nActIndex, ICanvas* pCanvas, const POINT& ptTileCenter, DWORD dwFlag) = NULL;
	virtual void ReleasePreView(int nActIndex) = NULL;					//�ͷ�PreView��ͼ����Դ
	virtual void SetState(int nActIndex, int nState) = NULL;			//���ý�ɫ״̬
	virtual void CreatePreView (int nActIndex, int nType) = NULL;		//���ô����Ľ�ɫ����

	virtual BOOL InitFactory () = NULL;			//��ʼ��
};
//------------------------------------------------------------------------
// ���������๤��
struct IItemClassFactoryServer
{
	virtual CMapItem* CreateItem(DWORD dwClassID, DWORD dwResID) = NULL;
	virtual void Release() = NULL;
};
//------------------------------------------------------------------------
