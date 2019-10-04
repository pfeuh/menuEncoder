#ifndef menuEncoder_h
#define menuEncoder_h

/*
 * file : menuEncoder.h
 * Copyright (c) pfeuh <ze.pfeuh@gmail>
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Arduino.h>

#define MENU_ENCODER_VERSION "1.0"

#define MENU_ENCODER_TRACK_DEBOUNCE_DURATION_MSEC 20
#define MENU_ENCODER_BUTTON_LONG_CLICK_MIN__DURATION_MSEC 200

#define MENU_ENCODER_CHAR_UP    'u'
#define MENU_ENCODER_CHAR_DOWN  'd'
#define MENU_ENCODER_CHAR_RIGHT 'r'
#define MENU_ENCODER_CHAR_LEFT  'l'

// let's manage an incrementation/decrementation
// and a short/long click at the same time
#define MENU_ENCODER_BUFFER_SIZE 2

enum MENU_ENCODER_trackDirection
{
    menuEncoder_trackDirectionUp=1,
    menuEncoder_trackDirectionDown=2
};

enum MENU_ENCODER_clickDuration
{
    menuEncoderDurationShort=1,
    menuEncoderDurationLong=2
};

class MENU_ENCODER
{
    public:
        MENU_ENCODER(int pin_track_a, int pin_track_b, int pin_button);
        void begin();
        void sequencer();
        void bind(void(*data_callback)(MENU_ENCODER_trackDirection direction), void(*button_callback)(MENU_ENCODER_clickDuration duration));
        byte available();
        byte read();

    private:
        int pinTrackA;
        int pinTrackB;
        int pinButton;
        byte oldA;
        byte oldButton;
        void (*dataHandler)(MENU_ENCODER_trackDirection direction);
        void (*buttonHandler)(MENU_ENCODER_clickDuration duration);
        bool trackDebouncing;
        bool buttonMeasuringDuration;
        unsigned long trackMilestone;
        unsigned long buttonMilestone;
        byte buffer[MENU_ENCODER_BUFFER_SIZE];
        byte bufferInPtr;
        byte bufferOutPtr;
    
        void startTrackDebounce();
        void startButtonMeasureDuration();
        void bufferAdd(byte c);

};

#endif
