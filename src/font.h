// Converted using ConvPNG
// This file contains all the graphics sources for easier inclusion in a project
#ifndef __font__
#define __font__
#include <stdint.h>

#define font_transparent_color_index 0

#define font_width 8
#define font_height 312
#define font_size 2498
extern uint8_t font_data[2498];
#define font ((gfx_sprite_t*)font_data)
#define sizeof_font_pal 6
extern uint16_t font_pal[3];

#endif
