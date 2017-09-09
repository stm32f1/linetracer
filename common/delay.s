.section ".text"

.align 8

.global delay_us
delay_us:
	push	{r1, lr}
	mov	r1, #8
	mul	r0, r0, r1
	nop
	nop
	nop
	nop

loop:	sub	r0, r0, #1
	cmp	r0, #0
	bne	loop
	pop	{r1, pc}
