/*
 * file : menuEncoder.cpp
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


#include "menuEncoder.h"

/*******************/
/* Private methods */
/*******************/

void MENU_ENCODER::startTrackDebounce()
{
    trackMilestone = millis() + MENU_ENCODER_TRACK_DEBOUNCE_DURATION_MSEC;
    trackDebouncing = true;
}

void MENU_ENCODER::startButtonMeasureDuration()
{
    buttonMilestone = millis() + MENU_ENCODER_BUTTON_LONG_CLICK_MIN__DURATION_MSEC;
    buttonMeasuringDuration = true;
}

void MENU_ENCODER::bufferAdd(byte c)
{
    buffer[bufferInPtr++] = c;
    bufferInPtr %= MENU_ENCODER_BUFFER_SIZE;
}

/******************/
/* Public methods */
/******************/

MENU_ENCODER::MENU_ENCODER(int pin_track_a, int pin_track_b, int pin_button)
{
    pinTrackA = pin_track_a;
    pinTrackB = pin_track_b;
    pinButton = pin_button;
}

void MENU_ENCODER::begin()
{
    pinMode(pinTrackA, INPUT_PULLUP);
    pinMode(pinTrackB, INPUT_PULLUP);
    pinMode(pinButton, INPUT_PULLUP);
    oldA = digitalRead(pinTrackA);
    oldButton = digitalRead(pinButton);
    trackDebouncing = false;
    buttonMeasuringDuration = false;
    bufferInPtr = 0;
    bufferOutPtr = 0;
}

void MENU_ENCODER::bind(void(*data_callback)(MENU_ENCODER_trackDirection direction), void(*button_callback)(enum MENU_ENCODER_clickDuration duration))
{
    dataHandler   = data_callback;
    buttonHandler   = button_callback;
}

void MENU_ENCODER::sequencer()
{
    if(trackDebouncing)
        if(millis() >= trackMilestone)
            trackDebouncing = false;

    if(!trackDebouncing)
    {
        byte newA = digitalRead(pinTrackA);
        
        // rising edge detected on track A?
        if ((oldA == LOW) && (newA == HIGH))
        {
            if (digitalRead(pinTrackB) == LOW)
            {
                // low level on track B -> incrementation
                if(dataHandler)
                    dataHandler(menuEncoder_trackDirectionUp);
                else
                    bufferAdd(MENU_ENCODER_CHAR_UP);
            }
            else
            {
                // high level on track B -> decrementation
                if(dataHandler)
                    dataHandler(menuEncoder_trackDirectionDown);
                else
                    bufferAdd(MENU_ENCODER_CHAR_DOWN);
            }
            startTrackDebounce();
        }
        oldA = newA;
    }
    
    if(buttonMeasuringDuration)
        if(millis() >= buttonMilestone)
        {
            buttonMeasuringDuration = false;
            
            if (digitalRead(pinButton) == LOW)
            {
                // button is still pushed -> long duration
                if(buttonHandler)
                    buttonHandler(menuEncoderDurationLong);
                else
                    bufferAdd(MENU_ENCODER_CHAR_LEFT);
            }
            else
            {
                // button is already released -> short duration
                if(buttonHandler)
                    buttonHandler(menuEncoderDurationShort);
                else
                    bufferAdd(MENU_ENCODER_CHAR_RIGHT);
            }
        }

    if(!buttonMeasuringDuration)
    {
        byte newButton = digitalRead(pinButton);
        
        if ((oldButton == HIGH) && (newButton == LOW))
            startButtonMeasureDuration();
        oldButton = newButton;
    }
}

byte MENU_ENCODER::available()
{
    return bufferInPtr - bufferOutPtr;
}

byte MENU_ENCODER::read()
{
    byte c = buffer[bufferOutPtr++];
    bufferOutPtr %= MENU_ENCODER_BUFFER_SIZE;
    return c;
}

