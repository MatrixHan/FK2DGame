/**
*	created:		2012-6-20   19:53
*	filename: 		FontAsm
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "Asm.h"
//------------------------------------------------------------------------
WSERR _stdcall font16_alpha(WBmpStruct* bmp, const WCharStruct* c)
{
	int w,h,w4;
	__asm
	{
		mov ebx,c		; edx = char ָ��
		mov ecx,bmp		; Ŀ��ͼ
		#include "fontclip.inc"
						; ebx=char(Դ)ָ��,ecx=back(Ŀ��)ָ��
						; esi=Դ��ַ, edi=Ŀ���ַ
	_blit:
		movd mm7,[ebx]WCharStruct.color
		pcmpeqd mm6,mm6
		mov edx,[ecx]WBmpStruct.pitch	; Ŀ��ͼ pitch
		mov eax,w
		punpcklwd mm7,mm7
		sub eax,4
		punpckldq mm7,mm7		; mm7=color
		mov w4,eax
		and eax,3
		shl eax,4
		mov ebx,[ebx]WCharStruct.w	; Դͼ pitch
		movd mm5,eax
		cld
		psllq mm6,mm5
		xor ecx,ecx
		movq mask,mm6
		align 4
	_loop:	
		mov eax,0x10101010
		cmp ecx,w
		jl _loop_pixel
	_nextline:
		xor ecx,ecx
		add esi,ebx
		add edi,edx
		dec h
		jnz _loop
		emms
		jmp _end
		align 4
	_4pixel:
		movq [edi+ecx*2],mm7
	_skip4pixel:
		add ecx,4
		jmp _loop
		align 4
	_loop_pixel:
		cmp ecx,w4
		jle _skip_not_tail
		sub eax,[esi+ecx]
		jmp _begin_draw
	_skip_not_tail:
		sub eax,[esi+ecx]		; 16-alpha ֵ
		jz _4pixel
	_begin_draw:
		cmp eax,0x10101010
		jz _skip4pixel
		pxor mm4,mm4
		movd mm6,eax
		movq mm3,mm7
		punpcklbw mm6,mm4		; mm5 = 16-alpha ͨ��
		movq mm0,[edi+ecx*2]		; Ŀ������
		movq mm4,mm3
		movq mm5,mm3
		psrlw mm3,4
		pand mm4,mask_g
		pand mm5,mask_b
		movq mm1,mm0
		movq mm2,mm0
		psrlw mm0,4
		pand mm1,mask_g
		psubw mm0,mm3
		movq mm3,mm2
		pand mm2,mask_b
		psubw mm1,mm4
		pmullw mm0,mm6
		psubw mm2,mm5
		pmullw mm1,mm6
		paddw mm0,mm7
		pmullw mm2,mm6
		psrlw mm1,4
		psrlw mm2,4
		paddw mm1,mm4
		paddw mm2,mm5
		pand mm0,mask_r
		pand mm1,mask_g
		pand mm2,mask_b
		por mm0,mm1
		cmp ecx,w4
		por mm0,mm2
		jle _not_tail
		pand mm3,mask
		psubusw mm0,mask
		por mm0,mm3
		movq [edi+ecx*2],mm0
		jmp _nextline
	_not_tail:
		movq [edi+ecx*2],mm0
		add ecx,4
		jmp _loop
	_end:	
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------
WSERR _stdcall font16_noalpha(WBmpStruct* bmp, const WCharStruct* c)
{
	int w,h,w4;
	__asm
	{
		mov ebx,c		; ebx = char ָ��
		mov ecx,bmp		; Ŀ��ͼ
		#include "fontclip.inc"
						; ebx=char(Դ)ָ��,ecx=back(Ŀ��)ָ��
						; esi=Դ��ַ, edi=Ŀ���ַ
	_blit:
		movd mm7,[ebx]WCharStruct.color	; mm7=?? ?? ?? color
		pcmpeqd mm6,mm6					; mm6=FFFFFFFFFFFFFFFF
		mov edx,[ecx]WBmpStruct.pitch	; Ŀ��ͼ pitch
		mov eax,w						; eax=w
		punpcklwd mm7,mm7				; mm7=?? ?? color color
		sub eax,4						; eax=eax-4
		punpckldq mm7,mm7				; mm7=color color color color
		mov w4,eax						; w4=w-4
		and eax,3						; eax=(w-4)%4
		mov ecx,0x000c000c				; ecx=000C 000C
		shl eax,4						; eax=eax*16(�ֽ���ת��λ��)
		movd mm1,ecx					; mm1=000C 000C
		mov ebx,[ebx]WCharStruct.w		; Դͼ pitch
		movd mm5,eax					; mm5=eax
		punpckldq mm1,mm1				; mm1=0x000c000c000c000c
		psllq mm6,mm5					; mm6����ĩβ�����봦��
		pxor mm4,mm4					; mm4=0
		xor ecx,ecx						; ecx=0,���ڴ����˵����صļ���
		cld
		jmp _loop
		align 4
	_4pixel:							; ��4������
		movq [edi+ecx*2],mm7

	_skip4pixel:
		add ecx,4						; ecx=ecx+4
	_loop:	
		mov eax,0x10101010				; eax=10 10 10 10
		cmp ecx,w						; ecx<w?
		jl _loop_pixel
	_nextline:
		xor ecx,ecx
		add esi,ebx
		add edi,edx
		dec h
		jnz _loop
		emms
		jmp _end
		align 4
	_loop_pixel:
		cmp ecx,w4						; ecx<=w4?
		jle _skip_not_tail
		sub eax,[esi+ecx]
		jmp _begin_draw
	_skip_not_tail:
		sub eax,[esi+ecx]				; 16-alpha ֵ
		jz _4pixel						; 
	_begin_draw:
		cmp eax,0x10101010				; eax==0x10101010?
		jz _skip4pixel
		movd mm5,eax					; mm5=?? ?? ?? ?? eax
		movq mm2,mm7					; mm2=color color color color
		punpcklbw mm5,mm4				; mm5=16-alpha ͨ��
		movq mm0,[edi+ecx*2]			; mm0=Ŀ������
		pcmpgtw mm5,mm1					; mm5=Ҫ����������0��䣬������FF���
		psubusw mm2,mm5					; mm2��mm5�෴
		pand mm5,mm0					; mm5=Ŀ��������Ҫ���ַ���������0���
		cmp ecx,w4						; ecx<=w4?
		por mm5,mm2						; mm5=��Ϻ�Ľ��
		jle _not_tail
		pand mm0,mm6
		psubusw mm5,mm6
		por mm0,mm5
		movq [edi+ecx*2],mm0
		jmp _nextline
	_not_tail:
		movq [edi+ecx*2],mm5			; ��Ϻ�Ľ��д��Ŀ��
		jmp _skip4pixel
	_end:	
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------
WSERR _stdcall font16_noalpha_with_outline(WBmpStruct* bmp, const WCharStruct* c)
{
	static __int64 _pixel = 0x0010001000100010;
	static __int64 _outline = 0x00FF00FF00FF00FF;
	int w,h,w4;
	__asm
	{
		mov ebx,c		; ebx = char ָ��
		mov ecx,bmp		; Ŀ��ͼ
		#include "fontclip.inc"
						; ebx=char(Դ)ָ��,ecx=back(Ŀ��)ָ��
						; esi=Դ��ַ, edi=Ŀ���ַ
	_blit:
		movd mm7,[ebx]WCharStruct.color	; mm7=?? ?? ?? color
		pcmpeqd mm6,mm6					; mm6=FFFFFFFFFFFFFFFF
		mov edx,[ecx]WBmpStruct.pitch	; edx=Ŀ��ͼpitch
		mov eax,w						; eax=w
		punpcklwd mm7,mm7				; mm7=?? ?? color color
		sub eax,4						; eax=eax-4
		punpckldq mm7,mm7				; mm7=color color color color
		mov w4,eax						; w4=w-4
		and eax,3						; eax=(w-4)%4
		mov ecx,0x00ff00ff				; ecx=000C 000C
		shl eax,4						; eax=eax*16(�ֽ���ת��λ��)
		movd mm3,[ebx]WCharStruct.outline;mm3=?? ?? ?? clr
		movd mm1,ecx					; mm1=000C 000C
		mov ebx,[ebx]WCharStruct.w		; ebx=Դͼpitch
		punpcklwd mm3,mm3				; mm7=?? ?? clr clr
		movd mm5,eax					; mm5=eax
		punpckldq mm1,mm1				; mm1=0x000c000c000c000c
		psllq mm6,mm5					; mm6����ĩβ�����봦��
		pxor mm4,mm4					; mm4=0
		punpckldq mm3,mm3				; mm3=clr clr clr clr(������ɫ��Ĭ���ú�ɫ���Ժ�ɸ�)
		xor ecx,ecx						; ecx=0,���ڴ����˵����صļ���
		cld
		jmp _loop
		align 4
	_4pixel:							; ��4������
		movq [edi+ecx*2],mm7
		jmp _skip4Pixel
	_4outline:
		movq [edi+ecx*2],mm3
	_skip4pixel:
		add ecx,4						; ecx=ecx+4
	_loop:	
		cmp ecx,w						; ecx<w?
		jl _loop_pixel
	_nextline:
		xor ecx,ecx
		add esi,ebx
		add edi,edx
		dec h
		jnz _loop
		emms
		jmp _end
		align 4
	_loop_pixel:
		cmp ecx,w4						; ecx<=w4?
		jle _skip_not_tail
		mov eax,[esi+ecx]
		jmp _begin_draw
	_skip_not_tail:
		mov eax,[esi+ecx]
		cmp eax,0x10101010
		jz _4pixel						; 
		cmp eax,0xffffffff
		jz _4outline
	_begin_draw:
		cmp eax,0x00000000				; eax==0x10101010?
		jz _skip4pixel

		// outline
		movd mm5,eax					; mm5=?? ?? ?? ?? eax
		movq mm2,mm3					; mm2=color color color color
		punpcklbw mm5,mm4				; mm5=16-alpha ͨ��0
		movq mm0,[edi+ecx*2]			; mm0=Ŀ������
		movq mm4,mm5
		pcmpeqd mm1,mm1
		pcmpeqw mm5,_outline			; mm5=Ҫ����������0��䣬������FF���
		psubusw mm1,mm5

		psubusw mm2,mm1					; mm2=����Ҫ���Ƶ�������0���
		pand mm1,mm0					; mm5=Ŀ��������Ҫ���ַ���������0���
		por mm1,mm2						; mm5=��Ϻ�Ľ��
		
		// pixel
		pcmpeqd mm2,mm2
		pcmpeqw mm4,_pixel				; mm5=Ҫ����������0��䣬������FF���
		psubusw mm2,mm4
		pand mm1,mm2
		pand mm4,mm7					; mm2=color color color color

		cmp ecx,w4						; ecx<=w4?
		por mm1,mm4
		pxor mm4,mm4
		jle _not_tail
		pand mm0,mm6
		psubusw mm1,mm6
		por mm0,mm1
		movq [edi+ecx*2],mm0
		jmp _nextline
	_not_tail:
		movq [edi+ecx*2],mm1			; ��Ϻ�Ľ��д��Ŀ��
		jmp _skip4pixel
	_end:	
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------