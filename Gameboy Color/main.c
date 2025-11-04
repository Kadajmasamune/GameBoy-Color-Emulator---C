#include <stdio.h>
#include "CART_HEADER.h"
#include "MemorySub.h"
//TODO : Create CPU now

int main(void) {
    // openRom();
    // printf("Hello, World!\n");

    /*Cart* cart = constructCart(DUMPED_ROM_FOO);

    for (int i = 0; i < 16; i++) {
        printf("%02X", cart->Title[i]);
    }

    destroyCart(cart);*/
    
    GBC_MemoryMap* mem = initMemoryMap();

    destroyMemoryMap(mem);
    return 0;
}