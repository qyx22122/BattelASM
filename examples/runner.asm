runner 8
;=======================

mv r2, pc
mv sp, pc
addi sp, #size:-1
ldi #size

mv r29, pc
	subi r0, 1
	ld r3, r2
	addi r2, 1
	push r3
	flag
	jnz r29, r0

mv r1, pc
subi r1, 2
st r16, r1