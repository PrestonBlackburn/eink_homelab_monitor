#include "utils.h"   //Examples
#include <stdio.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/rtc.h"
#include "lwip/apps/http_client.h"
#include "pico/cyw43_arch.h"
#include "init_wifi.h"
#include "http_request.h"
#include "ntp_client.h"


// will basically be a while loop of: 
// init/wakeup
// make http request
// update eink display
// enter deepsleep
int main(void) {
    stdio_init_all();
    DEV_Delay_ms(5000); 

    int retry_count = 5;
    int flash_wifi_led = 1;
    printf("Starting Init Wifi\n");
    int connect_status = init_wifi(retry_count, flash_wifi_led);
    if (connect_status != 0) {
        printf("WiFi init failed, stopping\n");
        return -1;
    }
    DEV_Delay_ms(5000); 

    // for troubleshooting
    test_dns_lookup();  
    DEV_Delay_ms(2000);

    printf("Starting HTTP Request Test\n");
    int http_status = test_http_request();
    DEV_Delay_ms(5000); 

    // printf("Run NTP Test\n");
    //run_ntp_test(); // no return yet, just logs

    // initialize rtc
    rtc_init();
    printf("Getting Current Time From NTP\n");
    time_t current_time;
    if (get_ntp_time(&current_time) == 0) {
        printf("Got NTP time\n");
        // Read back from RTC
        datetime_t dt;
        rtc_get_datetime(&dt);
        printf("RTC time: %04d-%02d-%02d %02d:%02d:%02d\n",
               dt.year, dt.month, dt.day,
               dt.hour, dt.min, dt.sec);
    } else {
        printf("Failed to get NTP time\n");
    }

    DEV_Delay_ms(5000); 

    // int eink_status = eink_test();
    datetime_t dt;
    rtc_get_datetime(&dt);
    int eink_status = set_eink_status(http_status, &dt);

    return 0;
}
