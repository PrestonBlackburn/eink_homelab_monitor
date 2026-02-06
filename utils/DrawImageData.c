#include "DrawImageData.h"
#include "EPD_2in13_V4.h"

void draw_online_status(UBYTE *image) {
    // Test Drawing
    // new image 122w x 250h in size, rotate 90, "white" color
	Paint_NewImage(image, EPD_2in13_V4_WIDTH, EPD_2in13_V4_HEIGHT, 90, WHITE);  	
    Debug("Drawing\r\n");
    //1.Select Image
    EPD_2in13_V4_Init();
    Paint_SelectImage(image);
    Paint_Clear(WHITE);

    // draw point: (X point, Y point)
    // draw line: (Xstart, Ystart) to (Xend, Yend)
    // draw rectange: (Xstart, Ystart) to (Xend, Yend)
    // draw circle: (X_Center, Y_Center) and a radius of Radius
    // draw string:  (Xstart Ystart)
    // draw num: (Xstart Ystart)
    // draw time: (Xstart, Ystart) as the left-top corner

    // outline of the icon
    int status_img_start_x = 28;
    int status_img_start_y = 33;
    int status_img_outline_end_x = 89;
    int status_img_outline_end_y = 89;
    DOT_PIXEL status_img_width = DOT_PIXEL_4X4;
    LINE_STYLE stats_img_rectangle_style = DRAW_FILL_EMPTY;

    // eyes of icon
    DOT_PIXEL eyes_point = DOT_PIXEL_8X8;
    DOT_STYLE eye_dot_style = DOT_STYLE_DFT;

    int eye_1_img_start_x = 46;
    int eye_1_img_start_y = 47;

    int eye_2_img_start_x = 71;
    int eye_2_img_start_y = 47;

    // icon mouth
    DOT_PIXEL mouth_point = DOT_PIXEL_4X4;
    LINE_STYLE mouth_style = LINE_STYLE_SOLID;
    int mouth_img_start_x = 45;
    int mouth_img_start_y = 73;
    int mouth_img_end_x = 73;
    int mouth_img_end_y = 73;
    // mouth corners for smile
    DOT_PIXEL mouth_corner_point = DOT_PIXEL_4X4;
    DOT_STYLE mouth_corner_dot_style = DOT_STYLE_DFT;
    int mouth_corner_1_start_x = 42;
    int mouth_corner_1_start_y = 69;
    int mouth_corner_2_start_x = 77;
    int mouth_corner_2_start_y = 69;



    // test image draw speed
    Paint_DrawRectangle(
        status_img_start_x, 
        status_img_start_y, 
        status_img_outline_end_x, 
        status_img_outline_end_y, 
        BLACK, 
        status_img_width, 
        stats_img_rectangle_style
    );
    // eyes
    Paint_DrawPoint(
        eye_1_img_start_x, 
        eye_1_img_start_y, 
        BLACK, 
        eyes_point, 
        eye_dot_style
    );
    Paint_DrawPoint(
        eye_2_img_start_x, 
        eye_2_img_start_y, 
        BLACK, 
        eyes_point, 
        eye_dot_style
    );
    // mouth
    Paint_DrawLine(
        mouth_img_start_x, 
        mouth_img_start_y, 
        mouth_img_end_x, 
        mouth_img_end_y, 
        BLACK, 
        mouth_point, 
        mouth_style
    );
    Paint_DrawPoint(
        mouth_corner_1_start_x, 
        mouth_corner_1_start_y, 
        BLACK, 
        mouth_corner_point, 
        mouth_corner_dot_style
    );
    Paint_DrawPoint(
        mouth_corner_2_start_x, 
        mouth_corner_2_start_y, 
        BLACK, 
        mouth_corner_point, 
        mouth_corner_dot_style
    );

};


