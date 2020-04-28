#ifndef buttons_part_h
#define buttons_part_h

#include <Wire.h>
#define but_count 6

int repeatStart = 1000;
int repeatPeriod = 10;

int pins[but_count] = {0, 35, 25, 26, 12, 13};
long pressStartedTime[but_count] = {0};
long lastRepeatedTime[but_count] = {0};
bool isPressed[but_count] = {false, false, false, false, false, false};

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
    buttonDownPresHandler,
    buttonRightPresHandler,
    buttonLeftPresHandler,
    buttonUpPresHandler
    //bothButtonsPressHandler,
};

void buttonsSetup()
{
    for(int i = 0; i<but_count; i++)
        pinMode(pins[i], INPUT_PULLUP);

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

    for(int i = 0; i<but_count; i++)
    {
        if(i == 0)
            continue;
        if(pressHappened[i])
            handlers[i]();
    }
}

#endif