#include "CPU.h"
#include "MemorySub.h"


BYTE cpu_read8(GBC_MemoryMap* mem, ADDR addr) { return read8(mem , addr);}
BYTE cpu_write8(GBC_MemoryMap* mem, ADDR addr, BYTE val) { return write8(mem, addr, val);}

CPU* CPUConstructor()
{
	CPU* cpu = malloc(sizeof(CPU));
	if (cpu == NULL){ printf("CPU Failed to Allocate on Heap Memory \n"); exit(1);}
	
	//Complete Registers.
	cpu->PC = 0x0000;
	cpu->AF.AF = 0x01B0;
	cpu->BC.BC = 0x0013;
	cpu->DE.DE = 0x00D8;
	cpu->HL.HL = 0x014D;

	cpu->SP = 0xFFFE;
	return cpu;
}