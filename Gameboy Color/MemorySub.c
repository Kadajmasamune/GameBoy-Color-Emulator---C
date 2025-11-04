#include "MemorySub.h"

BYTE read8(GBC_MemoryMap* mem, ADDR addr)
{
    if (addr < 0x4000)  // 0000–3FFF
        return mem->ROM_BANK_0[addr];

    else if (addr < 0x8000)  // 4000–7FFF
        return mem->ROM_BANK_SWITCHABLE[addr - 0x4000];

    else if (addr < 0xA000)  // 8000–9FFF
        return mem->VRAM[addr - 0x8000];

    else if (addr < 0xC000)  // A000–BFFF
        return mem->EXTERNAL_RAM[addr - 0xA000];

    else if (addr < 0xE000)  // C000–DFFF
        return mem->WRAM[addr - 0xC000];

    else if (addr < 0xFE00)  // E000–FDFF (Echo of C000–DDFF)
        return mem->WRAM[addr - 0xE000];

    else if (addr < 0xFEA0)  // FE00–FE9F
        return mem->OAM[addr - 0xFE00];

    else if (addr < 0xFF00)  // FEA0–FEFF (unusable)
        return 0xFF;

    else if (addr < 0xFF80)  // FF00–FF7F
        return mem->IO_REG[addr - 0xFF00];

    else if (addr < 0xFFFF)  // FF80–FFFE
        return mem->HRAM[addr - 0xFF80];

    else                     // FFFF
        return *mem->IE;
}


BYTE write8(GBC_MemoryMap* mem, ADDR address, BYTE data)
{
    // 0x0000–0x7FFF: ROM — writing here usually controls MBC registers (bank switching)
    if (address < 0x8000) {
        // Normally ignored or handled by MBC logic later.
        return 0;
    }

    // 0x8000–0x9FFF: VRAM
    else if (address >= 0x8000 && address <= 0x9FFF) {
        mem->VRAM[address - 0x8000] = data;
        return data;
    }

    // 0xA000–0xBFFF: External RAM
    else if (address >= 0xA000 && address <= 0xBFFF) {
        mem->EXTERNAL_RAM[address - 0xA000] = data;
        return data;
    }

    // 0xC000–0xDFFF: Work RAM
    else if (address >= 0xC000 && address <= 0xDFFF) {
        mem->WRAM[address - 0xC000] = data;
        return data;
    }

    // 0xE000–0xFDFF: Echo RAM (mirror of WRAM)
    else if (address >= 0xE000 && address <= 0xFDFF) {
        mem->WRAM[address - 0xE000] = data;
        return data;
    }

    // 0xFE00–0xFE9F: OAM (Sprite Attribute Table)
    else if (address >= 0xFE00 && address <= 0xFE9F) {
        mem->OAM[address - 0xFE00] = data;
        return data;
    }

    // 0xFEA0–0xFEFF: Unusable region
    else if (address >= 0xFEA0 && address <= 0xFEFF) {
        return 0xFF; // writes ignored
    }

    // 0xFF00–0xFF7F: I/O registers
    else if (address >= 0xFF00 && address <= 0xFF7F) {
        mem->IO_REG[address - 0xFF00] = data;
        return data;
    }

    // 0xFF80–0xFFFE: HRAM
    else if (address >= 0xFF80 && address <= 0xFFFE) {
        mem->HRAM[address - 0xFF80] = data;
        return data;
    }

    // 0xFFFF: Interrupt Enable Register
    else if (address == 0xFFFF) {
        *mem->IE = data;
        return data;
    }

    return 0; // Should never reach here
}



GBC_MemoryMap* initMemoryMap()
{
	//One long Cointiguous Block Of Memory that Allocates 65536 Bytes in Memory.
	//This will now be further sliced into sub-Divisions each for different Regions i.e. VRAM 

	GBC_MemoryMap* mem = malloc(sizeof(GBC_MemoryMap));
	if (mem == NULL)
	{
		printf("Failed to Construct Memory Map Pointer\n");
		exit(1);
	}

	mem->MEM = malloc(0x10000);
	if (mem->MEM == NULL)
	{
		printf("Failed to Allocate Memory within the Memory Map \n");
		exit(1);
	}


	mem->ROM_BANK_0 = mem->MEM + 0x0000;
	mem->ROM_BANK_SWITCHABLE = mem->MEM + 0x4000;
	mem->VRAM = mem->MEM + 0x8000;
	mem->EXTERNAL_RAM = mem->MEM + 0xA000;
	mem->WRAM = mem->MEM + 0xC000;
	mem->ECHO_RAM = mem->MEM + 0xE000;
	mem->OAM = mem->MEM + 0xFE00;
	mem->UNUSED = mem->MEM + 0xFEA0;
	mem->IO_REG = mem->MEM + 0xFF00;
	mem->HRAM = mem->MEM + 0xFF80;
	mem->IE = mem->MEM + 0xFFFF;

	return mem;
}
void destroyMemoryMap(GBC_MemoryMap* mem) {
	if (mem) {
		free(mem->MEM);
		free(mem);
	}
}
