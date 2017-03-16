/**
*	created:		2012-6-20   18:32
*	filename: 		RleAsm
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../../FKTrace/IFKTrace.h"
#include "../IFKCanvas.h"
#include "Asm.h"
//------------------------------------------------------------------------
#ifdef _USE_32LEVEL_ALPHA
	#define SHIFT	3
#else
	#define SHIFT	5
#endif
//------------------------------------------------------------------------
void __fastcall Memset16Bit (WPixel* dst, WPixel fill, int num)
{
    if (num <= 0) return;
    if (num&1) *dst++ = fill; // �������������ʱ��	��ǰ�����һ��
    if (num <2) return;
	
	_asm
	{
			mov   ax, fill    // ����Ҫ����ֵ
			mov   ecx, num    // �����Ŀ
			shl   eax, 16     // ���ֵ����16bit
			mov   edi, dst    // Ŀ���ַ
			add   ax, fill    // һ�δ���2������
			shr   ecx, 1      // �����Ŀ����2
			rep   stosd		
	}
}
//------------------------------------------------------------------------
void blit_alpha(WPixel* dest, WPixel* src, int alpha)
{
	alpha=alpha>>3;
	__asm{
		mov esi, src		;
		mov edi, dest		;
		mov eax, [esi]		;
		mov ebx, [edi]		;
		mov esi, alpha		;
		mov edi, 0x7e0f81f	; //00000111111000001111100000011111
		
		// ������ɫ
		mov cx,ax		; cx=r1g1b1
		mov dx,bx		; dx=r2g2b2
		sal eax,16		; eax=r1g1b1......
		sal ebx,16		; ebx=r2g2b2......
		mov ax,cx		; eax=r1g1b1r1g1b1
		mov bx,dx		; ebx=r2g2b2r2g2b2
		and eax,edi		; eax=..g1..r1..b1
		mul esi			; eax*=alpha
		neg esi			; -alpha
		and ebx,edi		; ebx=..g2..r2..b2
		add esi,0x20	; 32-alpha
		xchg eax,ebx	; ���� eax,ebx
		mul esi			; c2*=(32-alpha)
		add eax,ebx		; c1*alpha+c2*(32-alpha)
		sar eax,5		; color=(c1*alpha+c2*(32-alpha))/32
		
		//��ԭ�� RGB ��ʽ
		and eax,edi		; color=..g..r..b
		mov cx,ax		;
		sar eax,16		; 
		or ax,cx		; color=rgb (eax)
		mov edi, dest	;
		mov [edi], ax	;
	}
}
//------------------------------------------------------------------------
void blit_rle_copy(int dx, int dy, int w, int h, int pitch, void* ptr, WPixel* color, BYTE* bits)
{
	WPixel* dataptr=(WPixel*)ptr;
	int x_disc = dx<<1;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + x_disc);
	
	_asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������
		add esi, 2;
		align 4;
		
lineloops:
		cmp ebx, w;			// һ�н�����(j<w)?
		jnl next_line;
		
		xor ecx,ecx;		// ecx=0
		mov cl, [esi];		// ��Դ�ж�ȡһ���ֽڵ�cl
		cmp ecx, 0x80;		// �Ƿ���͸����־��
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0xc0;		// �Ƿ����ظ�����ɫ��
		jl repeat_handle;	// �ǣ���jump��repeat_handle
		
	//normal_handle:
		sub cl, 0xbf;
		add esi, 1;			// nPos += 1
loops2:	xor edx, edx;
		mov dl, [esi];		// ��ȡ��������ɫ���е���ɫ����index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		mov dx,[edx];		// ��ȡָ������ɫ
		mov [edi],dx;		// д����ɫ
		add edi, 2;			// Ŀ��ָ���2
		inc ebx;			// j++
		inc esi;			// nPos++
		//dec ecx;			// rep_count--
		//cmp ecx, 0;			// rep_count > 0 ?
		//jg loops2;
		loop loops2;
		jmp lineloops;
		
next_line:
		mov ecx, w;			// ��ȡw
		shl ecx, 1;			// w*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*w
		add edi, edx;		// edi += delta
		add eax, 1;			// i++
		cmp eax, h;			// �����н�����
		jnl done;			// all done
		xor ebx, ebx;		// j=0
		add esi, 2;
		jmp lineloops;
		
		
mask_handle: // ����͸��
		inc cl;
		add ebx, ecx;		// j+=ecx
		test ecx, 0x0001;	// �Ƿ�����?
		jnz pre_handle_one_pixel_for_mask;
		
memset16_for_mask:
		push eax;			// ������ǰ��ֵ
		mov eax, 0xf81ff81f;// ��ȡҪ����ֵ,һ�δ���2������
		shr ecx, 1			// �����Ŀ����2
		rep stosd			// �ظ�����ڴ洮
		pop eax;			// �ָ���ǰֵ
		
memset16_done_for_mask:
		inc esi;			// Դָ���ƶ�1���ֽ�
		jmp lineloops;

pre_handle_one_pixel_for_mask:
		mov word ptr [edi],0xf81f;	// д����ɫ
		add edi, 2;					// Ŀ���ַ+2
		cmp ecx, 1;					// �������Ҫ�����һ��
		jz  memset16_done_for_mask; // �򲻽����ڴ���䣬��Ϊǰ���Ѿ������
		jmp memset16_for_mask;		// 16bit�ڴ����
		
repeat_handle:
		xor edx, edx;		// edx=0
		sub cl, 0x7f;		// cl = cl - 127
		mov dl, [esi+1];	// ��ȡ�ظ���ɫ����ɫ����dl
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ������*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		add ebx, ecx;		// j+=ecx
		test ecx, 0x0001;	// �Ƿ�����?
		jnz pre_handle_one_pixel;
		
memset16:
		push eax;			// ������ǰ��ֵ
		mov ax, [edx]		// ��ȡҪ����ֵ
		shl eax, 16			// ���ֵ����16bit
		add ax, [edx]		// һ�δ���2������
		shr ecx, 1			// �����Ŀ����2
		rep stosd			// �ظ�����ڴ洮
		pop eax;			// �ָ���ǰֵ
		
memset16_done:
		add esi, 2;			// Դָ���ƶ�2���ֽ�
		jmp lineloops;
		
pre_handle_one_pixel:
		push eax;
		mov ax,[edx];		// ��ȡָ������ɫ
		mov [edi],ax;		// д����ɫ
		pop eax;
		add edi, 2;			// Ŀ���ַ+2
		cmp ecx, 1;			// �������Ҫ�����һ��
		jz  memset16_done;  // �򲻽����ڴ���䣬��Ϊǰ���Ѿ������
		jmp memset16;		// 16bit�ڴ����
		
done:
		nop
	}
}
//------------------------------------------------------------------------
void blit_rle_mask(int dx, int dy, int w, int h, int pitch, void* ptr, WPixel* color, BYTE* bits)
{
	WPixel* dataptr=(WPixel*)ptr;
	int x_disc = dx<<1;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + x_disc);
	

	_asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������
		add esi, 2;
		align 4;
		
lineloops:
		cmp ebx, w;			// һ�н�����(j<w)?
		jnl next_line;
		
		xor ecx,ecx;		// ecx=0
		mov cl, [esi];		// ��Դ�ж�ȡһ���ֽڵ�cl
		cmp ecx, 0x80;		// �Ƿ���͸����־��
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0xc0;		// �Ƿ����ظ�����ɫ��
		jl repeat_handle;	// �ǣ���jump��repeat_handle
		
//normal_handle:
		add esi, 1;			// nPos += 1
		sub cl, 0xbf;
loops2:	xor edx, edx;
		mov dl, [esi];		// ��ȡ�ظ���ɫ����ɫ����index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		inc ebx;			// j++
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		inc esi;			// nPos++
		mov dx,[edx];		// ��ȡָ������ɫ
		dec ecx;			// rep_count--
		mov [edi],dx;		// д����ɫ
		add edi, 2;			// Ŀ��ָ���2
		cmp ecx, 0;			// rep_count > 0 ?
		jg loops2;
		jmp lineloops;
		
next_line:
		mov ecx, w;			// ��ȡw
		shl ecx, 1;			// w*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*w
		add edi, edx;		// edi += delta
		add eax, 1;			// i++
		cmp eax, h;			// �����н�����
		jnl done;			// all done
		xor ebx, ebx;		// j=0
		add esi, 2;
		jmp lineloops;
		
		
mask_handle: // ����͸��
		inc cl;
		inc esi;
		add ebx, ecx;		// j+=ecx
		shl ecx, 1;			// cl*2
		add edi, ecx;		// Ŀ��ָ������͸��������
		jmp lineloops;
		
repeat_handle:
		xor edx, edx;		// edx=0
		sub cl, 0x7f;
		mov dl, [esi+1];	// ��ȡ�ظ���ɫ����ɫ����dl
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ������*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		add ebx, ecx;		// j+=ecx
		test ecx, 0x0001;	// �Ƿ�����?
		jnz pre_handle_one_pixel;
		
memset16:
		push eax;			// ������ǰ��ֵ
		mov ax, [edx]		// ��ȡҪ����ֵ
		shl eax, 16			// ���ֵ����16bit
		add ax, [edx]		// һ�δ���2������
		shr ecx, 1			// �����Ŀ����2
		rep stosd			// �ظ�����ڴ洮
		pop eax;			// �ָ���ǰֵ
		
memset16_done:
		add esi, 2;			// Դָ���ƶ�3���ֽ�
		jmp lineloops;
		
pre_handle_one_pixel:
		push eax;
		mov ax,[edx];		// ��ȡָ������ɫ
		mov [edi],ax;		// д����ɫ
		pop eax;
		add edi, 2;			// Ŀ���ַ+2
		cmp ecx, 1;			// �������Ҫ�����һ��
		jz  memset16_done;  // �򲻽����ڴ���䣬��Ϊǰ���Ѿ������
		jmp memset16;		// 16bit�ڴ����
		
done:
		nop
	}
}
//------------------------------------------------------------------------
void blit_rle_clip_copy(int dx, int dy, int w, int h, RECT& clip, int pitch, void* ptr, WPixel* color, BYTE* bits)
{
	WPixel* dataptr=(WPixel*)ptr;
	int x_disc = dx<<1;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + x_disc);
	int nClipW=clip.right-clip.left+1;
	int nClipH=clip.bottom-clip.top+1;
	
	int top=clip.top,right=clip.right,left=clip.left,bottom=clip.bottom;
	
	_asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		xor ecx, ecx;
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������

		// skip some line
		mov ebx, clip;
		mov ebx, [ebx]RECT.top;
		cmp ebx, 0;
		jz noskiploops;
skiploops:mov cx, word ptr [esi];
		add esi, ecx;
		inc eax;
		dec ebx;
		cmp ebx, 0;
		jg skiploops;
noskiploops:
		add esi, 2;
		align 4;

loops:
		cmp ebx, w;			// һ�н�����(j<w)?
		jnl next_line;
		
		xor ecx,ecx;		// ecx=0
		mov cl, [esi];		// ��Դ�ж�ȡһ���ֽڵ�cl
		cmp ecx, 0x80;		// �Ƿ���͸����־��
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0xc0;		// �Ƿ����ظ�����ɫ��
		jl repeat_handle;	// �ǣ���jump��repeat_handle

		
//////////////////////  �����ĵ����ش���  //////////////////////////
//handle_clip:
		sub cl, 0xbf;
		add esi, 1;			// nPos += 1
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left1__;
		pop eax;
		jmp normal_handle_out_clip;
ge_left1__:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg normal_handle_out_clip;
		
//normal_handle_in_clip:
		push esi;
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2__;		// delta1<0?
		sub ecx, eax;
		add esi, eax;
handle_delta2__:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js normal_handle_mem_copy;		// delta2<0?
		sub ecx, edx;
		
normal_handle_mem_copy:
		pop ebx;
		pop edx;
loops2:	cmp ecx, 0;
		jle noloops2;
		xor edx, edx;
		mov dl, [esi];		// ��ȡ��������ɫ���е���ɫ����index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		mov dx,[edx];		// ��ȡָ������ɫ
		mov [edi],dx;		// д����ɫ
		add edi, 2;			// Ŀ��ָ���2
		inc esi;			// nPos++
		dec ecx;			// rep_count--
		jmp loops2;

noloops2:
		pop eax;
		pop ecx;
		pop esi;
normal_handle_out_clip:
		add ebx, ecx;		// j+=rep_count
		add esi, ecx;		// Դָ���ƶ�rep_count���ֽ�
		jmp loops;

		
//////////////////////  �Ƿ���һ��?  //////////////////////////
next_line:
		mov ecx, nClipW;	// ��ȡnClipW
		shl ecx, 1;			// nClipW*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*nClipW
		add edi, edx;		// edi += delta
		add eax, 1;			// i++
		add esi, 2;
		cmp eax, bottom;	// �����н�����
		jg done;			// all done
		xor ebx, ebx;		// j=0
		jmp loops;


//////////////////////  ͸������  //////////////////////////
mask_handle: 
		inc cl;
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left1;
		pop eax;
		jmp mask_handle_out_clip;
ge_left1:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg mask_handle_out_clip;

//mask_handle_in_clip:
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2;		// delta1<0?
		sub ecx, eax;
handle_delta2:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js handle_mem_copy;		// delta2<0?
		sub ecx, edx;
handle_mem_copy:
		pop ebx;
		pop edx;			
		test ecx, 0x1;			// �Ƿ�����?
		jz memset16_for_mask;	// ��ż��
		mov word ptr [edi],0xf81f;	// д����ɫ
		add edi, 2;					// Ŀ���ַ+2
		cmp ecx, 1;					// �������Ҫ�����һ��
		jz  memset16_done_for_mask; // �򲻽����ڴ���䣬��Ϊǰ���Ѿ������
memset16_for_mask:
		mov eax, 0xf81ff81f;// ��ȡҪ����ֵ,һ�δ���2������
		shr ecx, 1			// �����Ŀ����2
		rep stosd			// �ظ�����ڴ洮
		
memset16_done_for_mask:
		pop eax;
		pop ecx;
mask_handle_out_clip:
		inc esi;			// Դָ���ƶ�1���ֽ�
		add ebx, ecx;		// j+=rep_count
		jmp loops;



//////////////////////  �ظ����ش���  //////////////////////////
repeat_handle: 
		sub cl, 0x7f;
		jmp handle_clip2;
handle_clip2:
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left2;
		pop eax;
		jmp repeat_handle_out_clip;
ge_left2:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jle repeat_handle_in_clip;
		jmp repeat_handle_out_clip;
repeat_handle_in_clip:
		xor edx, edx;		// edx=0
		mov dl, [esi+1];	// ��ȡ�ظ���ɫ����ɫ����dl,edx <- index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ������*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��,edx = color + 2*index

		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		cmp eax, 0;				// delta1<0?
		js handle_delta2_;
		sub ecx, eax;
handle_delta2_:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		cmp edx, 0;				// delta2<0?
		js handle_mem_copy_;
		sub ecx, edx;
handle_mem_copy_:
		pop ebx;
		pop edx;
		test ecx, 0x0001;		// �Ƿ�����?
		jz memset16;
		mov ax,[edx];		// ��ȡָ������ɫ
		mov [edi],ax;		// д����ɫ
		add edi, 2;			// Ŀ���ַ+2
		cmp ecx, 1;			// �������Ҫ�����һ��
		jz  memset16_done;  // �򲻽����ڴ���䣬��Ϊǰ���Ѿ������
memset16:
		mov ax, [edx]		// ��ȡҪ����ֵ
		shl eax, 16			// ���ֵ����16bit
		add ax, [edx]		// һ�δ���2������
		shr ecx, 1			// �����Ŀ����2
		rep stosd			// �ظ�����ڴ洮
		
memset16_done:
		pop eax;
		pop ecx;
repeat_handle_out_clip:
		add esi, 2;			// Դָ���ƶ�2���ֽ�
		add ebx, ecx;		// j+=rep_count
		jmp loops;
	
done:
		nop
	}
}
//------------------------------------------------------------------------
void blit_rle_clip_mask(int dx, int dy, int w, int h, RECT& clip, int pitch, void* ptr, WPixel* color, BYTE* bits)
{
	WPixel* dataptr=(WPixel*)ptr;
	int x_disc = dx<<1;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + x_disc);
	int nClipW=clip.right-clip.left+1;
	int nClipH=clip.bottom-clip.top+1;
	
	int top=clip.top,right=clip.right,left=clip.left,bottom=clip.bottom;
	
	_asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		xor ecx, ecx;
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������

		// skip some line
		mov ebx, clip;
		mov ebx, [ebx]RECT.top;
		cmp ebx, 0;
		jz noskiploops;
skiploops:mov cx, word ptr [esi];
		add esi, ecx;
		inc eax;
		dec ebx;
		cmp ebx, 0;
		jg skiploops;
noskiploops:
		add esi, 2;
		align 4;

loops:
		cmp ebx, w;			// һ�н�����(j<w)?
		jnl next_line;
		
		xor ecx,ecx;		// ecx=0
		mov cl, [esi];		// ��Դ�ж�ȡһ���ֽڵ�cl
		cmp ecx, 0x80;		// �Ƿ���͸����־��
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0xc0;		// �Ƿ����ظ�����ɫ��
		jl repeat_handle;	// �ǣ���jump��repeat_handle

		
//////////////////////  �����ĵ����ش���  //////////////////////////
//handle_clip:
		sub cl, 0xbf;
		add esi, 1;			// nPos += 1
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left1__;
		pop eax;
		jmp normal_handle_out_clip;
ge_left1__:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg normal_handle_out_clip;
		
//normal_handle_in_clip:
		push esi;
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2__;		// delta1<0?
		sub ecx, eax;
		add esi, eax;
handle_delta2__:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js normal_handle_mem_copy;		// delta2<0?
		sub ecx, edx;
		
normal_handle_mem_copy:
		pop ebx;
		pop edx;
loops2:	cmp ecx, 0;
		jle noloops2;
		xor edx, edx;
		mov dl, [esi];		// ��ȡ��������ɫ���е���ɫ����index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		mov dx,[edx];		// ��ȡָ������ɫ
		mov [edi],dx;		// д����ɫ
		add edi, 2;			// Ŀ��ָ���2
		inc esi;			// nPos++
		dec ecx;			// rep_count--
		jmp loops2;

noloops2:
		pop eax;
		pop ecx;
		pop esi;
normal_handle_out_clip:
		add ebx, ecx;		// j+=rep_count
		add esi, ecx;		// Դָ���ƶ�rep_count���ֽ�
		jmp loops;

		
//////////////////////  �Ƿ���һ��?  //////////////////////////
next_line:
		mov ecx, nClipW;	// ��ȡnClipW
		shl ecx, 1;			// nClipW*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*nClipW
		add edi, edx;		// edi += delta
		add eax, 1;			// i++
		add esi, 2;
		cmp eax, bottom;	// �����н�����
		jg done;			// all done
		xor ebx, ebx;		// j=0
		jmp loops;


//////////////////////  ͸������  //////////////////////////
mask_handle: 
		inc cl;
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left1;
		pop eax;
		jmp mask_handle_out_clip;
ge_left1:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg mask_handle_out_clip;

//mask_handle_in_clip:
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2;		// delta1<0?
		sub ecx, eax;
handle_delta2:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js handle_mem_copy;		// delta2<0?
		sub ecx, edx;
handle_mem_copy:
		pop ebx;
		pop edx;
		shl ecx, 1;
		add edi, ecx;
		
		pop eax;
		pop ecx;
mask_handle_out_clip:
		inc esi;			// Դָ���ƶ�1���ֽ�
		add ebx, ecx;		// j+=rep_count
		jmp loops;



//////////////////////  �ظ����ش���  //////////////////////////
repeat_handle: 
		sub cl, 0x7f;
		jmp handle_clip2;
handle_clip2:
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left2;
		pop eax;
		jmp repeat_handle_out_clip;
ge_left2:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jle repeat_handle_in_clip;
		jmp repeat_handle_out_clip;
repeat_handle_in_clip:
		xor edx, edx;		// edx=0
		mov dl, [esi+1];	// ��ȡ�ظ���ɫ����ɫ����dl,edx <- index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ������*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��,edx = color + 2*index

		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		cmp eax, 0;				// delta1<0?
		js handle_delta2_;
		sub ecx, eax;
handle_delta2_:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		cmp edx, 0;				// delta2<0?
		js handle_mem_copy_;
		sub ecx, edx;
handle_mem_copy_:
		pop ebx;
		pop edx;
		test ecx, 0x0001;		// �Ƿ�����?
		jz memset16;
		mov ax,[edx];		// ��ȡָ������ɫ
		mov [edi],ax;		// д����ɫ
		add edi, 2;			// Ŀ���ַ+2
		cmp ecx, 1;			// �������Ҫ�����һ��
		jz  memset16_done;  // �򲻽����ڴ���䣬��Ϊǰ���Ѿ������
memset16:
		mov ax, [edx]		// ��ȡҪ����ֵ
		shl eax, 16			// ���ֵ����16bit
		add ax, [edx]		// һ�δ���2������
		shr ecx, 1			// �����Ŀ����2
		rep stosd			// �ظ�����ڴ洮
		
memset16_done:
		pop eax;
		pop ecx;
repeat_handle_out_clip:
		add esi, 2;			// Դָ���ƶ�2���ֽ�
		add ebx, ecx;		// j+=rep_count
		jmp loops;
	
done:
		nop
	}
}
//------------------------------------------------------------------------
#define alpha25mask		0x39e7		// 1/4=0.25
#define alpha50mask		0xf7df		// 
#define alpha125mask	0x18e3		// 1/8=0.125
#define alpha50mask_2	0x7bef		// 1/2=0.5

#ifdef _USE_32LEVEL_ALPHA

#define alpha32mask	0x20		// 1/32=0.03125
#define alpha16mask	0x861		// 1/16=0.0625
#define alpha8mask	0x18e3		// 1/8=0.125
#define alpha4mask	0x39e7		// 1/4=0.25
#define alpha2mask	0x7bef		// 1/2=0.5
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=0/32
__declspec(naked) void _alpha_0(void)
{
	_asm
	{
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=1/32
__declspec(naked) void _alpha_1(void)
{
	_asm
	{
		shr ax, 5;				// ax=s/32
		mov bx, [edi];			// bx=d
		and ax, alpha32mask;
		add ax, bx;				// ax=s/32 + d
		shr bx, 5;				// bx=d/32
		and bx, alpha32mask;
		sub ax, bx;				// ax=s/32 + 31d/32
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=2/32
__declspec(naked) void _alpha_2(void)
{
	_asm
	{
		shr ax, 4;				// ax=s/16
		mov bx, [edi];			// bx=d
		and ax, alpha16mask;
		add ax, bx;				// ax=s/16 + d
		shr bx, 4;				// bx=d/16
		and bx, alpha16mask;
		sub ax, bx;				// ax=2s/32 + 30d/32
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=3/32
__declspec(naked) void _alpha_3(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 4;				// ax=s/16
		shr bx, 4;				// bx=d/16
		and ax, alpha16mask;
		and bx, alpha16mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/16
		mov dx, bx;				// dx=d/16
		shr ax, 1;				// ax=s/32
		shr bx, 1;				// bx=d/32
		and ax, alpha2mask;
		and bx, alpha2mask;
		add ax, cx;				// ax=s/32 + s/16 = 3s/32
		add bx, dx;				// bx=d/32 + d/16 = 3d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=29d/32
		add ax, cx;				// ax=3s/32 + 29d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=4/32
__declspec(naked) void _alpha_4(void)
{
	_asm
	{
		push cx;
		mov bx, [edi];			// bx=d
		shr ax, 3;				// ax=s/8
		mov cx, bx;				// cx=d
		and ax, alpha8mask;
		shr cx, 3;				// cx=d/8
		and cx, alpha8mask;
		sub bx, cx;				// bx=d - d/8 = 7d/8
		pop cx;
		add ax, bx;				// ax=4s/32 + 28d/32
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=5/32
__declspec(naked) void _alpha_5(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 3;				// ax=s/8
		shr bx, 3;				// bx=d/8
		and ax, alpha8mask;
		and bx, alpha8mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/8
		mov dx, bx;				// dx=d/8
		shr ax, 2;				// ax=s/32
		shr bx, 2;				// bx=d/32
		and ax, alpha4mask;
		and bx, alpha4mask;
		add ax, cx;				// ax=s/32 + s/8 = 5s/32
		add bx, dx;				// bx=d/32 + d/8 = 5d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=27d/32
		add ax, cx;				// ax=5s/32 + 27d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=6/32
__declspec(naked) void _alpha_6(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 3;				// ax=s/8
		shr bx, 3;				// bx=d/8
		and ax, alpha8mask;
		and bx, alpha8mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/8
		mov dx, bx;				// dx=d/8
		shr ax, 1;				// ax=s/16
		shr bx, 1;				// bx=d/16
		and ax, alpha2mask;
		and bx, alpha2mask;
		add ax, cx;				// ax=s/16 + s/8 = 6s/32
		add bx, dx;				// bx=d/16 + d/8 = 6d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=26d/32
		add ax, cx;				// ax=6s/32 + 26d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=7/32
__declspec(naked) void _alpha_7(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 2;				// ax=s/4
		shr bx, 2;				// bx=d/4
		and ax, alpha4mask;
		and bx, alpha4mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/4
		mov dx, bx;				// dx=d/4
		shr cx, 3;				// cx=s/32
		shr dx, 3;				// dx=d/32
		and cx, alpha8mask;
		and dx, alpha8mask;
		sub ax, cx;				// ax=s/4 - s/32 = 7s/32
		sub bx, dx;				// bx=d/4 + d/32 = 7d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=25d/32
		add ax, cx;				// ax=7s/32 + 25d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=8/32
__declspec(naked) void _alpha_8(void)
{
	_asm
	{
		push cx;
		mov bx, [edi];			// bx=d
		shr ax, 2;				// ax=s/4
		mov cx, bx;				// cx=d
		and ax, alpha4mask;
		shr cx, 2;				// cx=d/4
		and cx, alpha4mask;
		sub bx, cx;
		pop cx;
		add ax, bx;				// ax=8s/32 + 24d/32
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=9/32
__declspec(naked) void _alpha_9(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 2;				// ax=s/4
		shr bx, 2;				// bx=d/4
		and ax, alpha4mask;
		and bx, alpha4mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/4
		mov dx, bx;				// dx=d/4
		shr ax, 3;				// ax=s/32
		shr bx, 3;				// bx=d/32
		and ax, alpha8mask;
		and bx, alpha8mask;
		add ax, cx;				// ax=s/32 + s/4 = 9s/32
		add bx, dx;				// bx=d/32 + d/4 = 9d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=23d/32
		add ax, cx;				// ax=9s/32 + 23d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=10/32
__declspec(naked) void _alpha_10(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 2;				// ax=s/4
		shr bx, 2;				// bx=d/4
		and ax, alpha4mask;
		and bx, alpha4mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/4
		mov dx, bx;				// dx=d/4
		shr ax, 2;				// ax=s/16
		shr bx, 2;				// bx=d/16
		and ax, alpha4mask;
		and bx, alpha4mask;
		add ax, cx;				// ax=s/16 + s/4 = 10s/32
		add bx, dx;				// bx=d/16 + d/4 = 10d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=22d/32
		add ax, cx;				// ax=10s/32 + 22d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=11/32
__declspec(naked) void _alpha_11(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 2;				// ax=s/4
		shr bx, 2;				// bx=d/4
		and ax, alpha4mask;
		and bx, alpha4mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/4
		mov dx, bx;				// dx=d/4
		shr ax, 2;				// ax=s/16
		shr bx, 2;				// bx=d/16
		and ax, alpha4mask;
		and bx, alpha4mask;
		add cx, ax;				// ax=s/4 + s/16 = 10s/32
		add dx, bx;				// bx=d/4 + d/16 = 10d/32
		shr ax, 1;				// ax=s/32
		shr bx, 1;				// bx=d/32
		and ax, alpha2mask;
		and bx, alpha2mask;
		add ax, cx;				// ax=s/4 + s/16 + s/32 = 11s/32
		add bx, dx;				// bx=d/4 + d/16 + d/32 = 11d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=21d/32
		add ax, cx;				// ax=11s/32 + 21d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=12/32
__declspec(naked) void _alpha_12(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 2;				// ax=s/4
		shr bx, 2;				// bx=d/4
		and ax, alpha4mask;
		and bx, alpha4mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/4
		mov dx, bx;				// dx=d/4
		shr ax, 1;				// ax=s/8
		shr bx, 1;				// bx=d/8
		and ax, alpha2mask;
		and bx, alpha2mask;
		add ax, cx;				// ax=s/8 + s/4 = 12s/32
		add bx, dx;				// bx=d/8 + d/4 = 12d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=20d/32
		add ax, cx;				// ax=12s/32 + 20d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=13/32
__declspec(naked) void _alpha_13(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 2;				// ax=s/4
		shr bx, 2;				// bx=d/4
		and ax, alpha4mask;
		and bx, alpha4mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/4
		mov dx, bx;				// dx=d/4
		shr ax, 1;				// ax=s/8
		shr bx, 1;				// bx=d/8
		and ax, alpha2mask;
		and bx, alpha2mask;
		add cx, ax;				// ax=s/4 + s/8 = 12s/32
		add dx, bx;				// bx=d/4 + d/8 = 12d/32
		shr ax, 2;				// ax=s/32
		shr bx, 2;				// bx=d/32
		and ax, alpha4mask;
		and bx, alpha4mask;
		add ax, cx;				// ax=s/4 + s/8 + s/32 = 13s/32
		add bx, dx;				// bx=d/4 + d/8 + d/32 = 13d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=19d/32
		add ax, cx;				// ax=13s/32 + 19d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=14/32
__declspec(naked) void _alpha_14(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/2
		mov dx, bx;				// dx=d/2
		shr cx, 3;				// cx=s/16
		shr dx, 3;				// dx=d/16
		and cx, alpha8mask;
		and dx, alpha8mask;
		sub ax, cx;				// ax=s/2 - s/16 = 14s/32
		sub bx, dx;				// bx=d/2 + d/16 = 14d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=18d/32
		add ax, cx;				// ax=14s/32 + 18d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=15/32
__declspec(naked) void _alpha_15(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/2
		mov dx, bx;				// dx=d/2
		shr cx, 4;				// cx=s/32
		shr dx, 4;				// dx=d/32
		and cx, alpha16mask;
		and dx, alpha16mask;
		sub ax, cx;				// ax=s/2 - s/32 = 15s/32
		sub bx, dx;				// bx=d/2 + d/32 = 15d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=17d/32
		add ax, cx;				// ax=15s/32 + 17d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=16/32
__declspec(naked) void _alpha_16(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		add ax, bx;				// ax=16s/32 + 16d/32
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=17/32
__declspec(naked) void _alpha_17(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/2
		mov dx, bx;				// dx=d/2
		shr ax, 4;				// ax=s/32
		shr bx, 4;				// bx=d/32
		and ax, alpha16mask;
		and bx, alpha16mask;
		add ax, cx;				// ax=s/32 + s/2 = 17s/32
		add bx, dx;				// bx=d/32 + d/2 = 17d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=15d/32
		add ax, cx;				// ax=17s/32 + 15d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=18/32
__declspec(naked) void _alpha_18(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/2
		mov dx, bx;				// dx=d/2
		shr ax, 3;				// ax=s/16
		shr bx, 3;				// bx=d/16
		and ax, alpha8mask;
		and bx, alpha8mask;
		add ax, cx;				// ax=s/16 + s/2 = 18s/32
		add bx, dx;				// bx=d/16 + d/2 = 18d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=14d/32
		add ax, cx;				// ax=18s/32 + 14d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=19/32
__declspec(naked) void _alpha_19(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/2
		mov dx, bx;				// dx=d/2
		shr ax, 3;				// ax=s/16
		shr bx, 3;				// bx=d/16
		and ax, alpha8mask;
		and bx, alpha8mask;
		add cx, ax;				// ax=s/2 + s/16 = 18s/32
		add dx, bx;				// bx=d/2 + d/16 = 18d/32
		shr ax, 1;				// ax=s/32
		shr bx, 1;				// bx=d/32
		and ax, alpha2mask;
		and bx, alpha2mask;
		add ax, cx;				// ax=s/2 + s/16 + s/32 = 19s/32
		add bx, dx;				// bx=d/2 + d/16 + d/32 = 19d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=13d/32
		add ax, cx;				// ax=19s/32 + 13d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=20/32
__declspec(naked) void _alpha_20(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/2
		mov dx, bx;				// dx=d/2
		shr ax, 2;				// ax=s/8
		shr bx, 2;				// bx=d/8
		and ax, alpha4mask;
		and bx, alpha4mask;
		add ax, cx;				// ax=s/8 + s/2 = 20s/32
		add bx, dx;				// bx=d/8 + d/2 = 20d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=12d/32
		add ax, cx;				// ax=20s/32 + 12d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=21/32
__declspec(naked) void _alpha_21(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/2
		mov dx, bx;				// dx=d/2
		shr ax, 2;				// ax=s/8
		shr bx, 2;				// bx=d/8
		and ax, alpha4mask;
		and bx, alpha4mask;
		add cx, ax;				// ax=s/2 + s/8 = 20s/32
		add dx, bx;				// bx=d/2 + d/8 = 20d/32
		shr ax, 2;				// ax=s/32
		shr bx, 2;				// bx=d/32
		and ax, alpha4mask;
		and bx, alpha4mask;
		add ax, cx;				// ax=s/2 + s/8 + s/32 = 21s/32
		add bx, dx;				// bx=d/2 + d/8 + d/32 = 21d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=11d/32
		add ax, cx;				// ax=21s/32 + 11d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=22/32
__declspec(naked) void _alpha_22(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/2
		mov dx, bx;				// dx=d/2
		shr ax, 2;				// ax=s/8
		shr bx, 2;				// bx=d/8
		and ax, alpha4mask;
		and bx, alpha4mask;
		add cx, ax;				// ax=s/2 + s/8 = 20s/32
		add dx, bx;				// bx=d/2 + d/8 = 20d/32
		shr ax, 1;				// ax=s/16
		shr bx, 1;				// bx=d/16
		and ax, alpha2mask;
		and bx, alpha2mask;
		add ax, cx;				// ax=s/2 + s/8 + s/16 = 22s/32
		add bx, dx;				// bx=d/2 + d/8 + d/16 = 22d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=10d/32
		add ax, cx;				// ax=22s/32 + 10d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=23/32
__declspec(naked) void _alpha_23(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/2
		mov dx, bx;				// dx=d/2
		shr cx, 1;				// cx=s/4
		shr dx, 1;				// dx=d/4
		and cx, alpha2mask;
		and dx, alpha2mask;
		add ax, cx;				// ax=s/2 + s/4 = 24s/32
		add bx, dx;				// bx=d/2 + d/4 = 24d/32
		shr cx, 3;				// cx=s/32
		shr dx, 3;				// dx=d/32
		and cx, alpha8mask;
		and dx, alpha8mask;
		sub ax, cx;				// ax=s/2 + s/4 - s/32 = 23s/32
		sub bx, dx;				// bx=d/2 + d/4 - d/32 = 23d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=9d/32
		add ax, cx;				// ax=23s/32 + 9d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=24/32
__declspec(naked) void _alpha_24(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/2
		mov dx, bx;				// dx=d/2
		shr ax, 1;				// ax=s/4
		shr bx, 1;				// bx=d/4
		and ax, alpha2mask;
		and bx, alpha2mask;
		add ax, cx;				// ax=s/4 + s/2 = 24s/32
		add bx, dx;				// bx=d/4 + d/2 = 24d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=8d/32
		add ax, cx;				// ax=24s/32 + 8d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=25/32
__declspec(naked) void _alpha_25(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/2
		mov dx, bx;				// dx=d/2
		shr ax, 1;				// ax=s/4
		shr bx, 1;				// bx=d/4
		and ax, alpha2mask;
		and bx, alpha2mask;
		add cx, ax;				// ax=s/2 + s/4 = 24s/32
		add dx, bx;				// bx=d/2 + d/4 = 24d/32
		shr ax, 3;				// ax=s/32
		shr bx, 3;				// bx=d/32
		and ax, alpha8mask;
		and bx, alpha8mask;
		add ax, cx;				// ax=s/2 + s/4 + s/32 = 25s/32
		add bx, dx;				// bx=d/2 + d/4 + d/32 = 25d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=7d/32
		add ax, cx;				// ax=25s/32 + 7d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=26/32
__declspec(naked) void _alpha_26(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		shr ax, 1;				// ax=s/2
		shr bx, 1;				// bx=d/2
		and ax, alpha2mask;
		and bx, alpha2mask;
		push cx;
		push dx;
		mov cx, ax;				// cx=s/2
		mov dx, bx;				// dx=d/2
		shr ax, 1;				// ax=s/4
		shr bx, 1;				// bx=d/4
		and ax, alpha2mask;
		and bx, alpha2mask;
		add cx, ax;				// ax=s/2 + s/4 = 24s/32
		add dx, bx;				// bx=d/2 + d/4 = 24d/32
		shr ax, 2;				// ax=s/16
		shr bx, 2;				// bx=d/16
		and ax, alpha4mask;
		and bx, alpha4mask;
		add ax, cx;				// ax=s/2 + s/4 + s/16 = 26s/32
		add bx, dx;				// bx=d/2 + d/4 + d/16 = 26d/32
		pop dx;
		mov cx, [edi];			// cx=d
		sub cx, bx;				// cx=6d/32
		add ax, cx;				// ax=26s/32 + 6d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=27/32
__declspec(naked) void _alpha_27(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		push cx;
		shr bx, 3;				// bx=d/8
		push dx;
		and bx, alpha8mask;
		mov cx, ax;				// cx=s
		mov dx, bx;				// dx=d/8
		shr cx, 3;				// cx=s/8
		shr dx, 2;				// dx=d/32
		and cx, alpha8mask;
		and dx, alpha4mask;
		sub ax, cx;				// ax=s - s/8 = 28s/32
		add bx, dx;				// bx=d/8 + d/32 = 5d/32
		shr cx, 2;				// cx=s/32
		pop dx;
		and cx, alpha4mask;
		sub ax, cx;				// ax=s - s/8 - s/32 = 27s/32
		add ax, bx;				// ax=27s/32 + 5d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=28/32
__declspec(naked) void _alpha_28(void)
{
	_asm
	{
		mov bx, ax;				// ax=s
		shr bx, 3;				// bx=s/8
		and bx, alpha8mask;
		sub ax, bx;				// ax=s - s/8 = 7s/8
		mov bx, [edi];			// bx=d
		shr bx, 3;				// bx=d/8
		and bx, alpha8mask;
		add ax, bx;				// ax=28s/32 + 4d/32
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=29/32
__declspec(naked) void _alpha_29(void)
{
	_asm
	{
		mov bx, [edi];			// bx=d
		push cx;
		shr bx, 4;				// bx=d/16
		push dx;
		and bx, alpha16mask;
		mov cx, ax;				// cx=s
		mov dx, bx;				// dx=d/16
		shr cx, 4;				// cx=s/16
		shr dx, 1;				// dx=d/32
		and cx, alpha16mask;
		and dx, alpha2mask;
		sub ax, cx;				// ax=s - s/16 = 30s/32
		add bx, dx;				// bx=d/16 + d/32 = 3d/32
		shr cx, 1;				// cx=s/32
		pop dx;
		and cx, alpha2mask;
		sub ax, cx;				// ax=s - s/16 - s/32 = 29s/32
		add ax, bx;				// ax=29s/32 + 3d/32
		pop cx;
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=30/32
__declspec(naked) void _alpha_30(void)
{
	_asm
	{
		mov bx, ax;				// ax=s
		shr bx, 4;				// bx=s/16
		and bx, alpha16mask;
		sub ax, bx;				// ax=s - s/16 = 15s/16
		mov bx, [edi];			// bx=d
		shr bx, 4;				// bx=d/16
		and bx, alpha16mask;
		add ax, bx;				// ax=30s/32 + 2d/32
		mov [edi], ax;
		add edi, 2;
		ret;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=31/32
__declspec(naked) void _alpha_31(void)
{
//	_asm
//	{
//		mov bx, ax;				// ax=s
//		shr bx, 5;				// bx=s/32
//		and bx, alpha32mask;
//		sub ax, bx;				// ax=s - s/32 = 31s/32
//		mov bx, [edi];			// bx=d
//		shr bx, 5;				// bx=d/16
//		and bx, alpha32mask;
//		add ax, bx;				// ax=31s/32 + 1d/32
//		mov [edi], ax;
//		add edi, 2;
//		ret;
//	}
	// ���ǽ���=31/32��������ȫ��͸��
	_asm
	{
		mov [edi], ax;
		add edi, 2;
		RET;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=32/32
__declspec(naked) void _alpha_32(void)
{
	_asm
	{
		mov [edi], ax;
		add edi, 2;
		RET;
	}
}
#endif
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=0/8
__declspec(naked) void alpha_0(void)
{
	_asm
	{
		add edi, 2;
		RET;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=1/8
__declspec(naked) void alpha_1(void)
{
	_asm
	{
		SHR ax, 3;				// ax=s/8
		MOV bx, [edi];			// bx=d
		AND ax, alpha125mask;
		ADD ax, bx;				// ax=s/8 + d
		SHR bx, 3;				// bx=d/8
		AND bx, alpha125mask;
		SUB ax, bx;				// ax=s/8 + 7d/8
		mov [edi], ax;
		add edi, 2;
		RET;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=2/8
__declspec(naked) void alpha_2(void)
{
	_asm
	{
		SHR ax, 2;				// ax=s/4
		MOV bx, [edi];			// bx=d
		AND ax, alpha25mask;
		ADD ax, bx;				// ax=s/4 + d
		SHR bx, 2;				// bx=d/4
		AND bx, alpha25mask;
		SUB ax, bx;				// ax=s/4 + 3d/4
		mov [edi], ax;
		add edi, 2;
		//STOSW;
		RET;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=3/8
// result = 3*s/8 + 5*d/8 = (s+d)/8 + (s+d)/4 + d/4
__declspec(naked) void alpha_3(void)
{
	_asm
	{
		MOV bx, [edi];			// bx=d
		SHR ax, 2;				// ax=s/4
		SHR bx, 2;				// bx=d/4
	    AND ax, alpha25mask;
		AND bx, alpha25mask;
		PUSH dx;
		ADD ax, bx;				// ax=(s+d)/4
		MOV dx, ax;				// dx=(s+d)/4
		SHR ax, 1;				// ax=(s+d)/8
		ADD dx, bx;				// dx=(s+d)/4 + d/4
		AND ax, alpha50mask_2;
		ADD ax, dx;				// ax=(s+d)/8 + (s+d)/4 + d/4
		POP dx;
		mov [edi], ax;
		add edi, 2;
		//STOSW;
		RET;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=4/8
__declspec(naked) void alpha_4(void)
{
	_asm
	{
		MOV bx, [edi];
		AND ax, alpha50mask;
		AND bx, alpha50mask;
		ADD ax, bx;
		RCR ax, 1;
		mov [edi], ax;
		add edi, 2;
		//STOSW;
		RET;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=5/8
__declspec(naked) void alpha_5(void)
{
	_asm
	{
		MOV bx, [edi];
		PUSH dx;
		SHR bx, 2;
		SHR ax, 2;
		AND bx, alpha25mask;	// bx = d/4
		AND ax, alpha25mask;	// ax = s/4
		ADD bx, ax;				// bx = (s+d)/4
		MOV dx, bx;				// dx = (s+d)/4
		SHR bx, 1;				// bx = (s+d)/8
		ADD ax, dx;				// ax = s/4 + (s+d)/4
		AND bx, alpha50mask_2;
		ADD ax, bx;				// ax = s/4 + (s+d)/4 + (s+d)/8 = 5s/8 + 3d/8
		pop dx;
		mov [edi], ax;
		add edi, 2;
		//STOSW;
		RET;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=6/8
__declspec(naked) void alpha_6(void)
{
	_asm
	{
		MOV bx, ax;				// bx = s
		SHR bx, 2;				// bx = s/4
		AND bx, alpha25mask;
		SUB ax, bx;				// ax = s - s/4
		MOV bx, [edi];			// bx = d
		SHR bx, 2;				// bx = d/4
	    AND bx, alpha25mask;
		ADD ax, bx;				// ax = s - s/4 + d/4 = 3s/4 + d/4
		mov [edi], ax;
		add edi, 2;
		//STOSW;
		RET;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=7/8
__declspec(naked) void alpha_7(void)
{
	_asm
	{
		MOV bx, ax;				// bx = s
		SHR bx, 3;				// bx = s/8
		AND bx, alpha125mask;
		SUB ax, bx;				// ax = s - s/8 = 7s/8
		MOV bx, [edi];			// bx = d
		SHR bx, 3;				// bx = d/8
	    AND bx, alpha125mask;
		ADD ax, bx;				// ax = 7s/8 + d/8
		mov [edi], ax;
		add edi, 2;
		RET;
	}
}
//------------------------------------------------------------------------
// result=��*src + dest - ��*dest; ��=8/8
__declspec(naked) void alpha_8(void)
{
	_asm
	{
		mov [edi], ax;
		add edi, 2;
		RET;
	}
}
//------------------------------------------------------------------------
typedef void (*TRANS_DRAW_FUNC)(void);
#ifdef _USE_32LEVEL_ALPHA
TRANS_DRAW_FUNC alpha_level[] = 
{_alpha_0,_alpha_1,_alpha_2,_alpha_3,_alpha_4,_alpha_5,_alpha_6,_alpha_7,
_alpha_8,_alpha_9,_alpha_10,_alpha_11,_alpha_12,_alpha_13,_alpha_14,_alpha_15,
_alpha_16,_alpha_17,_alpha_18,_alpha_19,_alpha_20,_alpha_21,_alpha_22,_alpha_23,
_alpha_24,_alpha_25,_alpha_26,_alpha_27,_alpha_28,_alpha_29,_alpha_30,_alpha_31};

TRANS_DRAW_FUNC alpha_level_trans[8][32] = 
{
	{_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,
	_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,
	_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,
	_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_1,_alpha_2,_alpha_3},//1

	{_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,
	_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,
	_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,
	_alpha_0,_alpha_1,_alpha_2,_alpha_3,_alpha_4,_alpha_5,_alpha_6,_alpha_7},//2

	{_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,
	_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,
	_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_1,_alpha_2,_alpha_3,
	_alpha_4,_alpha_5,_alpha_6,_alpha_7,_alpha_8,_alpha_9,_alpha_10,_alpha_11},//3

	{_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,
	_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,
	_alpha_0,_alpha_1,_alpha_2,_alpha_3,_alpha_4,_alpha_5,_alpha_6,_alpha_7,
	_alpha_8,_alpha_9,_alpha_10,_alpha_11,_alpha_12,_alpha_13,_alpha_14,_alpha_15},//4

	{_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,
	_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_1,_alpha_2,_alpha_3,
	_alpha_4,_alpha_5,_alpha_6,_alpha_7,_alpha_8,_alpha_9,_alpha_10,_alpha_11,
	_alpha_12,_alpha_13,_alpha_14,_alpha_15,_alpha_16,_alpha_17,_alpha_18,_alpha_19},//5

	{_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,
	_alpha_0,_alpha_1,_alpha_2,_alpha_3,_alpha_4,_alpha_5,_alpha_6,_alpha_7,
	_alpha_8,_alpha_9,_alpha_10,_alpha_11,_alpha_12,_alpha_13,_alpha_14,_alpha_15,
	_alpha_16,_alpha_17,_alpha_18,_alpha_19,_alpha_20,_alpha_21,_alpha_22,_alpha_23},//6

	{_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_0,_alpha_1,_alpha_2,_alpha_3,
	_alpha_4,_alpha_5,_alpha_6,_alpha_7,_alpha_8,_alpha_9,_alpha_10,_alpha_11,
	_alpha_12,_alpha_13,_alpha_14,_alpha_15,_alpha_16,_alpha_17,_alpha_18,_alpha_19,
	_alpha_20,_alpha_21,_alpha_22,_alpha_23,_alpha_24,_alpha_25,_alpha_26,_alpha_27},//7

	{_alpha_0,_alpha_1,_alpha_2,_alpha_3,_alpha_4,_alpha_5,_alpha_6,_alpha_7,
	_alpha_8,_alpha_9,_alpha_10,_alpha_11,_alpha_12,_alpha_13,_alpha_14,_alpha_15,
	_alpha_16,_alpha_17,_alpha_18,_alpha_19,_alpha_20,_alpha_21,_alpha_22,_alpha_23,
	_alpha_24,_alpha_25,_alpha_26,_alpha_27,_alpha_28,_alpha_29,_alpha_30,_alpha_31},//8
};
#else
TRANS_DRAW_FUNC alpha_level[] = {alpha_1,alpha_2,alpha_3,alpha_4,alpha_5,alpha_6,alpha_7,alpha_8};
TRANS_DRAW_FUNC alpha_level_trans[8][8] = 
{
	{alpha_0,alpha_0,alpha_0,alpha_0,alpha_0,alpha_0,alpha_0,alpha_1},//1
	{alpha_0,alpha_0,alpha_0,alpha_1,alpha_1,alpha_1,alpha_1,alpha_2},//2
	{alpha_0,alpha_0,alpha_1,alpha_1,alpha_1,alpha_2,alpha_2,alpha_3},//3
	{alpha_0,alpha_1,alpha_1,alpha_2,alpha_2,alpha_3,alpha_3,alpha_4},//4
	{alpha_0,alpha_1,alpha_1,alpha_2,alpha_3,alpha_3,alpha_4,alpha_5},//5
	{alpha_0,alpha_1,alpha_2,alpha_3,alpha_3,alpha_4,alpha_5,alpha_6},//6
	{alpha_0,alpha_1,alpha_2,alpha_3,alpha_4,alpha_5,alpha_6,alpha_7},//7
	{alpha_1,alpha_2,alpha_3,alpha_4,alpha_5,alpha_6,alpha_7,alpha_8},//8
};
#endif
//------------------------------------------------------------------------

//void (*alpha_level[8]) = {alpha_1,alpha_2,alpha_3,alpha_4,alpha_5,alpha_6,alpha_7,alpha_8};
/*
WSERR __stdcall blit_rle_alpha(stBlitParam& bp)
{
	int dx = bp.d_x;
	int dy = bp.d_y;
	int w = bp.w;
	int h = bp.h;
	int pitch = bp.d_pitch;
	void* ptr = bp.d_ptr;
	WPixel* color = bp.s_pal;
	BYTE* bits = bp.s_bits;

	WPixel* dataptr=(WPixel*)ptr;
	int x_disc = dx<<1;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + x_disc);

	__asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������
		add esi, 2;
		cld;
		align 4;
loops1:
		
		cmp ebx, w;			// j >= w ?
		jnl next_line;		// ������һ��

		xor ecx, ecx;
		mov cl, [esi];		// ecx <-- [esi]
		cmp ecx, 0x40;
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0x80;
		jl shadow_handle;
		cmp ecx, 0xc0;
		jl alpha_8_handle;

// normal handle
		and cl, 0x3f;
		inc esi;			// nPos += 1
		inc cl;				// rep_count = cl + 1
		add ebx, ecx;		// j += rep_count
		push eax;
		push ebx;
loops2:
		lodsw;				// ax = [esi]
		movzx edx, al;		// ��ȡ�ظ���ɫ����ɫ����index
		movzx ebx, ah;		// alpha
		add edx, edx;		// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		mov ax, [edx];		// ��ȡָ������ɫ
		mov edx, offset alpha_level[0];
		shl ebx, 2;
		dec cl;				// rep_count--
		add edx, ebx;
		call [edx];
		cmp cl, 0;			// rep_count > 0 ?
		jg loops2;
		pop ebx;
		pop eax;
		jmp loops1;
		
next_line:
		mov ecx, w;			// ��ȡw
		shl ecx, 1;			// w*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*w
		add edi, edx;		// edi += delta
		inc eax;			// i++
		cmp eax, h;			// �����н�����i>=h��
		jnl done;			// all done
		xor ebx, ebx;		// j=0
		add esi, 2;			// �����б�־
		jmp loops1;
		
		
mask_handle: // ͸������
		inc cl;				// rep_count = cl + 1
		inc esi;			// Դָ���ƶ�1���ֽ�
		add ebx, ecx;		// j+=rep_count
		shl ecx, 1;			// rep_count*=2
		add edi, ecx;		// Ŀ��ָ������͸��������
		jmp loops1;

shadow_handle: // Ӱ�Ӵ���
		sub cl, 0x3f;
		add ebx, ecx;
		push eax;
		push ebx;
		inc esi;
		lodsb;				// al = [esi]
		movzx ebx, al;
		mov edx, offset alpha_level[0];
		shl ebx, 2;
		add edx, ebx;
loops4:	xor eax, eax;   
		dec cl;				// rep_count--
		call [edx];
		cmp cl, 0;			// rep_count > 0 ?
		jg loops4;
		pop ebx;
		pop eax;
		jmp loops1;

alpha_8_handle:
		sub cl, 0x7f;
		push eax;
		add ebx, ecx;
		push ebx;
		inc esi;
		mov ebx, color;
loops3:	xor edx, edx;		// edx=0
		mov dl, [esi];		// dl<--index
		shl edx, 1;			// edx=index*2��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��
		add edx, ebx;		// edx=&color[index]�õ���ɫ���ж�Ӧ��������ɫ��ַ
		mov ax,[edx];		// ax=color[index]��ȡָ������ɫ
		mov [edi], ax;
		inc esi;
		add edi, 2;
		dec cl;
		cmp cl, 0;
		jg loops3;
		pop ebx;
		pop eax;
		jmp loops1;
		
done:
		nop
	}
	return WS_OK;
}*/
//------------------------------------------------------------------------
WSERR __stdcall blit_rle_alpha(int dx, int dy, int w, int h, int pitch, void* ptr, WPixel* color, BYTE* bits)
{
	WPixel* dataptr=(WPixel*)ptr;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + (dx<<1));
	TRANS_DRAW_FUNC* myalpha_level = alpha_level;
	__asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������
		add esi, 2;
		cld;
		align 4;
