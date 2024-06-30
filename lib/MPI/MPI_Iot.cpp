
#include "MPI_lw.h"
#include "ledpwm.h"         // TODO pwm led for ESP32 using arduino IO for dispatch
#include "timerfun.h"         // TODO ESP32TIME rtc

#include <string.h>
#include "wsSerial.h"         // TODO wsSerial IO  for ESP32 using arduino IO for dispatch
#include <Freertos/FreeRTOS.h>
#include <Freertos/task.h>



int MPI_Iot_LED(int pwmvalue)
{
    pwmled(pwmvalue);
    return MPI_SUCCESS;
}
int MPI_Iot_LED_Blink(int n){
    int i,t;
    for (t=0;t<5;t++){
        for(int i=0;i<n+1;i++) {
            pwmled(0); delay(50);
            pwmled(255); delay(100);
        }
        pwmled(0); delay(50);
        delay(100);
    }
    pwmled(255); 
    return 0;
}


// mimic shell excute in tinyconsole, you can change to call mpich program name
int MPI_Iot_MPICH(const char *cmd){
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
                1,                         // Priority (1 is higher priority, adjust as needed)
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
