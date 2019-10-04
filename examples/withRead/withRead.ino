
#include "menuEncoder.h"

MENU_ENCODER encoder = MENU_ENCODER(4, 3, 2);

// let's define a set of values
//              Freezer, Coffee, Swimming pool Hammam Aquarium
int values[] = {-1,      75,     30,           40,     20}; 
int mins[]   = {-20,     60,     18,           40,     16}; 
int maxs[]   = {-1,      90,     38,           80,     23};
#define NB_VALUES (sizeof(values) / sizeof(int))
byte valuePointer = NB_VALUES - 1;

void printlnValue()
{
    Serial.println(values[valuePointer]);
}

void encoderDataHandler(MENU_ENCODER_trackDirection direction)
{
    if(direction == menuEncoder_trackDirectionUp)
    {
        if(values[valuePointer] < maxs[valuePointer])
            values[valuePointer]++;
        printlnValue();
    }    
    else
    {
        if(values[valuePointer] > mins[valuePointer])
            values[valuePointer]--;
        printlnValue();
    }
}

void encoderButtonHandler(MENU_ENCODER_clickDuration direction)
{
    if(direction == menuEncoderDurationShort)
        valuePointer = (valuePointer + 1) % NB_VALUES;
    else if(direction == menuEncoderDurationLong)
    {
        if(valuePointer) 
            valuePointer = (valuePointer - 1);
        else
            valuePointer = NB_VALUES - 1;
    }
        
    for(byte x = 0; x < NB_VALUES; x++)
    {
        if(x == valuePointer)
            Serial.write('<');
        else
            Serial.write(' ');
        switch(x)
        {
            case 0:
                Serial.print(F("Freezer"));
                break;
            case 1:
                Serial.print(F("Coffee"));
                break;
            case 2:
                Serial.print(F("Swimming pool"));
                break;
            case 3:
                Serial.print(F("Hammam"));
                break;
            case 4:
                Serial.print(F("Aquarium"));
                break;
            default:
                break;
        }
        if(x == valuePointer)
            Serial.write('>');
        else
            Serial.write(' ');
    }
    printlnValue();
}

void setup()
{
    encoder.begin();

    pinMode(LED_BUILTIN, OUTPUT);
    
    Serial.begin(9600);
    Serial.println(F("Incremental encoder v. " MENU_ENCODER_VERSION));
    Serial.println(F(__DATE__ " " __TIME__));
    Serial.println(F("Let's use avaiblable/read way"));
    Serial.println(F("Press button short/long to switch to another setpoint."));
    Serial.println(F("Clockwise / counter clockwise to adjust setpoint."));

    // let's start with Freezer temperature
    encoderButtonHandler(menuEncoderDurationShort);
}

void loop()
{
    encoder.sequencer();
    if(encoder.available())
        switch(encoder.read())
        {
            case MENU_ENCODER_CHAR_UP:
                encoderDataHandler(menuEncoder_trackDirectionUp);
                break;
            case MENU_ENCODER_CHAR_DOWN:
                encoderDataHandler(menuEncoder_trackDirectionDown);
                break;
            case MENU_ENCODER_CHAR_RIGHT:
                encoderButtonHandler(menuEncoderDurationShort);
                break;
            case MENU_ENCODER_CHAR_LEFT:
                encoderButtonHandler(menuEncoderDurationLong);
                break;
            default:
                break;
        }
    
    // status led sequencer
    if(millis() & 0x200)
        digitalWrite(LED_BUILTIN, 0);
    else
        digitalWrite(LED_BUILTIN, 1);
}
