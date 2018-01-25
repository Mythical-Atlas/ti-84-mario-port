#include <stdint.h>
#include <stdlib.h>

#include "mario.h"

uint16_t x; // screen x

bool up; // whether or not the up arrow is being pressed
bool left; // whether or not the left arrow is being pressed
bool right; // whether or not the right arrow is being pressed

bool direction; // direction mario's facing (0 = right, 1 = left)

uint32_t mx; // mario's x position (1/256 of a pixel)
uint32_t my; // mario's y position (1/256 of a pixel)
int16_t dx; // delta x (x speed)
int16_t dy; // delta y (y speed)

int16_t odx; // old dx from jump start

bool marioCanJump;
bool marioJumping;
bool marioGround;
uint8_t marioFrame;
uint8_t marioState;
uint8_t marioTimer;
uint8_t marioAnimation;
uint16_t marioGravity = 0x50/2;

/*

mario states/animations

0 = idle
1 = running
2 = skidding
3 = jumping

*/

void drawMario() {
	
	gfx_SetColor(0x30);
	gfx_Rectangle(floor((mx / 128 - x +  8) / 16) * 16, floor((my / 128 +  0) / 16) * 16, 16, 16);
	gfx_Rectangle(floor((mx / 128 - x + 24) / 16) * 16, floor((my / 128 +  0) / 16) * 16, 16, 16);
	gfx_Rectangle(floor((mx / 128 - x +  8) / 16) * 16, floor((my / 128 + 16) / 16) * 16, 16, 16);
	gfx_Rectangle(floor((mx / 128 - x + 24) / 16) * 16, floor((my / 128 + 16) / 16) * 16, 16, 16);
	
	gfx_SetColor(0x1f);
	gfx_Rectangle(floor(mx / 128 - x) +  8, floor(my / 128), 16, 16);
	
	/*gfx_SetColor(0x16);
	gfx_Rectangle(floor((mx / 128 - x +  8) / 16) * 16, floor((my / 128 + 16) / 16) * 16, 16, 16);
	gfx_Rectangle(floor((mx / 128 - x + 24) / 16) * 16, floor((my / 128 + 16) / 16) * 16, 16, 16);
	gfx_Rectangle(floor((mx / 128 - x +  8) / 16) * 16, floor((my / 128 + 32) / 16) * 16, 16, 16);
	gfx_Rectangle(floor((mx / 128 - x + 24) / 16) * 16, floor((my / 128 + 32) / 16) * 16, 16, 16);*/
	
	//gfx_SetColor(0x12);
	//gfx_FillRectangle(floor(mx / 128 - x) +  8, floor(my / 128) + 16, 16, 1);
	
	
	if(marioState == 0 && marioAnimation != 0) { // idle
		marioAnimation = 0;
		marioTimer = 0;
		marioFrame = 0;
	}
	if(marioState == 1)                        { // running
		if(marioAnimation != 1) {
			marioAnimation = 1;
			marioTimer = 0;
			marioFrame = 0;
		}
		else {
			marioTimer++;
			if(abs(dx) > 0xe0) {
				if(marioTimer >= 2) {
					marioTimer = 0;
					marioFrame++;
					if(marioFrame == 3) {marioFrame = 0;} // running anim has 3 frames
				}
			}
			else {
				if(marioTimer >= 4) {
					marioTimer = 0;
					marioFrame++;
					if(marioFrame == 3) {marioFrame = 0;} // running anim has 3 frames
				}
			}
		}
	}
	if(marioState == 2 && marioAnimation != 2) { // skidding
		marioAnimation = 2;
		marioTimer = 0;
		marioFrame = 0;
	}
	if(marioState == 3 && marioAnimation != 3) { // jumping
		marioAnimation = 3;
		marioTimer = 0;
		marioFrame = 0;
	}
	
	if(marioAnimation == 0) { // idle
		if(!direction) {drawSprite(0x00);}
		else {drawSprite(0x01);}
	}
	if(marioAnimation == 1) { // running
		if(marioFrame == 0) {
			if(!direction) {drawSprite(0x02);}
			else {drawSprite(0x05);}
		}
		if(marioFrame == 1) {
			if(!direction) {drawSprite(0x03);}
			else {drawSprite(0x06);}
		}
		if(marioFrame == 2) {
			if(!direction) {drawSprite(0x04);}
			else {drawSprite(0x07);}
		}
	}
	if(marioAnimation == 2) { // skidding
		if(!direction) {drawSprite(0x08);}
		else {drawSprite(0x09);}
	}
	if(marioAnimation == 3) { // jumping
		if(!direction) {drawSprite(0x0a);}
		else {drawSprite(0x0b);}
	}
}

