#include <tice.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <graphx.h>
#include <keypadc.h>

#include "colors.h"
#include "tiles.h"
#include "oneone.h"
#include "mario.h"

void drawTiles();

void drawString(char* string, uint16_t sx, uint16_t sy);

void main(void) {
	char mxString[0x10000];
	
	copyTiles();
	copySprites();

	tilemap.map = level;
	tilemap.tiles = (gfx_sprite_t**)tiles;
	tilemap.x_loc = 32;
	tilemap.width = 40;
	tilemap.height = 15;
	tilemap.draw_width = 17;
	tilemap.draw_height = 15;
	tilemap.tile_width = 16;
	tilemap.tile_height = 16;
	tilemap.type_width = gfx_tile_16_pixel;
    tilemap.type_height = gfx_tile_16_pixel;
	
	gfx_Begin(gfx_8bpp);
	gfx_SetTransparentColor(0x40);
	gfx_SetPalette(colors, 129, 0);
	gfx_SetDrawBuffer();
	
	oneone();
	
	while(1) {
		kb_Scan();
		
		if(kb_Data[6] & kb_Clear) {break;}
		
		up = 0;
		left = 0;
		right = 0;
		
		if(kb_Data[7] & kb_Up) {up = 1;}
		if(kb_Data[1] & kb_2nd) {up = 1;}
		if(kb_Data[7] & kb_Left) {left = 1;}
		if(kb_Data[7] & kb_Right) {right = 1;}
		
		updateMario();
		updateMario();
		
		gfx_FillScreen(0x22);
		
		drawTiles();
		drawMario();
		
		gfx_SetColor(0x1f);
		gfx_FillRectangle(0, 0, 32, 240);
		gfx_FillRectangle(320 - 32, 0, 32, 240);
		
		gfx_Blit(gfx_buffer);
	}
	
	gfx_End();
}

void drawTiles() {gfx_Tilemap(&tilemap, x, 0);}

void drawString(char* string, uint16_t sx, uint16_t sy) {
	uint16_t i;
	
	for(i = 0; i < strlen(string); i++) {
		if(string[i] >= '0' && string[i] <= '9') {gfx_TransparentSprite((gfx_sprite_t*)testTiles[string[i] - '0' + 1], sx + i * 8, sy);}
		if(toupper(string[i]) >= 'A' && toupper(string[i]) <= 'Z') {gfx_TransparentSprite((gfx_sprite_t*)tiles[toupper(string[i]) - 'A' + 10 + 1], sx + i * 8, sy);}
		if(string[i] == '-') {gfx_TransparentSprite((gfx_sprite_t*)tiles[string[i] - '-' + 0x28 + 1], sx + i * 8, sy);}
		if(string[i] == '*') {gfx_TransparentSprite((gfx_sprite_t*)tiles[string[i] - '*' + 0x29 + 1], sx + i * 8, sy);}
		if(string[i] == '!') {gfx_TransparentSprite((gfx_sprite_t*)tiles[string[i] - '!' + 0x2b + 1], sx + i * 8, sy);}
	}
}