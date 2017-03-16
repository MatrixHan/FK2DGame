/**
*	created:		2012-6-19   21:11
*	filename: 		WindowChar
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
extern BLIT_FUNC _font_func[2][2];
//------------------------------------------------------------------------
class WChar  
{
private:
	WCharStruct m_objdata;
	int m_bOutline;

public:
	WChar();
	virtual ~WChar();
	BOOL Create(ITTFont* pFont, unsigned int c, WPixel color, WPixel outlinecolor=0);	// ������ɫĬ��Ϊ��ɫ

	int GetW() const;																	// ȡ���
	int GetH() const;																	// ȡ�߶�
	int GetWidth() const;																// ȡ�ֿ�
	BLIT_FUNC DrawFunc(DWORD style) const;												//���draw����ָ��
	WCharStruct* GetData(){return &m_objdata;}

};
//------------------------------------------------------------------------