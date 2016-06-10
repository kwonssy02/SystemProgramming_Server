#ifndef _TOUCHPAD_H_
#define _TOUCHPAD_H_

/*(
typedef struct Button{
        int x;
        int y;
        int width;
        int height;
}BUTTON;

int bntCLick(int btnClick(BUTTON btn, int x, int y);I

*/
void readFirstCoordinate(int fd, int* cx , int* cy);
void drawCoordinate(unsigned char *fb_mem , int cx , int cy, int prex , int prey);
int touchPad();


#endif
