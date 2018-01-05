#include <stdint.h>
#include <stdlib.h>

#include "oneone.h"

uint16_t width = 80; // level width
uint16_t height = 30; // level height

//uint16_t x; // screen x
//uint32_t mx; // mario's x position (1/256 of a pixel)
//uint32_t my; // mario's y position (1/256 of a pixel)

gfx_tilemap_t tilemap;

uint8_t level[2400];

uint16_t blocks;
uint16_t xBlocks[80]; // width
uint16_t yBlocks[80]; // width

void newBlock(uint16_t bx, uint16_t by, uint16_t i);

void oneone() {
	uint8_t bush0[8] = {0x24, 0x36, 0x37, 0x36, 0x37, 0x36, 0x37, 0x24};
	uint8_t bush1[8] = {0x35, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x38};
	uint8_t bush2[4] = {0x24, 0x36, 0x37, 0x24};
	uint8_t bush3[4] = {0x35, 0x25, 0x25, 0x38};
	uint8_t bush4[6] = {0x24, 0x36, 0x37, 0x36, 0x37, 0x24};
	uint8_t bush5[6] = {0x35, 0x25, 0x25, 0x25, 0x25, 0x38};
	uint8_t hill0[10] = {0x24, 0x24, 0x24, 0x24, 0x31, 0x32};
	uint8_t hill1[10] = {0x24, 0x24, 0x24, 0x30, 0x26, 0x34, 0x33};
	uint8_t hill2[10] = {0x24, 0x24, 0x30, 0x26, 0x26, 0x26, 0x26, 0x33};
	uint8_t hill3[10] = {0x24, 0x30, 0x26, 0x34, 0x26, 0x26, 0x34, 0x26, 0x33};
	uint8_t hill4[10] = {0x30, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x33};
	uint8_t cloud0[4] = {0x24, 0x36, 0x37, 0x24};
	uint8_t cloud1[4] = {0x35, 0x25, 0x25, 0x38};
	uint8_t cloud2[4] = {0x35, 0x25, 0x25, 0x38};
	uint8_t cloud3[8] = {0x24, 0x36, 0x37, 0x36, 0x37, 0x36, 0x37, 0x24};
	uint8_t cloud4[8] = {0x35, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x38};
	uint8_t cloud6[6] = {0x24, 0x36, 0x37, 0x36, 0x37, 0x24};
	uint8_t cloud7[6] = {0x35, 0x25, 0x25, 0x25, 0x25, 0x38};
	uint8_t ground[4] = {0xb4, 0xb5, 0xb6, 0xb7};
	
	uint16_t i;
	
	width = 80;
	height = 30;
	
	mx = 40 * 128; // pixel position * 128
	my = 193 * 128; // pixel position * 128
	
	//for(i = 0; i < width * height; i++) {level[i] = (uint8_t)NULL;}
	
	/*level[0] = 0xb4;
	level[40] = 0xb5;
	level[80] = 0xb6;
	level[120] = 0xb7;*/
	
	for(i = 0; i < width; i++) {
		level[width * 26 + i] = ground[i % 2];
		level[width * 27 + i] = ground[(i % 2) + 2];
		level[width * 28 + i] = ground[i % 2];
		level[width * 29 + i] = ground[(i % 2) + 2];
	}
	
	blocks = width;
	
	for(i = 0; i < blocks; i += 2) {
		newBlock(i + 2, 13, i);
		newBlock(i + 2, 14, i + 1);
	}
	
	/*for(i = 0; i < 4; i++) {
		level[width * 5 + i + 17] = cloud0[i];
		level[width * 6 + i + 17] = cloud1[i];
	}
	for(i = 0; i < 6; i++) {
		level[width * 21 + i] = hill0[i];
		level[width * 23 + i + 32] = hill0[i];
		level[width * 21 + i + 96] = hill0[i];
		level[width * 23 + i + 128] = hill0[i];
		level[width * 21 + i + 192] = hill0[i];
		level[width * 23 + i + 224] = hill0[i];
		level[width * 21 + i + 288] = hill0[i];
		level[width * 23 + i + 320] = hill0[i];
		level[width * 21 + i + 384] = hill0[i];
		level[width * 23 + i + 416] = hill0[i];
	}
	for(i = 0; i < 7; i++) {
		level[width * 22 + i] = hill1[i];
		level[width * 24 + i + 32] = hill1[i];
		level[width * 22 + i + 96] = hill1[i];
		level[width * 24 + i + 128] = hill1[i];
		level[width * 22 + i + 192] = hill1[i];
		level[width * 24 + i + 224] = hill1[i];
		level[width * 22 + i + 288] = hill1[i];
		level[width * 24 + i + 320] = hill1[i];
		level[width * 22 + i + 384] = hill1[i];
		level[width * 24 + i + 416] = hill1[i];
	}
	for(i = 0; i < 8; i++) {
		level[width * 23 + i] = hill2[i];
		level[width * 25 + i + 32] = hill2[i];
		level[width * 23 + i + 96] = hill2[i];
		level[width * 25 + i + 128] = hill2[i];
		level[width * 23 + i + 192] = hill2[i];
		level[width * 25 + i + 224] = hill2[i];
		level[width * 23 + i + 288] = hill2[i];
		level[width * 25 + i + 320] = hill2[i];
		level[width * 23 + i + 384] = hill2[i];
		level[width * 25 + i + 416] = hill2[i];
		
		level[width * 24 + i + 23] = bush0[i];
		level[width * 25 + i + 23] = bush1[i];
		level[width * 24 + i + 119] = bush0[i];
		level[width * 25 + i + 119] = bush1[i];
		level[width * 24 + i + 215] = bush0[i];
		level[width * 25 + i + 215] = bush1[i];
		level[width * 24 + i + 311] = bush0[i];
		level[width * 25 + i + 311] = bush1[i];
		level[width * 24 + i + 407] = bush0[i];
		level[width * 25 + i + 407] = bush1[i];
	}
	for(i = 0; i < 9; i++) {
		level[width * 24 + i] = hill3[i];
		level[width * 24 + i + 96] = hill3[i];
		level[width * 24 + i + 192] = hill3[i];
		level[width * 24 + i + 288] = hill3[i];
		level[width * 24 + i + 384] = hill3[i];
	}
	for(i = 0; i < 10; i++) {
		level[width * 25 + i] = hill4[i];
		level[width * 25 + i + 96] = hill4[i];
		level[width * 25 + i + 192] = hill4[i];
		level[width * 25 + i + 288] = hill4[i];
		level[width * 25 + i + 384] = hill4[i];
	}*/
}

void newBlock(uint16_t bx, uint16_t by, uint16_t i) {
	xBlocks[i] = bx;
	yBlocks[i] = by;
}