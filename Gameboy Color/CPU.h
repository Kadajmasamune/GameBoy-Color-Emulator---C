#ifndef CPU_H
#define CPU_H
#include "Common.h"
#include <stdint.h>

// forward-declare memory map type so CPU.h doesn't need to include MemorySub.h
typedef struct GBC_MemoryMap GBC_MemoryMap;

//Flag Bit positions inside the F Register
#define FLAG_Z (1<<7) // Zero Flag
#define FLAG_A_S (1<<6) // Add/Sub Flag
#define FLAG_H_C (1<<5) // Half Carry Flag
#define FLAG_CF (1<<4) // Carrry Flag

typedef union {
    struct {
        uint8_t F;  // Lower byte (Flags)
        uint8_t A;  // Upper byte (Accumulator)
    };
    uint16_t AF;     // Combined 16-bit access
} AFRegister;


typedef union {
    struct {
        uint8_t C;
        uint8_t B;
    };
    uint16_t BC;
} BCRegister;


typedef union {
    struct {
        uint8_t E;
        uint8_t D;
    };
    uint16_t DE;
} DERegister;


typedef union {
    struct {
        uint8_t L;
        uint8_t H;
    };
    uint16_t HL;
} HLRegister;

typedef void (*InstructionFn)(CPU* cpu, GBC_MemoryMap* mem);


typedef struct {
    InstructionFn table[256];
    BYTE cycles[256];
} CPUInstructionSet;


typedef struct
{
    void* GARBAGE;

}OPCODES;
typedef struct {
    AFRegister AF;
    BCRegister BC;
    DERegister DE;
    HLRegister HL;

    uint16_t SP;  // Stack Pointer
    uint16_t PC;  // Program Counter
    
	CPUInstructionSet* instructionSet;
} CPU;

// Read/write helpers that operate on the memory map
BYTE cpu_read8(GBC_MemoryMap* mem, ADDR addr);
BYTE cpu_write8(GBC_MemoryMap* mem, ADDR addr, BYTE val);

CPU* CPUConstructor();

void execute(CPU* cpu, GBC_MemoryMap* mem, BYTE opcode);
BYTE fetch(CPU* cpu, GBC_MemoryMap * mem);

void CPU_Step(CPU* cpu, GBC_MemoryMap* mem);



//Implement Fetch Execute Cycle Here.

#endif // !CPU_H
