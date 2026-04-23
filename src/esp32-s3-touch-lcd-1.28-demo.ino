#include <Arduino.h>
#include "LCD_Test.h"
#include "GUI_Paint.h"
#include <math.h>
#include "vert.h"
#include "vert1.h"
#include <stdlib.h>
#include <stdio.h>

UWORD Imagesize = LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH * 2;
UWORD *BlackImage;
CST816S touch(6, 7, 13, 5); // sda, scl, rst, irq
float angle = 0.0; // Ajouter une variable pour stocker l'angle
float alpha_filter = 0.9; // Coefficient du filtre passe-bas

// Include the header files that contain the icons

long count = 0; // Loop count
int f = 0;
int finc = 1;

const int MAX_IMAGES = 40;

void setup() {
    Serial.begin(115200);
    touch.begin();
    // PSRAM Initialize
    if (psramInit()) {
        Serial.println("\nPSRAM is correctly initialized");
    } else {
        Serial.println("PSRAM not available");
    }
    if ((BlackImage = (UWORD *)ps_malloc(Imagesize)) == NULL) {
        Serial.println("Failed to apply for black memory...");
        exit(0);
    }

    // put your setup code here, to run once:
    if (DEV_Module_Init() != 0)
        Serial.println("GPIO Init Fail!");
    else
        Serial.println("GPIO Init successful!");
    
    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);
    /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
    Paint_NewImage((UBYTE *)BlackImage, LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, WHITE);
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE_0);
    Paint_Clear(WHITE);
    
    QMI8658_init();
    Serial.println("QMI8658_init\r\n");
    DEV_SET_PWM(100);
}

void loop() {
   
    uint16_t result;
    float acc[3], gyro[3];
    unsigned int tim_count = 0;
    static unsigned long previous_time = 0;
    unsigned long current_time = millis();
    float dt = (current_time - previous_time) / 1000.0; // Calculate time in seconds
    previous_time = current_time;
     static unsigned long start_time = millis(); // Time when the program starts

    // Read accelerometer and gyroscope values ---------------------------------
    QMI8658_read_xyz(acc, gyro, &tim_count);

    // Calculate angle from gyroscope
    float gyro_z = gyro[2] - 0.5;
    angle += (gyro_z * dt) / 2;

    // Calculate angle from accelerometer
    float accel_angle = atan2(acc[1], -acc[0]) * 180.0 / PI;
    angle = 0.80 * angle + 0.20 * accel_angle;


    if (current_time - start_time < 2000) {
        Paint_Clear(BLACK);
        Paint_DrawCircle(120, 120, 120, CYAN, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        Paint_DrawCircle(120, 120, 110, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        Paint_DrawString_EN(50, 140, "STARTING", &Font24, WHITE, WHITE); // Adjusted Y position
        LCD_1IN28_Display(BlackImage);
        return; // Exit the loop without drawing
    }
    // Display the image based on angle --------------------------------------
    if (angle < -1 && angle >= -40) {
        int imageIndex = -round(angle);
        Paint_DrawBitMap((const unsigned char*)epd_bitmap_allArray[MAX_IMAGES - imageIndex]);
    }

    if (angle > 1 && angle <= 40) {
        int imageIndex = round(angle);
        Paint_DrawBitMap((const unsigned char*)epd_bitmap_allArray1[MAX_IMAGES - imageIndex]);
    }

    if (angle < -50 && angle >= -89) {
        int imageIndex = 90 + round(angle);
        Paint_DrawBitMap((const unsigned char*)epd_bitmap_allArray1[MAX_IMAGES - imageIndex]);
    }

    if (angle > 50 && angle <= 89) {
        int imageIndex = 90 - round(angle);
        Paint_DrawBitMap((const unsigned char*)epd_bitmap_allArray[MAX_IMAGES - imageIndex]);
    }

    if (angle < -91 && angle >= -130) {
        int imageIndex = -round(angle) - 90;
        Paint_DrawBitMap((const unsigned char*)epd_bitmap_allArray[MAX_IMAGES - imageIndex]);
    }

    if (angle > 91 && angle <= 130) {
        int imageIndex = round(angle) - 90;
        Paint_DrawBitMap((const unsigned char*)epd_bitmap_allArray1[MAX_IMAGES - imageIndex]);
    }
    
    Paint_DrawRectangle(80, 150, 180, 190, BLACK, DOT_PIXEL_2X2, DRAW_FILL_FULL);

    // Display the image based on angle 0 --------------------------------------
    if ((angle > -1 && angle < 1) || (angle > 89 && angle < 91) || (angle < -89 && angle > -91)) {
        Paint_DrawCircle(120, 120, 120, GREEN, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        Paint_DrawCircle(120, 120, 105, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    }
    if ((angle < -44 && angle > -46) || (angle > 44 && angle < 46)) {
        Paint_DrawCircle(120, 120, 120, CYAN, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        Paint_DrawCircle(120, 120, 105, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    }

    Paint_DrawNum(110, 110, abs((int)angle), &Font24, 0, WHITE, BLACK);
    Paint_DrawString_EN(80, 140, "DEGRE", &Font24, WHITE, WHITE); // Adjusted Y position


    // Display the final image
    LCD_1IN28_Display(BlackImage);
}