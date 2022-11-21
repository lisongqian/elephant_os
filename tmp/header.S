header:
	program_length	dd	program_end-program_start
	start_addr	dd	program_start
;;;;;		以上文件头，以下文件体		;;;;;
body:
program_start:
	mov ax, 0x1234
	jmp $
program_end:
