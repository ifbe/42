[bits 64]
extern birth
extern windowstart
extern characterstart

extern eventread
extern eventwrite
extern windowread
extern windowwrite
extern characterread
extern characterwrite

extern characterstop
extern windowstop
extern death

section .text
global main
main:
	call birth

	mov rcx,pixbuf
	mov rdx,pixfmt
	mov r8,512
	mov r9,512
	call windowstart

	mov rcx,pixbuf
	mov rdx,pixfmt
	mov r8,512
	mov r9,512
	call characterstart

mainloop:
	call characterread
	call windowwrite

eventloop:
	call eventread
	cmp eax,0
	je bye
	cmp dword [rax+8],0
	je bye
	cmp dword [rax+8],0x656d6974
	je eventloop

	mov rcx,rax
	call characterwrite

	jmp mainloop

bye:
	call death
	ret

section .data
pixbuf:
	times (2048*1024*4)+0x100000 db 0
pixfmt:
	db 'b','g','r','a','8','8','8','8'