loops1:
		cmp ebx, w;			// j >= w ?
		jnl next_line;		// ������һ��

		xor ecx, ecx;
		mov cl, [esi];		// ecx <-- [esi]
		cmp ecx, 0x40;
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0x80;
		jl shadow_handle;
		cmp ecx, 0xc0;
		jl alpha_8_handle;

// normal handle
		and cl, 0x3f;
		inc esi;			// nPos += 1
		inc cl;				// rep_count = cl + 1
		add ebx, ecx;		// j += rep_count
		push eax;
		push ebx;
loops2:
		lodsw;				// ax = [esi]
		movzx edx, al;		// ��ȡ�ظ���ɫ����ɫ����index
		movzx ebx, ah;		// alpha
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		mov ax,[edx];		// ��ȡָ������ɫ
		mov edx, offset alpha_level[0];
		shl ebx, 2;
		dec cl;				// rep_count--
		add edx, ebx;
		call [edx];
		cmp cl, 0;			// rep_count > 0 ?
		jg loops2;
		pop ebx;
		pop eax;
		jmp loops1;
		
next_line:
		mov ecx, w;			// ��ȡw
		shl ecx, 1;			// w*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*w
		add edi, edx;		// edi += delta
		inc eax;			// i++
		cmp eax, h;			// �����н�����i>=h��
		jnl done;			// all done
		xor ebx, ebx;		// j=0
		add esi, 2;			// �����б�־
		jmp loops1;
		
		
