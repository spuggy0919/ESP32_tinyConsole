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
#include "WebServer.h"



void wsTextPrintBase64(int key,String msg); // declared in Webserver.cpp

// void app_main(void);

void TaskKeyIn( void *pvParameters );
void TaskBlink1(void *pvParameters);
void Taskprint(void *pvParameters);
void TasktriggerLog(void *pvParameters);
void TaskDummy(void *pvParameters);

QueueHandle_t queue;
QueueHandle_t queue1;
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;
SemaphoreHandle_t mux;
int txbuf[10];

int cmd_task(int argc, char *argv[]){
    //  app_main();
    //  return 0;
    queue = xQueueCreate( 10, sizeof( int ) );
    if(queue == NULL){
        Serial.println("Error creating the queue");
        return -1;
    }
    queue1 = xQueueCreate( 10, sizeof( int ) );
    if(queue1 == NULL){
        Serial.println("Error creating the queue1");
                return -1;

    }
    mux = xSemaphoreCreateMutex();
    if(mux == NULL){
        Serial.println("Error creating the queue1");
                return -1;

    }
    Serial.println("created the queues");
    Serial.println("created the mux");

    xTaskCreate(TaskKeyIn,"TaskKeyIn",4096,NULL,tskIDLE_PRIORITY+3,NULL);   
    xTaskCreate(TaskBlink1,"TaskBlink",4096,NULL,tskIDLE_PRIORITY+2,NULL);   
    // xTaskCreate(Taskprint,"Task3",1024,NULL,3,NULL);   
    // xTaskCreate(TasktriggerLog,"Task3",1024,NULL,3,NULL);   
    vTaskStartScheduler();
    return 0;
}

void TaskKeyIn( void *pvParameters ){
      // input key counter
 int button1;
 
 Serial.printf("TaskKeyIn Wakeup\n");

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
            if(xSemaphoreTake(mux, portMAX_DELAY) == pdTRUE) {
                xQueueSend(queue, &trigger, 10); 
                Serial.printf("queue triggrecnt(%2d)\n",trigger);
                trigger++;
                 xSemaphoreGive(mux);
            }


            //  eventSend((button == LOW)? "1" : "0","onoff");
            pwmled((button1 == LOW)? 255 : 0);
            //  eventSend((button == LOW)? "ON": "OFF" ,"gpiostate");
        }
    // vTaskDelete( NULL );
        vTaskDelay( 50/ portTICK_PERIOD_MS ); 

  }
}

void TaskBlink1(void *pvParameters) {
    int prevvalue=0;
    int keychange= 0;
    pwminit();
    Serial.printf("TaskBlink1 Wakeup\n");

    while(1) {
       Serial.println("TaskBlink1");
      if(xSemaphoreTake(mux, portMAX_DELAY) == pdTRUE) {

        if (xQueueReceive(queue, &keychange, 10)) {
            Serial.printf("recev triggrecnt(%2d)\n",keychange);

        } //portMAX_DELAY);
        xSemaphoreGive(mux);

      }

        if (keychange!=prevvalue) {
            // vTaskDelay( 5000 / portTICK_PERIOD_MS ); 
            prevvalue = keychange;
        } 
        if (keychange == 10) {
            // WSTransferBufferTaskInit(0);
            //  wsTextPrintln("hello this is task message!");

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





void WSTransferBuffer(int wi);

TaskHandle_t vTaskBufferHandle = NULL;

void WSTransferBufferTask(void *pvParameters){
   int idx;
   int wi = 0 ; // to textarea 
   while(1) {
    //    Serial.printf("WSTransferBufferTask\n");
      WSTransferBufferFlush(0);
    //   wsTextPrintBase64(0,"hello this is task message!");
      vTaskDelay( 10 / portTICK_PERIOD_MS ); 

   }

}
// static bool taskin=false;
bool WSTransferBufferTaskInit(int wi){
    // return true ; //not work for Task
  // if (taskin) return true;
  //   taskin = true;
  // queueTX move to stdio startup routine in setup()
   // queueTX = xQueueCreate(BUFFERSIZE, sizeof( unsigned char ) );
    // if(queueTX == NULL){
    //     Serial.println("Error creating the queue1");
    //     return false;
    // }
    bool ret = xTaskCreate(WSTransferBufferTask,"WSTxTask",4096,NULL,2,&vTaskBufferHandle);  
    Serial.println("task Create with Queue...");
    // vTaskStartScheduler();

    return ret;
}

void WSTransferBuffer(int wi){

 char obuf[2][BUFFERSIZE];
 int olen[2]={0,0};
  if (olen[wi]>0) {
    int len= olen[wi];
    obuf[wi][olen[wi]]=0; olen[wi]+=1;
    wsTextPrintBase64(wi,String(obuf[wi]));  olen[wi]=0;
    Serial.printf("vxTask:flash buffer %d\n",len);
  }
}
bool WSTransferBufferTaskDestroy(){
    if( vTaskBufferHandle!= NULL )
     {
        Serial.printf("task Delete...\n");
         vTaskDelete( vTaskBufferHandle);
         return true;
     }
     return false;
}
