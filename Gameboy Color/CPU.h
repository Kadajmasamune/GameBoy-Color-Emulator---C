#ifndef CPU_H
#define CPU_H
#include "Common.h"

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


typedef struct {

    AFRegister AF;
    BCRegister BC;
    DERegister DE;
    HLRegister HL;

    uint16_t SP;  // Stack Pointer
    uint16_t PC;  // Program Counter


} CPU;


#endif // !CPU_H
