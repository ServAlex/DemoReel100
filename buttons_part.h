#ifndef buttons_part_h
#define buttons_part_h

#include <Wire.h>

#define BUTTON_1 0
#define BUTTON_2 35

int repeatStart = 1000;
int repeatPeriod = 10;

// button 1
long button1_pressStartedTime = 0;
long button1_lastRepeatedTime = 0;
bool button1_isPressed = false;
// button 2
long button2_pressStartedTime = 0;
long button2_lastRepeatedTime = 0;
bool button2_isPressed = false;
// bothButtons
long bothButtons_pressStartedTime = 0;
long bothButtons_lastRepeatedTime = 0;
bool bothButtons_isPressed = false;

// task
TaskHandle_t core0Task;

void readButtons();

void core0TaskCode( void * pvParameters )
{
    Serial.print("core 0 task is running on core ");
    Serial.println(xPortGetCoreID());

    for(;;)
    {
        //delay(10);
        readButtons();
    } 
}

void button1PresHandler();
void button2PresHandler();
void bothButtonsPressHandler();

void buttonsSetup()
{
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);

    xTaskCreatePinnedToCore(
                        core0TaskCode,   // Task function.
                        "core0Task",     // name of task.
                        10000,       // Stack size of task 
                        NULL,        // parameter of the task
                        0,           // priority of the task 
                        &core0Task,      // Task handle to keep track of created task 
                        0);          // pin task to core 0 

}

void readButtons()
{
    long time = millis();

    int button1Val = digitalRead(BUTTON_1);
    int button2Val = digitalRead(BUTTON_2);
    
    //Serial.println(String(button1Val) + " " + String(button2Val));

    bool button1PressHappened = false;
    bool button2PressHappened = false;
    bool bothButtonsPressHappened = false;

    if (button1Val != HIGH) 
    {
        // button pressed
        if(button1_isPressed)
        {
            // been pressed before
            if(time - button1_pressStartedTime > repeatStart && time - button1_lastRepeatedTime > repeatPeriod)
            {
                button1_lastRepeatedTime = time;
                button1PressHappened = true;
            }
        }
        else
        {
            // pressed for the first time
            button1_isPressed = true;
            button1_pressStartedTime = time;
            button1_lastRepeatedTime = 0;
            button1PressHappened = true;
        }
    }
    else
    {
        // button not pressed
        button1_isPressed = false;
        button1_pressStartedTime = 0;
        button1_lastRepeatedTime = 0;
    }
    
    if (button2Val != HIGH) 
    {
        // button pressed
        if(button2_isPressed)
        {
            // been pressed before
            if(time - button2_pressStartedTime > repeatStart && time - button2_lastRepeatedTime > repeatPeriod)
            {
                button2_lastRepeatedTime = time;
                button2PressHappened = true;
            }
        }
        else
        {
            // pressed for the first time
            button2_isPressed = true;
            button2_pressStartedTime = time;
            button2_lastRepeatedTime = 0;
            button2PressHappened = true;
        }
    }
    else
    {
        // button not pressed
        button2_isPressed = false;
        button2_pressStartedTime = 0;
        button2_lastRepeatedTime = 0;
    }

    if (button1_isPressed && button2_isPressed) 
    {
        // button pressed
        if(bothButtons_isPressed)
        {
            // been pressed before
            if(time - bothButtons_pressStartedTime > repeatStart && time - bothButtons_lastRepeatedTime > repeatPeriod)
            {
                bothButtons_lastRepeatedTime = time;
                bothButtonsPressHappened = true;
            }
        }
        else
        {
            // pressed for the first time
            bothButtons_isPressed = true;
            bothButtons_pressStartedTime = time;
            bothButtons_lastRepeatedTime = 0;
            //button1PresHandler();
            bothButtonsPressHappened = true;
        }
    }
    else
    {
        // button not pressed
        bothButtons_isPressed = false;
        bothButtons_pressStartedTime = 0;
        bothButtons_lastRepeatedTime = 0;
    }


    if(bothButtonsPressHappened)
        bothButtonsPressHandler();
    else
    {
        if(button1PressHappened)
            button1PresHandler();

        if(button2PressHappened)
            button2PresHandler();
    }
}

#endif