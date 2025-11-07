#include "CPU.h"
#include "MemorySub.h"


BYTE cpu_read8(CPU* cpu, ADDR addr) { return read8(cpu->mem , addr);}
void cpu_write8(CPU* cpu, ADDR addr, BYTE val) { return write8(cpu->mem, addr, val);}

CPU* CPUConstructor()
{
	CPU* cpu = malloc(sizeof(CPU));
	if (cpu == NULL){ printf("CPU Failed to Allocate on Heap Memory \n"); exit(1);}

	//Complete Registers.
	cpu->PC = 0x1000;
	return cpu;
}