mask_handle: // ͸������
		inc cl;				// rep_count = cl + 1
		inc esi;			// Դָ���ƶ�1���ֽ�
		add ebx, ecx;		// j+=rep_count
		shl ecx, 1;			// rep_count*=2
		add edi, ecx;		// Ŀ��ָ������͸��������
		jmp loops1;

shadow_handle: // Ӱ�Ӵ���
		sub cl, 0x3f;
		add ebx, ecx;
		push eax;
		push ebx;
		inc esi;
		lodsb;				// al = [esi]
		movzx ebx, al;
		mov edx, offset alpha_level[0];
		shl ebx, 2;
		add edx, ebx;

// ���ܽ���ɫĬ��Ϊ0����Ϊ���ܵ�ɫ�����б�������
		mov ebx, color;
		mov ax, [ebx];	// �õ���ɫ
loops4:	push eax;
		dec cl;				// rep_count--
		call [edx];
		pop eax;
		cmp cl, 0;			// rep_count > 0 ?
		jg loops4;

/* ԭʼ���룺
loops4:	xor eax, eax;		
		dec cl;				// rep_count--
		call [edx];
		cmp cl, 0;			// rep_count > 0 ?
		jg loops4;*/

		pop ebx;
		pop eax;
		jmp loops1;

alpha_8_handle:
		sub cl, 0x7f;
		push eax;
		add ebx, ecx;
		push ebx;
		inc esi;
		mov ebx, color;
