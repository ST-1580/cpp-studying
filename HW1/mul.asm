section         .text

                global          _start

; main
;    r11 -- address of multiplier #1 (long number)
;    r12 -- address of multiplier #2 (long number)
;    r13 -- address for intermediate calculations (long number)

<<<<<<< HEAD
_start:
                sub             rsp, 5 * 256 * 8    
                mov             rcx, 256 					
                lea             rdi, [rsp + 256 * 8] 		
                call            read_long 			
                mov             rdi, rsp		
                call            read_long 			
        		lea             r11, [rsp + 1 * 256 * 8] 		 
                lea		        r12, [rsp + 2 * 256 * 8]	
                lea		        r13, [rsp + 4 * 256 * 8]
=======
                sub             rsp, 5 * 128 * 8 		
                mov             rcx, 128 			
		mov	        r13, rcx			
		mov		r14, 128 * 2			
                lea             rdi, [rsp + 128 * 8] 		
                call            read_long 			
                mov             rdi, rsp		
                call            read_long 			
        	lea             rsi, [rsp + 128 * 8] 		
                lea		r11, [rsp + 2 * 128 * 8]	
                lea		r12, [rsp + 4 * 128 * 8]
>>>>>>> 5ff84db63887d8ff7dd398ae02d11c646ef1c3f4

		call	     	mul_long_long		

<<<<<<< HEAD
		        mov		        rdi, r12
=======
		mov		rdi, r11
>>>>>>> 5ff84db63887d8ff7dd398ae02d11c646ef1c3f4
                call            write_long 		
                mov             al, 0x0a
                call            write_char
                jmp             exit 			
		
; multiplies two long numbers
;    r11 -- address of multiplier #1 (long number)
;    r12 -- address of multiplier #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    result is written to r12

mul_long_long:
                push            r11
<<<<<<< HEAD
                push            r12
                push            rcx
		        push		    rbx
		
		
.loop:
          		mov		        rbx, [r11]		

    		    push		    r11			
    		    call		    mul_long_short     		
    		    pop		        r11
=======
                push            r13
                push            rsi
		push		rbx
		
		
.loop:
          	mov		rbx, [rsi]		

    		push		rsi				
    		call		mul_long_short     		
    		pop		rsi
>>>>>>> 5ff84db63887d8ff7dd398ae02d11c646ef1c3f4

        	call		add_long_long
	
<<<<<<< HEAD
        		lea		        r12, [r12 + 8]
                lea             r11, [r11 + 8]  
=======
        	lea		r11, [r11 + 8]
                lea             rsi, [rsi + 8]  
>>>>>>> 5ff84db63887d8ff7dd398ae02d11c646ef1c3f4

                dec             rcx				
                jnz             .loop

<<<<<<< HEAD
		        pop		        rbx
                pop             rcx
                pop             r12
=======
		pop		rbx
                pop             rsi
                pop             r13
>>>>>>> 5ff84db63887d8ff7dd398ae02d11c646ef1c3f4
                pop             r11
                ret	


; adds long number to result
;    r12 -- address of summand #1 (long number)
;    r13 -- address of summand #2 (long number)
;    rcx -- length of long number #2 in qwords
; result:
;    sum is written to r12

add_long_long:
                push            r12
                push            r13
                push            rcx

                clc
