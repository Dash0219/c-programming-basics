#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "graphics.h"

#define WINDOW_SIZE 650
#define MAX_ITERATIONS 3000

// #define BOUND  0.5
// #define X_POS  -0.525
// #define Y_POS 0.2

//  default settings
#define BOUND 1.5
#define X_POS -0.625
#define Y_POS 0

int* hslToRgb(double h){
    int *r = malloc(3 * sizeof(int));
    int *g = r + 1;
    int *b = r + 2;
    int x = floor(255 * (1 - fabs(fmod((h / 60), 2) - 1)));

    if (h < 60){
        *r = 255;
        *g = x;
        *b = 0;
    }
    else if (h < 120){       
        *r = x;
        *g = 255;
        *b = 0;
    }
    else if (h < 180){
        *r = 0;
        *g = 255;
        *b = x;
    }
    else if (h < 240){
        *r = 0;
        *g = x;
        *b = 255;
    }
    else if (h < 300){
        *r = x;
        *g = 0;
        *b = 255;
    }
    else{
        *r = 255;
        *g = 0;
        *b = x;
    }
    return r;
}

void colorPixel(int x, int y, int n){
    double brightness;
    double hue = (1 - cbrt((double) n / MAX_ITERATIONS)) * 360;
    int *r = hslToRgb(hue);
    setRGBColour(*r, *(r + 1), *(r + 2));
    fillRect(x, y, 1, 1);
    free(r);
    return;
}

void draw(){
    setWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    foreground();

    for (int x = 0; x < WINDOW_SIZE; x++){
        for (int y = 0; y < WINDOW_SIZE; y++){
            double baseA = (double) x * 2 * BOUND / WINDOW_SIZE - BOUND + X_POS;
            double baseB = (double) y * 2 * BOUND / WINDOW_SIZE - BOUND + Y_POS;
            double newA = baseA;
            double newB = baseB;
            for (int n = 0; n < MAX_ITERATIONS; n++){
                if (newA * newA + newB * newB > 4){
                    colorPixel(x, y, n);
                    break;
                }
                double tempNewA = newA * newA - newB * newB + baseA;
                newB = 2 * newA * newB + baseB;
                newA = tempNewA;
            }
            if (newA * newA + newB * newB <= 4){
                setColour(black);
                fillRect(x, y, 1, 1);
            }
        }
    }
}

int main(){
    draw();
    return 0;
}