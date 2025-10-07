#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "./memmon.c"

#include "./examples/small_boy.c"
#include "./examples/runner.c"

//#define DEBUG
//#define VERBOSE
#define SILENT

//#define MEMMON
#ifdef MEMMON
	#define MATCH_TIME 100	//in seconds
#else
	#define MATCH_TIME 2	//in seconds
#endif

#define PROG_COUNT 2		//number of programs
#define NUM_OF_ROUNDS 16
#define MAX_PROG_SIZE 1024	//max number of instructions
#define MEM_SIZE (1<<16)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define UNUSED(value) (void)(value)

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
	OP_MV = 0x20,
	OP_ADD,
	OP_SUB,
	OP_NOT,
	OP_AND,
	OP_OR,
	OP_XOR,
	OP_SHL,
	OP_SHR,
	OP_JMP,
	OP_JZ,
	OP_JNZ,
	OP_JN,
	OP_JP,
	OP_LD,
	OP_ST,
	OP_PUSH,
	OP_POP,
	OP_ADDI,
	OP_SUBI,
	OP_SHLI,
	OP_SHRI,
	OP_FLAG = 0x3f
};

typedef struct {
	char name[16];
	Color color;
	uint16_t score;
	uint16_t reg[REG_COUNT];
	int16_t life;
	uint16_t org;
	uint16_t offset;
	uint16_t size;
	uint16_t* program_mem;
} program_t;

uint16_t get_rand_org(bool reset);
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
	
	srand(time(0));

#ifdef MEMMON
	memmon_init();
#endif

	
	program_t progs[PROG_COUNT] = {
		{.name = "Small_boy", .program_mem = small_boy_mem, .offset = small_boy_offset, .size = small_boy_size},
		{.name = "Runner", .program_mem = runner_mem, .offset = runner_offset, .size = runner_size},
	};
	
	for (int round = 0; round < NUM_OF_ROUNDS; round++) {
		printf("\nRound %d started...", round);

		for (int i = 0; i < PROG_COUNT; i++) {
#ifdef VERBOSE
			printf("\n\tGame %d started...\n", i);
#endif

			int winner = -1;
			init_programs(progs, PROG_COUNT);
			int ret = run_match(progs, PROG_COUNT, i, &winner);

#ifndef SILENT
			if (ret == RET_OK) {
				printf("\t%s won!\n\n", progs[winner].name);
			} else if (ret == RET_TIE) {
				printf("\tTie!\n\n");
			}
#else
			UNUSED(ret);
#endif

#ifdef MEMMON
			memon_reset();
#endif
		}
	}

	display_score(progs);
#ifdef MEMMON
	memmon_close();
#endif

}

uint16_t get_rand_org(bool reset) {
	static bool prog_cells[MEM_SIZE/MAX_PROG_SIZE] = {0};
	uint16_t org;

	if (reset) {
		for (int i = 0; i < (MEM_SIZE/MAX_PROG_SIZE); i++)
			prog_cells[i] = 0;
		return 0;
	}

	do {
		org = rand() % (MEM_SIZE/MAX_PROG_SIZE);
	} while(prog_cells[org]);

	prog_cells[org] = 1;
	return org * MAX_PROG_SIZE;
}

void init_program(program_t* program) {
	program->org = get_rand_org(0) + (((program->offset + program->size) <= MAX_PROG_SIZE) ? program->offset : 0);
#ifdef DEBUG
	printf("%s org : 0x%X\n",program->name, program->org);
#endif

	program->size = MIN(program->size, MAX_PROG_SIZE);

	for(uint16_t i = 0; i < program->size; i++) {
		memory[program->org + i] = program->program_mem[i];
#ifdef MEMMON
		memmon_update_pixle((program->org + i), program->color);
#endif
	}
	for(uint16_t i = 0; i < REG_COUNT; i++)
		program->reg[i] = 0;

	program->reg[PC] = program->org;
	program->life = 100;
}

int init_programs(program_t programs[], size_t len) {
	if (len > 64)
		return RET_FAILED;
	
	init_mem();

	get_rand_org(1);

	for(int i = 0; i < len; i++)
		programs[i].color = colors[i % NUM_OF_COLORS];

	for(int i = 0; i < len; i++)
		init_program(&programs[i]);

	return RET_OK;
}

void process_instruction(program_t* program) {
	uint16_t instruction = memory[program->reg[PC]];


	uint16_t sr1 = (instruction>>5) & 0x1f;
	uint16_t sr2 = (instruction) & 0x1f;
	uint16_t val = sr2;
	//load instruction
	if((instruction>>15) == OP_LDI) {
		program->reg[R0] = instruction;
		goto process_instruction_end;
	}
	switch(instruction>>10) {
		case OP_MV:
			program->reg[sr1] = program->reg[sr2];
			break;
		case OP_ADD:
			program->reg[sr1] += program->reg[sr2];
			break;
		case OP_SUB:
			program->reg[sr1] -= program->reg[sr2];
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
		case OP_XOR:
			program->reg[sr1] ^= program->reg[sr2];
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
			program->life--;
			break;
		case OP_JNZ:
			if(program->reg[sr2])
				program->reg[PC] = program->reg[sr1];
			program->life--;
			break;
		case OP_JN:
			if(program->reg[sr2] >> 15)
				program->reg[PC] = program->reg[sr1];
			program->life--;
			break;
		case OP_JP:
			if(!(program->reg[sr2] >> 15) && program->reg[sr2])
				program->reg[PC] = program->reg[sr1];
			program->life--;
			break;
		case OP_LD:
			program->reg[sr1] = memory[program->reg[sr2]];
			break;
		case OP_ST:
			memory[program->reg[sr2]] = program->reg[sr1];
			program->life -= 9;
#ifdef MEMMON
			memmon_update_pixle(program->reg[sr2], program->color);
#endif
			break;
		case OP_PUSH:
			memory[program->reg[SP]] = program->reg[sr1];
#ifdef MEMMON
			memmon_update_pixle(program->reg[SP], program->color);
#endif
			program->reg[SP]++;
			program->life -= 9;
			break;
		case OP_POP:
			program->reg[sr1] = memory[program->reg[SP]];
			program->reg[SP]--;
			break;
		case OP_FLAG:
			program->life = 101;
			break;
		case OP_ADDI:
			program->reg[sr1] += val;
			break;
		case OP_SUBI:
			program->reg[sr1] -= val;
			break;
		case OP_SHLI:
			program->reg[sr1] <<= val;
			break;
		case OP_SHRI:
			program->reg[sr1] >>= val;
			break;
		default:
			program->life -= 9;
	}
process_instruction_end:
#ifdef DEBUG
	printf("\n\nProgram : %s\n", program->name);
	printf("life : %d\n", program->life);
	printf("instruction : 0b%016b\n", instruction);
	print_reg(*program);
#endif
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

	int sum = 0;
	for (int i = 0; i < PROG_COUNT; i++)
		sum += programs[i].score;

	puts("\nPlace | Name             | Score  | Percentage");
	puts("------+------------------+--------+-----------");
	for (int i = 0; i < PROG_COUNT; i++)
		printf(" #%-3d | %-16s | %-6d | %.2f%%\n", i + 1, programs[i].name, programs[i].score, (double)programs[i].score / sum * 100);

}