.loop:
                mov             rax, [r13]
                lea             r13, [r13 + 8]
                adc             [r12], rax
                lea             r12, [r12 + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             r13
                pop             r12
                ret


; adds 64-bit number to long number
;    rdi -- address of summand #1 (long number)
;    rax -- summand #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    sum is written to rdi
add_long_short:
                push            rdi
                push            rcx
                push            rdx

                xor             rdx,rdx
.loop:
                add             [rdi], rax
                adc             rdx, 0				
                mov             rax, rdx
                xor             rdx, rdx
                add             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rdx
                pop             rcx
                pop             rdi
                ret

; multiplies long number by a short
;    rdi -- address of multiplier #1 (long number)
;    rbx -- multiplier #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    result is written to r13
mul_long_short:		
                push            rax
                push            rdi
                push            rcx
<<<<<<< HEAD
                push            r13
=======
		push		r12
>>>>>>> 5ff84db63887d8ff7dd398ae02d11c646ef1c3f4

                xor             r11, r11
.loop:
                mov             rax, [rdi]
                mul             rbx
                add             rax, r11
                adc             rdx, 0
                mov             [r13], rax
                add             r13, 8
                add             rdi, 8
<<<<<<< HEAD
                mov             r11, rdx
                dec             rcx
                jnz             .loop

                pop             r13
=======
		add		r12, 8
                mov             rsi, rdx
                dec             rcx
                jnz             .loop

		pop		r12
>>>>>>> 5ff84db63887d8ff7dd398ae02d11c646ef1c3f4
                pop             rcx
                pop             rdi
                pop             rax
                ret

; divides long number by a short
;    rdi -- address of dividend (long number)
;    rbx -- divisor (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    quotient is written to rdi
;    rdx -- remainder
div_long_short:
                push            rdi
                push            rax
                push            rcx

                lea             rdi, [rdi + 8 * rcx - 8]
                xor             rdx, rdx

.loop:
                mov             rax, [rdi]
                div             rbx
                mov             [rdi], rax
                sub             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rax
                pop             rdi
                ret

; assigns a zero to long number
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
set_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep stosq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; checks if a long number is a zero
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
; result:
;    ZF=1 if zero
is_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep scasq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; read long number from stdin
;    rdi -- location for output (long number)
;    rcx -- length of long number in qwords
read_long:
                push            rcx
                push            rdi

                call            set_zero
.loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              .done
                cmp             rax, '0'
                jb              .invalid_char
                cmp             rax, '9'
                ja              .invalid_char

                sub             rax, '0'
                mov             rbx, 10
<<<<<<< HEAD
		        mov		        r13, rdi
=======
		mov		r12, rdi
>>>>>>> 5ff84db63887d8ff7dd398ae02d11c646ef1c3f4
                call            mul_long_short
                call            add_long_short
                jmp             .loop

.done:
                pop             rdi
                pop             rcx
                ret

.invalid_char:								
                mov             rsi, invalid_char_msg
                mov             rdx, invalid_char_msg_size
                call            print_string
                call            write_char
                mov             al, 0x0a
                call            write_char

.skip_loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              exit
                jmp             .skip_loop

; write long number to stdout
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
;    rax -- length of answer in qwords
write_long:
                push            rax
                push            rcx
			
                mov             rax, 20
                mul             rcx
                mov             rbp, rsp
                sub             rsp, rax

                mov             rsi, rbp

.loop:
                mov             rbx, 10
                call            div_long_short
                add             rdx, '0'
                dec             rsi
                mov             [rsi], dl
                call            is_zero
                jnz             .loop

                mov             rdx, rbp
                sub             rdx, rsi
                call            print_string

                mov             rsp, rbp
                pop             rcx
                pop             rax
                ret

; read one char from stdin
; result:
;    rax == -1 if error occurs
;    rax \in [0; 255] if OK
read_char:
                push            rcx
                push            rdi

                sub             rsp, 1
                xor             rax, rax
                xor             rdi, rdi
                mov             rsi, rsp
                mov             rdx, 1
                syscall

                cmp             rax, 1
                jne             .error
                xor             rax, rax
                mov             al, [rsp]
                add             rsp, 1

                pop             rdi
                pop             rcx
                ret
.error:
                mov             rax, -1
                add             rsp, 1
                pop             rdi
                pop             rcx
                ret

; write one char to stdout, errors are ignored
;    al -- char
write_char:
                sub             rsp, 1
                mov             [rsp], al

                mov             rax, 1
                mov             rdi, 1
                mov             rsi, rsp
                mov             rdx, 1
                syscall
                add             rsp, 1
                ret

exit:
                mov             rax, 60
                xor             rdi, rdi
                syscall

; print string to stdout
;    rsi -- string
;    rdx -- size
print_string:
                push            rax

                mov             rax, 1
                mov             rdi, 1
                syscall

                pop             rax
                ret


section         .rodata

invalid_char_msg:      db              "Invalid character: "
invalid_char_msg_size: equ             $ - invalid_char_msg
