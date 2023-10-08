#include "definitions.h"
#include "Utils.h"

TaskHandle_t led_handle = NULL;

extern "C" {
    void app_main();
}

void app_main()
{
    led_handle = enable(LED_PIN, DUTY_CYCLE_40, FREQ_2, 0);
    enable(SPEAKER_PIN, DUTY_CYCLE_50, FREQ_12, BT_TIME);

    gpio_set_level(LED_PIN, LOW);
  
}
