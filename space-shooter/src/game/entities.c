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

#include "entities.h"

void entities_updateAnimationPanel(Entities_List* list, int32_t i) {
    float* panel = list->sprite->animations[list->currentAnimation[i]].frames[list->animationTick[i]];
    float* currentSpritePanel = list->currentSpritePanel + i * 2;

    currentSpritePanel[0] = panel[0];
    currentSpritePanel[1] = panel[1];
}

void entities_setAnimation(Entities_List* list, int32_t i, int32_t animation) {
    if (list->currentAnimation[i] == animation) {
        return;
    }

    list->currentAnimation[i] = animation;
    list->animationTick[i] = 0;
    entities_updateAnimationPanel(list, i);
}

void entities_updateAnimations(Entities_List* list) {
    for (int32_t i = 0; i < list->count; ++i) {
        int32_t numFrames = list->sprite->animations[list->currentAnimation[i]].numFrames;
        ++list->animationTick[i];
        if (list->animationTick[i] == numFrames) {
            Sprites_AnimationEndBehavior endBehavior = list->sprite->animations[list->currentAnimation[i]].endBehavior;

            if (endBehavior == SPRITES_ANIMATION_END_KILL) {
                list->dead[i] = true;
                continue;
            } else {
                list->animationTick[i] = 0; 
            }
        }

        entities_updateAnimationPanel(list, i);
    }
}

void entities_spawn(Entities_List* list, Entities_InitOptions* opts) {
    if (list->count == RENDERER_DRAWLIST_MAX) {
        return;
    }

    int32_t i = list->count;
    float* position = list->position + i * 2;
    float* velocity = list->velocity + i * 2;

    position[0] = opts->x; 
    position[1] = opts->y;
    velocity[0] = opts->vx;
    velocity[1] = opts->vy;
    list->currentAnimation[i] = opts->currentAnimation;
    list->animationTick[i]    = 0;
    list->scale[i]            = opts->scale > 0.0f ? opts->scale : 1.0f;
    list->alpha[i]            = 1.0f - opts->transparency;
    list->health[i]           = opts->health;
    list->whiteOut[i]         = opts->whiteOut;
    list->dead[i]             = false;

    if (list->sprite->animations) {
        entities_updateAnimationPanel(list, i);
    }

    ++list->count;
}

void entities_filterDead(Entities_List* list) {

    for (int32_t i = list->count - 1; i >= 0; --i) {
        if (list->dead[i]) {
            int32_t last = list->count - 1;
            float* position = list->position + i * 2;
            float* velocity = list->velocity + i * 2;
            float* lastPosition = list->position + last * 2;
            float* lastVelocity = list->velocity + last * 2;

            position[0] = lastPosition[0]; 
            position[1] = lastPosition[1];
            velocity[0] = lastVelocity[0];
            velocity[1] = lastVelocity[1];
            list->currentAnimation[i] = list->currentAnimation[last];
            list->animationTick[i]    = list->animationTick[last];
            list->scale[i]            = list->scale[last];
            list->alpha[i]            = list->alpha[last];
            list->health[i]           = list->health[last];
            list->whiteOut[i]         = list->whiteOut[last];
            list->dead[i]             = list->dead[last];

            --list->count;
        }
    }
}

void entities_fromText(Entities_List* list, const char* text, Entities_FromTextOptions* opts) {
    int32_t i = 0;

    if (opts->reset) {
        list->count = 0;
    }

    float scale = opts->scale > 0.0f ? opts->scale : 1.0f;

    while (text[i] && list->count < RENDERER_DRAWLIST_MAX) {
        int32_t animationIndex = sprites_charToAnimationIndex(text[i]);

        if (animationIndex < 0) {
            ++i;
            continue;
        }

        entities_spawn(list, &(Entities_InitOptions) { 
            .x = opts->x + i * list->sprite->panelDims[0] * scale * SPRITES_TEXT_SPACING_SCALE,
            .y = opts->y,
            .scale = scale,
            .transparency = opts->transparency,
            .currentAnimation = animationIndex
        });

        ++i;
    }

}