loops3:	xor edx, edx;		// edx=0
		mov dl, [esi];		// dl<--index
		shl edx, 1;			// edx=index*2��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��
		add edx, ebx;		// edx=&color[index]�õ���ɫ���ж�Ӧ��������ɫ��ַ
		mov ax,[edx];		// ax=color[index]��ȡָ������ɫ
		mov [edi], ax;
		inc esi;
		add edi, 2;
		dec cl;
		cmp cl, 0;
		jg loops3;
		pop ebx;
		pop eax;
		jmp loops1;
		
done:
		nop
	}
	return WS_OK;
}
//------------------------------------------------------------------------
WSERR __stdcall blit_rle_alpha_trans(int dx, int dy, int w, int h, int pitch, void* ptr, WPixel* color, BYTE* bits, DWORD userdata)
{
	if (userdata <= 0) return WS_OK;
	if (userdata >= 255) return blit_rle_alpha(dx, dy, w, h, pitch, ptr, color, bits);
	TRANS_DRAW_FUNC* myalpha_level = &alpha_level_trans[userdata>>5][0];
	WPixel* dataptr=(WPixel*)ptr;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + (dx<<1));

	__asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������
		add esi, 2;
		cld;
		align 4;
loops1:
		cmp ebx, w;			// j >= w ?
		jnl next_line;		// ������һ��

		xor ecx, ecx;
		mov cl, [esi];		// ecx <-- [esi]
		cmp ecx, 0x40;
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0x80;
		jl shadow_handle;
		cmp ecx, 0xc0;
		jl alpha_8_handle;

