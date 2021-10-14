#ifndef _ASSERT_H_
#define _ASSERT_H_

#define VERBOSE_OUTPUT

void assert_failed(uint8_t *func, uint8_t *file, uint32_t line);

#define ASSERT(expr) expr ? (void)0 : assert_failed((uint8_t *)__FUNCTION__, (uint8_t *)__FILE__, __LINE__)

#ifdef VERBOSE_OUTPUT

#define LOG(VAL) do{printf(VAL);printf(" in function %s, file %s on line %d\r\n", (uint8_t *)__FUNCTION__, (uint8_t *)__FILE__, __LINE__);}while(0)

#else//Not defined VERBOSE_OUTPUT

#define LOG(VAL) do{printf(VAL);printf("!\r\n");}while(0)

#endif//VERBOSE_OUTPUT

#endif //_ASSERT_H_
