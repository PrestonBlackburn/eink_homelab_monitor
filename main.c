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
#include "custom_sleep.h"
#include "pico/util/datetime.h" 
#include "pico/sleep.h"

int main(void) {

    stdio_init_all();
    DEV_Delay_ms(5000); 

    if (DEV_Module_Init() != 0) {
        printf("e-Paper init failed\n");
        return -1;
    }
    eink_init();

    ServerStatus server_status;
    server_status.server_online = false;
    int test_counter = 0;

    #if 1
        int retry_count = 5;
        int flash_wifi_led = 1;
        printf("Starting Init Wifi\n");
        int connect_status = init_wifi(retry_count, flash_wifi_led);
        if (connect_status != 0) {
            printf("WiFi init failed, stopping\n");
            return -1;
        }
        DEV_Delay_ms(1000); 
        // for troubleshooting
        // test_dns_lookup();  
        // DEV_Delay_ms(2000);

        // initialize rtc once
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
    #else
        printf("skipping wifi setup\n");
    #endif

    while (1) {

        #if 1

            printf("Starting HTTP Request Test\n");
            int http_status = test_http_request();
            DEV_Delay_ms(2000); 

            rtc_get_datetime(&dt);
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

            int days;
            int hours;
            if (diff_seconds < 86400) {
                days = 0;
                hours = diff_seconds / 3600;
            } else {
                days = diff_seconds / 86400;
                hours = (diff_seconds % 86400) / 3600;
            }
            server_status.uptime_days = days;
            server_status.uptime_hours = hours;

            DEV_Delay_ms(5000); 

        #else
            // for debugging loop
            printf("Starting Debug Loop %d\n", test_counter);
                    
            datetime_t dt;
            rtc_get_datetime(&dt);
            server_status.uptime_days = 0;
            server_status.uptime_hours = test_counter;
            server_status.last_state_change = dt;
            test_counter++;
            sleep_ms(100);

        #endif

        int eink_status = set_eink_status(&server_status, &dt);

        // I had a bunch of issues getting deepsleep working...
        // maybe revisit later with external rtc fow wakeup
        #if 0
            printf("Preparing for sleep\n");
            uart_default_tx_wait_blocking();
            
            // Deinitialize WiFi/CYW43 before sleep
            cyw43_arch_deinit();
            // Slow down clocks right before sleep
            sleep_run_from_dormant_source(DORMANT_SOURCE_XOSC);

            bool sleep_success = sleep_goto_sleep_for(5000, NULL);
            sleep_power_up(); // Restore clocks
            stdio_init_all(); // Restore UART

            if (sleep_success) {
                printf("Woke up!\n");
            } else {
                printf("Issue sleeping\n");
            }
            // Reinitialize WiFi for next loop
            printf("Reinitializing WiFi\n");
            if (init_wifi(retry_count, flash_wifi_led) != 0) {
                printf("WiFi reinit failed\n");
                return -1;
            }
        #else
            // poll time
            int min = 15;
            sleep_ms(1000 * 60 * min);

        #endif
    }

    return 0;
}