// normal handle
		and cl, 0x3f;
		inc esi;			// nPos += 1
		inc cl;				// rep_count = cl + 1
		add ebx, ecx;		// j += rep_count
		push eax;
		push ebx;
loops2:
		lodsw;				// ax = [esi]
		movzx edx, al;		// ��ȡ�ظ���ɫ����ɫ����index
		movzx ebx, ah;		// alpha
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		mov ax,[edx];		// ��ȡָ������ɫ
		mov edx, myalpha_level;
		shl ebx, 2;
		dec cl;				// rep_count--
		add edx, ebx;
		call [edx];
		cmp cl, 0;			// rep_count > 0 ?
		jg loops2;
		pop ebx;
		pop eax;
		jmp loops1;
		
next_line:
		mov ecx, w;			// ��ȡw
		shl ecx, 1;			// w*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*w
		add edi, edx;		// edi += delta
		inc eax;			// i++
		cmp eax, h;			// �����н�����i>=h��
		jnl done;			// all done
		xor ebx, ebx;		// j=0
		add esi, 2;			// �����б�־
		jmp loops1;
		
		
mask_handle: // ͸������
		inc cl;				// rep_count = cl + 1
		inc esi;			// Դָ���ƶ�1���ֽ�
		add ebx, ecx;		// j+=rep_count
		shl ecx, 1;			// rep_count*=2
		add edi, ecx;		// Ŀ��ָ������͸��������
		jmp loops1;