void draw_offline_status(UBYTE *image) {
    // Test Drawing
    // new image 122w x 250h in size, rotate 90, "white" color
	Paint_NewImage(image, EPD_2in13_V4_WIDTH, EPD_2in13_V4_HEIGHT, 90, WHITE);  	
    Debug("Drawing\r\n");
    //1.Select Image
    EPD_2in13_V4_Init();
    Paint_SelectImage(image);
    Paint_Clear(WHITE);

    // draw point: (X point, Y point)
    // draw line: (Xstart, Ystart) to (Xend, Yend)
    // draw rectange: (Xstart, Ystart) to (Xend, Yend)
    // draw circle: (X_Center, Y_Center) and a radius of Radius
    // draw string:  (Xstart Ystart)
    // draw num: (Xstart Ystart)
    // draw time: (Xstart, Ystart) as the left-top corner

    // outline of the icon
    int status_img_start_x = 28;
    int status_img_start_y = 33;
    int status_img_outline_end_x = 89;
    int status_img_outline_end_y = 89;
    DOT_PIXEL status_img_width = DOT_PIXEL_4X4;
    LINE_STYLE stats_img_rectangle_style = DRAW_FILL_EMPTY;

    // eyes of icon
    DOT_PIXEL eyes_point = DOT_PIXEL_4X4;
    DOT_STYLE eye_dot_style = DOT_STYLE_DFT;

    int eye_1_img_start_x = 46;
    int eye_1_img_start_y = 47;

    int eye_2_img_start_x = 71;
    int eye_2_img_start_y = 47;

    // icon mouth
    DOT_PIXEL mouth_point = DOT_PIXEL_4X4;
    LINE_STYLE mouth_style = LINE_STYLE_SOLID;
    int mouth_img_start_x = 45;
    int mouth_img_start_y = 73;
    int mouth_img_end_x = 73;
    int mouth_img_end_y = 73;
    // mouth corners for smile
    DOT_PIXEL mouth_corner_point = DOT_PIXEL_4X4;
    DOT_STYLE mouth_corner_dot_style = DOT_STYLE_DFT;
    int mouth_corner_1_start_x = 42;
    int mouth_corner_1_start_y = 77;
    int mouth_corner_2_start_x = 77;
    int mouth_corner_2_start_y = 77;



    // test image draw speed
    Paint_DrawRectangle(
        status_img_start_x, 
        status_img_start_y, 
        status_img_outline_end_x, 
        status_img_outline_end_y, 
        BLACK, 
        status_img_width, 
        stats_img_rectangle_style
    );
    // eyes
    // --- eye 1 -----
    // center: 46, 47
    Paint_DrawPoint(
        eye_1_img_start_x, 
        eye_1_img_start_y, 
        BLACK, 
        eyes_point, 
        eye_dot_style
    );
    int OFFSET = 4;
    Paint_DrawPoint(eye_1_img_start_x-OFFSET, eye_1_img_start_y+OFFSET, BLACK, eyes_point, eye_dot_style);
    Paint_DrawPoint(eye_1_img_start_x+OFFSET, eye_1_img_start_y+OFFSET, BLACK, eyes_point, eye_dot_style);
    Paint_DrawPoint(eye_1_img_start_x-OFFSET, eye_1_img_start_y-OFFSET, BLACK, eyes_point, eye_dot_style);
    Paint_DrawPoint(eye_1_img_start_x+OFFSET, eye_1_img_start_y-OFFSET, BLACK, eyes_point, eye_dot_style);

    // ----- eye 2 -----
    // center: 71, 47
    Paint_DrawPoint(
        eye_2_img_start_x, 
        eye_2_img_start_y, 
        BLACK, 
        eyes_point, 
        eye_dot_style
    );
    Paint_DrawPoint(eye_2_img_start_x-OFFSET, eye_2_img_start_y+OFFSET, BLACK, eyes_point, eye_dot_style);
    Paint_DrawPoint(eye_2_img_start_x+OFFSET, eye_2_img_start_y+OFFSET, BLACK, eyes_point, eye_dot_style);
    Paint_DrawPoint(eye_2_img_start_x-OFFSET, eye_2_img_start_y-OFFSET, BLACK, eyes_point, eye_dot_style);
    Paint_DrawPoint(eye_2_img_start_x+OFFSET, eye_2_img_start_y-OFFSET, BLACK, eyes_point, eye_dot_style);


    // mouth
    Paint_DrawLine(
        mouth_img_start_x, 
        mouth_img_start_y, 
        mouth_img_end_x, 
        mouth_img_end_y, 
        BLACK, 
        mouth_point, 
        mouth_style
    );
    Paint_DrawPoint(
        mouth_corner_1_start_x, 
        mouth_corner_1_start_y, 
        BLACK, 
        mouth_corner_point, 
        mouth_corner_dot_style
    );
    Paint_DrawPoint(
        mouth_corner_2_start_x, 
        mouth_corner_2_start_y, 
        BLACK, 
        mouth_corner_point, 
        mouth_corner_dot_style
    );


};