void updateMario() {
	checkMarioGround();
	
	if( left && !right) {
		if(marioGround) {
			if(dx > 0) {
				dx -= 0x34;
				marioState = 2;
			}
			else {
				dx -= 0x13;
				marioState = 1;
			}
			if(dx < -0x320) {dx = -0x320;}
			
			direction = 1;
		}
		else {
			if(dx <= 0) {
				     if(dx > -0x320) {dx -= 0x13;}
				else if(dx <= -0x320) {dx -= 0x1c;}
			}
			else if(dx > 0) {
				     if(dx >= 0x320) {dx -= 0x1c;}
				else if(dx < 0x320) {
					     if(odx >= 0x3a0) {dx -= 0x1a;}
					else if(odx < 0x3a0) {dx -= 0x13;}
				}
			}
			
			if(dx < -0x320) {dx = -0x320;}
			if(dx > 0x320) {dx = 0x320;}
		}
	}
	if(!left &&  right) {
		if(marioGround) {
			if(dx < 0) {
				dx += 0x34;
				marioState = 2;
			}
			else {
				dx += 0x13;
				marioState = 1;
			}
			if(dx > 0x320) {dx = 0x320;}
			
			direction = 0;
		}
		else {
			if(dx >= 0) {
				     if(dx < 0x320) {dx += 0x13;}
				else if(dx >= 0x320) {dx += 0x1c;}
			}
			else if(dx < 0) {
				     if(dx <= -0x320) {dx = 0x1c;}
				else if(dx > -0x320) {
					     if(odx <= -0x3a0) {dx += 0x1a;}
					else if(odx > -0x3a0) {dx += 0x13;}
				}
			}
			
			if(dx > 0x320) {dx = 0x320;}
			if(dx < -0x320) {dx = -0x320;}
		}
	}
	if(!left && !right
	||  left &&  right) {
		if(marioGround) {
			if(dx < -0x26) {
				if(marioState == 2) {dx += 0x34;}
				else {dx += 0x34;}
			}
			else if(dx > 0x26) {
				if(marioState == 2) {dx -= 0x34;}
				else {dx -= 0x34;}
			}
			else {
				dx = 0;
				marioState = 0;
			}
		}
	}
	if(marioGround) {
		if(marioState != 2) {
			if(dx == 0) {marioState = 0;}
			if(dx != 0) {marioState = 1;}
		}
		
		dy = 0;
		
		marioJumping = 0;
		if(!up) {marioCanJump = 1;}
		
		if(up && marioCanJump) {
			marioJumping = 1;
			marioCanJump = 0;
			marioGround = 0;
			marioState = 3;
			odx = dx;
			
			if(dx < 0x200) {
				dy = -0x800/3;
				marioGravity = 0x40/2;
			}
			if(dx >= 0x200 && dx < 0x500) {
				dy = -0x800/3;
				marioGravity = 0x3C/2;
			}
			if(dx >= 0x500) {
				dy = -0xa00/3;
				marioGravity = 0x50/2;
			}
		}
	}
	else {
		if(marioJumping) {
			if(!up) {
				if(marioGravity == 0x40/2) {marioGravity = 0xe0/2;}
				if(marioGravity == 0x3C/2) {marioGravity = 0xc0/2;}
				if(marioGravity == 0x50/2) {marioGravity = 0x120/2;}
			}
			
			if(dy > 0) {
				marioJumping = 0;
				
				if(marioGravity == 0x40/2) {marioGravity = 0xe0/2;}
				if(marioGravity == 0x3C/2) {marioGravity = 0xc0/2;}
				if(marioGravity == 0x50/2) {marioGravity = 0x120/2;}
			}
		}
		
		dy += marioGravity;
		
		marioState = 3;
	}
	
	mx += dx;
	
	collisionResponse();
	
	my += dy;
	
	collisionResponse();
	
	if(mx < (x + 24) * 128) {
		dx = 0;
		mx = (x + 24) * 128;
	}
	if(mx > (width * 8 - 24) * 128) {
		dx = 0;
		mx = (width * 8 - 24) * 128;
	}
	
	if(floor(mx / 128) - x > 80) {x += floor(mx / 128) - x - 80;}
	if(x > width * 8 - 288) {x = width * 8 - 288;}
}

