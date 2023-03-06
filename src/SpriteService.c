#include "SpriteService.h"

#include "Sprites.h"

#define SPRITE_COUNT 30

#define VRAM_sprites 0x18000

struct Sprite
{
    unsigned long m_controlAddress;
    int16_t m_xPos;
    int16_t m_yPos;
    BOOL m_isActive;
};

static struct Sprite m_sprites[SPRITE_COUNT];
uint8_t m_inactiveSprites[SPRITE_COUNT];
uint8_t m_inactiveSpriteCount = 0;

void InactivateSprite(uint8_t spriteIndex)
{
    struct Sprite *pSprite = &m_sprites[spriteIndex];
    // z-depth: Sprite disabled
    vpoke(0x00, pSprite->m_controlAddress + 6);
    pSprite->m_isActive = FALSE;

    m_inactiveSprites[m_inactiveSpriteCount++] = spriteIndex;
}

uint8_t ActivateSprite()
{
    uint8_t spriteIndex = m_inactiveSprites[--m_inactiveSpriteCount];

    struct Sprite *pSprite = &m_sprites[spriteIndex];
    // z-depth: Sprite in front of layer 1
    vpoke(3 << 2, pSprite->m_controlAddress + 6);
    pSprite->m_isActive = TRUE;

    return spriteIndex;
}

void SetupSprite(uint8_t spriteIndex, uint8_t ramDataIndex, uint16_t x, uint16_t y)
{
    struct Sprite *pSprite = &m_sprites[spriteIndex];

    pSprite->m_controlAddress = 0x11FC00 + spriteIndex * 8;
    pSprite->m_xPos = x;
    pSprite->m_yPos = y;

    // Inside address bits 12:5.
    // Set the outside address to increment with each access.
    vpoke(((VRAM_sprites + SpriteDefinitions[ramDataIndex].SpriteDataOffset) >> 5) & 0xFF, pSprite->m_controlAddress);

    // 8 Bits-Per-Pixel mode (sprites can have 256 colors).
    // Inside address bits 16:13 (sprite pattern starts at VRAM_sprites).
    VERA.data0 = (1 << 7) | ((VRAM_sprites + SpriteDefinitions[ramDataIndex].SpriteDataOffset) >> 13);

    // x co-ordinate, bits 7:0
    VERA.data0 = pSprite->m_xPos & 0xFF;

    // x co-ordinate, bits 9:8
    VERA.data0 = pSprite->m_xPos >> 8;

    // y co-ordinate, bits 7:0
    VERA.data0 = 0 & 0xFF;

    // y co-ordinate, bits 9:8
    VERA.data0 = 0 >> 8;

    // z-depth: in front of layer 1
    VERA.data0 = (3 << 2);

    // 32 pixels for width and height
    {
        int veraWidth = SpriteDefinitions[ramDataIndex].Width >> 4;
        int veraHeight = SpriteDefinitions[ramDataIndex].Width >> 4;
        VERA.data0 = (veraHeight << 6) | (veraWidth << 4);
    }
}

void SetSprite(uint8_t spriteIndex, uint8_t ramDataIndex)
{
    struct Sprite *pSprite = &m_sprites[spriteIndex];

    // Inside address bits 12:5.
    // Set the outside address to increment with each access.
    vpoke(((VRAM_sprites + SpriteDefinitions[ramDataIndex].SpriteDataOffset) >> 5) & 0xFF, pSprite->m_controlAddress);

    // 8 Bits-Per-Pixel mode (sprites can have 256 colors).
    // Inside address bits 16:13 (sprite pattern starts at VRAM_sprites).
    VERA.data0 = (1 << 7) | ((VRAM_sprites + SpriteDefinitions[ramDataIndex].SpriteDataOffset) >> 13);
}

void SetSpritePos(uint8_t spriteIndex, int16_t x, int16_t y)
{
    struct Sprite *pSprite = &m_sprites[spriteIndex];

    // x co-ordinate, bits 7:0
    vpoke(pSprite->m_xPos & 0xFF, pSprite->m_controlAddress + 2);

    // x co-ordinate, bits 9:8
    VERA.data0 = pSprite->m_xPos >> 8;

    // y co-ordinate, bits 7:0
    VERA.data0 = pSprite->m_yPos & 0xFF;

    // y co-ordinate, bits 9:8
    VERA.data0 = pSprite->m_yPos >> 8;

    pSprite->m_xPos = x;
    pSprite->m_yPos = y;
}

void GetSpritePos(uint8_t spriteIndex, int16_t *pX, int16_t *pY)
{
    *pX = m_sprites[spriteIndex].m_xPos;
    *pY = m_sprites[spriteIndex].m_yPos;
}

void InitializeSprites()
{
    int i, j, spriteIndex;

    // Initialize the sprites' information.
    for (i = 0; i < SPRITE_COUNT; i++) {
        SetupSprite(i, 0, -64, 0);
        InactivateSprite(i);
    }

    // Copy the sprite data into the video RAM.
    VERA.address = VRAM_sprites;
    VERA.address_hi = VRAM_sprites >> 16 | VERA_INC_1;
    i = 0;
    for (spriteIndex = 0; spriteIndex < sizeof(SpriteDefinitions) / sizeof(struct SpriteDefinition); spriteIndex++ ) {
        SpriteDefinitions[spriteIndex].SpriteDataOffset = i; // Record location of the sprite data
        for (j = 0 ; j < SpriteDefinitions[spriteIndex].Width * SpriteDefinitions[spriteIndex].Height; j++) {
            VERA.data0 = SpriteData[i++];
        }
    }
}
