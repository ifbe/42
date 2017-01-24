[bits 64]
extern birth
extern windowstart

extern eventread
extern eventwrite
extern windowread
extern windowwrite

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

mainloop:
	call aaaaaaaa
	call windowwrite

eventloop:
	call eventread
	cmp eax,0
	je bye
	cmp dword [rax+8],0
	je bye

	call bbbbbbbb
	jmp mainloop

bye:
	call death
	ret

aaaaaaaa:
	mov rdi,pixbuf
	mov rax,[color]
	mov rcx,512*512
	rep stosd
	ret

bbbbbbbb:
	inc qword [color]
	ret




section .data
pixbuf:
	times (2048*1024*4)+0x100000 db 0
pixfmt:
	db 'b','g','r','a','8','8','8','8'
color:
	dd 0
