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

/////////////////////////////////////////
// Platform-game abstraction interface
/////////////////////////////////////////


#ifndef _PLATFORM_INTERFACE_H_
#define _PLATFORM_INTERFACE_H_
#include <stdbool.h>
#include <stdint.h>
#include "data.h"

///////////////////////////////////////////////////////////////////////////////
// Game_InitOptions can be used to customize parts of initialization.
//
// Members:
// - keyboardFullscreenInstructions: Show keyboard controls for fullscreen
//      even if gamepad is attached.
// - hideQuitInstructions: Don't show quit instructions.
// - noAudio: Don't initialize audio.
///////////////////////////////////////////////////////////////////////////////

typedef struct {
    bool showInputToStartScreen;
    bool hideSystemInstructions;
    bool noAudio;
} Game_InitOptions;

////////////////////////////////////////////////////////////////////////
// Game_Input represents input from the platform layer into the game.
//
// Members:
// - velocity: Horizontal and vertical movement, normalized to
//      values between -1.0 and 1.0.
// - shoot: Whether a shoot input was received on this frame.
// - lastShoot:Whether a shoot input was received on the last frame.
// - keyboard: Whether this frame's input was from a keyboard. (Used
//      to modify input instructions for the player).
////////////////////////////////////////////////////////////////////////

typedef struct {
    float velocity[2];
    bool shoot;
    bool lastShoot;
    bool keyboard;
} Game_Input;

/////////////////////////////////////////////////////////////////////////
// Game lifecycle functions called by the platform layer and
// implemented by the game layer.
//
// - game_init(): Initialize game resources.
// - game_initAudio(): Initialize audio, if not done in game_init()
//      (e.g. for web after user interaction).
// - game_update(): Update game state based on time elapsed since
//      last frame.
// - game_draw(): Draw current frame.
// - game_resize(): Update rendering state to match the current window 
//      size.
// - game_close(): Release game resources.
/////////////////////////////////////////////////////////////////////////

bool game_init(Game_InitOptions* opts);
void game_initAudio(void);
void game_update(float elapsedTime); // In milliseconds
void game_draw(void);
void game_resize(int width, int height);
void game_close(void);

////////////////////////////////////////////////////////////////////////////
// Platform utilities called by the game layer and implemented by
// each platform layer.
//
// - platform_getInput(): Get current input state.
// - platform_loadSound(): Load a wave file into the audio system.
// - platform_playSound(): Output sound to an audio device.
// - platform_debugMessage(): Output a message intended for the developer 
//      while debugging.
// - platform_userMessage(): Output a message intended for the end
//      user.
// - platform_loadFile(): Load contents of a file into memory. 
//      Optionally, null-terminate if the data will be used as a 
//      string.
////////////////////////////////////////////////////////////////////////////

void platform_getInput(Game_Input* input);
int32_t platform_loadSound(const char* fileName);
void platform_playSound(int32_t id, bool loop);
void platform_debugMessage(const char* message);
void platform_userMessage(const char* message);
bool platform_loadFile(const char* fileName, Data_Buffer* buffer, bool nullTerminate);

#endif
