	.data

	.global my_string
	.global clear

my_string:	.string    0xC, "*", 0
board: .string 0xC, "+--------------------+", 0xA, 0xD
	   .string	    "|                    |", 0xA, 0xD
	   .string	    "|                    |", 0xA, 0xD
	   .string	    "|                    |", 0xA, 0xD
	   .string	    "|                    |", 0xA, 0xD
	   .string	    "|         *          |", 0xA, 0xD
	   .string	    "|                    |", 0xA, 0xD
	   .string	    "|                    |", 0xA, 0xD
	   .string	    "|                    |", 0xA, 0xD
	   .string	    "|                    |", 0xA, 0xD
	   .string	    "|                    |", 0xA, 0xD
	   .string	    "+--------------------+",  0x0
clear:	.string  0xC,  0

	.text
	.global project5
	.global read_character
	.global output_character
	.global output_string
	.global timer_init
	.global Timer_Handler
	.global UART0_Handler
	.global Switches_Handler


ptr_to_prompt:		.word board
ptr_clear:			.word clear

project5:
	PUSH {lr}   ; Store lr to stack

	bl timer_init   ; When you are ready to enable the timer interrupt,
	bl uart_interrupt_init		 ;enable uart and timer init
	ldr r5, ptr_to_prompt ;load prompt to r5


	mov r7, #0	;set direction as right
	mov r6, #130

		; Your code is placed here.  This is your main routine for
		; Project #4.  This should call your other routines such as
		; read_from_push_button and illuminate_RGB_LED.

	POP {lr}	  ; Restore lr from stack
	mov pc, lr


