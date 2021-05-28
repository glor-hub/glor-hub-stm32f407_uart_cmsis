#ifndef _DISCOVERY_KIT_H_
#define _DISCOVERY_KIT_H_

#define HARDWARE_TESTING_MODE

#define TRUE 1
#define FALSE 0

#define SYS_STA_READY ((uint32_t)0x00000000)
#define SYS_RCC_STA_ERR ((uint32_t)0x00000001)
#define SYS_SYS_TICK_STA_ERR ((uint32_t)0x00000002)

typedef enum {
    ENABLE_CMD,
    DISABLE_CMD,
    NUM_PERIPH_STATES
} ePeriphCmd;

#endif //_DISCOVERY_KIT_H_
