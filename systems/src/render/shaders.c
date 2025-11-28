#include "shaders.h"

static char SPRITE_SHADER_VERT[] = {
#embed "sprite.vert"
};

static char SPRITE_SHADER_FRAG[] = {
#embed "sprite.frag"
};

char* getSpriteVertShader()
{
    return SPRITE_SHADER_VERT;
};
char* getSpriteFragShader()
{
    return SPRITE_SHADER_FRAG;
};