uart_interrupt_init:

	PUSH {lr, r1-r12}
	MOV r4, #0xC000		;set the rxim bit in uartim
	MOVT r4, #0x4000
	LDR r5, [r4, #0x038]
	ORR r5, r5, #0x10
	STR r5, [r4, #0x038]

	MOV r4, #0xE000		;set the bit 5 in the EN0
	MOVT r4, #0xE000
	LDR r5, [r4, #0x100]
	ORR r5, r5, #0x20
	STR r5, [r4, #0x100]
	POP {lr, r1-r12}
	MOV pc, lr



UART0_Handler:


	; The following code is to clear the timer interrupt
	PUSH {lr}
	;Clear interrupt
	MOV r0, #0xC000
	MOVT r0, #0x4000

	LDRB r1, [r0]
	AND r1, r1, #0xEF
	STRB r1, [r0]


	BL read_character ;read character pressed into r8
	CMP r7, #2
	BEQ not_q
detect_a:
	CMP r8, #0x61		;if 'a' set direction as left
	BNE not_a
	mov r7, #1 ;Switch horizontal/vertical if keypress was enter key
	B not_q

not_a:
	CMP r8, #100		;if 'd' set direction as right
	BNE not_d
	mov r7, #0
	B not_q

not_d:
	CMP r8, #119		;if 'w' set direction as up
	BNE not_w
	mov r7, #3
	B not_q

not_w:
	CMP r8, #115		;if 's' set direction as down
	BNE not_w
	mov r7, #4
	B not_q

not_q:
	CMP r8, #113		;if 'q' set direction as quit
	BNE end
	mov r7, #2
	B end

	POP {lr}

		; Your code to handler the timer interrupt is placed here.


	bx lr   ; Return (remember, don't use mov pc,lr to return from a handler)




Switches_Handler:

	; This is a place holder for a switch interrupt handler
	; Not required for project 4.


	bx lr   ; Return (remember, don't use mov pc,lr to return from a handler)



Timer_Handler:
	; The following code is to clear the timer interrupt.
	; Keep this at the beginning of the handler to allow enough time for
	; the reset to occur.
	PUSH {lr}
	mov r0, #0
	movt r0, #0x4003
	ldrb r1, [r0, #0x024]
	orr r1, r1, #1
	strb r1, [r0, #0x024]

	MOV r1, r5
	ldr r4, ptr_clear
	BL output_string
	mov r4, r1


	CMP r7, #1			;see if direction is right or left or user wants to quit
	BEQ left
	CMP r7, #0
	BEQ right


	CMP r7, #3			;see if direction is up or down or user wants to quit
	BEQ up
	CMP r7, #4
	BEQ down
	CMP r7, #2

	BEQ quit

right:			;if direction is right
		;set counter to 0
d_char:
	;load the byte from the pointer r5 and see if it is '*'
	ldrb r3, [r5, r6]


	CMP r3, #42

	;if it is '*" jump to found_s
	BEQ found_d

	;if it is not '*' increment pointer r0 then loop back to a_char
	ADD r6, r6, #1
	B d_char

found_d:

	add r6, r6, #1		;add 1 to counter
	ldrb r3, [r5, r6]		;load the byte from the pointer r5 and see if it is '*'



	CMP r3, #124		;if it is '|" jump to quit
	BEQ quit
	CMP r3, #42	;if it is '*" jump to quit
	BEQ quit


	mov r2, #42
	strb r2, [r5, r6]		;store '*' at the right of previous '*'




	;store null at end of string

	mov r2, #42
	sub r6, r6, #1   ;store '*' at left of '*'
	strb r2, [r5, r6]

    add r6, r6, #1
	B end


up:			;if direction is up
		;set counter to 0
w_char:
	;load the byte from the pointer r5 and see if it is '*'
	ldrb r3, [r5, r6]


	CMP r3, #42

	;if it is '*" jump to found_w
	BEQ found_w

	;if it is not '*' increment pointer r0 then loop back to a_char
	ADD r6, r6, #1
	B w_char

found_w:

	sub r6, r6, #24	;add 24 to counter
	ldrb r3, [r5, r6] ;load the byte from the pointer r5 and see if it is '*'


	CMP r3, #45	;if it is '-" jump to quit
	BEQ quit
	CMP r3, #42	;if it is '*" jump to quit
	BEQ quit


	mov r2, #42
	strb r2, [r5, r6]		;store '*' at the top of previous '*'






	mov r2, #42
	add r6, r6, #24   ;store '*' at bottom of '*'
	strb r2, [r5, r6]

    sub r6, r6, #24
	B end



down:			;if direction is right
		;set counter to 0
s_char:
	;load the byte from the pointer r5 and see if it is '*'
	ldrb r3, [r5, r6]


	CMP r3, #42

	;if it is '*" jump to found_s
	BEQ found_s

	;if it is not '*' increment pointer r0 then loop back to a_char
	ADD r6, r6, #1
	B s_char

found_s:

	add r6, r6, #24	;add 1 to counter
	ldrb r3, [r5, r6]


	CMP r3, #45
	BEQ quit
	CMP r3, #42
	BEQ quit


	mov r2, #42
	strb r2, [r5, r6]		;store '*' at the right of previous '*'




	;store null at end of string

	mov r2, #42
	sub r6, r6, #24  ;store space at left of '*'
	strb r2, [r5, r6]

    add r6, r6, #24
	B end







left:			;if direction is left
	;set counter to 0
a_char:
	;load the byte from the pointer r5 and see if it is '*'
	ldrb r3, [r5, r6]


	CMP r3, #42

	;if it is '*" jump to found_a
	BEQ found_a

	;if it is not '*' increment pointer r0 then loop back to a_char
	ADD r6, r6, #1
	B a_char

found_a:


	cmp r6, #1		;if at leftmost position, pause or quit
	BEQ quit
	sub r6, r6, #1		;sub 1 from counter

	ldrb r3, [r5, r6]
	CMP r3, #124
	BEQ quit
	CMP r3, #42
	BEQ quit

	mov r2, #42  ;store '*' at left of previous '*'
	strb r2, [r5, r6]



	mov r2, #42
	strb r2, [r6]		;store space at previous '*'


	B end

quit:
	mov r7, #2		;quit as in stop '*'


end:



	mov r4, r5
	BL output_string

	; Your code to handler the timer interrupt is placed here.
	POP {lr}
	bx lr   ; Return (remember, don't use mov pc,lr to return from a handler)

timer_init:
	; Enable Clock for Timer 0
	mov r0, #0xE000
	movt r0, #0x400F
	ldrb r1, [r0, #0x604]
	orr r1, r1, #1
	strb r1, [r0, #0x604]

	; Disable Timer0
	mov r0, #0
	movt r0, #0x4003
	ldrb r1, [r0, #0x00C]
	bic r1, r1, #1
	strb r1, [r0, #0x00C]


	; Put Timer in 32-bit Mode
	mov r0, #0
	movt r0, #0x4003
	mov r1, #0
	strb r1, [r0]

	; Put Timer in Periodic MOde
	mov r0, #0
	movt r0, #0x4003
	ldrb r1, [r0, #0x004]
	bic r1, r1, #1
	orr r1, r1, #2
	strb r1, [r0, #0x004]

	; Setup Interval Period
	mov r0, #0
	movt r0, #0x4003
	mov r1, #0x1200
	movt r1, #0x7A
	str r1, [r0, #0x028]

	; Enable Timer to Interrupt Processor
	mov r0, #0
	movt r0, #0x4003
	ldrb r1, [r0, #0x018]
	orr r1, r1, #1
	str r1, [r0, #0x018]


	; Configure Processor to Allow Timer to Interrupt Processor
	mov r0, #0xE000
	movt r0, #0xE000
	ldr r1, [r0, #0x100]
	orr r1, r1, #0x80000
	str r1, [r0, #0x100]

	; Enable Timer0
	mov r0, #0
	movt r0, #0x4003
	ldrb r1, [r0, #0x00C]
	orr r1, r1, #1
	strb r1, [r0, #0x00C]

	mov pc, lr

	.end
