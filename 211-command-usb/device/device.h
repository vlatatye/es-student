#pragma once
#include <stdint.h>
#define DEVICE_NAME "es-led-module"
#define NAME_215 "es-cmd-usb"
#define FIRMWARE_VERSION "1.0.0"
#define VERSION 0x10000
#define REVISION 2
#define DEVICE_PROJECT "211-command-usb"
#define DEVICE_REPO "https://github.com/vlatatye/es-student"

#ifndef DEVICE_BOARD
#define DEVICE_BOARD "unknown"
#endif

struct info_t
{
    uint32_t version;
    char name[13];
    uint8_t revision;
     
};
//extern struct info_t device_card;
void device_info(void);
void dev_info(void);