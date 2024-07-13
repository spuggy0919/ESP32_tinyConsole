
#include "MPI_Lite.h"
#include "ledpwm.h"         // TODO pwm led for ESP32 using arduino IO for dispatch
#include "timerfun.h"         // TODO ESP32TIME rtc

#include <string.h>
#include "wsSerial.h"         // TODO wsSerial IO  for ESP32 using arduino IO for dispatch
#include <Freertos/FreeRTOS.h>
#include <Freertos/task.h>
#include <freertos/timers.h>


TimerHandle_t timerHandle_LED_Blink;
bool ledState = false;
int ledValue = false;
void timerLedBlinkCallback(TimerHandle_t xTimer) {
    pwmled(ledState ? 0:255);
    ledState = !ledState;
}
bool MPI_Iot_Setup()
{
    // Create a software timer
    timerHandle_LED_Blink = xTimerCreate(
        "LEDTimer",           // Timer name
        pdMS_TO_TICKS(100),  // Period in milliseconds (1 second)
        pdTRUE,               // Auto-reload timer
        (void *) 0,           // Timer ID
        timerLedBlinkCallback        // Timer callback function
    );
    return true;
}
static int ledLastState = 255;
int MPI_Iot_LED_Blink(int onoff,int pwmvalue){
    if (timerHandle_LED_Blink != NULL) {
        if (onoff){
            pwmled(pwmvalue);
            // Start the timer
            xTimerStart(timerHandle_LED_Blink, 0);
        }else{
            xTimerStop(timerHandle_LED_Blink, 0);
            pwmled(ledLastState);
        }
    }
    return MPI_SUCCESS;
}
int MPI_Iot_LED(int pwmvalue)
{
    xTimerStop(timerHandle_LED_Blink, 0);
    pwmled(pwmvalue);
    ledLastState = pwmvalue;
    return MPI_SUCCESS;
}
int MPI_Iot_LED_toggle()
{
    xTimerStop(timerHandle_LED_Blink, 0);
    ledLastState = 255 - ledLastState;
    pwmled(ledLastState);
    return MPI_SUCCESS;
}



// mimic shell excute in tinyconsole, you can change to call mpich program name
int MPI_Iot_MPIRUN(const char *cmd){ // run at localhost
        wsSerial.push(cmd,strlen(cmd)); //websocket input
        MPI_PRINTF(cmd); 
    return MPI_SUCCESS;
}


// Task function that delays for a specified duration and then calls myFunction
void timeoutTask(void *pvParameters) {
    TickType_t *pDelayDuration = (TickType_t *)pvParameters; // Cast pvParameters to TickType_t*
    
    // Delay for the specified duration
    vTaskDelay(*pDelayDuration);
    
    // Call the function after the delay, restart
        pwmled(0);
        ESP.restart();    
    // Task should delete itself if it reaches here (optional, depends on your application)
    vTaskDelete(NULL);
}
void setupTimeoutTask(TickType_t delayMillis) {
    // Create the task with appropriate priority and stack size
    xTaskCreate(timeoutTask,               // Task function
                "RestartTask",             // Task name (for debugging)
                configMINIMAL_STACK_SIZE,  // Stack size (configMINIMAL_STACK_SIZE is a FreeRTOS constant)
                &delayMillis,              // Task parameters (pointer to delay duration)
                3,                         // Priority (1 is higher priority, adjust as needed)
                NULL);                     // Task handle (optional, not used here)
}

#define RST_TIMEOUT 5000
int MPI_Iot_Restart(int shutdown){
    shutdown = (shutdown > RST_TIMEOUT)? RST_TIMEOUT : shutdown;
    TickType_t delayTime = pdMS_TO_TICKS(shutdown); // Example delay time: 1000 milliseconds
    // Initialize FreeRTOS and hardware if needed
    setupTimeoutTask(delayTime);
        // Start FreeRTOS scheduler if need
    //vTaskStartScheduler();
    return MPI_SUCCESS;
}

void MPI_Iot_SetEpoch(unsigned long  Epoch){
    rtc.setTime(Epoch);
}
unsigned long MPI_Iot_GetEpoch(){
    return rtc.getEpoch();          //  
}
