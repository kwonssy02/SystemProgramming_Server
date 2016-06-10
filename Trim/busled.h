#ifndef _BUSLED_H
#define _BUSLED_H
#define LED_DRIVER_NAME         "/dev/cnled"

void controlBusLed(int fd, int ledNo, int ledControl);

#endif
