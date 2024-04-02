/*
 * This file is part of ESP32_TinyConsole.
 *   Copyright (c) 2024 Spuggy0919, spuggy0919@gmail.com
 *
 * ESP32_TinyConsole is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ESP32_TinyConsole is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ESP32_TinyConsole.  If not, see 
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#include "jswrap_tc.h"
/**
 * Javascript: setInterval
 */
struct timestruc_t{
    int  timerID;
    SemaphoreHandle_t mutex;
    TickType_t delay;
    TaskHandle_t handle;
    jerry_value_t func;
};
typedef struct timestruc_t *pTimer_t;

typedef struct {
    void* params;
} TaskInfo;

#define MAX_TASKS 10 // Maximum number of tasks
TaskInfo taskArray[MAX_TASKS]; // Array to store task information
int numTimers = 0; // Current number of tasks

#define TASKSTACK 8192

    void deleteTimer(int taskid);
    void stopAllTimers(){
        if (numTimers==0) return;
        for(int i=0;i<MAX_TASKS;i++) {
            if (taskArray[i].params!=NULL){
                 deleteTimer(i);
                 Serial.printf("[timer]:%d exist free\n",i);
            }
        }
    }
    void flushTimerSpace(){
        stopAllTimers();
        for(int i=0;i<MAX_TASKS;i++) {
            taskArray[i].params=NULL;
        }
        numTimers = 0;
    }
    // Task that simulates a delayed function call (setTimeout equivalent)
    void timerCallback(TimerHandle_t xTimer) {
        pTimer_t p = (pTimer_t)pvTimerGetTimerID(xTimer);
        if (xSemaphoreTake(p->mutex,portMAX_DELAY)==pdTRUE){
            jerry_value_t callback_fn =  p->func;
            jerry_value_t global_obj_val = jerry_current_realm ();
            jerry_value_t result_val = jerry_call (callback_fn, global_obj_val, NULL, 0);
            jerry_value_free (result_val);
            jerry_value_free (global_obj_val);
        }
        xSemaphoreGive(p->mutex);
    }


    int allocAndCreateTimer(jerry_value_t callfunc, unsigned int delayMs, bool interval) {
        int i; 
        // find empty 
        for(i=0;i<MAX_TASKS;i++){
            if (taskArray[i].params==NULL) break;
        }
        if (i!=MAX_TASKS && taskArray[i].params==NULL) { // found empty
            // Calculate the interval in ticks
            pTimer_t params =(pTimer_t)malloc(sizeof(struct timestruc_t));
            taskArray[i].params = params;
            // jerry_value_t callback_fn_copy = jerry_value_copy (callfunc);
            String timername = ((interval) ? "timeoutTimer":"intervalTimer");
            timername+=String(i);
            params->func = callfunc; //callback_fn_copy;        // Store the task information in the array
            params->delay = pdMS_TO_TICKS(delayMs);           
            params->timerID = i;
            params->mutex=xSemaphoreCreateBinary();
            xSemaphoreGive(params->mutex);
            params->handle = xTimerCreate(timername.c_str(), pdMS_TO_TICKS(delayMs),
                        interval, params, timerCallback);
            xTimerStart(params->handle,0);
            numTimers++;
            Serial.printf("[settimer]:alloc(%d)%s\n",i,timername.c_str());
            return i;
        } 
        return -1;

    }

    void deleteTimer(int taskid) {
    // Find the task information in the array
        // Stop the task and delete it
        if (taskid<0||taskid>=MAX_TASKS) return;
        if (taskArray[taskid].params==NULL) return;
        pTimer_t p = (pTimer_t)taskArray[taskid].params;
        Serial.printf("[timer][clearInterval]:exit(%d)\n",p->timerID);
        if (xSemaphoreTake(p->mutex,portMAX_DELAY)==pdTRUE){
            xTimerStop(p->handle,0);//p->delay);
            xTimerDelete(p->handle,0);
            vSemaphoreDelete(p->mutex);
            jerry_value_free (p->func); 
            free(p);
            taskArray[taskid].params = NULL; // free in task
            numTimers--;
        }
    }



    int  setInterval(jerry_value_t callfunc, unsigned int delayMs) {
            // Create a task for the periodic function call
            return allocAndCreateTimer(callfunc, delayMs, true);
    }
    
    int  setTimeout(jerry_value_t callfunc, unsigned int delayMs) { 
            // Create a task to simulate the delayed function call
            return allocAndCreateTimer(callfunc, delayMs, false);

    }

