#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

//the size of the buffer must be equal to a power of two
#define RING_BUFF_SIZE 256

#define RING_BUFF_OK 0U
#define RING_BUFF_OVERLOAD_FLAG (1U << 0)
#define RING_BUFF_EMPTY_FLAG (1U << 1)

typedef struct {
    uint8_t in;
    uint8_t out;
    uint8_t count;
    uint8_t *p_buff;
} RingBuffer_t;

void RingBuffer_Init(RingBuffer_t *p_struct,  uint8_t *p_buffer);
uint8_t RingBuffer_WriteChar(RingBuffer_t *p_struct,  uint8_t *p_char);
uint8_t RingBuffer_ReadChar(RingBuffer_t *p_struct,  uint8_t *p_char);
uint8_t RingBuffer_GetCount(RingBuffer_t *p_struct);
void RingBuffer_Reset(RingBuffer_t *p_struct);

#endif //_RINGBUFFER_H_ 
