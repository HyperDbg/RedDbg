.code

SvmVmmCall proc frame	
	.endprolog
	
	push rbx ; prevent the corruption of certain guest registers used during vm unload
	push rcx
	push rdx 
	vmmcall
	pop rdx
	pop rcx
	pop rbx

	ret
SvmVmmCall endp

end