#ifndef SPRITE_SERVICE_H
#define SPRITE_SERVICE_H

#include "Types.h"

extern void InactivateSprite(uint8_t spriteIndex);
extern uint8_t ActivateSprite();
extern void SetupSprite(uint8_t spriteIndex, uint8_t ramDataIndex, uint16_t x, uint16_t y);
extern void SetSprite(uint8_t spriteIndex, uint8_t ramDataIndex);
extern void SetSpritePos(uint8_t spriteIndex, int16_t x, int16_t y);
extern void GetSpritePos(uint8_t spriteIndex, int16_t *pX, int16_t *pY);
extern void InitializeSprites();

#endif
