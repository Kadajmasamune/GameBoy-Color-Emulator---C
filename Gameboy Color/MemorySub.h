#ifndef MEMORY_SUB
#define MEMORY_SUB
#include "Common.h"
	
#include <stdint.h>

//Complete Memory Map is 64 KiloBytes.

// Give the struct a tag so other headers can forward-declare `struct GBC_MemoryMap`.
typedef struct GBC_MemoryMap {
    uint8_t* MEM; // 64KB unified memory block (malloc(0x10000))

    uint8_t* ROM_BANK_0;          // 0x0000 - 0x3FFF (16KB)
    uint8_t* ROM_BANK_SWITCHABLE; // 0x4000 - 0x7FFF (16KB)
    uint8_t* VRAM;                // 0x8000 - 0x9FFF (8KB)
    uint8_t* EXTERNAL_RAM;        // 0xA000 - 0xBFFF (8KB)
    uint8_t* WRAM;                // 0xC000 - 0xDFFF (8KB)
    uint8_t* ECHO_RAM;            // 0xE000 - 0xFDFF (mirror of WRAM)
    uint8_t* OAM;                 // 0xFE00 - 0xFE9F (sprite attribute table)
    uint8_t* UNUSED;              // 0xFEA0 - 0xFEFF (unusable)
    uint8_t* IO_REG;              // 0xFF00 - 0xFF7F (I/O registers)
    uint8_t* HRAM;                // 0xFF80 - 0xFFFE (high RAM)
    uint8_t* IE;                  // 0xFFFF (interrupt enable register)

} GBC_MemoryMap;



BYTE read8(GBC_MemoryMap* mem, ADDR addr);
BYTE write8(GBC_MemoryMap* mem , ADDR address, BYTE data);

void destroyMemoryMap(GBC_MemoryMap* mem);
GBC_MemoryMap* initMemoryMap();

#endif // !MEMORY_SUB