void drawSprite(uint16_t index) {gfx_TransparentSprite((gfx_sprite_t*)sprites[index], floor(mx / 128) - x + 8, floor(my / 128));}

void checkMarioGround() {
	marioGround = 0;
	
	if(checkBlocks(-24, 16, 16, 1, -24, 16)) {marioGround = 1;}
	if(checkBlocks(-24, 16, 16, 1, - 8, 16)) {marioGround = 1;}
	if(checkBlocks(-24, 16, 16, 1, -24, 32)) {marioGround = 1;}
	if(checkBlocks(-24, 16, 16, 1, - 8, 32)) {marioGround = 1;}
}

void collisionResponse() {
	bool tileUL = 0;
	bool tileUR = 0;
	bool tileDL = 0;
	bool tileDR = 0;
	
	bool top = 0;
	bool side = 0; // left
	bool xSpan = 0;
	bool ySpan = 0;
	
	if(checkBlocks(-24, 0, 16, 16, -24,  0)) {tileUL = 1;}
	if(checkBlocks(-24, 0, 16, 16, - 8,  0)) {tileUR = 1;}
	if(checkBlocks(-24, 0, 16, 16, -24, 16)) {tileDL = 1;}
	if(checkBlocks(-24, 0, 16, 16, - 8, 16)) {tileDR = 1;}
	
	if( tileUL && !tileUR && !tileDL && !tileDR) {popOut(mx, my, 16, 16, mx / 128 +  0, my / 128 +  0, 16, 16);}
	if(!tileUL &&  tileUR && !tileDL && !tileDR) {popOut(mx, my, 16, 16, mx / 128 + 16, my / 128 +  0, 16, 16);}
	if(!tileUL && !tileUR &&  tileDL && !tileDR) {popOut(mx, my, 16, 16, mx / 128 +  0, my / 128 + 16, 16, 16);}
	if(!tileUL && !tileUR && !tileDL &&  tileDR) {popOut(mx, my, 16, 16, mx / 128 + 16, my / 128 + 16, 16, 16);}
	
	if( tileUL &&  tileUR && !tileDL && !tileDR) {popOut(mx, my, 16, 16, mx / 128 +  0, my / 128 +  0, 32, 16);}
	if(!tileUL && !tileUR &&  tileDL &&  tileDR) {popOut(mx, my, 16, 16, mx / 128 +  0, my / 128 + 16, 32, 16);}
	if( tileUL && !tileUR &&  tileDL && !tileDR) {popOut(mx, my, 16, 16, mx / 128 +  0, my / 128 +  0, 16, 32);}
	if(!tileUL &&  tileUR && !tileDL &&  tileDR) {popOut(mx, my, 16, 16, mx / 128 + 16, my / 128 +  0, 16, 32);}
	
	/*if(tileUL || tileUR) {top = 1;}
	if(tileUL || tileDL) {side = 1;} // left
	
	if(tileUL && tileUR || tileDL && tileDR) {xSpan = 1;}
	if(tileUL && tileDL || tileUR && tileDR) {ySpan = 1;}
	
	if(tileUL || tileUR || tileDL || tileDR) {
		if(!xSpan && !ySpan) {
			if( top &&  side) {popOut(mx, my, 16, 16, mx / 128 +  0, my / 128 +  0, 16, 16);}
			if( top && !side) {popOut(mx, my, 16, 16, mx / 128 + 16, my / 128 +  0, 16, 16);}
			if(!top &&  side) {popOut(mx, my, 16, 16, mx / 128 +  0, my / 128 + 16, 16, 16);}
			if(!top && !side) {popOut(mx, my, 16, 16, mx / 128 + 16, my / 128 + 16, 16, 16);}
		}
		if( xSpan && !ySpan) {
			if( top)          {popOut(mx, my, 16, 16, mx / 128 +  0, my / 128 +  0, 32, 16);}
			if(!top)          {popOut(mx, my, 16, 16, mx / 128 +  0, my / 128 + 16, 32, 16);}
		}
		if(!xSpan &&  ySpan) {
			if        ( side) {popOut(mx, my, 16, 16, mx / 128 +  0, my / 128 +  0, 16, 32);}
			if        (!side) {popOut(mx, my, 16, 16, mx / 128 + 16, my / 128 +  0, 16, 32);}
		}
		if( xSpan &&  ySpan) {
			                   popOut(mx, my, 16, 16, mx / 128 +  0, my / 128 +  0, 32, 32);
		}
	}*/
	
	/*if     (tileUL) {popOut(mx, my, 16 * 128, 16 * 128, floor((mx / 128 +  0) / 16) * 16 * 128, floor((my / 128 +  0) / 16) * 16 * 128, 16 * 128, 16 * 128);}
	else if(tileUR) {popOut(mx, my, 16 * 128, 16 * 128, floor((mx / 128 + 16) / 16) * 16 * 128, floor((my / 128 +  0) / 16) * 16 * 128, 16 * 128, 16 * 128);}
	else if(tileDL) {popOut(mx, my, 16 * 128, 16 * 128, floor((mx / 128 +  0) / 16) * 16 * 128, floor((my / 128 + 16) / 16) * 16 * 128, 16 * 128, 16 * 128);}
	else if(tileDR) {popOut(mx, my, 16 * 128, 16 * 128, floor((mx / 128 + 16) / 16) * 16 * 128, floor((my / 128 + 16) / 16) * 16 * 128, 16 * 128, 16 * 128);}*/
	
	/*if(tileUL || tileUR) {top = 1;}
	if(tileUL || tileDL) {side = 1;}
	
	if(tileUL || tileUR || tileDL || tileDR) {
		if( top &&  side) {popOut(mx, my, 16 * 128, 16 * 128, floor((mx / 128 +  0) / 16) * 16 * 128, floor((my / 128 +  0) / 16) * 16 * 128, 16 * 128, 16 * 128);}
		if( top && !side) {popOut(mx, my, 16 * 128, 16 * 128, floor((mx / 128 + 16) / 16) * 16 * 128, floor((my / 128 +  0) / 16) * 16 * 128, 16 * 128, 16 * 128);}
		if(!top &&  side) {popOut(mx, my, 16 * 128, 16 * 128, floor((mx / 128 +  0) / 16) * 16 * 128, floor((my / 128 + 16) / 16) * 16 * 128, 16 * 128, 16 * 128);}
		if(!top && !side) {popOut(mx, my, 16 * 128, 16 * 128, floor((mx / 128 + 16) / 16) * 16 * 128, floor((my / 128 + 16) / 16) * 16 * 128, 16 * 128, 16 * 128);}
	}*/
}

