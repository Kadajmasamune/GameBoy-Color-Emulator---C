#include "EMULATOR.h"
#include "CART_HEADER.h"

Emulator* initEmulator()
{
	GBC_MemoryMap* mem = initMemoryMap();
	CPU* cpu = CPUConstructor();
	Cart* cart = constructCart(DUMPED_ROM_FOO);
	Cartridge_Header* header = constructCartHeader();
	Emulator* emu = malloc(sizeof(Emulator));


	if (emu == NULL)
	{
		printf("Failed to Initialize Emulator.. Exiting \n");
		exit(1);
	}
	//Add Cartridge into ROM BANK 0
	emu->cartridge = cart;
	emu->header = header;
	emu->mem = mem;
	emu->cpu = cpu;

	return emu;
}

void DestroyEmulator(Emulator* emu)
{
	free(emu);
}
