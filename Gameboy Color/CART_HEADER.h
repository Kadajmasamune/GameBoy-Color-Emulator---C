#ifndef CART_HEADER_H
#define CART_HEADER_H

#include "Common.h"  
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DUMPED_ROM_FOO "F:\\Projects\\GameBoy-Color-Emulator---C\\GameBoyColor\\Legend of Zelda, The - Oracle of Ages (USA, Australia)\\Legend of Zelda, The - Oracle of Ages (USA, Australia).gbc"


typedef uint8_t BYTE;
typedef uint16_t ADDR;   // address/offset size — 16-bit is enough for GB headers

typedef struct {
    ADDR start;
    ADDR end;
} MemoryRange;

typedef struct {
    MemoryRange EntryPoint;         // 0x0100 - 0x0103
    MemoryRange NintendoLogo;       // 0x0104 - 0x0133 (48 bytes)
    MemoryRange Title;              // 0x0134 - 0x0143 (16 bytes)
    MemoryRange ManufacturerCode;   // 0x013F - 0x0142 (4 bytes)
    ADDR CGB_FLAG;                  // 0x0143
    MemoryRange NEW_LICENSEE_CODE;  // 0x0144 - 0x0145 (2 bytes)
    ADDR SGB_FLAG;                  // 0x0146
    ADDR CARTRIDGE_TYPE;            // 0x0147
    ADDR ROM_SIZE;                  // 0x0148
    ADDR RAM_SIZE;                  // 0x0149
    ADDR DestinationCode;           // 0x014A
    ADDR OldLicenseeCode;           // 0x014B
    ADDR MaskRomVersionNumber;      // 0x014C
    ADDR Header_Checksum;           // 0x014D
    MemoryRange GlobalChecksum;     // 0x014E - 0x014F (2 bytes)
} Cartridge_Header;

/* Cart (runtime) representation:
   Use fixed-size arrays for fields with well-known lengths.
   Ownership: constructCart allocates Cart and its internal buffers;
   caller should free() it via a corresponding destructor.
*/
typedef struct {
    BYTE EntryPoint[4];
    BYTE NintendoLogo[48];
    BYTE Title[16];                // often null-terminated but may not be
    BYTE ManufacturerCode[4];
    BYTE CGB_FLAG;
    BYTE NEW_LICENSEE_CODE[2];
    BYTE SGB_FLAG;
    BYTE CARTRIDGE_TYPE;
    BYTE ROM_SIZE;
    BYTE RAM_SIZE;
    BYTE DestinationCode;
    BYTE OldLicenseeCode;
    BYTE MaskRomVersionNumber;
    BYTE Header_Checksum;
    uint16_t GlobalChecksum;

    // BYTE computed_Header_Checksum;  
} Cart;


typedef enum CartridgeType
{
    GarbageValue = 0x00
};

enum ROMSize
{
    GM = 2
};




/* API */

//Header Contains The Memory Ranges Of Various Fields In The Cartridge Header
Cartridge_Header* constructCartHeader(void);

//Constructs A Cart Object from a given Rom FILE
Cart* constructCart(const char* rom_path);

//Destroys the Cart from the Heap, since constructCart() uses malloc() to allocate the Object on the heap.
void destroyCart(Cart* cart);

//Reads Bytes from a Given Rom File, stores the output in the Destination Buffer, and further allows you to pinpoint the Offset and Number of Bytes to Read.
size_t read_bytes(FILE* rom, void* dest, uint32_t offset, size_t bytesToRead);

//Computes the Header Checksum from a Given ROM Byte Array and Cartridge Header Structure
BYTE computeHeaderChecksum(const BYTE* rom, const Cartridge_Header* header);



/* Optionally: BYTE computeHeaderChecksumFromFile(FILE* rom, const Cartridge_Header* header); */

#endif // CART_HEADER_H
