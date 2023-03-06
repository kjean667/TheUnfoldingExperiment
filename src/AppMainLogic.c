#include "Types.h"
#include "SpriteService.h"
#include "Sin.h"

uint8_t spriteIndex1;
uint8_t spriteIndex2;

uint16_t sinIndex1 = 0;
uint16_t sinIndex2 = 0;

void CreateMySprites()
{
    const uint8_t spriteDataIndex1 = 0;
    const uint8_t spriteDataIndex2 = 1;

    spriteIndex1 = ActivateSprite();
    SetSprite(spriteIndex1, spriteDataIndex1);
    SetSpritePos(spriteIndex1, -1, -1);

    spriteIndex2 = ActivateSprite();
    SetSprite(spriteIndex2, spriteDataIndex2);
    SetSpritePos(spriteIndex2, -1, -1);
}

void UpdateMySprites()
{
    int sprite1OffsetX = 50;
    int sprite1OffsetY = 50;
    int sprite2OffsetX = 200;
    int sprite2OffsetY = 100;
    SetSpritePos(spriteIndex1, sprite1OffsetX + Sin[sinIndex1], sprite1OffsetY + Sin[(sinIndex1 + CosOffset) % sizeof(Sin)]);
    SetSpritePos(spriteIndex2, sprite2OffsetX + Sin[sinIndex2] >> 1, sprite2OffsetY + Sin[(sinIndex2 + CosOffset) % sizeof(Sin)] >> 1);

    sinIndex1 = (sinIndex1 + 1) % sizeof(Sin);
    sinIndex2 = (sinIndex2 + 3) % sizeof(Sin);
}

void AppMainLogic_Initialize()
{
    CreateMySprites();
}

void AppMainLogic_ExecuteFrame()
{
    UpdateMySprites();
}