shadow_handle: // Ӱ�Ӵ���
		sub cl, 0x3f;
		add ebx, ecx;
		push eax;
		push ebx;
		inc esi;
		lodsb;				// al = [esi]
		movzx ebx, al;
		mov edx, myalpha_level;
		shl ebx, 2;
		add edx, ebx;

// ���ܽ���ɫĬ��Ϊ0����Ϊ���ܵ�ɫ�����б�������
		mov ebx, color;
		mov ax, [ebx];	// �õ���ɫ
loops4:	push eax;
		dec cl;				// rep_count--
		call [edx];
		pop eax;
		cmp cl, 0;			// rep_count > 0 ?
		jg loops4;

/* ԭʼ���룺
loops4:	xor eax, eax;		
		dec cl;				// rep_count--
		call [edx];
		cmp cl, 0;			// rep_count > 0 ?
		jg loops4;*/

		pop ebx;
		pop eax;
		jmp loops1;

alpha_8_handle:
		sub cl, 0x7f;
		push eax;
		add ebx, ecx;
		push ebx;
		inc esi;
loops3:	mov ebx, color;		// blit_rle_alpha��loops3��������һ��
		xor edx, edx;		// edx=0
		mov dl, [esi];		// dl<--index
		shl edx, 1;			// edx=index*2��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��
		add edx, ebx;		// edx=&color[index]�õ���ɫ���ж�Ӧ��������ɫ��ַ
		mov ax,[edx];		// ax=color[index]��ȡָ������ɫ

		// ����3����blit_rle_alpha��û�У���Ϊalpha=8/8��ֱ�ӽ�����ɫ�滻������Ҫ����alphaЧ��
		mov edx, myalpha_level;
#ifdef _USE_32LEVEL_ALPHA
		add edx, 124;		// edx=alpha_level_tmp[31]
#else
		add edx, 28;		// edx=alpha_level_tmp[7]
#endif
		call [edx];

		//mov [edi], ax;	// blit_rle_alpha����Ч
		inc esi;
		//add edi, 2;		// blit_rle_alpha����Ч
		dec cl;
		cmp cl, 0;
		jg loops3;
		pop ebx;
		pop eax;
		jmp loops1;
		
done:
		nop
	}
	return WS_OK;
}
//------------------------------------------------------------------------
WSERR __stdcall blit_rle_clip_alpha(int dx, int dy, int w, int h, RECT* clip, int pitch, void* ptr, WPixel* color, BYTE* bits)
{
	WPixel* dataptr=(WPixel*)ptr;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + (dx<<1));
	int nClipW=clip->right-clip->left+1;
	int nClipH=clip->bottom-clip->top+1;
	
	int top=clip->top,right=clip->right,left=clip->left,bottom=clip->bottom;

	_asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		xor ecx, ecx;
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������

		// skip some line
		mov ebx, clip;
		mov ebx, [ebx]RECT.top;
		cmp ebx, 0;
		jz noskiploops;
skiploops:mov cx, word ptr [esi];
		add esi, ecx;
		inc eax;
		dec ebx;
		cmp ebx, 0;
		jg skiploops;
noskiploops:
		add esi, 2;	// skip over line header
		align 4;

loops:
		cmp ebx, w;			// һ�н�����(j<w)?
		jnl next_line;
		
		xor ecx, ecx;
		mov cl, [esi];		// ecx <-- [esi]
		cmp ecx, 0x40;
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0x80;
		jl shadow_handle;
		cmp ecx, 0xc0;
		jl alpha_8_handle;

		
//////////////////////  �����ĵ����ش���  //////////////////////////
//handle_clip:
		sub cl, 0xbf;
		add esi, 1;			// nPos += 1
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left1__;
		pop eax;
		jmp normal_handle_out_clip;
ge_left1__:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg normal_handle_out_clip;
		
//normal_handle_in_clip:
		push esi;
		push ecx;
		push eax;
		push ebx;
		push edx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2__;		// delta1<0?
		sub ecx, eax;			// count-=delta1
		shl eax, 1;
		add esi, eax;			// nPos+=2*delta1
handle_delta2__:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js normal_handle_mem_copy;// delta2<0?
		sub ecx, edx;			// count-=delta2
		
normal_handle_mem_copy:
		pop edx;
loops2:	cmp ecx, 0;
		jle noloops2;
		lodsw;				// ax = [esi]
		movzx edx, al;		// ��ȡ�ظ���ɫ����ɫ����index
		movzx ebx, ah;		// alpha
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		mov ax,[edx];		// ��ȡָ������ɫ
		mov edx, offset alpha_level[0];
		shl ebx, 2;
		dec cl;				// rep_count--
		add edx, ebx;
		call [edx];
		jmp loops2;

noloops2:
		pop ebx;
		pop eax;
		pop ecx;
		pop esi;
normal_handle_out_clip:
		add ebx, ecx;		// j+=rep_count
		shl ecx, 1;
		add esi, ecx;		// Դָ���ƶ�rep_count���ֽ�
		jmp loops;

		
//////////////////////  �Ƿ���һ��?  //////////////////////////
next_line:
		mov ecx, nClipW;	// ��ȡnClipW
		shl ecx, 1;			// nClipW*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*nClipW
		add edi, edx;		// edi += delta
		inc eax;			// i++
		add esi, 2;			// skip over line header
		cmp eax, bottom;	// �����н�����
		jg done;			// all done
		xor ebx, ebx;		// j=0
		//cmp eax, 36;
		//jz testt;
		jmp loops;
//testt:
//		jmp loops;



//////////////////////  ͸������  //////////////////////////
mask_handle: 
		inc cl;
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left1;
		pop eax;
		jmp mask_handle_out_clip;
ge_left1:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg mask_handle_out_clip;
		
//mask_handle_in_clip:
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2;		// delta1<0?
		sub ecx, eax;
handle_delta2:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js handle_mem_copy;		// delta2<0?
		sub ecx, edx;
handle_mem_copy:
		pop ebx;
		pop edx;
		shl ecx, 1;
		add edi, ecx;
		
		pop eax;
		pop ecx;
mask_handle_out_clip:
		inc esi;			// Դָ���ƶ�1���ֽ�
		add ebx, ecx;		// j+=rep_count
		jmp loops;



//////////////////////  Ӱ��������  //////////////////////////
shadow_handle: 
		sub cl, 0x3f;
		jmp handle_clip2;
handle_clip2:
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left2;
		pop eax;
		jmp shadow_handle_out_clip;
ge_left2:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jle shadow_handle_in_clip;
		jmp shadow_handle_out_clip;
shadow_handle_in_clip:
		push ecx;
		push eax;
		push ebx;
		mov edx, ebx;
		xor ebx, ebx;			// ebx=0
		mov bl, [esi+1];		// ��ȡalpha����
		push ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		cmp eax, 0;				// delta1<0?
		js handle_delta2_;
		sub ecx, eax;			// count-=delta1
handle_delta2_:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		cmp edx, 0;				// delta2<0?
		js handle_alpha_fill_;
		sub ecx, edx;			// count-=delta2
handle_alpha_fill_:
		pop ebx;
//alpha_fill:
		mov edx, offset alpha_level[0];
		shl ebx, 2;
		add edx, ebx;

//  ���ܽ���ɫĬ��Ϊ0����Ϊ���ܵ�ɫ�����б�������
		mov ebx, color;
		mov ax, [ebx];	// �õ���ɫ
loops4:	cmp ecx, 0;
		jle noloops4;
		push eax;		// ax=��ɫ
		call [edx];
		pop eax;
		dec cl;				// rep_count--
		jmp loops4;

/* ԭʼ���룺
loops4:	cmp ecx, 0;
		jle noloops4;
		xor eax, eax;		// eax=0
		call [edx];
		dec cl;				// rep_count--
		jmp loops4;*/
		
noloops4:
		pop ebx;
		pop eax;
		pop ecx;
shadow_handle_out_clip:
		add esi, 2;			// Դָ���ƶ�2���ֽ�
		add ebx, ecx;		// j+=rep_count
		jmp loops;


//////////////////////  8��alpha������  //////////////////////////
alpha_8_handle:
		sub cl, 0x7f;
		add esi, 1;			// nPos += 1
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left3;
		pop eax;
		jmp alpha_8_handle_out_clip;
ge_left3:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg alpha_8_handle_out_clip;
		
//alpha_8_handle_in_clip:
		push esi;
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2___;	// delta1<0?
		sub ecx, eax;
		add esi, eax;
handle_delta2___:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js alpha_8_handle_mem_copy;		// delta2<0?
		sub ecx, edx;
		
alpha_8_handle_mem_copy:
		pop ebx;
		pop edx;
loops3:	cmp ecx, 0;
		jle noloops3;
		xor edx, edx;
		mov dl, [esi];		// ��ȡ��������ɫ���е���ɫ����index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		mov dx,[edx];		// ��ȡָ������ɫ
		mov [edi],dx;		// д����ɫ
		add edi, 2;			// Ŀ��ָ���2
		inc esi;			// nPos++
		dec ecx;			// rep_count--
		jmp loops3;
		
noloops3:
		pop eax;
		pop ecx;
		pop esi;
alpha_8_handle_out_clip:
		add ebx, ecx;		// j+=rep_count
		add esi, ecx;		// Դָ���ƶ�rep_count���ֽ�
		jmp loops;
	
done:
		nop
	}
	return WS_OK;
}
//------------------------------------------------------------------------
WSERR __stdcall blit_rle_clip_alpha_trans(int dx, int dy, int w, int h, RECT* clip, int pitch, void* ptr, WPixel* color, BYTE* bits, DWORD userdata)
{
	if (userdata <= 0) return WS_OK;
	if (userdata >= 255) return blit_rle_clip_alpha(dx, dy, w, h, clip, pitch, ptr, color, bits);
	TRANS_DRAW_FUNC* myalpha_level = &alpha_level_trans[userdata>>5][0];
	
	WPixel* dataptr=(WPixel*)ptr;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + (dx<<1));
	int nClipW=clip->right-clip->left+1;
	int nClipH=clip->bottom-clip->top+1;
	int top=clip->top,right=clip->right,left=clip->left,bottom=clip->bottom;

	_asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		xor ecx, ecx;
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������

		// skip some line
		mov ebx, clip;
		mov ebx, [ebx]RECT.top;
		cmp ebx, 0;
		jz noskiploops;
