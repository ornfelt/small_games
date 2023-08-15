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

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

//////////
// Time
//////////

#define SPACE_SHOOTER_MILLISECOND 1000000ll
#define SPACE_SHOOTER_SECOND (SPACE_SHOOTER_MILLISECOND * 1000ll)
#define SPACE_SHOOTER_MIN_FRAME_TIME (3 * SPACE_SHOOTER_MILLISECOND)
#define SPACE_SHOOTER_GAMEPAD_POLL_TIME SPACE_SHOOTER_SECOND

////////////
// Window
////////////

#define SPACE_SHOOTER_DEFAULT_WINDOWED_WIDTH 1200
#define SPACE_SHOOTER_DEFAULT_WINDOWED_HEIGHT 600
#define SPACE_SHOOTER_MSAA_SAMPLES 4

///////////
// Audio
///////////

#define SPACE_SHOOTER_AUDIO_SAMPLE_RATE 44100
#define SPACE_SHOOTER_AUDIO_CHANNELS 2
#define SPACE_SHOOTER_AUDIO_BPS 16
#define SPACE_SHOOTER_AUDIO_MIXER_CHANNELS 32
#define SPACE_SHOOTER_AUDIO_MAX_SOUNDS 16

/////////////
// Gamepad
/////////////

// From XInput: https://docs.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput#dead-zone
#define SPACE_SHOOTER_GAMEPAD_STICK_DEADZONE  7849
#define SPACE_SHOOTER_GAMEPAD_STICK_DEADZONE_FLOAT  (SPACE_SHOOTER_GAMEPAD_STICK_DEADZONE / 32767.0f)

#endif
