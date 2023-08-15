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

#ifdef SPACE_SHOOTER_OPENGLES
#include <GLES3/gl3.h>
#define VS_PREAMBLE "#version 300 es\n"
#define FS_PREAMBLE "#version 300 es\nprecision highp float;\n"
#else
#include "../../lib/simple-opengl-loader.h"
#define VS_PREAMBLE "#version 330\n"
#define FS_PREAMBLE "#version 330\n"
#endif

#include <malloc.h>
#include "renderer.h"
#include "../shared/data.h"
#include "../shared/platform-interface.h"
#include "../shared/debug.h"

static struct {
    int32_t width;
    int32_t height;
} window;

static struct {
    int32_t worldWidth;
    int32_t worldHeight;
    int32_t displayOffsetX;
    int32_t displayOffsetY;
    int32_t displayWidth;
    int32_t displayHeight;
} game;

static struct {
    GLuint panelIndex;
    GLuint pixelOffset;
    GLuint scale;
    GLuint whiteOut;
    GLuint alpha;
} buffers;

static struct {
    GLuint pixelClipSize;
    GLuint spriteSheet;
    GLuint panelPixelSize;
    GLuint spriteSheetDimensions;
} uniforms;

bool renderer_init(int worldWidth, int worldHeight) {
    game.worldWidth = worldWidth;
    game.worldHeight = worldHeight;

    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);

    Data_Buffer vsSource = { 0 };
    Data_Buffer fsSource = { 0 };

    if (!platform_loadFile("assets/shaders/vs.glsl", &vsSource, true)) {
        DEBUG_LOG("renderer_init: Unable to load vertex shader.");
        return false;
    }

    const char* vertexShaderParts[2] = {
        VS_PREAMBLE,
        (const char*) vsSource.data
    };

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 2, vertexShaderParts, NULL);
    glCompileShader(vertexShader);

    data_freeBuffer(&vsSource);

    if (!platform_loadFile("assets/shaders/fs.glsl", &fsSource, true)) {
        DEBUG_LOG("renderer_init: Unable to load fragment shader.");
        return false;
    }

    const char* fragmentShaderParts[2] = {
        FS_PREAMBLE,
        (const char*) fsSource.data
    };
        
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 2, fragmentShaderParts, NULL);
    glCompileShader(fragmentShader);

    data_freeBuffer(&fsSource);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);


    GLint result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    if (result != GL_TRUE) {
#ifdef SPACE_SHOOTER_DEBUG
        DEBUG_LOG("Program failed to link!");
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
        char buffer[1024];
        if (result != GL_TRUE) {
            DEBUG_LOG("Vertex shader failed to compile!");
            glGetShaderInfoLog(vertexShader, 1024, NULL, buffer);
            DEBUG_LOG(buffer);
        }
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
        if (result != GL_TRUE) {
            DEBUG_LOG("Fragment shader failed to compile!");
            glGetShaderInfoLog(fragmentShader, 1024, NULL, buffer);
            DEBUG_LOG(buffer);
        }
#endif

        return false;
    }

    glUseProgram(program);

    uniforms.panelPixelSize = glGetUniformLocation(program, "panelPixelSize");
    uniforms.spriteSheetDimensions = glGetUniformLocation(program, "spriteSheetDimensions");
    GLuint pixelClipSizeUniform = glGetUniformLocation(program, "pixelClipSize");
    GLuint spriteSheetUniform = glGetUniformLocation(program, "spriteSheet");

    glUniform2f(pixelClipSizeUniform, 2.0f / worldWidth, 2.0f / worldHeight);
    glUniform1i(spriteSheetUniform, 0);

    float positions[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f,  1.0f
    };

    GLuint spriteArray;
    glGenVertexArrays(1, &spriteArray);
    glBindVertexArray(spriteArray);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // Instanced attributes

    glGenBuffers(1, &buffers.pixelOffset);
    glGenBuffers(1, &buffers.pixelOffset);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.pixelOffset);
    glBufferData(GL_ARRAY_BUFFER, RENDERER_DRAWLIST_MAX * 2 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(1, 1);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &buffers.panelIndex);
    glGenBuffers(1, &buffers.panelIndex);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.panelIndex);
    glBufferData(GL_ARRAY_BUFFER, RENDERER_DRAWLIST_MAX * 2 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(2, 1);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &buffers.scale);
    glGenBuffers(1, &buffers.scale);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.scale);
    glBufferData(GL_ARRAY_BUFFER, RENDERER_DRAWLIST_MAX * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(3, 1);
    glEnableVertexAttribArray(3);

    glGenBuffers(1, &buffers.alpha);
    glGenBuffers(1, &buffers.alpha);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.alpha);
    glBufferData(GL_ARRAY_BUFFER, RENDERER_DRAWLIST_MAX * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(4, 1);
    glEnableVertexAttribArray(4);

    glGenBuffers(1, &buffers.whiteOut);
    glGenBuffers(1, &buffers.whiteOut);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.whiteOut);
    glBufferData(GL_ARRAY_BUFFER, RENDERER_DRAWLIST_MAX * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(5, 1);
    glEnableVertexAttribArray(5);

    return renderer_validate();
}

uint32_t renderer_createTexture(uint8_t* data, int32_t width, int32_t height) {
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return texture;
}

bool renderer_validate(void) {
    return glGetError() != GL_OUT_OF_MEMORY;
}


void renderer_resize(int32_t width, int32_t height) {
    window.width = width;
    window.height = height;

    float aspect = (float) game.worldWidth / game.worldHeight;
    game.displayWidth = width;
    game.displayHeight = (int32_t) (width / aspect);

    if (game.displayHeight > height) {
        game.displayHeight = height;
        game.displayWidth = (int32_t) (aspect * game.displayHeight);
    }

    game.displayOffsetX = (width - game.displayWidth) / 2;
    game.displayOffsetY = (height - game.displayHeight) / 2;

    glViewport(game.displayOffsetX, game.displayOffsetY, game.displayWidth, game.displayHeight);
}

void renderer_beforeFrame(void) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glScissor(0, 0, window.width, window.height);
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glScissor(game.displayOffsetX, game.displayOffsetY, game.displayWidth, game.displayHeight);
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_draw(Renderer_List* list) {
    if (list->count == 0) {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, list->sprite->texture);
    glUniform2fv(uniforms.panelPixelSize, 1, list->sprite->panelDims);
    glUniform2fv(uniforms.spriteSheetDimensions, 1, list->sprite->sheetDims);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.pixelOffset);
    glBufferSubData(GL_ARRAY_BUFFER, 0, list->count * 2 * sizeof(float), list->position);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.panelIndex);
    glBufferSubData(GL_ARRAY_BUFFER, 0, list->count * 2 * sizeof(float), list->currentSpritePanel);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.scale);
    glBufferSubData(GL_ARRAY_BUFFER, 0, list->count * sizeof(float), list->scale);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.alpha);
    glBufferSubData(GL_ARRAY_BUFFER, 0, list->count * sizeof(float), list->alpha);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.whiteOut);
    glBufferSubData(GL_ARRAY_BUFFER, 0, list->count * sizeof(float), list->whiteOut);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, list->count);
}
