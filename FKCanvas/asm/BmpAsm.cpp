/**
*	created:		2012-6-20   19:40
*	filename: 		BmpAsm
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "Asm.h"
//------------------------------------------------------------------------
__int64 mask = 0xf81ff81ff81ff81f;
__int64 xffdf = 0xffdfffdfffdfffdf;
__int64 mask_r = 0xf800f800f800f800;
__int64 mask_g = 0x07e007e007e007e0;
__int64 mask_b = 0x001f001f001f001f;
__int64 mask_alpha;
__int64 alpha;
//------------------------------------------------------------------------
// 16bitsλͼ����
WSERR _stdcall blit16(WBmpStruct* dest, const WBmpStruct* src)
{
	int w,h;
	__asm
	{
		mov ecx, dest					; ����Ŀ��ָ��
		mov ebx, src					; ����Դָ��
		#include "bmpclip.inc"
										; esi=Դ��ַ, edi=Ŀ���ַ
_blit:
		mov eax,w						; eax<--w
		mov edx,[ecx]WBmpStruct.pitch	; Ŀ��pitch
		add eax,eax						; eax=2*w
		mov ebx,[ebx]WBmpStruct.pitch	; Դpitch
		sub edx,eax						; edx=Ŀ��pitch-2*w
		sub ebx,eax						; ebx=Դpitch-2*w
		mov ecx,w						; ecx<--w
		mov eax,h						; eax<--h
		cld								; �����־����Ϊָ���������
		align 4
_loop:	
		sub ecx,4						; w=w-4
		jle _nextline					; w<=0?
		movq mm0,[esi]					; һ�ζ�4������(8���ֽ�)��mm0
		add esi,8						; Դָ���ƶ�8���ֽ�
		movq [edi],mm0					; һ��д4������(8���ֽ�)��Ŀ�괦
		add edi,8						; Ŀ��ָ���ƶ�8���ֽ�
		jmp _loop
_nextline:
		add ecx,4						; ecx��ʱ����ÿ��ʣ���������Ŀ
		rep movsw						; ���ecx=2����ʣ�����������
		add esi,ebx						; Դָ��ָ����һ��
		add edi,edx						; Ŀ��ָ��ָ����һ��
		mov ecx,w						; ecx<--w
		dec eax							; h=h-1
		jnz _loop
_end:
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------
// ��λͼ��Ϊcolorɫ
WSERR _stdcall clear16(WBmpStruct* bmp, WPixel color)
{
	__asm
	{
		mov ecx,bmp						; ����bmpָ��
		mov esi,[ecx]WBmpStruct.w		; esi<--w
		mov edi,[ecx]WBmpStruct.ptr		; edi<--ptr
		test esi,esi					; w==0?
		jnz _can_blit					; w!=0��blit
		jmp _end						; w==0����blit
		
_can_blit:
		mov ax,color					; ax<--color
		mov edx,[ecx]WBmpStruct.pitch	; edx<--pitch
		movd mm0,eax					; mm0<--color
		add esi,esi						; w*2
		punpcklwd mm0,mm0				; mm0<--c_c
		sub edx,esi						; edxΪ�м��:pitch-2*w
		punpckldq mm0,mm0				; mm0<--c_c_c_c
		cld								; �����־����Ϊָ���������
		mov esi,[ecx]WBmpStruct.h		; esi<--h
		movd mm2,[ecx]WBmpStruct.w		; mm2<--w
		test esi,esi					; h==0?
		jz _end							; h==0��
		movd ecx,mm2					; ecx<--w
		align 4
_loop:
		sub ecx,4						; w=w-4
		jle _nextline					; w<=0?
		movq [edi],mm0					; һ��д4������(8���ֽ�)��Ŀ�괦
		add edi,8						; Ŀ��ָ���ƶ�8���ֽ�
		jmp _loop
_nextline:
		add ecx,4						; ecx��ʱ����ÿ��ʣ���������Ŀ
		rep stosw						; ���ecx=2����ʣ�����������
		add edi,edx						; Ŀ��ָ��ָ����һ��
		movd ecx,mm2					; ecx<--w
		dec esi							; h=h-1
		jnz _loop
_end:
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------
// 16bitsλͼ����(���͸��ɫ)
WSERR _stdcall mask_blit16(WBmpStruct* dest,const WBmpStruct* src)
{
	int w,h;
	__asm
	{
		mov ecx,dest					; ����destָ��
		mov ebx,src						; ����srcָ��
		#include "bmpclip.inc"
										; ebx=src(Դ)ָ��,ecx=dest(Ŀ��)ָ��
										; esi=Դ��ַ, edi=Ŀ���ַ
_blit:
		mov eax,w						; eax<--w
		mov edx,[ecx]WBmpStruct.pitch	; Ŀ��pitch
		add eax,eax						; w*2
		mov ebx,[ebx]WBmpStruct.pitch	; Դpitch
		sub edx,eax						; edx<--Ŀ��pitch-2*w
		sub ebx,eax						; ebx<--Դpitch-2*w
		mov ecx,w						; ecx<--w
		cld								; �����־����Ϊָ���������
		movq mm7,mask					; mm7Ϊ��ɫ
		xor eax,eax						; eax<--0
		align 4
_loop:	
		sub ecx,4			; w=w-4
		jle _nextline		; w<=0?
		movq mm0,[esi]		; һ�ζ�4������(8���ֽ�)��mm0,����Ϊ0xf81faabbf81fccdd;
		add esi,8			; Դָ���ƶ�8���ֽ�
		movq mm1,mm0		; ���ݵ�mm1
		pcmpeqw mm0,mm7		; �Ƚ��Ƿ���͸��ɫ,�ǵĲ���Ϊ0xff,���ǵ�Ϊ0x00,mm0=0xffff0000ffff0000
		psubusw mm1,mm0		; �ٳ���͸���Ĳ��֣�����͸���Ĳ���Ϊ0,mm1=0x0000aabb0000ccdd
		pand mm0,[edi]		; ����Ŀ������,����[edi]=0x1122334455667788,��mm0=0x1122000055660000
		por mm0,mm1			; Ŀ����Դ���,mm0=0x1122aabb5566ccdd
		movq [edi],mm0		; һ��д4������(8���ֽ�)��Ŀ�괦
		add edi,8			; Ŀ��ָ���ƶ�8���ֽ�
		jmp _loop
_nextline:
		add ecx,4			; ecx��ʱ����ÿ��ʣ���������Ŀ
_looptail:
		lodsw				; ��ȡ2���ֽڵ�eax
		cmp eax,0xf81f		; �Ƚ�͸��ɫ
		jz _skip_pixel		; ͸��ɫ������������
		mov [edi],ax		; ��͸��ɫ��blit
_skip_pixel:
		add edi,2			; Ŀ��ָ���ƶ�2���ֽ�
		dec ecx				; һ��ʣ���������Ŀ
		jnz _looptail		; ��Ҫʣ������������
		add esi,ebx			; Դָ��ָ����һ��
		add edi,edx			; Ŀ��ָ��ָ����һ��
		mov ecx,w			; ecx<--w
		dec h				; h=h-1
		jnz _loop
_end:
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------
WSERR _stdcall additive_blit16(WBmpStruct* dest,const WBmpStruct* src)
{
	int w,h;
	__asm
	{
		mov ecx,dest		; ���� this ָ��
		mov ebx,src			; edx = bmp ָ��
		#include "bmpclip.inc"
							; ebx=src(Դ)ָ��,ecx=dest(Ŀ��)ָ��
							; esi=Դ��ַ, edi=Ŀ���ַ
_blit:
		mov eax,w
		mov edx,[ecx]WBmpStruct.pitch	; Ŀ��ͼ pitch
		add eax,eax
		mov ebx,[ebx]WBmpStruct.pitch	; Դͼ pitch
		sub edx,eax
		sub ebx,eax
		mov ecx,eax
		pcmpeqb mm7,mm7
		shr ecx,3
		cld
		and eax,7
		jnz _notzero
		mov eax,8
		dec ecx
_notzero:
		shl eax,3
		movd mm6,eax
		shr eax,3
		psllq mm7,mm6
		mov w,ecx
		add ebx,eax
		add edx,eax
		align 4
_loop:	
		dec ecx
		jl _nextline 
		movq mm0,[esi]
		movq mm3,[edi]
		movq mm1,mm0
		movq mm2,mm0
		movq mm4,mm3
		movq mm5,mm3
		psllw mm1,5
		psllw mm4,5
		psllw mm2,11
		psllw mm5,11
		paddusw mm0,mm3			; ��ɫ��
		paddusw mm1,mm4			; ��ɫ��
		paddusw mm2,mm5			; ��ɫ��
		psrlw mm1,5                                     
		psrlw mm2,11                                    
		pand mm0,mask_r
		pand mm1,mask_g
		por mm0,mm2
		por mm0,mm1
		add esi,8
		movq [edi],mm0
		add edi,8
		jmp _loop
_nextline:
		movq mm0,[esi]
		psubusb mm0,mm7
		movq mm3,[edi]
		movq mm1,mm0
		movq mm2,mm0
		movq mm4,mm3
		movq mm5,mm3
		psllw mm1,5
		psllw mm4,5
		psllw mm2,11
		psllw mm5,11
		paddusw mm0,mm3			; ��ɫ��
		paddusw mm1,mm4			; ��ɫ��
		paddusw mm2,mm5			; ��ɫ��
		psrlw mm1,5                                     
		psrlw mm2,11                                    
		pand mm0,mask_r
		pand mm1,mask_g
		por mm0,mm2
		por mm0,mm1
		add esi,ebx
		movq [edi],mm0
		add edi,edx
		mov ecx,w
		dec h
		jz _end
		jmp _loop
_end:
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------
WSERR _stdcall subtract_blit16(WBmpStruct* dest,const WBmpStruct* src)
{
	int w,h;
	__asm
	{
		mov ecx,dest					; ���� dest ָ��
		mov ebx,src						; edx = src ָ��
		#include "bmpclip.inc"
										; ebx=bmp(Դ)ָ��,ecx=this(Ŀ��)ָ��
										; esi=Դ��ַ, edi=Ŀ���ַ
_blit:
		mov eax,w
		mov edx,[ecx]WBmpStruct.pitch	; Ŀ��ͼ pitch
		add eax,eax
		mov ebx,[ebx]WBmpStruct.pitch	; Դͼ pitch
		sub edx,eax
		sub ebx,eax
		mov ecx,eax
		pcmpeqb mm7,mm7
		shr ecx,3
		cld
		and eax,7
		jnz _notzero
		mov eax,8
		dec ecx
_notzero:
		shl eax,3
		movd mm6,eax
		shr eax,3
		psllq mm7,mm6
		mov w,ecx
		add ebx,eax
		add edx,eax
		align 4
_loop:	
		dec ecx
		jl _nextline 
		movq mm0,[edi]
		movq mm3,[esi]
		movq mm1,mm0
		movq mm2,mm0
		movq mm4,mm3
		movq mm5,mm3
		psllw mm1,5
		psllw mm4,5
		psllw mm2,11
		psllw mm5,11
		psubusw mm0,mm3			; ��ɫ��
		psubusw mm1,mm4			; ��ɫ��
		psubusw mm2,mm5			; ��ɫ��
		psrlw mm1,5                                     
		psrlw mm2,11                                    
		pand mm0,mask_r
		pand mm1,mask_g
		por mm0,mm2
		por mm0,mm1
		add esi,8
		movq [edi],mm0
		add edi,8
		jmp _loop
_nextline:
		movq mm3,[esi]
		movq mm0,[edi]
		psubusb mm3,mm7
		movq mm1,mm0
		movq mm2,mm0
		movq mm4,mm3
		movq mm5,mm3
		psllw mm1,5
		psllw mm4,5
		psllw mm2,11
		psllw mm5,11
		psubusw mm0,mm3			; ��ɫ��
		psubusw mm1,mm4			; ��ɫ��
		psubusw mm2,mm5			; ��ɫ��
		psrlw mm1,5                                     
		psrlw mm2,11                                    
		pand mm0,mask_r
		pand mm1,mask_g
		por mm0,mm2
		por mm0,mm1
		add esi,ebx
		movq [edi],mm0
		add edi,edx
		mov ecx,w
		dec h
		jz _end
		jmp _loop
_end:
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------
// 16bitsλͼ��͸������
WSERR _stdcall trans_blit16(WBmpStruct* dest,const WBmpStruct* src)
{
	int w,h;
	__asm
	{
		mov ebx,src							; srcԴָ��
		mov ecx,dest						; destĿ��ָ��
		#include "bmpclip.inc"
											; ebx=src(Դ)ָ��,ecx=dest(Ŀ��)ָ��
											; esi=Դ��ַ, edi=Ŀ���ַ
_blit:
		mov eax,0x01000100					; eax<--0x0100 0100
		movd mm5,[ebx]WBmpStruct.userdata	; mm5<--alpha,����alpha=127=0x7f
		movd mm7,eax						; mm7<--0x0100 0100
		punpcklwd mm5,mm5					; mm5=0x0000 0000 007f 007f
		punpckldq mm7,mm7					; mm7=0x0100 0100 0100 0100
		punpckldq mm5,mm5					; mm5=0x007f 007f 007f 007f
		movq mm6,mm7						; mm6=mm7=0x100 0100 0100 0100
		mov eax,w							; eax<--w
		mov edx,[ecx]WBmpStruct.pitch		; Ŀ��pitch
		add eax,eax							; w*2
		mov ebx,[ebx]WBmpStruct.pitch		; Դpitch
		sub edx,eax							; edxΪĿ����м��:Ŀ��pitch-2*w
		sub ebx,eax							; ebxΪԴ���м��:Դpitch-2*w
		mov ecx,eax							; ecx<--w*2
		pcmpeqb mm0,mm0						; mm0=0xffffffffffffffff
		shr ecx,3							; ecx<--w*2/8=w/4,һ��blend4�����ص�ѭ������
		cld									; �����־����Ϊָ���������
		and eax,7							; �õ�8�ֽڶ����ʣ����ֽ���,����eax=2
		jnz _notzero						; ת��������0���������
		mov eax,8							; eax<--8
		dec ecx								; blendѭ��������1
_notzero:
		shl eax,3							; eax<--ʣ���ֽڵ�λ��,eax=16
		movd mm4,eax						; mm4<--ʣ���ֽڵ�λ��,mm0=16
		shr eax,3							; eax<--ʣ���ֽ���,eax=2
		psllq mm0,mm4						; mm0����mm4(16)λ,mm0=0xffffffffffff0000
		mov w,ecx							; w=blendѭ������
		pandn mm0,mm5						; mm0=0x0000 0000 0000 007f
		add ebx,eax							; 
		psubw mm6,mm0						; mm6=0x0100 0100 0100 0081
		psubw mm7,mm5						; mm7=0x0081 0081 0081 0081
		psrlw mm6,3							; mm6/8,mm6=0x0020 0020 0020 0010
		psrlw mm7,3							; mm7/8,mm7=0x0010 0010 0010 0010
		add edx,eax							; 
		align 4
_loop:	
		dec ecx								; ѭ��������1
		jge _blend4							; blend4������
		jmp _nextline						; 
_blend4:
		movq mm3,[esi]						; ��Դ��ȡ4������r1g1b1_r2g2b2_r3g3b3_r4g4b4
		movq mm0,[edi]						; ��Ŀ���ȡ4������R1G1B1_R2G2B2_R3G3B3_R4G4B4

		movq mm4,mm3						; mm4<--mm3
		movq mm5,mm3						; mm5<--mm3
		psrlw mm3,5							; ������ɫ�Ƴ�mm3=0r1g1_0r2g2_0r3g3_0r4g4
		pand mm4,mask_g						; �õ���ɫ����mm4=0g10_0g20_0g30_0g40
		pand mm5,mask_b						; �õ���ɫ����mm5=00b1_00b2_00b3_00b4

		movq mm1,mm0						; mm1<--mm0
		movq mm2,mm0						; mm2<--mm0
		psrlw mm0,5							; ������ɫ�Ƴ�mm0=0R1G1_0R2G2_0R3G3_0R4G4
		pand mm1,mask_g						; �õ���ɫ����mm1=0G10_0G20_0G30_0G40
		pand mm2,mask_b						; �õ���ɫ����mm2=00B1_00B2_00B3_00B4
		
		psubw mm0,mm3						; mm0=(0R1G1-0r1g1)_(0R2G2-0r2g2)_(0R3G3-0r3g3)_(0R4G4-0r4g4)
		psubw mm1,mm4						; mm1=(0G10-0g10)_(0G20-0g20)_(0G30-0g30)_(0G40-0g40)
		pmullw mm0,mm7						; ��˺�ȡ��λ
		psubw mm2,mm5						; mm2=(00B1-00b1)_(00B2-00b2)_(00B3-00b3)_(00B4-00b4)
		pmullw mm1,mm7						; ��˺�ȡ��λ
		paddw mm0,[esi]						; 
		pmullw mm2,mm7						; ��˺�ȡ��λ
		psrlw mm1,5							; 
		psrlw mm2,5							; 
		paddw mm1,mm4						; 
		paddw mm2,mm5						; 
		pand mm0,mask_r						; �������ɫ����
		pand mm1,mask_g						; �������ɫ����
		pand mm2,mask_b						; �������ɫ����
		por mm0,mm1							; mm0=���̻�ɫ
		por mm0,mm2							; mm0=��������ɫ
		add esi,8							; Դָ���ƶ�8���ֽ�
		movq [edi],mm0						; д��ϳɺ��4������
		add edi,8							; Ŀ��ָ���ƶ�8���ֽ�
		jmp _loop
_nextline:
		movq mm3,[esi]
		movq mm0,[edi]
		movq mm4,mm3
		movq mm5,mm3
		psrlw mm3,5
		pand mm4,mask_g
		pand mm5,mask_b
		movq mm1,mm0
		movq mm2,mm0
		psrlw mm0,5
		pand mm1,mask_g
		pand mm2,mask_b
		psubw mm0,mm3
		psubw mm1,mm4
		pmullw mm0,mm6
		psubw mm2,mm5
		pmullw mm1,mm6
		paddw mm0,[esi]
		pmullw mm2,mm6
		psrlw mm1,5
		psrlw mm2,5
		paddw mm1,mm4
		paddw mm2,mm5
		pand mm0,mask_r
		pand mm1,mask_g
		pand mm2,mask_b
		por mm0,mm1
		por mm0,mm2
		add esi,ebx
		movq [edi],mm0
		add edi,edx
		mov ecx,w
		dec h
		jz _end
		jmp _loop
_end:	
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------
// 16bitsλͼ��͸������(���͸��ɫ)
WSERR _stdcall mask_trans_blit16(WBmpStruct* dest,const WBmpStruct* src)
{
	int w,h;
	__asm
	{
		mov ebx,src		; srcԴ ָ��
		mov ecx,dest	; dest ָ��
		#include"bmpclip.inc"
						; ebx=src(Դ)ָ��,ecx=dest(Ŀ��)ָ��
						; esi=Դ��ַ, edi=Ŀ���ַ
	_blit:
		mov eax,0x1000100
		movd mm5,[ebx]WBmpStruct.userdata
		movd mm7,eax
		punpcklwd mm5,mm5
		punpckldq mm7,mm7
		punpckldq mm5,mm5		; mm5=alpha
		movq mm6,mm7			; mm6=mm7= 0x100 0100 0100 0100
		mov eax,w
		mov edx,[ecx]WBmpStruct.pitch	; Ŀ��ͼ pitch
		add eax,eax
		mov ebx,[ebx]WBmpStruct.pitch	; Դͼ pitch
		sub edx,eax
		sub ebx,eax
		mov ecx,eax
		pcmpeqb mm0,mm0
		shr ecx,3
		cld
		and eax,7
		jnz _notzero
		mov eax,8
		dec ecx
_notzero:
		shl eax,3
		movd mm4,eax
		shr eax,3
		psllq mm0,mm4
		mov w,ecx
		pandn mm0,mm5
		add ebx,eax
		psubw mm6,mm0
		psubw mm7,mm5
		psrlw mm6,3
		psrlw mm7,3
		add edx,eax
		paddusw mm7,xffdf
		paddusw mm6,xffdf
		movq alpha,mm7
		movq mask_alpha,mm6
		align 4
_loop:	
		dec ecx
		jge _blend4
		jmp _nextline
_blend4:
		movq mm1,mask
		movq mm3,[esi]
		movq mm0,[edi]
		movq mm7,alpha
		pcmpeqw mm1,mm3
		movq mm4,mm3
		paddusw mm7,mm1
		movq mm5,mm3
		psubusw mm7,xffdf

		psrlw mm3,5
		pand mm4,mask_g
		pand mm5,mask_b
		movq mm1,mm0
		movq mm2,mm0
		psrlw mm0,5
		pand mm1,mask_g
		pand mm2,mask_b
		psubw mm0,mm3
		psubw mm1,mm4
		pmullw mm0,mm7
		psubw mm2,mm5
		pmullw mm1,mm7
		paddw mm0,[esi]
		pmullw mm2,mm7
		psrlw mm1,5
		psrlw mm2,5
		paddw mm1,mm4
		paddw mm2,mm5
		pand mm0,mask_r
		pand mm1,mask_g
		pand mm2,mask_b
		por mm0,mm1
		por mm0,mm2
		add esi,8
		movq [edi],mm0
		add edi,8
		jmp _loop
_nextline:
		movq mm1,mask
		movq mm3,[esi]
		movq mm0,[edi]
		movq mm6,mask_alpha
		pcmpeqw mm1,mm3
		movq mm4,mm3
		paddusw mm6,mm1
		movq mm5,mm3
		psubusw mm6,xffdf

		psrlw mm3,5
		pand mm4,mask_g
		pand mm5,mask_b
		movq mm1,mm0
		movq mm2,mm0
		psrlw mm0,5
		pand mm1,mask_g
		pand mm2,mask_b
		psubw mm0,mm3
		psubw mm1,mm4
		pmullw mm0,mm6
		psubw mm2,mm5
		pmullw mm1,mm6
		paddw mm0,[esi]
		pmullw mm2,mm6
		psrlw mm1,5
		psrlw mm2,5
		paddw mm1,mm4
		paddw mm2,mm5
		pand mm0,mask_r
		pand mm1,mask_g
		pand mm2,mask_b
		por mm0,mm1
		por mm0,mm2

		add esi,ebx
		movq [edi],mm0
		add edi,edx
		mov ecx,w
		dec h
		jz _end
		jmp _loop
_end:	
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------
// ͬһ16λλͼbmp blit ��������(y<0)
WSERR _stdcall move16(WBmpStruct* bmp, const int x, const int y)
{
	int w;
	__asm
	{
		mov ebx, bmp
		mov edi, [ebx]WBmpStruct.ptr
		mov esi, [ebx]WBmpStruct.ptr	
		mov eax, x
		cmp eax, 0
		jg  _fgl
		neg eax
		add eax, eax
		add esi, eax
		mov eax, [ebx]WBmpStruct.pitch
		mov edx, y
		neg edx
		mul edx
		add esi, eax
		mov eax, x
		neg eax
		;add eax, eax
		jmp _fbegin
_fgl:				;(x > 0)
		add eax, eax
		add edi, eax
		mov eax, [ebx]WBmpStruct.pitch
		mov edx, y
		neg edx
		mul edx
		add esi, eax
		mov eax, x
		;add eax, eax
_fbegin:
		mov ecx, [ebx]WBmpStruct.w
		sub ecx, eax
		mov w, ecx
		mov edx, eax
		add edx, edx
		mov eax, [ebx]WBmpStruct.h
		add eax, y
		;cmp eax, 0
		;jz _end
		cld
		align 4
_floop:
		sub ecx, 4
		jle _fnextline
		movq mm0, [esi]
		add esi, 8
		movq [edi], mm0
		add edi, 8
		jmp _floop
_fnextline:
		add ecx, 4
		rep movsw
		add esi, edx
		add edi, edx
		mov ecx, w
		dec eax
		jnz _floop
// _end:
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------
// ͬһ16λλͼbmp blit ��������(y>0)
WSERR _stdcall back_move16(WBmpStruct* bmp, const int x, const int y)
{
	int w,t;
	__asm
	{
		mov ebx, bmp
		mov edi, [ebx]WBmpStruct.ptr
		mov esi, [ebx]WBmpStruct.ptr
		
		mov eax, [ebx]WBmpStruct.pitch
		mov edx, [ebx]WBmpStruct.h
		mul edx
		add esi, eax
		add edi, eax
		mov eax, [ebx]WBmpStruct.pitch
		mov edx, y
		mul edx
		sub esi, eax
		mov eax, x
		cmp eax, 0
		jg  _bgl
		neg eax
		add eax, eax
		sub edi, eax
		mov eax, x
		neg eax
		jmp _bbegin
		
_bgl:
		add eax, eax
		sub esi, eax
		mov eax, x
		
_bbegin:
		mov ecx, [ebx]WBmpStruct.w
		sub ecx, eax
		mov w, ecx
		mov edx, eax
		add edx, edx
		mov eax, [ebx]WBmpStruct.h
		sub eax, y
		;cmp eax, 0
		;jz _end
		cld
		align 4
		
_bloop:
		sub ecx, 4
		jle _bnextline
		sub esi, 8
		movq mm0, [esi]
		sub edi, 8
		movq [edi], mm0
		jmp _bloop
_bnextline:
		add ecx, 4
		jz _nadjust
		mov t, ecx
		sub esi, ecx
		sub esi, ecx
		sub edi, ecx
		sub edi, ecx
		rep movsw
		sub esi, t
		sub esi, t
		sub edi, t
		sub edi, t
_nadjust:
		sub esi, edx
		sub edi, edx
		;sub esi, [ebx]WBmpStruct.pitch
		;sub esi, [ebx]WBmpStruct.pitch
		;sub edi, [ebx]WBmpStruct.pitch
		;sub edi, [ebx]WBmpStruct.pitch
		mov ecx, w
		dec eax
		jnz _bloop
		
// _end:
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------