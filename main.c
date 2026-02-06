#include "utils.h"   //Examples
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "lwip/apps/http_client.h"
#include "pico/cyw43_arch.h"
#include "init_wifi.h"
#include "http_request.h"


// will basically be a while loop of: 
// init/wakeup
// make http request
// update eink display
// enter deepsleep
int main(void)
{
    DEV_Delay_ms(5000); 

    int retry_count = 5;
    int flash_wifi_led = 1;
    int connect_status = init_wifi(retry_count, flash_wifi_led);

    int http_status = test_http_request();

    int eink_status = eink_test();

    return 0;
}
