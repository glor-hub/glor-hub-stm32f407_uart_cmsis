#ifndef _ASSERT_H_
#define _ASSERT_H_

#ifndef NDEBUG

void assert_failed(uint8_t *func, uint8_t *file, uint32_t line);

#define ASSERT(expr) expr ? (void)0 : assert_failed((uint8_t *)__FUNCTION__, (uint8_t *)__FILE__, __LINE__)

#else

#define ASSERT(expr)

#endif//NDEBUG

#endif //_ASSERT_H_ 
