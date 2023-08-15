////////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
// 
// Copyright (c) 2021 Tarek Sherif
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
// Entities represent game objects as used by the game layer,
// including the player, enemies, bullets, explosions, text, etc.
//
//////////////////////////////////////////////////////////////////////

#ifndef _GAME_ENTITIES_H_
#define _GAME_ENTITIES_H_
#include "renderer.h"

///////////////////////////////////////////////////////////////////////
// The `Entities_List` struct contains a mixin of `Renderer_List` 
// (required for access to `position`, `scale`, `sprite` data
// manipulated in the simulation). It also contains the following 
// game-specific data:
// 
// Members:
// - velocity: current velocity of the entity
// - currentSpritePanel: current sprite panel to draw
// - currentAnimation: index of the current animation in the sprite 
//      animations array.
// - health: enemy hit point
// - dead: whether the entity should be removed from the list 
//      (performed by entities_filterDead() at the end of a frame)
///////////////////////////////////////////////////////////////////////

#define ENTITIES_LIST_BODY {\
    RENDERER_LIST_MIXIN(renderList);\
    float velocity[RENDERER_DRAWLIST_MAX * 2];\
    int32_t currentAnimation[RENDERER_DRAWLIST_MAX];\
    int32_t animationTick[RENDERER_DRAWLIST_MAX];\
    int32_t health[RENDERER_DRAWLIST_MAX];\
    bool dead[RENDERER_DRAWLIST_MAX];\
}

typedef struct ENTITIES_LIST_BODY Entities_List;

#define ENTITIES_LIST_MIXIN(name) union { struct ENTITIES_LIST_BODY; Entities_List name; }

typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    int32_t currentAnimation;
    int32_t health;
    float scale;
    float transparency;
    float whiteOut;
} Entities_InitOptions;

typedef struct {
    float x;
    float y;
    float scale;
    float transparency;
    bool reset;
} Entities_FromTextOptions;

///////////////////////////////////////////////////////////////////////
// Entity manipulation functions.
//
// - entities_spawn(): Spawn a new entity at the end of the list.
// - entities_init(): Initialize the entity at the given index in
//      the list.
// - entities_filterDead(): Remove dead entities from the list (called
//      at the end of a frame).
// - entities_setAnimation(): Set the animation sequence for the 
//      given entity.
// - entities_updateAnimationPanel(): Set the animation panel for 
//      the given entity based on the current animation tick.
// - entities_updateAnimations(): Increment ticks and update
//      animation panels for all entities in the list.
// - entities_fromText(): Create and entity representation of the 
//      provided string.
///////////////////////////////////////////////////////////////////////

void entities_spawn(Entities_List* list, Entities_InitOptions* opts);
void entities_init(Entities_List* list, int32_t i, Entities_InitOptions* opts);
void entities_filterDead(Entities_List* list);
void entities_setAnimation(Entities_List* list, int32_t i, int32_t animation);
void entities_updateAnimationPanel(Entities_List* list, int32_t i);
void entities_updateAnimations(Entities_List* list);
void entities_fromText(Entities_List* list, const char* text, Entities_FromTextOptions* opts);

#endif
