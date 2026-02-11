#include "utils.h"
#include "EPD_2in13_V4.h"
#include "GUI_Paint.h"

int eink_test(void)
{

    Debug("EPD_2in13_V4_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    Debug("e-Paper Init and Clear...\r\n");
	EPD_2in13_V4_Init();
    EPD_2in13_V4_Clear(); // white clear

    sleep_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2in13_V4_WIDTH % 8 == 0)? (EPD_2in13_V4_WIDTH / 8 ): (EPD_2in13_V4_WIDTH / 8 + 1)) * EPD_2in13_V4_HEIGHT;

    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
        return -1;
    }

    // test flashing screen
    Debug("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_2in13_V4_WIDTH, EPD_2in13_V4_HEIGHT, 90, WHITE);
	Paint_Clear(WHITE);

    DEV_Delay_ms(500);

#if 1

    draw_online_status(BlackImage);
    Paint_DrawString_EN(110, 40, "Server On", &Font20, BLACK, WHITE);
    // Paint_DrawNum(110, 80, 123456789, &Font16, BLACK, WHITE);
    Paint_ClearWindows(110, 80, 110 + Font16.Width * 9, 80 + Font16.Height, WHITE);
    Paint_DrawNum(110, 80, 123456789, &Font16, BLACK, WHITE);

    EPD_2in13_V4_Display_Base(BlackImage);
    DEV_Delay_ms(2000);

    // test update number section
    Paint_ClearWindows(110, 80, 110 + Font16.Width * 9, 80 + Font16.Height, WHITE);
    Paint_DrawNum(110, 80, 987654321, &Font16, BLACK, WHITE);
    EPD_2in13_V4_Display_Partial(BlackImage);
    DEV_Delay_ms(2000);


    draw_offline_status(BlackImage);
    Paint_DrawString_EN(110, 40, "Server Off", &Font20, BLACK, WHITE);
    // Paint_DrawNum(110, 80, 123456789, &Font16, BLACK, WHITE);
    Paint_ClearWindows(110, 80, 110 + Font16.Width * 9, 80 + Font16.Height, WHITE);
    Paint_DrawNum(110, 80, 123456789, &Font16, BLACK, WHITE);

    EPD_2in13_V4_Display_Base(BlackImage);
    DEV_Delay_ms(2000);

    // test update number section
    Paint_ClearWindows(110, 80, 110 + Font16.Width * 9, 80 + Font16.Height, WHITE);
    Paint_DrawNum(110, 80, 987654321, &Font16, BLACK, WHITE);
    EPD_2in13_V4_Display_Partial(BlackImage);
    DEV_Delay_ms(2000);

#endif

    Debug("paint new black image then clear...\r\n");
    Paint_NewImage(BlackImage, EPD_2in13_V4_WIDTH, EPD_2in13_V4_HEIGHT, 90, WHITE);
    Paint_Clear(WHITE);


    DEV_Delay_ms(2000);//important, at least 2s


    Debug("Clear...\r\n");
	EPD_2in13_V4_Init();
    EPD_2in13_V4_Clear();
	
    Debug("Goto Sleep...\r\n");
    EPD_2in13_V4_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    Debug("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();

    return 0;

}


//int set_eink_status(int status, datetime_t *t) {
int set_eink_status(ServerStatus *server_status) {

    Debug("Setting Eink Status\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    // create structure for paint function
    PAINT_TIME display_time;

    // display_time.Year = (UWORD)t->year;
    // display_time.Month = (UBYTE)t->month;
    // display_time.Day = (UBYTE)t->day;
    // display_time.Hour = (UBYTE)t->hour;
    // display_time.Min = (UBYTE)t->min;
    // display_time.Sec = (UBYTE)t->sec;

    display_time.Year = (UWORD)server_status->last_state_change.year;
    display_time.Month = (UBYTE)server_status->last_state_change.month;
    display_time.Day = (UBYTE)server_status->last_state_change.day;
    display_time.Hour = (UBYTE)server_status->last_state_change.hour;
    display_time.Min = (UBYTE)server_status->last_state_change.min;
    display_time.Sec = (UBYTE)server_status->last_state_change.sec;

    EPD_2in13_V4_Init_Fast();
    EPD_2in13_V4_Clear(); // white clear

    sleep_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2in13_V4_WIDTH % 8 == 0)? (EPD_2in13_V4_WIDTH / 8 ): (EPD_2in13_V4_WIDTH / 8 + 1)) * EPD_2in13_V4_HEIGHT;

    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
        return -1;
    }

    // test flashing screen
    Debug("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_2in13_V4_WIDTH, EPD_2in13_V4_HEIGHT, 90, WHITE);
	Paint_Clear(WHITE);

    DEV_Delay_ms(500);

    // online case
    int status_y = 10;
    int uptime_title_y = 45;
    int uptime_value_y = 60;
    int updated_title_y = 85;
    int updated_value_y = 100;


    if (server_status->server_online == true) {
        printf("Uptime: %d days, %d hours\n", server_status->uptime_days, server_status->uptime_hours);
        draw_online_status(BlackImage);
        Paint_DrawString_EN(110, status_y, "Server On", &Font20, BLACK, WHITE);

        Paint_ClearWindows(110, uptime_title_y, 110 + Font12.Width * 9, uptime_title_y + Font12.Height, WHITE);
        Paint_DrawString_EN(110, uptime_title_y, "Uptime", &Font12, BLACK, WHITE);

        Paint_ClearWindows(110, uptime_value_y, 110 + Font16.Width * 11, uptime_value_y + Font16.Height, WHITE);
        Paint_DrawUptime(110, uptime_value_y, server_status->uptime_days, server_status->uptime_hours, &Font16, BLACK, WHITE);

        Paint_ClearWindows(110, updated_title_y, 110 + Font12.Width * 9, updated_title_y + Font12.Height, WHITE);
        Paint_DrawString_EN(110, updated_title_y, "Updated", &Font12, BLACK, WHITE);

        Paint_ClearWindows(110, updated_value_y, 110 + Font16.Width * 9, updated_value_y + Font16.Height, WHITE);
        Paint_DrawDatetime(110, updated_value_y, &display_time, &Font16, WHITE, BLACK);

        EPD_2in13_V4_Display_Base(BlackImage);
        DEV_Delay_ms(2000);

        // test update number section
        // Paint_ClearWindows(110, 80, 110 + Font16.Width * 9, 80 + Font16.Height, WHITE);
        // Paint_DrawTime(110, 80, &display_time, &Font16, BLACK, WHITE);
        // // Paint_DrawNum(110, 80, 987654321, &Font16, BLACK, WHITE);
        // EPD_2in13_V4_Display_Partial(BlackImage);
        // DEV_Delay_ms(2000);
    }

    // offline case
    if (server_status->server_online != true) {
        printf("Uptime: %d days, %d hours\n", server_status->uptime_days, server_status->uptime_hours);
        draw_offline_status(BlackImage);
        Paint_DrawString_EN(110, status_y, "Server Off", &Font20, BLACK, WHITE);

        Paint_ClearWindows(110, uptime_title_y, 110 + Font12.Width * 9, uptime_title_y + Font12.Height, WHITE);
        Paint_DrawString_EN(110, uptime_title_y, "Downtime", &Font12, BLACK, WHITE);
        
        Paint_ClearWindows(110, uptime_value_y, 110 + Font16.Width * 11, uptime_value_y + Font16.Height, WHITE);
        Paint_DrawUptime(110, uptime_value_y, server_status->uptime_days, server_status->uptime_hours, &Font16, BLACK, WHITE);

        Paint_ClearWindows(110, updated_title_y, 110 + Font12.Width * 9, updated_title_y + Font12.Height, WHITE);
        Paint_DrawString_EN(110, updated_title_y, "Updated", &Font12, BLACK, WHITE);

        Paint_ClearWindows(110, updated_value_y, 110 + Font16.Width * 9, updated_value_y + Font16.Height, WHITE);
        Paint_DrawDatetime(110, updated_value_y, &display_time, &Font16, WHITE, BLACK);

        EPD_2in13_V4_Display_Base(BlackImage);
        DEV_Delay_ms(2000);

        // test update number section
        // Paint_ClearWindows(110, 80, 110 + Font16.Width * 9, 80 + Font16.Height, WHITE);
        // Paint_DrawTime(110, 80, , &Font16, BLACK, WHITE);
        // // Paint_DrawNum(110, 80, 987654321, &Font16, BLACK, WHITE);
        // EPD_2in13_V4_Display_Partial(BlackImage);
        // DEV_Delay_ms(2000);
    }

    // leave the last image on 
    Debug("Goto Sleep...\r\n");
    EPD_2in13_V4_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    Debug("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();

    return 0;
}

