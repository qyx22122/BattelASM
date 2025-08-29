static uint16_t small_boy_mem[] = {
	0b0000000000000001, //ldi r0, 1
	0b1000000001111110, //mv r1, pc
	0b1000100001000000, //add r1, r0
	0b1111100000000000, //flag
	0b1011100001000000  //jmp r1
};
static uint16_t small_boy_size = 5;
static uint16_t small_boy_offset = 69;
	