bool checkBlocks(int16_t x1, uint8_t y1, uint8_t w1, uint8_t h1, int16_t x2, uint8_t y2) { // x1/y1/w1/h1 = player, x2/y2 = block
	if(blocks[(uint16_t)floor((mx / 128 + x2) / 16)]
	         [(uint16_t)floor((my / 128 + y2) / 16)] == 1) {
		if(checkCollision(mx + x1 * 128, 
		                  my + y1 * 128, 
						  w1 * 128, 
						  h1 * 128, 
						  floor(mx / 128 + x2) * 128, 
						  floor(my / 128 + y2) * 128, 
						  16 * 128, 
						  16 * 128)) {return 1;}
	}
	return 0;
}

/*uint32_t testPop(uint32_t x1, uint16_t w1, uint32_t x2, uint16_t w2) {
	if(abs(x1 - (x2 - w1)) > abs(x1 - (x2 + w2))) {return x2 + w2 - 8 * 128;}
	else {return x2 - w1 + 8 * 128;}
}
uint32_t testPop2(uint32_t x1, uint16_t w1, uint32_t x2, uint16_t w2) {
	if(abs(x1 - (x2 - w1)) > abs(x1 - (x2 + w2))) {return x2 + w2;}
	else {return x2 - w1;}
}*/

