#include "EMULATOR.h"
#include "CART_HEADER.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

Emulator* initEmulator()
{
	GBC_MemoryMap* mem = initMemoryMap();
	CPU* cpu = CPUConstructor();
	Cart* cart = constructCart(DUMPED_ROM_FOO_YUGI);
	//Cart* cart = constructCart(DUMPED_ROM_FOO_ZELDA);

	Cartridge_Header* header = constructCartHeader();
	Emulator* emu = malloc(sizeof(Emulator));

	if (emu == NULL)
	{
		fprintf(stderr, "Failed to Initialize Emulator.. Exiting \n");
		exit(1);
	}

	// validate subsystem construction
	if (!mem || !cpu || !cart || !header) {
		fprintf(stderr, "Failed to allocate/init subsystems\n");
		if (cart) destroyCart(cart);
		if (mem) destroyMemoryMap(mem);
		if (cpu) free(cpu);
		if (header) free(header);
		free(emu);
		exit(1);
	}

	printf("Writing Rom To ROM BANK 0 ... \n");

	emu->cartridge = cart;
	emu->header = header;
	emu->mem = mem;
	emu->cpu = cpu;

	// WRAM cleared
	memset(emu->mem->WRAM, 0x00, 0x2000);

	printf("Copying Cartridge Header to Memory Map ... \n");

	// Copy each header field into the correct ROM addresses (0x0100 - 0x014F)
	memcpy(&emu->mem->ROM_BANK_0[0x0100], cart->EntryPoint, 4);            // 0x0100-0x0103
	memcpy(&emu->mem->ROM_BANK_0[0x0104], cart->NintendoLogo, 48);         // 0x0104-0x0133
	memcpy(&emu->mem->ROM_BANK_0[0x0134], cart->Title, 16);                // 0x0134-0x0143 (Title overlaps ManufacturerCode area; Manufacturer at 0x013F)
	memcpy(&emu->mem->ROM_BANK_0[0x013F], cart->ManufacturerCode, 4);      // 0x013F-0x0142
	emu->mem->ROM_BANK_0[0x0143] = cart->CGB_FLAG;                         // 0x0143
	memcpy(&emu->mem->ROM_BANK_0[0x0144], cart->NEW_LICENSEE_CODE, 2);     // 0x0144-0x0145
	emu->mem->ROM_BANK_0[0x0146] = cart->SGB_FLAG;                         // 0x0146
	emu->mem->ROM_BANK_0[0x0147] = cart->CARTRIDGE_TYPE;                   // 0x0147
	emu->mem->ROM_BANK_0[0x0148] = cart->ROM_SIZE;                         // 0x0148
	emu->mem->ROM_BANK_0[0x0149] = cart->RAM_SIZE;                         // 0x0149
	emu->mem->ROM_BANK_0[0x014A] = cart->DestinationCode;                  // 0x014A
	emu->mem->ROM_BANK_0[0x014B] = cart->OldLicenseeCode;                  // 0x014B
	emu->mem->ROM_BANK_0[0x014C] = cart->MaskRomVersionNumber;             // 0x014C
	emu->mem->ROM_BANK_0[0x014D] = cart->Header_Checksum;                  // 0x014D
	emu->mem->ROM_BANK_0[0x014E] = (uint8_t)(cart->GlobalChecksum >> 8);   // 0x014E (high)
	emu->mem->ROM_BANK_0[0x014F] = (uint8_t)(cart->GlobalChecksum & 0xFF); // 0x014F (low)

	if (!CompareNintendoLogo(emu))
	{
		fprintf(stderr, "Nintendo Logo Mismatch ! Exiting ... \n");
		DestroyEmulator(emu);
		exit(1);
	}

	BYTE checksum = computeHeaderChecksum(emu->mem->ROM_BANK_0, emu->header);

	printf("Computing Header checkSum and Validating... \n");
	if (checksum != emu->mem->ROM_BANK_0[0x014D])
	{
		fprintf(stderr, "Header Checksum Mismatch ! Exiting ... \n");
		DestroyEmulator(emu);
		exit(1);
	}

	printf("Cartridge Header Copied Successfully ! \n");
	printf("Cartridge Checksum PASSED ! \n");
	printf("Emulator Initialized Successfully ! \n");

	return emu;
}

bool CompareNintendoLogo(Emulator* emu)
{
	const BYTE NintendoLogo[48] = {
		0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
		0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
		0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
		0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
		0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
		0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
	};

	for (int i = 0; i < 48; i++) {
		if (emu->cartridge->NintendoLogo[i] != NintendoLogo[i]) {
			return false;
		}
	}
	return true;
}

void DestroyEmulator(Emulator* emu)
{
	if (!emu) return;

	// free owned resources (match allocations in initEmulator)
	if (emu->cartridge) destroyCart(emu->cartridge);
	if (emu->header) free(emu->header);
	if (emu->mem) destroyMemoryMap(emu->mem);
	if (emu->cpu) free(emu->cpu);

	free(emu);
}
