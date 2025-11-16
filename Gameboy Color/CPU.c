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

	cpu->instructionSet = CPUInstructionSetConstructor();
	cpu->fetch = &fetch;
	cpu->execute = &execute;
	return cpu;
}


BYTE fetch(CPU* cpu, GBC_MemoryMap* mem) 
{
	BYTE opcode = cpu_read8(mem, cpu->PC);
	cpu->PC++;
	return opcode;
}

void execute(CPU* cpu, GBC_MemoryMap* mem, BYTE opcode) {
	switch (opcode) {
	case 0x7F:  // LD A,A
		cpu->AF.A = cpu->AF.A;
		break;

	case 0x78:  // LD A,B
		cpu->AF.A = cpu->BC.B;
		break;

	case 0x40:  // LD B,B
		cpu->BC.B = cpu->BC.B;
		break;

	
	default:
		printf("Unknown opcode: 0x%02X\n", opcode);
		exit(1);
	}
}

void CPU_Step(CPU* cpu, GBC_MemoryMap* mem)
{
	BYTE opcode = fetch(cpu, mem);

	InstructionFn handler = cpu->instructionSet->table[opcode];
	if (handler) {
		handler(cpu, mem);
	}
	else {
		printf("Unknown opcode: 0x%02X at PC=0x%04X\n", opcode, cpu->PC - 1);
		exit(1);
	}
}
// ----------------------
// Register → Register
// ----------------------
void LD_r_r(CPU* cpu, BYTE* dest, BYTE* src)
{
	*dest = *src;
}

// ----------------------
// Immediate → Register
// ----------------------
void LD_r_n(CPU* cpu, BYTE* dest, BYTE value) 
{
	*dest = value;
}

// ----------------------
// (HL) Memory operations
// ----------------------
void LD_HL_r(CPU* cpu, GBC_MemoryMap* mem, BYTE value)
{
	cpu_write8(mem, cpu->HL.HL, value);
}

BYTE LD_r_HL(CPU* cpu, GBC_MemoryMap* mem) 
{
	return cpu_read8(mem, cpu->HL.HL);
}

// ----------------------
// 16-bit operations
// ----------------------
void LD_nn_A(CPU* cpu, GBC_MemoryMap* mem, uint16_t addr)
{
	cpu_write8(mem, addr, cpu->AF.A);
}

void LD_A_nn(CPU* cpu, GBC_MemoryMap* mem, uint16_t addr) 
{
	cpu->AF.A = cpu_read8(mem, addr);
}

//void CPUInstructionSetInit(CPUInstructionSet* set) {
//	for (int i = 0; i < 256; i++) {
//		set->table[i] = NULL;
//		set->cycles[i] = 0;
//	}
//
//	// Example: LD r,r
//	// A=0, B=1, C=2, D=3, E=4, H=5, L=6 (register codes)
//	BYTE* regs[] = {&cpu->BC.B, &cpu->BC.C, &cpu->DE.D, &cpu->DE.E, &cpu->HL.H, &cpu->HL.L, &cpu->AF.A };
//
//	for (int dest = 0; dest < 7; dest++) {
//		for (int src = 0; src < 7; src++) {
//			BYTE opcode = 0x40 + dest * 8 + src; // LD r,r formula from GB manual
//			set->table[opcode] = [dest, src](CPU* cpu, GBC_MemoryMap* mem) {
//				LD_r_r(cpu, regs[dest], regs[src]);
//				};
//			set->cycles[opcode] = 4;
//		}
//	}
//
//	// You can repeat similar logic for:
//	// - LD r,(HL)
//	// - LD (HL),r
//	// - LD r,n
//	// - LDI / LDD
//	// - INC / DEC registers
//	// - 16-bit loads
//	// - Special memory ops (A,(BC), etc.)
//}
