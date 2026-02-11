#include "utils.h"
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

    ServerStatus server_status;
    server_status.server_online = false;

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
    // test_dns_lookup();  
    // DEV_Delay_ms(2000);

    printf("Starting HTTP Request Test\n");
    int http_status = test_http_request();
    DEV_Delay_ms(5000); 

    // printf("Run NTP Test\n");
    //run_ntp_test(); // no return yet, just logs

    // initialize rtc
    rtc_init();
    datetime_t dt;

    printf("Getting Current Time From NTP\n");
    time_t current_time;
    if (get_ntp_time(&current_time) == 0) {
        printf("Got NTP time\n");
        // Read back from RTC
        rtc_get_datetime(&dt);
        printf("RTC time: %04d-%02d-%02d %02d:%02d:%02d\n",
               dt.year, dt.month, dt.day,
               dt.hour, dt.min, dt.sec);
    } else {
        printf("Failed to get NTP time\n");
    }

    if (http_status == 0) {
        printf("Server Online\n");
        if (server_status.server_online == false) {
            // really this should be a time diff (easier diff with int)
            server_status.last_state_change = dt;
            printf("State changed to ONLINE\n");
        }
        server_status.server_online = true;
    } else {
        printf("Server Offline\n");
        if (server_status.server_online == true) {
            // really this should be a time diff
            server_status.last_state_change = dt;
            printf("State changed to OFFLINE\n");
        }
        server_status.server_online = false;
    }
    // Convert datetime_t to time_t
    time_t time1, time2;
    datetime_to_time(&server_status.last_state_change, &time1);
    datetime_to_time(&dt, &time2);

    // Get difference in seconds
    time_t diff_seconds = time2 - time1;

    printf("Time difference: %ld seconds\n", diff_seconds);

    int days = diff_seconds / 86400;
    int hours = (diff_seconds % 86400) / 3600;
    server_status.uptime_days = days;
    server_status.uptime_hours = hours;

    DEV_Delay_ms(5000); 

    int eink_status = set_eink_status(&server_status);
    // int eink_status = set_eink_status(http_status, &dt);

    return 0;
}
