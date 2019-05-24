#include "screensaver.h"
#include "toaster.h"

#include "commands.h"
#include "command_handlers.h"

#include "apps_list.h"
#include "buttons.h"
#include "udi_cdc.h"
#include "lcd.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>


#define N_FLYERS   5 // Number of flying things

struct Flyer {       // Array of flying things
  int16_t x, y;      // Top-left position * 16 (for subpixel pos updates)
  int8_t  depth;     // Stacking order is also speed, 12-24 subpixels/frame
  uint8_t frame;     // Animation frame; Toasters cycle 0-3, Toast=255
} flyer[N_FLYERS];

uint32_t startTime;

// Flyer depth comparison function for qsort()
static int compare(const void *a, const void *b) {
  return ((struct Flyer *)a)->depth - ((struct Flyer *)b)->depth;
}

void screensaver_init(void) {
  for(uint8_t i=0; i<N_FLYERS; i++) {  // Randomize initial flyer states
    flyer[i].x     = (-32 + (random()%160)) * 16;
    flyer[i].y     = (-32 + (random()%96)) * 16;
    flyer[i].frame = (random()%3) ? (random()%4) : 255; // 66% toaster, else toast
    flyer[i].depth = 10 + (random()%16);             // Speed / stacking order
  }
  qsort(flyer, N_FLYERS, sizeof(struct Flyer), compare); // Sort depths

}

void app_screensaver_func(void) {

    while(SSD1306_isBusy());
    SSD1306_clearBufferFull();


    while(btn_state() != BUT_LEFT) {
        uint8_t i, f;
        int16_t x, y;
        bool resort = false;     // By default, don't re-sort depths

        SSD1306_drawBufferDMA();          // Update screen to show current positions
        SSD1306_clearBufferFull();     // Start drawing next frame

        for(i=0; i<N_FLYERS; i++) { // For each flyer...

            // First draw each item...
            f = (flyer[i].frame == 255) ? 4 : (flyer[i].frame++ & 3); // Frame #
            x = flyer[i].x / 16;
            y = flyer[i].y / 16;
            SSD1306_drawBitmapColor(x, y, mask[f], 32, 32, BLACK);
            SSD1306_drawBitmapColor(x, y, img[f], 32, 32, WHITE);

            // Then update position, checking if item moved off screen...
            flyer[i].x -= flyer[i].depth * 2; // Update position based on depth,
            flyer[i].y += flyer[i].depth;     // for a sort of pseudo-parallax effect.
            if((flyer[i].y >= (64*16)) || (flyer[i].x <= (-32*16))) { // Off screen?
                if((random()%7) < 5) {         // Pick random edge; 0-4 = top
                    flyer[i].x = (random()%160) * 16;
                    flyer[i].y = -32         * 16;
                } else {                    // 5-6 = right
                    flyer[i].x = 128         * 16;
                    flyer[i].y = (random()%64)  * 16;
                }
                flyer[i].frame = (random()%3) ? (random()%4) : 255; // 66% toaster, else toast
                flyer[i].depth = 10 + (random()%16);
                resort = true;
            }
        }
        // If any items were 'rebooted' to new position, re-sort all depths
        if(resort) qsort(flyer, N_FLYERS, sizeof(struct Flyer), compare);

        for (int i = 0; i < 3000000; i++)
            asm("nop");
    }

    while(btn_state());    /* wait for the button release */
}
