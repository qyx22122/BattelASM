#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

//#define DEBUG

#define PROG_COUNT 2		//number of programs
#define MATCH_TIME 15		//in seconds
#define MAX_PROG_SIZE 1024	//max number of instructions
#define MEM_SIZE (1<<16)

enum {
	RET_OK = 0,
	RET_FAILED,
	RET_TIE
};

enum {
	R0 = 0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	R8,
	R9,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15,
	R16,
	R17,
	R18,
	R19,
	R20,
	R21,
	R22,
	R23,
	R24,
	R25,
	R26,
	R27,
	R28,
	R29,
	SP,
	PC,
	REG_COUNT
};

enum {
	OP_LDI = 0x0,
	OP_MV = 0x10,
	OP_ADD,
	OP_NOT,
	OP_AND,
	OP_OR,
	OP_SHL,
	OP_SHR,
	OP_JMP,
	OP_JZ,
	OP_JN,
	OP_JP,
	OP_LD,
	OP_ST,
	OP_PUSH,
	OP_POP,
	OP_FLAG = 0x1f
};

typedef struct {
	char name[16];
	uint16_t score;
	uint16_t reg[REG_COUNT];
	uint16_t life;
	uint16_t org;
	uint16_t offset;
	uint16_t size;
	uint16_t* program_mem;
} program_t;

uint16_t get_rand_org();
void init_program(program_t* program);
int init_programs(program_t programs[], size_t len);
void process_instruction(program_t* program);
void print_reg(program_t program);
int run_match(program_t programs[], size_t len, int start_point, int* winner);
void init_mem();
int cmpProg(const void* prog1, const void* prog2);
void display_score(program_t programs[]);

uint16_t memory[MEM_SIZE];

int main() {

	uint16_t testprogmem1[16] = {0b0000000000000001, 0b1000000001111110, 0b1000100010000000, 0b1111100000000000, 0b1011100001000000};
	//uint16_t testprogmem2[16] = {0};
	
	program_t progs[PROG_COUNT] = {
		{.name = "Program 1", .program_mem = testprogmem1, .offset = 0, .size = 16},
		{.name = "Program 2", .program_mem = testprogmem1, .offset = 0, .size = 16},
	};
	
	for (int i = 0; i < PROG_COUNT; i++) {
		printf("Game %d started...\n", i);

		int winner = -1;
		init_programs(progs, PROG_COUNT);
		int ret = run_match(progs, PROG_COUNT, i, &winner);

		if (ret == RET_OK) {
			printf("%s won!\n\n", progs[winner].name);
		} else if (ret == RET_TIE) {
			printf("Tie!\n\n");
		}
	}

	display_score(progs);

}

uint16_t get_rand_org() {
	static bool prog_cells[MEM_SIZE/MAX_PROG_SIZE] = {0};
	uint16_t org;

	do {
		org = rand() % (MEM_SIZE/MAX_PROG_SIZE);
	} while(prog_cells[org]);

	prog_cells[org] = 1;
	return org * MAX_PROG_SIZE;
}

void init_program(program_t* program) {
	program->org = get_rand_org() + (((program->offset + program->size) <= MAX_PROG_SIZE) ? program->offset : 0);
#ifdef DEBUG
	printf("%s org : %d\n",program->name, program->org);
#endif
	for(uint16_t i = 0; i < program->size; i++)
		memory[program->org + i] = program->program_mem[i];
	for(uint16_t i = 0; i < REG_COUNT; i++)
		program->reg[i] = 0;

	program->reg[PC] = program->org;
	program->life = 100;
}

int init_programs(program_t programs[], size_t len) {
	if (len > 64)
		return RET_FAILED;
	
	init_mem();

	for(int i = 0; i < len; i++)
		init_program(&programs[i]);

	return RET_OK;
}

