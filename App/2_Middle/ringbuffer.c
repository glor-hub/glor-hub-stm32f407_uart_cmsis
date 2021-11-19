//********************************************************************************
//ringbuffer.c
//********************************************************************************
#include "stm32f4xx.h"
#include <stdbool.h>
#include "common.h"
#include "ringbuffer.h"


//********************************************************************************
//Macros
//********************************************************************************


//********************************************************************************
//Enums
//********************************************************************************

//********************************************************************************
//Typedefs
//********************************************************************************


//********************************************************************************
//Variables
//********************************************************************************

//********************************************************************************
//Prototypes
//********************************************************************************

static bool RingBuffer_isEmpty(RingBuffer_t *p_struct);

static bool RingBuffer_isFull(RingBuffer_t *p_struct);

//================================================================================
//Public
//================================================================================

void RingBuffer_Init(RingBuffer_t *p_struct,  uint8_t *p_buffer)
{
    p_struct->in = 0U;
    p_struct->out = 0U;
    p_struct->count = 0U;
    p_struct->p_buff = p_buffer;
}

uint8_t RingBuffer_WriteChar(RingBuffer_t *p_struct,  uint8_t *p_char)
{
    if(RingBuffer_isFull(p_struct)) {
        return RING_BUFF_OVERLOAD_FLAG;
    } else {
        p_struct->p_buff[p_struct->in++] = *p_char;
        p_struct->in %= RING_BUFF_SIZE;
        p_struct->count++;
        return RING_BUFF_OK;
    }
}


uint8_t RingBuffer_ReadChar(RingBuffer_t *p_struct,  uint8_t *p_char)
{
    if(RingBuffer_isEmpty(p_struct)) {
        return RING_BUFF_EMPTY_FLAG;
    } else {
        *p_char = p_struct->p_buff[p_struct->out++];
        p_struct->out %= RING_BUFF_SIZE;
        p_struct->count--;
        return RING_BUFF_OK;
    }
}

uint8_t RingBuffer_GetCount(RingBuffer_t *p_struct)
{
    return p_struct->count;
}

void RingBuffer_Reset(RingBuffer_t *p_struct)
{
    p_struct->out = p_struct->in;
    p_struct->count = 0U;
}

//================================================================================
//Private
//================================================================================

static bool RingBuffer_isEmpty(RingBuffer_t *p_struct)
{
    return p_struct->count == 0U;
}

static bool RingBuffer_isFull(RingBuffer_t *p_struct)
{
    return p_struct->count == RING_BUFF_SIZE;
}


