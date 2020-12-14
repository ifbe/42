section .text


[bits 64]
global get_entry_ap_start
global get_entry_ap_end

get_entry_ap_start:
	lea rax,[rel entry_ap_start]
	ret

get_entry_ap_end:
	lea rax,[rel entry_ap_end]
	ret


[bits 16]
entry_ap_start:
	xor eax,eax
	ret
entry_ap_end: