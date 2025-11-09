#include <stdio.h>
#include "EMULATOR.h"

//TODO : Create CPU now
#if defined(_WIN32)
#include <windows.h>
#endif

int main(int argc , char* argv) {
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

	//Argc and Argv will be used to pass the Rom Path from Command Line Arguments.

#if defined(_DEBUG)
    // Temporary startup pause to allow attaching debugger or setting breakpoints
#if defined(_WIN32)
    printf("PID: %u — attach debugger now, then press Enter to continue...\n", (unsigned)GetCurrentProcessId());
#else
    printf("Attach debugger now, then press Enter to continue...\n");
#endif
    getchar();
#endif

    Emulator* emulator = initEmulator();

    printf("Starting Emulation Loop ... \n");
    printf("Reading Cartridge Title.. \n");
    for (int i = 0; i < 16; i++) {
        printf("%c", emulator->cartridge->Title[i]);
    }
    DestroyEmulator(emulator);

    return 0;
}