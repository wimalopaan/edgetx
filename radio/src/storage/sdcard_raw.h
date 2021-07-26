#ifndef _SDCARD_RAW_H_
#define _SDCARD_RAW_H_

#include "FatFs/ff.h"

const char * openFile(const char * fullpath, FIL* file, uint16_t* size);

#if defined(EEPROM_SDCARD)
extern ModelHeader modelHeaders[MAX_MODELS];
#endif

#endif
