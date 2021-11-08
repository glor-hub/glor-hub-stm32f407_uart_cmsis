#ifndef _CLOCK_H_
#define _CLOCK_H_

bool Clock_Init(void);

#ifdef HARDWARE_TESTING_MODE
void Clock_Test(void);
#endif //HARDWARE_TESTING_MODE

#endif //_CLOCK_H_ 
