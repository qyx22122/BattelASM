static uint16_t small_boy_mem[] = {
	0b0000000000000001, //ldi r0, 1
	0b1000000000111111, //mv r1, pc
	0b1000010000100000, //add r1, r0
	0b1111110000000000, //flag
	0b1010010000100000  //jmp r1
};
static uint16_t small_boy_size = 5;
static uint16_t small_boy_offset = 69;
	
