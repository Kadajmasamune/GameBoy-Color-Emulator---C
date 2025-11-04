//
// Created by Rayan on 11/1/2025.
//

#ifndef COMMON_H
#define COMMON_H

#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

typedef uint8_t BYTE;
typedef uint16_t ADDR;   // address/offset size — 16-bit is enough for GB headers

typedef struct {
    ADDR start;
    ADDR end;
} MemoryRange;


#endif //COMMON_H