skiploops:mov cx, word ptr [esi];
		add esi, ecx;
		inc eax;
		dec ebx;
		cmp ebx, 0;
		jg skiploops;
noskiploops:
		add esi, 2;	// skip over line header
		align 4;

loops:
		cmp ebx, w;			// һ�н�����(j<w)?
		jnl next_line;
		
		xor ecx, ecx;
		mov cl, [esi];		// ecx <-- [esi]
		cmp ecx, 0x40;
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0x80;
		jl shadow_handle;
		cmp ecx, 0xc0;
		jl alpha_8_handle;

		
//////////////////////  �����ĵ����ش���  //////////////////////////
//handle_clip:
		sub cl, 0xbf;
		add esi, 1;			// nPos += 1
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left1__;
		pop eax;
		jmp normal_handle_out_clip;
ge_left1__:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg normal_handle_out_clip;
		
//normal_handle_in_clip:
		push esi;
		push ecx;
		push eax;
		push ebx;
		push edx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2__;		// delta1<0?
		sub ecx, eax;			// count-=delta1
		shl eax, 1;
		add esi, eax;			// nPos+=2*delta1
handle_delta2__:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js normal_handle_mem_copy;// delta2<0?
		sub ecx, edx;			// count-=delta2
		
normal_handle_mem_copy:
		pop edx;
loops2:	cmp ecx, 0;
		jle noloops2;
		lodsw;				// ax = [esi]
		movzx edx, al;		// ��ȡ�ظ���ɫ����ɫ����index
		movzx ebx, ah;		// alpha
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		mov ax,[edx];		// ��ȡָ������ɫ
		mov edx, myalpha_level;
		shl ebx, 2;
		dec cl;				// rep_count--
		add edx, ebx;
		call [edx];
		jmp loops2;

noloops2:
		pop ebx;
		pop eax;
		pop ecx;
		pop esi;
normal_handle_out_clip:
		add ebx, ecx;		// j+=rep_count
		shl ecx, 1;
		add esi, ecx;		// Դָ���ƶ�rep_count���ֽ�
		jmp loops;

		
//////////////////////  �Ƿ���һ��?  //////////////////////////
next_line:
		mov ecx, nClipW;	// ��ȡnClipW
		shl ecx, 1;			// nClipW*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*nClipW
		add edi, edx;		// edi += delta
		inc eax;			// i++
		add esi, 2;			// skip over line header
		cmp eax, bottom;	// �����н�����
		jg done;			// all done
		xor ebx, ebx;		// j=0
		//cmp eax, 36;
		//jz testt;
		jmp loops;
//testt:
//		jmp loops;



//////////////////////  ͸������  //////////////////////////
mask_handle: 
		inc cl;
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left1;
		pop eax;
		jmp mask_handle_out_clip;
ge_left1:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg mask_handle_out_clip;
		
//mask_handle_in_clip:
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2;		// delta1<0?
		sub ecx, eax;
handle_delta2:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js handle_mem_copy;		// delta2<0?
		sub ecx, edx;
handle_mem_copy:
		pop ebx;
		pop edx;
		shl ecx, 1;
		add edi, ecx;
		
		pop eax;
		pop ecx;
mask_handle_out_clip:
		inc esi;			// Դָ���ƶ�1���ֽ�
		add ebx, ecx;		// j+=rep_count
		jmp loops;



//////////////////////  Ӱ��������  //////////////////////////
shadow_handle: 
		sub cl, 0x3f;
		jmp handle_clip2;
handle_clip2:
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left2;
		pop eax;
		jmp shadow_handle_out_clip;
ge_left2:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jle shadow_handle_in_clip;
		jmp shadow_handle_out_clip;
shadow_handle_in_clip:
		push ecx;
		push eax;
		push ebx;
		mov edx, ebx;
		xor ebx, ebx;			// ebx=0
		mov bl, [esi+1];		// ��ȡalpha����
		push ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		cmp eax, 0;				// delta1<0?
		js handle_delta2_;
		sub ecx, eax;			// count-=delta1
handle_delta2_:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		cmp edx, 0;				// delta2<0?
		js handle_alpha_fill_;
		sub ecx, edx;			// count-=delta2
handle_alpha_fill_:
		pop ebx;
//alpha_fill:
		mov edx, myalpha_level;
		shl ebx, 2;
		add edx, ebx;

// ���ܽ���ɫĬ��Ϊ0����Ϊ���ܵ�ɫ�����б�������
		mov ebx, color;
		mov ax, [ebx];	// �õ���ɫ
loops4:	cmp ecx, 0;
		jle noloops4;
		push eax;		// ax=��ɫ
		call [edx];
		pop eax;
		dec cl;				// rep_count--
		jmp loops4;

/* ԭʼ���룺
loops4:	cmp ecx, 0;
		jle noloops4;
		xor eax, eax;		// eax=0
		call [edx];
		dec cl;				// rep_count--
		jmp loops4;*/
		
noloops4:
		pop ebx;
		pop eax;
		pop ecx;
shadow_handle_out_clip:
		add esi, 2;			// Դָ���ƶ�2���ֽ�
		add ebx, ecx;		// j+=rep_count
		jmp loops;


//////////////////////  8��alpha������  //////////////////////////
alpha_8_handle:
		sub cl, 0x7f;
		add esi, 1;			// nPos += 1
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left3;
		pop eax;
		jmp alpha_8_handle_out_clip;
ge_left3:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg alpha_8_handle_out_clip;
		
//alpha_8_handle_in_clip:
		push esi;
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2___;	// delta1<0?
		sub ecx, eax;
		add esi, eax;
handle_delta2___:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js alpha_8_handle_mem_copy;		// delta2<0?
		sub ecx, edx;
		
alpha_8_handle_mem_copy:
		//pop ebx;
		//pop edx;
loops3:	cmp ecx, 0;
		jle noloops3;
		xor edx, edx;
		mov dl, [esi];		// ��ȡ��������ɫ���е���ɫ����index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		//mov dx,[edx];		// ��ȡָ������ɫ
		mov ax,[edx];		// ��ȡָ������ɫ

		// ����3����blit_rle_clip_alpha��û�У���Ϊalpha=8/8��ֱ�ӽ�����ɫ�滻������Ҫ����alphaЧ��
		mov edx, myalpha_level;
#ifdef _USE_32LEVEL_ALPHA
		add edx, 124;		// edx=alpha_level_clip_tmp[31]
#else
		add edx, 28;		// edx=alpha_level_clip_tmp[7]
#endif
		call [edx];

		//mov [edi],dx;		// д����ɫ
		//add edi, 2;			// Ŀ��ָ���2
		inc esi;			// nPos++
		dec ecx;			// rep_count--
		jmp loops3;
		
noloops3:
		pop ebx;
		pop edx;
		pop eax;
		pop ecx;
		pop esi;
alpha_8_handle_out_clip:
		add ebx, ecx;		// j+=rep_count
		add esi, ecx;		// Դָ���ƶ�rep_count���ֽ�
		jmp loops;
	
done:
		nop
	}
	return WS_OK;
}
//------------------------------------------------------------------------
void blit_rle_copy_trans(int dx, int dy, int w, int h, int pitch, void* ptr, WPixel* color, BYTE* bits, DWORD userdata)
{
	WPixel* dataptr=(WPixel*)ptr;
	int x_disc = dx<<1;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + x_disc);
	TRANS_DRAW_FUNC alpha_draw;
	alpha_draw = alpha_level[userdata>>SHIFT];
	
	_asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������
		add esi, 2;
		align 4;
		
lineloops:
		cmp ebx, w;			// һ�н�����(j<w)?
		jnl next_line;
		
		xor ecx,ecx;		// ecx=0
		mov cl, [esi];		// ��Դ�ж�ȡһ���ֽڵ�cl
		cmp ecx, 0x80;		// �Ƿ���͸����־��
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0xc0;		// �Ƿ����ظ�����ɫ��
		jl repeat_handle;	// �ǣ���jump��repeat_handle
		
	//normal_handle:
		sub cl, 0xbf;
		add esi, 1;			// nPos += 1
		add ebx, ecx;		// j+=rep_count
		push ax;
		push bx;
loops2:	xor edx, edx;
		mov dl, [esi];		// ��ȡ��������ɫ���е���ɫ����index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		mov ax, [edx];
		call alpha_draw;
		inc esi;			// nPos++
		loop loops2;
		pop bx;
		pop ax;
		jmp lineloops;
		
next_line:
		mov ecx, w;			// ��ȡw
		shl ecx, 1;			// w*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*w
		add edi, edx;		// edi += delta
		add eax, 1;			// i++
		cmp eax, h;			// �����н�����
		jnl done;			// all done
		xor ebx, ebx;		// j=0
		add esi, 2;
		jmp lineloops;
		
// ����͸��
mask_handle:
		inc cl;
		add ebx, ecx;		// j+=ecx
		push ax;
		push bx;
loop_mask_trans:
		mov ax, 0xf81f;
		call alpha_draw;
		loop loop_mask_trans;
		pop bx;
		pop ax;
		inc esi;
		jmp lineloops;

// ����������ͬ������
repeat_handle:
		xor edx, edx;		// edx=0
		sub cl, 0x7f;		// cl = cl - 127
		mov dl, [esi+1];	// ��ȡ�ظ���ɫ����ɫ����dl
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ������*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		add ebx, ecx;		// j+=ecx
		push ax;
		push bx;
loop_repeat_trans:
		mov ax, [edx];
		call alpha_draw;
		loop loop_repeat_trans;
		pop bx;
		pop ax;
		add esi, 2;
		jmp lineloops;
		
done:
		nop
	}
}
//------------------------------------------------------------------------
void blit_rle_mask_trans(int dx, int dy, int w, int h, int pitch, void* ptr, WPixel* color, BYTE* bits, DWORD userdata)
{
	WPixel* dataptr=(WPixel*)ptr;
	int x_disc = dx<<1;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + x_disc);
	TRANS_DRAW_FUNC alpha_draw;
	alpha_draw = alpha_level[userdata>>SHIFT];

	_asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������
		add esi, 2;
		align 4;
		
lineloops:
		cmp ebx, w;			// һ�н�����(j<w)?
		jnl next_line;
		
		xor ecx,ecx;		// ecx=0
		mov cl, [esi];		// ��Դ�ж�ȡһ���ֽڵ�cl
		cmp ecx, 0x80;		// �Ƿ���͸����־��
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0xc0;		// �Ƿ����ظ�����ɫ��
		jl repeat_handle;	// �ǣ���jump��repeat_handle
		
//normal_handle:
		sub cl, 0xbf;
		add esi, 1;			// nPos += 1
		add ebx, ecx;		// j+=rep_count
		push ax;
		push bx;
loops2:	xor edx, edx;
		mov dl, [esi];		// ��ȡ��������ɫ���е���ɫ����index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		mov ax, [edx];
		call alpha_draw;
		inc esi;			// nPos++
		loop loops2;
		pop bx;
		pop ax;
		jmp lineloops;
		
next_line:
		mov ecx, w;			// ��ȡw
		shl ecx, 1;			// w*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*w
		add edi, edx;		// edi += delta
		add eax, 1;			// i++
		cmp eax, h;			// �����н�����
		jnl done;			// all done
		xor ebx, ebx;		// j=0
		add esi, 2;
		jmp lineloops;
		
// ����͸��
mask_handle: 
		inc cl;
		inc esi;
		add ebx, ecx;		// j+=ecx
		shl ecx, 1;			// cl*2
		add edi, ecx;		// Ŀ��ָ������͸��������
		jmp lineloops;
		
// ����������ͬ������
repeat_handle:
		xor edx, edx;		// edx=0
		sub cl, 0x7f;		// cl = cl - 127
		mov dl, [esi+1];	// ��ȡ�ظ���ɫ����ɫ����dl
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ������*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		add ebx, ecx;		// j+=ecx
		push ax;
		push bx;
