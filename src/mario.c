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
	/*gfx_SetColor(0x30);
	gfx_FillRectangle(floor((mx / 128 +  8) / 16) * 16, floor((my / 128 +  0) / 16) * 16, 16, 16);
	gfx_FillRectangle(floor((mx / 128 + 24) / 16) * 16, floor((my / 128 +  0) / 16) * 16, 16, 16);
	gfx_FillRectangle(floor((mx / 128 +  8) / 16) * 16, floor((my / 128 + 16) / 16) * 16, 16, 16);
	gfx_FillRectangle(floor((mx / 128 + 24) / 16) * 16, floor((my / 128 + 16) / 16) * 16, 16, 16);*/
	
	if(marioState == 0 && marioAnimation != 0) { // idle
		marioAnimation = 0;
		marioTimer = 0;
		marioFrame = 0;
	}
	if(marioState == 1) { // running
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
		if(!direction) {gfx_TransparentSprite((gfx_sprite_t*)sprites[0x00], floor(mx / 128) - x + 8, floor(my / 128));}
		else {gfx_TransparentSprite((gfx_sprite_t*)sprites[0x01], floor(mx / 128) - x + 8, floor(my / 128));}
	}
	if(marioAnimation == 1) { // running
		if(marioFrame == 0) {
			if(!direction) {gfx_TransparentSprite((gfx_sprite_t*)sprites[0x02], floor(mx / 128) - x + 8, floor(my / 128));}
			else {gfx_TransparentSprite((gfx_sprite_t*)sprites[0x05], floor(mx / 128) - x + 8, floor(my / 128));}
		}
		if(marioFrame == 1) {
			if(!direction) {gfx_TransparentSprite((gfx_sprite_t*)sprites[0x03], floor(mx / 128) - x + 8, floor(my / 128));}
			else {gfx_TransparentSprite((gfx_sprite_t*)sprites[0x06], floor(mx / 128) - x + 8, floor(my / 128));}
		}
		if(marioFrame == 2) {
			if(!direction) {gfx_TransparentSprite((gfx_sprite_t*)sprites[0x04], floor(mx / 128) - x + 8, floor(my / 128));}
			else {gfx_TransparentSprite((gfx_sprite_t*)sprites[0x07], floor(mx / 128) - x + 8, floor(my / 128));}
		}
	}
	if(marioAnimation == 2) { // skidding
		if(!direction) {gfx_TransparentSprite((gfx_sprite_t*)sprites[0x08], floor(mx / 128) - x + 8, floor(my / 128));}
		else {gfx_TransparentSprite((gfx_sprite_t*)sprites[0x09], floor(mx / 128) - x + 8, floor(my / 128));}
	}
	if(marioAnimation == 3) { // jumping
		if(!direction) {gfx_TransparentSprite((gfx_sprite_t*)sprites[0x0a], floor(mx / 128) - x + 8, floor(my / 128));}
		else {gfx_TransparentSprite((gfx_sprite_t*)sprites[0x0b], floor(mx / 128) - x + 8, floor(my / 128));}
	}
}
void updateMario() {
	checkMarioGround();
	
	if(left && !right) {
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
	if(!left && right) {
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
	if(!left && !right || left && right) {
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
		dy = 0;
		
		marioJumping = 0;
		if(!up) {marioCanJump = 1;}
		
		if(up && marioCanJump) {
			marioJumping = 1;
			marioCanJump = 0;
			marioGround = 0;
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
	
	reactHorizontal();
	
	my += dy;
	
	reactVertical();
	
	if(mx < (x + 24) * 128) {
		dx = 0;
		mx = (x + 24) * 128;
	}
	if(mx > (width * 8 - 24) * 128) {
		dx = 0;
		mx = (width * 8 - 24) * 128;
	}
	
	//if(dx == 0 && marioState != 2) {marioState = 0;}
	
	if(floor(mx / 128) - x > 80) {x += floor(mx / 128) - x - 80;}
	if(x > width * 8 - 288) {x = width * 8 - 288;}
}
void checkMarioGround() {
	marioGround = 0;
	
	if(blocks[(uint16_t)floor((mx / 128 +  8) / 16)][(uint16_t)floor((my / 128 +  0) / 16)] == 1) {if(checkCollision(floor(mx / 128), 16, floor((mx / 128 +  8) / 16) * 16, 16) && checkCollision(floor(my / 128) + 16, 1, floor((my / 128 +  0) / 16) * 16, 16)) {marioGround = 1;}}
	if(blocks[(uint16_t)floor((mx / 128 + 24) / 16)][(uint16_t)floor((my / 128 +  0) / 16)] == 1) {if(checkCollision(floor(mx / 128), 16, floor((mx / 128 + 24) / 16) * 16, 16) && checkCollision(floor(my / 128) + 16, 1, floor((my / 128 +  0) / 16) * 16, 16)) {marioGround = 1;}}
	if(blocks[(uint16_t)floor((mx / 128 +  8) / 16)][(uint16_t)floor((my / 128 + 16) / 16)] == 1) {if(checkCollision(floor(mx / 128), 16, floor((mx / 128 +  8) / 16) * 16, 16) && checkCollision(floor(my / 128) + 16, 1, floor((my / 128 + 16) / 16) * 16, 16)) {marioGround = 1;}}
	if(blocks[(uint16_t)floor((mx / 128 + 24) / 16)][(uint16_t)floor((my / 128 + 16) / 16)] == 1) {if(checkCollision(floor(mx / 128), 16, floor((mx / 128 + 24) / 16) * 16, 16) && checkCollision(floor(my / 128) + 16, 1, floor((my / 128 + 16) / 16) * 16, 16)) {marioGround = 1;}}
}

void reactHorizontal() {
	bool tileL = 0;
	bool tileR = 0;
	
	if(blocks[(uint16_t)floor((mx / 128 +  8) / 16)][(uint16_t)floor((my / 128 +  0) / 16)] == 1) {if(checkCollision(floor(mx / 128), 16, floor((mx / 128 +  8) / 16) * 16, 16) && checkCollision(floor(my / 128), 16, floor((my / 128 +  0) / 16) * 16, 16)) {tileL = 1;}}
	if(blocks[(uint16_t)floor((mx / 128 + 24) / 16)][(uint16_t)floor((my / 128 +  0) / 16)] == 1) {if(checkCollision(floor(mx / 128), 16, floor((mx / 128 + 24) / 16) * 16, 16) && checkCollision(floor(my / 128), 16, floor((my / 128 +  0) / 16) * 16, 16)) {tileR = 1;}}
	if(blocks[(uint16_t)floor((mx / 128 +  8) / 16)][(uint16_t)floor((my / 128 + 16) / 16)] == 1) {if(checkCollision(floor(mx / 128), 16, floor((mx / 128 +  8) / 16) * 16, 16) && checkCollision(floor(my / 128), 16, floor((my / 128 + 16) / 16) * 16, 16)) {tileL = 1;}}
	if(blocks[(uint16_t)floor((mx / 128 + 24) / 16)][(uint16_t)floor((my / 128 + 16) / 16)] == 1) {if(checkCollision(floor(mx / 128), 16, floor((mx / 128 + 24) / 16) * 16, 16) && checkCollision(floor(my / 128), 16, floor((my / 128 + 16) / 16) * 16, 16)) {tileR = 1;}}
	
	              if(tileL == 1) {mx += popOut(mx, 16 * 128, floor((mx / 128 +  0) / 16) * 16 * 128, 16 * 128);}
	if(tileL == 0 && tileR == 1) {mx += popOut(mx, 16 * 128, floor((mx / 128 + 16) / 16) * 16 * 128, 16 * 128);}
}

void reactVertical() {
	bool tileU = 0;
	bool tileD = 0;
	
	if(blocks[(uint16_t)floor((mx / 128 +  8) / 16)][(uint16_t)floor((my / 128 +  0) / 16)] == 1) {if(checkCollision(floor(mx / 128), 16, floor((mx / 128 +  8) / 16) * 16, 16) && checkCollision(floor(my / 128), 16, floor((my / 128 +  0) / 16) * 16, 16)) {tileU = 1;}}
	if(blocks[(uint16_t)floor((mx / 128 + 24) / 16)][(uint16_t)floor((my / 128 +  0) / 16)] == 1) {if(checkCollision(floor(mx / 128), 16, floor((mx / 128 + 24) / 16) * 16, 16) && checkCollision(floor(my / 128), 16, floor((my / 128 +  0) / 16) * 16, 16)) {tileU = 1;}}
	if(blocks[(uint16_t)floor((mx / 128 +  8) / 16)][(uint16_t)floor((my / 128 + 16) / 16)] == 1) {if(checkCollision(floor(mx / 128), 16, floor((mx / 128 +  8) / 16) * 16, 16) && checkCollision(floor(my / 128), 16, floor((my / 128 + 16) / 16) * 16, 16)) {tileD = 1;}}
	if(blocks[(uint16_t)floor((mx / 128 + 24) / 16)][(uint16_t)floor((my / 128 + 16) / 16)] == 1) {if(checkCollision(floor(mx / 128), 16, floor((mx / 128 + 24) / 16) * 16, 16) && checkCollision(floor(my / 128), 16, floor((my / 128 + 16) / 16) * 16, 16)) {tileD = 1;}}
	
	              if(tileU == 1) {my += popOut(my, 16 * 128, floor((my / 128 +  0) / 16) * 16 * 128, 16 * 128);}
	if(tileD == 1 && tileU == 0) {my += popOut(my, 16 * 128, floor((my / 128 + 16) / 16) * 16 * 128, 16 * 128);}
}

int16_t popOut(uint32_t x1, uint16_t w1, uint32_t x2, uint16_t w2) {return x2 - (x1 + w1);}

bool checkCollision(uint16_t xy1, uint16_t wh1, uint16_t xy2, uint16_t wh2) {
	if(xy1 >= xy2 && xy1 <= xy2 + wh2 || xy1 + wh1 >= xy2 && xy1 + wh1 <= xy2 + wh2) {return 1;}
	return 0;
}