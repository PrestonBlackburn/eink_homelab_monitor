#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/sync.h"      
#include "pico/runtime_init.h"
// #include "deep_sleep.h"
#include "hardware/structs/rtc.h"
#include "hardware/structs/scb.h"
#include "hardware/structs/clocks.h"

static bool is_rtc_enabled(void) {
    return !!(rtc_hw->ctrl & RTC_CTRL_RTC_ENABLE_BITS);
}

void deepsleep_minutes(uint32_t minutes) {
    printf("Starting Deep Sleep");
    // 1. Ensure RTC is running
    if (!is_rtc_enabled()) {
        rtc_hw->ctrl = RTC_CTRL_RTC_ENABLE_BITS;
        rtc_hw->setup_0 = (2026 << RTC_SETUP_0_YEAR_LSB) | (1 << RTC_SETUP_0_MONTH_LSB) | (25 << RTC_SETUP_0_DAY_LSB);
        rtc_hw->setup_1 = (0 << RTC_SETUP_1_DOTW_LSB) | (12 << RTC_SETUP_1_HOUR_LSB) | (0 << RTC_SETUP_1_MIN_LSB) | (0 << RTC_SETUP_1_SEC_LSB);
        rtc_hw->ctrl |= RTC_CTRL_LOAD_BITS;
        sleep_ms(10);
    }

    // 2. RTC_RTC_1 contains Hour/Min/Sec
    uint32_t rtc_1 = rtc_hw->rtc_1;
    uint32_t current_hour = (rtc_1 & RTC_RTC_0_HOUR_BITS) >> RTC_RTC_0_HOUR_LSB;
    uint32_t current_min = (rtc_1 & RTC_RTC_0_MIN_BITS) >> RTC_RTC_0_MIN_LSB;
    uint32_t current_sec = (rtc_1 & RTC_RTC_0_SEC_BITS) >> RTC_RTC_0_SEC_LSB;

    // 3. Calculate target
    // uint32_t target_min = (current_min + minutes) % 60;
    uint32_t target_sec = (current_sec + 5) % 60;
    uint32_t target_min = current_min + ((current_sec + 5) / 60);

    // verify rtc
    printf("RTC Current: %02u:%02u:%02u\n", current_hour, current_min, current_sec);
    printf("RTC Alarm  : XX:%02u:%02u\n", target_min, current_sec);
    uart_default_tx_wait_blocking();

    // Setup_0 usually handles Day/Month/Year matches
    // Setup_1 handles DOTW/Hour/Min/Sec matches
    rtc_hw->irq_setup_0 &= ~RTC_IRQ_SETUP_0_MATCH_ENA_BITS; 

    // Set match values in Setup 1 (Min/Sec)
    rtc_hw->irq_setup_1 = (target_min << RTC_IRQ_SETUP_1_MIN_LSB) |
                          (target_sec << RTC_IRQ_SETUP_1_SEC_LSB);
    
    // Enable matching for Minute and Second only
    rtc_hw->irq_setup_0 = RTC_IRQ_SETUP_0_MATCH_ENA_BITS | 
                          RTC_IRQ_SETUP_1_MIN_ENA_BITS | 
                          RTC_IRQ_SETUP_1_SEC_ENA_BITS;

    // prepare interupts
    rtc_hw->inte = RTC_INTE_RTC_BITS;

    // 5. Prepare Clocks
    uint32_t scr_save = scb_hw->scr;
    uint32_t clk0_save = clocks_hw->sleep_en0;
    uint32_t clk1_save = clocks_hw->sleep_en1;
    
    clocks_hw->sleep_en0 = CLOCKS_SLEEP_EN0_CLK_RTC_RTC_BITS | 
                            CLOCKS_SLEEP_EN0_CLK_SYS_PLL_SYS_BITS;
    clocks_hw->sleep_en1 = 0x0;

    // Enable RTC Interrupt
    rtc_hw->inte = RTC_INTE_RTC_BITS;

    // Set Deep Sleep bit
    scb_hw->scr |= M0PLUS_SCR_SLEEPDEEP_BITS;

    // 6. Go to sleep
    __wfi(); 

    // --- WAKE UP ---
    scb_hw->scr = scr_save;
    clocks_hw->sleep_en0 = clk0_save;
    clocks_hw->sleep_en1 = clk1_save;
    
    // Bring system clocks back
    clocks_init();
    stdio_init_all();
}