loop_repeat_trans:
		mov ax, [edx];
		call alpha_draw;
		loop loop_repeat_trans;
		pop bx;
		pop ax;
		add esi, 2;
		jmp lineloops;
		
done:
		nop
	}
}
//------------------------------------------------------------------------
void blit_rle_clip_copy_trans(int dx, int dy, int w, int h, RECT& clip, int pitch, void* ptr, WPixel* color, BYTE* bits, DWORD userdata)
{
	WPixel* dataptr=(WPixel*)ptr;
	int x_disc = dx<<1;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + x_disc);
	int nClipW=clip.right-clip.left+1;
	int nClipH=clip.bottom-clip.top+1;
	int top=clip.top,right=clip.right,left=clip.left,bottom=clip.bottom;

	TRANS_DRAW_FUNC alpha_draw;
	alpha_draw = alpha_level[userdata>>SHIFT];

	_asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		xor ecx, ecx;
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������

		// skip some line
		mov ebx, clip;
		mov ebx, [ebx]RECT.top;
		cmp ebx, 0;
		jz noskiploops;
skiploops:mov cx, word ptr [esi];
		add esi, ecx;
		inc eax;
		dec ebx;
		cmp ebx, 0;
		jg skiploops;
noskiploops:
		add esi, 2;
		align 4;

loops:
		cmp ebx, w;			// һ�н�����(j<w)?
		jnl next_line;
		
		xor ecx,ecx;		// ecx=0
		mov cl, [esi];		// ��Դ�ж�ȡһ���ֽڵ�cl
		cmp ecx, 0x80;		// �Ƿ���͸����־��
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0xc0;		// �Ƿ����ظ�����ɫ��
		jl repeat_handle;	// �ǣ���jump��repeat_handle

		
//////////////////////  �����ĵ����ش���  //////////////////////////
//handle_clip:
		sub cl, 0xbf;
		add esi, 1;			// nPos += 1
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left1__;
		pop eax;
		jmp normal_handle_out_clip;
ge_left1__:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg normal_handle_out_clip;
		
//normal_handle_in_clip:
		push esi;
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2__;		// delta1<0?
		sub ecx, eax;
		add esi, eax;
handle_delta2__:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js normal_handle_mem_copy;		// delta2<0?
		sub ecx, edx;
		
normal_handle_mem_copy:
		pop ebx;
		pop edx;
loops2:	cmp ecx, 0;
		jle noloops2;
		xor edx, edx;
		mov dl, [esi];		// ��ȡ��������ɫ���е���ɫ����index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		//mov dx,[edx];		// ��ȡָ������ɫ
		//mov [edi],dx;		// д����ɫ
		//add edi, 2;			// Ŀ��ָ���2
		push ax;
		push bx;
		mov ax, [edx];
		call alpha_draw;
		pop bx;
		pop ax;

		inc esi;			// nPos++
		dec ecx;			// rep_count--
		jmp loops2;

noloops2:
		pop eax;
		pop ecx;
		pop esi;
normal_handle_out_clip:
		add ebx, ecx;		// j+=rep_count
		add esi, ecx;		// Դָ���ƶ�rep_count���ֽ�
		jmp loops;

		
//////////////////////  �Ƿ���һ��?  //////////////////////////
next_line:
		mov ecx, nClipW;	// ��ȡnClipW
		shl ecx, 1;			// nClipW*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*nClipW
		add edi, edx;		// edi += delta
		add eax, 1;			// i++
		add esi, 2;
		cmp eax, bottom;	// �����н�����
		jg done;			// all done
		xor ebx, ebx;		// j=0
		jmp loops;


//////////////////////  ͸������  //////////////////////////
mask_handle: 
		inc cl;
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left1;
		pop eax;
		jmp mask_handle_out_clip;
ge_left1:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg mask_handle_out_clip;

//mask_handle_in_clip:
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2;		// delta1<0?
		sub ecx, eax;
handle_delta2:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js handle_mem_copy;		// delta2<0?
		sub ecx, edx;
handle_mem_copy:
		pop ebx;
		pop edx;			

		push ax;
		push bx;
loop_mask_trans:
		mov ax, 0xf81f;
		call alpha_draw;
		loop loop_mask_trans;
		pop bx;
		pop ax;
		
//memset16_done_for_mask:
		pop eax;
		pop ecx;
mask_handle_out_clip:
		inc esi;			// Դָ���ƶ�1���ֽ�
		add ebx, ecx;		// j+=rep_count
		jmp loops;



//////////////////////  �ظ����ش���  //////////////////////////
repeat_handle: 
		sub cl, 0x7f;
		jmp handle_clip2;
handle_clip2:
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left2;
		pop eax;
		jmp repeat_handle_out_clip;
ge_left2:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jle repeat_handle_in_clip;
		jmp repeat_handle_out_clip;
repeat_handle_in_clip:
		xor edx, edx;		// edx=0
		mov dl, [esi+1];	// ��ȡ�ظ���ɫ����ɫ����dl,edx <- index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ������*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��,edx = color + 2*index

		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		cmp eax, 0;				// delta1<0?
		js handle_delta2_;
		sub ecx, eax;
handle_delta2_:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		cmp edx, 0;				// delta2<0?
		js handle_mem_copy_;
		sub ecx, edx;
handle_mem_copy_:
		pop ebx;
		pop edx;

		push ax;
		push bx;
loop_repeat_trans:
		mov ax, [edx];
		call alpha_draw;
		loop loop_repeat_trans;
		pop bx;
		pop ax;
	
//memset16_done:
		pop eax;
		pop ecx;
repeat_handle_out_clip:
		add esi, 2;			// Դָ���ƶ�2���ֽ�
		add ebx, ecx;		// j+=rep_count
		jmp loops;
	
done:
		nop
	}
}
//------------------------------------------------------------------------
void blit_rle_clip_mask_trans(int dx, int dy, int w, int h, RECT& clip, int pitch, void* ptr, WPixel* color, BYTE* bits, DWORD userdata)
{
	WPixel* dataptr=(WPixel*)ptr;
	int x_disc = dx<<1;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + x_disc);
	int nClipW=clip.right-clip.left+1;
	int nClipH=clip.bottom-clip.top+1;
	int top=clip.top,right=clip.right,left=clip.left,bottom=clip.bottom;
	
	TRANS_DRAW_FUNC alpha_draw;
	alpha_draw = alpha_level[userdata>>SHIFT];

	_asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		xor ecx, ecx;
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������

		// skip some line
		mov ebx, clip;
		mov ebx, [ebx]RECT.top;
		cmp ebx, 0;
		jz noskiploops;
skiploops:mov cx, word ptr [esi];
		add esi, ecx;
		inc eax;
		dec ebx;
		cmp ebx, 0;
		jg skiploops;
noskiploops:
		add esi, 2;
		align 4;

loops:
		cmp ebx, w;			// һ�н�����(j<w)?
		jnl next_line;
		
		xor ecx,ecx;		// ecx=0
		mov cl, [esi];		// ��Դ�ж�ȡһ���ֽڵ�cl
		cmp ecx, 0x80;		// �Ƿ���͸����־��
		jl mask_handle;		// �ǣ���jump��mask_handle
		cmp ecx, 0xc0;		// �Ƿ����ظ�����ɫ��
		jl repeat_handle;	// �ǣ���jump��repeat_handle

		
//////////////////////  �����ĵ����ش���  //////////////////////////
//handle_clip:
		sub cl, 0xbf;
		add esi, 1;			// nPos += 1
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left1__;
		pop eax;
		jmp normal_handle_out_clip;
ge_left1__:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg normal_handle_out_clip;
		
//normal_handle_in_clip:
		push esi;
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2__;		// delta1<0?
		sub ecx, eax;
		add esi, eax;
handle_delta2__:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js normal_handle_mem_copy;		// delta2<0?
		sub ecx, edx;
		
normal_handle_mem_copy:
		pop ebx;
		pop edx;
loops2:	cmp ecx, 0;
		jle noloops2;
		xor edx, edx;
		mov dl, [esi];		// ��ȡ��������ɫ���е���ɫ����index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ��index*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��
		//mov dx,[edx];		// ��ȡָ������ɫ
		//mov [edi],dx;		// д����ɫ
		//add edi, 2;			// Ŀ��ָ���2
		push ax;
		push bx;
		mov ax, [edx];
		call alpha_draw;
		pop bx;
		pop ax;
		
		inc esi;			// nPos++
		dec ecx;			// rep_count--
		jmp loops2;

noloops2:
		pop eax;
		pop ecx;
		pop esi;
normal_handle_out_clip:
		add ebx, ecx;		// j+=rep_count
		add esi, ecx;		// Դָ���ƶ�rep_count���ֽ�
		jmp loops;

		
//////////////////////  �Ƿ���һ��?  //////////////////////////
next_line:
		mov ecx, nClipW;	// ��ȡnClipW
		shl ecx, 1;			// nClipW*2
		mov edx, pitch;		// ��ȡpitch
		sub edx, ecx;		// delta = pitch-2*nClipW
		add edi, edx;		// edi += delta
		add eax, 1;			// i++
		add esi, 2;
		cmp eax, bottom;	// �����н�����
		jg done;			// all done
		xor ebx, ebx;		// j=0
		jmp loops;


//////////////////////  ͸������  //////////////////////////
mask_handle: 
		inc cl;
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left1;
		pop eax;
		jmp mask_handle_out_clip;
ge_left1:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jg mask_handle_out_clip;

//mask_handle_in_clip:
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		js handle_delta2;		// delta1<0?
		sub ecx, eax;
handle_delta2:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		js handle_mem_copy;		// delta2<0?
		sub ecx, edx;
handle_mem_copy:
		pop ebx;
		pop edx;
		shl ecx, 1;
		add edi, ecx;
		
		pop eax;
		pop ecx;
mask_handle_out_clip:
		inc esi;			// Դָ���ƶ�1���ֽ�
		add ebx, ecx;		// j+=rep_count
		jmp loops;



//////////////////////  �ظ����ش���  //////////////////////////
repeat_handle: 
		sub cl, 0x7f;
		jmp handle_clip2;
handle_clip2:
		push eax;
		mov eax, left;
		sub eax, ecx; // eax=clip.left-rep_count
		cmp ebx, eax; // j>clip.left-rep_count?
		jg ge_left2;
		pop eax;
		jmp repeat_handle_out_clip;
ge_left2:
		pop eax;
		cmp ebx, right; // j<=clip.right?
		jle repeat_handle_in_clip;
		jmp repeat_handle_out_clip;
repeat_handle_in_clip:
		xor edx, edx;		// edx=0
		mov dl, [esi+1];	// ��ȡ�ظ���ɫ����ɫ����dl,edx <- index
		shl edx, 1;			// ��Ϊ��ɫ����һ����ɫ��2���ֽڣ�����Ҫ������*2
		add edx, color;		// �õ���ɫ���ж�Ӧ������λ��,edx = color + 2*index

		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;
		mov ebx, ecx;
		mov eax, left;
		sub eax, edx;			// delta1=clip.left-j
		cmp eax, 0;				// delta1<0?
		js handle_delta2_;
		sub ecx, eax;
handle_delta2_:
		add edx, ebx;
		sub edx, right;
		sub edx, 1;				// delta2=j+rep_count-clip.right-1
		cmp edx, 0;				// delta2<0?
		js handle_mem_copy_;
		sub ecx, edx;
handle_mem_copy_:
		pop ebx;
		pop edx;

		push ax;
		push bx;
loop_repeat_trans:
		mov ax, [edx];
		call alpha_draw;
		loop loop_repeat_trans;
		pop bx;
		pop ax;

//memset16_done:
		pop eax;
		pop ecx;
repeat_handle_out_clip:
		add esi, 2;			// Դָ���ƶ�2���ֽ�
		add ebx, ecx;		// j+=rep_count
		jmp loops;
	
done:
		nop
	}
}
//------------------------------------------------------------------------