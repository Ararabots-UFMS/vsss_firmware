/*
This example code is in the Public Domain (or CC0 licensed, at your option.)

Unless required by applicable law or agreed to in writing, this
software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied.
*/

#define SPP_TAG "Eymael"

#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "PIDController.h"

#include "bluetooth.h"

#include "Motors.h"
#include "Voltimetro.h"
#include "time.h"
#include "sys/time.h"
#include "driver/mcpwm.h"

#include "definitions.h"

extern "C" {
    void app_main();
}

void voltimetro(void * pvParamters){
    Voltimetro voltimetro(R1,R2);

    /* Select the GPIO to be used */
    gpio_pad_select_gpio(SPEAKER_PIN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(SPEAKER_PIN, GPIO_MODE_OUTPUT);


    while(true){

        // If the read voltage is less than 9 volts it activates the buzzer
        if (voltimetro.getVoltage() < V_MIN) {
            gpio_set_level(SPEAKER_PIN, HIGH);
        }
        else {
            gpio_set_level(SPEAKER_PIN, LOW);
        }
        vTaskDelay(MEASURE_TIME);
    }
}

void app_main(){


    xTaskCreate(voltimetro, "voltimetro", TASK_SIZE, NULL, 0, NULL);
    // Voltimetro a(R1,R2);
    // while(1){
    //     float f = a.getVoltage();
    //     printf("Voltage: %f\n", f);
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }
    
    // printf("Restarting now.\n");
    // fflush(stdout);
    // // esp_restart();

}
