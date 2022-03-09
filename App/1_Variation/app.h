#ifndef _APP_H_
#define _APP_H_

#define _APP_DEBUG_

#define USE_RING_BUFFER

#ifndef USE_RING_BUFFER
#define USE_DMA
#endif //USE_RING_BUFFER

bool App_Init(void);
void App_IdleTask(void);
void App_Test(void);

#endif //_APP_H_ 
