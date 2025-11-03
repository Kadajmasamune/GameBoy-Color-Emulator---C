//
// Created by Rayan on 11/1/2025.
//
#include "CART_HEADER.h"

size_t read_bytes(FILE* rom, void* dest, uint32_t offset, size_t bytesToRead) {
    if (!rom || !dest) {
        fprintf(stderr, "read_bytes: invalid argument\n");
        return 0;
    }

    if (fseek(rom, (long)offset, SEEK_SET) != 0) {
        perror("fseek");
        return 0;
    }

    size_t got = fread(dest, 1, bytesToRead, rom);
    if (got != bytesToRead) {
        fprintf(stderr, "Warning: expected %zu bytes at 0x%04X, read %zu\n",
            bytesToRead, offset, got);
    }
    return got;
}


Cartridge_Header* constructCartHeader(void) {
    Cartridge_Header* h = malloc(sizeof(Cartridge_Header));
    if (!h) exit(EXIT_FAILURE);

    h->EntryPoint = (MemoryRange){ 0x0100, 0x0103 };
    h->NintendoLogo = (MemoryRange){ 0x0104, 0x0133 };
    h->Title = (MemoryRange){ 0x0134, 0x0143 };
    h->ManufacturerCode = (MemoryRange){ 0x013F, 0x0142 };
    h->CGB_FLAG = 0x0143;
    h->NEW_LICENSEE_CODE = (MemoryRange){ 0x0144, 0x0145 };
    h->SGB_FLAG = 0x0146;
    h->CARTRIDGE_TYPE = 0x0147;
    h->ROM_SIZE = 0x0148;
    h->RAM_SIZE = 0x0149;
    h->DestinationCode = 0x014A;
    h->OldLicenseeCode = 0x014B;
    h->MaskRomVersionNumber = 0x014C;
    h->Header_Checksum = 0x014D;
    h->GlobalChecksum = (MemoryRange){ 0x014E, 0x014F };
    return h;
}

Cart* constructCart(const char* rom_path) {
    FILE* f = fopen(rom_path, "rb");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    Cartridge_Header* h = constructCartHeader();
    Cart* c = malloc(sizeof(Cart));

    if (!c) { fclose(f); free(h); return NULL; }

    // read EntryPoint (4 bytes)
    read_bytes(f, c->EntryPoint, h->EntryPoint.start,
        h->EntryPoint.end - h->EntryPoint.start + 1);

    // Nintendo logo (48 bytes)
    read_bytes(f, c->NintendoLogo, h->NintendoLogo.start,
        h->NintendoLogo.end - h->NintendoLogo.start + 1);

    // Title (16 bytes)
    read_bytes(f, c->Title, h->Title.start,
        h->Title.end - h->Title.start + 1);

    // Manufacturer code (4 bytes covering 0x013F-0x0142)
    read_bytes(f, c->ManufacturerCode, h->ManufacturerCode.start,
        h->ManufacturerCode.end - h->ManufacturerCode.start + 1);

    // single-byte flags
    read_bytes(f, &c->CGB_FLAG, h->CGB_FLAG, 1);

    // new licensee (2 bytes)
    read_bytes(f, c->NEW_LICENSEE_CODE, h->NEW_LICENSEE_CODE.start,
        h->NEW_LICENSEE_CODE.end - h->NEW_LICENSEE_CODE.start + 1);

    read_bytes(f, &c->SGB_FLAG, h->SGB_FLAG, 1);
    read_bytes(f, &c->CARTRIDGE_TYPE, h->CARTRIDGE_TYPE, 1);
    read_bytes(f, &c->ROM_SIZE, h->ROM_SIZE, 1);
    read_bytes(f, &c->RAM_SIZE, h->RAM_SIZE, 1);
    read_bytes(f, &c->DestinationCode, h->DestinationCode, 1);
    read_bytes(f, &c->OldLicenseeCode, h->OldLicenseeCode, 1);
    read_bytes(f, &c->MaskRomVersionNumber, h->MaskRomVersionNumber, 1);
    read_bytes(f, &c->Header_Checksum, h->Header_Checksum, 1);


    // Computation of Header Checksum Below >>>>>

    // BYTE x = 0;
    // for(uint16_t i = h->Title.start; i <= h->MaskRomVersionNumber; ++i) {
    //     BYTE b = 0;
    //     read_bytes(f, &b, i, 1);
    //     x = x - b - 1;
    // }


    // global checksum (2 bytes)
    BYTE global[2] = { 0 };
    read_bytes(f, global, h->GlobalChecksum.start, 2);
    c->GlobalChecksum = ((uint16_t)global[0] << 8) | global[1];


    fclose(f);
    free(h);
    return c;
}

void destroyCart(Cart* cart) {
    free(cart);
}


BYTE computeHeaderChecksum(const BYTE* rom, const Cartridge_Header* header) {
    // header->Title.start is 0x0134; header->MaskRomVersionNumber is 0x014C
    uint8_t x = 0;
    for (uint16_t i = header->Title.start; i <= header->MaskRomVersionNumber; ++i) {
        x = x - rom[i] - 1;
    }
    return x;
}