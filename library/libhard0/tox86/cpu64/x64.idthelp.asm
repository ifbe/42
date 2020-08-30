[bits 64]
global getisr03
global getisr80

section .text
getisr03:
	lea rax,[rel fuckshit]
	ret
getisr80:
	lea rax,[rel fuckshit]
	ret
fuckshit:
	iretq