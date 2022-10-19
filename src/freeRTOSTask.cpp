/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 *  this for testing purpose, no in project kernal
 */
#include <Arduino.h>
#include "ledpwm.h"
#include "keyin.h"


QueueHandle_t queue;
QueueHandle_t queue1;
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;

int txbuf[10];
void TaskKeyIn( void *pvParameters ){
      // input key counter
 int button1;
 button1 = debounceKeyRead(BUTTON_PIN); // read new state  
 int trigger = 0;
 bool keyChange = false;
  while(1){
         button1 = debounceKeyRead(BUTTON_PIN); // read new state  
        if(button1 == LOW) {
            trigger++; 
            if (trigger>30) trigger = 1;
            // wsTextPrint("E: triggerCnt"+String(triggerCnt));
            // eventSend(String(triggerCnt).c_str(),"switchcnt");
            Serial.printf("The button is pressed(%2d)\n",trigger);
            // toggle state of LED
            pwmled(255);
            // eventSend("ON","gpiostate");

        }
        if (KeygetChange()){
            //txbuf[0]=triggerCnt;
            xQueueSend(queue, &trigger, 10); 
            Serial.printf("queue triggrecnt(%2d)\n",trigger);
            trigger++;

            //  eventSend((button == LOW)? "1" : "0","onoff");
            pwmled((button1 == LOW)? 255 : 0);
            //  eventSend((button == LOW)? "ON": "OFF" ,"gpiostate");
        }
        vTaskDelay( 500 / portTICK_PERIOD_MS ); 

  }
}
// bool WSTransferBufferTaskInit(int wi);
void wsTextPrintBase64(int key,String msg);

void TaskBlink1(void *pvParameters) {
    int prevvalue=0;
    int keychange= 0;
    pwminit();

    while(1) {
        // Serial.println("TaskBlink1");

        if (xQueueReceive(queue, &keychange, 10)) {
            Serial.printf("recev triggrecnt(%2d)\n",keychange);

        } //portMAX_DELAY);

        // if (keychange!=prevvalue) {
        //     vTaskDelay( 5000 / portTICK_PERIOD_MS ); 
        //     prevvalue = keychange;
        // } 
        if (keychange == 10) {
            // WSTransferBufferTaskInit(0);
             wsTextPrintBase64(0,"hello this is task message!");

        }
        if (keychange%3) {
            pwmled(255);
            vTaskDelay( 500 / portTICK_PERIOD_MS ); 
            pwmled(0);
            vTaskDelay( 500 / portTICK_PERIOD_MS ); 
        }
    }
}
void Taskprint(void *pvParameters)  {
     int counter = 0;
    while(1){
        // xQueueSend(queue1, &keychange, 1);
        Serial.println("Taskprint");

        counter++;
        Serial.println(counter);
        vTaskDelay( 500 / portTICK_PERIOD_MS ); 
    }
}
void TasktriggerLog(void *pvParameters)  {
    int tricnt=0;
    while(1){
       xQueueReceive(queue1, &tricnt, portMAX_DELAY);
      if (tricnt) pwmtriggerstart();
      pwmtrigger(500, tricnt, (bool)(tricnt%2)); // true:pwm, false: no pwm
      vTaskDelay( 500 / portTICK_PERIOD_MS ); 
      Serial.println(pwmlogData());
      vTaskDelay( 500 / portTICK_PERIOD_MS ); 

    }

}  // similar as log data
void TaskDummy(void *pvParameters)  {
}
void app_main(void);

int cmd_task(int argc, char *argv[]){
    //  app_main();
     return 0;
    queue = xQueueCreate( 10, sizeof( int ) );
    if(queue == NULL){
        Serial.println("Error creating the queue");
    }
    queue1 = xQueueCreate( 10, sizeof( int ) );
    if(queue1 == NULL){
        Serial.println("Error creating the queue1");
    }
    xTaskCreate(TaskBlink1,"Task1",1024,NULL,2,NULL);   
    xTaskCreate(TaskKeyIn,"Task2",1024,NULL,4,NULL);   
    // xTaskCreate(Taskprint,"Task3",1024,NULL,3,NULL);   
    // xTaskCreate(TasktriggerLog,"Task3",1024,NULL,3,NULL);   
    // vTaskStartScheduler();
    return 0;
}