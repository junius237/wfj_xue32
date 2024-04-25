#ifndef __WWDG_H
#define __WWFG_H

#include "./SYSTEM/sys/sys.h"
extern WWDG_HandleTypeDef g_wwdg_handle;
void  wwdg_init(uint8_t tr,uint8_t wr,uint32_t fprer);


#endif