/**
 * Javascript: setTimeout
 */
JERRYXX_DECLARE_FUNCTION(set_timeout)
{
  JERRYX_UNUSED(call_info_p);
  jerry_value_t callback_fn = 0;
  uint32_t delay_time = 0;

  const jerryx_arg_t mapping[] =
      {
          jerryx_arg_function(&callback_fn, JERRYX_ARG_REQUIRED),
          jerryx_arg_uint32(&delay_time, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
      };

  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv))
  {
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");  //4

      
    return jerry_number(setTimeout(callback_fn, delay_time));
} /* js_set_timeout */

/**
 * Javascript: clearTimeout
 */
JERRYXX_DECLARE_FUNCTION(delete_timer)
{
  JERRYX_UNUSED(call_info_p);
  jerry_value_t callback_fn = 0;
  int32_t taskid = 0;

  const jerryx_arg_t mapping[] =
      {
          jerryx_arg_int32(&taskid, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
      };

  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv))
  {
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  //4


     deleteTimer(taskid) ;



    return jerry_undefined();
} /* js_delete_timer */


JERRYXX_DECLARE_FUNCTION(set_interval)
{
  JERRYX_UNUSED(call_info_p);
  jerry_value_t callback_fn = 0;
  uint32_t delay_time = 0;

  const jerryx_arg_t mapping[] =
      {
          jerryx_arg_function(&callback_fn, JERRYX_ARG_REQUIRED),
          jerryx_arg_uint32(&delay_time, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
      };

  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv))
  {
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");  //4
    // if (((uint32_t)callback_fn)==NULL) return -1;
    
    return jerry_number(setInterval(callback_fn, delay_time));
  
} /* js_set_interval */

// JERRYXX_DECLARE_FUNCTION(millis) defined in Jerryscipt lib
// {
//   JERRYX_UNUSED(call_info_p);
//   JERRYX_UNUSED(args_p);
//   JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

//   return jerry_number(millis()); // ardunino millis incorrect??
// } /* js_millis */



// /**
//  * Register Extra API into JavaScript global object.
//  *
//  * @return true - if the operation was successful,
//  *         false - otherwise.
//  */
// bool jerryxx_register_extra_api(void)
// {
//   bool ret = false;
//   /* Register the print function in the global object */
//   JERRYXX_BOOL_CHK(jerryx_register_global("print", jerryx_handler_print));

// cleanup:
//   return ret;
// }

/*******************************************************************************
 *                                   Extra API                                 *
 ******************************************************************************/
/**
 * Register Extra API into JavaScript global object.
 *
 * @return true - if the operation was successful,
 *         false - otherwise.
 */
bool jerryxx_register_extra_api(void)
{
  bool ret = false;
  flushTimerSpace();
  /* Register the print function in the global object */
  JERRYXX_BOOL_CHK(jerryx_register_global("print", jerryx_handler_print));

  /* Register the setTimeout function in the global object */
  JERRYXX_BOOL_CHK(jerryx_register_global("setTimeout", js_set_timeout));

  /* Register the clearTimeout function in the global object */
  JERRYXX_BOOL_CHK(jerryx_register_global("clearTimeout", js_delete_timer));

  /* Register the setInterval function in the global object */
  JERRYXX_BOOL_CHK(jerryx_register_global("setInterval", js_set_interval));

  /* Register the clearInterval function in the global object */
  JERRYXX_BOOL_CHK(jerryx_register_global("clearInterval", js_delete_timer));

//   JERRYXX_BOOL_CHK(jerryx_register_global("millis", js_millis));

cleanup:
  return ret;
} /* jerryxx_register_extra_api */