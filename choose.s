@ lab1/func.s
@ Copyright (c) 2018-20 J. M. Spivey

        .syntax unified
        .global func

        .text
        .thumb_func
integerdiv:
        movs r2, #0 @initialise the quotient to 0. we keep the remainder in r0
        	loop1:
        cmp r0, r1 	@if r < y ...
        blo end		@  break out of loop
        adds r2, #1	@q = q + 1
        subs r0, r1	@r = r - y
        b loop1		@back to start of loop
        	end:
        movs r0, r2	@return the quotient
        bx lr
func:
@ ----------------
@ Two parameters are in registers r0 and r1
	@r0 is n, r1 = r
	movs r2, #1
	movs r3, #1
		loop:
	cmp r1, #0
	beq done
	cmp r0, #0
	beq done
	muls r2, r0, r2
	muls r3, r1, r3
	subs r0, #1
	subs r1, #1
	b loop
		done:
	movs r0, r2
	movs r1, r3
	bl integerdiv
	bx lr

