#include "Utils.h"
#include "definitions.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"


void thingEnable(void* pvParameters){

    thing* enable = (thing*) pvParameters;
    //ESP_LOGE("","%d %f %f %lld ", enable->pin, enable->duty, enable->freq, enable->lifetime);
    /* Select the GPIO to be used */
    esp_rom_gpio_pad_select_gpio(enable->pin);

    /* Set the GPIO as a push/pull output */
    gpio_set_direction(enable->pin, GPIO_MODE_OUTPUT);

    long long int last_time, current_time = esp_timer_get_time();
    last_time = esp_timer_get_time();
    bool condition = enable->lifetime != 0 ? (current_time - last_time) < enable->lifetime : true;
    while(condition == true)
    {
        /* Turn ON (output HIGH) */
        gpio_set_level(enable->pin, HIGH);
        vTaskDelay(1000*enable->duty*enable->freq / portTICK_PERIOD_MS);

        /* Turn OFF (output LOW) */
        gpio_set_level(enable->pin, LOW);
        vTaskDelay(1000*(1.0-enable->duty)*enable->freq / portTICK_PERIOD_MS);
        current_time = esp_timer_get_time();
        condition = enable->lifetime != 0 ? (current_time - last_time) < enable->lifetime : true;
    }

      /* turn GPIO low to delete task */
    gpio_set_level(enable->pin, LOW);


    free(enable);
    /* delete the task */
    vTaskDelete(NULL);
}


// Lifetime is the period in microseconds that the thread should execute
// For convention if lifetime is zero, then the thread should execute forever
TaskHandle_t enable(gpio_num_t pino, float duty, float freq, long long int lifetime)
{

    /* struct to hold information about what will happen */
    thing* enable = (thing* ) malloc(sizeof(thing));

    enable->pin = pino;
    enable->duty = duty;
    enable->freq = freq;
    enable->lifetime = lifetime;

    TaskHandle_t handle;

    /* create a task and start to execute */
    auto x = xTaskCreatePinnedToCore(thingEnable, "thingEnable", 2048,
                                      (void*) enable, 0, &handle, CORE_ONE);

    if(x != pdPASS) ESP_LOGE("ENABLE", "Error creating thread, ERR_CODE: #%X", x);
    else
    {
      #ifdef DEBUG
        ESP_LOGI("ENABLE", "Task thingEnable to pin %u created!", pino);
      #endif
    }

    return handle;
}
