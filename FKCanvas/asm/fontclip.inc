

	mov eax,[ecx]WBmpStruct.ky
	mov edi,[ecx]WBmpStruct.ptr	; back ������ָ��
	mov edx,[ebx]WCharStruct.h	; ���� char �ĸ߶�
	mov esi,[ebx]WCharStruct.data	; Char ������ָ��
	mov h,edx
	test edx,edx
	jz _endclip
	sub eax,[ebx]WCharStruct.ky
	jns _under_firstline

	add h,eax		; ������ҪBLIT������
	jle _endclip
	imul eax,[ebx]WCharStruct.w	; ��� eax �е�ָ��ƫ�� (����)
	sub esi,eax
	mov eax,h
	jmp _adjust_height

_endclip:
	;xor eax,eax
	;mov eax, 1
	;neg eax
	;ret
	jmp _end
	
_under_firstline:
	cmp eax,[ecx]WBmpStruct.h	; �Ƚ� back λͼ�ĸ߶�
	jge _endclip
	mov edx,eax
	add eax,h
	imul edx,[ecx]WBmpStruct.pitch	; ���� this λͼָ��
	add edi,edx

_adjust_height:
	
	sub eax,[ecx]WBmpStruct.h	; bmp ͼ���¶˺� this ͼ�¶˲�(����)
	jl _dealwith_width
	sub h,eax		; ������Ҫ blit ������

_dealwith_width:
	mov edx,[ebx]WCharStruct.w	; ���� bmp �Ŀ���
	mov eax,[ecx]WBmpStruct.kx
	mov w,edx
	sub eax,[ebx]WCharStruct.kx	; eax = BLIT �� this λͼ����λ��
	jns _right_firstcol

	add w,eax		; ������Ҫ BLIT ������
	jle _endclip
	sub esi,eax
	mov eax,w
	jmp _adjust_width

_right_firstcol:
	cmp eax,[ecx]WBmpStruct.w	; �Ƚ� back λͼ�Ŀ���
	jge _endclip			; ����λͼ����ת
	lea edi,[edi+eax*2]		; ���� back λͼָ��
	add eax,w

_adjust_width:
	sub eax,[ecx]WBmpStruct.w	; bmp ͼ���Ҷ˺� this λͼ�Ҷ˲�(����)
	jl _blit
	sub w,eax