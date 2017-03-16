	;
;                ..;;;;,                     ;;;,    
;            ..;;;"` ;;:           ,;;;;;: ,;;;:.,;..          _/
;      ,;..,;;"`    :;;'            :;;"`,;;;;;;"":;;         _/ 
;      ;;;"  `:;;. ;;'           ..;;:  .;;,.;;:',;;"    _/_/_/_/_/
;     .;;`   ,;;" .;"          ,;;;;;;" :;`',;",;;"         _/
;    ,;;,:.,;;;  ,;:          :" ,;:` , `:.;;;;;'`         _/   
;    ;;"'':;;:. .;; .          ,;;;,;:;;,;;;, ,;             _/
;   :;;..;;;;;; :;' :.        :;;;"` `:;;;;;,;,.;.          _/
; .;;":;;`  '"";;:  ';;       '""   .;;`.;";:;;;;` ;,  _/_/_/_/_/
;;;;" `'       "::. ,;;:          .;"`  ::. '   .,;;;     _/ 
;""             ';;;;;;;"        ""     ';;;;;;;;;;`     _/
;
;                       Windsoul++
;
;               by �Ʒ� (Cloud Wu)  1999-2001
;
;       http://member.netease.com/~cloudwu 
;		mailto:cloudwu@263.net
; 
;		���Ķ� readme.txt �еİ�Ȩ��Ϣ
;		See readme.txt for copyright information.
;
;		Description:		16λAlphaλͼ Blit
;		Original Author:	�Ʒ�
;		Authors:
;		Create Time:		2001/3/10
;		Modify Time:
;
;:*W*:._.:*I*:_.:*N*:._.:*D*:._.:*S*:._.:*O*:._.:*U*:._.:*L*:._.:

[bits 32]

%include "asm/c32.mac"
%include "asm/wpp.inc"
%include "asm/bmpclip.mac"

[section .text class=code use32]

; �� 16 λ Alpha λͼ bmp BLIT �� this λͼ��
;WSERR _stdcall _alpha_blit16(WBmpStruct *,const WAlphaBmpStruct *);
function _alpha_blit16
proc __alpha_blit16@8
	saveesi
	saveedi
	saveebx
	saveebp
%$this	arg
%$bmp	arg
%$w	locvar
%$h	locvar
%$w4	locvar
	endlocvar

	mov ebx,var(%$bmp)				; ebx = bmp ָ��(AlphaBitmap)
	mov ecx,var(%$this)				; ���� this ָ��(Bitmap)
	mov ebp,[ebx+WBmpStruct.ptr]	; ebpΪԴλͼ������ָ��
	bmpclip	16						; ���ô���(16 bit)
									; ebx=bmp(Դ)ָ��,ecx=this(Ŀ��)ָ��
									; esi=Դ��ַ, edi=Ŀ���ַ
.blit:
	pcmpeqd mm6,mm6					; mm6=0xFFFFFFFFFFFFFFFF
	sub ebp,esi						; ebp=Դλͼ������ָ��(ebp)��ʵ�ʻ�ͼʱ����ʼָ��(esi)��ƫ��,ebp<=0
	mov edx,[ecx+WBmpStruct.pitch]	; edx=Ŀ��ͼ pitch
	neg ebp							; ebp>=0
	mov eax,var(%$w)				; eax=w
	shr ebp,1						; ebp=Դλͼ������ָ��ƫ�Ƶ�1/2,���ڶ�λͨ����ʵ��ָ��
	sub eax,4						; eax=w-4
	xor ecx,ecx						; ecx=0,�ں������ڿ�ȼ���
	mov var(%$w4),eax				; w4=w-4
	and eax,3						; eax=(w-4)/4������
	add ebp,[ebx+WAlphaBmpStruct.channel]	; Դλͼͨ����ʵ�ʻ�ͼָ��
	shl eax,4								; eaxΪ��βʣ��������Ŀ����Ӧ��λ��
	mov ebx,[ebx+WBmpStruct.pitch]	; ebx=Դͼ pitch
	movd mm5,eax					; mm5=eax
	sar ebx,1						; ebx=Դͼ pitch/2�����ں����ͨ��ָ�����
	psllq mm6,mm5					; mm6������β���ò���4������ʱ���вü�
	cld								; ����ָ��Ϊ����ģʽ
	align 4
.loop:	
	mov eax,0xffffffff				; eax=0xffffffff
	cmp ecx,var(%$w)				; ecx<w?
	jl .loop_pixel					; ecx<w
.nextline:							; ������һ��
	xor ecx,ecx						; ecx=0 (��ȼ���)
	add ebp,ebx						; ͨ��ָ��ı䵽��һ��
	lea esi,[esi+ebx*2]				; Դλͼ����ָ��ı䵽��һ��
	add edi,edx						; Ŀ��λͼ����ָ��ı䵽��һ��
	dec dword var(%$h)				; h�߶ȼ�1
	jnz .loop
	emms
	jmp .end
.skip4pixel:
	add ecx,4				; ��ȼ�������4
	jmp .loop

.loop_pixel:
	sub eax,[ebp+ecx]		; eax=255-alpha ֵ(��¼4��) eax=0xb4b3b2b1 (b1...b4Ϊa1...a4����255-alpha�Ľ��)
	jz .4pixel				; ˵��4�����ص��alpha��Ϊ255��ֻ��Դλͼ������alpha���
	cmp eax,0xffffffff		; 4��alphaֵ�Ƿ񶼵���0?
	jz .skip4pixel			; �ǵģ���������4������
	pxor mm4,mm4			; mm4=0
	movd mm7,eax			; mm7=4��(255-alpha)=0x00000000 b4b3b2b1
	movq mm0,[edi+ecx*2]	; Ŀ������
	punpcklbw mm7,mm4		; mm7 = 0x00b4 00b3 00b2 00b1;byte->word
	movq mm1,mm0			; mm1=mm0=Ŀ������
	movq mm4,mm0			; mm4=mm0=Ŀ������
	movq mm2,[mask_b]		; mm2=0x001f 001f 001f 001f (��ɫ����)
	psrlw mm0,11			; mm0 = red (��word����11λ)
	pand mm2,mm4			; mm2 = blue
	pand mm1,[mask_g]		; mm1 = green(������û����λ���� gggggg00000)
	pmullw mm0,mm7			; mm0=red*(255-alpha)
	psrlw mm1,5				; mm1=green
	pmullw mm2,mm7			; mm2=blue*(255-alpha)
	psllw mm0,3				; mm0=red<<3(�н�λ��ʵ������<<11,Ȼ��>>8(�Ǻ�,��Ҫ����256��))
	pmullw mm1,mm7			; mm1=green*(255-alpha)
	psrlw mm2,8				; mm2=blue>>8(�Ǻ�,��Ҫ����256��)
	psrlw mm1,3				; mm1=green>>3(ʵ������<<5,Ȼ��>>8(�Ǻ�,��Ҫ����256��))
	pand mm0,[mask_r]		; red����?
	pand mm1,[mask_g]		; green����?
	por mm0,mm2				; 
	por mm0,mm1				; 
	cmp ecx,var(%$w4)		; ecx<=w4?
	paddw mm0,[esi+ecx*2]	; mm0=Դ���غ�Ŀ�����ص�alpha���(Դ������λͼ����ʱ��������alpha���,����Ͳ���Ҫ�����)
	jle .not_tail			; ecx<=w4,��û�е���ĩ
.clip:						; ������β��û��4����blit��Ҫ���вü�
	pand mm4,mm6			; ��Ŀ����������Ч�ĵ���0x0000�滻
	psubusw mm0,mm6			; ��mm0����Ч�ĵ���0x0000�滻
	por mm0,mm4				; Դ��Ŀ�����ػ��
	movq [edi+ecx*2],mm0	; д���Ϻ������
	jmp .nextline			; ������һ��
.4pixel:					; �ƶ�4������alpha��ϼ����4�����ص�
	cmp ecx,var(%$w4)		; ecx<=w4?
	movq mm0,[esi+ecx*2]	; mm0=Դ����
	jle .neednotclip		; ecx<=w4,����ü�
	movq mm4,[edi+ecx*2]	; ��Ҫ�ü�,mm4=Ŀ������
	jmp .clip				; ����ü�

.neednotclip:				; ����Ҫ�ü������ݴ���
	movq [edi+ecx*2],mm0	; ���������д��Ŀ��λͼ
	add ecx,4				; ��ȵ���4
	jmp .loop

.not_tail:					; ��û�е���β
	movq [edi+ecx*2],mm0	; ���������д��Ŀ��λͼ
	add ecx,4				; ��ȵ���4
	jmp .loop
.end:	
	xor eax,eax
;	mov eax,var(%$this)		; ���� this ָ��
endproc

function _alpha_trans_blit16
proc __alpha_trans_blit16@8
	saveesi
	saveedi
	saveebx
	saveebp
%$this	arg
%$bmp	arg
%$w	locvar
%$h	locvar
%$w4	locvar
	endlocvar

	mov ebx,var(%$bmp)		; edx = bmp ָ��
	movd mm7,[ebx+WBmpStruct.data]
	mov ecx,var(%$this)		; this ָ��
	mov ebp,[ebx+WBmpStruct.ptr]	; ebp ΪԴͼָ��
	bmpclip	16			; ���ô���(16 bit)
					; ebx=bmp(Ŀ��)ָ��,ecx=this(Դ)ָ��
					; esi=Դ��ַ, edi=Ŀ���ַ
.blit:
	punpcklwd mm7,mm7
	pcmpeqd mm6,mm6
	punpckldq mm7,mm7		; mm7=alpha
	sub ebp,esi
	mov edx,[ecx+WBmpStruct.pitch]	; Ŀ��ͼ pitch
	neg ebp
	mov eax,var(%$w)
	shr ebp,1
	sub eax,4
	xor ecx,ecx
	mov var(%$w4),eax
	and eax,3
	add ebp,[ebx+WAlphaBmpStruct.channel]
	shl eax,4
	mov ebx,[ebx+WBmpStruct.pitch]	; Դͼ pitch
	movd mm5,eax
	sar ebx,1
	psllq mm6,mm5
	cld
	movq [mask],mm6
	align 4
.loop:	
	cmp ecx,var(%$w)
	jl .loop_pixel
.nextline:
	xor ecx,ecx
	add ebp,ebx
	lea esi,[esi+ebx*2]
	add edi,edx
	dec dword var(%$h)
	jnz .loop
	emms
	jmp .end
.skip4pixel:
	add ecx,4
	jmp .loop
.loop_pixel:
	mov eax,[ebp+ecx]		; alpha ֵ
	test eax,eax
	jz .skip4pixel

	movd mm6,eax
	pcmpeqb mm4,mm4
	movq mm3,[edi+ecx*2]
	punpcklbw mm6,mm4
	movq mm0,[esi+ecx*2]		; Ŀ������
	pcmpeqw mm6,mm4
	movq mm4,mm3
	pand mm6,mm7			; mm6 = alpha
	movq mm5,mm3
	psrlw mm6,3

	psrlw mm3,5
	pand mm4,[mask_g]
	pand mm5,[mask_b]
	movq mm1,mm0
	movq mm2,mm0
	psrlw mm0,5
	pand mm1,[mask_g]
	pand mm2,[mask_b]
	psubw mm0,mm3
	psubw mm1,mm4
	pmullw mm0,mm6
	movq mm3,[edi+ecx*2]
	psubw mm2,mm5
	pmullw mm1,mm6
	paddw mm0,mm3
	pmullw mm2,mm6
	psrlw mm1,5
	psrlw mm2,5
	paddw mm1,mm4
	paddw mm2,mm5
	pand mm0,[mask_r]
	pand mm1,[mask_g]
	pand mm2,[mask_b]
	por mm0,mm1
	por mm0,mm2

	cmp ecx,var(%$w4)
	jle .not_tail
	pand mm3,[mask]
	psubusw mm0,[mask]
	por mm0,mm3
	movq [edi+ecx*2],mm0
	jmp .nextline
.not_tail:
	movq [edi+ecx*2],mm0
	add ecx,4
	jmp .loop
.end:	
	xor eax,eax
endproc

[section .data class=data use32]
mask_r	dd	0xf800f800,0xf800f800
mask_g	dd	0x07e007e0,0x07e007e0
mask_b	dd	0x001f001f,0x001f001f

[section .bss class=bss use32]
mask	resq	1