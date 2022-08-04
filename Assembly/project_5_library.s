.text
	.global read_character
	.global output_character
	.global output_string

U0FR: 	.equ 0x18	; UART0 Flag Register

	; Add read_character, output_character, output_string from your prevous projects
   	; Be sure to modify read_character so that it just reads from the UART0 DAta Register
    	; and does not poll since we are now using interrupts

output_string:
	PUSH {lr, r10}   ; Store register lr on stack

	MOV r10, r4  ;move the initial pointer of the string in r0 to r10
INITIAL_LOOP1:
	LDRB r0, [r10]   ;load the byte from r10 to r0
	BL output_character ;call output character
	CMP r0, #0  ;see if it is null
	BNE OUTPUT_LOOP1	;if not null, go to output_loop
	B END_LOOP1 ;else go to end loop

OUTPUT_LOOP1:
	ADD r10, r10, #1  ;increment pointer address by 1
	B INITIAL_LOOP1 ;branch back to initial loop

END_LOOP1:
		; Your code for your output_string routine is placed here

	POP {lr, r10}
	mov pc, lr

read_character:
			PUSH {lr, r2-r3}   ; Store register lr and r4 on stack
			MOV r2, #0xC000		; Load address of Flag register
			MOVT r2, #0x4000


			LDRB r8, [r2]		; Read byte from Flag register

			POP {lr, r2-r3} ; Restore registers back for parent function
			mov pc, lr


output_character:

	PUSH {lr, r4}   ; Store register lr and r4 on stack
	MOV r2, #0xC000	; Load address of Flag register
	MOVT r2, #0x4000

OUTPUT_LOOP:
			LDRB r3, [r2, #U0FR]	; Read TxFF, wait until it is 0
			AND r4, r3, #32
			CMP r4, #0
			BNE OUTPUT_LOOP
			STRB r0, [r2]	; Store byte into Flag register

			POP {lr, r4} ; Restore registers back for parent function
			mov pc, lr



	.end