void process_instruction(program_t* program) {
	uint16_t instruction = memory[program->reg[PC]];
#ifdef DEBUG
	printf("\n\nProgram : %s\n", program->name);
	printf("life : %d\n", program->life);
	printf("instruction : %x\n", instruction);
	print_reg(*program);
#endif


	uint16_t sr1 = (instruction>>6) & 0x1f;
	uint16_t sr2 = (instruction>>1) & 0x1f;
	//load instruction
	if((instruction>>15) == OP_LDI) {
		program->reg[R0] = instruction;
		program->reg[PC]++;
		program->life--;
		return;
	}
	switch(instruction>>11) {
		case OP_MV:
			program->reg[sr1] = program->reg[sr2];
			break;
		case OP_ADD:
			program->reg[sr1] += program->reg[sr2];
			break;
		case OP_NOT:
			program->reg[sr1] = ~program->reg[sr1];
			break;
		case OP_AND:
			program->reg[sr1] &= program->reg[sr2];
			break;
		case OP_OR:
			program->reg[sr1] |= program->reg[sr2];
			break;
		case OP_SHL:
			program->reg[sr1] <<= program->reg[sr2];
			break;
		case OP_SHR:
			program->reg[sr1] >>= program->reg[sr2];
			break;
		case OP_JMP:
			program->reg[PC] = program->reg[sr1];
			break;
		case OP_JZ:
			if(!program->reg[sr2])
				program->reg[PC] = program->reg[sr1];
			break;
		case OP_JN:
			if(program->reg[sr2] < 0)
				program->reg[PC] = program->reg[sr1];
			break;
		case OP_JP:
			if(program->reg[sr2] > 0)
				program->reg[PC] = program->reg[sr1];
			break;
		case OP_LD:
			program->reg[sr1] = memory[program->reg[sr2]];
			break;
		case OP_ST:
			memory[program->reg[sr2]] = program->reg[sr1];
			break;
		case OP_PUSH:
			memory[program->reg[SP]] = program->reg[sr1];
			program->reg[SP]++;
			break;
		case OP_POP:
			program->reg[sr1] = memory[program->reg[SP]];
			program->reg[SP]--;
			break;
		case OP_FLAG:
			program->life = 101;
			break;
	}
	program->reg[PC]++;
	program->life--;
	return;
}

void print_reg(program_t program) {
	for(uint16_t i = 0; i < REG_COUNT; i++){
		printf("REG%-2d : 0x%-4X | ", i, program.reg[i]);
		if(!((i+1)%4))
			printf("\n");
	}
}

int run_match(program_t programs[], size_t len, int start_point, int* winner) {
	bool prog_dead[len];
	for(size_t i = 0; i < len; i++)
		prog_dead[i] = 0;

	int num_of_alive = len;
	time_t start_time = time(0);


	int i = start_point;
	while((time(0) - start_time) < MATCH_TIME) {
		if (!prog_dead[i]){
			process_instruction(&programs[i]);

			if (num_of_alive == 1) {
				*winner = i;
				(&programs[i])->score += 8;
				return RET_OK;
			}

			if (num_of_alive == 0)
				return RET_TIE;

			if (programs[i].life <= 0) {
				prog_dead[i] = 1;
				num_of_alive--;
			}
		}

		i++;
		i %= len;
	}
	for (int i = 0; i < len; i++)
		(&programs[i])->score += !prog_dead[i];
	return RET_TIE;
}

void init_mem() {
	for(int i = 0; i < MEM_SIZE; i++)
		memory[i] = 0;
}

int cmpProg(const void* prog1, const void* prog2) {
	return ((program_t*)prog2)->score - ((program_t*)prog1)->score;
}

void display_score(program_t programs[]) {
	qsort(programs, PROG_COUNT, sizeof(program_t), cmpProg);

	puts("Place | Name             | Score");
	puts("------+------------------+------");
	for(int i = 0; i < PROG_COUNT; i++)
		printf(" #%-3d | %-16s | %d\n", i+1, programs[i].name, programs[i].score);
}
