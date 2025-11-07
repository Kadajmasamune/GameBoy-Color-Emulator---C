#include <stdio.h>
#include "EMULATOR.h"

//TODO : Create CPU now

int main(void) {
    // openRom();
    // printf("Hello, World!\n");

    /*Cart* cart = constructCart(DUMPED_ROM_FOO);

    for (int i = 0; i < 16; i++) {
        printf("%02X", cart->Title[i]);
    }

    destroyCart(cart);*/
    
    /*GBC_MemoryMap* mem = initMemoryMap();

    destroyMemoryMap(mem);*/

    //A Region of Memory, Size 64 KB is allocated on the Heap for the Gameboy Color, This is the Memory Map, and the core of the Emulator where everything happens.
    //Current Flow of the project :
    /*
        Currently, a Memory Map is Initialized using the initMemoryMap() function (MemorySub.h)
        This returns a GBC_MemoryMap Object, Which has Various sectors each with their own specific Properties, i.e. VRAM for Holding Textures etc.
        
        As of Now, I need to keep everything Object oriented and make sure nothing Collides with another.

        1) Init Memory Map
        2) Store cartridge Header location in ROM_BANK_0
        3) Store Other Various Data in the other Sectors of the Map.
    */
    int i = 1;

    Emulator* emulator = initEmulator();
    
    if(i != 1) DestroyEmulator(emulator);

    return 0;
}