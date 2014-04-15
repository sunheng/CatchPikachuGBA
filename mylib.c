/*
	Sunheng Taing
*/

#include "mylib.h"

//unsigned short alias u16 is in mylib.h
u16 *videoBuffer = (unsigned short *)0x6000000;

//Prototypes are in mylib.h as well

/*
	Sets a pixel at certain row and col to a color.
*/
void setPixel(int r, int c, u16 color) {
	videoBuffer[OFFSET(r, c, 240)] = color;
}

/*
	VBlank here.
*/	
void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}


void drawPicture(const unsigned short arr[])
{
	DMA[3].src = arr;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (100*240) | DMA_ON;
}

void drawImage3(int r, int c, int width, int height, const 
u16* image) {
	int i;
	for(i = 0; i < height; i++) {
		DMA[3].src = &image[i * width];
		DMA[3].dst = &videoBuffer[(r + i) * 240 + c];
		DMA[3].cnt = (width) | DMA_ON; 
	}
}

void fillScreen(volatile unsigned short color)
{
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (160*240) | DMA_SOURCE_FIXED | DMA_ON;
}

void drawTrailColor(int r, int c, int width, int height, volatile unsigned short color) {
	int i;
	for(i = 0; i < height; i++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[(r + i) * 240 + c];
		DMA[3].cnt = (width) | DMA_SOURCE_FIXED | DMA_ON; 
	}
}

