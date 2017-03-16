/**
*	created:		2012-6-20   18:41
*	filename: 		RleAsm16
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../../FKTrace/IFKTrace.h"
#include "../IFKCanvas.h"
#include "Asm.h"
//------------------------------------------------------------------------
/** 256��tga��Ⱦ��565ģʽʱ��alpha����ԭ��

	//green
	//i=sg*a + (63-a)*dg;
	//i=(i+32)+((i+32)>>6)>>6;

	//red,blue
	//i=sr*a + (31-a)*dr;
	//i=(i+16)+((i+16)>>5)>>5;

** ����������Ѿ��������´���(ԴԤ��)
	src = src * 255 / alpha;
	if (src > 255) src = 255;
	src = src * alpha + 128;
	src = (src + src / 256) / 256;

** ��������ϱ�Ϊ�� 
	//green
	//i=(63-a)*dg;
	//i=(i+32)+((i+32)>>6)>>6;
	//green=sg+i;

	//red,blue
	//i=sr*a + (31-a)*dr;
	//i=(i+16)+((i+16)>>5)>>5;
	//red(blue)=sr(sb)+i;
*/
//------------------------------------------------------------------------
__int64 _16=0x0010001000100010;		// 16
__int64 _32=0x0020002000200020;		// 32
__int64 _31=0x001f001f001f001f;		// 31
__int64 _63=0x003f003f003f003f;		// 63
//------------------------------------------------------------------------
WSERR __stdcall blit_rle_alpha16(int dx, int dy, int w, int h, int pitch, void* ptr, BYTE* bits)
{
	WPixel* dataptr=(WPixel*)ptr;
	int x_disc = dx<<1;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + x_disc);
	int pitch1 = pitch - 2*w;
	int w4;

	__asm
	{
		xor eax, eax;		// i=0, �߶ȼ���
		xor ebx, ebx;		// j=0, ��ȼ���
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������
		add esi, 2;			// ����ÿ�е������ֽ�
		cld;
		align 4;

_loop:
		cmp ebx, w;			// j >= w ?
		jge _next_line;		// ������һ��

		xor ecx, ecx;		// ecx = 0
		mov cl, [esi];		// ecx <-- [esi]
		cmp ecx, 0x80;		// ecx < 0x80 ?
		jl _mask_handle;	// �ǣ���jump��mask_handle

// alpha�������
		pcmpeqd mm6,mm6;		// mm6=0xFFFFFFFFFFFFFFFF
		sub cl, 0x7f;			// rep_count;
		inc esi;				// nPos += 1
		add ebx, ecx;			// j += rep_count
		push eax;
		push ebx;
		mov edx, ecx;			// ��ȡw4��ֵ
		sub edx, 4;
		mov w4, edx;
		lea edx, [esi+ecx*2];	// edx = ����λ��
		xor ebx,ebx;			// ����
		// ��ȡ�ü�����
		mov eax, ecx;			// eax=rep_count
		and eax,3				// eax=rep_count/4������
		shl eax,4				// ��������Ӧ��λ��
		movd mm5,eax			// mm5=eax
		psllq mm6,mm5			// mm6������β���ò���4������ʱ���вü�	
		align 4;

_loop1:
		mov eax,[edx+ebx];		// ����4��alpha, 
		test eax, eax;			// alpha == 0 ?
		jz _4pixel;				// ˵��4�����ص��ԭ����alpha��Ϊ32��ֻ��Դ���أ�����alpha���
		/*
		pxor mm4,mm4;			// mm4=0
		movd mm7,eax;			// mm7=4��(32-alpha)=0x00000000 b4b3b2b1
		movq mm0,[edi+ebx*2];	// Ŀ������
		punpcklbw mm7,mm4;		// mm7 = 0x00b4 00b3 00b2 00b1; byte->word
		movq mm1,mm0;			// mm1=mm0=Ŀ������
		movq mm4,mm0;			// mm4=mm0=Ŀ������
		movq mm2,mask_b;		// mm2=0x001f 001f 001f 001f (��ɫ����)
		psrlw mm0,11;			// mm0 = red (��word����11λ)
		pand mm2,mm4;			// mm2 = blue
		pand mm1,mask_g;		// mm1 = green(������û����λ���� gggggg00000)
		pmullw mm0,mm7;			// mm0=red*(32-alpha)
		psrlw mm1,5;			// mm1=green
		pmullw mm2,mm7;			// mm2=blue*(32-alpha)
		psllw mm0,3;			// mm0=red<<6(�н�λ��ʵ������<<11,Ȼ��>>5(�Ǻ�,��Ҫ����32��))
		pmullw mm1,mm7;			// mm1=green*(32-alpha)
		psrlw mm2,8;			// mm2=blue>>5(�Ǻ�,��Ҫ����32��)
		psrlw mm1,3;			// mm1=green>>3(ʵ������<<5,Ȼ��>>5(�Ǻ�,��Ҫ����32��))
		pand mm0,mask_r;		// red����?
		pand mm1,mask_g;		// green����?
		por mm0,mm2;			// color = red|blue
		por mm0,mm1;			// color = red|green|blue,��ʱcolor = ��*dst/32, ����32-alpha
		*/
		pxor mm4,mm4				// a1:mm4=0
		movd mm5,[edx+ebx]			// a2:mm5=00 00 00 00 a3 a2 a1 a0
		punpcklbw mm5,mm4			// a3:mm5=00a3 00a2 00a1 00a0;byte->word
		movq mm0,[edi+ebx*2]		// r1:mm0=dst3 dst2 dst1 dst0
		psrlw mm5,2					// a4:mm5=a?/4,��ɫ������64��alpha����
		movq mm7,_63				// a5:mm7=63 63 63 63
		movq mm1,mm0				// g1:mm1=dst3 dst2 dst1 dst0
		psubw mm7,mm5				// a6:mm7=63-a?
		movq mm2,mm0				// b1:mm2=dst3 dst2 dst1 dst0
		pand mm1,mask_g				// g2a:mm1=dg?(gggggg00000)
		psrlw mm1,5					// g2b:mm1=dg3 dg2 dg1 dg0
		pmullw mm1,mm7				// g3:mm1=dg?*(63-a?)
		psrlw mm5,1					// a7:mm5=a?/2��(��)ɫ������32��alpha����
		paddw mm1,_32				// g4:mm1=dg?+32
		movq mm7,_31				// a8:mm7=31 31 31 31
		movq mm4,mm1				// g5:mm4=(dg?+32)
		psubw mm7,mm5				// a9:mm7=31-a?
		psrlw mm4,6					// g6:mm4=(dg?+32)/64
		psrlw mm0,11				// r2:mm0=dr3 dr2 dr1 dr0
		paddw mm1,mm4				// g7:mm1=((dg?+32)+(dg?+32)/64)
		pmullw mm0,mm7				// r3:mm0=dr?*(31-a?)
		psrlw mm1,1					// g8:mm1=green>>1(ʵ������<<5(565),Ȼ��>>6(alpha))
		pand mm2,mask_b				// b2:mm2=db3 db2 db1 db0
		paddw mm0,_16				// r4:mm0=dr?+16
		pmullw mm2,mm7				// b3:mm2=db?*(31-a?)
		movq mm3,mm0				// r5:mm3=(dr?+16)
		paddw mm2,_16				// b4:mm2=db?+16
		psrlw mm3,5					// r6:mm3=(dr?+16)/32
		movq mm5,mm2				// b5:mm5=(db?+16)
		paddw mm0, mm3				// r7:mm0=((dr?+16)+(dr?+16)/32)
		psrlw mm5,5					// b6:mm5=(db?+16)/32
		psllw mm0,6					// r8:mm0=red<<6(�н�λ��ʵ������<<11(565),Ȼ��>>5(alpha)
		paddw mm2,mm5				// b7:mm2=((db?+16)+(db?+16)/32)
		pand mm0,mask_r				// r9:mm0=0r00 0r00 0r00 0r00
		psrlw mm2,5					// b8:mm2=blue>>5(alpha)
		pand mm1,mask_g				// g10:mm1=00g0 00g0 00g0 00g0
		por mm0,mm2					// mm0=0r0b 0r0b 0r0b 0r0b
		por mm0,mm1					// mm0=0rgb 0rgb 0rgb 0rgb 

		cmp ebx,w4;				// ebx<=w4?
		paddw mm0,[esi+ebx*2]	// mm0=Դ���غ�Ŀ�����ص�alpha���(Դ������λͼ����ʱ��������alpha���,����Ͳ���Ҫ�����)
		jle _not_tail			// ecx<=w4,��û�е���ĩ
		align 4;

_clip: // ������β��û��4����blit��Ҫ���вü�
		movq mm4,[edi+ebx*2]	// mm4=dest
		pand mm4,mm6;			// ��Ŀ����������Ч�ĵ���0x0000�滻
		psubusw mm0,mm6;		// ��mm0����Ч�ĵ���0x0000�滻
		por mm0,mm4;			// Դ��Ŀ�����ػ��
		movq [edi+ebx*2],mm0;	// д���Ϻ������
		jmp _alph_blit_end;
		align 4;

_4pixel: // �ƶ�4������alpha��ϼ����4�����ص�
		cmp ebx,w4;				// ebx<=w4?
		movq mm0,[esi+ebx*2];	// mm0=Դ����
		jle _neednotclip;		// ebx<=w4,����ü�
		movq mm4,[edi+ebx*2];	// ��Ҫ�ü�,mm4=Ŀ������
		jmp _clip;				// ����ü�
		align 4;
		
_neednotclip: // ����Ҫ�ü������ݴ���
		movq [edi+ebx*2],mm0;	// ���������д��Ŀ��λͼ
		add ebx,4;				// ������4
		jmp _loop1;
		align 4;
		
_not_tail: // ��û�е���β
		movq [edi+ebx*2],mm0	// ���������д��Ŀ��λͼ
		add ebx,4				// ������4
		jmp _loop1;
		align 4;

_alph_blit_end:
		add esi, ecx;
		shl ecx, 1;
		add esi, ecx;
		add edi, ecx;
		pop ebx;
		pop eax;
		jmp _loop;
		align 4;
		
_next_line:	// ��һ�д���
		mov edx, pitch1;	// ��ȡpitch1
		add edi, edx;		// edi += pitch1
		inc eax;			// i++
		cmp eax, h;			// �����н�����i>=h��
		jge _done;			// all done
		xor ebx, ebx;		// j=0
		add esi, 2;			// �����б�־
		jmp _loop;
		align 4
		
_mask_handle: // ͸������
		inc cl;				// rep_count = cl + 1
		inc esi;			// Դָ���ƶ�1���ֽ�
		add ebx, ecx;		// j+=rep_count
		shl ecx, 1;			// rep_count*=2
		add edi, ecx;		// Ŀ��ָ������͸��������
		jmp _loop;
		align 4
	
_done:
		emms;
	}
	
	return WS_OK;
}
//------------------------------------------------------------------------
WSERR __stdcall blit_rle_clip_alpha16(int dx, int dy, int w, int h, RECT* clip, int pitch, void* ptr, BYTE* bits)
{
	WPixel* dataptr=(WPixel*)ptr;
	WPixel* startptr = (WPixel*)((DWORD)dataptr + dy*pitch + (dx<<1));
	int nClipW=clip->right-clip->left+1;
	int nClipH=clip->bottom-clip->top+1;
	int top=clip->top,right=clip->right,left=clip->left,bottom=clip->bottom;

	int pitch1 = pitch - 2*nClipW;
	int w4;

	_asm
	{
		xor eax, eax;		// i=0
		xor ebx, ebx;		// j=0
		xor ecx, ecx;		// ecx = 0
		mov esi, bits;		// Դ����
		mov edi, startptr;	// Ŀ������

		// skip some line
		mov ebx, clip;			// ebx = clip�ĵ�ַ
		mov ebx, [ebx]RECT.top; // ebx = clip.top
		cmp ebx, 0;				// clip.top == 0 ?
		jz _noskiploops;
_skiploops:
		mov cx, word ptr [esi];	// ��ȡÿ��ѹ����ĳ���
		add esi, ecx;			// Դָ������cx����
		inc eax;				// i++
		dec ebx;				// ebx--
		cmp ebx, 0;				// �ж�ѭ���Ƿ����
		jg _skiploops;
_noskiploops:
		add esi, 2;				// skip over line header
		align 4;

_loop:
		cmp ebx, w;			// һ�н�����(j<w)?
		jnl _next_line;
		
		xor ecx, ecx;		// ecx = 0
		mov cl, [esi];		// ecx <-- [esi]
		cmp ecx, 0x80;		// ecx < 0x80 ?
		jl _mask_handle;	// �ǣ���jump��mask_handle

		
//////////////////////  �����ĵ����ش���  //////////////////////////
//_handle_clip:
		sub cl, 0x7f;				// cl = rep_count
		add esi, 1;					// nPos += 1
		mov edx, left;				// edx = clip.left
		sub edx, ecx;				// edx = clip.left - rep_count
		cmp ebx, edx;				// j > clip.left - rep_count ?
		jg _ge_left2;
		jmp _normal_handle_out_clip;
_ge_left2:
		cmp ebx, right;				// j <= clip.right ?
		jg _normal_handle_out_clip;
		
//_normal_handle_in_clip:
		push esi;
		push ecx;
		push eax;
		push ebx;
		mov eax, ecx;				// eax = rep_count
		mov edx, ecx;				// edx = rep_count
		add edx, ebx;				// edx = rep_count + j
		sub edx, right;				// edx = rep_count + j - clip.right
		sub edx, 1;					// edx = delta2 = j + rep_count - clip.right - 1
		js _handle_delta1;
		sub ecx, edx;				// rep_count -= delta2
_handle_delta1:
		lea edx, [esi+eax*2];		// edx = ����λ��
		mov eax, left;				// eax = clip.left
		sub eax, ebx;				// eax = delta1 = clip.left - j
		js _normal_handle_mem_copy;	// delta1 < 0 ?
		sub ecx, eax;				// rep_count -= delta1
		add edx, eax;				// alpha��ַƫ��delta1
		shl eax, 1;					// eax = delta1*2
		add esi, eax;				// nPos += delta1*2
		
_normal_handle_mem_copy:
		// alpha�������,ecx=�������rep_count,edx=alphaλ��
		pcmpeqd mm6,mm6;		// mm6=0xFFFFFFFFFFFFFFFF
		mov ebx, ecx;			// ��ȡw4��ֵ
		sub ebx, 4;
		mov w4, ebx;
		xor ebx,ebx;			// ebx���ڼ���
		// ��ȡ�ü�����
		mov eax, ecx;			// eax=rep_count
		and eax,3				// eax=rep_count/4������
		shl eax,4				// ��������Ӧ��λ��
		movd mm5,eax			// mm5=eax
		psllq mm6,mm5			// mm6������β���ò���4������ʱ���вü�	
			
_loop1:
		mov eax,[edx+ebx];		// ����4��alpha, 
		test eax, eax;			// alpha == 0 ?
		jz _4pixel;				// ˵��4�����ص��ԭ����alpha��Ϊ255��ֻ��Դ���أ�����alpha���
/*
		pxor mm4,mm4;			// mm4=0
		movd mm7,eax;			// mm7=4��(255-alpha)=0x00000000 b4b3b2b1
		movq mm0,[edi+ebx*2];	// Ŀ������
		punpcklbw mm7,mm4;		// mm7 = 0x00b4 00b3 00b2 00b1; byte->word
		movq mm1,mm0;			// mm1=mm0=Ŀ������
		movq mm4,mm0;			// mm4=mm0=Ŀ������
		movq mm2,mask_b;		// mm2=0x001f 001f 001f 001f (��ɫ����)
		psrlw mm0,11;			// mm0 = red (��word����11λ)
		pand mm2,mm4;			// mm2 = blue
		pand mm1,mask_g;		// mm1 = green(������û����λ���� gggggg00000)
		pmullw mm0,mm7;			// mm0=red*(255-alpha)
		psrlw mm1,5;			// mm1=green
		pmullw mm2,mm7;			// mm2=blue*(255-alpha)
		psllw mm0,3;			// mm0=red<<3(�н�λ��ʵ������<<11,Ȼ��>>8(�Ǻ�,��Ҫ����256��))
		pmullw mm1,mm7;			// mm1=green*(255-alpha)
		psrlw mm2,8;			// mm2=blue>>8(�Ǻ�,��Ҫ����256��)
		psrlw mm1,3;			// mm1=green>>3(ʵ������<<5,Ȼ��>>8(�Ǻ�,��Ҫ����256��))
		pand mm0,mask_r;		// red����?
		pand mm1,mask_g;		// green����?
		por mm0,mm2;			// color = red|blue
		por mm0,mm1;			// color = red|green|blue,��ʱcolor = ��*dst/256, ����256-alpha
		*/
		pxor mm4,mm4				// a1:mm4=0
		movd mm5,[edx+ebx]			// a2:mm5=00 00 00 00 a3 a2 a1 a0
		punpcklbw mm5,mm4			// a3:mm5=00a3 00a2 00a1 00a0;byte->word
		movq mm0,[edi+ebx*2]		// r1:mm0=dst3 dst2 dst1 dst0
		psrlw mm5,2					// a4:mm5=a?/4,��ɫ������64��alpha����
		movq mm7,_63				// a5:mm7=63 63 63 63
		movq mm1,mm0				// g1:mm1=dst3 dst2 dst1 dst0
		psubw mm7,mm5				// a6:mm7=63-a?
		movq mm2,mm0				// b1:mm2=dst3 dst2 dst1 dst0
		pand mm1,mask_g				// g2a:mm1=dg?(gggggg00000)
		psrlw mm1,5					// g2b:mm1=dg3 dg2 dg1 dg0
		pmullw mm1,mm7				// g3:mm1=dg?*(63-a?)
		psrlw mm5,1					// a7:mm5=a?/2��(��)ɫ������32��alpha����
		paddw mm1,_32				// g4:mm1=dg?+32
		movq mm7,_31				// a8:mm7=31 31 31 31
		movq mm4,mm1				// g5:mm4=(dg?+32)
		psubw mm7,mm5				// a9:mm7=31-a?
		psrlw mm4,6					// g6:mm4=(dg?+32)/64
		psrlw mm0,11				// r2:mm0=dr3 dr2 dr1 dr0
		paddw mm1,mm4				// g7:mm1=((dg?+32)+(dg?+32)/64)
		pmullw mm0,mm7				// r3:mm0=dr?*(31-a?)
		psrlw mm1,1					// g8:mm1=green>>1(ʵ������<<5(565),Ȼ��>>6(alpha))
		pand mm2,mask_b				// b2:mm2=db3 db2 db1 db0
		paddw mm0,_16				// r4:mm0=dr?+16
		pmullw mm2,mm7				// b3:mm2=db?*(31-a?)
		movq mm3,mm0				// r5:mm3=(dr?+16)
		paddw mm2,_16				// b4:mm2=db?+16
		psrlw mm3,5					// r6:mm3=(dr?+16)/32
		movq mm5,mm2				// b5:mm5=(db?+16)
		paddw mm0, mm3				// r7:mm0=((dr?+16)+(dr?+16)/32)
		psrlw mm5,5					// b6:mm5=(db?+16)/32
		psllw mm0,6					// r8:mm0=red<<6(�н�λ��ʵ������<<11(565),Ȼ��>>5(alpha)
		paddw mm2,mm5				// b7:mm2=((db?+16)+(db?+16)/32)
		pand mm0,mask_r				// r9:mm0=0r00 0r00 0r00 0r00
		psrlw mm2,5					// b8:mm2=blue>>5(alpha)
		pand mm1,mask_g				// g10:mm1=00g0 00g0 00g0 00g0
		por mm0,mm2					// mm0=0r0b 0r0b 0r0b 0r0b
		por mm0,mm1					// mm0=0rgb 0rgb 0rgb 0rgb 

		cmp ebx,w4				// ebx<=w4?
		paddw mm0,[esi+ebx*2]	// mm0=Դ���غ�Ŀ�����ص�alpha���(Դ������λͼ����ʱ��������alpha���,����Ͳ���Ҫ�����)
		jle _not_tail			// ecx<=w4,��û�е���ĩ
			
_clip: // ������β��û��4����blit��Ҫ���вü�
		movq mm4,[edi+ebx*2]	// mm4=dest
		pand mm4,mm6;			// ��Ŀ����������Ч�ĵ���0x0000�滻
		psubusw mm0,mm6;		// ��mm0����Ч�ĵ���0x0000�滻
		por mm0,mm4;			// Դ��Ŀ�����ػ��
		movq [edi+ebx*2],mm0;	// д���Ϻ������
		jmp _alph_blit_end;
		
_4pixel: // �ƶ�4������alpha��ϼ����4�����ص�
		cmp ebx,w4;				// ebx<=w4?
		movq mm0,[esi+ebx*2];	// mm0=Դ����
		jle _neednotclip;		// ebx<=w4,����ü�
		movq mm4,[edi+ebx*2];	// ��Ҫ�ü�,mm4=Ŀ������
		jmp _clip;				// ����ü�
		
_neednotclip: // ����Ҫ�ü������ݴ���
		movq [edi+ebx*2],mm0;	// ���������д��Ŀ��λͼ
		add ebx,4;				// ������4
		jmp _loop1;
		
_not_tail: // ��û�е���β
		movq [edi+ebx*2],mm0	// ���������д��Ŀ��λͼ
		add ebx,4				// ������4
		jmp _loop1;
		
_alph_blit_end:
		shl ecx, 1;
		add edi, ecx;
		pop ebx;
		pop eax;
		pop ecx;
		pop esi;

_normal_handle_out_clip:
		add ebx, ecx;		// j += rep_count
		add esi, ecx;		// Դָ���ƶ�rep_count���ֽ�
		shl ecx, 1;			// ecx = rep_count*2
		add esi, ecx;		// Դָ���ƶ�rep_count*2���ֽ�
		jmp _loop;

		
//////////////////////  �Ƿ���һ��?  //////////////////////////
_next_line:
		mov edx, pitch1;	// ��ȡpitch1
		add edi, edx;		// edi += pitch1
		inc eax;			// i++
		add esi, 2;			// skip over line header
		cmp eax, bottom;	// �����н�����
		jg _end;			// all done
		xor ebx, ebx;		// j=0
		jmp _loop;



//////////////////////  ͸������  //////////////////////////
_mask_handle: 
		inc cl;						// cl = rep_count
		mov edx, left;				// edx = left
		sub edx, ecx;				// edx = clip.left - rep_count
		cmp ebx, edx;				// j > clip.left - rep_count ?
		jg _ge_left1;
		jmp _mask_handle_out_clip;
_ge_left1:
		cmp ebx, right;				// j <= clip.right ?
		jg _mask_handle_out_clip;
		
//mask_handle_in_clip: // �ڲü�����
		push ecx;
		push eax;
		push edx;
		push ebx;
		mov edx, ebx;			// edx = j
		mov ebx, ecx;			// ebx = rep_count
		mov eax, left;			// eax = clip.left
		sub eax, edx;			// delta1 = clip.left-j
		js _handle_delta2;		// delta1 < 0 ?
		sub ecx, eax;			// rep_count -= delta1
_handle_delta2:
		add edx, ebx;			// edx = j + rep_count
		sub edx, right;			// edx = j + rep_count - clip.right
		sub edx, 1;				// delta2 = j + rep_count - clip.right - 1
		js _handle_mem_copy;	// delta2 < 0 ?
		sub ecx, edx;			// // rep_count -= delta2
_handle_mem_copy:
		pop ebx;
		pop edx;
		shl ecx, 1;				// ecx = rep_count*2
		add edi, ecx;			// edi += rep_count*2
		
		pop eax;
		pop ecx;
_mask_handle_out_clip:
		inc esi;				// Դָ���ƶ�1���ֽ�
		add ebx, ecx;			// j += rep_count
		jmp _loop;


_end:
		emms;
	}
	return WS_OK;
}
//------------------------------------------------------------------------
// �����ҽ�alphaasm.cpp�еĵ�һ������������ǰ��˵���¹�������޸ĺ�Ĵ���
// Ҫ�������tga������Ĵ���(�μ��ļ�ͷ)
// ��16λAlphaλͼsrc BLIT��destλͼ��
WSERR _stdcall alpha_blit16_modify(WBmpStruct* dest, const WAlphaBmpStruct* src)
{
	int w,h,w4,dst_pitch=dest->pitch;
	static __int64 _16=0x0010001000100010;		// 16
	static __int64 _32=0x0020002000200020;		// 32
	static __int64 _31=0x001f001f001f001f;		// 31
	static __int64 _63=0x003f003f003f003f;		// 63
	__asm
	{
		mov ebx,src					; ebx = src ָ��(AlphaBitmap)
		mov ecx,dest				; ���� dest ָ��(Bitmap)
		#include "bmpclip.inc"		; ���ô���
									; ebx=src(Դ)ָ��,ecx=dest(Ŀ��)ָ��
									; esi=Դ��ַ, edi=Ŀ���ַ
_blit:
		mov edx,[ebx]WBmpStruct.ptr	; edx ΪԴͼָ��
		pcmpeqd mm6,mm6				; mm6=0xFFFFFFFFFFFFFFFF
		sub edx,esi					; ebp=Դλͼ������ָ��(ebp)��ʵ�ʻ�ͼʱ����ʼָ��(esi)��ƫ��,ebp<=0
		//mov edx,[ecx]WBmpStruct.pitch	; Ŀ��ͼ pitch
		neg edx						; ebp>=0
		mov eax,w					; eax=w
		shr edx,1					; ebp=Դλͼ������ָ��ƫ�Ƶ�1/2,���ڶ�λͨ����ʵ��ָ��
		sub eax,4					; eax=w-4
		xor ecx,ecx					; ecx=0,�ں������ڿ�ȼ���
		mov w4,eax					; w4=w-4
		and eax,3					; eax=w/4������
		add edx,[ebx]WAlphaBmpStruct.channel	; Դλͼͨ����ʵ�ʻ�ͼָ��
		shl eax,4								; eaxΪ��βʣ��������Ŀ����Ӧ��λ��
		mov ebx,[ebx]WBmpStruct.pitch			; Դͼ pitch
		movd mm5,eax				; mm5=eax
		sar ebx,1					; ebx=Դͼ pitch/2�����ں����ͨ��ָ�����
		psllq mm6,mm5				; mm6������β���ò���4������ʱ���вü�
		cld							; ����ָ��Ϊ����ģʽ
		align 4
_loop:	
		mov eax,0xffffffff			; eax=0xffffffff
		cmp ecx,w					; ecx<w?
		jl _loop_pixel				; ecx<w
_nextline:							; ������һ��
		xor ecx,ecx					; ecx=0 (��ȼ���)
		add edx,ebx					; ͨ��ָ��ı䵽��һ��
		lea esi,[esi+ebx*2]			; Դλͼ����ָ��ı䵽��һ��
		add edi,dst_pitch  			; Ŀ��λͼ����ָ��ı䵽��һ��//edx
		dec h						; h�߶ȼ�1
		jnz _loop
		emms
		jmp _end
_skip4pixel:
		add ecx,4					; ��ȼ�������4
		jmp _loop

_loop_pixel:
		sub eax,[edx+ecx]			; eax=255-alpha ֵ(��¼4��) eax=0xb4b3b2b1 (b1...b4Ϊa1...a4����255-alpha�Ľ��)
		jz _4pixel					; ˵��4�����ص��alpha��Ϊ255��ֻ��Դλͼ������alpha���
		cmp eax,0xffffffff			; 4��alphaֵ�Ƿ񶼵���0?
		jz _skip4pixel				; �ǵģ���������4������

		// mm0,mm3 for red;
		// mm1,mm4 for green;
		// mm2,mm5 for blue;
		// mm4,mm5,mm7 for alpha

		pxor mm4,mm4				// a1:mm4=0
		movd mm5,[edx+ecx]			// a2:mm5=00 00 00 00 a3 a2 a1 a0
		punpcklbw mm5,mm4			// a3:mm5=00a3 00a2 00a1 00a0;byte->word
		movq mm0,[edi+ecx*2]		// r1:mm0=dst3 dst2 dst1 dst0
		psrlw mm5,2					// a4:mm5=a?/4,��ɫ������64��alpha����
		movq mm7,_63				// a5:mm7=63 63 63 63
		movq mm1,mm0				// g1:mm1=dst3 dst2 dst1 dst0
		psubw mm7,mm5				// a6:mm7=63-a?
		movq mm2,mm0				// b1:mm2=dst3 dst2 dst1 dst0
		pand mm1,mask_g				// g2a:mm1=dg?(gggggg00000)
		psrlw mm1,5					// g2b:mm1=dg3 dg2 dg1 dg0
		pmullw mm1,mm7				// g3:mm1=dg?*(63-a?)
		psrlw mm5,1					// a7:mm5=a?/2��(��)ɫ������32��alpha����
		paddw mm1,_32				// g4:mm1=dg?+32
		movq mm7,_31				// a8:mm7=31 31 31 31
		movq mm4,mm1				// g5:mm4=(dg?+32)
		psubw mm7,mm5				// a9:mm7=31-a?
		psrlw mm4,6					// g6:mm4=(dg?+32)/64
		psrlw mm0,11				// r2:mm0=dr3 dr2 dr1 dr0
		paddw mm1,mm4				// g7:mm1=((dg?+32)+(dg?+32)/64)
		pmullw mm0,mm7				// r3:mm0=dr?*(31-a?)
		psrlw mm1,1					// g8:mm1=green>>1(ʵ������<<5(565),Ȼ��>>6(alpha))
		pand mm2,mask_b				// b2:mm2=db3 db2 db1 db0
		paddw mm0,_16				// r4:mm0=dr?+16
		pmullw mm2,mm7				// b3:mm2=db?*(31-a?)
		movq mm3,mm0				// r5:mm3=(dr?+16)
		paddw mm2,_16				// b4:mm2=db?+16
		psrlw mm3,5					// r6:mm3=(dr?+16)/32
		movq mm5,mm2				// b5:mm5=(db?+16)
		paddw mm0, mm3				// r7:mm0=((dr?+16)+(dr?+16)/32)
		psrlw mm5,5					// b6:mm5=(db?+16)/32
		psllw mm0,6					// r8:mm0=red<<6(�н�λ��ʵ������<<11(565),Ȼ��>>5(alpha)
		paddw mm2,mm5				// b7:mm2=((db?+16)+(db?+16)/32)
		pand mm0,mask_r				// r9:mm0=0r00 0r00 0r00 0r00
		psrlw mm2,5					// b8:mm2=blue>>5(alpha)
		pand mm1,mask_g				// g10:mm1=00g0 00g0 00g0 00g0
		por mm0,mm2					// mm0=0r0b 0r0b 0r0b 0r0b
		por mm0,mm1					// mm0=0rgb 0rgb 0rgb 0rgb 

		cmp ecx,w4					; ecx<=w4?
		paddw mm0,[esi+ecx*2]		; mm0=Դ���غ�Ŀ�����ص�alpha���(Դ������λͼ����ʱ��������alpha���,����Ͳ���Ҫ�����)
		jle _not_tail				; ecx<=w4,��û�е���ĩ
_clip:								; ������β��û��4����blit��Ҫ���вü�
		movq mm4,[edi+ecx*2]
		pand mm4,mm6				; ��Ŀ����������Ч�ĵ���0x0000�滻
		psubusw mm0,mm6				; ��mm0����Ч�ĵ���0x0000�滻
		por mm0,mm4					; Դ��Ŀ�����ػ��
		movq [edi+ecx*2],mm0		; д���Ϻ������
		jmp _nextline				; ������һ��
_4pixel:							; �ƶ�4������alpha��ϼ����4�����ص�
		cmp ecx,w4					; ecx<=w4?
		movq mm0,[esi+ecx*2]		; mm0=Դ����
		jle _neednotclip			; ecx<=w4,����ü�
		;movq [edi+ecx*2],mm0		; ��Ҫ�ü�,mm4=Ŀ������
		movq mm4,[edi+ecx*2]		; ����ü�
		jmp _clip

_neednotclip:						; ����Ҫ�ü������ݴ���
		movq [edi+ecx*2],mm0		; ���������д��Ŀ��λͼ
		add ecx,4					; ��ȵ���4
		jmp _loop

_not_tail:							; ��û�е���β
		movq [edi+ecx*2],mm0		; ���������д��Ŀ��λͼ
		add ecx,4					; ��ȵ���4
		jmp _loop
_end:
		emms
	}
	return WS_OK;
}
//------------------------------------------------------------------------
#ifdef _DEBUG
// ���������Ǹ�������c++ʵ��
void alpha_blit16_bycplusplus(unsigned char *lpAlpha,unsigned int iAlpPitch,
				unsigned char *lpSrc,unsigned int iSrcX, unsigned int iSrcY,
				unsigned int iSrcPitch, unsigned char *lpDst,
				unsigned int iDstX, unsigned int iDstY,
				unsigned int iDstW, unsigned int iDstH,
				unsigned int iDstPitch)
{
	unsigned char *lpLinearDstBp=(iDstX<<1)+(iDstY*iDstPitch)+lpDst; //base pointer for linear destination
	unsigned char *lpLinearSrcBp=(iSrcX<<1)+(iSrcY*iSrcPitch)+lpSrc; //base pointer for linear source
	unsigned char *lpLinearAlpBp=iSrcX+(iSrcY*iAlpPitch)+lpAlpha; //base pointer for linear alpha
	//green
	//i=(63-a)*dg;
	//i=(i+32)+((i+32)>>6)>>6;
	// green = sg + i;
	//red
	//i=(31-a)*dr;
	//i=(i+16)+((i+16)>>5)>>5;
	// red = sr + i;

	int r,g,b,sr,sg,sb,dr,dg,db;
	for (unsigned int i=0; i<iDstH; i++)
	{
		for (unsigned int j=0; j<iDstW; j++)
		{
			WPixel* src = (WPixel*)(lpLinearSrcBp + i*iSrcPitch+j*2);
			WPixel* dst = (WPixel*)(lpLinearDstBp + i*iDstPitch+j*2);
			BYTE alpha = *(lpLinearAlpBp + i*iAlpPitch+j);
			// green
			sg = src->green;
			dg = dst->green * (63 - alpha / 4) + 32;
			dg = (dg + dg / 64) / 64;
			g = sg + dg;
			dst->green = g>63?63:g;
			
			// red
			sr = src->red;
			dr = dst->red * (31 - alpha / 8) + 16;
			dr = (dr + dr / 32) / 32;
			r = sr + dr;
			dst->red = r>31?31:r;

			// blue
			sb = src->blue;
			db = dst->blue * (31 - alpha / 8) + 16;
			db = (db + db / 32) / 32;
			b = sb + db;
			dst->blue = b>31?31:b;

		}
	}
}
//------------------------------------------------------------------------
// Intel�ٷ��ṩ
// �����������֮ǰ��Ҫ��tgaͼƬ���л�ԭ����
// src = src * 255 / alpha;
// if (src > 255) src = 255;
void ablend_565(unsigned char *lpAlpha,unsigned int iAlpPitch,
	unsigned char *lpSrc,unsigned int iSrcX, unsigned int iSrcY,
	unsigned int iSrcPitch, unsigned char *lpDst,
	unsigned int iDstX, unsigned int iDstY,
	unsigned int iDstW, unsigned int iDstH,
	unsigned int iDstPitch)
{
	//Mask for isolating the red,green, and blue components
	static __int64 MASKB=0x001F001F001F001F;
	static __int64 MASKG=0x07E007E007E007E0;
	static __int64 MASKSHIFTG=0x03F003F003F003F0;
	static __int64 MASKR=0xF800F800F800F800;

	//constants used by the integer alpha blending equation
	static __int64 SIXTEEN=0x0010001000100010;		// 16
	static __int64 FIVETWELVE=0x0200020002000200;	// 512
	static __int64 SIXONES=0x003F003F003F003F;		// 111111b
	unsigned char *lpLinearDstBp=(iDstX<<1)+(iDstY*iDstPitch)+lpDst; //base pointer for linear destination
	unsigned char *lpLinearSrcBp=(iSrcX<<1)+(iSrcY*iSrcPitch)+lpSrc; //base pointer for linear source
	unsigned char *lpLinearAlpBp=iSrcX+(iSrcY*iAlpPitch)+lpAlpha; //base pointer for linear alpha

	_asm{

		mov esi,lpLinearSrcBp;	//src
		mov edi,lpLinearDstBp;	//dst
		mov eax,lpLinearAlpBp;	//alpha
		mov ecx,iDstH;			//ecx=number of lines to copy
		mov ebx,iDstW;			//ebx=span width to copy
		test esi,6;				//check if source address is qword aligned
								//since addr coming in is always word aligned(16bit)
		jnz done;				//if not qword aligned we don't do anything

primeloop:
		movd mm1,[eax];			//mm1=00 00 00 00 a3 a2 a1 a0
		pxor mm2,mm2;			//mm2=0;
		movq mm4,[esi];			//g1: mm4=src3 src2 src1 src0
		punpcklbw mm1,mm2;		//mm1=00a3 00a2 00a1 00a0
loopqword:
		mov edx,[eax];
		test ebx,0xFFFFFFFC;	//check if only 3 pixels left
		jz checkback;			//3 or less pixels left
		//early out tests
		cmp edx,0xffffffff;		//test for alpha value of 1
		je copyback;			//if 1's copy the source pixels to the destination
		test edx,0xffffffff;	//test for alpha value of 0
		jz leavefront;			//if so go to the next 4 pixels
								//the alpha blend starts
		//green
		//i=a*sg+(63-a)*dg;
		//i=(i+32)+((i+32)>>6)>>6;
		//red
		//i=a*sr+(31-a)*dr;
		//i=(i+16)+((i+16)>>5)>>5;
		movq mm5,[edi];			//g2: mm5=dst3 dst2 dst1 dst0
		psrlw mm1,2;			//mm1=a?>>2 nuke out lower 2 bits
		movq mm7,MASKSHIFTG;	//g3: mm7=1 bit shifted green mask
		psrlw mm4,1;			//g3a: move src green down by 1 so that we won't overflow
		movq mm0,mm1;			//mm0=00a3 00a2 00a1 00a0
		psrlw mm5,1;			//g3b: move dst green down by 1 so that we won't overflow
		psrlw mm1,1;			//mm1=a?>>1 nuke out lower 1 bits
		pand mm4,mm7;			//g5: mm4=sg3 sg2 sg1 sg0
		movq mm2,SIXONES;		//g4: mm2=63
		pand mm5,mm7;			//g7: mm5=dg3 dg2 dg1 dg0
		movq mm3,[esi];			//b1: mm3=src3 src2 src1 src0
		psubsb mm2,mm0;			//g6: mm2=63-a3 63-a2 63-a1 63-a0
		movq mm7,MASKB;			//b2: mm7=BLUE MASK
		pmullw mm4,mm0;			//g8: mm4=sg?*a?
		movq mm0,[edi];			//b3: mm0=dst3 dst2 dst1 dst0
		pmullw mm5,mm2;			//g9: mm5=dg?*(1-a?)
		movq mm2,mm7;			//b4: mm2=fiveones
		pand mm3,mm7;			//b4: mm3=sb3 sb2 sb1 sb0
		pmullw mm3,mm1;			//b6: mm3=sb?*a?
		pand mm0,mm7;			//b5: mm0=db3 db2 db1 db0
		movq mm7,[esi];			//r1: mm7=src3 src2 src1 src0
		paddw mm4,mm5;			//g10: mm4=sg?*a?+dg?*(1-a?)
		pand mm7,MASKR;			//r2: mm7=sr3 sr2 sr1 sr0��
		psubsb mm2,mm1;			//b5a: mm2=31-a3 31-a2 31-a1 31-a0
		paddw mm4,FIVETWELVE;	//g11: mm4=(mm4+512) green
		pmullw mm0,mm2;			//b7: mm0=db?*(1-a?)
		movq mm5,mm4;			//g12: mm5=mm4 green
		psrlw mm7,11;			//r4: shift src red down to position 0
		psrlw mm4,6;			//g13: mm4=mm4>>6
		paddw mm4,mm5;			//g14: mm4=mm4+mm5 green
		paddw mm0,mm3;			//b8: mm0=sb?*a?+db?*(1-a?)
		movq mm5,[edi];			//r3: mm5=dst3 dst2 dst1 dst0
		paddw mm0,SIXTEEN;		//b9: mm0=(mm0+16) blue
		pand mm5,MASKR;			//r5: mm5=dr3 dr2 dr1 dr0
		psrlw mm4,5;			//g15: mm4=0?g0 0?g0 0?g0 0?g0 green
		movq mm3,mm0;			//b10: mm3=mm0 blue
		psrlw mm0,5;			//b11: mm0=mm0>>5 blue
		psrlw mm5,11;			//r6: shift dst red down to position 0
		paddw mm0,mm3;			//b12: mm0=mm3+mm0 blue
		psrlw mm0,5;			//b13: mm0=000b 000b 000b 000b blue
		pmullw mm7,mm1;			//mm7=sr?*a?
		pand mm4,MASKG;			//g16: mm4=00g0 00g0 00g0 00g0 green
		pmullw mm5,mm2;			//r7: mm5=dr?*(31-a?)
		por mm0,mm4;			//mm0=00gb 00gb 00gb 00gb
		add eax,4;				//move to next 4 alphas
		add esi,8;				//move to next 4 pixels in src
		add edi,8;				//move to next 4 pixels in dst
		movd mm1,[eax];			//mm1=00 00 00 00 a3 a2 a1 a0
		paddw mm5,mm7;			//r8: mm5=sr?*a?+dr?*(31-a?)
		paddw mm5,SIXTEEN;		//r9: mm5=(mm5+16) red
		pxor mm2,mm2;			//mm2=0;
		movq mm7,mm5;			//r10: mm7=mm5 red
		psrlw mm5,5;			//r11: mm5=mm5>>5 red
		movq mm4,[esi];			//g1: mm4=src3 src2 src1 src0
		paddw mm5,mm7;			//r12: mm5=mm7+mm5 red
		punpcklbw mm1,mm2;		//mm1=00a3 00a2 00a1 00a0
		psrlw mm5,5;			//r13: mm5=mm5>>5 red
		psllw mm5,11;			//r14: mm5=mm5<<10 red
		por mm0,mm5;			//mm0=0rgb 0rgb 0rgb 0rgb
		sub ebx,4;				//polished off 4 pixels
		movq [edi-8],mm0;		//dst=0rgb 0rgb 0rgb 0rgb
		jmp loopqword;			//go back to start

copyback:
		movq [edi],mm4;			//copy source to destination
leavefront:
		add edi,8;				//advance destination by 4 pixels
		add eax,4;				//advance alpha by 4
		add esi,8;				//advance source by 4 pixels
		sub ebx,4;				//decrease pixel count by 4
		jmp primeloop;

checkback:
		test ebx,0xFF;			//check if 0 pixels left
		jz nextline;			//done with this span
//backalign:					//work out back end pixels
		movq mm5,[edi];			//g2: mm5=dst3 dst2 dst1 dst0
		psrlw mm1,2;			//mm1=a?>>2 nuke out lower 2 bits
		movq mm7,MASKSHIFTG;	//g3: mm7=shift 1 bit green mask
		psrlw mm4,1;			//g3a: move src green down by 1 so that we won't overflow
		movq mm0,mm1;			//mm0=00a3 00a2 00a1 00a0
		psrlw mm5,1;			//g3b: move dst green down by 1 so that we won't overflow
		psrlw mm1,1;			//mm1=a?>>1 nuke out lower 1 bits
		pand mm4,mm7;			//g5: mm4=sg3 sg2 sg1 sg0
		movq mm2,SIXONES;		//g4: mm2=63
		pand mm5,mm7;			//g7: mm5=dg3 dg2 dg1 dg0
		movq mm3,[esi];			//b1: mm3=src3 src2 src1 src0
		psubsb mm2,mm0;			//g6: mm2=63-a3 63-a2 63-a1 63-a0
		movq mm7,MASKB;			//b2: mm7=BLUE MASK
		pmullw mm4,mm0;			//g8: mm4=sg?*a?
		movq mm0,[edi];			//b3: mm0=dst3 dst2 dst1 dst0
		pmullw mm5,mm2;			//g9: mm5=dg?*(1-a?)
		movq mm2,mm7;			//b4: mm2=fiveones
		pand mm3,mm7;			//b4: mm3=sr3 sr2 sr1 sr0
		pmullw mm3,mm1;			//b6: mm3=sb?*a?
		pand mm0,mm7;			//b5: mm0=db3 db2 db1 db0
		movq mm7,[esi];			//r1: mm7=src3 src2 src1 src0
		paddw mm4,mm5;			//g10: mm4=sg?*a?+dg?*(1-a?)
		pand mm7,MASKR;			//r2: mm7=sr3 sr2 sr1 sr0
		psubsb mm2,mm1;			//b5a: mm2=31-a3 31-a2 31-a1 31-a0
		paddw mm4,FIVETWELVE;	//g11: mm4=(i+512) green
		pmullw mm0,mm2;			//b7: mm0=db?*(1-a?)
		movq mm5,mm4;			//g12: mm5=(i+512) green
		psrlw mm7,11;			//r4: shift src red down to position 0
		psrlw mm4,6;			//g13: mm4=(i+512)>>6
		paddw mm4,mm5;			//g14: mm4=(i+512)+((i+512)>>6) green
		paddw mm0,mm3;			//b8: mm0=sb?*a?+db?*(1-a?)
		movq mm5,[edi];			//r3: mm5=dst3 dst2 dst1 dst0
		paddw mm0,SIXTEEN;		//b9: mm0=(i+16) blue
		pand mm5,MASKR;			//r5: mm5=dr3 dr2 dr1 dr0
		psrlw mm4,5;			//g15: mm4=0?g0 0?g0 0?g0 0?g0 green
		movq mm3,mm0;			//b10: mm3=(i+16) blue
		psrlw mm0,5;			//b11: mm0=(i+16)>>5 blue
		psrlw mm5,11;			//r6: shift dst red down to position 0
		paddw mm0,mm3;			//b12: mm0=(i+16)+(i+16)>>5 blue
		psrlw mm0,5;			//b13: mm0=000r 000r 000r 000r blue
		pmullw mm7,mm1;			//mm7=sr?*a?
		pand mm4,MASKG;			//g16: mm4=00g0 00g0 00g0 00g0 green
		pmullw mm5,mm2;			//r7: mm5=dr?*(31-a?)
		por mm0,mm4;			//mm0=00gb 00gb 00gb 00gb
		add eax,4;				//move to next 4 alphas
		//stall
		paddw mm5,mm7;			//r8: mm5=sr?*a?+dr?*(31-a?)
		paddw mm5,SIXTEEN;		//r9: mm5=(i+16) red
		movq mm7,mm5;			//r10: mm7=(i+16) red
		psrlw mm5,5;			//r11: mm5=(i+16)>>5 red
		paddw mm5,mm7;			//r12: mm5=(i+16)+((i+16)>>5) red
		psrlw mm5,5;			//r13: mm5=(i+16)+((i+16)>>5)>>5 red
		psllw mm5,11;			//r14: mm5=mm5<<10 red
		por mm0,mm5;			//mm0=0rgb 0rgb 0rgb 0rgb
		test ebx,2;				//check if there are 2 pixels
		jz oneendpixel;			//goto one pixel if that's it
		movd [edi],mm0;			//dst=0000 0000 0rgb 0rgb
		psrlq mm0,32;			//mm0>>32
		add edi,4;				//edi=edi+4
		sub ebx,2;				//saved 2 pixels
		jz nextline;			//all done goto next line
oneendpixel:					//work on last pixel
		movd edx,mm0;			//edx=0rgb
		mov [edi],dx;			//dst=0rgb
	
nextline:						//goto next line
		dec ecx;				//nuke one line
		jz done;				//all done
		mov eax,lpLinearAlpBp;	//alpha
		mov esi,lpLinearSrcBp;	//src
		mov edi,lpLinearDstBp;	//dst
		add eax,iAlpPitch;		//inc alpha ptr by 1 line
		add esi,iSrcPitch;		//inc src ptr by 1 line
		add edi,iDstPitch;		//inc dst ptr by 1 line
		mov lpLinearAlpBp,eax;	//save new alpha base ptr
		mov ebx,iDstW;			//ebx=span width to copy
		mov lpLinearSrcBp,esi;	//save new src base ptr
		mov lpLinearDstBp,edi;	//save new dst base ptr
		jmp primeloop;			//start the next span
done:
		emms
	}
}
//------------------------------------------------------------------------
// �����������֮ǰ��Ҫ��tgaͼƬ���л�ԭ����
// src = src * 255 / alpha;
// if (src > 255) src = 255;
void __AlphaBlend565(const BYTE *pAlphas, DWORD iAlpPitch, const WORD *pSrc, DWORD iSrcPitch,
				WORD *pDest, DWORD uDestW, DWORD uDestH, DWORD uDestPitch)
{
	static __int64 MASKB=0x001F001F001F001F;
	static __int64 MASKG=0x07E007E007E007E0;
	static __int64 MASKSHIFTG=0x03F003F003F003F0;
	static __int64 MASKR=0xF800F800F800F800;

	static __int64 SIXTEEN=0x0010001000100010;
	static __int64 FIVETWELVE=0x0200020002000200;
	static __int64 SIXONES=0x003F003F003F003F;

	//ASSERT( uDestW && uDestH );

	_asm
	{
		mov	esi, pSrc;			//src
		mov	edi, pDest;			//dst
		mov	eax, pAlphas;		//alpha
		mov	ecx, uDestH;			//ecx=number of lines to copy
		mov	ebx, uDestW;			//ebx=span width to copy

line_start:
		test	ebx,0xFFFFFFFC;	//check if only 3 pixels left
		jz		checkback;	//3 or less pixels left

primeloop:
		movd	mm1,[eax];	//mm1=00 00 00 00 a3 a2 a1 a0
		pxor	mm2,mm2;	//mm2=0;

		movq	mm4,[esi];	//g1: mm4=src3 src2 src1 src0
		punpcklbw mm1,mm2;	//mm1=00a3 00a2 00a1 00a0

loopqword:
		mov		edx,[eax];

		//early out tests 
		cmp		edx,0xffffffff;	//test for alpha value of 1
		je		copyback;	//if 1's copy the source pixels to the destination

		test	edx,0xffffffff;	//test for alpha value of 0
		jz		leavefront;	//if so go to the next 4 pixels
		
		movq	mm5,[edi];		//g2: mm5=dst3 dst2 dst1 dst0
		psrlw	mm1,2;			//mm1=a?>>2 nuke out lower 2 bits
		
		movq	mm7,MASKSHIFTG;	//g3: mm7=1 bit shifted green mask
		psrlw	mm4,1;			//g3a: move src green down by 1 so that we won't overflow

		movq	mm0,mm1;		//mm0=00a3 00a2 00a1 00a0
		psrlw	mm5,1;			//g3b: move dst green down by 1 so that we won't overflow

		psrlw	mm1,1;			//mm1=a?>>1 nuke out lower 1 bits
		pand	mm4,mm7;		//g5: mm4=sg3 sg2 sg1 sg0

		movq	mm2,SIXONES;	//g4: mm2=63
		pand	mm5,mm7;		//g7: mm5=dg3 dg2 dg1 dg0

		movq	mm3,[esi];		//b1: mm3=src3 src2 src1 src0
		psubsb	mm2,mm0;		//g6: mm2=63-a3 63-a2 63-a1 63-a0 

		movq	mm7,MASKB;		//b2: mm7=BLUE MASK
		pmullw	mm4,mm0;		//g8: mm4=sg?*a?

		movq	mm0,[edi];		//b3: mm0=dst3 dst2 dst1 dst0
		pmullw	mm5,mm2;		//g9: mm5=dg?*(1-a?)

		movq	mm2,mm7;		//b4: mm2=fiveones
		pand	mm3,mm7;		//b4: mm3=sb3 sb2 sb1 sb0

		pmullw	mm3,mm1;		//b6: mm3=sb?*a?
		pand	mm0,mm7;		//b5: mm0=db3 db2 db1 db0
		
		movq	mm7,[esi];		//r1: mm7=src3 src2 src1 src0
		paddw	mm4,mm5;		//g10: mm4=sg?*a?+dg?*(1-a?)

		pand	mm7,MASKR;		//r2: mm7=sr3 sr2 sr1 sr0
		psubsb	mm2,mm1;		//b5a: mm2=31-a3 31-a2 31-a1 31-a0 

		paddw	mm4,FIVETWELVE;	//g11: mm4=(mm4+512) green
		pmullw	mm0,mm2;		//b7: mm0=db?*(1-a?)

		movq	mm5,mm4;		//g12: mm5=mm4 green
		psrlw	mm7,11;			//r4: shift src red down to position 0
		
		psrlw	mm4,6;			//g13: mm4=mm4>>6

		paddw	mm4,mm5;		//g14: mm4=mm4+mm5 green
		
		paddw	mm0,mm3;		//b8: mm0=sb?*a?+db?*(1-a?)

		movq	mm5,[edi];		//r3: mm5=dst3 dst2 dst1 dst0		

		paddw	mm0,SIXTEEN;	//b9: mm0=(mm0+16) blue

		pand	mm5,MASKR;		//r5: mm5=dr3 dr2 dr1 dr0
		psrlw	mm4,5;			//g15: mm4=0?g0 0?g0 0?g0 0?g0 green

		movq	mm3,mm0;		//b10: mm3=mm0 blue
		psrlw	mm0,5;			//b11: mm0=mm0>>5 blue

		psrlw	mm5,11;			//r6: shift dst red down to position 0
		paddw	mm0,mm3;		//b12: mm0=mm3+mm0 blue

		psrlw	mm0,5;			//b13: mm0=000b 000b 000b 000b blue
		pmullw	mm7,mm1;		//mm7=sr?*a?

		pand	mm4,MASKG;		//g16: mm4=00g0 00g0 00g0 00g0 green
		pmullw	mm5,mm2;		//r7: mm5=dr?*(31-a?)

		por		mm0,mm4;	//mm0=00gb 00gb 00gb 00gb
		paddw	mm5,mm7;	//r8: mm5=sr?*a?+dr?*(31-a?)

		add	eax,4;			//move to next 4 alphas
		paddw	mm5,SIXTEEN	//r9: mm5=(mm5+16) red

		add	esi,8;			//move to next 4 pixels in src
		movq	mm7,mm5;	//r10: mm7=mm5 red

		add	edi,8;			//move to next 4 pixels in dst
		psrlw	mm5,5;		//r11: mm5=mm5>>5 red

		pxor	mm2,mm2;	//mm2=0;
		paddw	mm5,mm7;	//r12: mm5=mm7+mm5 red

		sub		ebx,4;		//polished off 4 pixels
		psrlw	mm5,5;		//r13: mm5=mm5>>5 red

		psllw	mm5,11;		//r14: mm5=mm5<<10 red
				
		test	ebx,0xFFFFFFFC;	//check if only 3 or less pixels left
		por		mm5,mm0;	//mm0=0rgb 0rgb 0rgb 0rgb

		movq	[edi-8],mm5	//dst=0rgb 0rgb 0rgb 0rgb
		jz		checkback

		movd	mm1,[eax];	//mm1=00 00 00 00 a3 a2 a1 a0
		movq	mm4,[esi];	//g1: mm4=src3 src2 src1 src0

		punpcklbw mm1,mm2;	//mm1=00a3 00a2 00a1 00a0
		jmp		loopqword;

copyback:
		movq [edi],mm4;		//copy source to destination
leavefront:
		sub  	ebx,4;		//decrease pixel count by 4
		add  	eax,4;		//advance alpha by 4
		add  	esi,8;		//advance source by 4 pixels
		add		edi,8;		//advance destination by 4 pixels

		test	ebx,0xFFFFFFFC;	//check if only 3 or less pixels left
		jnz		primeloop;

checkback:
		test	ebx,0xFF;		//check if 0 pixels left
		jz	nextline;			//done with this span

//backalign:	//work out back end pixels		
		movq	mm5,[edi];		//g2: mm5=dst3 dst2 dst1 dst0
		psrlw	mm1,2;			//mm1=a?>>2 nuke out lower 2 bits
		
		movq	mm7,MASKSHIFTG;	//g3: mm7=shift 1 bit green mask
		psrlw	mm4,1;			//g3a: move src green down by 1 so that we won't overflow

		movq	mm0,mm1;		//mm0=00a3 00a2 00a1 00a0
		psrlw	mm5,1;			//g3b: move dst green down by 1 so that we won't overflow

		psrlw	mm1,1;			//mm1=a?>>1 nuke out lower 1 bits
		pand	mm4,mm7;		//g5: mm4=sg3 sg2 sg1 sg0

		movq	mm2,SIXONES;	//g4: mm2=63
		pand	mm5,mm7;		//g7: mm5=dg3 dg2 dg1 dg0

		movq	mm3,[esi];		//b1: mm3=src3 src2 src1 src0
		psubsb	mm2,mm0;		//g6: mm2=63-a3 63-a2 63-a1 63-a0 

		movq	mm7,MASKB;		//b2: mm7=BLUE MASK
		pmullw	mm4,mm0;		//g8: mm4=sg?*a?

		movq	mm0,[edi];		//b3: mm0=dst3 dst2 dst1 dst0
		pmullw	mm5,mm2;		//g9: mm5=dg?*(1-a?)

		movq	mm2,mm7;		//b4: mm2=fiveones
		pand	mm3,mm7;		//b4: mm3=sr3 sr2 sr1 sr0

		pmullw	mm3,mm1;		//b6: mm3=sb?*a?
		pand	mm0,mm7;		//b5: mm0=db3 db2 db1 db0
		
		movq	mm7,[esi];		//r1: mm7=src3 src2 src1 src0
		paddw	mm4,mm5;		//g10: mm4=sg?*a?+dg?*(1-a?)

		pand	mm7,MASKR;		//r2: mm7=sr3 sr2 sr1 sr0
		psubsb	mm2,mm1;		//b5a: mm2=31-a3 31-a2 31-a1 31-a0 

		paddw	mm4,FIVETWELVE;	//g11: mm4=(i+512) green
		pmullw	mm0,mm2;		//b7: mm0=db?*(1-a?)

		movq	mm5,mm4;		//g12: mm5=(i+512) green
		psrlw	mm7,11;			//r4: shift src red down to position 0
		
		psrlw	mm4,6;			//g13: mm4=(i+512)>>6

		paddw	mm4,mm5;		//g14: mm4=(i+512)+((i+512)>>6) green
		
		paddw	mm0,mm3;		//b8: mm0=sb?*a?+db?*(1-a?)

		movq	mm5,[edi];		//r3: mm5=dst3 dst2 dst1 dst0		

		paddw	mm0,SIXTEEN;	//b9: mm0=(i+16) blue

		pand	mm5,MASKR;		//r5: mm5=dr3 dr2 dr1 dr0
		psrlw	mm4,5;			//g15: mm4=0?g0 0?g0 0?g0 0?g0 green

		movq	mm3,mm0;		//b10: mm3=(i+16) blue
		psrlw	mm0,5;			//b11: mm0=(i+16)>>5 blue

		psrlw	mm5,11;			//r6: shift dst red down to position 0
		paddw	mm0,mm3;		//b12: mm0=(i+16)+(i+16)>>5 blue

		psrlw	mm0,5;			//b13: mm0=000r 000r 000r 000r blue
		pmullw	mm7,mm1;		//mm7=sr?*a?

		pand	mm4,MASKG;		//g16: mm4=00g0 00g0 00g0 00g0 green
		pmullw	mm5,mm2;		//r7: mm5=dr?*(31-a?)

		por	mm0,mm4;			//mm0=00gb 00gb 00gb 00gb
		add	eax,4;				//move to next 4 alphas

		//stall

		paddw	mm5,mm7;		//r8: mm5=sr?*a?+dr?*(31-a?)

		paddw	mm5,SIXTEEN;	//r9: mm5=(i+16) red

		movq	mm7,mm5;		//r10: mm7=(i+16) red
		psrlw	mm5,5;			//r11: mm5=(i+16)>>5 red

		paddw	mm5,mm7;		//r12: mm5=(i+16)+((i+16)>>5) red

		psrlw	mm5,5;			//r13: mm5=(i+16)+((i+16)>>5)>>5 red

		psllw	mm5,11;			//r14: mm5=mm5<<10 red
				
		por	mm0,mm5;			//mm0=0rgb 0rgb 0rgb 0rgb
		test	ebx,2;			//check if there are 2 pixels

		jz	oneendpixel;		//goto one pixel if that's it

		movd	[edi],mm0;		//dst=0000 0000 0rgb 0rgb
		psrlq	mm0,32;			//mm0>>32

		add	edi,4;				//edi=edi+4
		sub	ebx,2;				//saved 2 pixels

		jz	nextline;			//all done goto next line
oneendpixel:					//work on last pixel
		movd	edx,mm0;		//edx=0rgb

		mov	[edi],dx;			//dst=0rgb

nextline:						//goto next line
		dec	ecx;				//nuke one line
		jz	done;				//all done

		mov	eax, pAlphas;		//alpha
		mov	esi, pSrc;			//src

		mov	edi, pDest;			//dst
		add	eax,iAlpPitch;		//inc alpha ptr by 1 line

		add	esi,iSrcPitch;		//inc src ptr by 1 line
		add	edi,uDestPitch;		//inc dst ptr by 1 line

		mov	pAlphas, eax;		//save new alpha base ptr
		mov	ebx,uDestW;			//ebx=span width to copy

		mov	pSrc,esi;			//save new src base ptr
		mov	pDest,edi;			//save new dst base ptr

		jmp	line_start;			//start the next span
done:
		emms
	}
}

#endif