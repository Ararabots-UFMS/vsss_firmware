#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

//Defines a thing to be enabled, has a GPIO pin, a duty cycle and a frequency
typedef struct {
   gpio_num_t pin; 		// GPIO of the pin
   float duty;			// duty cycle of the GPIO
   float freq;			// toggle frequency
   long long int lifetime;
} thing;



// FreeRTOS task that enables some pin passed as argument
void thingEnable(void* pvParameters);

// receive a GPIO pin, a duty cycle and frquency to enable that pin
TaskHandle_t enable(gpio_num_t pino, float duty, float freq, long long int lifetime);
