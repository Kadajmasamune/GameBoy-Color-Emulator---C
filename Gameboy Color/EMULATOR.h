#ifndef EMULATOR_H
#define EMULATOR_H
#include "MemorySub.h"
#include "CART_HEADER.h"
#include "CPU.h"
#include <stdbool.h>
typedef struct
{ 
	GBC_MemoryMap* mem;
	CPU* cpu;
	Cart* cartridge;
	Cartridge_Header* header;
	//Add on More Components

}Emulator;

Emulator* initEmulator();
void DestroyEmulator(Emulator* emu);

bool CompareNintendoLogo(Emulator* emu);


#endif // !EMULATOR_H