void popOut(uint32_t x1, uint32_t y1, uint16_t w1, uint16_t h1, uint32_t x2, uint32_t y2, uint16_t w2, uint16_t h2) {
	uint32_t output1 = 0;
	uint32_t output2 = 0;
	
	bool tileU;
	bool tileD;
	bool tileL;
	bool tileR;
	
	tileU = !checkCollision(x1, floor(y2 / 16) * 16 * 128 - h2 * 128, w1, h1, x2, y2, w2, h2);
	tileD = !checkCollision(x1, floor(y2 / 16) * 16 * 128 + h1 * 128, w1, h1, x2, y2, w2, h2);
	tileL = !checkCollision(floor(x2 / 16) * 16 * 128 - w2 * 128 + 8 * 128, y1, w1, h1, x2, y2, w2, h2);
	tileR = !checkCollision(floor(x2 / 16) * 16 * 128 + w1 * 128 - 8 * 128, y1, w1, h1, x2, y2, w2, h2);
	
	if(abs(x1 - (floor(x2 / 16) * 16 * 128 - w1 * 128 + 8 * 128)) < 
	   abs(x1 - (floor(x2 / 16) * 16 * 128 + w2 * 128 - 8 * 128))) {output1 = floor(x2 / 16) * 16 * 128 - w2 * 128 + 8 * 128;}
	if(abs(x1 - (floor(x2 / 16) * 16 * 128 - w1 * 128 + 8 * 128)) > 
	   abs(x1 - (floor(x2 / 16) * 16 * 128 + w2 * 128 - 8 * 128))) {output1 = floor(x2 / 16) * 16 * 128 + w1 * 128 - 8 * 128;}
	
	if( tileL && !tileR) {output1 = floor(x2 / 16) * 16 * 128 - w2 * 128 + 8 * 128;}
	if(!tileL &&  tileR) {output1 = floor(x2 / 16) * 16 * 128 + w1 * 128 - 8 * 128;}
	
	if(abs(y1 - (floor(y2 / 16) * 16 * 128 - h1 * 128)) < 
	   abs(y1 - (floor(y2 / 16) * 16 * 128 + h2 * 128))) {output2 = floor(y2 / 16) * 16 * 128 - h2 * 128;}
	if(abs(y1 - (floor(y2 / 16) * 16 * 128 - h1 * 128)) > 
	   abs(y1 - (floor(y2 / 16) * 16 * 128 + h2 * 128))) {output2 = floor(y2 / 16) * 16 * 128 + h1 * 128;}
	   
	if( tileU && !tileD) {output2 = floor(y2 / 16) * 16 * 128 - h2 * 128;}
	if(!tileU &&  tileD) {output2 = floor(y2 / 16) * 16 * 128 + h1 * 128;}
	
	if     (!tileL && !tileR && (tileU || tileD)) {
		my = output2;
		dy = 0;
	}
	else if((tileL || tileR) && !tileU && !tileD) {
		mx = output1;
		dx = 0;
	}
	else if((tileL || tileR) && (tileU || tileD)) {
		if(abs(x1 - output1) < abs(y1 - output2)) {
			mx = output1;
			dx = 0;
		}
		else {
			my = output2;
			dy = 0;
		}
	}
}

bool checkCollision(uint32_t x1, uint32_t y1, uint16_t w1, uint16_t h1, uint32_t x2, uint32_t y2, uint16_t w2, uint16_t h2) {
	if    (x1 >= x2 && x1 <= x2 + w2 || x1 + w1 > x2 && x1 + w1 < x2 + w2) {
		if(y1 >= y2 && y1 <= y2 + h2 || y1 + h1 > y2 && y1 + h1 < y2 + h2) {return 1;}
	}
	
	return 0; // else
}