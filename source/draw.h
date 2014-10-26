#ifndef DRAW_H
#define DRAW_H

#define TOP_ASPECT_X 0x5
#define TOP_ASPECT_Y 0x3
#define TOP_HEIGHT 240
#define TOP_WIDTH 400
#define BOTTOM_HEIGHT 240
#define BOTTOM_WIDTH 320

#include <3ds/types.h>
#include <3ds/gfx.h>

void clearScreen(u8* screen,gfxScreen_t screenPos);
void drawPixel(int x, int y, char r, char g, char b, u8* screen);
void drawChar(char letter,int x,int y, char r, char g, char b, u8* screen);
void drawString(char* word, int x,int y, char r, char g, char b, u8* screen,gfxScreen_t screenPos);
void drawLine( int x1, int y1, int x2, int y2, char r, char g, char b, u8* screen);
void drawRect( int x1, int y1, int x2, int y2, char r, char g, char b, u8* screen);
void drawFillRect( int x1, int y1, int x2, int y2, char r, char g, char b, u8* screen);
void drawCircle(int x, int y, int radius, char r, char g, char b, u8* screen);
void drawFillCircle(int x, int y, int radius, char r , char g, char b, u8* screen);
void drawCircleCircum(int xc, int yc, int x, int y, char r, char g, char b, u8* screen);
#endif
