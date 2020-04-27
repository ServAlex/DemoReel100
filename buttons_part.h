#ifndef buttons_part_h
#define buttons_part_h

#include <Wire.h>
/*
#define BUTTON_1 0
#define BUTTON_2 1

#define BUTTON_UP 2
#define BUTTON_DOWN 3
#define BUTTON_LEFT 4
#define BUTTON_RIGHT 5
*/
#define but_count 6

int repeatStart = 1000;
int repeatPeriod = 10;

int pins[but_count] = {0, 35, 25, 26, 12, 13};
long pressStartedTime[but_count] = {0};
long lastRepeatedTime[but_count] = {0};
bool isPressed[but_count] = {0};

// task
TaskHandle_t core0Task;

void readButtons();
void secondTask();

void core0TaskCode( void * pvParameters )
{
    Serial.print("core 0 task is running on core ");
    Serial.println(xPortGetCoreID());

    for(;;)
    {
        //delay(10);
        readButtons();
        secondTask();
    } 
}

void button1PresHandler();
void button2PresHandler();
void buttonUpPresHandler();
void buttonDownPresHandler();
void buttonLeftPresHandler();
void buttonRightPresHandler();
//void bothButtonsPressHandler();

typedef void (*HandlerList[])();

HandlerList handlers = {
    button1PresHandler,
    button2PresHandler,
    buttonUpPresHandler,
    buttonDownPresHandler,
    buttonLeftPresHandler,
    buttonRightPresHandler
    //bothButtonsPressHandler,
};

void buttonsSetup()
{
    for(int i = 0; i<but_count; i++)
        pinMode(pins[i], INPUT_PULLUP);

    //pinMode(0, INPUT_PULLUP);
    //pinMode(35, INPUT_PULLUP);

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

    int vals[but_count] = {0};
    for(int i = 0; i<but_count; i++)
        vals[i] = digitalRead(pins[i]);

    bool pressHappened[but_count] = {0};
    for(int i = 0; i<but_count; i++)
    {
        pressHappened[i] = false;

        if (vals[i] != HIGH) 
        {
            // button pressed
            if(isPressed[i])
            {
                // been pressed before
                if(time - pressStartedTime[i] > repeatStart && time - lastRepeatedTime[i] > repeatPeriod)
                {
                    lastRepeatedTime[i] = time;
                    pressHappened[i] = true;
                }
            }
            else
            {
                // pressed for the first time
                isPressed[i] = true;
                pressStartedTime[i] = time;
                lastRepeatedTime[i] = 0;
                pressHappened[i] = true;
            }
        }
        else
        {
            // button not pressed
            isPressed[i] = false;
            pressStartedTime[i] = 0;
            lastRepeatedTime[i] = 0;
        }

    }

/*
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
*/
/*
    if(bothButtonsPressHappened)
        bothButtonsPressHandler();
    else
    {
        if(button1PressHappened)
            button1PresHandler();

        if(button2PressHappened)
            button2PresHandler();
    }
*/
    Serial.println("reacting");

    for(int i = 0; i<but_count; i++)
    {
        Serial.print(String(vals[i]));
        //if(pressHappened[i])
        if(vals[i] != HIGH)
        {
            //handlers[i]();
        }
    }

    Serial.println("\nhandler");
    //button1PresHandler();

    Serial.println("done reading buttons");
}

#endif