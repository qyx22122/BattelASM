static uint16_t runner_mem[] = {
	0b1000000001011111, // mv r2, pc
	0b1000001111011111, // mv sp, pc
	0b1100101111001101, // addi sp, #size:-1
	0b0000000000001110, // ldi #size
	0b1000001110111111, // mv r29, pc
	0b1100110000000001, // 	subi r0, 1
	0b1011100001100010, // 	ld r3, r2
	0b1100100001000001, // 	addi r2, 1
	0b1100000001100000, // 	push r3
	0b1111110000000000, // 	flag
	0b1010111110100000, // 	jnz r29, r0
	0b1000000000111111, // mv r1, pc
	0b1100110000100010, // subi r1, 2
	0b1011111000000001, // st r16, r1
};
static uint16_t runner_size = 14;
static uint16_t runner_offset = 8;
