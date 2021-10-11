#ifndef _COMMON_H_
#define _COMMON_H_

#define TRUE 1
#define FALSE 0

#define PASSED 0
#define FAILED 1

//#define NDEBUG
#define HARDWARE_TESTING_MODE

typedef enum {
    ENABLE_CMD = 0,
    DISABLE_CMD,
    NUM_PERIPH_STATES
} ePeriphCmd;

#endif //_COMMON_H_
