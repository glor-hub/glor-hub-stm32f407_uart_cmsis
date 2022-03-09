#ifndef _ARM_DMA_H_
#define _ARM_DMA_H_

typedef enum {
    ARM_DMA_STREAM_0 = 0,
    ARM_DMA_STREAM_1,
    ARM_DMA_STREAM_2,
    ARM_DMA_STREAM_3,
    ARM_DMA_STREAM_4,
    ARM_DMA_STREAM_5,
    ARM_DMA_STREAM_6,
    ARM_DMA_STREAM_7,
    NUM_ARM_DMA_STREAM
} eARM_DMA_StreamNumbers;

typedef enum {
    ARM_DMA_CHAN_0 = 0,
    ARM_DMA_CHAN_1,
    ARM_DMA_CHAN_2,
    ARM_DMA_CHAN_3,
    ARM_DMA_CHAN_4,
    ARM_DMA_CHAN_5,
    ARM_DMA_CHAN_6,
    ARM_DMA_CHAN_7,
    NUM_ARM_DMA_CHAN
} eARM_DMA_ChanNumbers;

//DMA modes
typedef enum {
    ARM_DMA_DIRECT_MODE = 0,
    ARM_DMA_FIFO_REGULAR_MODE,
    ARM_DMA_FIFO_CIRCULAR_MODE,
    ARM_DMA_FIFO_DOUBLE_BUFFER_MODE,
    NUM_ARM_DMA_MODE
} eARM_DMA_Modes;

/****** DMA Event *****/

#define ARM_DMA_EVENT_TRANSFER_COMPLETE     (1UL << 0)
#define ARM_DMA_EVENT_TRANSFER_ERROR        (1UL << 1)
#define ARM_DMA_EVENT_DIRECT_MODE_ERROR     (1UL << 2)
#define ARM_DMA_EVENT_FIFO_ERROR            (1UL << 3)

// FIFO size in bytes
#define FIFO_SIZE 16

// Stream priorities
#define ARM_DMA_LOW_PRIORITY        (uint8_t)0x00
#define ARM_DMA_MIDDLE_PRIORITY     (uint8_t)0x01
#define ARM_DMA_HIGH_PRIORITY       (uint8_t)0x02
#define ARM_DMA_VERY_HIGH_PRIORITY  (uint8_t)0x03

// Memory data size MSIZE (increment is done according to MSIZE
// If increment anable)
#define ARM_DMA_1_BYTE_MSIZE    (uint32_t)0x00
#define ARM_DMA_2_BYTE_MSIZE    (uint32_t)0x01
#define ARM_DMA_4_BYTE_MSIZE    (uint32_t)0x02

// Peripheral data size PSIZE (increment is done according to PSIZE
// If increment anable)
#define ARM_DMA_1_BYTE_PSIZE    (uint32_t)0x00
#define ARM_DMA_2_BYTE_PSIZE    (uint32_t)0x01
#define ARM_DMA_4_BYTE_PSIZE    (uint32_t)0x02

// Memory increment:
// Memory address pointer is fixed
#define ARM_DMA_MINC_DISABLE    0UL
// Memory address pointer is incremented
#define ARM_DMA_MINC_ENABLE     1UL

// Peripheral increment:
// Peripheral address pointer is fixed
#define ARM_DMA_PINC_DISABLE    0UL
// Peripheral address pointer is incremented
#define ARM_DMA_PINC_ENABLE     1UL

// Stream tranfer direction
#define ARM_DMA_PERIPHERAL_TO_MEMORY_DIR    (uint32_t)0x00
#define ARM_DMA_MEMORY_TO_PERIPHERAL_DIR    (uint32_t)0x01
#define ARM_DMA_MEMORY_TO_MEMORY_DIR        (uint32_t)0x02

// FIFO threshold
// Used only in FIFO mode
#define ARM_DMA_QUARTER_FIFO_THRESHOLD          (uint32_t)0x00
#define ARM_DMA_THREE_QUARTERS_FIFO_THRESHOLD   (uint32_t)0x02
#define ARM_DMA_FULL_FIFO_THRESHOLD             (uint32_t)0x03

// FIFO transaction type
#define ARM_DMA_FIFO_SINGLE_TRANSACTION             (uint32_t)0x00
#define ARM_DMA_FIFO_BURST_OF_4_BEATS_TRANSACTION   (uint32_t)0x01
#define ARM_DMA_FIFO_BURST_OF_8_BEATS_TRANSACTION   (uint32_t)0x02
#define ARM_DMA_FIFO_BURST_OF_16_BEATS_TRANSACTION  (uint32_t)0x03

typedef void (*ARM_DMA_SignalEvent_t)(uint32_t event);

bool ARM_DMA_Initialize(ePeriphTypes dma_name);
bool ARM_DMA_Uninitialize(ePeriphTypes dma_name);
bool ARM_DMA_StreamConfig(ePeriphTypes dma_name, eARM_DMA_StreamNumbers stream_num,
                          eARM_DMA_ChanNumbers chan_num, uint8_t priority,
                          eARM_DMA_Modes mode, uint32_t periph_addr,
                          uint32_t mem0_addr, uint32_t mem1_addr,
                          uint32_t periph_size, uint32_t mem_size,
                          uint32_t periph_incr_ena, uint32_t mem_incr_ena,
                          uint32_t data_size, uint32_t dir, uint32_t fifo_thresh,
                          uint32_t fifo_mem_trans_type, uint32_t fifo_periph_trans_type,
                          ARM_DMA_SignalEvent_t  cb_event);
bool ARM_DMA_StreamEnable(ePeriphTypes dma_name, eARM_DMA_StreamNumbers stream_num);
void ARM_DMA_StreamDisable(ePeriphTypes dma_name, eARM_DMA_StreamNumbers stream_num);
#endif //_ARM_DMA_H_ 
