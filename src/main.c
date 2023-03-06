#include "Types.h"
#include "SpriteService.h"
#include "AppMainLogic.h"

#include "Palette.h"

#define VRAM_bitmap 0x04000
#define VRAM_palette 0x1FA00

void main(void)
{
    int i, x, y;

    // Switch back to the uppercase character set.
    cbm_k_bsout(CH_FONT_UPPER);

    videomode(VIDEOMODE_320x240);
    // HSCALE and VSCALE will set the fractional scaling factor of the display.
    // Setting this value to 128 will output 1 output pixel for every input pixel.
    // Setting this to 64 will output 2 output pixels for every input pixel.
    VERA.display.hscale = 64;
    VERA.display.vscale = 64;

    VERA.layer0.config = 7; // Bitmap mode | 8 bpp
    VERA.layer0.tilebase = (VRAM_bitmap >> 9);

    // Clear layer
    VERA.control = 0;
    VERA.address_hi = 0x10; // Set the address to increment 1 with each access.
    VERA.address = VRAM_bitmap;
    for (y = 0; y < 240; y++) {
        for (x = 0; x < 320; x++) {
            VERA.data0 = 0;
        }
    }

    InitializeSprites();

    // Copy the palette data into the video RAM.
    VERA.address = VRAM_palette;
    for (i = 0; ++i < 256*2; ) {
        VERA.data0 = palette[i];
    }

    vera_sprites_enable(1);

    AppMainLogic_Initialize();

    puts("\npress any key to stop.");
    do {
        // Wait until the start of the next video frame.
//        vpoke(0x0f, 0x11FA00); // Raster counter
        waitvsync();
//        vpoke(0x00, 0x11FA00); // Raster counter

        AppMainLogic_ExecuteFrame();
    } while (!kbhit());
    cgetc();

    vera_sprites_enable(0);
}
